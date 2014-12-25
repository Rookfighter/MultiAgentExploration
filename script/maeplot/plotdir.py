import os
import shutil
from maeplot.algorithmdir import AlgorithmDirectory
from maeplot.utils import getSubdirectoriesConcat, mkdirRec
from maeplot.plotdata import plotCoverageReachedAfterTime, plotTimeToReachCoverage,\
    plotNumberOfVisits, plotTimeBetweenVisits
from maeplot.experiment import AVAILABLE_WORLDS, AVAILABLE_ALGORITHMS
from maeplot.meandata import MeanVisits, MeanCoverageEvents, MeanTimeEvents,\
    MeanTimeBetweenVisits

SUMMARY_DIRECTORY = "summary"

class PlotDirectory(object):
    
    def __init__(self):
        self.reset()
    
    def reset(self):
        self.directory_ = ""
        self.algorithmDirs_ = []
        
        self.timeToReachCoverage_ = dict()
        self.coverageReachedAfterTime_ = dict()
        for algorithm in AVAILABLE_ALGORITHMS:
            self.timeToReachCoverage_[algorithm] = dict()
            self.coverageReachedAfterTime_[algorithm] = dict()
            for worldType in AVAILABLE_WORLDS:
                self.timeToReachCoverage_[algorithm][worldType] = MeanCoverageEvents()
                self.coverageReachedAfterTime_[algorithm][worldType] = MeanTimeEvents()
                
        self.meanNumberOfVisits_ = dict()
        self.meanTimeBetweenVisits_ = dict()
        for worldType in AVAILABLE_WORLDS:
            self.meanNumberOfVisits_[worldType] = dict()
            self.meanTimeBetweenVisits_[worldType] = dict()
            for algorithm in AVAILABLE_ALGORITHMS:
                self.meanNumberOfVisits_[worldType][algorithm] = MeanVisits()
                self.meanTimeBetweenVisits_[worldType][algorithm] = MeanTimeBetweenVisits()
        
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
            self.updateNumberOfVisits()
            self.updateTimeBetweenVisits()
     
    def updateTimeToReachCoverage(self):
        algorithmName = self.algorithmDirs_[-1].getName()
        
        for worldType, robotCountDict in self.algorithmDirs_[-1].meanCoverageEvents_.iteritems(): 
            for coverageEvents in  robotCountDict.values():
                if coverageEvents.hasData():
                    self.timeToReachCoverage_[algorithmName][worldType].addMean(coverageEvents)
    
    def updateCoverageReachedAfterTime(self):
        algorithmName = self.algorithmDirs_[-1].getName()
        
        for worldType, robotCountDict in self.algorithmDirs_[-1].meanTimeEvents_.iteritems():
            for timeEvents in  robotCountDict.values():
                if timeEvents.hasData():
                    self.coverageReachedAfterTime_[algorithmName][worldType].addMean(timeEvents)
    
    def updateNumberOfVisits(self):
        algorithmName = self.algorithmDirs_[-1].getName()
        
        for worldType, robotCountDict in self.algorithmDirs_[-1].meanTileVisits_.iteritems(): 
            for meanVisits in robotCountDict.values():
                if meanVisits.hasData():
                    self.meanNumberOfVisits_[worldType][algorithmName].addMean(meanVisits)
                    
    def updateTimeBetweenVisits(self):
        algorithmName = self.algorithmDirs_[-1].getName()
        
        for worldType, robotCountDict in self.algorithmDirs_[-1].meanTileTimeBetweenVisits_.iteritems(): 
            for meanTime in robotCountDict.values():
                if meanTime.hasData():
                    self.meanTimeBetweenVisits_[worldType][algorithmName].addMean(meanTime)
        
    def save(self):
        
        for algoDir in self.algorithmDirs_:
            algoDir.save()
    
    def plot(self):
        if os.path.isdir(self.getSummaryDir()):
            shutil.rmtree(self.getSummaryDir())
        mkdirRec(self.getSummaryDir())
        
        print "plotting time to reach coverage"
        plotTimeToReachCoverage(self.timeToReachCoverage_, self.getSummaryDir(), 0.90)
        
        print "plotting coverage reached after time"
        plotCoverageReachedAfterTime(self.coverageReachedAfterTime_, self.getSummaryDir(), 60)
        
        print "plotting number of visits"
        plotNumberOfVisits(self.meanNumberOfVisits_, self.getSummaryDir())
        
        print "plotting time between visits"
        plotTimeBetweenVisits(self.meanTimeBetweenVisits_, self.getSummaryDir())
        
        for algoDir in self.algorithmDirs_:
            algoDir.plot()
            
    def getSummaryDir(self):
        return os.path.join(self.directory_, SUMMARY_DIRECTORY)