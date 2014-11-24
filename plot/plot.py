import os
import sys
import subprocess

# Filenames
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

def plotCoverageEvents(infile, outfile):
    subprocess.call(["gnuplot" , "-e", "infile='" + infile + "'; outfile='" + outfile + "'", PLOT_VISITS_HEATMAP_FILE])
    
def plotVisitsHeatMap(infile, outfile):
    subprocess.call(["gnuplot" , "-e", "infile='" + infile + "'; outfile='" + outfile + "'", PLOT_VISITS_HEATMAP_FILE])

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
            for comment in self.comments_:
                f.write(comment + "\n")
            if self.data_:
                for line in xrange(0, len(self.data_[0])):
                    toWrite = ""
                    for column in self.data_:
                        toWrite += column[line] + " "
                    toWrite.strip()
                    f.write(toWrite) 
    
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
        self.data = []
        self.comments = []
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
        coverageEvents = []
        experimentConfig = []
        meanTileTimeBetweenVisits = []
        tileVisits = []
        
        data = DataFile()
        filename = ""
        
        print "-- loading data of '" + self.directory_ + "'"
        
        filename = data.load(os.path.join(self.directoy_, COVERAGE_EVENTS_FILE))
        print "-- loading coverageEvents '" + filename + "'"
        data.load(filename)
        self.coverageEvents_.append(data.columnAsFloat(0))
        self.coverageEvents_.append(data.columnAsLong(1))
        
        filename = data.load(os.path.join(self.directoy_, EXPERIMENT_CONFIG_FILE))
        print "-- loading experimenConfig '" + filename + "'"
        data.load(filename)
        self.experimentConfig_ = data.columnAsLong(0)
        
        filename = data.load(os.path.join(self.directoy_, FINAL_COVERAGE_FILE))
        print "-- loading finalCoverage '" + filename + "'"
        data.load(filename)
        self.finalCoverage_ = data.columnAsFloat(0)[0]
        
        filename = data.load(os.path.join(self.directoy_, MEAN_GRID_TIME_BEWTEEN_VISITS_FILE))
        print "-- loading meanGridTimeBetweenVisits '" + filename + "'"
        data.load(filename)
        self.meanGridTimeBetweenVisits_ = data.columnAsLong(0)[0]
        
        filename = data.load(os.path.join(self.directoy_, MEAN_TILE_TIME_BEWTEEN_VISITS_FILE))
        print "-- loading meanTileTimeBetweenVisits '" + filename + "'"
        data.load(filename)
        self.meanTileTimeBetweenVisits_.append(data.columnAsLong(0))
        self.meanTileTimeBetweenVisits_.append(data.columnAsLong(1))
        self.meanTileTimeBetweenVisits_.append(data.columnAsLong(2))
        
        filename = data.load(os.path.join(self.directoy_, TILE_VISITS_FILE))
        print "-- loading tileVisits '" + filename + "'"
        data.load(filename)
        self.tileVisits_.append(data.columnAsLong(0))
        self.tileVisits_.append(data.columnAsLong(1))
        self.tileVisits_.append(data.columnAsLong(2))
        
        filename = data.load(os.path.join(self.directoy_, MEAN_GRID_VISITS_FILE))
        print "-- loading meanGridVisits '" + filename + "'"
        data.load(filename)
        self.meanGridVisits_ = data.columnAsFloat(0)[0]
        
    def plot(self):
        infile = ""
        outfile = ""
        
        print "-- plotting data of '" + self.directory_ + "'"
        
        infile = os.path.join(self.directoy_, TILE_VISITS_FILE)
        outfile = os.path.join(self.directoy_, OUT_TILE_VISITS_HEATMAP_FILE)
        plotVisitsHeatMap(infile, outfile)
        
        infile = os.path.join(self.directoy_, COVERAGE_EVENTS_FILE)
        outfile = os.path.join(self.directoy_, OUT_COVERAGE_EVENTS_FILE)
        plotCoverageEvents(infile, outfile)

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
        self.clear()
        self.directory_ = directory
        
        print "++ processing '" + self.directory_ + "'"
        
        subDirectories = getSubdirectoriesConcat(self.directory_)
        experimentCount = len(subDirectories)
        experimentDir = ExperimentDirectory()
        
        for subDir in subDirectories:
            experimentDir.load(subDir)
            experimentDir.plot()
            
            robotCount = str(experimentDir.experimentConfig_[0][0])
            
            if robotCount in self.robotCountSamples_:
                robotCountSamples_[robotCount] += 1
            else:
                robotCountSamples_[robotCount] = 1
            
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
                
                for timeSum, timeToAdd in zip(self.meanTileTimeBetweenVisits_[robotCount][2], experimentDir.meanTileTimeBetweenVisits[2]):
                    timeSum += timeToAdd
            else:
                self.meanTileTimeBetweenVisits_[robotCount] = experimentDir.meanTileTimeBetweenVisits_
                
            # calculate sum of tileVisits
            if robotCount in self.meanTileVisits_:
                assert(len(self.meanTileVisits_[robotCount]) == len(experimentDir.tileVisits_))
                assert(len(self.meanTileVisits_[robotCount][2]) == len(experimentDir.tileVisits_[2]))
                
                for visitSum, visitsToAdd in zip(self.meanTileVisits_[robotCount][2], experimentDir.tileVisits[2]):
                    visitSum += visitsToAdd
            else:
                self.meanTileVisits_[robotCount] = experimentDir.tileVisits_
            
        # calc mean of coverageEvents
        for key, value in self.meanCoverageEvents_:
            sampleCount = robotCountSamples_[key]
            for coverage in value[1]:
                coverage /= samplecount
                
        # calc mean of gridTimeBetweenVisits
        for key, value in self.meanGridTimeBetweenVisits_:
            sampleCount = robotCountSamples_[key]
            value /= sampleCount
        
        # calc mean of gridVisits
        for key, value in self.meanGridVisits_:
            sampleCount = robotCountSamples_[key]
            value /= sampleCount
            
        # calc mean of tileTimeBetweenVisits
        for key, value in self.meanTileTimeBetweenVisits_:
            sampleCount = robotCountSamples_[key]
            for time in value[2]:
                time /= sampleCount
                
        # calc mean of tileVisits
        for key, value in self.meanTileVisits_:
            sampleCount = robotCountSamples_[key]
            for visits in value[2]:
                visits /= sampleCount
                
        self.save()
        self.plot()
                
    def save(self):
        print "-- saving data for algorithm"
        
        datafile = DataFile()
        filename = ""
        for key in robotCountSamples_:
            filename = self.getFilename(MEAN_COVERAGE_EVENTS_FILE, key)
            datafile.clear()
            datafile.addComment()
            datafile.addFloatColumn(self.meanCoverageEvents_[key][0])
            datafile.addLongColumn(self.meanCoverageEvents_[key][1])
            datafile.save(filename)
            
            filename = self.getFilename(MEAN_GRID_TIME_BEWTEEN_VISITS_FILE, key)
            datafile.clear()
            datafile.addComment()
            datafile.addLongColumn([ self.meanGridTimeBetweenVisits_[key] ])
            datafile.save(filename)
            
            filename = self.getFilename(MEAN_GRID_VISITS_FILE, key)
            datafile.clear()
            datafile.addComment()
            datafile.addFloatColumn([ self.meanGridVisits_[key] ])
            datafile.save(filename)
            
            filename = self.getFilename(MEAN_TILE_TIME_BEWTEEN_VISITS_FILE, key)
            datafile.clear()
            datafile.addComment()
            datafile.addLongColumn(self.meanTileTimeBetweenVisits_[key][0])
            datafile.addLongColumn(self.meanTileTimeBetweenVisits_[key][1])
            datafile.addLongColumn(self.meanTileTimeBetweenVisits_[key][2])
            datafile.save(filename)
            
            filename = self.getFilename(MEAN_TILE_VISITS_FILE, key)
            datafile.clear()
            datafile.addComment()
            datafile.addLongColumn(self.meanTileVisits_[key][0])
            datafile.addLongColumn(self.meanTileVisits_[key][1])
            datafile.addLongColumn(self.meanTileVisits_[key][2])
            datafile.save(filename)
        
    def plot(self):
        infile = ""
        outfile = ""
        
        print "-- plotting data of algorithm"
        
        for key in robotCountSamples_:
            infile = self.getFilename(MEAN_TILE_VISITS_FILE, key)
            outfile = self.getFilename(OUT_TILE_VISITS_HEATMAP_FILE, key)
            plotVisitsHeatMap(infile, outfile)
            
            infile = self.getFilename(MEAN_COVERAGE_EVENTS_FILE, key)
            outfile = self.getFilename(OUT_COVERAGE_EVENTS_FILE, key)
            plotCoverageEvents(infile, outfile)
            
            
    def getFilename(self, filename, robotCount):
        return os.path.join(self.directory_, robotCount + "-" + filename)    
    
    def clear(self):
        self.directory = ""
        self.experimentCount_ = 0
        self.robotCountSamples_ = dict()
        self.meanCoverageEvents_ = dict()
        self.meanGridTimeBetweenVisits_ = dict()
        self.meanGridVisits_ = dict()
        self.meanTileTimeBetweenVisits_ = dict()
        self.meanTileVisits_ = dict()
    
    
def getImmediateSubdirectories(p_dir):
    return [name for name in os.listdir(p_dir)
            if os.path.isdir(os.path.join(p_dir, name))]
            
def getSubdirectoriesConcat(p_dir):
    subDirs = getImmediateSubdirectories(p_dir)
    for i in xrange(0, len(subDirs)):
        subDirs[i] = os.path.join(p_dir, subDirs[i])
    return subDirs
            
def createMeanTileVisits(p_algoDir):

    print "== create meanTileVisit files"
    
    subDirs = getSubdirectoriesConcat(p_algoDir)
    if len(subDirs) == 0:
        return
    
    print "-- read values from files"
    dircnt = 0
    maxlinecnt = -1
    allTileVisits = []
    allMeanGridVisits = []
    
    # process all subdirectories of algorithm directory
    for runDir in subDirs:
        # get filenames for visit files
        tileVisitsFile = os.path.join(runDir, TILE_VISITS_FILE)
        meanGridVisitsFile = os.path.join(runDir, MEAN_GRID_VISITS_FILE)
        allTileVisits.append([])
        
        # process file containing visits per tile
        with open(tileVisitsFile) as f:
            linecnt = 0
            for line in f:
                # only lines that are no comments are valid
                if line[0] == '#' or len(line) == 0:
                    continue
                    
                values = line.split(' ')
                allTileVisits[dircnt].append([values[0], values[1], float(values[2])])
                linecnt += 1
            if maxlinecnt < 0:
                maxlinecnt = linecnt
            
            # all files must have same amount of lines
            # => all simulation runs had floorplan with same size
            assert linecnt == maxlinecnt
        
        # process file containing mean visits of the whole grid
        with open(meanGridVisitsFile) as f:
            for line in f:
                if line[0] == '#' or len(line) == 0:
                    continue
                allMeanGridVisits.append(float(line))
        dircnt += 1
    
    print "-- calculate mean values"
    # calculate mean visit count for every tile from all files
    meanTileVisits = []
    for i in xrange(0, maxlinecnt):
        tileVisitSum = 0.0
        # sum is calculated from line value of all files
        for j in xrange(0, dircnt):
            tileVisitSum += allTileVisits[j][i][2]
        meanTileVisits.append([allTileVisits[0][i][0], allTileVisits[0][i][1], tileVisitSum / dircnt])
    
    # calculate mean visit count of all grids
    meanGridVisits = 0.0
    for meanGridVisitsValue in allMeanGridVisits:
        meanGridVisits += meanGridVisitsValue
    meanGridVisits = meanGridVisits / len(allMeanGridVisits)
        
    meanTileVisitsFile = os.path.join(p_algoDir, MEAN_TILE_VISITS_FILE)
    print "-- save to '" + meanTileVisitsFile + "'"
    
    with open(meanTileVisitsFile, 'w') as f:
        f.write("# [x y meanVisitCount]")
        for visit in meanTileVisits:
            f.write("\n" + visit[0] + " " + visit[1] + " " + repr(visit[2]))
            
    meanGridVisitsFile = os.path.join(p_algoDir, MEAN_GRID_VISITS_FILE)
    print "-- save to '" + meanGridVisitsFile + "'"
    
    with open(meanGridVisitsFile, 'w') as f:
        f.write("# [meanVisitCount]\n")
        f.write(repr(meanGridVisits)) 

def createMeanTimeBetweenVisits(p_algoDir):
    print "== create meanTimeBetweenVisits files"
    
    subDirs = getSubdirectoriesConcat(p_algoDir)
    
    if len(subDirs) == 0:
        return
    
    print "-- read values from files"
    dircnt = 0
    maxlinecnt = -1
    allTileTimes = []
    allGridTimes = []
    # process all subdirectories of algorithm directory
    for runDir in subDirs:
        # get filenames of timeBetweenVisit files
        tileTimesFile = os.path.join(runDir, MEAN_TILE_TIME_BEWTEEN_VISITS_FILE)
        gridTimesFile = os.path.join(runDir, MEAN_GRID_TIME_BEWTEEN_VISITS_FILE)
        allTileTimes.append([])
        # process file containing meanTimeBetweenVisits of each tile
        with open(tileTimesFile) as f:
            linecnt = 0
            for line in f:
                # only non-comment lines are processed
                if line[0] == '#' or len(line) == 0:
                    continue
                values = line.split(' ')
                
                allTileTimes[dircnt].append([values[0], values[1], long(float(values[2]))])
                linecnt += 1
            if  maxlinecnt < 0:
                maxlinecnt = linecnt
            # all files must have same amount of lines
            # => all simulation runs had floorplan with same size
            assert maxlinecnt == linecnt
        
        # process file containing meanTimeBetweenVisits of whole grid
        with open(gridTimesFile) as f:
            for line in f:
                if line[0] == '#' or len(line) == 0:
                    continue
                allGridTimes.append(long(float(line)))
        dircnt += 1
    
    print "-- calculate mean values"
    # calculate meanTimeBetweenVisits for each tile from all files
    meanTileTimes = []
    for i in xrange(0, maxlinecnt):
        lineSum = 0L
        for j in xrange(0, dircnt):
            lineSum += allTileTimes[j][i][2]
        meanTileTimes.append([allTileTimes[0][i][0], allTileTimes[0][i][1], lineSum / dircnt])
    
    # calculate meanTimeBetweenVisits for grid from all files
    meanGridTimes = 0L
    for gridTimesValue in allGridTimes:
        meanGridTimes += gridTimesValue
    meanGridTimes = meanGridTimes / len(allGridTimes)
    
    meanTileTimeFile = os.path.normpath(p_algoDir) + "/" + MEAN_TILE_TIME_BEWTEEN_VISITS_FILE
    print "-- save to '" + meanTileTimeFile + "'"
    
    with open(meanTileTimeFile, 'w') as f:
        f.write("# [x y meanTimeBetweenVisits(usec)]")
        for time in meanTileTimes:
            f.write("\n" + time[0] + " " + time[1] + " " + str(time[2]))
            
    meanGridTimeFile = os.path.normpath(p_algoDir) + "/" + MEAN_GRID_TIME_BEWTEEN_VISITS_FILE
    print "-- save to '" + meanGridTimeFile + "'"
    with open(meanGridTimeFile, 'w') as f:
        f.write("# [meanTimeBetweenVisits(usec)]\n")
        f.write(str(meanGridTimes))

def createMeanCoverageEvents(p_algoDir):
    print "== create meanCoverageEvents files"
    
    subDirs = getSubdirectoriesConcat(p_algoDir)
    
    if len(subDirs) == 0:
        return
    
    print "-- read values from files"
    dircnt = 0
    maxlinecnt = 0
    allCoverageEvents = []
    # process all subdirectories of algorithm directory
    for runDir in subDirs:
        # get filenames of coverageEvents files
        coverageEventsFile = os.path.join(runDir, COVERAGE_EVENTS_FILE)
        allCoverageEvents.append([])
        # process file containing coverageEvents and their timestamps
        with open(coverageEventsFile) as f:
            linecnt = 0
            for line in f:
                if line[0] == '#' or len(line) == 0:
                    continue
                values = line.split(' ')
                
                allCoverageEvents[dircnt].append([values[0], long(float(values[1]))])
                linecnt += 1
            # some files may have less events
            # => did not reach a certain coverage
            maxlinecnt = max(maxlinecnt, linecnt)
        dircnt += 1
    
    print "-- calculate mean values"
    # calcutlate mean timestamps for each coverageEvent from all files
    meanCoverageEvents = []
    for i in xrange(0, maxlinecnt):
        lineSum = 0L
        # we have to count the lines
        # some files may have less events, because they never reached the coverage
        linecnt = 0
        for j in xrange(0, len(allCoverageEvents)):
            if len(allCoverageEvents[j]) < i:
                continue
            lineSum += allCoverageEvents[j][i][1]
            linecnt += 1
        meanCoverageEvents.append([allCoverageEvents[0][i][0], lineSum / linecnt])
    
    meanCoverageEventsFile = os.path.normpath(p_algoDir) + "/" + MEAN_COVERAGE_EVENTS_FILE
    print "-- save to '" + meanCoverageEventsFile + "'"
    
    with open(meanCoverageEventsFile, 'w') as f:
        f.write("# [coverage timestamp(usec)]")
        for time in meanCoverageEvents:
            f.write("\n" + time[0] + " " + str(time[1]))
            
            
def plotDataFiles(p_algoDir):
    print "== plot data files"
    
    subDirs = getImmediateSubdirectories(p_algoDir)
    
    if len(subDirs) == 0:
        return
    
    for i in xrange(0, len(subDirs)):
        subDirs[i] = os.path.join(p_algoDir, subDirs[i])
    
    print "-- create plots for each simulation run"
    for runDir in subDirs:
        tileVisitsFile = os.path.join(runDir, TILE_VISITS_FILE)
        tileVisitsHeatmapOutFile = os.path.join(runDir, OUT_TILE_VISITS_HEATMAP_FILE)
        coverageEventsFile = os.path.join(runDir, COVERAGE_EVENTS_FILE)
        coverageEventsOutFile = os.path.join(runDir, OUT_COVERAGE_EVENTS_FILE)
        subprocess.call(["gnuplot" , "-e", "infile='" + tileVisitsFile + "'; outfile='" + tileVisitsHeatmapOutFile + "'", PLOT_VISITS_HEATMAP_FILE])
        subprocess.call(["gnuplot" , "-e", "infile='" + coverageEventsFile + "'; outfile='" + coverageEventsOutFile + "'", PLOT_COVERAGE_EVENTS_FILE])
    
    print "-- create plots for algorithm"
    meanTileVisitsFile = os.path.join(p_algoDir, MEAN_TILE_VISITS_FILE)
    meanTileVisitsHeatmapOutFile = os.path.join(p_algoDir, OUT_MEAN_TILE_VISITS_HEATMAP_FILE)
    meanCoverageEventsFile = os.path.join(p_algoDir, MEAN_COVERAGE_EVENTS_FILE)
    meanCoverageOutFile = os.path.join(p_algoDir, OUT_MEAN_COVERAGE_EVENTS_FILE)
    subprocess.call(["gnuplot" , "-e", "infile='" + meanTileVisitsFile + "'; outfile='" + meanTileVisitsHeatmapOutFile + "'", PLOT_VISITS_HEATMAP_FILE])
    subprocess.call(["gnuplot" , "-e", "infile='" + meanCoverageEventsFile + "'; outfile='" + meanCoverageOutFile + "'", PLOT_COVERAGE_EVENTS_FILE])
    
# main
originDir = os.getcwd()
baseDir = os.path.dirname(os.path.realpath(sys.argv[0]))
algorithmDirs = getImmediateSubdirectories(baseDir)

print ""
print "Plot MultiAgentExploration v1.0"
print "-------------------------------"
print "cd to '" + baseDir + "'"
print ""
os.chdir(baseDir)

for algoDir in algorithmDirs:

    print "++ " + os.path.basename(os.path.normpath(algoDir))
    
    createMeanTileVisits(algoDir)
    createMeanTimeBetweenVisits(algoDir)
    createMeanCoverageEvents(algoDir)
    plotDataFiles(algoDir)
        
print ""
print "return to '" + originDir + "'"
os.chdir(originDir)
