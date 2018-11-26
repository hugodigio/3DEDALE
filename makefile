UNAME_S := $(shell uname -s)
SOURCES_3D = ./src/OPENGL/2Dto3D.cpp
SOURCS_MENUS = ./src/MASTER/window.cpp ./src/MASTER/mainMenu.cpp ./src/MASTER/game.cpp
ifeq ($(UNAME_S),Linux)
	OPENCV_LIBS = -L/usr/local/lib -pthread -lopencv_imgcodecs -lopencv_imgproc -lopencv_core -lopencv_highgui
	GLFW_LIBS = -lGLU -lGL  -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor -lglut
	INC = -I/usr/local/include/opencv2/
	OSX_LIBS=
endif

#règle all
all : main
 
main: main.o 
	$(CXX) -o main main.o $(SOURCES_3D) $(SOURCS_MENUS) $(OSX_LIBS) $(OPENCV_LIBS) $(GLFW_LIBS)
	rm -rf *.o
	./main
	
clean :
	rm -rf main