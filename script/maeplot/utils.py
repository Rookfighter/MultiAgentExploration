import os
import errno
import math

def sameFloat(a, b, eps):
    return abs(a - b) <= eps;

def mkdirRec(directory):
    try:
        os.makedirs(directory)
    except OSError as exc: 
        if exc.errno == errno.EEXIST and os.path.isdir(directory):
            pass

def getImmediateSubdirectories(p_dir):
    return [name for name in os.listdir(p_dir)
            if os.path.isdir(os.path.join(p_dir, name))]
            
def getSubdirectoriesConcat(p_dir):
    subDirs = getImmediateSubdirectories(p_dir)
    for i in xrange(0, len(subDirs)):
        subDirs[i] = os.path.join(p_dir, subDirs[i])
    return subDirs

def printHelp():
    print "Usage: python plot.py <data directory>"
    
def calcStandardDeviation(data, meanValue):
    assert(len(data) > 0)
    
    diffSum = 0.0
    count = float(len(data))
    for value in data:
        diffVal = float(value - meanValue)
        diffSum = diffSum + (diffVal * diffVal)
        
    return math.sqrt(diffSum / count) 
        
    