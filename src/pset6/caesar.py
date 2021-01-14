import sys
from math import *
if len(sys.argv) != 2:
    print("Try again and type in a number in the command prompt")
    exit(1)

x = input("")
k = int(sys.argv[1])
for i in range(len(x)):
    if x[i].isalpha():
        k = k % 26
        v = chr(ord(x[i]) + k)
        y = k / 26
        u = chr(ord(v) - k)
        if ord(u) < 91 and ord(v) > 90:
            v = chr(ord(v) - ((floor(y) * 26) + 26))
        if (ord(u) > 96 and ord(u) < 123) and ord(v) > 122:
            v = chr(ord(v) - ((floor(y) * 26) + 26))
        print(v, end = "")
    else:
        print(x[i], end = "")
print("")
