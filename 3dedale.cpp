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

void detection() {
	Mat img = getImage();
	int largeur = img.cols;
	int hauteur = img.rows;

	//Rectangle initial
	Point pt1;
	pt1.x = 250;
	pt1.y = 100;
	
	Point pt2;
	pt2.x = largeur - 250;
	pt2.y = hauteur - 100;
	
	x_init 		= pt1.x;
	y_init 		= pt1.y;
	width_init  = pt2.x - pt1.x;
	height_init = pt2.y - pt1.y;
	
	while(1){
		Mat img = getImage();
		flip(img, img, 1);
		Mat image_lines = img.clone();
    
		rectangle(image_lines, pt1, pt2, (127,255,0), 5);
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
    Mat frame = getImage();  
    flip(frame, frame, 1);
 
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

    string trackerType = "KCF";
 
    Ptr<MultiTracker> multiTracker = MultiTracker::create();
    for (int i = 0; i < nombre_de_trackers; i++)
		  multiTracker->add(TrackerKCF::create(), frame, Rect2d(bboxes[i]));
     
    while(1)
    {     
		Mat frame = getImage();
		flip(frame, frame, 1);
	    bool ok = multiTracker->update(frame);
	    
		if (!ok)
			putText(frame, "Error", Point(25,25), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,255),2);

	  for(unsigned i=0; i<multiTracker->getObjects().size(); i++)
	  {
		rectangle(frame, multiTracker->getObjects()[i], 2, 1);
		
		if (i == 0)
			points_finaux.clear();
		
		Point p;
	
		/* Points dans l'image :
		 * 		0    1
		 * 		2    3
		 * 
		 * +70 permet d'avoir la moyenne
		 */
		
		if (i == 0){
			p.x = multiTracker->getObjects()[i].x + 70;
			p.y = multiTracker->getObjects()[i].y + 70;
			points_finaux.push_back(p);
		}
		
		if (i == 1){
			p.x = multiTracker->getObjects()[i].x + multiTracker->getObjects()[i].width + 70;
			p.y = multiTracker->getObjects()[i].y + 70;
			points_finaux.push_back(p);
		}
		
		if (i == 2){
			p.x = multiTracker->getObjects()[i].x + 70;
			p.y = multiTracker->getObjects()[i].y + multiTracker->getObjects()[i].height + 70;
			points_finaux.push_back(p);
		}
		
		if (i == 3){
			p.x = multiTracker->getObjects()[i].x + multiTracker->getObjects()[i].width + 70;
			p.y = multiTracker->getObjects()[i].y + multiTracker->getObjects()[i].height + 70;
			points_finaux.push_back(p);
		}
	  }
	  homography = findHomography(points_finaux, points_initiaux, CV_RANSAC);

	  imshow("Camera", frame);
	   
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
