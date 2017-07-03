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

float dotproduct(float *A, float *B, int n) {
    int i;
    float s = .0;
#ifdef USE_OMP4
#pragma omp simd reduction(+: s)
#endif
    for (i=0; i<n; i++) {
        s += A[i] * B[i];
    }
    return s;
}

int main(int argc, char *argv[]) {
    float *A, *B;
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
    float d = 0;
    for (r=0; r<REPS; r++) {
        d += dotproduct(A, B, n);
    }
    d /= REPS;
    double t1 = omp_get_wtime();

    if (fabs(((float) n) - d) > 1e-10) {
        fprintf(stderr, "ERROR in dotproduct\n");
    }
    printf("CPU time: %.3fs (%s)\n", t1 - t0, TAG);

    free(A);
    free(B);

    return 0;
}
