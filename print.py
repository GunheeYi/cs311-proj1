import sys

fn = sys.argv[1]

with open(fn, "r") as f:
    s = f.readline()
    c = 64
    while c<len(s):
        #str(int(c/32+1)) + " " + 
        print(s[c:c+32])
        c += 32