
CXX = g++ -fPIC

all: HashTableVoidTest IRCServer

HashTableVoidTest: HashTableVoidTest.cc HashTableVoid.cc
	g++ -g -o HashTableVoidTest HashTableVoidTest.cc HashTableVoid.cc

IRCServer: IRCServer.cc LinkedList.cpp
	g++ -g -o IRCServer IRCServer.cc HashTableVoid.cc LinkedList.cpp
	git add -A  >> .local.git.out
	git commit -a -m "Make IRCServer" >> .local.git.out

clean:
	rm -f *.out
	rm -f *.o HashTableVoidTest IRCServer


