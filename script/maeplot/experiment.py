
AVAILABLE_WORLDS = ["open", "obstructed", "office"]
WORLD_NAMES = {"open":"Open",
               "obstructed":"Obstructed",
               "office":"Office"}
AVAILABLE_ROBOT_COUNTS = [1, 2, 4, 8]
AVAILABLE_ALGORITHMS = ["randomwalk", "nodecounting", "lrtastar", "wagner", "compass"]
ALGORITHM_NAMES = {"randomwalk":"Randomwalk",
                   "nodecounting":"Nodecounting",
                   "lrtastar":"LRTA*",
                   "wagner":"Wagner",
                   "compass":"Batalin-Sukhatme"}

def getAlgorithmNames():
    result = []
    for algo in AVAILABLE_ALGORITHMS:
        result.append(ALGORITHM_NAMES[algo])
        
    return result

def getWorldNames():
    result = []
    for world in AVAILABLE_WORLDS:
        result.append(WORLD_NAMES[world])
        
    return result