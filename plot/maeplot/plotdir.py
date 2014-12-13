import os
from algorithmdir import AlgorithmDirectory
from utils import getSubdirectoriesConcat
from utils import mkdirRec
from plotdata import plotCoverageReachedAfterTime
from plotdata import plotTimeToReachCoverage
from meandata import MeanTimeEvents
from maeplot.meandata import MeanCoverageEvents

SUMMARY_DIRECTORY = "summary"
PACKAGE_DIRECTORY = "maeplot"

COVERAGE_REACHED_AFTER_TIME_FILE = "coverage-reached-after-time-{0}.png"
TIME_TO_REACH_COVERAGE_FILE = "time-to-reach-coverage-{0}.png"

class PlotDirectory(object):
    
    def __init__(self):
        self.reset()
    
    def reset(self):
        self.directory_ = ""
        self.algorithmDirs_ = []
        self.timeToReachCoverage_ = dict() # per algorithm per terrain
        self.coverageReachedAfterTime = dict() # per algorithm per terrain
        
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
            
            self.updateTimeToReachCoverage()
            self.updateCoverageReachedAfterTime()
            
     
    def updateTimeToReachCoverage(self):
        algorithmName = self.algorithmDirs_[-1].getName()
        if not algorithmName in self.timeToReachCoverage_:
                self.timeToReachCoverage_[algorithmName] = dict()
        
        for worldType in self.algorithmDirs_[-1].worldTypes_:
            if not worldType in self.timeToReachCoverage_[algorithmName]:
                self.timeToReachCoverage_[algorithmName][worldType] = MeanTimeEvents()
                
            for timeEvents in  self.algorithmDirs_[-1].meanTimeEvents_[worldType].values():
                self.timeToReachCoverage_[algorithmName][worldType].add(timeEvents.getMean())
    
    def updateCoverageReachedAfterTime(self):
        algorithmName = self.algorithmDirs_[-1].getName()
        if not algorithmName in self.coverageReachedAfterTime:
                self.coverageReachedAfterTime[algorithmName] = dict()
        
        for worldType in self.algorithmDirs_[-1].worldTypes_:
            if not worldType in self.coverageReachedAfterTime[algorithmName]:
                self.coverageReachedAfterTime[algorithmName][worldType] = MeanCoverageEvents()
                
            for coverageEvents in  self.algorithmDirs_[-1].meanCoverageEvents_[worldType].values():
                self.coverageReachedAfterTime[algorithmName][worldType].add(coverageEvents.getMean())
              
    def save(self):
        
        for algoDir in self.algorithmDirs_:
            algoDir.save()
    
    def plot(self):
        mkdirRec(self.getSummaryDir())
        
        print "plotting time to reach coverage"
        filename = os.path.join(self.getSummaryDir(), TIME_TO_REACH_COVERAGE_FILE)
        plotTimeToReachCoverage(self.timeToReachCoverage_, filename)
        
        print "plotting coverage reached after time"
        filename = os.path.join(self.getSummaryDir(), COVERAGE_REACHED_AFTER_TIME_FILE)
        plotCoverageReachedAfterTime(self.coverageReachedAfterTime, filename)
        
        for algoDir in self.algorithmDirs_:
            algoDir.plot()
            
    def getSummaryDir(self):
        return os.path.join(self.directory_, SUMMARY_DIRECTORY)