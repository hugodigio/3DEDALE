#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>
#include <iostream>
#include <vector>

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
double compteur = 0; //le nombre d'image identiques à la suite

vector <Point> points_initiaux;
vector <Point> points_finaux;

Point start;
Point finish;

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

Mat image_contours(Mat frame) {
	Mat src_gray;
	Mat dst;
	Mat detected_edges;
	
	cvtColor(frame, src_gray, CV_BGR2GRAY);
		
	blur( src_gray, detected_edges, Size(3,3) );

	// Canny detector
	Canny( detected_edges, detected_edges, 30, 90, 3 );

	// Using Canny's output as a mask, we display our result
	dst = Scalar::all(0);

	frame.copyTo( dst, detected_edges);
	
	return dst;
	
}

bool findCircle(Mat image) {
    
    Mat gray;
    cvtColor( image, gray, CV_BGR2GRAY);
    vector<Vec3f> circles;
    
    HoughCircles( gray, circles, CV_HOUGH_GRADIENT, 1, gray.rows/8, 200, 100, 0, 0 );
    
    for( size_t i = 0; i < circles.size(); i++ )
    {
        cout << "(" << circles[i][0] << "," << circles[i][1] << ") , " << circles[i][2] << endl;
    }
    
    if (circles.size() != 0)
		return true;
    return false;
}

void findStartFinish(Mat image) {
	Mat gray;
	Mat blur;
	Mat bw;
	Mat dil;
	Mat er;
		
	vector<vector<Point> > squares;
	vector<vector<Point> > contours;
	vector<Point> approx;
		
	cvtColor(image, gray, CV_BGR2GRAY);
	medianBlur(gray, blur, 3);
	Canny(blur, bw, 50, 150, 3);
		
	dilate(bw, dil, Mat(), Point(-1,-1));

	findContours(dil, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
		
	float area = 0;
		
	for (size_t i = 0; i < contours.size(); i++) {

		approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);
			
		if(approx.size() == 3 ) {
			finish.x = (contours[i][0].x + contours[i][1].x + contours[i][2].x) / 3;
			finish.y = (contours[i][0].y + contours[i][1].y + contours[i][2].y) / 3;
		}
		
		if(approx.size() == 4 && contours[i][0].x > 100) {
			start.x = (contours[i][0].x + contours[i][1].x + contours[i][2].x + contours[i][3].x) / 4;
			start.y = (contours[i][0].y + contours[i][1].y + contours[i][2].y + contours[i][3].y) / 4;
		}
		if(approx.size() == 5) {
			cout << "Penta" << endl;
		}
	}
	//cout << "FINISH : " << finish.x << " " << finish.y << endl;
}

void detection() {
	Mat frame = getImage();
	Mat image_tampon;
	Mat temp;
	Mat result;
	
	bool verif = false;
	
	double pourcentage = 0.0;
	double incertitude = 0.975;
	double valeur_max = 75;
	
	int largeur = frame.cols;
	int hauteur = frame.rows;
	int taille = largeur * hauteur;
	//1280*720 = 921600
	
	char str[200];

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
	
	cvtColor(image_contours(frame), image_tampon, CV_BGR2GRAY);
	
	while(!verif){
		frame = getImage();
		flip(frame, frame, 1);

		cvtColor(image_contours(frame), temp, CV_BGR2GRAY);
		
		compare (temp, image_tampon, result, CMP_EQ);
		pourcentage  = countNonZero(result);
		if (pourcentage > taille * incertitude)
			compteur++;
		else
			compteur = 0;
			
		pourcentage = (compteur/valeur_max)*100;
		sprintf(str,"Acquisition : %d %%",int(pourcentage));
		
		putText(frame, str, Point(25,25), FONT_HERSHEY_SIMPLEX, 1, Scalar(255,255,255),2);
		
		rectangle(frame, pt1, pt2, (255,255,255), 5);
		
		imshow( "Camera", frame );
		
		image_tampon = temp.clone();
		
		char c = cvWaitKey(33);
		if( c == 27 ) break;
		
		if (compteur >= valeur_max)
			verif = true;
	}
	
	verif = false;
	
	Rect myROI (pt1.x + 5, pt1.y + 5, pt2.x - 250 - 5, pt2.y - 100 - 5);
	Mat croppedImage = frame(myROI);
	findStartFinish(croppedImage);
	
	//finish.x += pt1.x - 5;
	//finish.y += pt1.y - 5;
	cout << "START  : " << start.x  << " " << start.y  << endl;
	cout << "FINISH : " << finish.x << " " << finish.y << endl;
	
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
	
	Mat gray;
	cvtColor(frame, gray, CV_BGR2GRAY);
	findWalls(gray);
	
	cout << x_init << " " << y_init << " " << x_init + width_init << " " << y_init + height_init << endl;
}


void tracking() {
    Mat frame = getImage();  
    flip(frame, frame, 1);
 
    // x y width height   
    Rect bboxes[4]; // 4 -> nombre de trackers
    Rect bbox1(x_init - taille_box/2, y_init - taille_box/2, taille_box, taille_box);
    Rect bbox2(x_init + width_init - taille_box/2, y_init - taille_box/2, taille_box, taille_box); 
    Rect bbox3(x_init - taille_box/2, y_init + height_init - taille_box/2, taille_box, taille_box);
    Rect bbox4(x_init + width_init - taille_box/2, y_init + height_init - taille_box/2, taille_box, taille_box); 
    
    bboxes[0] = bbox1;
    bboxes[1] = bbox2; 
    bboxes[2] = bbox3;
    bboxes[3] = bbox4; 

    string trackerType = "KCF";
 
    Ptr<MultiTracker> multiTracker = MultiTracker::create();
    for (int i = 0; i < 4; i++) // 4 -> nombre de trackers
		  multiTracker->add(TrackerKCF::create(), frame, Rect2d(bboxes[i]));
     
    while(1)
    {     
		Mat frame = getImage();
		flip(frame, frame, 1);
		
		//code raph le bg <3
		Mat gray;
		cvtColor(frame, gray, CV_BGR2GRAY);
		findWalls(gray);
		
		bool ok = multiTracker->update(frame);
	    
		if (!ok)
			putText(frame, "Error", Point(25,25), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,255),2);
		

		for(unsigned i=0; i<multiTracker->getObjects().size(); i++)
		{
			rectangle(frame, multiTracker->getObjects()[i], (255,255,255), 3);
		
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
				p.x = multiTracker->getObjects()[i].x + taille_box/2;
				p.y = multiTracker->getObjects()[i].y + taille_box/2;
				points_finaux.push_back(p);
				
				putText(frame, "Finish", Point(finish.x + p.x - 50,finish.y + p.y + 50), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,255),2);
				putText(frame, "Start" , Point(start.x + p.x  - 50,start.y  + p.y + 50), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,255),2);
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
			}
			
			/*
			cout << p.x << " " << p.y << endl;
			if (i == 3)
				cout << endl;
			*/
			
		}
		homography = findHomography(points_finaux, points_initiaux, CV_RANSAC);
		//imshow("Camera", frame);
		
		
	   
		if  (waitKey(1) == 27) break;
	 
		}
	
}
/*
void afficherCercle() {
	
	Mat image_circle;
	Mat img_gray;
    
    while(1)
    {     
		Mat frame = getImage();
		flip(frame, frame, 1);
		image_circle = frame.clone();
		cvtColor( frame, img_gray, CV_BGR2GRAY );
		/// Reduce the noise so we avoid false circle detection
		GaussianBlur( img_gray, img_gray, Size(9, 9), 2, 2 );
		vector<Vec3f> circles;
		
		/// Apply the Hough Transform to find the circles
		HoughCircles( img_gray, circles, CV_HOUGH_GRADIENT, 1, img_gray.rows/8, 200, 100, 0, 0 );
		
		/// Draw the circles detected
		for( size_t i = 0; i < circles.size(); i++ )
		{
			Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
			int radius = cvRound(circles[i][2]);
			// circle center
			circle( image_circle, center, 3, Scalar(0,255,0), -1, 8, 0 );
			// circle outline
			circle( image_circle, center, radius, Scalar(0,0,255), 3, 8, 0 );
		}
		
		imshow( "Camera", image_circle );
		if  (waitKey(1) == 27) break;
	}
}

void afficherTriangle() {
	Mat image;
	while(1){
		image = getImage();
		// needed matrix
		Mat gray;
		Mat blur;
		Mat bw;
		Mat dil;
		Mat er;
		
		vector<vector<Point> > squares;
		vector<vector<Point> > contours;
		vector<Point> approx;
		
		cvtColor(image, gray, CV_BGR2GRAY);
		medianBlur(gray, blur, 3);
		Canny(blur, bw, 50, 150, 3);
		
		// dilate to ensure there is no cut off lines
		dilate(bw, dil, Mat(), Point(-1,-1));
		
		// find all contours
		findContours(dil, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
		
		float area = 0;
		
		// loop to find the squares
		for (size_t i = 0; i < contours.size(); i++) {
			
			// approximate contour with accuracy proportional
			// to the contour perimeter
			approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);
			//fillConvexPoly(image, approx, Scalar(255,0,255));
			
			if(approx.size()==4 ) { 
				
				if(fabs(contourArea(approx) > area)) {
					
					polylines( image, approx, true, Scalar(0,0,255), 2 );
				}
			}
		}
		imshow("Camera", image);
		if  (waitKey(1) == 27) break;
	}
}
*/

int main( int argc, char* argv[] ){
	
	initCamera();
	
	cvNamedWindow( "Camera", CV_WINDOW_AUTOSIZE );
	
	//cvNamedWindow( "Test", CV_WINDOW_AUTOSIZE );
	
	/* PARTIE RECONNAISSANCE */
	detection();
	
	/* PARTIE TRACKING*/
	//tracking();
	
	//afficherCercle();
	//afficherTriangle();
	
	closeCamera();
	
	waitKey(0);
	cvDestroyWindow("Camera");
	return 0;
}
