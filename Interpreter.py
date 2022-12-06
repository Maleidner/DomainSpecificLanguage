import sys
from Datatypes import *
import operators

class Interpreter:
    

    def runOperator(self, fct, arg, handling):
        evalArgs = [];

        if handling == "NO":
            for a in arg:
                evalArgs.append(self.ev(a))
            return self.callOperator(fct, evalArgs)

        elif handling == "DEF":
            argsAreLists = False
            listLengths = []
            for a in arg:
                ea = self.ev(a)
                if isinstance(ea, ListType):
                    argsAreLists = True
                    listLengths.append(len(ea.value))
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
                            tmp.append(e[i].value)

                        modifiedArgs.append(tmp)

                result = list()

                for i in range(refLength):
                    callArgs = []
                    for m in modifiedArgs:
                        callArgs.append(m)
                    temp = [callArgs[0][i], callArgs[1][i]]
                    result.append(self.callOperator(fct, temp))
                result2 = []
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
            return StrType (node["value"])

        elif node["type"] == "NUMBER":
            return NumType (node["value"])

        elif node["type"] == "TRUE":
            return BoolType (node["value"])

        elif node["type"] == "FALSE":
            return BoolType (node["value"])

        elif node["type"] == "NULL":
            return BoolType (node["type"])


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
            r = not res
            return BoolType (r)

        #elif node["type"] == "LIST":
         #   return ListType (node["args"])
        elif node["type"] == "LIST":
            list_ = ListType()
            for element in node['args']:
                val = self.ev(element)
                list_.value += [val]
            return list_


        elif node["type"] == "POWER":
            res = self.ev(node['arg'][0])
            for element in node['arg'][1:]:
                val = self.ev(element)
                if type(res) == ListType and type(val) == ListType:
                    for i in range(len(val.value)):
                        res.value[i].value = int(res.value[i]) ** int(val.value[i].value)
                elif type(res) == ListType:
                    for i in range(len(res.value)):
                        res.value[i].value = int(res.value[i]) ** int(val)
                elif type(val) == ListType:
                    temp = res
                    res = val
                    val = temp
                    for i in range(len(res.value)):
                        res.value[i].value = int(res.value[i]) ** int(val)
                else:
                    res.value = int(res.value) ** int(val.value)
                return res.value
            #res = self.ev(node["arg"][0])
            #for element in node['arg'][1:]:
             #   val = self.ev(element)
              #  res = int(res.value) ** int(val)
            #print(res)
            #return NumType(res)  
 

        elif node["type"] == "PLUS":
            return self.runOperator("_plus", node["arg"], "DEF")
          


        if node['type'] == 'TIMES':
            res = self.ev(node['arg'][0])
            for element in node['arg'][1:]:
                val = self.ev(element)
                if type(res) == ListType and type(val) == ListType:
                    for i in range(len(val.value)):
                        res.value[i].value = int(res.value[i]) * int(val.value[i].value)
                elif type(res) == ListType:
                    for i in range(len(res.value)):
                        res.value[i].value = int(res.value[i]) * int(val)
                elif type(val) == ListType:
                    temp = res
                    res = val
                    val = temp
                    for i in range(len(res.value)):
                        res.value[i].value = int(res.value[i]) * int(val)
                else:
                    res.value *= val.value
                return res 


        # elif node["type"] == "TIMES":
        #     res = 1
        #     for element in node['arg']:
        #         val = self.ev(element)
        #         res *= int(val)
        #     print(res)
        #     return NumType(res) 


        elif node["type"] == "MINUS":
            return self.runOperator("_minus", node["arg"], "DEF")


        elif node["type"] == "DIVIDE":
            res = 1
            for element in node['arg']:
                val = self.ev(element)
                res = int(val) / res
            print(res)
            return NumType(res) 


        