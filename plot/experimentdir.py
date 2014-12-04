import os
from datafile import DataFile

# all files that are in an experiment directory
TILE_VISITS_FILE = "tile-visits.dat"
MEAN_GRID_VISITS_FILE = "mean-grid-visits.dat"
MEAN_TILE_TIME_BEWTEEN_VISITS_FILE = "mean-tile-time-between-visits.dat"
MEAN_GRID_TIME_BEWTEEN_VISITS_FILE = "mean-grid-time-between-visits.dat"
COVERAGE_EVENTS_FILE = "coverage-events.dat"
FINAL_COVERAGE_FILE = "final-coverage.dat"
EXPERIMENT_CONFIG_FILE = "experiment-config.dat"

class ExperimentDirectory:
    directory_ = ""
    coverageEvents_ = []
    finalCoverage_ = 0.0
    meanGridTimeBetweenVisits_ = 0L
    meanTileTimeBetweenVisits_ = []
    tileVisits_ = []
    meanGridVisits_ = 0.0
    robotCount_ = 0
    worldType_ = ""
    
    def reset(self):
        self.directory_ = ""
        self.coverageEvents_ = []
        self.finalCoverage_ = 0.0
        self.meanGridTimeBetweenVisits_ = 0L
        self.meanTileTimeBetweenVisits_ = []
        self.tileVisits_ = []
        self.meanGridVisits_ = 0.0
    
    def load(self, directory):
        assert(os.path.isdir(directory))
        
        self.reset()
        
        self.directory_ = directory
        dataFile = DataFile()
        
        print "-- loading data from '" + self.getName() + "'"
        
        # load coverage events
        filename = os.path.join(self.directory_, COVERAGE_EVENTS_FILE)
        dataFile.load(filename)
        self.coverageEvents_= dataFile.getDataAs("fl")
        
        # load experiment config
        filename = os.path.join(self.directory_, EXPERIMENT_CONFIG_FILE)
        dataFile.load(filename)
        self.robotCount_ = int(dataFile.data_[0][0]);
        self.worldType_ = dataFile.data_[0][1];
        
        # load final coverage
        filename = os.path.join(self.directory_, FINAL_COVERAGE_FILE)
        dataFile.load(filename)
        self.finalCoverage_ = dataFile.getDataAs("f")[0][0]
        
        # load meanGridTimeBetweenVisits
        filename = os.path.join(self.directory_, MEAN_GRID_TIME_BEWTEEN_VISITS_FILE)
        dataFile.load(filename)
        self.meanGridTimeBetweenVisits_ = dataFile.getDataAs("f")[0][0]
        
        # load meanTileTimeBetweenVisits
        filename = os.path.join(self.directory_, MEAN_TILE_TIME_BEWTEEN_VISITS_FILE)
        dataFile.load(filename)
        self.meanTileTimeBetweenVisits_ = dataFile.getDataAs("lll")
        
        # load tileVisits
        filename = os.path.join(self.directory_, TILE_VISITS_FILE)
        dataFile.load(filename)
        self.tileVisits_ = dataFile.getDataAs("lll")
        
        # load meanGridVisits
        filename = os.path.join(self.directory_, MEAN_GRID_VISITS_FILE)
        dataFile.load(filename)
        self.meanGridVisits_ = dataFile.getDataAs("f")[0][0]
        
    def getName(self):
        return os.path.basename(os.path.normpath(self.directory_))