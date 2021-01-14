#include <stdio.h>
#include <cs50.h>
    
int main(void)
{
    printf("How many minutes do you take a shower for every day:\n");
    int a = GetInt();
    int x = a * 12;
    printf("%i minutes equals %i bottles\n", a, x);
    return 0;
}
