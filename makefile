 UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
	OPENCV_LIBS = -L/usr/local/lib -pthread -lopencv_imgcodecs -lopencv_imgproc -lopencv_core -lopencv_highgui
	GLFW_LIBS = -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor
	INC = -I/usr/local/include/opencv2/
	FLAGS = -lGL -lGLU -lglut -lBox2d
endif

ifeq ($(UNAME_S),Darwin)
	OPENCV_LIBS = -L ./lib/ -lIlmImf -llibtiff -lopencv_imgcodecs -lippicv -llibwebp -lopencv_imgproc -llibjasper -lopencv_core -lzlib -llibjpeg -lopencv_hal -llibpng -lopencv_highgui 
#	BOX2D_LIBS = -L ./lib/ -lBox2D
	INC = -I ./lib/include/
	FLAGS = -framework OpenGL -framework glut -framework OpenCL -framework Cocoa 
endif

all: main

main: main.cpp 
#	$(CXX) -o main main.cpp $(INC) $(FLAGS) $(OPENCV_LIBS) $(BOX2D_LIBS) 
	$(CXX) -o main main.cpp $(INC) $(OPENCV_LIBS) $(FLAGS)  

clean:
	rm main
