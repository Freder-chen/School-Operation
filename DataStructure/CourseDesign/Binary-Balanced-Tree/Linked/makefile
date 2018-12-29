balanced_tree_test: balanced_tree.o balanced_tree_test.o
	gcc balanced_tree_test.o balanced_tree.o -o balanced_tree_test
	rm -f balanced_tree_test.o balanced_tree.o

balanced_tree.o: balanced_tree.c balanced_tree.h
	gcc -c balanced_tree.c -o balanced_tree.o

balanced_tree_test.o: balanced_tree_test.c
	gcc -c balanced_tree_test.c -o balanced_tree_test.o

run:
	./balanced_tree_test

clean:
	rm -f balanced_tree_test balanced_tree_test.exe
