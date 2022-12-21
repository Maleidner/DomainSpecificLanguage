import sys
from Datatypes import *
import operators
symbol_table = {};

class Interpreter:
    
    def __init__(self):
        self.now = TimeType(value = datetime.now())

    def runOperator(self, fct, arg, handling, unary = False):
        evalArgs = [];
        result2 = []

        if handling == "NO":
            for a in arg:
                evalArgs.append(self.ev(a))
            return self.callOperator(fct, evalArgs)

        elif handling == "DEF":
            argsAreLists = False
            listLengths = []


            #TODO
            if unary == True:
                for a in arg:
                    ea = self.ev(a)
                    if isinstance(ea, ListType):
                        return self.callOperator(fct, [ea])
                    else:
                        return self.callOperator(fct, [ea])


            if fct == "_iswithin":
                i = 1
                for a in arg:
                    ea = self.ev(a)
                    if i == 2 and isinstance(ea, ListType):
                        if ea.value[0].value < ea.value[1].value:
                            evalArgs.append(ea.value[0].value)
                        else:
                            evalArgs.append(ea.value[1].value)
                    elif i == 3 and isinstance(ea, ListType):
                        if ea.value[0].value < ea.value[1].value:
                            evalArgs.append(ea.value[1].value)
                        else:
                            evalArgs.append(ea.value[0].value)
                    else:
                        evalArgs.append(ea.value)
                    i = i+1
                
                return self.callOperator(fct, evalArgs)
                
            for a in arg:
                ea = self.ev(a)
                if isinstance(ea, ListType):
                    argsAreLists = True
                    listLengths.append(len(ea.value))
                    evalArgs.append(ea.value)
                elif(type(ea) == list):
                    argsAreLists = True
                    listLengths.append(len(ea))
                    evalArgs.append(ea)
                else:
                    evalArgs.append(ea.value)

            if not argsAreLists:
                return self.callOperator(fct, evalArgs)

            else:
                sameLength = True
                refLength = listLengths[0]
                for l in listLengths:
                    if l != refLength:
                        sameLength = False

                if not sameLength:
                    return NullType()

              #  modifiedArgs = [evalArgs[0]] * refLength
                modifiedArgs = []
                for e in evalArgs:
                    #if type(e) == float or type(e) == int:
                   #     modifiedArgs.append([e] * refLength)
                    if type(e) == float or type(e) == int:
                        modifiedArgs.append([e] * refLength)
                    elif type(e) == list:
                        e = list(e)
                        tmp = []
                        for i in range(len(e)): 
                            tmp.append(e[i])
                        modifiedArgs.append(tmp)

                
                result = list()

                for i in range(refLength):
                    callArgs = []
                    for m in modifiedArgs:
                        callArgs.append(m)
                    temp = [callArgs[0][i], callArgs[1][i]]
                    if type(temp[0]) == NumType:
                        result.append(self.callOperator(fct, [temp[0].value, temp[1]]))
                    elif type(temp[1]) == NumType:
                        result.append(self.callOperator(fct, [temp[0], temp[1].value]))
                    else:
                        result.append(self.callOperator(fct, temp))
                
                for i in range(len(result)):
                    result2.append(result[i].value)
                
                return result2

    def callOperator(self, fct, earg):
        if len(earg) == 1:
            return getattr(operators, fct)(earg[0])
        elif len(earg) == 2:
            return getattr(operators, fct)(earg[0], earg[1])
        elif len(earg) == 3:
             return getattr(operators, fct)(earg[0], earg[1], earg[2])

    # Evaluiert einen Knoten im AST
    def ev(self, node):
        if node["type"] == "STATEMENTBLOCK":
            for s in node["statements"]:
                self.ev(s)
                
        elif node["type"] == "WRITE":
            print (self.ev(node["arg"])) 
 
        elif node["type"] == "STRTOKEN":
            return StrType (node["value"], timestamp = datetime.now())

        elif node["type"] == "NUMBER":
            return NumType (node["value"], timestamp = datetime.now())

        elif node["type"] == "TRUE":
            return BoolType (node["value"], timestamp = datetime.now())

        elif node["type"] == "FALSE":
            return BoolType (node["value"], timestamp = datetime.now())

        elif node["type"] == "NULL":
            return BoolType (node["type"], timestamp = datetime.now())
        
       

        elif node["type"] == "AND":
            res = self.ev(node["arg"][0])
            for element in node['arg'][1:]:
                r = res and self.ev(element)
            return BoolType (r)

        elif node["type"] == "OR":
            res = self.ev(node["arg"][0])
            for element in node['arg'][1:]:
                r = res or self.ev(element)
            return BoolType (r)


        elif node["type"] == "NOT":
            res = self.ev(node["arg"][0])
            r = not eval(res.value)
            return BoolType (r)

        #elif node["type"] == "LIST":
         #   return ListType (node["args"])
        elif node["type"] == "LIST":
            list_ = ListType()
            for element in node['args']:
                val = self.ev(element)
                list_.value += [val]
            return list_


        elif node["type"] == "NOW":
            return self.now


        elif node["type"] == "POWER":
            return self.runOperator("_power", node["arg"], "DEF")
 

        elif node["type"] == "PLUS":
            return self.runOperator("_plus", node["arg"], "DEF")
          
        elif node["type"] == "MAXIMUM":
            return self.runOperator("_maximum", node["arg"], "DEF", True)

        elif node["type"] == "MINIMUM":
            return self.runOperator("_minimum", node["arg"], "DEF", True)

        elif node["type"] == "FIRST":
            return self.runOperator("_first", node["arg"], "DEF", True)

        elif node["type"] == "LAST":
            return self.runOperator("_last", node["arg"], "DEF", True)
        
        elif node["type"] == "SUM":
            return self.runOperator("_sum", node["arg"], "DEF", True)

        elif node["type"] == "COUNT":
            return self.runOperator("_count", node["arg"], "DEF", True)

        elif node["type"] == "TIMEOF":
            val = self.ev(node["arg"])
            if type(val) == ListType:
                timestamp_list = ListType()
                for i in val.value:
                    timestamp_list.value += [TimeType(i.timestamp)]
                return timestamp_list
            else:
                return TimeType(value=val.timestamp)

        elif node["type"] == "TIMEASSIGNMENT":
            ts = self.ev(node["arg"])
          #  if (not isinstance(ts, TimeType)):
           #     raise "TIMEASSIGNMENT"
            symbol_table[node.get("varname")].timestamp = ts.value

        elif node['type'] == 'TIMES':
            return self.runOperator("_times", node["arg"], "DEF")


        elif node["type"] == "MINUS":
            return self.runOperator("_minus", node["arg"], "DEF")


        elif node["type"] == "DIVIDE":
            return self.runOperator("_divide", node["arg"], "DEF")


        elif node["type"] == "ASSIGN":
            symbol_table[node.get("varname")] = self.ev(node.get("arg"))

        elif node["type"] == "VARIABLE":
            return symbol_table.get(node.get("name"), NullType)

        elif node["type"] == "LT":
            return self.runOperator("_lessthan", node["arg"], "DEF")

        elif node["type"] == "ISWITHIN":
            return self.runOperator("_iswithin", node["arg"], "DEF")