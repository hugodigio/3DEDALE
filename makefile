OPENCV_LIBS = -L./lib/ -lIlmImf -llibtiff -lopencv_imgcodecs -lippicv -llibwebp -lopencv_imgproc -llibjasper -lopencv_core -lzlib -llibjpeg -lopencv_hal -llibpng -lopencv_highgui
OSX_LIBS = -framework OpenCL -framework Cocoa
INC = -I./include/

all : analyseur

test : test.cpp test.h
	$(CXX) -c test.cpp $(INC)

analyse_dedale : analyse_dedale.cpp
	$(CXX) -std=c++11 -o analyse_dedale.o -c analyse_dedale.cpp $(INC)

analyseur : analyse_dedale
	$(CXX) -std=c++11 -o analyseur analyse_dedale.o $(OSX_LIBS) $(OPENCV_LIBS)

clean :
	rm *.o analyseur
