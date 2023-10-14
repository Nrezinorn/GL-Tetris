CXX = g++
CXXFLAGS = -w -Wall -g -DDEBUG -I/usr/include/glm `pkg-config --cflags freetype2`
LDFLAGS =  `/usr/bin/sdl2-config --libs` `pkg-config --libs SDL2_mixer` `pkg-config --libs gl` `pkg-config freetype2 --libs`

PROG=GLTetris2022.bin

#give up on object files in a folder of their own for now
OBJDIR=obj

src = $(wildcard src/*.cpp) \
	$(wildcard src/Tetris/*.cpp)
obj = $(src:.cpp=.o)
dep = $(obj:.o=.d)

#http://nuclear.mutantstargoat.com/articles/make/  
# buy this guy a beer or soda

all:  $(PROG) 

$(PROG): $(obj)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS)

.PHONY: clean

clean:
	rm -f $(obj)
	rm -f $(PROG)
