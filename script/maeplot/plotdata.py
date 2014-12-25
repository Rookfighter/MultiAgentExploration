import matplotlib.pyplot as plt
import matplotlib.colors as colors
from maeplot.utils import sameFloat, usecToMin, coverageToPercent
from maeplot.experiment import AVAILABLE_WORLDS, AVAILABLE_ALGORITHMS, ALGORITHM_NAMES

class ColorCyle:
    def __init__(self):
        self.current_ = 0
        self.colors_ = []
        alphaVal = 0.6
        colConv = colors.ColorConverter()
        self.colors_.append(colConv.to_rgba('red', alphaVal))
        self.colors_.append(colConv.to_rgba('blue', alphaVal))
        self.colors_.append(colConv.to_rgba('orange', alphaVal))
        self.colors_.append(colConv.to_rgba('olive', alphaVal))
        self.colors_.append(colConv.to_rgba('cyan', alphaVal))
    
    def next(self):
        result = self.colors_[self.current_]
        self.current_ = (self.current_ + 1) % len(self.colors_)
        return result

def plotCoverageEventsPerCount(data, outfile):
    plt.figure()
    plt.cla()
    plt.xlabel('minutes')
    plt.ylabel('coverage')
    plt.title('Coverage per amount of Robots')
    
    for robotCount in sorted(data):
        meanData = data[robotCount].getMean(convertTime=usecToMin, convertCoverage=coverageToPercent)
        labelText = ""
        
        if robotCount == 1:
            labelText = str(robotCount) + " robot"
        else:
            labelText = str(robotCount) + " robots"
        plt.plot(meanData[1], meanData[0], linestyle='-', marker='s', label=labelText)
    
    plt.legend(loc='lower right')
    plt.savefig(outfile)  

def plotBarChartPerAlgorithmPerTerrain(data, dataErr=None, outfile="", yAxLabel="", plotTitle=""):
    assert(len(data) == len(AVAILABLE_ALGORITHMS))
    
    colors = ColorCyle()
    barWidth = 1.0 / (len(data) + 2)
    
    plt.figure()
    plt.cla()
    fig, ax = plt.subplots()
    ax.set_ylabel(yAxLabel)
    ax.set_title(plotTitle)
    
    for algoCount, algoName in enumerate(AVAILABLE_ALGORITHMS):
        algoData = data[algoName]
        algoErr = None
        if dataErr != None:
            algoErr = dataErr[algoName]
        
        leftBorders = [i + (algoCount * barWidth) for i in xrange(len(algoData))]
        rects = ax.bar(leftBorders, algoData, yerr=algoErr, width=barWidth, label=ALGORITHM_NAMES[algoName], color=colors.next())
        # add value label on top of bars
        for rect in rects:
            barHeight = rect.get_height()
            ax.text(rect.get_x() + rect.get_width() / 2.0, 1.0 * barHeight, '%d'%int(barHeight),
                    ha='center', va='bottom')
            
    ax.set_xticks([(i + (len(data) * barWidth) / 2) for i in xrange(len(AVAILABLE_WORLDS))])
    ax.set_xticklabels(AVAILABLE_WORLDS)
    ax.legend(loc='upper right')
    
    fig.set_size_inches(18.5,10.5)
    if len(outfile) > 0:
        plt.savefig(outfile, dpi=100)
    else:
        plt.show()

def plotBarChartPerTerrainPerAlgorithm(data, dataErr=None, outfile="", yAxLabel="", plotTitle=""):
    assert(len(data) == len(AVAILABLE_WORLDS))
    
    colors = ColorCyle()
    barWidth = 1.0 / (len(data) + 2)
    
    plt.figure()
    plt.cla()
    fig, ax = plt.subplots()
    ax.set_ylabel(yAxLabel)
    ax.set_title(plotTitle)
    
    for worldCount, worldName in enumerate(AVAILABLE_WORLDS):
        worldData = data[worldName]
        worldErr = None
        if dataErr != None:
            worldErr = dataErr[worldName]
            
        leftBorders = [i + (worldCount * barWidth) for i in xrange(len(worldData))]
        rects = ax.bar(leftBorders, worldData, yerr=worldErr, width=barWidth, label=worldName, color=colors.next())
        # add value label on top of bars
        for rect in rects:
            barHeight = rect.get_height()
            ax.text(rect.get_x() + rect.get_width() / 2.0, 1.0 * barHeight, '%d'%int(barHeight),
                    ha='center', va='bottom')
            
    ax.set_xticks([(i + (len(data) * barWidth) / 2) for i in xrange(len(AVAILABLE_ALGORITHMS))])
    ax.set_xticklabels(AVAILABLE_ALGORITHMS)
    ax.legend(loc='upper right')
    
    fig.set_size_inches(18.5,10.5)
    if len(outfile) > 0:
        plt.savefig(outfile, dpi=100)
    else:
        plt.show()    
    
def plotTimeToReachCoverage(data, outfile, coverage):
    assert(len(data) > 0)
    assert(len(data.values()[0]) > 0)
    
    coveragePercent = int(100 * coverage)
    title = "Time to reach {0}% Coverage".format(coveragePercent)
    finalOutfile = outfile.format(coveragePercent)
    yAxLabel = "minutes"
    dataPerAlgo = dict()
    errPerAlgo = dict()
    
    for algoName, worldDict in data.iteritems():
        algoData = []
        algoErr = []
        
        for worldType in AVAILABLE_WORLDS:
            meanData = worldDict[worldType].getMean(convertTime=usecToMin)
            found = False
            for coverageEvent, coverageTime, stdDev in zip(*meanData):
                if sameFloat(coverageEvent, coverage, 0.01):
                    found = True
                    algoData.append(coverageTime)
                    algoErr.append(stdDev)
                    break
            if not found:
                algoData.append(0)
                algoErr.append(0.0)
        
        assert(len(algoData) == len(AVAILABLE_WORLDS))    
        dataPerAlgo[algoName] = algoData
        errPerAlgo[algoName] = algoErr
    
    plotBarChartPerAlgorithmPerTerrain(dataPerAlgo, dataErr=errPerAlgo, outfile=finalOutfile, yAxLabel=yAxLabel, plotTitle=title)
    
def plotCoverageReachedAfterTime(data, outfile, time):
    assert(len(data) > 0)
    assert(len(data.values()[0]) > 0)
    
    title = "Coverage reached after {0} minutes".format(time)
    finalOutfile = outfile.format(time)
    yAxLabel = "coverage"
    dataPerAlgo = dict()
    errPerAlgo = dict()
    
    for algoName, worldDict in data.iteritems():
        algoData = []
        algoErr = []
        
        for worldType in AVAILABLE_WORLDS:
            meanData = worldDict[worldType].getMean(convertTime=usecToMin, convertCoverage=coverageToPercent)
            found = False
            for coverageEvent, coverageTime, stdDev in zip(*meanData):
                if int(coverageTime) == time:
                    found = True
                    algoData.append(coverageEvent)
                    algoErr.append(stdDev)
                    break
                
            if not found:
                algoData.append(0)
                algoErr.append(0.0)
        
        assert(len(algoData) == len(AVAILABLE_WORLDS))    
        dataPerAlgo[algoName] = algoData
        errPerAlgo[algoName] = algoErr
        
    plotBarChartPerAlgorithmPerTerrain(dataPerAlgo, dataErr=errPerAlgo, outfile=finalOutfile, yAxLabel=yAxLabel, plotTitle=title)