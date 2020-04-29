main: main.o bplus.o node.o
	g++ -ggdb -Wall -pthread -std=c++11 -o main main.o bplus.o node.o

main.o: main.cpp bplus.h
	g++ -ggdb -Wall -pthread -Wno-sign-compare -std=c++11 -c main.cpp

bplus.o: bplus.cpp bplus.h
	g++ -ggdb -Wall -pthread -Wno-sign-compare -std=c++11 -c bplus.cpp 

node.o: node.cpp bplus.h
	g++ -ggdb -Wall -pthread -Wno-sign-compare -std=c++11 -c node.cpp 

clean:
	rm *.o main



