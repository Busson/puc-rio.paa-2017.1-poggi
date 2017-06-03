#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>

#define MAX 200

double complex polar(double mag, double ang)
{
    double rp = mag * (cos ( (ang) ) );
    double ip = mag * (sin ( (ang) ) );
    return rp + ip * I;
}

int log_2(int N) // calc log2 of int numbers
{
    int k = N, i = 0;
    while(k) {
        k >>= 1;
        i++;
    }
    return i - 1;
}

int check(int n)    // checking if the number of element is a power of 2
{
    return n > 0 && (n & (n - 1)) == 0;
}

int reverse(int N, int n) // calc reverse number
{
    int j, p = 0;
    for(j = 1; j <= log_2(N); j++) {
        if(n & (1 << (log_2(N) - j)))
            p |= 1 << (j - 1);
    }
    return p;
}

void ordina(double complex* f1, int N) // use the reverse order in the array
{
    double complex f2[MAX];
    for(int i = 0; i < N; i++)
        f2[i] = f1[reverse(N, i)];
    for(int j = 0; j < N; j++)
        f1[j] = f2[j];
}

void transform(double complex* f, int N) //
{
    ordina(f, N); // first: reverse order
    double complex *W;
    W = (double complex *) malloc(N / 2 * sizeof(double complex));
    W[1] = polar(1., -2. * M_PI / N);
    W[0] = 1;
    for(int i = 2; i < N / 2; i++)
        W[i] = pow(W[1], i);
    int n = 1;
    int a = N / 2;
    for(int j = 0; j < log_2(N); j++) {
        for(int i = 0; i < N; i++) {
            if(!(i & n)) {
                double complex temp = f[i];
                double complex Temp = W[(i * a) % (n * a)] * f[i + n];
                f[i] = temp + Temp;
                f[i + n] = temp - Temp;
            }
        }
        n *= 2;
        a = a / 2;
    }
}

void FFT(double complex* f, int N, double d)
{
    transform(f, N);
    for(int i = 0; i < N; i++)
        f[i] *= d; // multiplying step
}

void print_complex(double complex z)
{
    printf("%.2f + (%.2fi)\n", creal(z), cimag(z));
}

int main()
{
    // Collecting input
    int n;
    do {
        printf("What is the array dimension? (MUST be power of 2)\n");
        scanf("%d", &n);
    } while(!check(n));

    double d = 1;
    double complex vec[MAX];
    printf("Write input\n");
    for(int i = 0; i < n; i++) {
        printf("Element number: %d\n", i);
        scanf("%lf", &vec[i]);
    }

    // Running FFT
    FFT(vec, n, d);

    // Print output
    printf("This is the FFT:\n");
    for(int j = 0; j < n; j++)
        print_complex(vec[j]);

    return 0;
}
