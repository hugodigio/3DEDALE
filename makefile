UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	OPENCV_LIBS = -L/usr/local/lib -pthread -lopencv_imgcodecs -lopencv_imgproc -lopencv_core -lopencv_highgui
	GLFW_LIBS = -lGL -lglfw -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor
	INC = -I/usr/local/include/opencv2/ 
	OSX_LIBS=
endif

all : Projet
 
main: main.cpp
	$(CXX) -c main.cpp $(INC)
 
Projet: main
	$(CXX) -o Projet main.o $(OSX_LIBS) $(OPENCV_LIBS) $(GLFW_LIBS)
	rm *.o
	
clean :
	rm -rf Projet
