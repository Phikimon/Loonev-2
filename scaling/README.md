### Task
Write simple computational program that will
scale perfectly(5% accuracy) on multicore computers.

Note: consider hyperthreading and turboboost side-effects.

### Usage:
```bash
$ make
$ ./logarithm
Usage: ./error_function N_CORES
$ time ./logarithm 2
core_num = 0; cpu_num = 0
core_num = 1; cpu_num = 1
    Integral =      18.746549276251

    real    0m10.496s
    user    0m20.960s
    sys 0m0.020s
$ time ./logarithm 1
core_num = 0; cpu_num = 0
    Integral =     18.7465492762492

    real    0m20.583s
    user    0m20.548s
    sys 0m0.028s

# Clean up
$ make clean
```
