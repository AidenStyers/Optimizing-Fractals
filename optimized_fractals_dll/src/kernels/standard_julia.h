#pragma once

static const char* standard_julia_source = R"CLC(

__kernel void standard_julia(
    float cx, // X position of center 
    float cy, // Y position of center
    float scale, // Scale of fractal, how wide/long each pixel of the resulting image is in the complex plane
    int width, // Width of resulting image 
    int height, // Height of resulting image
    int max_iter, // Maximum number of iterations
    int bailout_radius,
    int color_density, // How many iterations it takes for colors to loop
    __constant int* color_palette, // 4x3 array of RGB values to color graph
    __global uchar* output,
    float cr, // Extra constants found via standard_fractal
    float ci,
    __constant float* coeffs
) {
    
    // Get the x and y coordinates of the pixel that this kernel is calculating
    int x = get_global_id(0);
    int y = get_global_id(1);

    // Check that these coordinates are valid
    if (x >= width || y >= height)
        return;

    // Get the complex coordinates to calculate with
    float zr = cx + (x - width * 0.5f) * scale;
    float zi = cy + (y - height * 0.5f) * scale;

    // Calculate parts of iteration that do not change, speeds up code
    float p0 = coeffs[0]*cr + -1*ci*coeffs[1] + coeffs[2]*cr*cr + -2*ci*coeffs[3]*cr + -1*ci*ci*coeffs[2];
    float p1 = coeffs[16] + coeffs[4] + coeffs[18]*cr + -1*ci*coeffs[19] + coeffs[6]*cr + -1*ci*coeffs[7] + coeffs[20]*cr*cr + -2*ci*coeffs[21]*cr + -1*ci*ci*coeffs[20] + coeffs[8]*cr*cr + -2*ci*coeffs[9]*cr + -1*ci*ci*coeffs[8];
    float p2 = coeffs[34] + coeffs[22] + coeffs[10] + coeffs[36]*cr + -1*ci*coeffs[37] + coeffs[24]*cr + -1*ci*coeffs[25] + coeffs[12]*cr + -1*ci*coeffs[13] + coeffs[38]*cr*cr + -2*ci*coeffs[39]*cr + -1*ci*ci*coeffs[38] + coeffs[26]*cr*cr + -2*ci*coeffs[27]*cr + -1*ci*ci*coeffs[26] + coeffs[14]*cr*cr + -2*ci*coeffs[15]*cr + -1*ci*ci*coeffs[14];
    float p3 = coeffs[40] + coeffs[28] + coeffs[42]*cr + -1*ci*coeffs[43] + coeffs[30]*cr + -1*ci*coeffs[31] + coeffs[44]*cr*cr + -2*ci*coeffs[45]*cr + -1*ci*ci*coeffs[44] + coeffs[32]*cr*cr + -2*ci*coeffs[33]*cr + -1*ci*ci*coeffs[32];
    float p4 = coeffs[46] + coeffs[48]*cr + -1*ci*coeffs[49] + coeffs[50]*cr*cr + -2*ci*coeffs[51]*cr + -1*ci*ci*coeffs[50];
    float p5 = coeffs[17] + -1*coeffs[5] + coeffs[19]*cr + ci*coeffs[18] + -1*coeffs[7]*cr + -1*ci*coeffs[6] + coeffs[21]*cr*cr + 2*ci*coeffs[20]*cr + -1*ci*ci*coeffs[21] + -1*coeffs[9]*cr*cr + -2*ci*coeffs[8]*cr + ci*ci*coeffs[9];
    float p6 = 2*coeffs[35] + -2*coeffs[11] + 2*coeffs[37]*cr + 2*ci*coeffs[36] + -2*coeffs[13]*cr + -2*ci*coeffs[12] + 2*coeffs[39]*cr*cr + 4*ci*coeffs[38]*cr + -2*ci*ci*coeffs[39] + -2*coeffs[15]*cr*cr + -4*ci*coeffs[14]*cr + 2*ci*ci*coeffs[15];
    float p7 = coeffs[41] + -1*coeffs[29] + coeffs[43]*cr + ci*coeffs[42] + -1*coeffs[31]*cr + -1*ci*coeffs[30] + coeffs[45]*cr*cr + 2*ci*coeffs[44]*cr + -1*ci*ci*coeffs[45] + -1*coeffs[33]*cr*cr + -2*ci*coeffs[32]*cr + ci*ci*coeffs[33];
    float p8 = -1*coeffs[34] + coeffs[22] + -1*coeffs[10] + -1*coeffs[36]*cr + ci*coeffs[37] + coeffs[24]*cr + -1*ci*coeffs[25] + -1*coeffs[12]*cr + ci*coeffs[13] + -1*coeffs[38]*cr*cr + 2*ci*coeffs[39]*cr + ci*ci*coeffs[38] + coeffs[26]*cr*cr + -2*ci*coeffs[27]*cr + -1*ci*ci*coeffs[26] + -1*coeffs[14]*cr*cr + 2*ci*coeffs[15]*cr + ci*ci*coeffs[14];
    float p9 = 2*coeffs[46] + 2*coeffs[48]*cr + -2*ci*coeffs[49] + 2*coeffs[50]*cr*cr + -4*ci*coeffs[51]*cr + -2*ci*ci*coeffs[50];
    float p10 = coeffs[1]*cr + ci*coeffs[0] + coeffs[3]*cr*cr + 2*ci*coeffs[2]*cr + -1*ci*ci*coeffs[3];
    float p11 = coeffs[17] + coeffs[5] + coeffs[19]*cr + ci*coeffs[18] + coeffs[7]*cr + ci*coeffs[6] + coeffs[21]*cr*cr + 2*ci*coeffs[20]*cr + -1*ci*ci*coeffs[21] + coeffs[9]*cr*cr + 2*ci*coeffs[8]*cr + -1*ci*ci*coeffs[9];
    float p12 = coeffs[35] + coeffs[23] + coeffs[11] + coeffs[37]*cr + ci*coeffs[36] + coeffs[25]*cr + ci*coeffs[24] + coeffs[13]*cr + ci*coeffs[12] + coeffs[39]*cr*cr + 2*ci*coeffs[38]*cr + -1*ci*ci*coeffs[39] + coeffs[27]*cr*cr + 2*ci*coeffs[26]*cr + -1*ci*ci*coeffs[27] + coeffs[15]*cr*cr + 2*ci*coeffs[14]*cr + -1*ci*ci*coeffs[15];
    float p13 = coeffs[41] + coeffs[29] + coeffs[43]*cr + ci*coeffs[42] + coeffs[31]*cr + ci*coeffs[30] + coeffs[45]*cr*cr + 2*ci*coeffs[44]*cr + -1*ci*ci*coeffs[45] + coeffs[33]*cr*cr + 2*ci*coeffs[32]*cr + -1*ci*ci*coeffs[33];
    float p14 = coeffs[47] + coeffs[49]*cr + ci*coeffs[48] + coeffs[51]*cr*cr + 2*ci*coeffs[50]*cr + -1*ci*ci*coeffs[51];
    float p15 = -1*coeffs[16] + coeffs[4] + -1*coeffs[18]*cr + ci*coeffs[19] + coeffs[6]*cr + -1*ci*coeffs[7] + -1*coeffs[20]*cr*cr + 2*ci*coeffs[21]*cr + ci*ci*coeffs[20] + coeffs[8]*cr*cr + -2*ci*coeffs[9]*cr + -1*ci*ci*coeffs[8];
    float p16 = -2*coeffs[34] + 2*coeffs[10] + -2*coeffs[36]*cr + 2*ci*coeffs[37] + 2*coeffs[12]*cr + -2*ci*coeffs[13] + -2*coeffs[38]*cr*cr + 4*ci*coeffs[39]*cr + 2*ci*ci*coeffs[38] + 2*coeffs[14]*cr*cr + -4*ci*coeffs[15]*cr + -2*ci*ci*coeffs[14];
    float p17 = -1*coeffs[40] + coeffs[28] + -1*coeffs[42]*cr + ci*coeffs[43] + coeffs[30]*cr + -1*ci*coeffs[31] + -1*coeffs[44]*cr*cr + 2*ci*coeffs[45]*cr + ci*ci*coeffs[44] + coeffs[32]*cr*cr + -2*ci*coeffs[33]*cr + -1*ci*ci*coeffs[32];
    float p18 = -1*coeffs[35] + coeffs[23] + -1*coeffs[11] + -1*coeffs[37]*cr + -1*ci*coeffs[36] + coeffs[25]*cr + ci*coeffs[24] + -1*coeffs[13]*cr + -1*ci*coeffs[12] + -1*coeffs[39]*cr*cr + -2*ci*coeffs[38]*cr + ci*ci*coeffs[39] + coeffs[27]*cr*cr + 2*ci*coeffs[26]*cr + -1*ci*ci*coeffs[27] + -1*coeffs[15]*cr*cr + -2*ci*coeffs[14]*cr + ci*ci*coeffs[15];
    float p19 = 2*coeffs[47] + 2*coeffs[49]*cr + 2*ci*coeffs[48] + 2*coeffs[51]*cr*cr + 4*ci*coeffs[50]*cr + -2*ci*ci*coeffs[51];

    // Iterate to determine how the pixel should be colored
    // The code for this was generated by fractal_calculations.iypnb
    int iter = 0;
    while (zr*zr + zi*zi <= bailout_radius && iter < max_iter) {
        float temp = p0 + (p1 + (p2 + (p3 + (p4) * zr) * zr) * zr) * zr + (p5 + (p6 + (p7) * zr) * zr + (p8 + (p3 + (p9) * zr) * zr + (p7 + (p4) * zi) * zi) * zi) * zi;
        zi = p10 + (p11 + (p12 + (p13 + (p14) * zr) * zr) * zr) * zr + (p15 + (p16 + (p17) * zr) * zr + (p18 + (p13 + (p19) * zr) * zr + (p17 + (p14) * zi) * zi) * zi) * zi;
        zr = temp;
        iter++;
    }

    int idx = (y * width + x) * 3;

    float colorSum = 0;

    // If all of color_coeffs is zero then do boring coloring
    for (int i = 0; i<12; i++) {
        colorSum += color_palette[i];
    }
    
    if (colorSum == 0) {

        // Boring coloring
        if (iter >= max_iter) {
            output[idx + 0] = 0;
            output[idx + 1] = 0;
            output[idx + 2] = 0;
        }
        else {
            output[idx + 0] = 255;
            output[idx + 1] = 255;
            output[idx + 2] = 255;
        }

    } 
    else {

        if (iter >= max_iter) {
            output[idx + 0] = 0;
            output[idx + 1] = 0;
            output[idx + 2] = 0;
        }
        else {

            float c = (iter % color_density) / ((float)color_density);

            // Color the output based on a cubic spline between the chosen colors
            // We interpolate between each RGB value so that the resulting cubic polynomials have derivative zero at each knot
            // The math here is confusing, but its just a lot of linear algebra done beforehand
            if (c < 0.25f) {
                output[idx + 0] = (uchar) (color_palette[0] + 48*(color_palette[3] - color_palette[0])*c*c + 128*(color_palette[0] - color_palette[3])*c*c*c);
                output[idx + 1] = (uchar) (color_palette[1] + 48*(color_palette[4] - color_palette[1])*c*c + 128*(color_palette[1] - color_palette[4])*c*c*c);
                output[idx + 2] = (uchar) (color_palette[2] + 48*(color_palette[5] - color_palette[2])*c*c + 128*(color_palette[2] - color_palette[5])*c*c*c);
            }
            else if (c < 0.5f) {
                output[idx + 0] = (uchar) ((5*color_palette[6] - 4*color_palette[3]) + 48*(color_palette[3] - color_palette[6])*c - 144*(color_palette[3] - color_palette[6])*c*c + 128*(color_palette[3] - color_palette[6])*c*c*c);
                output[idx + 1] = (uchar) ((5*color_palette[7] - 4*color_palette[4]) + 48*(color_palette[4] - color_palette[7])*c - 144*(color_palette[4] - color_palette[7])*c*c + 128*(color_palette[4] - color_palette[7])*c*c*c);
                output[idx + 2] = (uchar) ((5*color_palette[8] - 4*color_palette[5]) + 48*(color_palette[5] - color_palette[8])*c - 144*(color_palette[5] - color_palette[8])*c*c + 128*(color_palette[5] - color_palette[8])*c*c*c);
            }
            else if (c < 0.75f) {
                output[idx + 0] = (uchar) ((28*color_palette[9] - 27*color_palette[6]) + 144*(color_palette[6] - color_palette[9])*c - 240*(color_palette[6] - color_palette[9])*c*c + 128*(color_palette[6] - color_palette[9])*c*c*c);
                output[idx + 1] = (uchar) ((28*color_palette[10] - 27*color_palette[7]) + 144*(color_palette[7] - color_palette[10])*c - 240*(color_palette[7] - color_palette[10])*c*c + 128*(color_palette[7] - color_palette[10])*c*c*c);
                output[idx + 2] = (uchar) ((28*color_palette[11] - 27*color_palette[8]) + 144*(color_palette[8] - color_palette[11])*c - 240*(color_palette[8] - color_palette[11])*c*c + 128*(color_palette[8] - color_palette[11])*c*c*c); 
            }
            else {
                output[idx + 0] = (uchar) ((81*color_palette[0] - 80*color_palette[9]) + 288*(color_palette[9] - color_palette[0])*c - 336*(color_palette[9] - color_palette[0])*c*c + 128*(color_palette[9] - color_palette[0])*c*c*c);
                output[idx + 1] = (uchar) ((81*color_palette[1] - 80*color_palette[10]) + 288*(color_palette[11] - color_palette[1])*c - 336*(color_palette[10] - color_palette[1])*c*c + 128*(color_palette[10] - color_palette[1])*c*c*c);
                output[idx + 2] = (uchar) ((81*color_palette[2] - 80*color_palette[11]) + 288*(color_palette[10] - color_palette[2])*c - 336*(color_palette[11] - color_palette[2])*c*c + 128*(color_palette[11] - color_palette[2])*c*c*c);
            }
        }
    }
}
)CLC";