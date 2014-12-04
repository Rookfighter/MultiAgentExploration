import os
import sys
from maeplot.plotdir import PlotDirectory

# main
originDir = os.getcwd()
baseDir = os.path.dirname(os.path.realpath(sys.argv[0]))

print ""
print "Plot MultiAgentExploration 2nd Edition v1.0"
print "-------------------------------------------"

print "cd to '" + baseDir + "'"
print ""
os.chdir(baseDir)

plotDir = PlotDirectory()
plotDir.load(baseDir)
plotDir.save()
plotDir.plot()

print ""
print "return to '" + originDir + "'"
os.chdir(originDir)
print "-------------------------------------------" 
print "Finished: successful"
