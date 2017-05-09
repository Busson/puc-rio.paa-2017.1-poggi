#include <stdio.h>
#include <stdlib.h>

int read_input(char *filename, int **p, int **q)
{
    int n;
    FILE *file = fopen(filename, "r");

    if (file)
    {
        fscanf(file, "%d", &n);

        *p = (int *) malloc((n + 1) * sizeof(int));
        for (int i = 0; i < n + 1; i++)
            fscanf(file, "%d", *p+i);

        *q = (int *) malloc((n + 1) * sizeof(int));
        for (int i = 0; i < n + 1; i++)
            fscanf(file, "%d", *q+i);

        fclose(file);
    }

    return n;
}

int* multiply1(int P[], int Q[], int n)
{
    int result_size = 2*n + 1;
    int *result = (int *) calloc(result_size, sizeof(int));

    for (int i = 0; i < n + 1; i++)
        for (int j = 0; j < n + 1; j++)
            result[i+j] += P[i] * Q[j];

    return result;
}

int* multiply2(int P[], int Q[], int n)
{
    int result_size = 2*n + 1;
    int *result = (int *) calloc(result_size, sizeof(int));

    // Base case
    if (n == 1)
    {
        result[0] = P[0] * Q[0];
        return result;
    }

    // Divide
    int d = n / 2;
    if (n % 2 == 1)
        d++;

    int *pHigh = (int *) calloc(d, sizeof(int));
    int *qHigh = (int *) calloc(d, sizeof(int));
    int *pLow  = (int *) calloc(d-n%2, sizeof(int));
    int *qLow  = (int *) calloc(d-n%2, sizeof(int));

    for (int i = 0; i < d; i++)
    {
        pHigh[i] = P[i+d];
        qHigh[i] = Q[i+d];

        pLow[i] = P[i];
        qLow[i] = Q[i];
    }

    int *auxP = (int *) calloc(d, sizeof(int));
    int *auxQ = (int *) calloc(d, sizeof(int));

    for(int i = 0; i < d; i++)
    {
        auxP[i] = pLow[i]+pHigh[i];
        auxQ[i] = qLow[i]+qHigh[i];
    }

    // Conquer
    int *lowPQ  = multiply2(pLow,qLow,d);
    int *middle = multiply2(auxP, auxQ, d);
    int *highPQ = multiply2(pHigh,qHigh,d);

    // Combine
    for (int i = 0; i < n; i++)
    {
        result[i]     += lowPQ[i];
        result[i+d]   += middle[i]  - lowPQ[i] - highPQ[i];
        result[i+2*d] += highPQ[i];
    }

    free(pHigh); free(qHigh); free(pLow); free(qLow);
    free(lowPQ); free(middle); free(highPQ);

    return result;
}

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

    result = multiply2(p, q, n + 1);

    printf("Result: ");
    for (int i = 0; i < 2*n + 1; i++)
        printf("%d ", result[i]);
    printf("\n");

    free(p);
    free(q);
    free(result);

    return 1;
}
