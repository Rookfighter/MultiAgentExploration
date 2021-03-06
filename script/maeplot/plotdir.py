import os
import shutil
from maeplot.algorithmdir import AlgorithmDirectory
from maeplot.utils import getSubdirectoriesConcat, mkdirRec
from maeplot.plotdata import plotCoverageReachedAfterTime, plotTimeToReachCoverage,\
    plotNumberOfVisits, plotTimeBetweenVisits,\
    plotTimeToReachCoveragePerRobotCount,\
    plotCoverageReachedAfterTimePerRobotCount
from maeplot.experiment import AVAILABLE_WORLDS, AVAILABLE_ALGORITHMS,\
    AVAILABLE_ROBOT_COUNTS
from maeplot.meandata import MeanCoverageEvents, MeanTimeEvents,\
    MeanVisitsEvents, MeanTimeBetweenVisitsEvents

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
                
        self.meanNumberOfVisitsEvents_ = dict()
        self.meanTimeBetweenVisitsEvents_ = dict()
        for worldType in AVAILABLE_WORLDS:
            self.meanNumberOfVisitsEvents_[worldType] = dict()
            self.meanTimeBetweenVisitsEvents_[worldType] = dict()
            for algorithm in AVAILABLE_ALGORITHMS:
                self.meanNumberOfVisitsEvents_[worldType][algorithm] = MeanVisitsEvents()
                self.meanTimeBetweenVisitsEvents_[worldType][algorithm] = MeanTimeBetweenVisitsEvents()
                
        self.timeToReachCoveragePerRobotCount_ = dict()
        self.coverageReachedAfterTimePerRobotCount_ = dict()
        for robotCount in AVAILABLE_ROBOT_COUNTS:
            self.timeToReachCoveragePerRobotCount_[robotCount] = dict()
            self.coverageReachedAfterTimePerRobotCount_[robotCount] = dict()
            for algorithm in AVAILABLE_ALGORITHMS:
                self.timeToReachCoveragePerRobotCount_[robotCount][algorithm] = MeanCoverageEvents()
                self.coverageReachedAfterTimePerRobotCount_[robotCount][algorithm] = MeanTimeEvents()
        
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
            
            self.updateNumberOfVisitsEvents()
            self.updateTimeBetweenVisitsEvents()
            
            self.updateTimeToReachCoveragePerRobotCount()
            self.updateCoverageReachedAfterTimePerRobotCount()
     
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
    
    def updateNumberOfVisitsEvents(self):
        algorithmName = self.algorithmDirs_[-1].getName()
        
        for worldType, robotCountDict in self.algorithmDirs_[-1].meanVisitsEvents_.iteritems(): 
            for meanVisits in robotCountDict.values():
                if meanVisits.hasData():
                    self.meanNumberOfVisitsEvents_[worldType][algorithmName].addMean(meanVisits)
                    
    def updateTimeBetweenVisitsEvents(self):
        algorithmName = self.algorithmDirs_[-1].getName()
        
        for worldType, robotCountDict in self.algorithmDirs_[-1].meanTimeBetweenVisitsEvents_.iteritems(): 
            for meanTime in robotCountDict.values():
                if meanTime.hasData():
                    self.meanTimeBetweenVisitsEvents_[worldType][algorithmName].addMean(meanTime)
    
    def updateTimeToReachCoveragePerRobotCount(self):
        algorithmName = self.algorithmDirs_[-1].getName()
        
        for robotCountDict in self.algorithmDirs_[-1].meanCoverageEvents_.values(): 
            for robotCount, coverageEvents in  robotCountDict.iteritems():
                if coverageEvents.hasData():
                    self.timeToReachCoveragePerRobotCount_[robotCount][algorithmName].addMean(coverageEvents)
        
    def updateCoverageReachedAfterTimePerRobotCount(self):
        algorithmName = self.algorithmDirs_[-1].getName()
        
        for robotCountDict in self.algorithmDirs_[-1].meanTimeEvents_.values():
            for robotCount, timeEvents in  robotCountDict.iteritems():
                if timeEvents.hasData():
                    self.coverageReachedAfterTimePerRobotCount_[robotCount][algorithmName].addMean(timeEvents)
        
    def save(self):
        
        for algoDir in self.algorithmDirs_:
            algoDir.save()
    
    def plot(self):
        if os.path.isdir(self.getSummaryDir()):
            shutil.rmtree(self.getSummaryDir())
        mkdirRec(self.getSummaryDir())
        
        coverage = 0.85
        minutes = 15
        
        print "plotting time to reach coverage"
        plotTimeToReachCoverage(self.timeToReachCoverage_, self.getSummaryDir(), coverage)
        
        print "plotting coverage reached after time"
        plotCoverageReachedAfterTime(self.coverageReachedAfterTime_, self.getSummaryDir(), minutes)
        
        print "plotting number of visits"
        plotNumberOfVisits(self.meanNumberOfVisitsEvents_, self.getSummaryDir(), coverage)
        
        print "plotting time between visits"
        plotTimeBetweenVisits(self.meanTimeBetweenVisitsEvents_, self.getSummaryDir(), coverage)
        
        print "plotting time to reach coverage per robot count"
        plotTimeToReachCoveragePerRobotCount(self.timeToReachCoveragePerRobotCount_, self.getSummaryDir(), coverage)
        
        print "plotting  coverage reached after time per robot count"
        plotCoverageReachedAfterTimePerRobotCount(self.coverageReachedAfterTimePerRobotCount_, self.getSummaryDir(), minutes)
        
        for algoDir in self.algorithmDirs_:
            algoDir.plot()
            
    def getSummaryDir(self):
        return os.path.join(self.directory_, SUMMARY_DIRECTORY)