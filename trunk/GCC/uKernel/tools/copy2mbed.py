import sys, os
from shutil import copy

if len(sys.argv) is not 3:
    print "-------------------------"
    print "Wrong number of arguments"
    print "-------------------------"
    print """Proper usage:
#> python copy2mbed.py <src path> <mbed root drive letter>
"""
#    sys.exit(1)

hexFile = sys.argv[1] if(len(sys.argv) > 1) else "../uKernel/Debug/bin/uKernel.hex "
mbedRoot = sys.argv[2] if(len(sys.argv) > 2) else "F:"

    
if not os.path.exists(hexFile):
    print "Error: hex file path does not  exist!"
    
elif not os.path.exists(mbedRoot):
    print "Error: mbed drive letter incorrect or mbed not connected!"

    
print hexFile
print mbedRoot

copy(hexFile, mbedRoot)

