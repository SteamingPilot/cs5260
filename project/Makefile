all:
	gcc -g -fopenmp -o ./parallel-solver ./parallel-solver.c -lm
	gcc -g -o ./serial-solver ./serial-solver.c -lm

serial:
	gcc -g -o ./serial-solver ./serial-solver.c 

parallel:
	gcc -g -fopenmp -lm -o ./parallel-solver ./parallel-solver.c

test: t.c
	gcc -g -o ./t ./t.c

test_run:
	./t
