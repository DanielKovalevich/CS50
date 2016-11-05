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
 
 const char* FILE_NAME = "card.raw";
 const int MAX_JPEG = 50;
 const int BLOCK_SIZE = 512;
 const int FIRST_BYTE = 0xff,
           SECOND_BYTE = 0xd8,
           THIRD_BYTE = 0xff;

int main(int argc, char* argv[])
{
    // Ensure proper usage
    if (argc != 1)
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
    
    int *buffer;
    buffer = (int *) malloc(BLOCK_SIZE);
    
    while (currentJPEG < MAX_JPEG){
        
        long position = 0;
        bool foundPosition = false;
        bool nextJPG = false;
    
        int values[BLOCK_SIZE];
    
        // Goes over the file until the correct bytes are found
        do{
            fread(&buffer, BLOCK_SIZE , 1, inptr);
        
            values[BLOCK_SIZE] = *buffer;
            
            if (values[position] == FIRST_BYTE){
            
                if (values[position + 1] == SECOND_BYTE){
                
                    if (values[position + 2] == THIRD_BYTE){
                    
                        switch (values[position + 3]){
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
                                foundPosition = true;
                                break;
                        }
                    }
                }
            }
            else
                position++;
        } while (foundPosition == false);
        
        // Creates the jpg files
        do {
            char *title = NULL;
            // Prints the name with the correct format
            if (currentJPEG < 10)
                sprintf(title, "00%d.jpg", currentJPEG);
            else
                sprintf(title, "0%d.jpg", currentJPEG);
            FILE* img = fopen(title, "a");
            
            fwrite(&buffer, BLOCK_SIZE, 1, img);
            fread(&buffer, BLOCK_SIZE , 1, inptr);
            
            if 
            
        } while(nextJPG == false);
    }
    
    free(buffer);
}