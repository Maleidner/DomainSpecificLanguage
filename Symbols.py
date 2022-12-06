from Datatypes import NullType

class Symbols:
    def __init__(self):
        self.symbols = {}
    
    def setSymbol(self, varname, value):
        self.symbols[varname] = value
    
    def getSymbols(self, varname):
        if not self.symbols.__contains__(varname):
            self.symbols[varname] = NullType()
        return self.symbols[varname]