#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

//create one byte data type
typedef uint8_t BYTE;

typedef struct //move at the end
{
    BYTE byte;
}
BLOCK;

int main(int argc, char *argv[])
{
    //check if two arguments are given
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover infile\n");
        return 1;
    }

    //remember filename
    char *infile = argv[1];

    //open input file
    FILE *inptr = fopen(infile, "r"); //rb or r?
    if (inptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    //allocate "block sized" memory
    BLOCK *memory = malloc(sizeof(char) * 512);

    //name of the output .jpgs
    char namefile[sizeof "%03i.jpg"];
    int k = 0;

    //iterate through input file
    do
    {
        //read from input file to buffer
        fread(memory, (sizeof(char) * 512), 1, inptr); //check if end of file?
        if (memory == NULL)
        {
            fprintf(stderr, "Could not allocate memory.\n");
            return 4;
        }

        //checks to see if buffers first 4 bytes indicate the beginning of a jpg
        if (memory[0].byte == 0xff &&  memory[1].byte == 0xd8 && memory[2].byte == 0xff && ((memory[3].byte & 0xf0) == 0xe0))
        {
            //name new file
            sprintf(namefile, "%03i.jpg", k); //infile or img? or a new char array of [8]?

            //open output file
            FILE *img = fopen(namefile, "w"); //namefile not infile coz we tryna create a new file aka jpg for each image
            if (img == NULL)
            {
                fclose(img);
                fprintf(stderr, "Could not open. \n");
                return 5;
            }

            //write new file till the start of a new jpg or the end of the input file
            do
            {
                fwrite(memory, (sizeof(char) * 512), 1, img); //invalid write of size 8(memory)?y

                free(memory);
                memory = malloc(sizeof(char) * 512);

                fread(memory, (sizeof(char) * 512), 1, inptr);
            }
            while ((!(memory[0].byte == 0xff &&  memory[1].byte == 0xd8 && memory[2].byte == 0xff && ((memory[3].byte & 0xf0) == 0xe0)))
                   && (!feof(inptr))); //if fread == 0 to silence conditional..

            //checks if buffer's first 4 byte indicate a new jpg
            if (memory[0].byte == 0xff &&  memory[1].byte == 0xd8 && memory[2].byte == 0xff && ((memory[3].byte & 0xf0) == 0xe0))
            {
                //move input file position indicator for fread at the beginning of loop
                fseek(inptr, -(512), SEEK_CUR);

                //close output file
                fclose(img);
                k++;
            }
            //else if the end of input file has been reached
            else
            {
                free(memory);

                //the unknown quanity of last bytes read will be written
                fread(memory, 1, 512, inptr);
                int lastbytes = (fread(memory, 1, 512, inptr));
                fwrite(memory, 1, lastbytes, img); //invalid write of size 8(memory)?y

                //close output file
                fclose(img);

                //move input file position indicator, cause end of file terminates outer do while loop
                fseek(inptr, -(512), SEEK_CUR);
            }
        }

        //expand buffer
        memory = realloc(memory, 512);
    }
    while (!feof(inptr));

    //free expanded buffer all at once
    free(memory);
    return 0;
}
