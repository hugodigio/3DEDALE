UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	OPENCV_LIBS = -L/usr/local/lib -pthread -lopencv_imgcodecs -lopencv_imgproc -lopencv_core -lopencv_highgui
	GLFW_LIBS = -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor
	INC = -I/usr/local/include/opencv2/
	OSX_LIBS=
	FLAGS = -lGL -lGLU -lglut
endif

ifeq ($(UNAME_S),Darwin)
	FLAGS = -framework OpenGL -framework glut
endif

all: main

main: main.o 
	$(CXX) -o $@ main.o $(FLAGS) 

main.o: main.cpp 
	$(CXX) -c main.cpp $(FLAGS) 

clean:
	rm *.o main
