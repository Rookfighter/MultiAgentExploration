import os
import shutil
from experimentdir import ExperimentDirectory
from datafile import DataFile
from utils import mkdirRec
from utils import getSubdirectoriesConcat
from plotdata import plotCoverageEventsPerCount

from meandata import MeanFinalCoverage
from meandata import MeanCoverageEvents
from meandata import MeanGridTimeBetweenVisits
from meandata import MeanGridVisits
from meandata import MeanTileTimeBetweenVisits
from meandata import MeanTileVisits
from meandata import MeanTimeEvents
from experiment import AVAILABLE_ROBOT_COUNTS
from experiment import AVAILABLE_WORLDS

SUMMARY_DIRECTORY = "summary"

# data filenames
TILE_VISITS_FILE = "tile-visits.dat"
MEAN_GRID_VISITS_FILE = "mean-grid-visits.dat"
MEAN_TILE_TIME_BEWTEEN_VISITS_FILE = "mean-tile-time-between-visits.dat"
MEAN_GRID_TIME_BEWTEEN_VISITS_FILE = "mean-grid-time-between-visits.dat"
COVERAGE_EVENTS_FILE = "coverage-events.dat"
TIME_EVENTS_FILE = "time-events.dat"
FINAL_COVERAGE_FILE = "final-coverage.dat"

# plot file names
COVERAGE_EVENTS_PER_COUNT = "coverage-events-per-robots.png"

class AlgorithmDirectory:
    
    def __init__(self):
        self.reset()
    
    def reset(self):
        self.directory_ = ""
        self.meanCoverageEvents_ = dict()
        self.meanTimeEvents_ = dict()
        self.meanTileTimeBetweenVisits_ = dict()
        self.meanTileVisits_ = dict()
        self.meanGridTimeBetweenVisits_ = dict()
        self.meanGridVisits_ = dict()
        self.meanFinalCoverage_ = dict()
        
        for worldType in AVAILABLE_WORLDS:
            self.meanCoverageEvents_[worldType] = dict()
            self.meanTimeEvents_[worldType] = dict()
            self.meanTileTimeBetweenVisits_[worldType] = dict()
            self.meanTileVisits_[worldType] = dict()
            self.meanGridTimeBetweenVisits_[worldType] = dict()
            self.meanGridVisits_[worldType] = dict()
            self.meanFinalCoverage_[worldType] = dict()
            
            for robotCount in AVAILABLE_ROBOT_COUNTS:
                self.meanCoverageEvents_[worldType][robotCount] = MeanCoverageEvents()
                self.meanTimeEvents_[worldType][robotCount] = MeanTimeEvents()
                self.meanTileTimeBetweenVisits_[worldType][robotCount] = MeanTileTimeBetweenVisits()
                self.meanTileVisits_[worldType][robotCount] = MeanTileVisits()
                self.meanGridTimeBetweenVisits_[worldType][robotCount] = MeanGridTimeBetweenVisits()
                self.meanGridVisits_[worldType][robotCount] = MeanGridVisits()
                self.meanFinalCoverage_[worldType][robotCount] = MeanFinalCoverage()
        
    def load(self, directory):
        assert(os.path.isdir(directory))
        self.reset()
        self.directory_ = directory
        
        print "loading '" + self.getName() + "'..."
        
        subDirectories = getSubdirectoriesConcat(self.directory_)
        experimentDir = ExperimentDirectory()
        
        for subDir in subDirectories:
            if os.path.basename(subDir) == SUMMARY_DIRECTORY:
                continue
            
            experimentDir.load(subDir)
            worldType = experimentDir.worldType_
            robotCount = experimentDir.robotCount_
            
            assert(worldType in AVAILABLE_WORLDS)
            assert(robotCount in AVAILABLE_ROBOT_COUNTS)
            
            self.meanCoverageEvents_[worldType][robotCount].add(experimentDir.coverageEvents_)
            self.meanTimeEvents_[worldType][robotCount].add(experimentDir.timeEvents_)
            self.meanTileTimeBetweenVisits_[worldType][robotCount].add(experimentDir.meanTileTimeBetweenVisits_)
            self.meanTileVisits_[worldType][robotCount].add(experimentDir.tileVisits_)
            self.meanGridTimeBetweenVisits_[worldType][robotCount].add(experimentDir.meanGridTimeBetweenVisits_)
            self.meanGridVisits_[worldType][robotCount].add(experimentDir.meanGridVisits_)
            self.meanFinalCoverage_[worldType][robotCount].add(experimentDir.finalCoverage_)
  
    def save(self):
        print "saving summary of '" + self.getName() + "'..."
        
        summaryDir = self.getSummaryDir()
        if os.path.isdir(summaryDir):
            shutil.rmtree(summaryDir, True, False)
        mkdirRec(summaryDir)
        dataFile = DataFile()
        
        filename = ""
        for worldType in AVAILABLE_WORLDS:
            print "-- saving data of '" + worldType + "'"
            worldDir = os.path.join(summaryDir,worldType)
            mkdirRec(worldDir)
            
            for robotCount in AVAILABLE_ROBOT_COUNTS:
                
                if self.meanCoverageEvents_[worldType][robotCount].hasData():
                    filename = self.getSaveFilename(worldDir, COVERAGE_EVENTS_FILE, robotCount)
                    dataFile.reset()
                    dataFile.addComment("# [coverage meanTimestamp(usec)]")
                    data = self.meanCoverageEvents_[worldType][robotCount].getMean()
                    dataFile.setDataAs(data, "fl")
                    dataFile.save(filename)
                
                if self.meanTimeEvents_[worldType][robotCount].hasData():
                    filename = self.getSaveFilename(worldDir, TIME_EVENTS_FILE, robotCount)
                    dataFile.reset()
                    dataFile.addComment("# [coverage meanTimestamp(usec)]")
                    data = self.meanTimeEvents_[worldType][robotCount].getMean()
                    dataFile.setDataAs(data, "fl")
                    dataFile.save(filename)
                
                if self.meanTileTimeBetweenVisits_[worldType][robotCount].hasData():
                    filename = self.getSaveFilename(worldDir, MEAN_TILE_TIME_BEWTEEN_VISITS_FILE, robotCount)
                    dataFile.reset()
                    dataFile.addComment("# [x y meanTimeBetweenVisits(usec)]")
                    data = self.meanTileTimeBetweenVisits_[worldType][robotCount].getMean()
                    dataFile.setDataAs(data, "iil")
                    dataFile.save(filename)
                
                if self.meanTileVisits_[worldType][robotCount].hasData():
                    filename = self.getSaveFilename(worldDir, TILE_VISITS_FILE, robotCount)
                    dataFile.reset()
                    dataFile.addComment("# [x y meanVisitCount]")
                    data = self.meanTileVisits_[worldType][robotCount].getMean()
                    dataFile.setDataAs(data, "iif")
                    dataFile.save(filename)
                
                if self.meanGridTimeBetweenVisits_[worldType][robotCount].hasData():
                    filename = self.getSaveFilename(worldDir, MEAN_GRID_TIME_BEWTEEN_VISITS_FILE, robotCount)
                    dataFile.reset()
                    dataFile.addComment("# [meanTimeBetweenVisits(usec)]")
                    data = self.meanGridTimeBetweenVisits_[worldType][robotCount].getMean()
                    dataFile.setDataAs(data, "l")
                    dataFile.save(filename)
                
                if self.meanGridVisits_[worldType][robotCount].hasData():
                    filename = self.getSaveFilename(worldDir, MEAN_GRID_VISITS_FILE, robotCount)
                    dataFile.reset()
                    dataFile.addComment("# [meanVisitCount]")
                    data = self.meanGridVisits_[worldType][robotCount].getMean()
                    dataFile.setDataAs(data, "f")
                    dataFile.save(filename)
                
                if self.meanFinalCoverage_[worldType][robotCount].hasData():
                    filename = self.getSaveFilename(worldDir, FINAL_COVERAGE_FILE, robotCount)
                    dataFile.reset()
                    dataFile.addComment("# [coverage timeStamp(usec)]")
                    data = self.meanFinalCoverage_[worldType][robotCount].getMean()
                    dataFile.setDataAs(data, "fl")
                    dataFile.save(filename)
        
    def plot(self):
        print "plotting summary data of '" + self.getName() + "'";
        
        summaryDir = self.getSummaryDir()
        for worldType in AVAILABLE_WORLDS:
            worldDir = os.path.join(summaryDir,worldType)
            plotFile = os.path.join(worldDir, COVERAGE_EVENTS_PER_COUNT)
            
            plotCoverageEventsPerCount(self.meanCoverageEvents_[worldType], plotFile)
            
    def getSaveFilename(self, worldDir, filename, robotCount):
        return os.path.join(worldDir, str(robotCount) + "-" + filename)    
    
    def getName(self):
        return os.path.basename(os.path.normpath(self.directory_))
    
    def getSummaryDir(self):
        return os.path.join(self.directory_, SUMMARY_DIRECTORY)