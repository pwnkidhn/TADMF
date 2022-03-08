import datetime

seedPath = "Z:\\SHARE\\SEED.txt"

if __name__=='__main__':
    ctime = str(datetime.datetime.now())
    a = open(seedPath, 'a')
    a.write("[VMLogIn - " + str(ctime) + "]\n")
    a.close()
