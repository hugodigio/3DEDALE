#include "opencv2/opencv.hpp"
#include <iostream>
#include <string>
using namespace cv;

String type2str(int type) {
  String r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch ( depth ) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
  }

  r += "C";
  r += (chans+'0');

  return r;
}

int main(int, char**)
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened()){  // check if we succeeded
        std::cout << "fail" << std::endl; 
        return -1;
    }    
    std::cout << "pas fail" << std::endl;
    Mat edges;
    namedWindow("edges",1);
    while(1){
        Mat frame;
        cap >> frame; // get a new frame from camera
        std::cout << "frame: " << frame.cols << "x" << frame.rows << std::endl;
        String ty =  type2str( frame.type() );
        printf("Matrix: %s %dx%d \n", ty.c_str(), frame.cols, frame.rows );
        imshow("edges", frame);
        if(waitKey(30) >= 0) break;
    }

    
    waitKey(0);
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}