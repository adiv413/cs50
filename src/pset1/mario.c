#include <stdio.h>
#include <cs50.h>

int main(void){
    printf("Height:\n");
    int n = get_int();
    if(n == 0){
        return 0;
    };
    if(n < 1 || n > 23){
        printf("Retry:");
        n = get_int();
    };

    int x = 2;
    int y = n - 1;

    for (int i = 0; i < n; i++){
        for (int ii = 0; ii < y; ii++)
            printf(" ");
            y--;
        for (int b = 0; b < x; b++){
            printf("#");
        };
        x++;
        printf("\n");
    }
    return 0;
}