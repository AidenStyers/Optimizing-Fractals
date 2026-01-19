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

    float p0 = cr*cr + -1*ci*ci;
    float p1 = ci*cr;
    float p2 = 2*cr*cr + -2*ci*ci;

    float real_out = coeffs[0]*cr + (p0) * coeffs[1] + (ci*coeffs[9] + -1*ci*coeffs[3] + p1*2*coeffs[10] + p1*-2*coeffs[4] + (-1*coeffs[17] + coeffs[11] + -1*coeffs[5] + -1*coeffs[18]*cr + coeffs[12]*cr + -1*coeffs[6]*cr - (p0) * coeffs[7] - (p0) * coeffs[19] + (p0) * coeffs[13] + (ci*coeffs[21] + -1*ci*coeffs[15] + p1*2*coeffs[22] + p1*-2*coeffs[16] + (coeffs[23] + coeffs[24]*cr + (p0) * coeffs[25]) * zi) * zi) * zi) * zi + (coeffs[8] + coeffs[2] + coeffs[9]*cr + coeffs[3]*cr + (p0) * coeffs[4] + (p0) * coeffs[10] + (2*ci*coeffs[18] + -2*ci*coeffs[6] + p1*4*coeffs[19] + p1*-4*coeffs[7] + (coeffs[20] + coeffs[14] + coeffs[21]*cr + coeffs[15]*cr + (p0) * coeffs[16] + (p0) * coeffs[22]) * zi) * zi + (coeffs[17] + coeffs[11] + coeffs[5] + coeffs[18]*cr + coeffs[12]*cr + coeffs[6]*cr + (p0) * coeffs[7] + (p0) * coeffs[19] + (p0) * coeffs[13] + (ci*coeffs[21] + -1*ci*coeffs[15] + p1*2*coeffs[22] + p1*-2*coeffs[16] + (2*coeffs[23] + 2*coeffs[24]*cr + (p2) * coeffs[25]) * zi) * zi + (coeffs[20] + coeffs[14] + coeffs[21]*cr + coeffs[15]*cr + (p0) * coeffs[16] + (p0) * coeffs[22] + (coeffs[23] + coeffs[24]*cr + (p0) * coeffs[25]) * zr) * zr) * zr) * zr;
    float imag_out = ci*coeffs[0] + p1*2*coeffs[1] + (ci*coeffs[9] + ci*coeffs[3] + p1*2*coeffs[10] + p1*2*coeffs[4] + (ci*coeffs[18] + ci*coeffs[12] + ci*coeffs[6] + p1*2*coeffs[19] + p1*2*coeffs[13] + p1*2*coeffs[7] + (ci*coeffs[21] + ci*coeffs[15] + p1*2*coeffs[22] + p1*2*coeffs[16] + (ci*coeffs[24] + p1*2*coeffs[25]) * zr) * zr) * zr) * zr + (-1*coeffs[8] + coeffs[2] + -1*coeffs[9]*cr + coeffs[3]*cr + (p0) * coeffs[4] - (p0) * coeffs[10] + (-2*coeffs[17] + 2*coeffs[5] + -2*coeffs[18]*cr + 2*coeffs[6]*cr + (p2) * coeffs[7] - (p2) * coeffs[19] + (-1*coeffs[20] + coeffs[14] + -1*coeffs[21]*cr + coeffs[15]*cr + (p0) * coeffs[16] - (p0) * coeffs[22]) * zr) * zr + (-1*ci*coeffs[18] + ci*coeffs[12] + -1*ci*coeffs[6] + p1*-2*coeffs[19] + p1*2*coeffs[13] + p1*-2*coeffs[7] + (ci*coeffs[21] + ci*coeffs[15] + p1*2*coeffs[22] + p1*2*coeffs[16] + (2*ci*coeffs[24] + p1*4*coeffs[25]) * zr) * zr + (-1*coeffs[20] + coeffs[14] + -1*coeffs[21]*cr + coeffs[15]*cr + (p0) * coeffs[16] + (ci*coeffs[24] + p1*2*coeffs[25]) * zi - (p0) * coeffs[22]) * zi) * zi) * zi;
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
    std::cout << "Optimal: " << normal_calc(zr, zi, cr, ci, coeffs) << std::endl;

    for (int i = 0; i<52; i++) {
        coeffs[i] = 1.0f;
    }

    // Test on all 1's
    std::cout << "Normal Test: " << normal_calc(1.0f, 1.0f, 1.0f, 1.0f, coeffs) << std::endl;
    std::cout << "Optimal Test: " << normal_calc(1.0f, 1.0f, 1.0f, 1.0f, coeffs) << std::endl;

}
