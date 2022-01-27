#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cs50.h>

typedef uint8_t BYTE;
int BLOCK_SIZE = 512;

int main(int argc, char *argv[])
{

    // Creating a new type

    // checking if correct amount of commands
    if (argc != 2)
    {
        printf("Usage: recover.c 'command'\n");
        return 1;
    }
    // opening the command as a file
    FILE *file = fopen(argv[1],"r");
    if (!file)
    {
        printf("Not a File");
        return 1;
    }
    bool same_img = false; // new image detection
    BYTE buffer[BLOCK_SIZE]; // buffer of 512 bytes
    int file_number = 0; // starting file_number
    char file_name[8];
    // Loop until fread() is false
    while (fread(buffer, 1, BLOCK_SIZE, file))
    {
        jump: // jump point if i find a a new image
            if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0 && same_img == false)
            {
                // first block of images

                sprintf(file_name, "%03i.jpg", file_number);
                FILE *img = fopen(file_name,"w");
                fwrite(buffer, 1, BLOCK_SIZE, img);
                file_number++;
                same_img = true;
                fclose(img);

            }
            // if not first block of image do this
            else
            {
                // if a block of new image jump to start of loop
                if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
                {
                    same_img = false;
                    goto jump;
                }
                // if not a new image continue to write into file
                else if(same_img == true)
                {
                    FILE *img = fopen(file_name, "a");
                    fwrite(buffer, 1, BLOCK_SIZE, img);
                    fclose(img);
                }
                else
                {
                    continue;
                }
            }


    }
}




