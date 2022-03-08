from parse import *
import frida
import sys, os

def on_message(message, data):
    print("[%s] => %s" % (message,data))


def hook(code, target):
	pid = frida.spawn(target)
	session = frida.attach(pid)

	script = session.create_script(code)
	script.on('message',on_message)
	script.load()

	frida.resume(pid)
	sys.stdin.read()
	session.detach()


def make_script(APIs,output):
	script = ''
	script += 'var logFile = new File("input/%s","wb");\n' % (output)

	for API in list(APIs.values()):
		script += 'try{\nconst %s = Module.findExportByName("%s","%s");\n' % (API.funcName, API.dllName, API.funcName)

		script += 'Interceptor.attach(%s,{\nonEnter: function(args){\nvar log = "%s:"\n' % (API.funcName, API.funcName)	
		for i in range(len(API.params) - 1):
			if API.params[i]['type'][-3:] == 'STR':
				script += 'log += args[%d].readAnsiString() + ", ";\n' % i 
			else:
				script += 'log += args[%d].toString() + ", ";\n' % i

		script += '\nlogFile.write(log + "\\n");\n},'
		script += 'onLeave: function(retVal){\nlogFile.write(retVal.toString() + "\\n");\n'
		script += '}});}\ncatch(err){}\n\n\n'

	return script

if __name__ == '__main__':
	if len(sys.argv) != 3:
		print("usage : python3 hook.py [output] [target]")
		sys.exit(-1)

	APIs = parse_header()
	code = make_script(APIs, sys.argv[1])
	hook(code, sys.argv[2])
	#print(code)




