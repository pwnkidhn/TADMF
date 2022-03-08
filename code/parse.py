import Api

def extract_struct_data():
    
    header_data = []
    struct_data = []
    struct_tmp_data = []
    struct_flag = 0
    count = 0

    with open("header/wingdi.h","r") as file:
        header_data = file.readlines()
        
    for line in header_data:
        skip_char = ("#","//","/*")
        
        #Skip annotation
        if line.startswith(skip_char):
            continue
        
        #parse struct_data 
        elif line.startswith("typedef struct"):
            if '{' in line:
                count += 1
            struct_tmp_data.append([line])
            struct_flag = 1
            continue
          
        #data
        if struct_flag == 1:
            
            #skip union
            if "union" in line:
                count = 0
                struct_flag = 0
                struct_tmp_data = []
                continue
            
            if '{' in line:
                count += 1

            if '}' in line:
                count -= 1
                if count == 0:
                    struct_tmp_data.append([line])
                    struct_data.append(struct_tmp_data)
                    struct_tmp_data = []
                    struct_flag = 0
                    continue
                        
            struct_tmp_data.append([line])

    extract_struct = []
    data_type = ""

    # extract_struct_data
    for line in struct_data:
        for idx in line:
            if "typedef" in idx[0]:
                continue
            if ';' not in idx[0] and '\n' in idx[0] and '}' not in idx[0]:
                continue
            
            if "}" in idx[0]:
                str_line = idx[0].replace('}'," ")
                str_line = str_line.replace(','," ")
                str_line = str_line.replace(';'," ")
                str_line = str_line.strip()
                str_list = str_line.split(" ")
                data_type = str_list[0]+ "|"+ data_type[:-1]
                extract_struct.append(data_type)
                data_type = ""
            else:
                pointer_flag = 0
                str_line = idx[0].strip()
                str_list = str_line.split(" ")
                for tmp in str_list:
                    if '*' in tmp and ';' in tmp:
                        pointer_flag = 1
                #print(str_list)
                if pointer_flag == 1:
                    data_type += str_list[0] + " *,"
                    pointer_flag = 0
                else:
                    data_type += str_list[0] +","
    
    extract_struct.sort()
    with open("input/s_information.csv","w") as file:
        file.write("\n".join(extract_struct))



# edit 1,2 
def extract_api_data():
    
    header_data = []
    api_data = []
    api_tmp_data = ""
    api_flag = 0

    with open("header/wingdi.h","r") as file:
        header_data = file.readlines()
        
    for line in header_data:
        skip_char = ("#","//","/*")
        
        #Skip annotation
        if line.startswith(skip_char):
            continue
        #parse struct_data 
        elif "WINGDIAPI" in line:
            if ';' in line and api_flag == 0:
                api_data.append(' '.join(line.strip().split()))
                continue
            
            if ';' not in line:
                api_flag = 1

        if ';' in line and api_flag == 1 :
            api_tmp_data += line.strip()
            api_data.append(' '.join(api_tmp_data.split()))
            api_tmp_data = ""
            api_flag = 0
            continue

        if api_flag == 1:
            api_tmp_data += " "+line.strip()
            api_tmp_data = api_tmp_data.strip()

    api = []
    for line in api_data:
        idx = line.index('(')
        tmp = line[:idx+1] + line[idx+1:].lstrip()        
        api.append(tmp)

    with open("input/header.txt","w") as file:
        file.write("\n".join(api))
    

def set_structs():
    
    with open("input/s_information.csv","rt",encoding='UTF-8') as file:
        struct_data = file.readlines()
    
    struct = {}
    for line in struct_data:
        name = line.split('|')[0]
        args = line.replace('\n','').split('|')[1].split(',')
        struct[name] = args
    
    Api.API.struct = struct
    #print(Api.API.struct)

def parse_header():
    
    extract_api_data()
    extract_struct_data()
    set_structs()
    
    with open("input/header.txt","r") as file:
        lines = file.readlines()
    
    #print (lines)
    
    APIs = {}
    for line in lines:
        retType = line.split(' ')[1]
        funcName = line.split('(')[0].split(' ')[3]
        dllName = line.split(' ')[0]
        API = {}
                
        temp = line[line.index('(') + 1 : ][:-3].split(',')
        flag = 0
        param = ''
        params = []
        
        for tmp in temp:
            flag += tmp.count('(')
            flag -= tmp.count(')')
            if flag != 0:
                param += tmp + ','
            
            elif flag == 0:
                param += tmp
                params.append(param)
                param = ''
                
        paramTypes = []
        paramNames = []
        paramSAL = []
        
        
        for param in params:
            try:
                if param.split(' ')[-2] == '*':
                    paramTypes.append(param.split(' ')[-3])
                    p_str = '*'+param.split(' ')[-1]
                    #print(p_str)
                    paramNames.append(p_str)
                    paramSAL.append(' '.join(param.split(' ')[:-2]))
                else:
                    paramTypes.append(param.split(' ')[-2])
                    paramNames.append(param.split(' ')[-1])
                    paramSAL.append(' '.join(param.split(' ')[:-2]))
            except:
                pass
        APIs[funcName] = Api.API(funcName, retType, paramNames, paramTypes, paramSAL, dllName)
        
    return APIs
    
if __name__ == "__main__":
    APIs = parse_header()
    for API in list(APIs.values()):
        print(API.funcName)
        print(API.params)
            
   
    