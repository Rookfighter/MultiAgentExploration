import matplotlib.pyplot as plt
import matplotlib.colors as colors
from utils import sameFloat
from experiment import AVAILABLE_WORLDS

def getBarChartColorCyle():
    colorCycle = []
    alphaVal = 0.6
    colConv = colors.ColorConverter()
    colorCycle.append(colConv.to_rgba('red', alphaVal))
    colorCycle.append(colConv.to_rgba('blue', alphaVal))
    colorCycle.append(colConv.to_rgba('orange', alphaVal))
    colorCycle.append(colConv.to_rgba('olive', alphaVal))
    colorCycle.append(colConv.to_rgba('cyan', alphaVal))
    
    return colorCycle

def usecToMin(usec):
    return float(usec / 1000) / 60000

def plotCoverageEventsPerCount(data, outfile):
    plt.figure()
    plt.cla()
    plt.xlabel('minutes')
    plt.ylabel('coverage')
    plt.title('Coverage per amount of Robots')
    
    for robotCount in sorted(data):
        meanData = data[robotCount].getMean()
        meanData[1] = [usecToMin(time) for time in meanData[1]]
        labelText = ""
        
        if robotCount == 1:
            labelText = str(robotCount) + " robot"
        else:
            labelText = str(robotCount) + " robots"
        plt.plot(meanData[1], meanData[0], linestyle='-', marker='s', label=labelText)
    
    plt.legend(loc='lower right')
    plt.savefig(outfile)
    
def plotFinalCoverageBarchart(ylabel, title, data, useTime, outfile):
    colorCycle = getBarChartColorCyle();
    
    availableWorldsTmp = set()
    for worldDict in data.values():
        for worldType in worldDict:
            availableWorldsTmp.add(worldType)
    
    availableWorlds = sorted(availableWorldsTmp)
    width = 1.0 / (len(data) + 2)
    
    for line in [-1, -2]:
        plt.figure()
        plt.cla()
        
        fig, ax = plt.subplots()
        ax.set_ylabel(ylabel)
        
        insertStr = ""
        if useTime:
            coverageVal = worldDict[worldType].getMean()[0][line]
            insertStr = str(int(100 * coverageVal))
        else:
            timeVal = worldDict[worldType].getMean()[1][line]
            insertStr = str(int(usecToMin(timeVal)))
        
        formatedTitle = title.format(insertStr)
        formatedOutfile = outfile.format(insertStr)
        ax.set_title(formatedTitle)
        
        algorithmCount = 0
        for algorithm, worldDict in data.iteritems():
            toPlot = []
            for worldType in availableWorlds:
                if worldType in worldDict:
                    if useTime:
                        dataVal = worldDict[worldType].getMean()[1][line]
                        toPlot.append(usecToMin(dataVal))
                    else:
                        dataVal = worldDict[worldType].getMean()[0][line]
                        toPlot.append(dataVal)
                else:
                    toPlot.append(0.0)
            
            leftBorders = [i + (algorithmCount * width) for i in xrange(len(toPlot))]
            ax.bar(leftBorders, toPlot, width=width, label=algorithm, color=colorCycle[algorithmCount % len(colorCycle)])
            algorithmCount = algorithmCount + 1
            
        ax.set_xticks([(i + (len(availableWorlds) * width) / 2) for i in xrange(len(availableWorlds))])
        ax.set_xticklabels(availableWorlds)
        ax.legend(loc='upper right')
        
        fig.set_size_inches(18.5,10.5)
        plt.savefig(formatedOutfile, dpi=100)
        

def plotTimeToReachCoverage(data, outfile, coverage):
    assert(len(data) > 0)
    assert(len(data.values()[0]) > 0)
    
    colorCycle = getBarChartColorCyle()
    coveragePercent = int(100 * coverage)
    title = "Time to reach {0}% Coverage".format(coveragePercent)
    finalOutfile = outfile.format(coveragePercent)
    yAxLabel = "minutes"
    barWidth = 1.0 / (len(data) + 2)
    
    plt.figure()
    plt.cla()
    fig, ax = plt.subplots()
    ax.set_ylabel(yAxLabel)
    ax.set_title(title)
    
    for algoCount, (algorithmName, worldDict) in enumerate(data.iteritems()):
        algorithmData = []
        
        for worldType in AVAILABLE_WORLDS:
            meanData = worldDict[worldType].getMean()
            found = False
            for i, (coverageEvent, coverageTime) in enumerate(zip(*meanData)):
                # TODO compare float
                if sameFloat(coverageEvent, coverage, 0.01):
                    found = True
                    algorithmData.append(usecToMin(coverageTime))
                    break
                
            if not found:
                algorithmData.append(0)
        
        assert(len(algorithmData) == len(AVAILABLE_WORLDS))    
        leftBorders = [i + (algoCount * barWidth) for i in xrange(len(algorithmData))]
        ax.bar(leftBorders, algorithmData, width=barWidth, label=algorithmName, color=colorCycle[algoCount % len(colorCycle)])
    
    ax.set_xticks([(i + (len(AVAILABLE_WORLDS) * barWidth) / 2) for i in xrange(len(AVAILABLE_WORLDS))])
    ax.set_xticklabels(AVAILABLE_WORLDS)
    ax.legend(loc='upper right')
    
    fig.set_size_inches(18.5,10.5)
    plt.savefig(finalOutfile, dpi=100)
    
def plotCoverageReachedAfterTime(data, outfile, time):
    assert(len(data) > 0)
    assert(len(data.values()[0]) > 0)
    
    colorCycle = getBarChartColorCyle()
    title = "Coverage reached after {0} minutes".format(time)
    finalOutfile = outfile.format(time)
    yAxLabel = "coverage"
    barWidth = 1.0 / (len(data) + 2)
    
    plt.figure()
    plt.cla()
    fig, ax = plt.subplots()
    ax.set_ylabel(yAxLabel)
    ax.set_title(title)
    
    for algoCount, (algorithmName, worldDict) in enumerate(data.iteritems()):
        algorithmData = []
        
        for worldType in AVAILABLE_WORLDS:
            meanData = worldDict[worldType].getMean()
            found = False
            for i, (coverageEvent, coverageTime) in enumerate(zip(*meanData)):
                if int(usecToMin(coverageTime)) == time:
                    found = True
                    algorithmData.append(coverageEvent)
                    break
                
            if not found:
                algorithmData.append(0)
        
        assert(len(algorithmData) == len(AVAILABLE_WORLDS))    
        leftBorders = [i + (algoCount * barWidth) for i in xrange(len(algorithmData))]
        ax.bar(leftBorders, algorithmData, width=barWidth, label=algorithmName, color=colorCycle[algoCount % len(colorCycle)])
    
    ax.set_xticks([(i + (len(AVAILABLE_WORLDS) * barWidth) / 2) for i in xrange(len(AVAILABLE_WORLDS))])
    ax.set_xticklabels(AVAILABLE_WORLDS)
    ax.legend(loc='upper right')
    
    fig.set_size_inches(18.5,10.5)
    plt.savefig(finalOutfile, dpi=100)