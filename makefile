UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	OPENCV_LIBS = -L/usr/local/lib -pthread -lopencv_imgcodecs -lopencv_imgproc -lopencv_core -lopencv_highgui -lopencv_videoio -lopencv_tracking -lopencv_calib3d
	GLFW_LIBS = -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor
	INC = -I/usr/local/include/opencv2/
	OSX_LIBS = 
endif

all : Projet
 
camera: src/detection/camera.cpp
	$(CXX) -c src/detection/camera.cpp $(INC)

wallsDetector: src/detection/walls_detector.cpp
	$(CXX) -c src/detection/walls_detector.cpp $(INC)
 
Projet: camera wallsDetector
	$(CXX) -o Projet camera.o walls_detector.o $(OSX_LIBS) $(OPENCV_LIBS) $(GLFW_LIBS)
	rm *.o
	
clean :
	rm -rf Projet
