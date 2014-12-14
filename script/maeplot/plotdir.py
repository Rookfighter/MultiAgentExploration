import os
import shutil
from algorithmdir import AlgorithmDirectory
from utils import getSubdirectoriesConcat
from utils import mkdirRec
from plotdata import plotCoverageReachedAfterTime
from plotdata import plotTimeToReachCoverage
from meandata import MeanCoverageEvents
from meandata import MeanTimeEvents
from experiment import AVAILABLE_WORLDS
from experiment import AVAILABLE_ALGORITHMS

SUMMARY_DIRECTORY = "summary"

COVERAGE_REACHED_AFTER_TIME_FILE = "coverage-reached-after-time-{0}.png"
TIME_TO_REACH_COVERAGE_FILE = "time-to-reach-coverage-{0}.png"

class PlotDirectory(object):
    
    def __init__(self):
        self.reset()
    
    def reset(self):
        self.directory_ = ""
        self.algorithmDirs_ = []
        
        self.timeToReachCoverage_ = dict()
        self.coverageReachedAfterTime = dict()
        for algorithm in AVAILABLE_ALGORITHMS:
            self.timeToReachCoverage_[algorithm] = dict()
            self.coverageReachedAfterTime[algorithm] = dict()
            for worldType in AVAILABLE_WORLDS:
                self.timeToReachCoverage_[algorithm][worldType] = MeanCoverageEvents()
                self.coverageReachedAfterTime[algorithm][worldType] = MeanTimeEvents()
        
    def load(self, directory):
        assert(os.path.isdir(directory))
        
        self.reset()
        self.directory_ = directory
        
        algorithmDirs = getSubdirectoriesConcat(self.directory_)
        print "found " + str(len(algorithmDirs)) + " algorithm data directories"
        
        for algoDir in algorithmDirs:
            if os.path.basename(algoDir) == SUMMARY_DIRECTORY:
                continue
            
            self.algorithmDirs_.append(AlgorithmDirectory())
            self.algorithmDirs_[-1].load(algoDir)
            
            self.updateTimeToReachCoverage()
            self.updateCoverageReachedAfterTime()
            
     
    def updateTimeToReachCoverage(self):
        algorithmName = self.algorithmDirs_[-1].getName()
        
        for worldType, robotCountDict in self.algorithmDirs_[-1].meanCoverageEvents_.iteritems(): 
            for coverageEvents in  robotCountDict.values():
                if coverageEvents.hasData():
                    self.timeToReachCoverage_[algorithmName][worldType].add(coverageEvents.getMean())
    
    def updateCoverageReachedAfterTime(self):
        algorithmName = self.algorithmDirs_[-1].getName()
        
        for worldType, robotCountDict in self.algorithmDirs_[-1].meanTimeEvents_.iteritems():
            for timeEvents in  robotCountDict.values():
                if timeEvents.hasData():
                    self.coverageReachedAfterTime[algorithmName][worldType].add(timeEvents.getMean())
              
    def save(self):
        
        for algoDir in self.algorithmDirs_:
            algoDir.save()
    
    def plot(self):
        if os.path.isdir(self.getSummaryDir()):
            shutil.rmtree(self.getSummaryDir())
        mkdirRec(self.getSummaryDir())
        
        print "plotting time to reach coverage"
        filename = os.path.join(self.getSummaryDir(), TIME_TO_REACH_COVERAGE_FILE)
        plotTimeToReachCoverage(self.timeToReachCoverage_, filename, 0.85)
        
        print "plotting coverage reached after time"
        filename = os.path.join(self.getSummaryDir(), COVERAGE_REACHED_AFTER_TIME_FILE)
        plotCoverageReachedAfterTime(self.coverageReachedAfterTime, filename, 60)
        
        for algoDir in self.algorithmDirs_:
            algoDir.plot()
            
    def getSummaryDir(self):
        return os.path.join(self.directory_, SUMMARY_DIRECTORY)