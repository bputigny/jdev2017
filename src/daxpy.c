#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include <omp.h>

#define REPS 10000

#ifdef USE_OMP4
#define TAG "SIMD"
#else
#define TAG "Scalar"
#endif

void daxpy(double a, double *X, double *Y, int n) {
    int i;
#ifdef USE_OMP4
#pragma omp simd
#endif
    for (i=0; i<n; i++) {
        Y[i] = a*X[i] + Y[i];
    }
}

int main(int argc, char *argv[]) {
    double *A, *B;
    int i, r, n = 10000;

    char c;
    while ((c = getopt(argc, argv, "n:")) != EOF) {
        switch(c) {
            case 'n':
                n = atoi(optarg);
                break;
            default:
                exit(EXIT_FAILURE);
        }
    }

    A = malloc(n * sizeof(*A));
    B = malloc(n * sizeof(*B));

    if (!A || !B) {
        fprintf(stderr, "malloc failed\n");
        exit(EXIT_FAILURE);
    }

    for (i=0; i<n; i++) {
        A[i] = i+1;
        B[i] = 1./A[i];
    }

    double t0 = omp_get_wtime();
    for (r=0; r<REPS; r++) {
        daxpy(1.0, A, B, n);
    }
    double t1 = omp_get_wtime();

    printf("CPU time: %.3fs (%s)\n", t1 - t0, TAG);

    free(A);
    free(B);

    return 0;
}
