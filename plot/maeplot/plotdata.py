import matplotlib.pyplot as plt
import matplotlib.colors as colors

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
    
def plotFinalCoverageBarchart(ylabel, title, data, dataIndex, outfile):
    plt.figure()
    plt.cla()
    
    fig, ax = plt.subplots()
    ax.set_ylabel(ylabel)
    ax.set_title(title)
    colorCycle = getBarChartColorCyle();
    
    availableWorldsTmp = set()
    for worldDict in data.values():
        for worldType in worldDict:
            availableWorldsTmp.add(worldType)
    
    availableWorlds = sorted(availableWorldsTmp)
    width = 1.0 / (len(data) + 2)
    
    algorithmCount = 0
    for algorithm, worldDict in data.iteritems():
        toPlot = []
        for worldType in availableWorlds:
            if worldType in worldDict:
                dataVal = worldDict[worldType].getMean()[dataIndex][0]
                if dataIndex == 1:
                    toPlot.append(usecToMin(dataVal))
                else:
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
    plt.savefig(outfile, dpi=100)

def plotFinalCoverageTimes(data, outfile):
    plotFinalCoverageBarchart('minutes', 'Time to reach Coverage', data, 1, outfile)
    
def plotFinalCoverage(data, outfile):
    plotFinalCoverageBarchart('coverage', 'Coverage reached after Time', data, 0, outfile)