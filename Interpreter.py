import sys
from Datatypes import *
class Interpreter:
    
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
            res = 0
            for element in node['arg']:
                val = self.ev(element)
                res += int(val)
            print(res)
            return NumType(res) 


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
            res = 0
            for element in node['arg']:
                val = self.ev(element)
                res = int(val) -res
                res2 = res - int(val)
            print(res2)
            return NumType(res2)


        elif node["type"] == "DIVIDE":
            res = 1
            for element in node['arg']:
                val = self.ev(element)
                res = int(val) / res
            print(res)
            return NumType(res) 