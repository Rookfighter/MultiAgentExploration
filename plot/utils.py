import os

def mkdirRec(directory):
    try:
        os.makedirs(directory)
    except OSError as exc: 
        if exc.errno == errno.EEXIST and os.path.isdir(directory):
            pass

def getImmediateSubdirectories(p_dir):
    return [name for name in os.listdir(p_dir)
            if os.path.isdir(os.path.join(p_dir, name))]
            
def getSubdirectoriesConcat(p_dir):
    subDirs = getImmediateSubdirectories(p_dir)
    for i in xrange(0, len(subDirs)):
        subDirs[i] = os.path.join(p_dir, subDirs[i])
    return subDirs