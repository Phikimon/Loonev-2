### Task
Implement AVL trees and write unit tests that
execute >95% of program lines.

### Usage:
```bash
$ make
$ ./unit_test
All tests have succeded!

# Check coverage
$ gcov -abcu ./avlt.c
File 'avlt.c'
Lines executed:99.43% of 176
Branches executed:100.00% of 112
Taken at least once:80.36% of 112
Calls executed:72.86% of 70
Creating 'avlt.c.gcov'
$ gcov -abcu ./avln.c
File 'avln.c'
Lines executed:100.00% of 74
Branches executed:100.00% of 42
Taken at least once:90.48% of 42
Calls executed:82.61% of 23
Creating 'avln.c.gcov'

# Check tree structure
$ xdot treeDump001.gv

# Clean up
$ make clean
```
