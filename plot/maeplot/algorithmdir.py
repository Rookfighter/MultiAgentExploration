import os
from experimentdir import ExperimentDirectory
from datafile import DataFile
from utils import mkdirRec
from utils import getSubdirectoriesConcat

SUMMARY_DIRECTORY = "summary"

TILE_VISITS_FILE = "tile-visits.dat"
MEAN_GRID_VISITS_FILE = "mean-grid-visits.dat"
MEAN_TILE_TIME_BEWTEEN_VISITS_FILE = "mean-tile-time-between-visits.dat"
MEAN_GRID_TIME_BEWTEEN_VISITS_FILE = "mean-grid-time-between-visits.dat"
COVERAGE_EVENTS_FILE = "coverage-events.dat"
FINAL_COVERAGE_FILE = "final-coverage.dat"

class MeanCoverageEvents:
    
    # [coverage, timeStamp, count]
    coverageEventsData_ = [[],[],[]]
    
    def reset(self):
        self.coverageEventsData_ = [[],[],[]]
    
    def add(self, coverageEvents):
        assert(len(coverageEvents) == 2)
        
        for coverage in coverageEvents[0]:
            if not coverage in self.coverageEventsData_[0]:
                self.coverageEventsData_[0].append(coverage)
                self.coverageEventsData_[0].sort()
                idx = self.coverageEventsData_[0].index(coverage)
                self.coverageEventsData_[1].insert(idx, 0L)
                self.coverageEventsData_[2].insert(idx, 0)
        
        for toAddCoverage, toAddTime in zip(*coverageEvents):
            for i,coverage in self.coverageEventsData_[0]:
                if coverage == toAddCoverage:
                    self.coverageEventsData_[1][i] = self.coverageEventsData_[1][i] + toAddTime
                    self.coverageEventsData_[2][i] = self.coverageEventsData_[2][i] + 1
                
    def getMean(self):
        result = [[],[]]
        for coverage, time, count  in zip(*self.coverageEventsData_):
            result[0].append(coverage)
            result[1].append(time / count)
            
        return result

class MeanTileTimeBetweenVisits:
    
    # (time, count)
    tileTimeData_ = dict()
    
    def reset(self):
        self.tileTimeData_ = dict()
    
    def add(self, tileTimeBetweenVisits):
        assert(len(tileTimeBetweenVisits) == 3)
        
        if len(self.tileTimeData_) == 0:
            for x,y in zip(tileTimeBetweenVisits[0], tileTimeBetweenVisits[1]):
                key = self.valueToKey(x,y)
                self.tileTimeData_[key] = (0L,0)
                
        assert(len(tileTimeBetweenVisits[0]) == len(self.tileTimeData_))
        
        for x,y,tileTime in zip(*tileTimeBetweenVisits):
            key = self.getKey(x,y)
            
            assert(key in self.tileTimeData_)
            if tileTime > 0:
                self.tileTimeData_[key][0] = self.tileTimeData_[key][0] + tileTime
                self.tileTimeData_[key][1] = self.tileTimeData_[key][1] + 1
                               
    def getMean(self):
        result = [[],[],[]]
        
        for key,values in self.tileTimeSum_:
            coord = self.keyToValue(key)
            
            result[0].append(coord[0])
            result[1].append(coord[1])
            
            if values[1] == 0:
                result[2].append(0L)
            else:
                result[2].append(long(values[0] / values[1]))
            
        return result 
            
    def valueToKey(self, x, y):
        result = str(x) + "," + str(y)
        return result
    
    def keyToValue(self, key):
        values = key.split(',')
        assert(len(values) == 2)
        
        return (int(values[0]), int(values[1]))
    
    
class MeanTileVisits:
    # (visits, count)
    tileVisitsData_ = dict()
    
    def reset(self):
        self.tileVisitsData_ = dict()
    
    def add(self, tileVisits):
        assert(len(tileVisits) == 3)
        
        if(len(self.tileVisitsData_) == 0):
            for x,y in zip(tileVisits[0], tileVisits[1]):
                key = self.valueToKey(x, y)
                self.tileVisitsData_[key] = (0, 0)
        
        assert(len(tileVisits[0]) == len(self.tileVisitsData_))
        
        for x,y,visits in zip(tileVisits[0], tileVisits[1],tileVisits[2]):
            key = self.valueToKey(x, y)
            
            self.tileVisitsData_[key][0] = self.tileVisitsData_[key][0] + visits
            self.tileVisitsData_[key][1] = self.tileVisitsData_[key][1] + 1 
    
    def getMean(self):
        result = [[], [], []]
        
        for key, values in self.tileVisitsSum_:
            coord = self.keyToValue(key)
            result[0].append(coord[0])
            result[1].append(coord[1])
            
            if values[1] == 0:
                result[2].append(0.0)
            else:
                result[2].append(float(values[0]) / float(values[1]))
            
        return result
    
    def valueToKey(self, x, y):
        result = str(x) + "," + str(y)
        return result
    
    def keyToValue(self, key):
        values = key.split(',')
        assert(len(values) == 2)
        
        return (int(values[0]), int(values[1]))

class AlgorithmDirectory:
    directory_ = ""
    
    meanCoverageEvents_ = dict()
    meanTileTimeBetweenVisits_ = dict()
    meanTileVisits_ = dict()
    meanGridTimeBetweenVisits_ = dict()
    meanGridVisits_ = dict()
    meanFinalCoverage_ = dict()
    
    worldTypes_ = set()
    
    def reset(self):
        self.directory_ = ""
        
        self.meanCoverageEvents_ = dict()
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
        
        print "-- loading '" + self.getName() + "'"
        
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
                self.meanTileTimeBetweenVisits_[worldKey] = dict()
                self.meanTileVisits_[worldKey] = dict()
                self.meanGridTimeBetweenVisits_[worldKey] = dict()
                self.meanGridVisits_[worldKey] = dict()
                self.meanFinalCoverage_[worldKey] = dict()
            
            if not robotKey in self.meanCoverageEvents_[worldKey]:
                self.meanCoverageEvents_[worldKey][robotKey] = MeanCoverageEvents()
                self.meanTileTimeBetweenVisits_[worldKey][robotKey] = MeanTileTimeBetweenVisits()
                self.meanTileVisits_[worldKey][robotKey] = MeanTileVisits()
                self.meanGridTimeBetweenVisits_[worldKey][robotKey] = (0L, 0)
                self.meanGridVisits_[worldKey][robotKey] = (0.0, 0)
                self.meanFinalCoverage_[worldKey] = (0.0, 0L, 0)
                
            self.meanCoverageEvents_[worldKey][robotKey].add(experimentDir.coverageEvents_)
            
            self.meanTileTimeBetweenVisits_[worldKey][robotKey].add(experimentDir.meanTileTimeBetweenVisits_)
            
            self.meanTileVisits_[worldKey][robotKey].add(experimentDir.tileVisits_)
            
            self.meanGridTimeBetweenVisits_[worldKey][robotKey][0] = self.meanGridTimeBetweenVisits_[worldKey][robotKey][0] + experimentDir.meanGridTimeBetweenVisits_
            self.meanGridTimeBetweenVisits_[worldKey][robotKey][1] = self.meanGridTimeBetweenVisits_[worldKey][robotKey][1] + 1
            
            self.meanGridVisits_[worldKey][robotKey][0] = self.meanGridVisits_[worldKey][robotKey][0] + experimentDir.meanGridVisits_
            self.meanGridVisits_[worldKey][robotKey][1] = self.meanGridVisits_[worldKey][robotKey][1] + 1
            
            self.meanFinalCoverage_[worldKey][robotKey][0] = self.meanFinalCoverage_[worldKey][robotKey][0] + experimentDir.finalCoverage_[0][0]
            self.meanFinalCoverage_[worldKey][robotKey][1] = self.meanFinalCoverage_[worldKey][robotKey][1] + experimentDir.finalCoverage_[1][0]
            self.meanFinalCoverage_[worldKey][robotKey][2] = self.meanFinalCoverage_[worldKey][robotKey][2] + 1
            
  
    def save(self):
        print "-- saving summary of '" + self.getName() + "'"
        
        summaryDir = self.getSummaryDir()
        mkdirRec(summaryDir)
        dataFile = DataFile()
        
        filename = ""
        for worldType in self.worldTypes_:
            worldDir = os.path.join(summaryDir,worldType)
            mkdirRec(worldDir)
            
            for robotCount in self.meanCoverageEvents_[worldType]:
                
                filename = self.getSaveFilename(worldDir, COVERAGE_EVENTS_FILE, robotCount)
                dataFile.reset()
                dataFile.addComment("# [coverage meanTimestamp(usec)]")
                data = self.meanCoverageEvents_[worldType][robotCount].getMean()
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
                data = [[ self.meanGridTimeBetweenVisits_[worldType][robotCount][0] / self.meanGridTimeBetweenVisits_[worldType][robotCount][1] ]]
                dataFile.setDataAs(data, "l")
                dataFile.save(filename)
                
                filename = self.getSaveFilename(worldDir, MEAN_GRID_VISITS_FILE, robotCount)
                dataFile.reset()
                dataFile.addComment("# [meanVisitCount]")
                data = [[ self.meanGridVisits_[worldType][robotCount][0] / self.meanGridVisits_[worldType][robotCount][1] ]]
                dataFile.setDataAs(data, "f")
                dataFile.save(filename)
        
    def plot(self):
        print "-- plotting summary data of '" + self.getName() + "'";
            
            
    def getSaveFilename(self, worldDir, filename, robotCount):
        return os.path.join(worldDir, str(robotCount) + "-" + filename)    
    
    def getName(self):
        return os.path.basename(os.path.normpath(self.directory_))
    
    def getSummaryDir(self):
        return os.path.join(self.directory_, SUMMARY_DIRECTORY)