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

int cpt = 0;

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
    
    return ConvexHullPoints;
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
    // Create a tracker
    string trackerType = "KCF";
 
    Ptr<Tracker> tracker;
 
    #if (CV_MINOR_VERSION < 3)
    {
        tracker = Tracker::create(trackerType);
    }
    #else
    {
        tracker = TrackerKCF::create();
    }
    #endif
    // Read video
    VideoCapture video("video.mov");
     
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
    Rect2d bbox(170, 50, 75, 75); 
 
    // Uncomment the line below to select a different bounding box 
    // bbox = selectROI(frame, false); 
    // Display bounding box. 
    rectangle(frame, bbox, Scalar( 255, 0, 0 ), 2, 1 ); 
 
    imshow("Tracking", frame); 
    tracker->init(frame, bbox);
     
    while(video.read(frame))
    {     
        // Start timer
        double timer = (double)getTickCount();
         
        // Update the tracking result
        bool ok = tracker->update(frame, bbox);
         
        // Calculate Frames per second (FPS)
        float fps = getTickFrequency() / ((double)getTickCount() - timer);
         
        if (ok)
        {
            // Tracking success : Draw the tracked object
            rectangle(frame, bbox, Scalar( 255, 0, 0 ), 2, 1 );
        }
        else
        {
            // Tracking failure detected.
            putText(frame, "Tracking failure detected", Point(100,80), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0,0,255),2);
        }
         
        // Display tracker type on frame
        putText(frame, trackerType + " Tracker", Point(100,20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50,170,50),2);
         
        // Display FPS on frame
        putText(frame, "FPS : " + SSTR(int(fps)), Point(100,50), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50,170,50), 2);
 
        // Display frame.
        imshow("Tracking", frame);
         
        // Exit if ESC pressed.
        int k = waitKey(1);
        if(k == 27)
        {
            break;
        }
 
    }
	
}

int main( int argc, char* argv[] ){
	
	initCamera();
	//initGLFW();
	//closeCamera();
	/*
	do{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Vérifie si on a appuyé sur la touche échap (ESC) ou si la fenêtre a été fermée
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
	glfwWindowShouldClose(window) == 0 );
	*/
	
	tracking();
	
    //CvCapture* capture = cvCaptureFromCAM( 0 );
    cvNamedWindow( "Camera", CV_WINDOW_AUTOSIZE );
    //IplImage* frame;
    /*
    while(1){
         Mat img = getImage();

         flip(img, img, 1);
         img = contour(img);
         imshow( "Camera", img );
         char c = cvWaitKey(33);
        if( c == 27 ) break;
    }
    //cvWaitKey(0);
    
    cvDestroyWindow( "Camera" );
    */
}
