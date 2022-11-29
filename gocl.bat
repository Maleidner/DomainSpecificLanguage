cls

lemon -s grammar.y

del parse.cpp

copy grammar.c parse.cpp

cl /EHsc /O2 parse.cpp cjson.cpp




