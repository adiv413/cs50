n = int(input("Height: "))
if n == 0:
    exit(0)
if n < 1 or n > 23:
    print("Retry:")
    n = int(input(""))

x = 2
y = n - 1

for i in range(n):
    print(" " * y, end = "")
    y -= 1;
    print("#" * x, end = "")
    x += 1;
    print("");
exit(0);
