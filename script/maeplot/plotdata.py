import matplotlib.pyplot as plt
import matplotlib.colors as colors
from maeplot.utils import sameFloat, coverageToPercent, msecToMin
from maeplot.experiment import AVAILABLE_WORLDS, AVAILABLE_ALGORITHMS, ALGORITHM_NAMES,\
    getAlgorithmNames, AVAILABLE_ROBOT_COUNTS
import os

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
    plt.figure("coverage-events-per-count")
    plt.clf()
    plt.xlabel('minutes')
    plt.ylabel('coverage')
    plt.title('Coverage per amount of Robots')
    
    for robotCount in sorted(data):
        meanData = data[robotCount].getMean(convertTime=msecToMin, convertCoverage=coverageToPercent)
        labelText = ""
        
        if robotCount == 1:
            labelText = str(robotCount) + " robot"
        else:
            labelText = str(robotCount) + " robots"
        plt.plot(meanData[1], meanData[0], linestyle='-', marker='s', label=labelText)
    
    plt.legend(loc='lower right')
    plt.savefig(outfile, dpi=100)  

def plotBarChartPerAlgorithmPerTerrain(data, dataErr=None, outfile="", yAxLabel="", plotTitle="", maxVal=0, legendPos='upper right'):
    assert(len(data) == len(AVAILABLE_ALGORITHMS))
    
    colors = ColorCyle()
    barWidth = 1.0 / (len(data) + 1)
    
    fig, ax = plt.subplots()
    ax.set_ylabel(yAxLabel)
    ax.set_title(plotTitle)
    
    if maxVal > 0:
        ax.set_autoscaley_on(False)
        ax.set_ylim([0,maxVal])
    
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
            ax.text(rect.get_x() + rect.get_width() / 2.0, 1.0 * barHeight, '%.1f'%barHeight,
                    ha='center', va='bottom')
            
    ax.set_xticks([(i + (len(data) * barWidth) / 2) for i in xrange(len(AVAILABLE_WORLDS))])
    ax.set_xticklabels(AVAILABLE_WORLDS)
    ax.legend(loc=legendPos)
    
    fig.set_size_inches(9.6,5.4)
    
    if len(outfile) > 0:
        plt.savefig(outfile, dpi=100)
    else:
        plt.show()

def plotBarChartPerTerrainPerAlgorithm(data, dataErr=None, outfile="", yAxLabel="", plotTitle="", maxVal=0, legendPos='upper right'):
    assert(len(data) == len(AVAILABLE_WORLDS))
    
    colors = ColorCyle()
    barWidth = 1.0 / (len(data) + 1)
    
    fig, ax = plt.subplots()
    ax.set_ylabel(yAxLabel)
    ax.set_title(plotTitle)
    
    if maxVal > 0:
        ax.set_autoscaley_on(False)
        ax.set_ylim([0,maxVal])
    
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
            ax.text(rect.get_x() + rect.get_width() / 2.0, 1.0 * barHeight, '%.1f'%barHeight,
                    ha='center', va='bottom')
            
    ax.set_xticks([(i + (len(data) * barWidth) / 2) for i in xrange(len(AVAILABLE_ALGORITHMS))])
    ax.set_xticklabels(getAlgorithmNames())
    ax.legend(loc=legendPos)
    
    fig.set_size_inches(9.6,5.4)
    
    if len(outfile) > 0:
        plt.savefig(outfile, dpi=100)
    else:
        plt.show()    

def plotBarChartPerRobotCountPerAlgorithm(data, dataErr=None, outfile="", yAxLabel="", plotTitle="", maxVal=0, legendPos='upper right'):
    assert(len(data) == len(AVAILABLE_ROBOT_COUNTS))
    
    colors = ColorCyle()
    barWidth = 1.0 / (len(data) + 1)
    
    fig, ax = plt.subplots()
    ax.set_ylabel(yAxLabel)
    ax.set_title(plotTitle)
    
    if maxVal > 0:
        ax.set_autoscaley_on(False)
        ax.set_ylim([0,maxVal])
    
    for robotNum, robotCount in enumerate(AVAILABLE_ROBOT_COUNTS):
        robotCountData = data[robotCount]
        robotCountErr = None
        if dataErr != None:
            robotCountErr = dataErr[robotCount]
            
        leftBorders = [i + (robotNum * barWidth) for i in xrange(len(robotCountData))]
        
        labelStr = ""
        if robotCount == 1:
            labelStr = "1 Robot"
        else:
            labelStr = str(robotCount) + " Robots"
        rects = ax.bar(leftBorders, robotCountData, yerr=robotCountErr, width=barWidth, label=labelStr, color=colors.next())
        # add value label on top of bars
        for rect in rects:
            barHeight = rect.get_height()
            ax.text(rect.get_x() + rect.get_width() / 2.0, 1.0 * barHeight, '%.1f'%barHeight,
                    ha='center', va='bottom')
            
    ax.set_xticks([(i + (len(data) * barWidth) / 2) for i in xrange(len(AVAILABLE_ALGORITHMS))])
    ax.set_xticklabels(getAlgorithmNames())
    ax.legend(loc=legendPos)
    
    fig.set_size_inches(9.6,5.4)
        
    if len(outfile) > 0:
        plt.savefig(outfile, dpi=100)
    else:
        plt.show() 

def plotTimeToReachCoverage(data, outdir, coverageToPlot):
    assert(len(data) > 0)
    assert(len(data.values()[0]) > 0)
    
    coveragePercent = int(coverageToPercent(coverageToPlot))
    TIME_TO_REACH_COVERAGE_FILE = "time-to-reach-coverage-{0}.png".format(coveragePercent)
    
    dataPerAlgo = dict()
    errPerAlgo = dict()
    
    for algoName, worldDict in data.iteritems():
        algoData = []
        algoErr = []
        
        for worldType in AVAILABLE_WORLDS:
            meanData = worldDict[worldType].getMean(convertTime=msecToMin)
            found = False
            for coverage, coverageTime, stdDev in zip(*meanData):
                if sameFloat(coverage, coverageToPlot, 0.01):
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
        
    title = "Time to reach {0}% Coverage".format(coveragePercent)
    outfile = os.path.join(outdir, TIME_TO_REACH_COVERAGE_FILE)
    yAxLabel = "minutes"
    plotBarChartPerAlgorithmPerTerrain(dataPerAlgo, dataErr=errPerAlgo, outfile=outfile, yAxLabel=yAxLabel, plotTitle=title, maxVal=270, legendPos='upper left')

def plotCoverageReachedAfterTime(data, outdir, time):
    assert(len(data) > 0)
    assert(len(data.values()[0]) > 0)
    
    COVERAGE_REACHED_AFTER_TIME_FILE = "coverage-reached-after-time-{0}.png"
    
    dataPerAlgo = dict()
    errPerAlgo = dict()
    
    for algoName, worldDict in data.iteritems():
        algoData = []
        algoErr = []
        
        for worldName in AVAILABLE_WORLDS:
            meanData = worldDict[worldName].getMean(convertTime=msecToMin, convertCoverage=coverageToPercent)
            found = False
            # search for time in mean values
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
    
    title = "Coverage reached after {0} minutes".format(time)
    outfile = os.path.join(outdir, COVERAGE_REACHED_AFTER_TIME_FILE).format(time)
    yAxLabel = "coverage"
    plotBarChartPerAlgorithmPerTerrain(dataPerAlgo, dataErr=errPerAlgo, outfile=outfile, yAxLabel=yAxLabel, plotTitle=title, maxVal=150)

def plotNumberOfVisits(data, outdir, coverageToPlot):
    assert(len(data) > 0)
    
    coveragePercent = int(coverageToPercent(coverageToPlot))
    MEAN_NUMBER_OF_VISITS_FILE = "number-of-visits-mean-to-{0}-coverage.png".format(coveragePercent)
    STD_DEV_NUMBER_OF_VISITS_FILE = "number-of-visits-standard-deviation-to-{0}-coverage.png".format(coveragePercent)
    
    dataPerWorld = dict()
    errPerWorld = dict()
    for worldName, algoDict in data.iteritems():
        worldData = []
        worldErr = []
        
        for algoName in AVAILABLE_ALGORITHMS:
            meanData = algoDict[algoName].getMean()
            found = False
            for coverage, visits, stdDev in zip(*meanData):
                if sameFloat(coverageToPlot, coverage, 0.01):
                    worldData.append(visits)
                    worldErr.append(stdDev)
                    found = True
                    break
            if not found:
                worldData.append(0.0)
                worldErr.append(0.0)
            
        dataPerWorld[worldName] = worldData
        errPerWorld[worldName] = worldErr
    
    outfile = os.path.join(outdir, MEAN_NUMBER_OF_VISITS_FILE)
    title = "Mean Number of Visits to {0}% Coverage".format(coveragePercent)
    yAxisLabel = "visits"   
    plotBarChartPerTerrainPerAlgorithm(dataPerWorld, dataErr=None, outfile=outfile, yAxLabel=yAxisLabel, plotTitle=title)
    
    outfile = os.path.join(outdir, STD_DEV_NUMBER_OF_VISITS_FILE)
    title = "Standard Deviation of Number of Visits to {0}% Coverage".format(coveragePercent)
    yAxisLabel = "standard deviation"
    plotBarChartPerTerrainPerAlgorithm(errPerWorld, dataErr=None, outfile=outfile, yAxLabel=yAxisLabel, plotTitle=title)
    
def plotTimeBetweenVisits(data, outdir, coverageToPlot):
    assert(len(data) > 0)
    
    coveragePercent = int(coverageToPercent(coverageToPlot))
    MEAN_NUMBER_OF_VISITS_FILE = "time-between-visits-mean-to-{0}-coverage.png".format(coveragePercent)
    STD_DEV_NUMBER_OF_VISITS_FILE = "time-between-visits-standard-deviation-to-{0}-coverage.png".format(coveragePercent)
    
    dataPerWorld = dict()
    errPerWorld = dict()
    for worldName, algoDict in data.iteritems():
        worldData = []
        worldErr = []
        
        for algoName in AVAILABLE_ALGORITHMS:
            meanData = algoDict[algoName].getMean(convertTime=msecToMin)
            found = False
            for coverage, time, stdDev in zip(*meanData):
                if sameFloat(coverageToPlot, coverage, 0.01):
                    worldData.append(time)
                    worldErr.append(stdDev)
                    found = True
                    break
            
            if not found:
                worldData.append(0)
                worldErr.append(0.0)
                
        dataPerWorld[worldName] = worldData
        errPerWorld[worldName] = worldErr
    
    outfile = os.path.join(outdir, MEAN_NUMBER_OF_VISITS_FILE)
    title = "Mean Time between Visits to {0}% Coverage".format(coveragePercent)
    yAxisLabel = "minutes"   
    plotBarChartPerTerrainPerAlgorithm(dataPerWorld, dataErr=None, outfile=outfile, yAxLabel=yAxisLabel, plotTitle=title, maxVal=13)
    
    outfile = os.path.join(outdir, STD_DEV_NUMBER_OF_VISITS_FILE)
    title = "Standard Deviation of Time between Visits to {0}% Coverage".format(coveragePercent)
    yAxisLabel = "standard deviation"
    plotBarChartPerTerrainPerAlgorithm(errPerWorld, dataErr=None, outfile=outfile, yAxLabel=yAxisLabel, plotTitle=title, maxVal=13)
    
def plotTimeToReachCoveragePerRobotCount(data, outdir, coverageToPlot):
    assert(len(data) > 0)
    assert(len(data.values()[0]) > 0)
    
    coveragePercent = int(coverageToPercent(coverageToPlot))
    TIME_TO_REACH_COVERAGE_FILE = "time-to-reach-coverage-{0}-per-robot-count.png".format(coveragePercent)
    
    dataPerRobotCount = dict()
    errPerRobotCount = dict()
    
    for robotCount, algoDict in data.iteritems():
        robotCountData = []
        robotCountErr = []
        
        for algorithm in AVAILABLE_ALGORITHMS:
            meanData = algoDict[algorithm].getMean(convertTime=msecToMin)
            found = False
            for coverage, coverageTime, stdDev in zip(*meanData):
                if sameFloat(coverage, coverageToPlot, 0.01):
                    found = True
                    robotCountData.append(coverageTime)
                    robotCountErr.append(stdDev)
                    break
            if not found:
                robotCountData.append(0)
                robotCountErr.append(0.0)
        
        assert(len(robotCountData) == len(AVAILABLE_ALGORITHMS))    
        dataPerRobotCount[robotCount] = robotCountData
        errPerRobotCount[robotCount] = robotCountErr
        
    title = "Time to reach {0}% Coverage".format(coveragePercent)
    outfile = os.path.join(outdir, TIME_TO_REACH_COVERAGE_FILE)
    yAxLabel = "minutes"
    plotBarChartPerRobotCountPerAlgorithm(dataPerRobotCount, dataErr=errPerRobotCount, outfile=outfile, yAxLabel=yAxLabel, plotTitle=title, maxVal=320)
    
def plotCoverageReachedAfterTimePerRobotCount(data, outdir, time):
    assert(len(data) > 0)
    assert(len(data.values()[0]) > 0)
    
    COVERAGE_REACHED_AFTER_TIME_FILE = "coverage-reached-after-time-{0}-per-robot-count.png"
    
    dataPerRobotCount = dict()
    errPerRobotCount = dict()
    
    for robotCount, algoDict in data.iteritems():
        robotCountData = []
        robotCountErr = []
        
        for algorithm in AVAILABLE_ALGORITHMS:
            meanData = algoDict[algorithm].getMean(convertTime=msecToMin, convertCoverage=coverageToPercent)
            found = False
            # search for time in mean values
            for coverageEvent, coverageTime, stdDev in zip(*meanData):
                if int(coverageTime) == time:
                    found = True
                    robotCountData.append(coverageEvent)
                    robotCountErr.append(stdDev)
                    break
            if not found:
                robotCountData.append(0)
                robotCountErr.append(0.0)
        
        assert(len(robotCountData) == len(AVAILABLE_ALGORITHMS))    
        dataPerRobotCount[robotCount] = robotCountData
        errPerRobotCount[robotCount] = robotCountErr
    
    title = "Coverage reached after {0} minutes".format(time)
    outfile = os.path.join(outdir, COVERAGE_REACHED_AFTER_TIME_FILE).format(time)
    yAxLabel = "coverage"
    plotBarChartPerRobotCountPerAlgorithm(dataPerRobotCount, dataErr=errPerRobotCount, outfile=outfile, yAxLabel=yAxLabel, plotTitle=title, maxVal=150)