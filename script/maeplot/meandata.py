from utils import calcStandardDeviation

class MeanTimeEvents:
    
    def __init__(self):
        self.reset()
    
    def reset(self):
        # [timeStamp] (coverageSum, [coverages])
        self.timeEventsData_ = dict()
    
    def add(self, timeEvents):
        assert(len(timeEvents) == 2)
        
        for time in timeEvents[1]:
            if not time in self.timeEventsData_:
                self.timeEventsData_[time] = [0.0, []]
        
        for coverage, time in zip(*timeEvents):
            self.timeEventsData_[time][0] = self.timeEventsData_[time][0] + coverage
            self.timeEventsData_[time][1].append(coverage)
            
    def addMean(self, meanData):
        for time in meanData.timeEventsData_:
            if not time in self.timeEventsData_:
                self.timeEventsData_[time] = [0.0, []]
                
            self.timeEventsData_[time][0] = self.timeEventsData_[time][0] + meanData.timeEventsData_[time][0]
            self.timeEventsData_[time][1] = self.timeEventsData_[time][1] + meanData.timeEventsData_[time][1]
                
    def getMean(self):
        
        # [meanCoverage, time, standardDeviation]
        result = [[],[], []]
        
        for time in sorted(self.timeEventsData_):
            result[1].append(time)
            coverageSum = self.timeEventsData_[time][0]
            count = len(self.timeEventsData_[time][1])
            result[0].append(coverageSum / count)
            standardDeviation = calcStandardDeviation(self.timeEventsData_[time][1], result[0][-1])
            result[2].append(standardDeviation)
            
        return result
    
    def hasData(self):
        return len(self.timeEventsData_) > 0

class MeanCoverageEvents:
    
    def __init__(self):
        self.reset()
    
    def reset(self):
        # [coverage] (timeStampSum, [timeStamps])
        self.coverageEventsData_ = dict()
    
    def add(self, coverageEvents):
        assert(len(coverageEvents) == 2)
        
        for coverage in coverageEvents[0]:
            if not coverage in self.coverageEventsData_:
                self.coverageEventsData_[coverage] = [0L, []]
        
        for coverage, time in zip(*coverageEvents):
            self.coverageEventsData_[coverage][0] = self.coverageEventsData_[coverage][0] + time
            self.coverageEventsData_[coverage][1].append(time)
            
    def addMean(self, meanData):
        for coverage in meanData.coverageEventsData_:
            if not coverage in self.coverageEventsData_:
                self.coverageEventsData_[coverage] = [0L, []]
                
            self.coverageEventsData_[coverage][0] = self.coverageEventsData_[coverage][0] + meanData.coverageEventsData_[coverage][0]
            self.coverageEventsData_[coverage][1] = self.coverageEventsData_[coverage][1] + meanData.coverageEventsData_[coverage][1]
                
    def getMean(self):
        
        # [coverage, meanTime, standardDeviation]
        result = [[],[]]
        for coverage in sorted(self.coverageEventsData_):
            result[0].append(coverage)
            time = self.coverageEventsData_[coverage][0]
            count = len(self.coverageEventsData_[coverage][1])
            result[1].append(time / count)
            standardDeviation = calcStandardDeviation(self.coverageEventsData_[coverage][1], result[0][-1])
            result[2].append(standardDeviation)
            
        return result
    
    def hasData(self):
        return len(self.coverageEventsData_) > 0

class MeanTileTimeBetweenVisits:
    
    def __init__(self):
        self.reset()
    
    def reset(self):
        # [x,y] (timeSum, [times])
        self.tileTimeData_ = dict()
    
    def add(self, tileTimeBetweenVisits):
        assert(len(tileTimeBetweenVisits) == 3)
        
        if len(self.tileTimeData_) == 0:
            for x,y in zip(tileTimeBetweenVisits[0], tileTimeBetweenVisits[1]):
                key = self.valueToKey(x,y)
                self.tileTimeData_[key] = [0L, []]
                
        assert(len(tileTimeBetweenVisits[0]) == len(self.tileTimeData_))
        
        for x,y,tileTime in zip(*tileTimeBetweenVisits):
            key = self.valueToKey(x,y)
            assert(key in self.tileTimeData_)
            if tileTime > 0:
                self.tileTimeData_[key][0] = self.tileTimeData_[key][0] + tileTime
                self.tileTimeData_[key][1].append(tileTime)
    
    def addMean(self, meanData):
        for key in meanData.tileTimeData_:
            if not key in self.tileTimeData_:
                self.tileTimeData_[key] = [0L, []]
                
            self.tileTimeData_[key][0] = self.tileTimeData_[key][0] + meanData.tileTimeData_[key][0]
            self.tileTimeData_[key][1] = self.tileTimeData_[key][1] + meanData.tileTimeData_[key][1]
                               
    def getMeanGrid(self):
        
        # [x, y, meanTileTimeBetweenVisits, standardDeviation]
        result = [[], [], [], []]
        
        for key in sorted(self.tileTimeData_):
            coord = self.keyToValue(key)
            result[0].append(coord[0])
            result[1].append(coord[1])
            values = self.tileTimeData_[key]
            count = len(values[1])
            if count == 0:
                result[2].append(0L)
                result[3].append(0.0)
            else:
                result[2].append(long(values[0] / count))
                standardDeviation = calcStandardDeviation(values[1], result[2][-1])
                result[3].append(standardDeviation)
            
        return result 
    
    def getMeanValue(self):
        
        allTimes = []
        timeSum = 0L
        for values in self.tileTimeData_.itervalues():
            allTimes = allTimes + values[1]
            timeSum = timeSum + values[0]
            
        meanVal = long(timeSum / len(allTimes))
        stdDev = calcStandardDeviation(allTimes, meanVal)
        
        return (meanVal, stdDev)
        
    def valueToKey(self, x, y):
        result = str(x) + "," + str(y)
        return result
    
    def keyToValue(self, key):
        values = key.split(',')
        assert(len(values) == 2)
        
        return (int(values[0]), int(values[1]))
    
    def hasData(self):
        return len(self.tileTimeData_) > 0
    
class MeanTileVisits:
    
    def __init__(self):
        self.reset()
    
    def reset(self):
        # [x,y] (visitsSum, [visits])
        self.tileVisitsData_ = dict()
    
    def add(self, tileVisits):
        assert(len(tileVisits) == 3)
        
        if(len(self.tileVisitsData_) == 0):
            for x,y in zip(tileVisits[0], tileVisits[1]):
                key = self.valueToKey(x, y)
                self.tileVisitsData_[key] = [0, []]
        
        assert(len(tileVisits[0]) == len(self.tileVisitsData_))
        
        for x,y,visits in zip(tileVisits[0], tileVisits[1],tileVisits[2]):
            key = self.valueToKey(x, y)
            
            self.tileVisitsData_[key][0] = self.tileVisitsData_[key][0] + visits
            self.tileVisitsData_[key][1].append(visits)
            
    def addMean(self, meanData):
        for key in meanData.tileVisitsData_:
            if not key in self.tileVisitsData_:
                self.tileVisitsData_[key] = [0L, []]
                
            self.tileVisitsData_[key][0] = self.tileVisitsData_[key][0] + meanData.tileVisitsData_[key][0]
            self.tileVisitsData_[key][1] = self.tileVisitsData_[key][1] + meanData.tileVisitsData_[key][1]
            
    def hasData(self):
        return len(self.tileVisitsData_) > 0
    
    def getMeanGrid(self):
        # [x, y, meanTileVisits, standardDeviation]
        result = [[], [], [], []]
        
        for key in sorted(self.tileVisitsData_):
            coord = self.keyToValue(key)
            result[0].append(coord[0])
            result[1].append(coord[1])
            values = self.tileVisitsData_[key]
            count = len(values[1])
            
            if count == 0:
                result[2].append(0.0)
                result[3].append(0.0)
            else:
                result[2].append(float(values[0]) / float(count))
                standardDeviation = calcStandardDeviation(values[1], result[2][-1])
                result[3].append(standardDeviation)
            
        return result
    
    def getMeanValue(self):
        
        allVisits = []
        visitSum = 0
        for values in self.tileVisitsData_.itervalues():
            allVisits = allVisits + values[1]
            visitSum = visitSum + values[0]
            
        meanVal = long(visitSum / len(allVisits))
        stdDev = calcStandardDeviation(allVisits, meanVal)
        
        return (meanVal, stdDev)
    
    def valueToKey(self, x, y):
        result = str(x) + "," + str(y)
        return result
    
    def keyToValue(self, key):
        values = key.split(',')
        assert(len(values) == 2)
        
        return (int(values[0]), int(values[1]))

class MeanGridTimeBetweenVisits:
    
    def __init__(self):
        self.reset()
    
    def reset(self):
        self.gridTimeSum_ = 0L
        self.gridTimes_ = []
        
    def add(self, timeBetweenVisits):
        self.gridTimeSum_ = self.gridTimeSum_ + timeBetweenVisits
        self.gridTimes_.append(timeBetweenVisits)

    def getMean(self):
        count = len(self.gridTimes_)
        if count == 0:
            return [[0L], [0.0]]
        else:
            meanVal = long(self.gridTimeSum_ / count)
            stdDev = calcStandardDeviation(self.gridTimes_, meanVal)
            return [[meanVal], [stdDev]]
    
    def hasData(self):
        return len(self.gridTimes_) > 0
    
class MeanGridVisits:
    
    def __init__(self):
        self.reset()
    
    def reset(self):
        self.gridVisitsSum_ = 0.0
        self.gridVisits_ = []
        
    def add(self, gridVisits):
        self.gridVisitsSum_ = self.gridVisitsSum_ + gridVisits
        self.gridVisits_.append(gridVisits)
        
    def getMean(self):
        count = len(self.gridVisits_)
        if count == 0:
            return [[0.0], [0.0]]
        else:
            meanVal = self.gridVisitsSum_ / count
            stdDev = calcStandardDeviation(self.gridVisits_, meanVal)
            return[[meanVal], [stdDev]]
    
    def hasData(self):
        return self.gridVisitsCount_ > 0
class MeanFinalCoverage:
    
    def __init__(self):
        self.reset()
    
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
    
    def hasData(self):
        return self.finalCoverageCount_ > 0