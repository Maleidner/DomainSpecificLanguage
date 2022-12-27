from Datatypes import *

def _plus(arg1, arg2):
    return NumType(arg1 + arg2)

def _minus(arg1, arg2):
    return NumType(arg1 - arg2)

def _divide(arg1, arg2):
    return NumType(arg1 / arg2)

def _times(arg1, arg2):
    return NumType(arg1 * arg2)

def _power(arg1, arg2):
    return NumType(arg1 ** arg2)

def _lessthan(arg1, arg2):
    correct = False
    if arg1 < arg2:
        correct = True
    return BoolType(correct)

def _greaterthan(arg1, arg2):
    correct = False
    if arg1 > arg2:
        correct = True
    return BoolType(correct)

def _lessthanorequal(arg1, arg2):
    correct = False
    if arg1 <= arg2:
        correct = True
    return BoolType(correct)

def _greaterthan(arg1, arg2):
    correct = False
    if arg1 >= arg2:
        correct = True
    return BoolType(correct)

def _equal(arg1, arg2):
    correct = False
    if arg1 == arg2:
        correct = True
    return BoolType(correct)

def _seqto(arg1, arg2):
    print("hallo")

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
