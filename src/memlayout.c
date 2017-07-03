#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include <omp.h>

#define REPS 1000000

#define REAL    float

#ifdef USE_OMP4
#define TAG "SIMD"
#else
#define TAG "Scalar"
#endif

#ifdef USE_OMP4
void init(REAL *x, REAL *y, int n) {
    int i;
#pragma omp simd
    for (i=0; i<n; i++) {
        x[i] = i/(REAL) n;
        y[i] = i/(REAL) n;
    }
}

void update(REAL *x, REAL *y, int n) {
    int i;
#pragma omp simd
    for (i=0; i<n; i++) {
        x[i] = (x[i] + y[i]) / 2.;
        y[i] = (x[i] + y[i]) / 2.;
    }
}
#else
typedef struct {
    REAL x;
    REAL y;
} point_t;

void init(point_t *pts, int n) {
    int i;
    for (i=0; i<n; i++) {
        pts[i].x = i/(REAL) n;
        pts[i].y = i/(REAL) n;
    }
}

void update(point_t *pts, int n) {
    int i;
    for (i=0; i<n; i++) {
        pts[i].x = (pts[i].x + pts[i].y) / 2.;
        pts[i].y = (pts[i].x + pts[i].y) / 2.;
    }
}
#endif

int main(int argc, char *argv[]) {
#ifdef USE_OMP4
    REAL *x, *y;
#else
    point_t *pts;
#endif
    int i, r, n = 64;

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

#ifdef USE_OMP4
    x = malloc(n * sizeof(*x));
    y = malloc(n * sizeof(*y));

    if (!x || !y) {
        fprintf(stderr, "malloc failed\n");
        exit(EXIT_FAILURE);
    }
#else
    pts = malloc(n*sizeof(*pts));
    if (!pts) {
        fprintf(stderr, "malloc failed\n");
        exit(EXIT_FAILURE);
    }
#endif

    for (i=0; i<n; i++) {
#ifdef USE_OMP4
        init(x, y, n);
#else
        init(pts, n);
#endif
    }

    double t0 = omp_get_wtime();
    for (r=0; r<REPS; r++) {
#ifdef USE_OMP4
        update(x, y, n);
#else
        update(pts, n);
#endif
    }
    // d /= REPS;
    double t1 = omp_get_wtime();

    printf("CPU time: %.3fs (%s)\n", t1 - t0, TAG);

#ifdef USE_OMP4
    free(x);
    free(y);
#else
    free(pts);
#endif

    return 0;
}
