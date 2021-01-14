from cs50 import get_float
count = 0
i = get_float("How much money is owed: ")
if i >= 1:
	i = get_float("How much money is owed: ")
	i *= 100
	x = round(i)
i *= 100
x = round(i)
while(x >= 25):
	x -= 25
	count += 1

while(x >= 10):

	x -= 10
	count += 1

while(x >= 5):

	x -= 5;
	count += 1

while(x >= 1):

	x -= 1;
	count += 1

print(count)
