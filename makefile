UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	OPENCV_LIBS = -L/usr/local/lib -pthread -lopencv_imgcodecs -lopencv_imgproc -lopencv_core -lopencv_highgui -lopencv_videoio
	GLFW_LIBS = -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor
	INC = -I/usr/local/include/opencv2/
	OSX_LIBS=
endif

all : Projet
 
test: test.cpp
	$(CXX) -c test.cpp $(INC)
 
Projet: test
	$(CXX) -o Projet test.o $(OSX_LIBS) $(OPENCV_LIBS) $(GLFW_LIBS)
	rm *.o
	
clean :
	rm -rf Projet
