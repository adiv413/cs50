#include<stdio.h>
#include<cs50.h>
#include<ctype.h>
#include<string.h>
#include<math.h>

int main(int argc, string argv[]){
    if(argc != 2){
        printf("Try again and type in a number in the command prompt\n");
        return 1;
    }
    string x = GetString();
    int k = atoi(argv[1]);
    for(int i = 0; i < strlen(x); i++){
        if(isalpha(x[i])){
        k = k % 26;
        unsigned char v = x[i] + k;
        int y = k / 26;
        char u = v - k;
        if(u < 91 && v > 90){
            v -= ((floor(y) * 26) + 26);
        }
        if((u > 96 && u < 123) && v > 122){
            v -= ((floor(y) * 26) + 26);
        }
        printf("%c", v);
        }
        else{
            printf("%c", x[i]);
        }
    }
    printf("\n");
    
}