/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Recovers JPEGs from a forensic image.
 */
 
 #include <stdio.h>
 #include <stdlib.h>
 #include <stdbool.h>
 #include <stdint.h>
 
 const char* FILE_NAME = "card.raw";
 const int MAX_JPEG = 50;
 const int BLOCK_SIZE = 512;
 const uint8_t FIRST_BYTE = 0xff,
               SECOND_BYTE = 0xd8,
               THIRD_BYTE = 0xff;

int main(int argc, char* argv[])
{
    // Ensure proper usage
    if (argc != 1 && argv[0] != NULL)
    {
        printf("Usage: ./recover\n");
        return 1;
    }
    
    FILE* inptr = fopen(FILE_NAME, "r");  
    
    // Check if file opens
    if (inptr == NULL){
        printf("Cannot open file!");
        return 2;
    }
    
    int currentJPEG = 0;
    bool startJPG = false;
    uint8_t buffer[BLOCK_SIZE];
    
    // Output file object
    FILE* img = NULL;
    
    while (currentJPEG < MAX_JPEG){
        bool newJPG = false;
        
        if (fread(&buffer, BLOCK_SIZE , 1, inptr) == 1){
            
            // Check for first few bytes of a jpg
            if (buffer[0] == FIRST_BYTE){
                if (buffer[1] == SECOND_BYTE){
                    if (buffer[2] == THIRD_BYTE){
                        switch (buffer[3]){
                            case 0xe0:
                            case 0xe1:
                            case 0xe2:
                            case 0xe3:
                            case 0xe4:
                            case 0xe5:
                            case 0xe6:
                            case 0xe7:
                            case 0xe8:
                            case 0xe9:
                            case 0xea:
                            case 0xeb:
                            case 0xec:
                            case 0xed:
                            case 0xee:
                            case 0xef:
                                newJPG = true;
                                startJPG = true;
                                break;
                        }
                    }
                }
            }
        
            if (startJPG){
                // Creates the jpg files
                if (newJPG){
                    char title[8];
                    // Prints the name with the correct format
                    sprintf(title, "%03d.jpg", currentJPEG);
                
                    img = fopen(title, "a");
                    currentJPEG++;
                }
                
                fwrite(&buffer, BLOCK_SIZE, 1, img);
            }
        }
        else{
            fclose(inptr);
            fclose(img);
        }
    }
}