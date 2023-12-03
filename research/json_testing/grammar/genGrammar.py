"""
TO BE RUN FROM PROJECT ROOT
Generates a JSON grammar using the given width for the arrays and objects
- <width> the number of elements allowed in the generated grammar's width and depth
"""
import sys
import functools

base = """ 

/** Taken from "The Definitive ANTLR 4 Reference" by Terence Parr */

// Derived from https://json.org
grammar JSON_%d;

json
   : value EOF
   ;

obj
   : %s
   | '{' '}'
   ;

pair
   : string ':' value
   ;

arr
   : %s
   | '[' ']'
   ;

string
   : '"a"'
   | '"b"'
   ;

number
   : '0'
   | '1'
   ;

value
   : string
   | number
   | obj
   | arr
   | 'true'
   | 'false'
   | 'null'
   ;

"""

# Generate the new string based on a given width parameter.
assert len(sys.argv) > 1
width = int(sys.argv[1])

# build out obj and arr strings
objstr = ""
arrstr = ""
tmpobj = []
tmparr = []
if width >= 1:
   objstr = "\'{\' pair "
   arrstr = "\'[\' value "
   tmpobj.append(objstr + "\'}\'")
   tmparr.append(arrstr + "\']\'")
   for i in range(width - 1):
      objstr = objstr + "\',\' pair "
      arrstr = arrstr + " \',\' value "
      tmpobj.append(objstr + "\'}\'")
      tmparr.append(arrstr + "\']\'")
# Construct final string in decreasing width order
tmpobj.reverse()
tmparr.reverse()
objstr = functools.reduce(lambda l , r: l + "\n   | " + r, tmpobj)
arrstr = functools.reduce(lambda l , r: l + "\n   | " + r, tmparr)

# Build out grammar string and write to a file
grammar = base % (width, objstr, arrstr)
with open("json_testing/grammar/JSON_%d.g4" % width, 'w') as f:
    f.write(grammar)
    

