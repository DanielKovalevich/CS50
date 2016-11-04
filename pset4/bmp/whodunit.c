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
    if (argc != 3)
    {
        printf("Usage: ./copy infile outfile\n");
        return 1;
    }

    // remember filenames
    char* infile = argv[1];
    char* outfile = argv[2];

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

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine padding for scanlines
    int padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // iterate over pixels in scanline
        for (int j = 0; j < bi.biWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
            
            // Gets rid of the red color in the image
            if (triple.rgbtRed == 255){
                triple.rgbtBlue = 255;
                triple.rgbtGreen = 255;
            }
            
            // Makes the image a little clearer
            if (triple.rgbtRed < 240 && triple.rgbtRed > 225) {
                triple.rgbtRed = 240;
                triple.rgbtBlue = 240;
                triple.rgbtGreen = 240;
            } else if (triple.rgbtRed < 225 && triple.rgbtRed > 200) {
                triple.rgbtRed = 225;
                triple.rgbtBlue = 225;
                triple.rgbtGreen = 225;
            } else if (triple.rgbtRed < 200 && triple.rgbtRed > 175) {
                triple.rgbtRed = 125;
                triple.rgbtBlue = 125;
                triple.rgbtGreen = 125;
            } else if (triple.rgbtRed <= 175 && triple.rgbtRed > 150) {
                triple.rgbtRed = 60;
                triple.rgbtBlue = 60;
                triple.rgbtGreen = 60;
            } else if (triple.rgbtRed <= 150 && triple.rgbtRed > 100) {
                triple.rgbtRed = 50;
                triple.rgbtBlue = 50;
                triple.rgbtGreen = 50;
            } else if (triple.rgbtRed <= 150 && triple.rgbtRed > 100) {
                triple.rgbtRed = 25;
                triple.rgbtBlue = 25;
                triple.rgbtGreen = 25;
            } else if (triple.rgbtRed <= 100 && triple.rgbtRed > 50) {
                triple.rgbtRed = 15;
                triple.rgbtBlue = 15;
                triple.rgbtGreen = 15;
            } else if (triple.rgbtRed <= 50 && triple.rgbtRed > 0) {
                triple.rgbtRed = 0;
                triple.rgbtBlue = 0;
                triple.rgbtGreen = 0;
            }
            
            // write RGB triple to outfile
            fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
        }

        // skip over padding, if any
        fseek(inptr, padding, SEEK_CUR);

        // then add it back (to demonstrate how)
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, outptr);
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}
