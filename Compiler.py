import json
import subprocess
import os

class Compiler:
    def compile (self, tokenstream):
        result = subprocess.run(['parse.exe'], input=tokenstream.encode('utf-8'),stdout=subprocess.PIPE) 
        output_as_string = result.stdout.decode("utf-8")
        return (json.loads(output_as_string))