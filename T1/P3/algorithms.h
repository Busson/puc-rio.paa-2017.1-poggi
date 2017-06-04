typedef struct
{
   double r; /* real */
   double i; /* imaginary */
} complex;

// Helpers
int read_input(char*, int**, int**);
void fast_fourier_transform(complex*, complex*, int, int);
complex complex_mul(complex, complex);

// Real work
int* multiply_trivial(int[], int[], int);
int* multiply_divide_conquer(int[], int[], int);
int* multiply_fft(int[], int[], int);
