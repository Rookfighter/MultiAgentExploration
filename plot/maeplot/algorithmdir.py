import os
import shutil
from experimentdir import ExperimentDirectory
from datafile import DataFile
from utils import mkdirRec
from utils import getSubdirectoriesConcat
from plotdata import plotCoverageEventsPerCount

from meandata import MeanCoverageEvents
from meandata import MeanFinalCoverage
from meandata import MeanTimeEvents
from meandata import MeanGridTimeBetweenVisits
from meandata import MeanGridVisits
from meandata import MeanTileTimeBetweenVisits
from meandata import MeanTileVisits

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
        
        self.worldTypes_ = set()
        
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
            worldKey = experimentDir.worldType_
            robotKey = experimentDir.robotCount_
            
            self.worldTypes_.add(experimentDir.worldType_)
            
            if not worldKey in self.meanCoverageEvents_:
                self.meanCoverageEvents_[worldKey] = dict()
                self.meanTimeEvents_[worldKey] = dict()
                self.meanTileTimeBetweenVisits_[worldKey] = dict()
                self.meanTileVisits_[worldKey] = dict()
                self.meanGridTimeBetweenVisits_[worldKey] = dict()
                self.meanGridVisits_[worldKey] = dict()
                self.meanFinalCoverage_[worldKey] = dict()
            
            if not robotKey in self.meanCoverageEvents_[worldKey]:
                self.meanCoverageEvents_[worldKey][robotKey] = MeanCoverageEvents()
                self.meanTimeEvents_[worldKey][robotKey] = MeanTimeEvents()
                self.meanTileTimeBetweenVisits_[worldKey][robotKey] = MeanTileTimeBetweenVisits()
                self.meanTileVisits_[worldKey][robotKey] = MeanTileVisits()
                self.meanGridTimeBetweenVisits_[worldKey][robotKey] = MeanGridTimeBetweenVisits()
                self.meanGridVisits_[worldKey][robotKey] = MeanGridVisits()
                self.meanFinalCoverage_[worldKey][robotKey] = MeanFinalCoverage()
                
            self.meanCoverageEvents_[worldKey][robotKey].add(experimentDir.coverageEvents_)
            self.meanTimeEvents_[worldKey][robotKey].add(experimentDir.timeEvents_)
            self.meanTileTimeBetweenVisits_[worldKey][robotKey].add(experimentDir.meanTileTimeBetweenVisits_)
            self.meanTileVisits_[worldKey][robotKey].add(experimentDir.tileVisits_)
            self.meanGridTimeBetweenVisits_[worldKey][robotKey].add(experimentDir.meanGridTimeBetweenVisits_)
            self.meanGridVisits_[worldKey][robotKey].add(experimentDir.meanGridVisits_)
            self.meanFinalCoverage_[worldKey][robotKey].add(experimentDir.finalCoverage_)
  
    def save(self):
        print "saving summary of '" + self.getName() + "'..."
        
        summaryDir = self.getSummaryDir()
        if os.path.isdir(summaryDir):
            shutil.rmtree(summaryDir, True, False)
        mkdirRec(summaryDir)
        dataFile = DataFile()
        
        filename = ""
        for worldType in self.worldTypes_:
            print "-- saving data of '" + worldType + "'"
            worldDir = os.path.join(summaryDir,worldType)
            mkdirRec(worldDir)
            
            for robotCount in self.meanCoverageEvents_[worldType]:
                
                filename = self.getSaveFilename(worldDir, COVERAGE_EVENTS_FILE, robotCount)
                dataFile.reset()
                dataFile.addComment("# [coverage meanTimestamp(usec)]")
                data = self.meanCoverageEvents_[worldType][robotCount].getMean()
                dataFile.setDataAs(data, "fl")
                dataFile.save(filename)
                
                filename = self.getSaveFilename(worldDir, TIME_EVENTS_FILE, robotCount)
                dataFile.reset()
                dataFile.addComment("# [coverage meanTimestamp(usec)]")
                data = self.meanTimeEvents_[worldType][robotCount].getMean()
                dataFile.setDataAs(data, "fl")
                dataFile.save(filename)
                
                filename = self.getSaveFilename(worldDir, MEAN_TILE_TIME_BEWTEEN_VISITS_FILE, robotCount)
                dataFile.reset()
                dataFile.addComment("# [x y meanTimeBetweenVisits(usec)]")
                data = self.meanTileTimeBetweenVisits_[worldType][robotCount].getMean()
                dataFile.setDataAs(data, "iil")
                dataFile.save(filename)
                
                filename = self.getSaveFilename(worldDir, TILE_VISITS_FILE, robotCount)
                dataFile.reset()
                dataFile.addComment("# [x y meanVisitCount]")
                data = self.meanTileVisits_[worldType][robotCount].getMean()
                dataFile.setDataAs(data, "iif")
                dataFile.save(filename)
                
                filename = self.getSaveFilename(worldDir, MEAN_GRID_TIME_BEWTEEN_VISITS_FILE, robotCount)
                dataFile.reset()
                dataFile.addComment("# [meanTimeBetweenVisits(usec)]")
                data = self.meanGridTimeBetweenVisits_[worldType][robotCount].getMean()
                dataFile.setDataAs(data, "l")
                dataFile.save(filename)
                
                filename = self.getSaveFilename(worldDir, MEAN_GRID_VISITS_FILE, robotCount)
                dataFile.reset()
                dataFile.addComment("# [meanVisitCount]")
                data = self.meanGridVisits_[worldType][robotCount].getMean()
                dataFile.setDataAs(data, "f")
                dataFile.save(filename)
                
                filename = self.getSaveFilename(worldDir, FINAL_COVERAGE_FILE, robotCount)
                dataFile.reset()
                dataFile.addComment("# [coverage timeStamp(usec)]")
                data = self.meanFinalCoverage_[worldType][robotCount].getMean()
                dataFile.setDataAs(data, "fl")
                dataFile.save(filename)
        
    def plot(self):
        print "plotting summary data of '" + self.getName() + "'";
        
        summaryDir = self.getSummaryDir()
        for worldType in self.worldTypes_:
            worldDir = os.path.join(summaryDir,worldType)
            plotFile = os.path.join(worldDir, COVERAGE_EVENTS_PER_COUNT)
            
            plotCoverageEventsPerCount(self.meanCoverageEvents_[worldType], plotFile)
            
    def getSaveFilename(self, worldDir, filename, robotCount):
        return os.path.join(worldDir, str(robotCount) + "-" + filename)    
    
    def getName(self):
        return os.path.basename(os.path.normpath(self.directory_))
    
    def getSummaryDir(self):
        return os.path.join(self.directory_, SUMMARY_DIRECTORY)