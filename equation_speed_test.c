// Find speedup of optimized equations 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
 

int main(){

    // Test the entire process runs times
    int runs = 1000000; 

    // How many times to iterate the equation before doing another run
    int iters = 1000;
    int coeffs[52];
    int cr; 
    int ci;

    clock_t start, end;
    double optimized_cpu_time_used, unoptimized_cpu_time_used;

    start = clock();

    for (int i = 0; i<runs; i++){

        // Generate random coeffs, cr, and ci
        for (int c = 0; c<52; c++){
            coeffs[c] = 2 * ((float)rand() / (float)RAND_MAX) - 1; 
        }

        cr = 2 * ((float)rand() / (float)RAND_MAX) - 1;
        ci = 2 * ((float)rand() / (float)RAND_MAX) - 1;

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

        float zr = 0;
        float zi = 0;

        for (int j = 0; j<iters; j++){
            float temp = p0 + (p1 + (p2 + (p3 + (p4) * zr) * zr) * zr) * zr + (p5 + (p6 + (p7) * zr) * zr + (p8 + (p3 + (p9) * zr) * zr + (p7 + (p4) * zi) * zi) * zi) * zi;
            zi = p10 + (p11 + (p12 + (p13 + (p14) * zr) * zr) * zr) * zr + (p15 + (p16 + (p17) * zr) * zr + (p18 + (p13 + (p19) * zr) * zr + (p17 + (p14) * zi) * zi) * zi) * zi;
            zr = temp;
        }

    }

    end = clock();

    optimized_cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Optimized runtime: %f seconds\n", optimized_cpu_time_used);


    // Now run the unoptimized version

    start = clock();

    for (int i = 0; i<runs; i++){

        // Generate random coeffs, cr, and ci
        for (int c = 0; c<52; c++){
            coeffs[c] = 2 * ((float)rand() / (float)RAND_MAX) - 1; 
        }

        cr = 2 * ((float)rand() / (float)RAND_MAX) - 1;
        ci = 2 * ((float)rand() / (float)RAND_MAX) - 1;

        float zr = 0;
        float zi = 0;

        // Do no precalculations and don't minimize multiplications
        for (int j = 0; j<iters; j++){
            float temp = coeffs[16]*zr + coeffs[17]*zi + coeffs[34]*zr*zr + 2*coeffs[35]*zi*zr + -1*coeffs[34]*zi*zi + coeffs[4]*zr + -1*coeffs[5]*zi + coeffs[22]*zr*zr + coeffs[22]*zi*zi + coeffs[40]*zr*zr*zr + coeffs[41]*zi*zr*zr + coeffs[40]*zi*zi*zr + coeffs[41]*zi*zi*zi + coeffs[10]*zr*zr + -2*coeffs[11]*zi*zr + -1*coeffs[10]*zi*zi + coeffs[28]*zr*zr*zr + -1*coeffs[29]*zi*zr*zr + coeffs[28]*zi*zi*zr + -1*coeffs[29]*zi*zi*zi + coeffs[46]*zr*zr*zr*zr + 2*coeffs[46]*zi*zi*zr*zr + coeffs[46]*zi*zi*zi*zi + coeffs[0]*cr + -1*ci*coeffs[1] + coeffs[18]*cr*zr + coeffs[19]*cr*zi + -1*ci*coeffs[19]*zr + ci*coeffs[18]*zi + coeffs[36]*cr*zr*zr + 2*coeffs[37]*cr*zi*zr + -1*coeffs[36]*cr*zi*zi + -1*ci*coeffs[37]*zr*zr + 2*ci*coeffs[36]*zi*zr + ci*coeffs[37]*zi*zi + coeffs[6]*cr*zr + -1*coeffs[7]*cr*zi + -1*ci*coeffs[7]*zr + -1*ci*coeffs[6]*zi + coeffs[24]*cr*zr*zr + coeffs[24]*cr*zi*zi + -1*ci*coeffs[25]*zr*zr + -1*ci*coeffs[25]*zi*zi + coeffs[42]*cr*zr*zr*zr + coeffs[43]*cr*zi*zr*zr + coeffs[42]*cr*zi*zi*zr + coeffs[43]*cr*zi*zi*zi + -1*ci*coeffs[43]*zr*zr*zr + ci*coeffs[42]*zi*zr*zr + -1*ci*coeffs[43]*zi*zi*zr + ci*coeffs[42]*zi*zi*zi + coeffs[12]*cr*zr*zr + -2*coeffs[13]*cr*zi*zr + -1*coeffs[12]*cr*zi*zi + -1*ci*coeffs[13]*zr*zr + -2*ci*coeffs[12]*zi*zr + ci*coeffs[13]*zi*zi + coeffs[30]*cr*zr*zr*zr + -1*coeffs[31]*cr*zi*zr*zr + coeffs[30]*cr*zi*zi*zr + -1*coeffs[31]*cr*zi*zi*zi + -1*ci*coeffs[31]*zr*zr*zr + -1*ci*coeffs[30]*zi*zr*zr + -1*ci*coeffs[31]*zi*zi*zr + -1*ci*coeffs[30]*zi*zi*zi + coeffs[48]*cr*zr*zr*zr*zr + 2*coeffs[48]*cr*zi*zi*zr*zr + coeffs[48]*cr*zi*zi*zi*zi + -1*ci*coeffs[49]*zr*zr*zr*zr + -2*ci*coeffs[49]*zi*zi*zr*zr + -1*ci*coeffs[49]*zi*zi*zi*zi + coeffs[2]*cr*cr + -2*ci*coeffs[3]*cr + -1*ci*ci*coeffs[2] + coeffs[20]*cr*cr*zr + coeffs[21]*cr*cr*zi + -2*ci*coeffs[21]*cr*zr + 2*ci*coeffs[20]*cr*zi + -1*ci*ci*coeffs[20]*zr + -1*ci*ci*coeffs[21]*zi + coeffs[38]*cr*cr*zr*zr + 2*coeffs[39]*cr*cr*zi*zr + -1*coeffs[38]*cr*cr*zi*zi + -2*ci*coeffs[39]*cr*zr*zr + 4*ci*coeffs[38]*cr*zi*zr + 2*ci*coeffs[39]*cr*zi*zi + -1*ci*ci*coeffs[38]*zr*zr + -2*ci*ci*coeffs[39]*zi*zr + ci*ci*coeffs[38]*zi*zi + coeffs[8]*cr*cr*zr + -1*coeffs[9]*cr*cr*zi + -2*ci*coeffs[9]*cr*zr + -2*ci*coeffs[8]*cr*zi + -1*ci*ci*coeffs[8]*zr + ci*ci*coeffs[9]*zi + coeffs[26]*cr*cr*zr*zr + coeffs[26]*cr*cr*zi*zi + -2*ci*coeffs[27]*cr*zr*zr + -2*ci*coeffs[27]*cr*zi*zi + -1*ci*ci*coeffs[26]*zr*zr + -1*ci*ci*coeffs[26]*zi*zi + coeffs[44]*cr*cr*zr*zr*zr + coeffs[45]*cr*cr*zi*zr*zr + coeffs[44]*cr*cr*zi*zi*zr + coeffs[45]*cr*cr*zi*zi*zi + -2*ci*coeffs[45]*cr*zr*zr*zr + 2*ci*coeffs[44]*cr*zi*zr*zr + -2*ci*coeffs[45]*cr*zi*zi*zr + 2*ci*coeffs[44]*cr*zi*zi*zi + -1*ci*ci*coeffs[44]*zr*zr*zr + -1*ci*ci*coeffs[45]*zi*zr*zr + -1*ci*ci*coeffs[44]*zi*zi*zr + -1*ci*ci*coeffs[45]*zi*zi*zi + coeffs[14]*cr*cr*zr*zr + -2*coeffs[15]*cr*cr*zi*zr + -1*coeffs[14]*cr*cr*zi*zi + -2*ci*coeffs[15]*cr*zr*zr + -4*ci*coeffs[14]*cr*zi*zr + 2*ci*coeffs[15]*cr*zi*zi + -1*ci*ci*coeffs[14]*zr*zr + 2*ci*ci*coeffs[15]*zi*zr + ci*ci*coeffs[14]*zi*zi + coeffs[32]*cr*cr*zr*zr*zr + -1*coeffs[33]*cr*cr*zi*zr*zr + coeffs[32]*cr*cr*zi*zi*zr + -1*coeffs[33]*cr*cr*zi*zi*zi + -2*ci*coeffs[33]*cr*zr*zr*zr + -2*ci*coeffs[32]*cr*zi*zr*zr + -2*ci*coeffs[33]*cr*zi*zi*zr + -2*ci*coeffs[32]*cr*zi*zi*zi + -1*ci*ci*coeffs[32]*zr*zr*zr + ci*ci*coeffs[33]*zi*zr*zr + -1*ci*ci*coeffs[32]*zi*zi*zr + ci*ci*coeffs[33]*zi*zi*zi + coeffs[50]*cr*cr*zr*zr*zr*zr + 2*coeffs[50]*cr*cr*zi*zi*zr*zr + coeffs[50]*cr*cr*zi*zi*zi*zi + -2*ci*coeffs[51]*cr*zr*zr*zr*zr + -4*ci*coeffs[51]*cr*zi*zi*zr*zr + -2*ci*coeffs[51]*cr*zi*zi*zi*zi + -1*ci*ci*coeffs[50]*zr*zr*zr*zr + -2*ci*ci*coeffs[50]*zi*zi*zr*zr + -1*ci*ci*coeffs[50]*zi*zi*zi*zi;
            zi = coeffs[17]*zr + -1*coeffs[16]*zi + coeffs[35]*zr*zr + -2*coeffs[34]*zi*zr + -1*coeffs[35]*zi*zi + coeffs[5]*zr + coeffs[4]*zi + coeffs[23]*zr*zr + coeffs[23]*zi*zi + coeffs[41]*zr*zr*zr + -1*coeffs[40]*zi*zr*zr + coeffs[41]*zi*zi*zr + -1*coeffs[40]*zi*zi*zi + coeffs[11]*zr*zr + 2*coeffs[10]*zi*zr + -1*coeffs[11]*zi*zi + coeffs[29]*zr*zr*zr + coeffs[28]*zi*zr*zr + coeffs[29]*zi*zi*zr + coeffs[28]*zi*zi*zi + coeffs[47]*zr*zr*zr*zr + 2*coeffs[47]*zi*zi*zr*zr + coeffs[47]*zi*zi*zi*zi + coeffs[1]*cr + ci*coeffs[0] + coeffs[19]*cr*zr + -1*coeffs[18]*cr*zi + ci*coeffs[18]*zr + ci*coeffs[19]*zi + coeffs[37]*cr*zr*zr + -2*coeffs[36]*cr*zi*zr + -1*coeffs[37]*cr*zi*zi + ci*coeffs[36]*zr*zr + 2*ci*coeffs[37]*zi*zr + -1*ci*coeffs[36]*zi*zi + coeffs[7]*cr*zr + coeffs[6]*cr*zi + ci*coeffs[6]*zr + -1*ci*coeffs[7]*zi + coeffs[25]*cr*zr*zr + coeffs[25]*cr*zi*zi + ci*coeffs[24]*zr*zr + ci*coeffs[24]*zi*zi + coeffs[43]*cr*zr*zr*zr + -1*coeffs[42]*cr*zi*zr*zr + coeffs[43]*cr*zi*zi*zr + -1*coeffs[42]*cr*zi*zi*zi + ci*coeffs[42]*zr*zr*zr + ci*coeffs[43]*zi*zr*zr + ci*coeffs[42]*zi*zi*zr + ci*coeffs[43]*zi*zi*zi + coeffs[13]*cr*zr*zr + 2*coeffs[12]*cr*zi*zr + -1*coeffs[13]*cr*zi*zi + ci*coeffs[12]*zr*zr + -2*ci*coeffs[13]*zi*zr + -1*ci*coeffs[12]*zi*zi + coeffs[31]*cr*zr*zr*zr + coeffs[30]*cr*zi*zr*zr + coeffs[31]*cr*zi*zi*zr + coeffs[30]*cr*zi*zi*zi + ci*coeffs[30]*zr*zr*zr + -1*ci*coeffs[31]*zi*zr*zr + ci*coeffs[30]*zi*zi*zr + -1*ci*coeffs[31]*zi*zi*zi + coeffs[49]*cr*zr*zr*zr*zr + 2*coeffs[49]*cr*zi*zi*zr*zr + coeffs[49]*cr*zi*zi*zi*zi + ci*coeffs[48]*zr*zr*zr*zr + 2*ci*coeffs[48]*zi*zi*zr*zr + ci*coeffs[48]*zi*zi*zi*zi + coeffs[3]*cr*cr + 2*ci*coeffs[2]*cr + -1*ci*ci*coeffs[3] + coeffs[21]*cr*cr*zr + -1*coeffs[20]*cr*cr*zi + 2*ci*coeffs[20]*cr*zr + 2*ci*coeffs[21]*cr*zi + -1*ci*ci*coeffs[21]*zr + ci*ci*coeffs[20]*zi + coeffs[39]*cr*cr*zr*zr + -2*coeffs[38]*cr*cr*zi*zr + -1*coeffs[39]*cr*cr*zi*zi + 2*ci*coeffs[38]*cr*zr*zr + 4*ci*coeffs[39]*cr*zi*zr + -2*ci*coeffs[38]*cr*zi*zi + -1*ci*ci*coeffs[39]*zr*zr + 2*ci*ci*coeffs[38]*zi*zr + ci*ci*coeffs[39]*zi*zi + coeffs[9]*cr*cr*zr + coeffs[8]*cr*cr*zi + 2*ci*coeffs[8]*cr*zr + -2*ci*coeffs[9]*cr*zi + -1*ci*ci*coeffs[9]*zr + -1*ci*ci*coeffs[8]*zi + coeffs[27]*cr*cr*zr*zr + coeffs[27]*cr*cr*zi*zi + 2*ci*coeffs[26]*cr*zr*zr + 2*ci*coeffs[26]*cr*zi*zi + -1*ci*ci*coeffs[27]*zr*zr + -1*ci*ci*coeffs[27]*zi*zi + coeffs[45]*cr*cr*zr*zr*zr + -1*coeffs[44]*cr*cr*zi*zr*zr + coeffs[45]*cr*cr*zi*zi*zr + -1*coeffs[44]*cr*cr*zi*zi*zi + 2*ci*coeffs[44]*cr*zr*zr*zr + 2*ci*coeffs[45]*cr*zi*zr*zr + 2*ci*coeffs[44]*cr*zi*zi*zr + 2*ci*coeffs[45]*cr*zi*zi*zi + -1*ci*ci*coeffs[45]*zr*zr*zr + ci*ci*coeffs[44]*zi*zr*zr + -1*ci*ci*coeffs[45]*zi*zi*zr + ci*ci*coeffs[44]*zi*zi*zi + coeffs[15]*cr*cr*zr*zr + 2*coeffs[14]*cr*cr*zi*zr + -1*coeffs[15]*cr*cr*zi*zi + 2*ci*coeffs[14]*cr*zr*zr + -4*ci*coeffs[15]*cr*zi*zr + -2*ci*coeffs[14]*cr*zi*zi + -1*ci*ci*coeffs[15]*zr*zr + -2*ci*ci*coeffs[14]*zi*zr + ci*ci*coeffs[15]*zi*zi + coeffs[33]*cr*cr*zr*zr*zr + coeffs[32]*cr*cr*zi*zr*zr + coeffs[33]*cr*cr*zi*zi*zr + coeffs[32]*cr*cr*zi*zi*zi + 2*ci*coeffs[32]*cr*zr*zr*zr + -2*ci*coeffs[33]*cr*zi*zr*zr + 2*ci*coeffs[32]*cr*zi*zi*zr + -2*ci*coeffs[33]*cr*zi*zi*zi + -1*ci*ci*coeffs[33]*zr*zr*zr + -1*ci*ci*coeffs[32]*zi*zr*zr + -1*ci*ci*coeffs[33]*zi*zi*zr + -1*ci*ci*coeffs[32]*zi*zi*zi + coeffs[51]*cr*cr*zr*zr*zr*zr + 2*coeffs[51]*cr*cr*zi*zi*zr*zr + coeffs[51]*cr*cr*zi*zi*zi*zi + 2*ci*coeffs[50]*cr*zr*zr*zr*zr + 4*ci*coeffs[50]*cr*zi*zi*zr*zr + 2*ci*coeffs[50]*cr*zi*zi*zi*zi + -1*ci*ci*coeffs[51]*zr*zr*zr*zr + -2*ci*ci*coeffs[51]*zi*zi*zr*zr + -1*ci*ci*coeffs[51]*zi*zi*zi*zi;
            zr = temp;
        }
        
    }

    end = clock();

    unoptimized_cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Unoptimized runtime: %f seconds\n", unoptimized_cpu_time_used);

    printf("Optimized code is %f times faster\n", unoptimized_cpu_time_used / optimized_cpu_time_used);
}