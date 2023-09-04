PREFIX=/usr/local
CXX=g++-10
CXXFLAGS=

all: repcmp

repcmp: main.cpp -lrepcomp
	$(CXX) $(CXXFLAGS) main.cpp -o repcmp -lrepcomp -lcurl -lrpm

-lrepcomp: comparator.o converter.o httprequest.o
	mkdir -p $(PREFIX)/lib
	$(CXX) -shared converter.o comparator.o httprequest.o -o $(PREFIX)/lib/librepcomp.so 

comparator.o: Headers/Comparator.h Sourse/Comparator.cpp
	$(CXX) -c -fPIC Sourse/Comparator.cpp -o comparator.o

converter.o: Headers/Converter.h Sourse/Converter.cpp
	$(CXX) -c -fPIC Sourse/Converter.cpp -o converter.o

httprequest.o: Headers/Httprequest.h Sourse/Httprequest.cpp
	$(CXX) -c -fPIC Sourse/Httprequest.cpp -o httprequest.o

install: all
	mkdir -p $(PREFIX)/bin
	install repcmp $(PREFIX)/bin/