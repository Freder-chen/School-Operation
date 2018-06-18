graph_test: ../../Queue/link_queue.o graph.o graph_test.o
	gcc ../../Queue/link_queue.o graph_test.o graph.o -o graph_test
	rm -f graph_test.o graph.o ../../Queue/link_queue.o

../../Queue/link_queue.o: ../../Queue/link_queue.c ../../Queue/link_queue.h
	gcc -c ../../Queue/link_queue.c -o ../../Queue/link_queue.o

graph.o: graph.c graph.h
	gcc -c graph.c -o graph.o

graph_test.o: graph_test.c
	gcc -c graph_test.c -o graph_test.o

clean:
	rm -f graph_test graph_test.exe