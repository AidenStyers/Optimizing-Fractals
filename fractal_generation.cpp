#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <vector>
#include "lodepng.h"
#include <cmath>
#include <complex>
#include <chrono>





// Functions which can be called by external script
extern "C" {

    __declspec(dllexport)
    // Generates a black and white fractal with the following specifications, puts the data in pixelData
    void generateGreyscaleFractal( std::vector<unsigned char>& pixelData, float minX, float maxX, float minY, float maxY, int xPixels, int yPixels, std::complex<float> (*iterator)(std::complex<float>, std::complex<float>), int maxIter) {

        std::complex<float> startPosition;
        std::complex<float> iterate;
        int small;

        for (int row = 0; row < yPixels; row++) {
            
            startPosition.imag( maxY - row * ( (maxY-minY) / yPixels ) );

            for (int col = 0; col < xPixels; col++) {

                startPosition.real( minX + col * ( (maxX-minX) / xPixels ) );
                
                iterate = 0;

                small = 1;

                for (int i = 0; i < maxIter; i++) {
                    if (std::norm(iterate) > 4) {
                        small = 0;
                        break;
                    }
                    iterate = iterate * iterate * iterate + iterate * iterate + startPosition;
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

    // Uses data from generateGreyscaleFractal to actually create a png of the fractal
    __declspec(dllexport)
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


    // Creates an vector of 0/1 values for a fractal, faster and easier to evaluate structure
    // Output stored in boolArray
    // Range set to [-2,2]^2
    // Coefficents of function to be iterated are in 3D array of floats coeffs
    // pixelSplit gives the resolution of the fractal, generates a pixelxpixel array
    __declspec(dllexport)
    void generateBoolFractal(std::vector<bool>& boolArray, std::complex<float> coeffs[3][3][3], int maxIter, int pixels) {

        std::complex<float> c;
        std::complex<float> z;
        std::complex<float> zb;
        int diverged;

        for (int row = 0; row < pixels; row++) {

            c.imag( 2 - row * 4.0 / pixels );

            for (int col = 0; col < pixels; col++) {

                c.real( (col * 4.0 / pixels) - 2 );

                z = 0;

                zb = 0;

                diverged = 0;

                for (int iteration = 0; iteration < maxIter; iteration++) {

                    // Find conjugate of z
                    zb = std::conj(z);

                    // Add all 26 terms to iterate z
                    // Wrote these out manually to make it faster????? Maybe??
                    z = 
                    coeffs[1][0][0]*c + coeffs[2][0][0]*c*c +

                    coeffs[0][1][0]*z + coeffs[1][1][0]*c*z + coeffs[2][1][0]*c*c*z +
                    coeffs[0][0][1]*zb + coeffs[1][0][1]*c*zb + coeffs[2][0][1]*c*c*zb +

                    coeffs[0][2][0]*z*z + coeffs[1][2][0]*c*z*z + coeffs[2][2][0]*c*c*z*z +
                    coeffs[0][0][2]*zb*zb + coeffs[1][0][2]*c*zb*zb + coeffs[2][0][2]*c*c*zb*zb +
                    coeffs[0][1][1]*z*zb  + coeffs[1][1][1]*c*z*zb + coeffs[2][1][1]*c*c*z*zb +

                    coeffs[0][2][1]*z*z*zb  + coeffs[1][2][1]*c*z*z*zb + coeffs[2][2][1]*c*c*z*z*zb +
                    coeffs[0][1][2]*z*zb*zb  + coeffs[1][1][2]*c*z*zb*zb + coeffs[2][1][2]*c*c*z*zb*zb +

                    coeffs[0][2][2]*z*z*zb*zb  + coeffs[1][2][2]*c*z*z*zb*zb + coeffs[2][2][2]*c*c*z*z*zb*zb;

                    // Check if z has grown large
                    if (std::norm(z) > 1000) {
                        diverged = 1;
                        break;
                    }
                }

                if (diverged) {
                    boolArray[row * pixels + col] = false;
                }
                else {
                    boolArray[row * pixels + col] = true;
                }

            }
        }

    }

    // Generates a simplefractal with the input parameters using generateBoolFractal
    // Then find how 'cool' the result is and returns that
    __declspec(dllexport)
    int coolness(float* parameters, int pixels) {

        // Create array to be filled with fractal data
        std::vector<bool> boolArray(pixels*pixels);

        // Take apart input array to make parameter array
        std::complex<float> coeffs[3][3][3];

        for (int i = 0; i < 27; i++) {
            coeffs[i%3][(i%9)/3][i/9] = std::complex<float>(parameters[i],parameters[i+1]);
        }

        // Call generateBoolFractal
        generateBoolFractal(boolArray, coeffs, 30, pixels);


        // Find 'coolness'
        int count = 0;

        for (int row = 0; row < pixels-1; row++) {

            for (int col = 0; col < pixels-1; col++) {
                
                if (boolArray[row*pixels + col] != boolArray[row*pixels+col+1]) {
                    count++;
                }

                if (boolArray[row*pixels + col] != boolArray[(row+1)*pixels+col]) {
                    count++;
                }
                
            }

            if (boolArray[row*pixels + (pixels - 1)] != boolArray[(row+1)*pixels + (pixels - 1)]) {
                count++;
            }
        }

        for (int col = 0; col < pixels-1; col++) {
                
            if (boolArray[(pixels-1)*pixels + col] != boolArray[(pixels-1)*pixels+col+1]) {
                count++;
            }
                
        }

        return count;

    }

    // Tests generateBoolFractal on the mandelbrot set, doesn't actually run tests it just returns a ton of info about what was generated
    __declspec(dllexport)
    void testBoolFractal() {

        // Generate the mandelbrot set using generateBoolFractal and check that is it valid
        
        // Vector to be filled with fractal
        std::vector<bool> fractal(512*512 + 5);

        // Parameters for the mandelbrot set
        std::complex<float> coeffs[3][3][3] = {std::complex<float>(0.0,0.0)};

        coeffs[1][0][0] = 1;

        coeffs[0][2][0] = 1;

        std::cout << coeffs[2][2][0] << std::endl; 

        // Generate fractal

        auto start = std::chrono::high_resolution_clock::now();

        generateBoolFractal(fractal, coeffs, 50, 512);

        std::chrono::duration<double, std::milli> duration = std::chrono::high_resolution_clock::now() - start;

        std::cout << "Fractal took " << duration.count() << " milliseconds to run" << std::endl;

        // Check that edges of fractal are basically all set to false

        int trueCount = 0;

        for (int row = 0; row < 512; row++) {

            if (fractal[row * 512]) {
                trueCount += 1;
            }

            if (fractal[row * 512 + 511]) {
                trueCount += 1;
            }

        }

        for (int col = 0; col < 512; col++) {

            if (fractal[col]) {
                trueCount += 1;
            }

            if (fractal[511 * 512 + col]) {
                trueCount += 1;
            }
        }

        std::cout << "These were " << trueCount << " trues on the edges" << std::endl;

        // Find the percentage of trues and the amount fo trues per column

        trueCount = 0;

        int preTrues;
        int colTrues[512];

        for (int row = 0; row < 512; row++) { 

            preTrues = trueCount;

            for (int col = 0; col < 512; col++) {
                if (fractal[row * 512 + col]) {
                    trueCount += 1;
                }
            }

            colTrues[row] = trueCount - preTrues;

        }

        float percentage = trueCount / (1.0 * 512*512);
        
        std::cout << "The proportion of trues was: " << percentage << std::endl;

        for (int row = 0; row < 512; row++) {
            std::cout << colTrues[row] << std::endl;
        }
    }

    // Generates an image using generateBoolFractal's data
    __declspec(dllexport)
    int formattedGreyscaleFractal(const char* outputFile, float* parameters, int pixels) {

        // Take apart input array to make parameter array
        std::complex<float> coeffs[3][3][3];

        for (int i = 0; i < 27; i++) {
            coeffs[i%3][(i%9)/3][i/9] = std::complex<float>(parameters[2*i],parameters[2*i+1]);
        }

        // Create array to be filled with fractal
        std::vector<bool> boolData(pixels * pixels);

        // Call generateGreyScaleFractal on boolData
        generateBoolFractal(boolData, coeffs, 30, pixels);

        // Convert it to an array of RGB values
        std::vector<unsigned char> pixelData(pixels * pixels * 4);

        for (int row = 0; row < pixels; row++) {
            for (int col = 0; col < pixels; col++) {

                if (boolData[row * pixels + col]) {
                    pixelData[row * pixels * 4 + col * 4 + 0] = 0; // R
                    pixelData[row * pixels * 4 + col * 4 + 1] = 0; // G
                    pixelData[row * pixels * 4 + col * 4 + 2] = 0; // B
                    pixelData[row * pixels * 4 + col * 4 + 3] = 255; // A
                }
                else {
                    pixelData[row * pixels * 4 + col * 4 + 0] = 255; // R
                    pixelData[row * pixels * 4 + col * 4 + 1] = 255; // G
                    pixelData[row * pixels * 4 + col * 4 + 2] = 255; // B
                    pixelData[row * pixels * 4 + col * 4 + 3] = 255; // A
                }
            }
        }


        // Save pixelData to PNG image
        unsigned error = lodepng::encode(outputFile, pixelData, pixels, pixels);

        //if there's an error, display it
        if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;

        // Successful return message
        return 1;
    }


    // Creates an vector of 0/1 values for a fractal, faster and easier to evaluate structure
    // Output stored in boolArray
    // Range set to [-2,2]^2
    // Coefficents of function to be iterated are in 3D array of floats coeffs
    // pixelSplit gives the resolution of the fractal, generates a pixelxpixel array
    __declspec(dllexport)
    int formattedFractal(const char* outputFile, float* parameters, float xPosition, float yPosition, float radius, int maxIter, int pixels) {


        // Unpack the input parameters into the coeffs array
        std::complex<float> coeffs[3][3][3];

        for (int i = 0; i < 27; i++) {
            coeffs[i%3][(i%9)/3][i/9] = std::complex<float>(parameters[2*i],parameters[2*i+1]);
        }

        // Array to be filled with image
        std::vector<unsigned char> pixelData(pixels * pixels * 4);

        // Values to be iterated on to generate fractal
        std::complex<float> c(xPosition - radius, yPosition + radius);
        std::complex<float> z;
        std::complex<float> zb;
        float norm;

        // True/false indicator for if values diverge
        int diverged;

        // Amount to change the starting real and imaginary values each iteration of the loop
        float realDelta = 2.0 * radius / pixels;
        std::complex<float> imagDelta(0, -2.0 * radius / pixels);

        for (int row = 0; row < pixels; row++) {

            for (int col = 0; col < pixels; col++) {

                z = 0;
                zb = 0;
                norm = 0;

                diverged = 0;

                for (int iteration = 0; iteration < maxIter; iteration++) {

                    
                    // Add all 26 terms to iterate z
                    // Wrote these out manually to make it faster????? Maybe??
                    z = 
                    coeffs[1][0][0]*c + coeffs[2][0][0]*c*c +

                    coeffs[0][1][0]*z + coeffs[1][1][0]*c*z + coeffs[2][1][0]*c*c*z +
                    coeffs[0][0][1]*zb + coeffs[1][0][1]*c*zb + coeffs[2][0][1]*c*c*zb +

                    coeffs[0][2][0]*z*z + coeffs[1][2][0]*c*z*z + coeffs[2][2][0]*c*c*z*z +
                    coeffs[0][0][2]*zb*zb + coeffs[1][0][2]*c*zb*zb + coeffs[2][0][2]*c*c*zb*zb +
                    coeffs[0][1][1]*z*zb  + coeffs[1][1][1]*c*z*zb + coeffs[2][1][1]*c*c*z*zb +

                    coeffs[0][2][1]*z*z*zb  + coeffs[1][2][1]*c*z*z*zb + coeffs[2][2][1]*c*c*z*z*zb +
                    coeffs[0][1][2]*z*zb*zb  + coeffs[1][1][2]*c*z*zb*zb + coeffs[2][1][2]*c*c*z*zb*zb +

                    coeffs[0][2][2]*z*z*zb*zb  + coeffs[1][2][2]*c*z*z*zb*zb + coeffs[2][2][2]*c*c*z*z*zb*zb;

                    // Find conjugate and norm of z
                    zb = std::conj(z);
                    norm = z.real() * z.real() + z.imag() * z.imag();

                    // Check if z has grown large
                    if (norm > 10000) {
                        diverged = 1;
                        break;
                    }

                }

                if (diverged) {
                    pixelData[row * pixels * 4 + col * 4 + 0] = 255; // R
                    pixelData[row * pixels * 4 + col * 4 + 1] = 255; // G
                    pixelData[row * pixels * 4 + col * 4 + 2] = 255; // B
                    pixelData[row * pixels * 4 + col * 4 + 3] = 255; // A
                    
                }
                else {
                    pixelData[row * pixels * 4 + col * 4 + 0] = 0; // R
                    pixelData[row * pixels * 4 + col * 4 + 1] = 0; // G
                    pixelData[row * pixels * 4 + col * 4 + 2] = 0; // B
                    pixelData[row * pixels * 4 + col * 4 + 3] = 255; // A   
                }

                // Increase c.real
                c += realDelta;               
            }
            
            // Lower c.imag
            c += imagDelta;

            // Reset c.real
            c.real(xPosition - radius);
        }



        // Save pixelData to PNG image
        unsigned error = lodepng::encode(outputFile, pixelData, pixels, pixels);

        //if there's an error, display it
        if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;

        // Successful return message
        return 4;

    }


}

