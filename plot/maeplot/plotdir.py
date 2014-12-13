import os
from algorithmdir import AlgorithmDirectory
from utils import getSubdirectoriesConcat
from utils import mkdirRec
from meandata import MeanFinalCoverage
from plotdata import plotFinalCoverageTimes
from plotdata import plotFinalCoverage

SUMMARY_DIRECTORY = "summary"
PACKAGE_DIRECTORY = "maeplot"

FINAL_COVERAGE_PLOT_FILE = "coverage-reached-after-time.png"
FINAL_COVERAGE_TIME_PLOT_FILE = "time-to-reach-coverage.png"

class PlotDirectory(object):
    
    def __init__(self):
        self.reset()
    
    def reset(self):
        self.directory_ = ""
        self.algorithmDirs_ = []
        self.meanFinalCoverage_ = dict() # per algorithm per terrain
        
    def load(self, directory):
        assert(os.path.isdir(directory))
        
        self.reset()
        self.directory_ = directory
        
        algorithmDirs = getSubdirectoriesConcat(self.directory_)
        
        for algoDir in algorithmDirs:
            if os.path.basename(algoDir) == SUMMARY_DIRECTORY or os.path.basename(algoDir) == PACKAGE_DIRECTORY:
                continue
            
            self.algorithmDirs_.append(AlgorithmDirectory())
            self.algorithmDirs_[-1].load(algoDir)
            
            algorithmName = self.algorithmDirs_[-1].getName()
            if not algorithmName in self.meanFinalCoverage_:
                    self.meanFinalCoverage_[algorithmName] = dict()
            
            for worldType in self.algorithmDirs_[-1].worldTypes_:
                if not worldType in self.meanFinalCoverage_[algorithmName]:
                    self.meanFinalCoverage_[algorithmName][worldType] = MeanFinalCoverage()
                for finalCoverage in  self.algorithmDirs_[-1].meanFinalCoverage_[worldType].values():
                    self.meanFinalCoverage_[algorithmName][worldType].add(finalCoverage.getMean())
            
    def save(self):
        
        for algoDir in self.algorithmDirs_:
            algoDir.save()
    
    def plot(self):
        mkdirRec(self.getSummaryDir())
        
        print "plotting final coverage"
        filename = os.path.join(self.getSummaryDir(), FINAL_COVERAGE_PLOT_FILE)
        plotFinalCoverage(self.meanFinalCoverage_, filename)
        
        print "plotting final coverage times"
        filename = os.path.join(self.getSummaryDir(),FINAL_COVERAGE_TIME_PLOT_FILE)
        plotFinalCoverageTimes(self.meanFinalCoverage_, filename)
        
        for algoDir in self.algorithmDirs_:
            algoDir.plot()
            
    def getSummaryDir(self):
        return os.path.join(self.directory_, SUMMARY_DIRECTORY)