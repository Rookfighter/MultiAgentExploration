import os
from datafile import DataFile

# all files that are in an experiment directory
TILE_VISITS_FILE = "tile-visits.dat"
VISITS_EVENTS_FILE = "visits-events.dat"
TIME_BETWEEN_VISITS_EVENTS_FILE = "time-between-visits-events.dat"
MEAN_TILE_TIME_BEWTEEN_VISITS_FILE = "mean-tile-time-between-visits.dat"
COVERAGE_EVENTS_FILE = "coverage-events.dat"
TIME_EVENTS_FILE = "time-events.dat"
FINAL_COVERAGE_FILE = "final-coverage.dat"
EXPERIMENT_CONFIG_FILE = "experiment-config.dat"

class ExperimentDirectory:
    
    def __init__(self):
        self.reset()
    
    def reset(self):
        self.directory_ = ""
        self.coverageEvents_ = []
        self.timeEvents_ = []
        self.finalCoverage_ = []
        self.meanTileTimeBetweenVisits_ = []
        self.tileVisits_ = []
        self.timeBetweenVisitsEvents_ = []
        self.visitsEvents_ = []
    
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
        
        # load time events
        filename = os.path.join(self.directory_, TIME_EVENTS_FILE)
        dataFile.load(filename)
        self.timeEvents_= dataFile.getDataAs("fl")
        
        # load experiment config
        filename = os.path.join(self.directory_, EXPERIMENT_CONFIG_FILE)
        dataFile.load(filename)
        self.robotCount_ = int(dataFile.data_[0][0]);
        self.worldType_ = dataFile.data_[0][1];
        
        # load final coverage
        filename = os.path.join(self.directory_, FINAL_COVERAGE_FILE)
        dataFile.load(filename)
        self.finalCoverage_ = dataFile.getDataAs("fl")
        
        # load meanTileTimeBetweenVisits
        filename = os.path.join(self.directory_, MEAN_TILE_TIME_BEWTEEN_VISITS_FILE)
        dataFile.load(filename)
        self.meanTileTimeBetweenVisits_ = dataFile.getDataAs("iil")
        
        # load tileVisits
        filename = os.path.join(self.directory_, TILE_VISITS_FILE)
        dataFile.load(filename)
        self.tileVisits_ = dataFile.getDataAs("iii")
        
        # load visitsEvents
        filename = os.path.join(self.directory_, VISITS_EVENTS_FILE)
        dataFile.load(filename)
        self.visitsEvents_ = dataFile.getDataAs("fff")
        
        # load timeBetweenVisitsEvents
        filename = os.path.join(self.directory_, TIME_BETWEEN_VISITS_EVENTS_FILE)
        dataFile.load(filename)
        self.timeBetweenVisitsEvents_ = dataFile.getDataAs("fll")
        
    def getName(self):
        return os.path.basename(os.path.normpath(self.directory_))