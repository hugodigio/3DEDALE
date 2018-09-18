#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;
using std::vector;

// Convert to string
#define SSTR( x ) static_cast< std::ostringstream & >( \
( std::ostringstream() << std::dec << x ) ).str()

CvCapture* capture;
GLFWwindow* window;

int treshold_value = 40;

int nombre_de_trackers = 4;

Point minmin;
Point minmax;
Point maxmin;
Point maxmax;

void initCamera() {
	capture = cvCaptureFromCAM( 0 );
}

void closeCamera() {
	cvReleaseCapture( &capture );
}

bool initGLFW () {
	glfwInit();
	
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // On veut OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Pour rendre MacOS heureux ; ne devrait pas être nécessaire
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	window = glfwCreateWindow(640, 480, "Test", NULL, NULL);
	glfwMakeContextCurrent(window);
	
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

Mat getImage () {
	IplImage* frame;
	frame = cvQueryFrame( capture );
    Mat image = cvarrToMat(frame);
	return image;
}

vector<Point> contoursConvexHull( Mat image, int tresh)
{
    Mat image_convex;
    vector<vector<Point> > vec_contours;

    cvtColor(image, image_convex, CV_BGR2GRAY);
    threshold( image_convex, image_convex, tresh, 255, 0 ); //Seuillage
    blur(image_convex, image_convex, Size(3, 3));
    
    Canny(image_convex, image_convex, 0, 100, 3, true);
    
    findContours( image_convex, vec_contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE );
 
    vector<Point> ConvexHullPoints;
    vector<Point> pts;
    
    for ( size_t i = 0; i< vec_contours.size(); i++)
        for ( size_t j = 0; j< vec_contours[i].size(); j++)
            pts.push_back(vec_contours[i][j]);
    convexHull( pts, ConvexHullPoints );
    
    cout << ConvexHullPoints << endl;
    
    return ConvexHullPoints;
}

void getMinMax (vector<Point> ConvexHullPoints) {
	//minmin::x = 2;
	
	
	
}

Mat contour ( Mat image )
{
    Mat image_lines = image.clone();
    Mat image_fill = image.clone();
    
    vector<Point> pts_contours = contoursConvexHull(image, treshold_value);
    
    polylines( image_lines, pts_contours, true, Scalar(0,0,255), 2 );
    
    fillConvexPoly(image_fill, pts_contours, Scalar(255,0,255));
    
    return image_fill;
}


int tracking() {
    // Read video
    VideoCapture video("images/video.mov");
     
    // Exit if video is not opened
    if(!video.isOpened())
    {
        cout << "Could not read video file" << endl; 
        return 1; 
    } 
 
    // Read first frame 
    Mat frame; 
    bool ok = video.read(frame); 
 
    // Define initial bounding box 
    // x y width height
    
    Rect bboxes[nombre_de_trackers];
    Rect bbox1(170, 50, 75, 75);
    Rect bbox2(875, 65, 75, 75); 
    Rect bbox3(140, 600, 75, 75);
    Rect bbox4(900, 600, 75, 75); 
    
    bboxes[0] = bbox1;
    bboxes[1] = bbox2; 
    bboxes[2] = bbox3;
    bboxes[3] = bbox4; 
    
    // Create a tracker
    string trackerType = "KCF";
 
    Ptr<MultiTracker> multiTracker = MultiTracker::create();
    for (int i = 0; i < nombre_de_trackers; i++)
		  multiTracker->add(TrackerKCF::create(), frame, Rect2d(bboxes[i]));
 
     
    while(video.read(frame))
    {     
        //Update the tracking result with new frame
	  multiTracker->update(frame);
	 
	  // Draw tracked objects
	  for(unsigned i=0; i<multiTracker->getObjects().size(); i++)
	  {
		rectangle(frame, multiTracker->getObjects()[i], 2, 1);
	  }
	 
	  // Show frame
	  imshow("MultiTracker", frame);
	   
	  // quit on x button
	  if  (waitKey(1) == 27) break;
	 
		}
	
}

int main( int argc, char* argv[] ){
	int a = 0;
	
	if (argc <= 1)
		a = 1;
	else
		a = 2;
		
	//initGLFW();

	
	/* PARTIE TRACKING*/
	if (a == 1) {
		tracking();
	}
	
	/* PARTIE RECONNAISSANCE */
	if (a == 2) {
		initCamera();
		cvNamedWindow( "Camera", CV_WINDOW_AUTOSIZE );
		IplImage* frame;
		
		while(1){
			 Mat img = getImage();

			 flip(img, img, 1);
			 img = contour(img);
			 imshow( "Camera", img );
			 char c = cvWaitKey(33);
			if( c == 27 ) break;
		}
		closeCamera();
		
		cvDestroyWindow( "Camera" );
    }
    return 0;
}
