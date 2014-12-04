from experimentdir import ExperimentDirectory
from utils import mkdirRec
from utils import getSubdirectoriesConcat

SUMMARY_DIRECTORY = "summary"

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
    
    # [x, y, time, count]
    tileTimeData_ = [[], [], [], []]
    
    def reset(self):
        self.tileTimeSum_ = dict()
        self.coverageEventsCount_ = dict()
    
    def add(self, tileTimeBetweenVisits):
        assert(len(tileTimeBetweenVisits) == 3)
        
        for x,y,tileTime in zip(tileTimeBetweenVisits[0], tileTimeBetweenVisits[1],tileTimeBetweenVisits[2]):
            key = self.getKey(x,y)
            
            if key in self.tileTimeSum_:
                if tileTime > 0:
                    self.tileTimeSum_[key] = self.tileTimeSum_[key] + tileTime
                    self.tileTimeCount_[key] = self.tileTimeCount_[key] + 1
            else:
                if tileTime > 0:
                    self.tileTimeSum_[key] = tileTime
                    self.tileTimeCount_[key] = 1
                else:
                    self.tileTimeSum_[key] = 0L
                    self.tileTimeCount_[key] = 0
                    
    def getMean(self):
        result = dict()
        
        for key in self.tileTimeSum_:
            if self.tileTimeCount_[key] != 0:
                result[key] = self.tileTimeSum_[key] / self.tileTimeCount_[key]
            
        return result 
            
    def getKey(self, x, y):
        result = str(x) + "," + str(y)
        return result
    
class MeanTileVisits:
    tileVisitsSum_ = dict()
    tileVisitsCount_ = dict()
    
    def reset(self):
        self.tileVisitsSum_ = dict()
        self.tileVisitsCount_ = dict()
    
    def add(self, tileVisits):
        assert(len(tileVisits) == 3)
        
        for x,y,visits in zip(tileVisits[0], tileVisits[1],tileVisits[2]):
            key = self.getKey(x,y)
            
            if key in self.tileTimeSum_:
                self.tileVisitsSum_[key] = self.tileVisitsSum_[key] + visits
                self.tileVisitsCount_[key] = self.tileVisitsCount_[key] + 1
            else:
                self.tileVisitsSum_[key] = visits
                self.tileVisitsCount_[key] = 1
    
    def getMean(self):
        result = dict()
        
        for key in self.tileVisitsSum_:
            result[key] = self.tileVisitsSum_[key] / self.tileVisitsCount_[key]
            
        return result
    
    def getKey(self, x, y):
        result = str(x) + "," + str(y)
        return result

class AlgorithmDirectory:
    directory_ = ""
    meanCoverageEvents_ = dict()
    meanGridTimeBetweenVisits_ = dict()
    meanGridVisits_ = dict()
    meanTileTimeBetweenVisits_ = dict()
    meanTileVisits_ = dict()
    
    def reset(self):
        self.directory_ = ""
        self.meanCoverageEvents_ = dict()
        self.meanGridTimeBetweenVisits_ = dict()
        self.meanGridVisits_ = dict()
        self.meanTileTimeBetweenVisits_ = dict()
        self.meanTileVisits_ = dict()
        
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
            
            if not worldKey in self.meanCoverageEvents_:
                self.meanCoverageEvents_[worldKey] = dict()
                self.meanGridTimeBetweenVisits_[worldKey] = dict()
                self.meanGridVisits_[worldKey] = dict()
                self.meanTileTimeBetweenVisits_[worldKey] = dict()
                self.meanTileVisits_[worldKey] = dict()
            
            if not robotKey in self.meanCoverageEvents_[worldKey]:
                self.meanCoverageEvents_[worldKey][robotKey] = MeanCoverageEvents()
                self.meanGridTimeBetweenVisits_[worldKey][robotKey] = (0L, 0)
                self.meanGridVisits_[worldKey][robotKey] = (0.0, 0)
                self.meanTileTimeBetweenVisits_[worldKey][robotKey] = MeanTileTimeBetweenVisits()
                self.meanTileVisits_[worldKey][robotKey] = MeanTileVisits()
                
            self.meanCoverageEvents_[worldKey][robotKey].add(experimentDir.coverageEvents_)
            
            self.meanGridTimeBetweenVisits_[worldKey][robotKey][0] = self.meanGridTimeBetweenVisits_[worldKey][robotKey][0] + experimentDir.meanGridTimeBetweenVisits_
            self.meanGridTimeBetweenVisits_[worldKey][robotKey][1] = self.meanGridTimeBetweenVisits_[worldKey][robotKey][1] + 1
            
            self.meanGridVisits_[worldKey][robotKey][0] = self.meanGridVisits_[worldKey][robotKey][0] + experimentDir.meanGridVisits_
            self.meanGridVisits_[worldKey][robotKey][1] = self.meanGridVisits_[worldKey][robotKey][1] + 1
            
            self.meanTileTimeBetweenVisits_[worldKey][robotKey].add(experimentDir.meanTileTimeBetweenVisits_)
            
            self.meanTileVisits_[worldKey][robotKey].add(experimentDir.tileVisits_)
                
    def save(self):
        print "-- saving summary of '" + self.getName() + "'"
        
        mkdirRec(self.getSummaryDir())
        datafile = DataFile()
        
        filename = ""
        for key in self.robotCountSamples_:
            filename = self.getSaveFilename(MEAN_COVERAGE_EVENTS_FILE, key)
            datafile.clear()
            datafile.addComment("# [coverage meanTimestamp(usec)]")
            datafile.addFloatColumn(self.meanCoverageEvents_[key][0])
            datafile.addLongColumn(self.meanCoverageEvents_[key][1])
            datafile.save(filename)
            
            filename = self.getSaveFilename(MEAN_GRID_TIME_BEWTEEN_VISITS_FILE, key)
            datafile.clear()
            datafile.addComment("# [meanTimeBetweenVisits(usec)]")
            datafile.addLongColumn([ self.meanGridTimeBetweenVisits_[key] ])
            datafile.save(filename)
            
            filename = self.getSaveFilename(MEAN_GRID_VISITS_FILE, key)
            datafile.clear()
            datafile.addComment("# [meanVisitCount]")
            datafile.addFloatColumn([ self.meanGridVisits_[key] ])
            datafile.save(filename)
            
            filename = self.getSaveFilename(MEAN_TILE_TIME_BEWTEEN_VISITS_FILE, key)
            datafile.clear()
            datafile.addComment("# [x y meanTimeBetweenVisits(usec)]")
            datafile.addLongColumn(self.meanTileTimeBetweenVisits_[key][0])
            datafile.addLongColumn(self.meanTileTimeBetweenVisits_[key][1])
            datafile.addLongColumn(self.meanTileTimeBetweenVisits_[key][2])
            datafile.save(filename)
            
            filename = self.getSaveFilename(MEAN_TILE_VISITS_FILE, key)
            datafile.clear()
            datafile.addComment("# [x y meanVisitCount]")
            datafile.addLongColumn(self.meanTileVisits_[key][0])
            datafile.addLongColumn(self.meanTileVisits_[key][1])
            datafile.addLongColumn(self.meanTileVisits_[key][2])
            datafile.save(filename)
        
    def plot(self):
        print "-- plotting summary data of '" + self.getName() + "'";
        mkdirRec(self.getSummaryDir())
        
        for key in self.robotCountSamples_:
            infile = self.getSaveFilename(MEAN_TILE_VISITS_FILE, key)
            outfile = self.getSaveFilename(OUT_TILE_VISITS_HEATMAP_FILE, key)
            plotVisitsHeatMap(infile, outfile)
            
            infile = self.getSaveFilename(MEAN_COVERAGE_EVENTS_FILE, key)
            outfile = self.getSaveFilename(OUT_COVERAGE_EVENTS_FILE, key)
            plotCoverageEvents(infile, outfile)
            
            
    def getSaveFilename(self, filename, robotCount):
        return os.path.join(self.getSummaryDir(), robotCount + "-" + filename)    
    
    def getName(self):
        return os.path.basename(os.path.normpath(self.directory_))
    
    def getSummaryDir(self):
        return os.path.join(self.directory_, SUMMARY_DIRECTORY)