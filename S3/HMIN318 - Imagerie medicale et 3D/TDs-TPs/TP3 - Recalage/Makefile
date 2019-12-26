CXXFLAGS      = -O2 -Wall 
LFLAGS        = 
LIBS          = -L/usr/X11R6/lib -lm -lpthread -lX11
INCPATH       = -I../cimg -I.

all: icpImg

icpImg: mesh.o img.o icpImg.o
	g++ $(LFLAGS) -o icpImg mesh.o img.o icpImg.o $(LIBS)

icpImg.o: icpImg.cpp
	gcc -c $(CXXFLAGS) $(INCPATH) -o icpImg.o icpImg.cpp

mesh.o: mesh.cpp
	gcc -c $(CXXFLAGS) $(INCPATH) -o mesh.o mesh.cpp

img.o: img.cpp
	gcc -c $(CXXFLAGS) $(INCPATH) -o img.o img.cpp

clean:
	rm img.o mesh.o icpImg.o

