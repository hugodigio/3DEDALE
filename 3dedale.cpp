#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;
using std::vector;

CvCapture* capture;

int treshold_value = 40;

int nombre_de_trackers = 4;

Mat homography;

int x_init;
int y_init;
int width_init;
int height_init;

vector <Point> points_initiaux;
vector <Point> points_finaux;

void initCamera() {
	capture = cvCaptureFromCAM( 0 );
}

void closeCamera() {
	cvReleaseCapture( &capture );
}

Mat getImage () {
	IplImage* frame;
	frame = cvQueryFrame( capture );
    Mat image = cvarrToMat(frame);
	return image;
}

Rect contoursConvexHull( Mat image, int tresh)
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
    
    Rect bounding_rect = boundingRect (ConvexHullPoints);
    
    x_init = bounding_rect.x;
    y_init = bounding_rect.y;
    width_init = bounding_rect.width;
    height_init = bounding_rect.height;
    
    return bounding_rect;
}

void detection ()
{
	while(1){
		Mat img = getImage();
		flip(img, img, 1);
		Mat image_lines = img.clone();
    
		Rect pts_contours = contoursConvexHull(img, treshold_value);
    
		rectangle(image_lines, pts_contours, (127,255,0), 5);
		imshow( "Camera", image_lines );
		char c = cvWaitKey(33);
		if( c == 27 ) break;
	}
	
	Point p;
	
	p.x = x_init;
	p.y = y_init;
	points_initiaux.push_back(p);
	
	p.x = x_init + width_init;
	p.y = y_init;
	points_initiaux.push_back(p);
	
	p.x = x_init;
	p.y = y_init + height_init;
	points_initiaux.push_back(p);
	
	p.x = x_init + width_init;
	p.y = y_init + height_init;
	points_initiaux.push_back(p);
}


void tracking() {
    // Read first frame 
    Mat frame = getImage();  
    flip(frame, frame, 1);
 
    // Define initial bounding box 
    // x y width height
    
    Rect bboxes[nombre_de_trackers];
    Rect bbox1(x_init - 35, y_init - 35, 70, 70);
    Rect bbox2(x_init + width_init - 35, y_init - 35, 70, 70); 
    Rect bbox3(x_init - 35, y_init + height_init - 35, 70, 70);
    Rect bbox4(x_init + width_init - 35, y_init + height_init - 35, 70, 70); 
    
    bboxes[0] = bbox1;
    bboxes[1] = bbox2; 
    bboxes[2] = bbox3;
    bboxes[3] = bbox4; 
    
    // Create a tracker
    string trackerType = "KCF";
 
    Ptr<MultiTracker> multiTracker = MultiTracker::create();
    for (int i = 0; i < nombre_de_trackers; i++)
		  multiTracker->add(TrackerKCF::create(), frame, Rect2d(bboxes[i]));
 
     
    while(1)
    {     
		Mat frame = getImage();
		flip(frame, frame, 1);
      //Update the tracking result with new frame
	  multiTracker->update(frame);
	 
	  // Draw tracked objects
	  for(unsigned i=0; i<multiTracker->getObjects().size(); i++)
	  {
		rectangle(frame, multiTracker->getObjects()[i], 2, 1);
		
		points_finaux.clear();
		
		Point p;
	
		p.x = multiTracker->getObjects()[i].x;
		p.y = multiTracker->getObjects()[i].y;
		points_finaux.push_back(p);
		
		p.x = multiTracker->getObjects()[i].x + multiTracker->getObjects()[i].width;
		p.y = multiTracker->getObjects()[i].y;
		points_finaux.push_back(p);
		
		p.x = multiTracker->getObjects()[i].x;
		p.y = multiTracker->getObjects()[i].y + multiTracker->getObjects()[i].height;
		points_finaux.push_back(p);
		
		p.x = multiTracker->getObjects()[i].x + multiTracker->getObjects()[i].width;
		p.y = multiTracker->getObjects()[i].y + multiTracker->getObjects()[i].height;
		points_finaux.push_back(p);

		homography = findHomography(points_finaux, points_initiaux, CV_RANSAC);
		
		//cout << homography << endl;
	  }
	 
	  // Show frame
	  imshow("Camera", frame);
	   
	  // quit on x button
	  if  (waitKey(1) == 27) break;
	 
		}
	
}

int main( int argc, char* argv[] ){
	
	initCamera();
	
	cvNamedWindow( "Camera", CV_WINDOW_AUTOSIZE );
	
	/* PARTIE RECONNAISSANCE */
	detection();
	
	/* PARTIE TRACKING*/
	tracking();
	
	closeCamera();
		
	cvDestroyWindow("Camera");
    return 0;
}
