import os
import sys
from maeplot.plotdir import PlotDirectory
from maeplot.utils import printHelp

# main
originDir = os.getcwd()
baseDir = os.path.dirname(os.path.realpath(sys.argv[0]))

if len(sys.argv) < 2:
    printHelp()
    exit(0)

targetDir = os.path.abspath(sys.argv[1])

print ""
print "Plot MultiAgentExploration 2nd Edition v1.0"
print "-------------------------------------------"

print "cd to '" + targetDir + "'"
print ""
os.chdir(targetDir)

plotDir = PlotDirectory()
plotDir.load(targetDir)
plotDir.save()
plotDir.plot()

print ""
print "return to '" + originDir + "'"
os.chdir(originDir)
print "-------------------------------------------" 
print "Finished: successful"
