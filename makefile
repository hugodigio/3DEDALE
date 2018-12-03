UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	OPENCV_LIBS = -L/usr/local/lib -pthread -lopencv_imgcodecs -lopencv_imgproc -lopencv_core -lopencv_highgui -lopencv_videoio -lopencv_tracking -lopencv_calib3d
	INC = -I/usr/local/include/opencv2/
	GLFW_LIBS = -lGL -lGLU  -lpthread -ldl -lglut
	OSX_LIBS = 
endif

all : Projet
 
camera: src/OPENCV/camera.cpp
	$(CXX) -c src/OPENCV/camera.cpp $(INC)

wallsDetector: src/OPENCV/walls_detector.cpp
	$(CXX) -c src/OPENCV/walls_detector.cpp $(INC)

inclinaison: src/CONVERT/inclinaison.cpp
	$(CXX) -c src/CONVERT/inclinaison.cpp $(INC)
	
game: src/MASTER/game.cpp
	$(CXX) -c src/MASTER/game.cpp $(INC)

mainMenu: src/MASTER/mainMenu.cpp
	$(CXX) -c src/MASTER/mainMenu.cpp $(INC)

window: src/MASTER/window.cpp
	$(CXX) -c src/MASTER/window.cpp $(INC)

2Dto3D: src/OPENGL/2Dto3D.cpp
	$(CXX) -c src/OPENGL/2Dto3D.cpp $(INC)
 
st: main.cpp
	$(CXX) -c main.cpp $(INC)
 
Projet: camera wallsDetector inclinaison window mainMenu game 2Dto3D st
	$(CXX) -o Projet camera.o walls_detector.o inclinaison.o main.o window.o mainMenu.o game.o 2Dto3D.o $(OSX_LIBS) $(OPENCV_LIBS) $(GLFW_LIBS)
	rm *.o
	./Projet
	
clean :
	rm -rf Projet
