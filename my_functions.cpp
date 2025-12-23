#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <vector>
#include "lodepng.h"





// Functions which can be called by external script
extern "C" {
	
	int square(int x) {
        return x * x;
    }

    struct vect {
        float x;
        float y;
    };

    vect mandelbrotIter(vect z, vect c) {
        vect output;
        output.x = c.x + z.x * z.x - z.y * z.y;
        output.y = c.y + 2 * z.x * z.y;
        return output;
    }

    void generateGreyscaleFractal( std::vector<unsigned char>& pixelData, float minX, float maxX, float minY, float maxY, int xPixels, int yPixels, vect (*iterator)(vect, vect), int maxIter) {

        for (int row = 0; row < yPixels; row++) {
            for (int col = 0; col < xPixels; col++) {

                pixelData[row * xPixels * 4 + col * 4 + 0] = (unsigned char)row; // R
                pixelData[row * xPixels * 4 + col * 4 + 1] = (unsigned char)col; // G
                pixelData[row * xPixels * 4 + col * 4 + 2] = 100;              // B
                pixelData[row * xPixels * 4 + col * 4 + 3] = 255;              // A

            }
        }
        
    }

    void greyscaleFractal(const char* outputFile, float minX, float maxX, float minY, float maxY, int xPixels, int yPixels, int maxIter) {

        std::vector<unsigned char> pixelData(xPixels * yPixels * 4);

        // Set the mandelbrot as default iterator TODO: MAKE THIS DYNAMIC
        vect (*iterator)(vect, vect) = &mandelbrotIter;

        // Call generateGreyScaleFractal on pixelData
        generateGreyscaleFractal(pixelData, minX, maxX, minY, maxY, xPixels, yPixels, iterator, maxIter);

        // Save pixelData to PNG image
        unsigned error = lodepng::encode(outputFile, pixelData, xPixels, yPixels);

        //if there's an error, display it
        if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;

    }
}

