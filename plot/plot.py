import os
import sys
import subprocess
import errno

# Filenames
SUMMARY_DIRECTORY = "summary"

TILE_VISITS_FILE = "tile-visits.dat"
MEAN_GRID_VISITS_FILE = "mean-grid-visits.dat"
MEAN_TILE_TIME_BEWTEEN_VISITS_FILE = "mean-tile-time-between-visits.dat"
MEAN_GRID_TIME_BEWTEEN_VISITS_FILE = "mean-grid-time-between-visits.dat"
COVERAGE_EVENTS_FILE = "coverage-events.dat"
FINAL_COVERAGE_FILE = "final-coverage.dat"
EXPERIMENT_CONFIG_FILE = "experiment-config.dat"

MEAN_COVERAGE_EVENTS_FILE = "mean-coverage-events.dat"
MEAN_TILE_VISITS_FILE = "mean-tile-visits.dat"

PLOT_COVERAGE_EVENTS_FILE = "plot-coverage-events.gp"
PLOT_VISITS_HEATMAP_FILE = "plot-visits-heatmap.gp"

OUT_TILE_VISITS_HEATMAP_FILE = "tile-visits-heatmap.png"
OUT_COVERAGE_EVENTS_FILE = "coverage-events.png"
OUT_MEAN_TILE_VISITS_HEATMAP_FILE = "mean-tile-visits-heatmap.png"
OUT_MEAN_COVERAGE_EVENTS_FILE = "mean-coverage-events.png"

def mkdirRec(directory):
    try:
        os.makedirs(directory)
    except OSError as exc: 
        if exc.errno == errno.EEXIST and os.path.isdir(directory):
            pass

def plotCoverageEvents(infile, outfile):
    subprocess.call(["gnuplot" , "-e", "infile='" + infile + "'; outfile='" + outfile + "'", PLOT_COVERAGE_EVENTS_FILE])
    
def plotVisitsHeatMap(infile, outfile):
    subprocess.call(["gnuplot" , "-e", "infile='" + infile + "'; outfile='" + outfile + "'", PLOT_VISITS_HEATMAP_FILE])

def getImmediateSubdirectories(p_dir):
    return [name for name in os.listdir(p_dir)
            if os.path.isdir(os.path.join(p_dir, name))]
            
def getSubdirectoriesConcat(p_dir):
    subDirs = getImmediateSubdirectories(p_dir)
    for i in xrange(0, len(subDirs)):
        subDirs[i] = os.path.join(p_dir, subDirs[i])
    return subDirs

class DataFile:
    loadedFrom_ = ""
    comments_ = []
    data_ = []
    
    def load(self, filepath):
        assert(os.path.isfile(filepath))
        self.clear()
        self.loadedFrom_ = filepath
        with open(self.loadedFrom_, 'r') as f:
            columnCount = -1
            for line in f:
                # only lines that are no comments are valid
                if line[0] == '#':
                    self.comments_.append(line)
                else:
                    values = line.split(' ')
                    if columnCount < 0:
                        columnCount = len(values)
                        # add columns for data
                        for i in xrange(0, columnCount):
                            self.data_.append([])
                    assert(len(values) == columnCount)
                    
                    # add data strings to correct column
                    for i in xrange(0, len(values)):
                        self.data_[i].append(values[i])
    
    def save(self, filepath):
        with open(filepath, 'w') as f:
            commentContent = ""
            for comment in self.comments_:
                commentContent += comment + "\n"
            commentContent.strip()
            f.write(commentContent)
            if self.data_:
                dataContent = ""
                for line in xrange(0, len(self.data_[0])):
                    for column in self.data_:
                        dataContent += column[line] + " "
                    dataContent.strip()
                    dataContent += "\n"
                dataContent.strip()
                f.write(dataContent) 
    
    def columnAsLong(self, column):
        assert(column < len(self.data_))
        result = []
        
        for value in self.data_[column]:
            result.append(long(float(value)))
        
        return result

    def columnAsFloat(self, column):
        assert(column < len(self.data_))
        result = []
        
        for value in self.data_[column]:
            result.append(float(value))
        
        return result
        
    def addLongColumn(self, longColumn):
        toAdd = []
        for value in longColumn:
            toAdd.append(str(value))
        self.data_.append(toAdd)
   
    def addFloatColumn(self, floatColumn):
        toAdd = []
        for value in floatColumn:
            toAdd.append(repr(value))
        self.data_.append(toAdd)
        
    def addComment(self, comment):
        self.comments_.append(comment)
        
    def clear(self):
        self.data_ = []
        self.comments_ = []
        self.filepath_ = ""
        
class ExperimentDirectory:
    directory_ = ""
    coverageEvents_ = []
    experimentConfig_ = []
    finalCoverage_ = 0.0
    meanGridTimeBetweenVisits_ = 0L
    meanTileTimeBetweenVisits_ = []
    tileVisits_ = []
    meanGridVisits_ = 0.0
    
    def load(self, directory):
        assert(os.path.isdir(directory))
        self.directory_ = directory
        self.coverageEvents_ = []
        self.experimentConfig_ = []
        self.meanTileTimeBetweenVisits_ = []
        self.tileVisits_ = []
        
        data = DataFile()
        
        print "-- loading data of '" + self.getName() + "'"
        
        filename = os.path.join(self.directory_, COVERAGE_EVENTS_FILE)
        data.load(filename)
        self.coverageEvents_.append(data.columnAsFloat(0))
        self.coverageEvents_.append(data.columnAsLong(1))
        
        filename = os.path.join(self.directory_, EXPERIMENT_CONFIG_FILE)
        data.load(filename)
        self.experimentConfig_ = data.columnAsLong(0)
        
        filename = os.path.join(self.directory_, FINAL_COVERAGE_FILE)
        data.load(filename)
        self.finalCoverage_ = data.columnAsFloat(0)[0]
        
        filename = os.path.join(self.directory_, MEAN_GRID_TIME_BEWTEEN_VISITS_FILE)
        data.load(filename)
        self.meanGridTimeBetweenVisits_ = data.columnAsLong(0)[0]
        
        filename = os.path.join(self.directory_, MEAN_TILE_TIME_BEWTEEN_VISITS_FILE)
        data.load(filename)
        self.meanTileTimeBetweenVisits_.append(data.columnAsLong(0))
        self.meanTileTimeBetweenVisits_.append(data.columnAsLong(1))
        self.meanTileTimeBetweenVisits_.append(data.columnAsLong(2))
        
        filename = os.path.join(self.directory_, TILE_VISITS_FILE)
        data.load(filename)
        self.tileVisits_.append(data.columnAsLong(0))
        self.tileVisits_.append(data.columnAsLong(1))
        self.tileVisits_.append(data.columnAsLong(2))
        
        filename = os.path.join(self.directory_, MEAN_GRID_VISITS_FILE)
        data.load(filename)
        self.meanGridVisits_ = data.columnAsFloat(0)[0]
        
    def plot(self):
        print "-- plotting data of '" + self.getName() + "'"
        
        infile = os.path.join(self.directory_, TILE_VISITS_FILE)
        outfile = os.path.join(self.directory_, OUT_TILE_VISITS_HEATMAP_FILE)
        plotVisitsHeatMap(infile, outfile)
        
        infile = os.path.join(self.directory_, COVERAGE_EVENTS_FILE)
        outfile = os.path.join(self.directory_, OUT_COVERAGE_EVENTS_FILE)
        plotCoverageEvents(infile, outfile)
        
    def getName(self):
        return os.path.basename(os.path.normpath(self.directory_))

class AlgorithmDirectory:
    directory_ = ""
    experimentCount_ = 0
    robotCountSamples_ = dict()
    meanCoverageEvents_ = dict()
    meanGridTimeBetweenVisits_ = dict()
    meanGridVisits_ = dict()
    meanTileTimeBetweenVisits_ = dict()
    meanTileVisits_ = dict()
    
    def process(self, directory):
        assert(os.path.isdir(directory))
        self.clear()
        self.directory_ = directory
        
        print "++ processing '" + self.getName() + "'"
        
        subDirectories = getSubdirectoriesConcat(self.directory_)
        experimentDir = ExperimentDirectory()
        
        for subDir in subDirectories:
            experimentDir.load(subDir)
            experimentDir.plot()
            
            robotCount = str(experimentDir.experimentConfig_[0])
            
            if robotCount in self.robotCountSamples_:
                self.robotCountSamples_[robotCount] += 1
            else:
                self.robotCountSamples_[robotCount] = 1
            
            # calculate sum of coverage events
            if robotCount in self.meanCoverageEvents_:
                assert(len(self.meanCoverageEvents_[robotCount]) == len(experimentDir.coverageEvents_))
                assert(len(self.meanCoverageEvents_[robotCount][1]) == len(experimentDir.coverageEvents_[1]))
                
                for coverageSum, coverageToAdd in zip(self.meanCoverageEvents_[robotCount][1], experimentDir.coverageEvents_[1]):
                    coverageSum += coverageToAdd
            else:
                self.meanCoverageEvents_[robotCount] = experimentDir.coverageEvents_
           
            # calculate sum of meanGridTimeBetweenVisits
            if robotCount in self.meanGridTimeBetweenVisits_:
                self.meanGridTimeBetweenVisits_[robotCount] += experimentDir.meanGridTimeBetweenVisits_
            else:
                self.meanGridTimeBetweenVisits_[robotCount] = experimentDir.meanGridTimeBetweenVisits_
            
            # calculate sum of meanGridVisits
            if robotCount in self.meanGridVisits_:
                self.meanGridVisits_[robotCount] += experimentDir.meanGridVisits_
            else:
                self.meanGridVisits_[robotCount] = experimentDir.meanGridVisits_
                
            # calculate sum of meanTileTimeBetweenVisits
            if robotCount in self.meanTileTimeBetweenVisits_:
                assert(len(self.meanTileTimeBetweenVisits_[robotCount]) == len(experimentDir.meanTileTimeBetweenVisits_))
                assert(len(self.meanTileTimeBetweenVisits_[robotCount][2]) == len(experimentDir.meanTileTimeBetweenVisits_[2]))
                
                for timeSum, timeToAdd in zip(self.meanTileTimeBetweenVisits_[robotCount][2], experimentDir.meanTileTimeBetweenVisits_[2]):
                    timeSum += timeToAdd
            else:
                self.meanTileTimeBetweenVisits_[robotCount] = experimentDir.meanTileTimeBetweenVisits_
                
            # calculate sum of tileVisits
            if robotCount in self.meanTileVisits_:
                assert(len(self.meanTileVisits_[robotCount]) == len(experimentDir.tileVisits_))
                assert(len(self.meanTileVisits_[robotCount][2]) == len(experimentDir.tileVisits_[2]))
                
                for visitSum, visitsToAdd in zip(self.meanTileVisits_[robotCount][2], experimentDir.tileVisits_[2]):
                    visitSum += visitsToAdd
            else:
                self.meanTileVisits_[robotCount] = experimentDir.tileVisits_
        
        print "-- calculating summary of '" + os.path.basename(os.path.normpath(self.directory_)) + "'"
            
        # calc mean of coverageEvents
        for key, value in self.meanCoverageEvents_.iteritems():
            sampleCount = self.robotCountSamples_[key]
            for coverage in value[1]:
                coverage /= sampleCount
                
        # calc mean of gridTimeBetweenVisits
        for key, value in self.meanGridTimeBetweenVisits_.iteritems():
            sampleCount = self.robotCountSamples_[key]
            value /= sampleCount
        
        # calc mean of gridVisits
        for key, value in self.meanGridVisits_.iteritems():
            sampleCount = self.robotCountSamples_[key]
            value /= sampleCount
            
        # calc mean of tileTimeBetweenVisits
        for key, value in self.meanTileTimeBetweenVisits_.iteritems():
            sampleCount = self.robotCountSamples_[key]
            for time in value[2]:
                time /= sampleCount
                
        # calc mean of tileVisits
        for key, value in self.meanTileVisits_.iteritems():
            sampleCount = self.robotCountSamples_[key]
            for visits in value[2]:
                visits /= sampleCount
                
        self.save()
        self.plot()
                
    def save(self):
        print "-- saving summary data of '" + self.getName() + "'"
        
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
    
    def clear(self):
        self.directory_ = ""
        self.experimentCount_ = 0
        self.robotCountSamples_ = dict()
        self.meanCoverageEvents_ = dict()
        self.meanGridTimeBetweenVisits_ = dict()
        self.meanGridVisits_ = dict()
        self.meanTileTimeBetweenVisits_ = dict()
        self.meanTileVisits_ = dict()
            
# main
originDir = os.getcwd()
baseDir = os.path.dirname(os.path.realpath(sys.argv[0]))
algorithmDirs = getImmediateSubdirectories(baseDir)

print ""
print "Plot MultiAgentExploration v1.1"
print "-------------------------------"
print "cd to '" + baseDir + "'"
print ""
os.chdir(baseDir)

for algoDir in algorithmDirs:
    
    algorithmDirectory = AlgorithmDirectory()
    algorithmDirectory.process(algoDir)
        
print ""
print "return to '" + originDir + "'"
os.chdir(originDir)
