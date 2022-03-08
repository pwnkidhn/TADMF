import copy

class API:
	struct = {}
	structNames = {}
	def __init__(self, funcName, retType, paramNames, paramTypes, paramSAL, dllName):
		self.funcName = funcName
		self.retType = retType
		self.params = []

		if dllName == 'WINGDIAPI':
			self.dllName = 'GDI32.dll'
		elif dllName == 'WINUSERAPI':
			self.dllName = 'User32.dll'
		elif dllName == 'WINSPOOLAPI':
			self.dllName = 'WinSpool.drv'

		for i in range(len(paramNames)):
			tmp = {}
			
			tmp['name'] = paramNames[i].replace('*','')
			tmp['type'] = paramTypes[i]

			tmp['property'] = {}

			if '**' in paramNames[i]:
				tmp['property']['isDPTR'] = True
			elif '*' in paramNames[i]:
				tmp['property']['isPTR'] = True
    
			if self.structNames.get(paramTypes[i]):
				tmp['property']['struct'] = self.structNames[paramTypes[i]]
			elif self.struct.get(paramTypes[i]):
				tmp['property']['struct'] = paramTypes[i]

			elif "LPPOINT" in tmp['type']:
				tmp['property']['struct'] = "POINT"
    
			elif "DEVMODEA" in tmp['type']:
				tmp['property']['struct'] = "DEVMODEA"
			elif "LPSIZE" in tmp['type']:
				tmp['property']['struct'] = "SIZE"  
    
			if 'Inout' in paramSAL[i]:
				tmp['property']['IO'] = 'IO'
			elif 'In' in paramSAL[i]:
				tmp['property']['IO'] = 'I'
			elif 'Out' in paramSAL[i]:
				tmp['property']['IO'] = 'O' 
			else:
				tmp['property']['IO'] = ''

			self.params.append(tmp)

		self.params.append({'name':'ret', 'type':retType, 'property':{'IO':'O'}})
	
class Log(API):
	def __init__(self, API, values):
		self.funcName = API.funcName
		self.retType = API.retType
		self.params = copy.deepcopy(API.params)

		for i in range(len(values)):
			self.params[i]['value'] = values[i]

	def makeCode(self, cnt):
		apiCall = "%s %s = %s(" %(self.retType, str(cnt) + '_ret', self.funcName)

		for i in range(len(self.params) -1):
			if self.params[i]['property'].get('dep'):
				apiCall += '%s, '%(self.params[i]['property']['dep'])
			else:
				apiCall += '%s, '%(self.params[i]['value'])

		apiCall = apiCall[:-2] + ');'
		return apiCall
