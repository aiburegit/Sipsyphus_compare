PREFIX=/usr/local
CXX=g++-10
CXXFLAGS=
LD_LIBRARY_PATH="$LD_LIBRARY_PATH":/my/path

all: repcmp

repcmp: main.cpp -lrepcomp
	$(CXX) $(CXXFLAGS) main.cpp -o repcmp  -L$(PREFIX)/lib -lrepcomp 

-lrepcomp: comparator.o converter.o httprequest.o
	export LD_LIBRARY_PATH
	mkdir -p $(PREFIX)/lib
	$(CXX) -shared converter.o comparator.o httprequest.o -o $(PREFIX)/lib/librepcomp.so -lcurl -lrpm

comparator.o: Headers/Comparator.h Sourse/Comparator.cpp
	$(CXX) -c -fPIC Sourse/Comparator.cpp -o comparator.o

converter.o: Headers/Converter.h Sourse/Converter.cpp
	$(CXX) -c -fPIC Sourse/Converter.cpp -o converter.o

httprequest.o: Headers/Httprequest.h Sourse/Httprequest.cpp
	$(CXX) -c -fPIC Sourse/Httprequest.cpp -o httprequest.o

install: all
	mkdir -p $(PREFIX)/bin
	install repcmp $(PREFIX)/bin/