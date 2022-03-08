import os
import subprocess
import sys
import random
import datetime

exePath = "Fuzzer.exe"
seedPath = "Z:\\SHARE\\SEED.txt"

def makeSeed():
    random.seed()
    seed = str(random.randint(0, 2147483647))
    return seed

def runFuzz():
    seed = makeSeed()
    run = subprocess.Popen([exePath, seed]).wait()
    i = 0
    while True:
        if i == 2:
            break
        if run != 0:
            run = subprocess.Popen([exePath, seed]).wait()
        else:
            run = subprocess.Popen([exePath, seed]).wait()
        i += 1

if __name__ == '__main__':
    ctime = str(datetime.datetime.now())
    a = open(seedPath, 'a')
    a.write("[FZStart - " + str(ctime) + "]\n")
    a.close()
    while True:
        runFuzz()







