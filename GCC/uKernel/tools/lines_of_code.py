import os, sys


def addAllButComments(line):
    numb = 1
    bl = False
    tpl = (bl, numb)
    if (line.find("/*") != -1) and (line.find("*/") == -1):
        addAllButComments.cntr = addAllButComments.cntr + 1
        tpl = (True, 0)
    elif (line.find("*/") != -1) and (line.find("/*") == -1 ):
        numb = addAllButComments.cntr
        addAllButComments.cntr = 0
        tpl = (True, numb)
    elif (line.find("/*") != -1) and (line.find("*/") != -1) and (line.find(";") == -1):
        tpl = (True, 0)
    elif (line.find("//") != -1) and (line.find("//") < line.find(";")):
        tpl = (True, 0)
    else:
        if addAllButComments.cntr:
            addAllButComments.cntr = addAllButComments.cntr + 1
        tpl = (False, 1)

    if not tpl[0]:
        return 1
    elif tpl[1]:
        return 0 - tpl[1]
    else:
        return 0

addAllButComments.cntr = 0

def isNotEmpty(line):
    if not line.strip():
        if line not in ["\n", "\r\r"]:
            return True
    return False        

path = sys.argv[1]
print sys.argv[1]

header_files = [os.path.join(path, name) for name in os.listdir(sys.argv[1])\
                    if name.endswith((".h", ".H"))]
source_files = [os.path.join(path, name) for  name in os.listdir(sys.argv[1])\
                    if name.endswith((".c"))]
assembly_files = [os.path.join(path, name) for name in os.listdir(sys.argv[1])\
                      if name.endswith((".s", ".asm", ".S"))]
print "------------------"
print ".c files:"
print "------------------"
for item in source_files:
    print item
print "------------------"
print ".h files:"
print "------------------"
for item in header_files:
    print item

print "------------------"
print "assembly files:"
print "------------------"
for item in assembly_files:
    print item
print "------------------"
cntr = 0
for item in source_files:
    with open(item) as f:
        if ( "cr_startup_plc17.c" not in item
             and  "system_LPC17xx.c" not in item
             and  "core_cm3.c" not in item):
            print item
            for line in f.readlines():
                cntr = cntr + addAllButComments(line)

for item in header_files:
    with open(item) as f:
        #exceptions
        if (   "core_cm3.h" not in item 
            and  "LPC17xx.h" not in item
            and  "system_LPC17xx.h" not in item):
            print item
            for line in f.readlines():
                cntr = cntr + addAllButComments(line)
for item in assembly_files:
    if "switch.s" in item:
        with open(item) as f:
            print item
            for line in f.readlines():
                cntr = cntr + addAllButComments(line)
             


print "In total: %d"%(cntr), "lines of code"
      
