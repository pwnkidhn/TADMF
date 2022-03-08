from parse import *
import Api
import Model

def make_log(lines):
    APIs = parse_header()
    
    Logs = []
    flag = 0
    
    funcName = ""
    values = []
    count = 0
    
    for i in range(len(lines)):
        
        if flag == 0:
            funcName = lines[i].split(':')[0]
            values = lines[i].split(':')[1].split(', ')[:-1]
            flag += 1
            continue
        
        if ':' in lines[i]:
            flag += 1
        else:
            flag -= 1
        
        if flag == 0:
            retVal = lines[i].replace('\n', '')
            values.append(retVal)
            Log = Api.Log(APIs[funcName],values)
            values = []
            Logs.append(Log)
            count += 1
            if count == 5000:
                break
            
    return Logs


def filter(i):
    
    with open('input/Log'+str(i*2)+'.txt',"r",encoding='UTF-8') as f1, open('input/Log'+str(i*2+1)+'.txt',"r",encoding='UTF-8') as f2:
        Logs1 = make_log(f1.readlines())
        Logs2 = make_log(f2.readlines())
    
    for i in range(len(Logs1)):
        for j in range(len(Logs1[i].params)):
            try:
                if Logs1[i].params[j]['value'] == Logs2[i].params[j]['value']:
                    Logs1[i].params[j]['property']['const'] = True
                    continue
                else:
                    Logs1[i].params[j]['property']['const'] = False
                    
            except:
                Logs1[i].params[j]['property']['const'] = False
                continue

    return Logs1


def load_log(i):
    Logs = filter(i)
    return Logs

def check_IO(param):
    if param['property']['const']:
        return ''
    return param['property']['IO']


def find_dependency(depModel, log_list):
    i = 0
    dep_num = 0
    length = len(log_list)
    rev_list = log_list[::-1]
    
    for log in log_list:
        i += 1
        p_i = 0
        node = Model.Node(log)
        for param in log.params:
            if param['property']['const'] == True:
                node.params[p_i].isConst = True
                node.params[p_i].constValue.append(param['value'])
            
            if 'I' in check_IO(param):
                for ancestor in rev_list[length - i:]:
                    a_i = 0
                    for adult_param in ancestor.params:
                        if 'O' in check_IO(adult_param):
                            if(adult_param['value'] == param['value']):
                                if ancestor.funcName != node.funcName:
                                    node.set_dependency(p_i,ancestor.funcName, (a_i+1)%len(ancestor.params)-1)
                        a_i += 1
            p_i += 1
        depModel.append_node(node)
        dep_num += 1
    print(str(dep_num) + " of new dependencies had been found!")
    
    
def model(depModel, log_lists): #log_lists -> list of log_list
    for log_list in log_lists:
        find_dependency(depModel, log_list)
        depModel.print()
        print('---------------------------------------------------------')
        


if __name__ == "__main__":
    
    depModel = Model.Model()
    log_list = load_log(0)
    
    find_dependency(depModel,log_list)
    depModel.print()
    
    
    