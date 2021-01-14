/**
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    // ensure proper usage
    typedef uint8_t BYTE;
    BYTE buffer[512];
    int count = 0;
    FILE *file;
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }


    // open input file 
    file = fopen("card.raw", "r");
    FILE *img = NULL;
    if (file == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 2;
    }
    
    while(fread(buffer, 512, 1, file))
    {
        if ((buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] == 0xe0 || buffer[3] == 0xe1)))
        {
            char filename[8];
            sprintf(filename, "%03d.jpg", count);
            
            img = fopen(filename, "w");
            count ++;
            
        }
    if (img != NULL)    
        fwrite(buffer, 512, 1, img);
    }
    // close infile
    fclose(file);
    fclose(img);

    // success
    return 0;
}
