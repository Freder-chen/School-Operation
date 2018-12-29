sort_analysis: sort.o sort_analysis.o
	gcc sort.o sort_analysis.o -o sort_analysis
	rm -f sort.o sort_analysis.o

sort.o: sort.c sort.h
	gcc -c sort.c -o sort.o

sort_analysis.o: sort_analysis.c
	gcc -c sort_analysis.c -o sort_analysis.o

run:
	./sort_analysis

clean:
	rm -f sort_analysis sort_analysis.exe
