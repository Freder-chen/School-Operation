huffman_test: ../common.o huffman.o huffman_test.o
	gcc -o huffman_test huffman_test.o huffman.o ../common.o
	rm -f ../common.o huffman.o huffman_test.o

../common.o: ../common.c ../common.h
	gcc -c ../common.c -o ../common.o

huffman.o: huffman.c huffman.h ../common.h
	gcc -c huffman.c -o huffman.o

huffman_test.o: huffman_test.c huffman.h ../common.h
	gcc -c huffman_test.c -o huffman_test.o

clean:
	rm -f huffman_test huffman_test.exe