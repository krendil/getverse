CC = gcc
CXX = g++
CFLAGS=--std=gnu99 -c
CXXFLAGS=$(shell pkg-config --cflags sword) -Wall -c
LDFLAGS=$(shell pkg-config --libs sword) -lstdc++
DEBUGFLAGS = -ggdb
RELEASEFLAGS = -O3

release: CFLAGS += $(RELEASEFLAGS)
debug: CFLAGS += $(DEBUGFLAGS)

release: CXXFLAGS += $(RELEASEFLAGS)
debug: CXXFLAGS += $(DEBUGFLAGS)

release debug: getverse

%.o: %.c
	$(CC) -o $@ $(CFLAGS) $^
	
%.o: %.cpp
	$(CXX) -o $@ $(CXXFLAGS) $^

getverse: getverse.o swordwrapper.o
	$(CC) -o $@ $(LDFLAGS) $^

clean:
	rm -f *.o getverse

test:
	@echo CFLAGS IS $(LDFLAGS)

