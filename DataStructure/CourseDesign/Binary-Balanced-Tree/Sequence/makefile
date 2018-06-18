bst_test: bst.o bst_test.o
	gcc bst_test.o bst.o -o bst_test
	rm -f bst_test.o bst.o

bst.o: bst.c bst.h
	gcc -c bst.c -o bst.o

bst_test.o: bst_test.c
	gcc -c bst_test.c -o bst_test.o

clean:
	rm -f bst_test bst_test.exe
