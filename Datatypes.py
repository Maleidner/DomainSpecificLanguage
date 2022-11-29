from datetime import datetime 
class StrType:  
    def __init__(self, value, timestamp=None):
        self.value = value
        self.timestamp = timestamp
    def __str__(self):
        return str(self.value) 


class NumType:  
    def __init__(self, value, timestamp=None):
        self.value = value
        self.timestamp = timestamp
    def __int__(self):
        return int(self.value) 

'''class ListType:  
    def __init__(self, value):
        self.value = value
    #def __list__(self):
    def __str__(self):
        result = []
        for v in self.value:
            result.append(v["value"])
        return str(result)
        #return list(result.value)
'''

class ListType:  
    def __init__(self):
        self.value = []
    def __str__(self):
        res = []
        for element in self.value:
            res.append(element.value)
        return str(res)


class BoolType:
    def __init__(self, value, timestamp=None):
        # bool(value)
        self.value = value
        self.timestamp = timestamp
    def __str__(self):
        return str(self.value) 
    