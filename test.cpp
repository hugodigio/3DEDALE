#include "opencv2/opencv.hpp"

int main( int argc, char* argv[] ){
      CvCapture* capture = cvCaptureFromCAM( 0 );//Permet la capture de la caméra
      cvNamedWindow( "Afficher une camera", CV_WINDOW_AUTOSIZE );
      IplImage* frame;
      while(1){
           frame = cvQueryFrame( capture );
           cvShowImage( "Afficher une camera", frame );
           char c = cvWaitKey(33);
           if( c == 27 ) break;
      }
      cvReleaseCapture( &capture );
      cvDestroyWindow( "Afficher une camera" );
}
