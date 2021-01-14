#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(void){
    string g = GetString();
    char b = g[0];
    putchar(toupper(b));
    for(int i = 0; i < strlen(g); i++){
        if(g[i] == ' '){
            char c = g[i + 1];
            putchar(toupper(c));
        }
    };
    printf("\n");
 }