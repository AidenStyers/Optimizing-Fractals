#include <random>
#include <iostream>
#include <complex>


// Function which calculates normal style
std::complex<float> normal_calc(float zr, float zi, float cr, float ci, float coeffs[]) {

    // Convert all inputs to std::complex
    std::complex<float> z(zr, zi);
    std::complex<float> c(cr, ci);

    std::complex<float> cCoeffs[26];

    for (int i = 0; i<26; i++) {
        cCoeffs[i] = std::complex<float>(coeffs[2*i], coeffs[2*i+1]);
    }

    std::complex<float> out = 0;

    for (int cExp = 0; cExp<3; cExp++) {
        for (int zExp = 0; zExp<3; zExp++) {
            for (int zbExp = 0; zbExp<3; zbExp++) {

                if (cExp + zExp + zbExp != 0) {

                    std::complex<float> toAdd(1,0);

                    for (int i = 0; i<cExp; i++) { toAdd = toAdd * c; }
                    for (int i = 0; i<zExp; i++) { toAdd = toAdd * z; }
                    for (int i = 0; i<zbExp; i++) { toAdd = toAdd * std::conj(z); }

                    // Mulitply by the correct coefficient
                    toAdd = toAdd * cCoeffs[cExp + 3*zExp + 9*zbExp - 1];

                    out += toAdd;
                }
            }
        }
    }

    return out;

}

// Optimal code
std::complex<float> optimal_calc(float zr, float zi, float cr, float ci, float coeffs[]) {

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

    float real_out = p0 + (p1 + (p2 + (p3 + (p4) * zr) * zr) * zr) * zr + (p5 + (p6 + (p7) * zr) * zr + (p8 + (p3 + (p9) * zr) * zr + (p7 + (p4) * zi) * zi) * zi) * zi;
    float imag_out = p10 + (p11 + (p12 + (p13 + (p14) * zr) * zr) * zr) * zr + (p15 + (p16 + (p17) * zr) * zr + (p18 + (p13 + (p19) * zr) * zr + (p17 + (p14) * zi) * zi) * zi) * zi;
    return std::complex(real_out, imag_out);

} 


int main() {

    // Choose random coeffs and values for zr, zi, cr, ci

    // 1. Create a random device to seed the engine
    std::random_device rd;
    std::mt19937 engine(rd());

    std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

    float zr = dist(engine);
    float zi = dist(engine);
    float cr = dist(engine);
    float ci = dist(engine);

    float coeffs[52];

    for (int i = 0; i<52; i++) {
        coeffs[i] = dist(engine);
    }

    // Calculate normal style
    std::cout << "Normal: " << normal_calc(zr, zi, cr, ci, coeffs) << std::endl;

    // Calculate crazy style
    std::cout << "Optimal: " << optimal_calc(zr, zi, cr, ci, coeffs) << std::endl;

    for (int i = 0; i<26; i++) {
        coeffs[2*i] = 1.0f;
        coeffs[2*i+1] = 0.0f;
    }

    // Test on all 1's
    std::cout << "Normal Test: " << normal_calc(1.0f, 0.0f, 1.0f, 0.0f, coeffs) << std::endl;
    std::cout << "Optimal Test: " << optimal_calc(1.0f, 0.0f, 1.0f, 0.0f, coeffs) << std::endl;

    for (int i = 0; i<52; i++) {
        coeffs[i] = 0.0f;
    }

    coeffs[0] = 1.0f;
    coeffs[10] = 1.0f;

    std::cout << "Mandelbrot testing:" << std::endl;
    std::cout << "Mandelbrot testing:" << std::endl;
}
