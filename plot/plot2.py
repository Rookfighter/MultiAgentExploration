import os
import sys
from maeplot.algorithmdir import AlgorithmDirectory
from maeplot.utils import getImmediateSubdirectories

# main
originDir = os.getcwd()
baseDir = os.path.dirname(os.path.realpath(sys.argv[0]))
algorithmDirs = getImmediateSubdirectories(baseDir)

print ""
print "Plot MultiAgentExploration v1.2"
print "-------------------------------"
print "cd to '" + baseDir + "'"
print ""
os.chdir(baseDir)

for algoDir in algorithmDirs:
    
    algorithmDirectory = AlgorithmDirectory()
    algorithmDirectory.load(algoDir)
        
print ""
print "return to '" + originDir + "'"
os.chdir(originDir)
