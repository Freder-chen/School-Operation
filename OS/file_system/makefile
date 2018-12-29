main: file_sys.o main.o base.o inode.o debug.o
	gcc -o main main.o file_sys.o base.o inode.o debug.o
	rm file_sys.o main.o base.o inode.o debug.o

main.o: main.c file_sys.h
	gcc -c main.c

file_sys.o: file_sys.c file_sys.h base.h
	gcc -c file_sys.c

base.o: base.c base.h
	gcc -c base.c

inode.o: inode.c base.h file_sys.h
	gcc -c inode.c

debug.o: debug.c debug.h
	gcc -c debug.c

clean:
	rm -f main main.exe