import sys

fn = sys.argv[1]

with open(fn, "r") as f:
    s = f.readline()
    c = 0
    while c<len(s):
        print(str(int(c/32+1)) + " " + s[c:c+32])
        c += 32