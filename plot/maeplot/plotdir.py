import os
from algorithmdir import AlgorithmDirectory
from utils import getSubdirectoriesConcat

SUMMARY_DIRECTORY = "summary"
PACKAGE_DIRECTORY = "maeplot"

class PlotDirectory(object):
    
    directory_ = ""
    algorithmDirs_ = []
    
    def reset(self):
        self.directory_ = ""
        self.algorithmDirs_ = []
        
    def load(self, directory):
        assert(os.path.isdir(directory))
        
        self.reset()
        self.directory_ = directory
        
        algorithmDirs = getSubdirectoriesConcat(self.directory_)
        
        for algoDir in algorithmDirs:
            if os.path.basename(algoDir) == SUMMARY_DIRECTORY or os.path.basename(algoDir) == PACKAGE_DIRECTORY:
                continue
            
            self.algorithmDirs_.append(AlgorithmDirectory())
            self.algorithmDirs_[-1].load(algoDir)
           
    def save(self):
        for algoDir in self.algorithmDirs_:
            algoDir.save()
    
    def plot(self):
        for algoDir in self.algorithmDirs_:
            algoDir.plot()