#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <vector>
#include "lodepng.h"
#include <cmath>
#include <complex>





// Functions which can be called by external script
extern "C" {

    // Generates a black and white fractal with the following specifications, puts the data in pixelData
    void generateGreyscaleFractal( std::vector<unsigned char>& pixelData, float minX, float maxX, float minY, float maxY, int xPixels, int yPixels, std::complex<float> (*iterator)(std::complex<float>, std::complex<float>), int maxIter) {

        std::complex<float> startPosition;
        std::complex<float> iterate;
        int small;

        for (int row = 0; row < yPixels; row++) {
            
            startPosition.imag( maxY - row * ( (maxY-minY) / yPixels ) );

            for (int col = 0; col < xPixels; col++) {

                startPosition.real( minX + col * ( (maxX-minX) / xPixels ) );
                
                iterate = startPosition;

                small = 1;

                for (int i = 0; i < maxIter; i++) {
                    if (std::norm(iterate) > 4) {
                        small = 0;
                        break;
                    }
                    iterate = iterate * iterate * iterate + iterate * iterate;
                }

                if (small == 0) {
                    pixelData[row * xPixels * 4 + col * 4 + 0] = 255; // R
                    pixelData[row * xPixels * 4 + col * 4 + 1] = 255; // G
                    pixelData[row * xPixels * 4 + col * 4 + 2] = 255; // B
                    pixelData[row * xPixels * 4 + col * 4 + 3] = 255; // A
                }
                if (small == 1) {
                    pixelData[row * xPixels * 4 + col * 4 + 0] = 0; // R
                    pixelData[row * xPixels * 4 + col * 4 + 1] = 0; // G
                    pixelData[row * xPixels * 4 + col * 4 + 2] = 0; // B
                    pixelData[row * xPixels * 4 + col * 4 + 3] = 255; // A
                }
                
            }
        }
        
    }

    void greyscaleFractal(const char* outputFile, float minX, float maxX, float minY, float maxY, int xPixels, int yPixels, int maxIter) {

        std::vector<unsigned char> pixelData(xPixels * yPixels * 4);

        // Set the mandelbrot as default iterator TODO: MAKE THIS DYNAMIC
        std::complex<float> (*iterator)(std::complex<float>,std::complex<float>) = NULL;

        // Call generateGreyScaleFractal on pixelData
        generateGreyscaleFractal(pixelData, minX, maxX, minY, maxY, xPixels, yPixels, iterator, maxIter);

        // Save pixelData to PNG image
        unsigned error = lodepng::encode(outputFile, pixelData, xPixels, yPixels);

        //if there's an error, display it
        if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;

    }
}

