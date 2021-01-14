#include <stdio.h>
#include <cs50.h>
#include <math.h>
int main(void)
{
	float i;
	int count = 0;
	do
	{
	printf("How much money is owed?");
	i = GetFloat();
	}
	while(i<=0);

	i *= 100 ;
	int x = roundf(i) ;

	while(x >= 25)
	{
	x -= 25;
	count++ ;
	}
	while(x >= 10)
	{
	x -= 10;
	count++;
	}
	while(x >= 5)
	{
	x -= 5;
	count++ ;
	}
	while(x >= 1)
	{
	x -= 1;
	count++ ;
	}
	printf("%d\n", count) ;
}