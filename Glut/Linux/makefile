all : GraphicsTest

INCLUDES = -F/System/Library/Frameworks -lglut -lGLU

GraphicsTest: main.cpp
	g++ main.cpp -o GraphicsTest $(INCLUDES) 
	
clean:
	rm -f GraphicsTest *~

