CC=clang
CFLAGS=-Wall -Wextra -pedantic -std=c99

build: square_root_decomposition
build: sparse_table
build: segment_trees

square_root_decomposition: square_root_decomposition.o math_utils.o
	$(CC) $(CFLAGS) $^ -o $@
sparse_table: sparse_table.o math_utils.o
	$(CC) $(CFLAGS) $^ -o $@ 
segment_trees: segment_trees.o math_utils.o
	$(CC) $(CFLAGS) $^ -o $@
math_utils.o: math_utils.c
	$(CC) $(CFLAGS) -c $< -o $@

run-p1:
	./square_root_decomposition
run-p2:
	./sparse_table
run-p3:
	./segment_trees

run-best:
	./sparse_table

clean:
	rm -f *.o
	rm square_root_decomposition
	rm sparse_table
	rm segment_trees

