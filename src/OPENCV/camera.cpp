#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>
#include <iostream>
#include <vector>
#include <ctime>

#include "camera.hpp"
#include "walls_detector.hpp"

using namespace std;
using namespace cv;
using std::vector;

CvCapture* capture;

Mat homography;

int x_init;
int y_init;
int width_init;
int height_init;

int taille_box = 70;

vector <Point2f> points_initiaux;
vector <Point2f> points_finaux;

Point2f start;
Point2f finish;

//Partie tracker
Rect bboxes[4]; // 4 -> nombre de trackers
string trackerType;
Ptr<MultiTracker> multiTracker;

void initCamera() {
	capture = cvCaptureFromCAM( 0 );
}

void closeCamera() {
	cvReleaseCapture( &capture );
}

void test_camera() {
	while(1) {
		Mat frame = getImage();
		cout << frame.rows << " " << frame.cols << endl;
		imshow("Camera", frame);
	}
}

void start_camera(){
	initCamera();
	cvNamedWindow( "Camera", CV_WINDOW_AUTOSIZE );
}

Mat getImage () {
	IplImage* frame;
	frame = cvQueryFrame( capture );
	Mat image = cvarrToMat(frame);
	flip(image, image, 1);
	return image;
}

vector<Vec4f> detection() {
	Mat frame = getImage();
	Mat temp;
	Mat result;

	bool verif = false;
	
	double pourcentage = 0.0;
	double incertitude = 0.971;//0.975
	double valeur_max = 75;
	
	int largeur = frame.cols;
	int hauteur = frame.rows;
	int taille = largeur * hauteur;
	//1280*720 = 921600
	
	char str[200];
	char c;

	//Rectangle initial
	Point pt1;
	pt1.x = 250;
	pt1.y = 100;
	
	Point pt2;
	pt2.x = largeur - 250;
	pt2.y = hauteur - 100;
	
	//cout << pt1.x << " " << pt1.y << " , " << pt2.x << " " << pt2.y << endl << endl;
	
	x_init 		= pt1.x;
	y_init 		= pt1.y;
	width_init  = pt2.x - pt1.x;
	height_init = pt2.y - pt1.y;

	clock_t startT;
	double duration;
	double retenir;
	vector<Vec4f> lines;
	while (!verif) {
		startT = clock();
		duration = 0;
		while (duration < 3){
			frame = getImage();
				
			rectangle(frame, pt1, pt2, (255,255,255), 5);
			stringstream ss;
			ss << (3.0 - duration);
			string str1 = ss.str();
			putText(frame, str1, Point(25, 25), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,255),2);
			
			imshow("Camera", frame);
			waitKey(33);
			duration = (clock() - startT) / (double)CLOCKS_PER_SEC;
		}
		
		frame = getImage();

		int pixel_crop = -20;
			
		Rect myROI (pt1.x + pixel_crop, pt1.y + pixel_crop, pt2.x - 250 - pixel_crop, pt2.y - 100 - pixel_crop);
		Mat croppedImage = frame(myROI);
		//findStartFinish(croppedImage);

		Mat gray, walls;
		walls = frame.clone();

		cvtColor(croppedImage, gray, CV_BGR2GRAY);

		lines = findWalls(gray);

		for( size_t i = 0; i < lines.size(); i++ ) {
			lines[i][0] += pt1.x + pixel_crop;
			lines[i][1] += pt1.y + pixel_crop;
			lines[i][2] += pt1.x + pixel_crop;
			lines[i][3] += pt1.y + pixel_crop;
		}
		
		for(int i=0; i<lines.size(); i++){
			cout << lines[i] << endl;
		}	

		for( size_t i = 0; i < lines.size(); i++ ) {
			line( walls, Point(lines[i][0], lines[i][1]),
			Point(lines[i][2], lines[i][3]), Scalar(255,0,127), 3, 8 );
		}
		imshow("Camera", walls);
		c = cvWaitKey(0);
		if( c == 32 ) {
			verif = true;
		}
	}
	
	startT = clock();
	duration = 0;
	while (duration < 2){
		frame = getImage();
			
		rectangle(frame, pt1, pt2, (255,255,255), 5);
		stringstream ss;
		ss << (2.0 - duration);
		string str1 = ss.str();
		putText(frame, str1, Point(25, 25), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,255),2);
		
		imshow ("Camera", frame);
		waitKey(33);
		duration = (clock() - startT) / (double)CLOCKS_PER_SEC;
	}
	
	//finish.x += pt1.x - 5;
	//finish.y += pt1.y - 5;
	start.x = 25;
	start.y = 25;
	
	finish.x = -25;
	finish.y = -25;
	
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
	
	cout << x_init << " " << y_init << " " << x_init + width_init << " " << y_init + height_init << endl;
	
	//cvDestroyWindow("Camera");
	return lines;
}


void init_tracking(Mat frame) {
	// x y width height   
    //Rect bboxes[4]; // 4 -> nombre de trackers
    Rect bbox1(x_init - taille_box/2, y_init - taille_box/2, taille_box, taille_box);
    Rect bbox2(x_init + width_init - taille_box/2, y_init - taille_box/2, taille_box, taille_box); 
    Rect bbox3(x_init - taille_box/2, y_init + height_init - taille_box/2, taille_box, taille_box);
    Rect bbox4(x_init + width_init - taille_box/2, y_init + height_init - taille_box/2, taille_box, taille_box); 
    
    bboxes[0] = bbox1;
    bboxes[1] = bbox2; 
    bboxes[2] = bbox3;
    bboxes[3] = bbox4; 

    trackerType = "KCF";
 
    multiTracker = MultiTracker::create();
	for (int i = 0; i < 4; i++) // 4 -> nombre de trackers
		multiTracker->add(TrackerKCF::create(), frame, Rect2d(bboxes[i]));
}

vector<Point2f> tracking(Mat frame) { 
	bool ok = multiTracker->update(frame);
	    
	if (!ok)
		putText(frame, "Error", Point(25,25), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,255),2);
		

	for(unsigned i=0; i<multiTracker->getObjects().size(); i++)
	{
		rectangle(frame, multiTracker->getObjects()[i], (255,255,255), 3);
	
		if (i == 0)
			points_finaux.clear();
		
		Point2f p;
		
		/* Points dans l'image :
		 * 		0    1
		 * 		2    3
		 * 
		 * +70 permet d'avoir la moyenne
		 */
			
		if (i == 0){
			p.x = multiTracker->getObjects()[i].x + taille_box/2;
			p.y = multiTracker->getObjects()[i].y + taille_box/2;
			points_finaux.push_back(p);
				
			putText(frame, "Start" , Point(start.x + p.x + 25, start.y  + p.y + 25), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,255),2);
		}
			
		if (i == 1){
			p.x = multiTracker->getObjects()[i].x + multiTracker->getObjects()[i].width - taille_box/2;
			p.y = multiTracker->getObjects()[i].y + taille_box/2;
			points_finaux.push_back(p);
		}
			
		if (i == 2){
			p.x = multiTracker->getObjects()[i].x + taille_box/2;
			p.y = multiTracker->getObjects()[i].y + multiTracker->getObjects()[i].height - taille_box/2;
				
			points_finaux.push_back(p);
		}
			
		if (i == 3){
			p.x = multiTracker->getObjects()[i].x + multiTracker->getObjects()[i].width - taille_box/2;
			p.y = multiTracker->getObjects()[i].y + multiTracker->getObjects()[i].height - taille_box/2;
			points_finaux.push_back(p);
				
			putText(frame, "Finish", Point(finish.x + p.x - 100, finish.y + p.y - 25), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,255),2);
		}
			
	}
	homography = findHomography(points_finaux, points_initiaux, CV_RANSAC);
	imshow("Camera", frame);
	waitKey(33);
	
	return points_finaux;
	 
}
	
