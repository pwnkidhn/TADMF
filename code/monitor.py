import os
import sys
import time
import subprocess
import datetime

from os import chdir

#Need to change
dirPath = "C:\\Program Files (x86)\\VMware\\VMware VIX"
vmxPath = "C:\\Users\\H-2\\Desktop\\Fuzzer2\\Windows 10 x64 (2).vmx"
vmIP = "192.168.18.129"

sharePath = "D:\\Fuzzer\\Fuzzer2"

#Need not to change
shareName = "SHARE"
seedPath = str(sharePath + "\\\\SEED.txt")
resultPath = str(sharePath + "\\\\RESULT.txt")
FZPath = "C:\\Users\\HooN\\Desktop\\start.bat"

chdir(dirPath)

def vmStart():
    os.system('vmrun.exe start \"' + vmxPath + '\" gui')
    print ('VM start')

def vmReset():
    os.system('vmrun.exe reset \"' + vmxPath + '\" hard')
    print ('VM reset')

def setShare():
    os.system('vmrun.exe enableSharedFolders \"' + vmxPath + '\"')
    print('enableShare')
    os.system('vmrun.exe addSharedFolder \"' + vmxPath + '\" ' + shareName + ' \"' + sharePath + '\"')
    print('addShare')

def runFuzzer():
    os.system('vmrun.exe -T ws -gu HooN -gp 1234 runProgramInGuest \"' + vmxPath + '\" -Interactive \"' + FZPath + '\"')
    print('Fuzzer Run')

def monitor():
    mtime = os.path.getmtime(seedPath)
    ctime = datetime.datetime.now()
    cHour = datetime.datetime.now().hour
    cMin = datetime.datetime.now().minute
    mHour = datetime.datetime.fromtimestamp(mtime).hour
    mMin = datetime.datetime.fromtimestamp(mtime).minute

    if mMin < 10:
        mMin = '0' + str(mMin)

    if cMin < 10:
        cMin = '0' + str(cMin)

    current = int(str(cHour) + str(cMin))
    modified = int(str(mHour) + str(mMin))

    alive = True if os.system("ping -n 1 " + vmIP) is 0 else False

    if alive:
        print("[Time Check] Current : " + str(current))
        print("[Time Check] Modiefied : " + str(modified))
        if current > modified + 1:
            print("[Time Out]")
            f = open(seedPath, "a")
            print ("[Process Hang - " + str(ctime) + "]\n")
            f.write("[Process Hang - " + str(ctime) + "]\n")
            f.close()
            processKill()
            runFuzzer()
    else:
        f = open(seedPath, "a")
        print ("[Kernel Panic - " + str(ctime) + "]\n")
        f.write("[Kernel Panic - " + str(ctime) + "]\n")
        f.close()
        vmReset()
        checkLogin()

def checkLogin():
    while True:
        f = open(seedPath, 'r', encoding='UTF8')
        comp = f.readlines()[-1]
        if 'VMLogIn' in comp:
            break
        f.close()
    runFuzzer()

def processKill():
    cmd = ['vmrun.exe', '-T', 'ws', '-gu', 'HooN', '-gp', '1234', 'listProcessesInGuest', vmxPath]
    data = subprocess.check_output(cmd)
    data_str = str(data).replace('\\n', '\n')
    data_list = data_str.split('\n')
    del data_list[-1]
    del data_list[0]
    #print(data_list)

    for i in range(len(data_list)):
        if 'run.exe' in data_list[i]:
            data_list[i] = data_list[i].replace('pid=','')
            data_list[i] = data_list[i].replace(',','')
            data_list[i] = data_list[i].split()
            os.system('vmrun.exe -T ws -gu HooN -gp 1234 killProcessInGuest \"' + vmxPath + '\" ' + str(data_list[i][0]))
            print('Kill Process')

if __name__=='__main__':

    vmStart()
    setShare()
    checkLogin()
    while True:
        monitor()
        time.sleep(3)
