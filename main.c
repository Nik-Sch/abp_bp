#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void sin_func(double amp, double f, double Fs, int len, double *out) {
    for (int i = 0; i < len; i++) {
        out[i] += amp * cos(2*M_PI*f/Fs*i);
    }
}

void band_pass_fir(double *out, double *signal, int len) {
    const double b0 = 0.1205498139,
                 b1 = 0,
                 b2 = -0.1205498139,
                 a1 = -1.7626236142,
                 a2 = 0.8273910712;

    for (int i = 2; i < len; i++) {
        out[i] = (b0 * signal[i  ]) +
                 (b1 * signal[i-1]) +
                 (b2 * signal[i-2]) -
                 (a1 * out[i-1]) -
                 (a2 * out[i-1]);

        out[i-2] = out[i-1];
        out[i-1] = out[i];
    }
}

void high_pass_filter(double* input, double* output, int len, double Fs, double cut_off) {
    double RC = 1.0/(cut_off*2*M_PI);
    double dt = 1.0/Fs;
    double alpha = dt/(RC+dt);
    output[0] = input[0];

    for(int i = 1; i < len; i++) {
        output[i] = alpha * (output[i-1] + input[i] - output[i-1]);
    }
}
void low_pass_filter(double* input, double* output, int len, double Fs, double cut_off) {
    double RC = 1.0/(cut_off*2*M_PI);
    double dt = 1.0/Fs;
    double alpha = dt/(RC+dt);
    output[0] = input[0];

    for(int i = 1; i < len; i++) {
        output[i] = output[i-1] + (alpha*(input[i] - output[i-1]));
    }
}


int main(int argc, char **argv) {
    int len = 1000;
    double amp = 1;
    double f1 = 1000; // Hertz
    double f2 = 250; // Hertz
    double f3 = 63; // Hertz
    double cut_off1 = 500;
    double cut_off2 = 125;
    double Fs = 24000;
    double *x = malloc(sizeof(double) * len);
    double *low1 = malloc(sizeof(double) * len);
    double *low2 = malloc(sizeof(double) * len);
    double *low3 = malloc(sizeof(double) * len);
    double *high1 = malloc(sizeof(double) * len);
    double *band1 = malloc(sizeof(double) * len);
    double *band2 = malloc(sizeof(double) * len);


    for (int i = 0; i < len; i++) {
        x[i] = 0x00;
    }
    sin_func(amp, f1, Fs, len, x);
    sin_func(amp, f2, Fs, len, x);
    sin_func(amp, f3, Fs, len, x);

    low_pass_filter(x, low1, len, Fs, cut_off1);
    // low_pass_filter(low1, low2, len, Fs, cut_off1);
    // low_pass_filter(low2, low3, len, Fs, cut_off1);

    high_pass_filter(x, high1, len, Fs, cut_off2);
    high_pass_filter(low1, band1, len, Fs, cut_off2);




    for (int i = 0; i < len; i++) {
        printf("%f %f\n", x[i], band1[i]);
        // printf("%f %f %f %f\n", x[i], low1[i], high1[i], band1[i]);
    }

    return 0;
}
