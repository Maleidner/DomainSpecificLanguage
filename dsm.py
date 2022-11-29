import json


from Tokenizer import Tokenizer
from Compiler import Compiler
from Interpreter import Interpreter

tokenizer = Tokenizer("tokenizertext.txt")
tokenstream = tokenizer.tokenize()
print(str(tokenstream))
ast = Compiler().compile(json.dumps(tokenstream))
print(json.dumps(ast, indent = 4))
Interpreter().ev(ast)

# primitive typ checken ob liste, wenn Liste elementweise mit for loop