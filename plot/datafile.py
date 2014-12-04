import os

class DataFile:
    loadedFrom_ = ""
    comments_ = []
    data_ = []
    
    def reset(self):
        self.data_ = []
        self.comments_ = []
        self.loadedFrom_ = ""
    
    def load(self, filepath):
        assert(os.path.isfile(filepath))
        self.reset()
        self.loadedFrom_ = filepath
        with open(self.loadedFrom_, 'r') as f:
            for line in f:
                # only lines that are no comments are valid
                if line[0] == '#':
                    self.comments_.append(line)
                else:
                    values = line.split(' ')
                    
                    if len(self.data_) == 0:
                        while len(self.data_) < len(values):
                            self.data_.append([])
                            
                    assert(len(values) == len(self.data_))
                    
                    # add data strings to correct column
                    for value, dataList in zip(values, self.data_):
                        dataList.append(value)
    
    def save(self, filepath):
        with open(filepath, 'w') as f:
            # add comments in top of file
            commentContent = ""
            for comment in self.comments_:
                commentContent += comment + "\n"
            commentContent.strip()
            f.write(commentContent)
            # after comments add data
            if self.data_:
                dataContent = ""
                for line in zip(*self.data_):
                    for value in line:
                        dataContent += value + " "
                    dataContent.strip()
                    dataContent += "\n"
                dataContent.strip()
                f.write(dataContent)
    
    def getDataAs(self, formatStr):
        assert(len(self.data_) == len(formatStr))
        
        result = []
        while len(result) < len(self.data_):
            result.append([])
        
        for fmt, dataColumn, resultColumn in zip(formatStr, self.data_, result):
            for value in dataColumn:
                resultColumn.append(self.strToNum(value, fmt))
        
        return result;
    
    def setDataAs(self, data, formatStr):
        assert(len(data) == len(formatStr))
        
        self.data_ = []
        while len(self.data_) < len(data):
            self.data_.append([])
        
        for fmt, dataColumn, toSetColumn in zip(formatStr, self.data_, data):
            for value in toSetColumn:
                dataColumn.append(self.numToStr(value, fmt))
        
    def strToNum(self, numString, fmt):
        if fmt == 'f':
            return float(numString)
        elif fmt == 'l':
            return long(float(numString))
        else:
            raise ValueError("unknown format '" + format + "'")
        
    def numToStr(self, num, fmt):
        if fmt == 'f':
            return repr(num)
        elif fmt == 'l':
            return str(num)
        else:
            raise ValueError("unknown format '" + format + "'")
        
    def addComment(self, comment):
        self.comments_.append(comment)