# MAKEFILE

CXX = g++
CFLAGS = -Wall

INCLUDE = -I/usr/local/include
LDFLAGS = -L/usr/local/lib
LDLIBS = -lcurl -lcrypto -ljsoncpp

SOURCES = main.cpp
OUT = test


all: build

restclient.o : restclient.h restclient.cpp
	$(CXX) $(CFLAGS) -c $(INCLUDE) $(CFLAGS) $(LDFLAGS) $(LDLIBS) restclient.cpp

WykopSDK.o : WykopSDK.h WykopSDK.cpp
	$(CXX) $(CFLAGS) -c $(INCLUDE) -I. $(CFLAGS) $(LDFLAGS) $(LDLIBS) WykopSDK.cpp


build: WykopSDK.o restclient.o main.cpp
	$(CXX) -o $(OUT) restclient.o WykopSDK.o $(INCLUDE) $(CFLAGS) $(LDFLAGS) $(LDLIBS) $(SOURCES)

clean:
	rm -f *.o
	rm -f $(OUT)
	rm -rf obj
