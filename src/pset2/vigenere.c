#include<stdio.h>
#include<cs50.h>
#include<ctype.h>
#include<string.h>
#include<math.h>

int main(int argc, char* argv[]){
    if(argc != 2){
        printf("Try again and type in a word in the command prompt\n");
        return 1;
    }
    char* r = argv[1];
    int h = strlen(r);
    for (int g = 0; g < h; g++)
    {
        if(!isalpha(r[g]))
        {
            printf("No numbers or special characters in the command prompt. Try Again.\n");
            return 1;
        }
    }
    int k;
    string x = GetString();
    int p = 0;
    for(int i = 0; i < strlen(x); i++){
        if(isalpha(x[i]))
        {
            if(p > strlen(argv[1]) - 1)
                p = 0;
            k = r[p];
            if(k < 91 && k > 64){
                k = k - 65;
            }
            if(k > 96 && k < 123)
            k = k - 97;
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
            //printf("\nCurrent Position in the argv[%d] is: %c\n", p, r[p]);
            p++;
            }
        else{
            printf("%c", x[i]);
        }
    }
    printf("\n");
    
}