#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include <sys/time.h>

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

	int largeur = frame.cols;
	int hauteur = frame.rows;
	int taille = largeur * hauteur;
	double ratio = (double)largeur/(double)hauteur;
	int hauteur_ratio = (int)(largeur - 2*(largeur*0.2))/(1.48);
	
	//Rectangle initial
	Point pt1;
	pt1.x = largeur * 0.2; // =250 pour 1280
	pt1.y = (hauteur - hauteur_ratio)/2;; // =100 pour 720
	Point pt2;
	pt2.x = largeur - (largeur*0.2);
	pt2.y = hauteur - (hauteur - hauteur_ratio)/2;;
	
	x_init 		= pt1.x;
	y_init 		= pt1.y;
	width_init  = pt2.x - pt1.x;
	height_init = pt2.y - pt1.y;
	
	//Variables d'affichage
	char str[200];
	char c;
	
	//Temps de tratement
	struct timeval tv1, tv2;
	double duration;
	double temps_traitement = 8.0; // Le temps de traitement de la première boucle
	
	vector<Vec4f> lines;
	
	bool verif = false;
	while (!verif) {
		gettimeofday(&tv1, NULL);
		duration = 0;
		while (duration < temps_traitement){
			frame = getImage();
				
			rectangle(frame, pt1, pt2, (255,255,255), 5);
			stringstream ss;
			ss << (temps_traitement - duration);
			string str1 = "Detection des murs : " + ss.str() + " secondes...";
			putText(frame, str1, Point(25, 25), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,255),2);
			
			imshow("Camera", frame);
			waitKey(33);
			gettimeofday(&tv2, NULL);
			duration = (tv2.tv_sec - tv1.tv_sec);
		}
		
		frame = getImage();

		int pixel_crop = 20;
			
		Rect myROI (pt1.x - pixel_crop, pt1.y - pixel_crop, pt2.x + pixel_crop - pt1.x, pt2.y + pixel_crop - pt1.y);
		Mat croppedImage = frame(myROI);

		Mat gray, walls;
		walls = frame.clone();

		cvtColor(croppedImage, gray, CV_BGR2GRAY);

		lines = findWalls(gray);

		for( size_t i = 0; i < lines.size(); i++ ) {
			lines[i][0] += pt1.x - pixel_crop;
			lines[i][1] += pt1.y - pixel_crop;
			lines[i][2] += pt1.x - pixel_crop;
			lines[i][3] += pt1.y - pixel_crop;
		}
		
		for(int i=0; i<lines.size(); i++){
			cout << lines[i] << endl;
		}	

		for( size_t i = 0; i < lines.size(); i++ ) {
			line( walls, Point(lines[i][0], lines[i][1]),
			Point(lines[i][2], lines[i][3]), Scalar(255,0,127), 3, 8 );
		}
		String message = "Espace pour continuer, n'importe quelle touche pour recommencer.";
		putText(walls, message, Point(25, 25), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,255),2);
		imshow("Camera", walls); // REMPLACER croppedImage par walls
		c = cvWaitKey(0);
		if( c == 32 ) {
			verif = true;
		}
	}
	
	gettimeofday(&tv1, NULL);
	duration = 0;
	temps_traitement = 5.0;  // Le temps de traitement de la deuxième boucle
	while (duration < temps_traitement){
		frame = getImage();
			
		rectangle(frame, pt1, pt2, (255,255,255), 5);
		stringstream ss;
		ss << (temps_traitement - duration);
		string str1 = "Tracking des coins : " + ss.str() + " secondes...";
		putText(frame, str1, Point(25, 25), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,255),2);
		
		imshow ("Camera", frame);
		waitKey(33);
		gettimeofday(&tv2, NULL);
		duration = (tv2.tv_sec - tv1.tv_sec);
	}

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
		
	start.x = 25;
	start.y = 25;
	
	finish.x = -25;
	finish.y = -25;
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
	imshow("Camera", frame);
	waitKey(33);
	
	return points_finaux;
	 
}
	