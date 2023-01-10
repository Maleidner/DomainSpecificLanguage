from Datatypes import *

def _plus(arg1, arg2):
    var = arg1.value + arg2.value
    if arg1.timestamp == arg2.timestamp:
       return NumType(var, timestamp = arg1.timestamp)
    return NumType(var)

def _minus(arg1, arg2):
    var = arg1.value - arg2.value
    if arg1.timestamp == arg2.timestamp:
        return NumType(var, timestamp = arg1.timestamp)
    return NumType(var)

def _uminus(arg1):
    return NumType(-arg1.value)


def _divide(arg1, arg2):
    var = arg1.value / arg2.value
    if arg1.timestamp == arg2.timestamp:
         return NumType(var, timestamp = arg1.timestamp)
    return NumType(var)

def _times(arg1, arg2):
    var = arg1.value * arg2.value
    if arg1.timestamp == arg2.timestamp:
         return NumType(var, timestamp = arg1.timestamp)
    return NumType(var)

def _power(arg1, arg2):
    var = arg1.value ** arg2.value
    if arg1.timestamp == arg2.timestamp:
         return NumType(var, timestamp = arg1.timestamp)
    return NumType(var)

def _lessthan(arg1, arg2):
    correct = False
    if arg1.value < arg2.value:
        correct = True
    return BoolType(correct)

def _greaterthan(arg1, arg2):
    correct = False
    if arg1.value > arg2.value:
        correct = True
    return BoolType(correct)

def _lessthanorequal(arg1, arg2):
    correct = False
    if arg1.value <= arg2.value:
        correct = True
    return BoolType(correct)

def _greaterthan(arg1, arg2):
    correct = False
    if arg1.value >= arg2.value:
        correct = True
    return BoolType(correct)

def _equal(arg1, arg2):
    correct = False
    if arg1.value == arg2.value:
        correct = True
    return BoolType(correct)

def _seqto(arg1, arg2):
    l = []
    for i in range(int(arg1.value), int(arg2.value)+1):
        l.append(i)
    return l

def _maximum(arg1):
    if isinstance(arg1, ListType):
        tmp = []
        for i in arg1.value:
            tmp.append(i.value)
        result = max(tmp)
    else:
        result = arg1
    return NumType(result)


def _minimum(arg1):
    if isinstance(arg1, ListType):
        tmp = []
        for i in arg1.value:
            tmp.append(i.value)
        result = min(tmp)
    else:
        result = arg1
    return NumType(result)

def _first(arg1):
    if isinstance(arg1, ListType):
        result = arg1.value[0].value
    else:
        result = arg1.value
    return NumType(result)

def _last(arg1):
    if isinstance(arg1, ListType):
        length = len(arg1.value)
        result = arg1.value[length-1].value
    else:
        result = arg1.value
    return NumType(result)

def _sum(arg1):
    if isinstance(arg1, ListType):
        result = 0
        for i in arg1.value:
            result = result + i.value
    else:
        result = arg1.value
    return NumType(result)

def _count(arg1):
    if isinstance(arg1, ListType):
        result = len(arg1.value)
    else:
        result = 1
    return NumType(result)

def _timeof(arg1):
    return arg1.timestamp 

def _iswithin(arg1, arg2, arg3):
    if arg1 > arg2 and arg1 < arg3:
        return BoolType(True)
    else:
        return BoolType(False)
