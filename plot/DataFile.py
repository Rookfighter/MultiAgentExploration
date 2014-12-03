import os

class DataFile:
    loadedFrom_ = ""
    comments_ = []
    data_ = []
    
    def load(self, filepath):
        assert(os.path.isfile(filepath))
        self.reset()
        self.loadedFrom_ = filepath
        with open(self.loadedFrom_, 'r') as f:
            columnCount = -1
            for line in f:
                # only lines that are no comments are valid
                if line[0] == '#':
                    self.comments_.append(line)
                else:
                    values = line.split(' ')
                    if columnCount < 0:
                        columnCount = len(values)
                        
                    assert(len(values) == columnCount)
                    
                    # add data strings to correct column
                    self.data_.append(tuple(values))
    
    def save(self, filepath):
        with open(filepath, 'w') as f:
            commentContent = ""
            for comment in self.comments_:
                commentContent += comment + "\n"
            commentContent.strip()
            f.write(commentContent)
            if self.data_:
                dataContent = ""
                for line in data_:
                    for value in line:
                        dataContent += value + " "
                    dataContent.strip()
                    dataContent += "\n"
                dataContent.strip()
                f.write(dataContent) 
    
    def columnAsLong(self, column):
        assert(len(self.data_) > 0 and column < len(self.data_[0]))
        result = []
        
        for line in self.data_:
            result.append(long(float(line[column])))
        
        return result

    def columnAsFloat(self, column):
        assert(len(self.data_) > 0 and column < len(self.data_[0]))
        result = []
        
        for line in self.data_:
            result.append(float(line[column]))
        
        return result
        
    def addLongColumn(self, longColumn):
        if len(self.data_) == 0:
            for value in longColumn:
                self.data_.append((str(value),))
        else:
            assert(len(self.data_) == len(longColumn))
            for value, line in zip(longColumn, self.data_):
                line += (str(value),)
   
    def addFloatColumn(self, floatColumn):
        if len(self.data_) == 0:
            for value in floatColumn:
                self.data_.append((repr(value),))
        else:
            assert(len(self.data_) == len(floatColumn))
            for value, line in zip(floatColumn, self.data_):
                line += (repr(value),)
        
    def addComment(self, comment):
        self.comments_.append(comment)
        
    def reset(self):
        self.data_ = []
        self.comments_ = []
        self.filepath_ = ""