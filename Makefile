all: daxpy dotproduct func memlayout

CC 		= gcc
CFLAGS	= -O3 -Wall -g -fopenmp -march=native
LIBS	= -lm

%: src/%.c
	$(CC) $(CFLAGS) -o $@-scal $^ $(LIBS)
	$(CC) -DUSE_OMP4 $(CFLAGS) -o $@-simd $^ $(LIBS)

clean:
	rm -f daxpy-* dotproduct-* func-* memlayout-*
