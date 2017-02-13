DFS = blockdevice.cpp filsys.cpp inode.cpp main.cpp

all: $(DFS)
	g++ -std=c++11 -o dummyFS $(DFS)

run:
	g++ -std=c++11 -o dummyFS $(DFS)
	./dummyFS

clean:
	rm -rf dummyFS
