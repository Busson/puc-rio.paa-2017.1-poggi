#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "algorithms.h"
#include "CPUTimer.h"
static CPUTimer totaltime;

int main(int argc, char **argv)
{
    char *filename = argv[1];
    if (!filename)
    {
        printf("Error: missing filename...\n");
        return 0;
    }

    int *p = NULL, *q = NULL, *result = NULL;
    int n = read_input(filename, &p, &q);

    totaltime.reset();
    totaltime.start();
    result = multiply_trivial(p, q, n + 1);
    totaltime.stop();
    printf("Trivial: %lf seconds\n", totaltime.getCPUTotalSecs());
    free(result);

    totaltime.reset();
    totaltime.start();
    result = multiply_divide_conquer(p, q, n + 1);
    totaltime.stop();
    printf("Divide-n-Conquer: %lf seconds\n", totaltime.getCPUTotalSecs());
    free(result);

    totaltime.reset();
    totaltime.start();
    result = multiply_fft(p, q, n + 1);
    totaltime.stop();
    printf("Fast Fourier Transform: %lf seconds\n", totaltime.getCPUTotalSecs());
    free(result);

    free(p);
    free(q);

    return 1;
}
