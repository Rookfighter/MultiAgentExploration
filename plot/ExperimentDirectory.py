import os
from DataFile import DataFile

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
        data = DataFile()
        
        print "-- loading data from '" + self.getName() + "'"
        
        # load coverage events
        filename = os.path.join(self.directory_, COVERAGE_EVENTS_FILE)
        data.load(filename)
        self.coverageEvents_.append(data.columnAsFloat(0))
        self.coverageEvents_.append(data.columnAsLong(1))
        
        # load experiment config
        filename = os.path.join(self.directory_, EXPERIMENT_CONFIG_FILE)
        data.load(filename)
        self.robotCount_ = int(data.data_[0][0]);
        self.worldType_ = data.data_[1][0];
        
        # load final coverage
        filename = os.path.join(self.directory_, FINAL_COVERAGE_FILE)
        data.load(filename)
        self.finalCoverage_ = data.columnAsFloat(0)[0]
        
        # load meanGridTimeBetweenVisits
        filename = os.path.join(self.directory_, MEAN_GRID_TIME_BEWTEEN_VISITS_FILE)
        data.load(filename)
        self.meanGridTimeBetweenVisits_ = data.columnAsLong(0)[0]
        
        # load meanTileTimeBetweenVisits
        filename = os.path.join(self.directory_, MEAN_TILE_TIME_BEWTEEN_VISITS_FILE)
        data.load(filename)
        self.meanTileTimeBetweenVisits_.append(data.columnAsLong(0))
        self.meanTileTimeBetweenVisits_.append(data.columnAsLong(1))
        self.meanTileTimeBetweenVisits_.append(data.columnAsLong(2))
        
        # load tileVisits
        filename = os.path.join(self.directory_, TILE_VISITS_FILE)
        data.load(filename)
        self.tileVisits_.append(data.columnAsLong(0))
        self.tileVisits_.append(data.columnAsLong(1))
        self.tileVisits_.append(data.columnAsLong(2))
        
        # load meanGridVisits
        filename = os.path.join(self.directory_, MEAN_GRID_VISITS_FILE)
        data.load(filename)
        self.meanGridVisits_ = data.columnAsFloat(0)[0]
        
    def getName(self):
        return os.path.basename(os.path.normpath(self.directory_))