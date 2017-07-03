#!/usr/bin/python

import matplotlib.pyplot as plt
import subprocess

benchs = ['daxpy', 'dotproduct', 'func', 'memlayout']
speedups = []

subprocess.call(['make'])

for bench in benchs:
    scalar = './%s-scal' % bench
    simd = './%s-simd' % bench
    out = subprocess.check_output([scalar])
    time_scal = float(out.split()[2][0:-1])
    out = subprocess.check_output([simd])
    time_simd = float(out.split()[2][0:-1])
    speedup = time_scal / time_simd
    print('%10s speedup: %.2f' % (bench, speedup))
    speedups.append(speedup)

x = range(0, len(benchs))
plt.bar(x, speedups)
plt.xticks(x, benchs);
plt.ylabel('Speedup')
plt.show()

