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
            for i,coverage in enumerate(self.coverageEventsData_[0]):
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
                self.tileTimeData_[key] = [0L, 0]
                
        assert(len(tileTimeBetweenVisits[0]) == len(self.tileTimeData_))
        
        for x,y,tileTime in zip(*tileTimeBetweenVisits):
            key = self.valueToKey(x,y)
            assert(key in self.tileTimeData_)
            if tileTime > 0:
                self.tileTimeData_[key][0] = self.tileTimeData_[key][0] + tileTime
                self.tileTimeData_[key][1] = self.tileTimeData_[key][1] + 1
                               
    def getMean(self):
        result = [[],[],[]]
        
        for key,values in self.tileTimeData_.iteritems():
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
                self.tileVisitsData_[key] = [0, 0]
        
        assert(len(tileVisits[0]) == len(self.tileVisitsData_))
        
        for x,y,visits in zip(tileVisits[0], tileVisits[1],tileVisits[2]):
            key = self.valueToKey(x, y)
            
            self.tileVisitsData_[key][0] = self.tileVisitsData_[key][0] + visits
            self.tileVisitsData_[key][1] = self.tileVisitsData_[key][1] + 1 
    
    def getMean(self):
        result = [[], [], []]
        
        for key, values in self.tileVisitsData_.iteritems():
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

class MeanGridTimeBetweenVisits:
    
    # [time, count]
    gridTimeSum_ = 0L
    gridTimeCount_ = 0
    
    def reset(self):
        self.gridTimeSum_ = 0L
        self.gridTimeCount_ = 0
        
    def add(self, timeBetweenVisits):
        self.gridTimeSum_ = self.gridTimeSum_ + timeBetweenVisits
        self.gridTimeCount_ = self.gridTimeCount_ + 1
        
    def getMean(self):
        if self.gridTimeCount_ == 0:
            return [[0L]]
        else:
            return [[long(self.gridTimeSum_ / self.gridTimeCount_)]]
        
class MeanGridVisits:
    
    gridVisitsSum_ = 0.0
    gridVisitsCount_ = 0;
    
    def reset(self):
        self.gridVisitsSum_ = 0.0
        self.gridVisitsCount_ = 0
        
    def add(self, gridVisits):
        self.gridVisitsSum_ = self.gridVisitsSum_ + gridVisits
        self.gridVisitsCount_ = self.gridVisitsCount_ + 1
        
    def getMean(self):
        if self.gridVisitsCount_ == 0:
            return [[0.0]]
        else:
            return[[self.gridVisitsSum_ / self.gridVisitsCount_]]

class MeanFinalCoverage:
    
    finalCoverageSum_ = 0.0
    finalTimeSum_ = 0L
    finalCoverageCount_ = 0
    
    def reset(self):
        self.finalCoverageSum_ = 0.0
        self.finalTimeSum_ = 0L
        self.finalCoverageCount_ = 0
        
    def add(self, finalCoverage):
        assert(len(finalCoverage) == 2)
        
        self.finalCoverageSum_ = self.finalCoverageSum_ + finalCoverage[0][0]
        self.finalTimeSum_ = self.finalTimeSum_ + finalCoverage[1][0]
        self.finalCoverageCount_ = self.finalCoverageCount_ + 1
        
    def getMean(self):
        if self.finalCoverageCount_ == 0:
            return [[0.0], [0L]]
        else:
            result = [[], []]
            result[0].append(self.finalCoverageSum_ / self.finalCoverageCount_)
            result[1].append(self.finalTimeSum_ / self.finalCoverageCount_)
            return result
