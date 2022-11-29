gcc src/master.c -lncurses -lbmp -lm -o bin/master
gcc src/processA.c -lncurses -lbmp -lm -o bin/ProcessA
gcc src/processB.c -lncurses -lbmp -lm -o bin/processB
./bin/master
