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

#ifdef USE_OMP4
#pragma omp declare simd
#endif
float f(float x) {
    return x*x + 2*x + 1;
}

int main(int argc, char *argv[]) {
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

    double t0 = omp_get_wtime();
    float s = .0;
    for (r=0; r<REPS; r++) {
#ifdef USE_OMP4
#pragma omp simd reduction(+: s)
#endif
        for (i=0; i<n; i++) {
            float x0 = i;
            s += f(x0);
        }
    }
    s /= REPS;
    double t1 = omp_get_wtime();

    FILE *f = fopen("/dev/null", "r");
    fprintf(f, "s=%e\n", s);
    fclose(f);
    printf("CPU time: %.3fs (%s)\n", t1 - t0, TAG);

    return 0;
}
