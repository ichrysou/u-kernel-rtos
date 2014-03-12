import os, shutil

files = [f for f in os.listdir("./") if os.path.isfile(f)]
for f in files:
    print f
    if f.endswith(".c") or f.endswith(".h"):
        shutil.copy(f, "./src")

    
    
