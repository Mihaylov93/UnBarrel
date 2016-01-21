// Petar Mihaylov Petrov
// 17/01/2016
// AGH University of Science and Technology / ULPGC University Las Palmas of Gran Canaria

// CHANGELOG
// -- 20/01/2016 Changelog created
// -- 20/01/2016 Implemented basic user interface
// -- 20/01/2016 Fixed memory erros and crashes on free()

// TODO
// Code revising
// Better UI
// Input error detection and asking for correction to the user 

// Limitations:
//    - no 12-bit-per-channel JPEG
//    - no JPEGs with arithmetic coding
//
// Basic usage
//    int x,y,n;
//    unsigned char *data = stbi_load(filename, &x, &y, &n, 0);
//    stbi_image_free(data)
 
 

 
#define STBI_ONLY_JPEG
#define STB_IMAGE_IMPLEMENTATION
#include "lib/stb_image.h"
 
#define TJE_IMPLEMENTATION
#include "lib/tiny_jpeg.h"
 
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
 

 
 
int main( int argc, char **argv ){
     
    char inFile[100];
    char outFile[100];
    char inA[64],inB[64],inC[64],inS[64];
    
    puts ( "Enter input filename:" );
    gets ( inFile );
    puts ( "Enter output filename:" );
    gets ( outFile );
    puts ( "CORRECTION PARAMETERS" );
    puts ( "Enter parameter A" );
    gets( inA );
    puts ( "Enter parameter B" );
    gets( inB );
    puts ( "Enter parameter C" );
    gets( inC );
    puts ( "Enter scaling S -> ( d = s - ( a - b - c ) )" );
    gets( inS );
    
    int width, height, cpp = 0;
    
    /* Basic usage
    int x,y,n;
    unsigned char *data = stbi_load(filename, &x, &y, &n, 0);
    stbi_image_free(data)
    
    The return value from an image loader is an 'unsigned char *' which points
    to the pixel data. The pixel data consists of *y scanlines of *x pixels,
    with each pixel consisting of N j; */
    
    unsigned char *data = stbi_load ( inFile, &width, &height, &cpp, 0);
    if ( !data ) { //stbi_load proccess data if not null
        puts( "Could not find file" );
        return EXIT_FAILURE;
    }
     
    
    printf ( "Width: %i\nHeight: %i\nComponents per pixel: %i\n", width, height, cpp );
    
    unsigned char *pPixel;
   
    pPixel = malloc ( width * cpp * height * sizeof ( unsigned char ) );
 
    unsigned char* correct_distortion ( int imageWidth, int imageHeight ){
        
        // Formula taken from http://mipav.cit.nih.gov/pubwiki/index.php/Barrel_Distortion_Correction
        
        /*
        a, b, c and FoV are physical properties of a lens/camera-combination at a given focus distance.
        At different focus settings, FoV will change noticeably, but usually it is fine to reuse a, b, and c even then.
        */
        
        double a = atof(inA);                // affects only the outermost pixels of the image
        double b = atof(inB);                 // most cases only require b optimization
        double c = atof(inC);                // uniform correction
        double d = atof(inS); - ( a - b - c );    // Scaling of the image
        
        int r = 0; 
        double deltaX, deltaY, dstR, srcR, factor, srcXd, srcYd = 0;
        
        double const centerX  = ( imageWidth - 1 ) / 2.0;
        double const centerY = ( imageHeight - 1 ) / 2.0;
       
        for ( int x = 0; x < width; x++ ) {
            for ( int y = 0; y < height; y++ ) {
                
                r = min ( imageWidth, imageHeight ) / 2.0;    // radius of the circle


                // cartesian coordinates of the destination point (relative to the centre of the image)
                deltaX = ( x - centerX ) / r;
                deltaY = ( y - centerY ) / r;

                // distance or radius of destination image
                dstR = sqrt ( pow ( deltaX, 2 ) + pow ( deltaY, 2 ) );

                // distance or radius of src image (with formula)
                // r_src = (a * r3_corr + b * r2_corr + c * r_corr + d) * r_corr
                srcR = ( a * pow ( dstR, 3 ) + b * pow ( dstR, 2 ) + c * dstR + d ) * dstR;

                // comparing old and new distance to get factor
                factor = ABS( dstR / srcR );

                // coordinates in source image
                srcXd = centerX + ( deltaX * factor * r );
                srcYd = centerY + ( deltaY * factor * r );

                // Casting the double coordinates into int
                int srcX = (int) srcXd;
                int srcY = (int) srcYd;

                if ( srcX >= 0 && srcY >= 0 && srcX < width && srcY < height ) {
                    // Fix for the memory crash
                    if ( !( x == 0 && y ==0 ) ){
                        pPixel [ y * width * 3 + x * 3 - 1 ] = data [ srcY * width * 3 + srcX * 3 - 1 ];
                        pPixel [ y * width * 3 + x * 3 - 2 ] = data [ srcY * width * 3 + srcX * 3 - 2 ];
                        pPixel [ y * width * 3 + x * 3 - 3 ] = data [ srcY * width * 3 + srcX * 3 - 3 ];
                    }else{
                        pPixel [ y * width * 3 + x * 3 ] = data [ srcY * width * 3 + srcX * 3 - 1 ];
                        pPixel [ y * width * 3 + x * 3 ] = data [ srcY * width * 3 + srcX * 3 - 2 ];
                        pPixel [ y * width * 3 + x * 3 ] = data [ srcY * width * 3 + srcX * 3 - 3 ];
                    }
                }
            }
        }

        return pPixel;

    }
     
     
    if ( !tje_encode_to_file_at_quality(outFile, 3 , width, height, cpp, correct_distortion (width,height) ) ) {
        fprintf ( stderr, "Could not write JPEG\n" );
        return EXIT_FAILURE;
    }
     
     
    stbi_image_free( data );
    free( pPixel );
  
     
    return EXIT_SUCCESS;
     
}
 


 
