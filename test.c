#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef uint8_t BYTE;

typedef struct //move at the end
{
    BYTE byte;
}
BLOCK;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover infile\n" );
        return 1;
    }

    char *infile = argv[1];

    FILE *inptr = fopen(infile, "r");
    if(inptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    BLOCK *memory = malloc(sizeof(char) * 512);
    char namefile[sizeof "%03i.jpg"];
    int k = 0;

    do
    {
        fread(memory, (sizeof(char) * 512), 1, inptr); //check if end of file?
        if(memory == NULL)
        {
            fprintf(stderr, "Could not allocate memory.\n");
            return 4;
        }

        //compares members of structure to the first 4 values of jpg
        if (memory[0].byte == 0xff &&  memory[1].byte == 0xd8 && memory[2].byte == 0xff && (memory[3].byte & 0xf0) == 0xe0)
        {
            sprintf(infile, "%03i.jpg", 2); //infile or img? or a new char array of [8]?

            FILE *img = fopen(namefile, "w"); //namefile not infile coz we tryna create a new file aka jpg for each image
            if(img == NULL)
            {
                fclose(img);
                fprintf(stderr, "Could not open. \n");
                return 5;
            }

            do
            {
                fwrite(memory, (sizeof(char) * 512), 1, img); //invalid write of size 8(memory)?y

                free(memory);
                memory = malloc(sizeof(char) * 512);

                fread(memory, (sizeof(char) * 512), 1, inptr);
            }
            while ((memory[0].byte != 0xff &&  memory[1].byte != 0xd8 && memory[2].byte != 0xff && (memory[3].byte & 0xf0) != 0xe0)
            && (fread(memory, (sizeof(char) * 512), 1, inptr) == 1)); //return 512? if it doesnt continue

            fclose(img);
        }
        memory = realloc(memory, 512);
    }
    while (fread(memory, (sizeof(char) * 512), 1, inptr) == 1);

    free(memory);

    if (feof(inptr))
    {
        fclose(inptr);
    } //else error

    return 0;

}

            if (feof(inptr))
            {
                fwrite(memory, (sizeof(char) * 512), 1, img); //invalid write of size 8(memory)?y

                free(memory);
                memory = malloc(sizeof(char) * 512);

                fread(memory, 1, 512, inptr);
                fwrite(memory, 1, 512, img); //invalid write of size 8(memory)?y

                fclose(img);
                return 0;
            }