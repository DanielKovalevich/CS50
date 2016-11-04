/**
 * copy.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: ./copy n infile outfile\n");
        return 1;
    }
    
    int size = atoi(argv[1]);
    
    if (size > 100){
        printf("n must be less than 100");
        return 3;
    }

    // remember filenames
    char* infile = argv[2];
    char* outfile = argv[3];

    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);
    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);
    

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    
    // Created for the new file
    BITMAPFILEHEADER bf2 = bf;
    BITMAPINFOHEADER bi2 = bi;
    
    //Changes in BITMAP FILE AND INFO HEADER to resize
    bi2.biWidth = bi.biWidth * size;
    bi2.biHeight = bi.biHeight * size;
    
    // determine padding for scanlines
    int padding =  (4 - (bi2.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int oldPadding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    bi2.biSizeImage = bi2.biWidth * abs(bi2.biHeight) * sizeof(RGBTRIPLE) + padding * abs(bi2.biHeight);
    bf2.bfSize = bi2.biSizeImage + 54;

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf2, sizeof(BITMAPFILEHEADER), 1, outptr);
    // write outfile's BITMAPINFOHEADER
    fwrite(&bi2, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // Holds pixels to output
        RGBTRIPLE row[bi.biWidth];
        // iterate over pixels in scanline
        for (int j = 0; j < bi.biWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;
            
            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
            
            row[j] = triple;
        }
        
        // skip over padding, if any
        fseek(inptr, oldPadding, SEEK_CUR);
        
        // Prints out pixels
        for (int m = 0; m < size; m++){
            for (int p = 0; p < bi.biWidth; p++){
                for (int q = 0; q < size; q++)
                    fwrite(&row[p], sizeof(RGBTRIPLE), 1, outptr);
            }
            
            // Adds the padding (if any)
            for (int k = 0; k < padding; k++) {
                fputc(0x00, outptr);
            }
        } 
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}
