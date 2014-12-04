import matplotlib.pyplot as plt

def usecToMin(usec):
    return float(usec / 1000) / 60000

def plotCoverageEventsPerCount(data, outfile):
    plt.cla()
    plt.xlabel('minutes')
    plt.ylabel('coverage')
    plt.title('Coverage per amount of Robots')
    plt.grid(True)
    ax = plt.subplot(111)
    
    for robotCount, meanCoverage in data.iteritems():
        meanData = meanCoverage.getMean()
        meanData[1] = [usecToMin(time) for time in meanData[1]]
        labelText = ""
        
        if robotCount == 1:
            labelText = str(robotCount) + " robot"
        else:
            labelText = str(robotCount) + " robots"
        plt.plot(meanData[1], meanData[0], label=labelText)
        
    #plt.legend(bbox_to_anchor=(1.05, 1), loc=2, borderaxespad=0.)
    # Shrink current axis by 20%
    box = ax.get_position()
    ax.set_position([box.x0, box.y0, box.width * 0.8, box.height])
    
    plt.legend(loc=2, bbox_to_anchor=(1, 1.02))
    plt.savefig(outfile)
        