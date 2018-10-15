#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>
#include <iostream>
#include <vector>

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

void detection() {
	Mat frame = getImage();
	Mat image_tampon;
	Mat temp;
	Mat result;
	
	bool verif = false;
	
	double pourcentage = 0.0;
	double incertitude = 0.96;
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
		
		putText(temp, str, Point(25,25), FONT_HERSHEY_SIMPLEX, 1, Scalar(255,255,255),2);
		
		rectangle(temp, pt1, pt2, (255,255,255), 5);
		
		imshow( "Camera", temp );
		
		image_tampon = temp.clone();
		
		char c = cvWaitKey(33);
		if( c == 27 ) break;
		
		if (compteur >= valeur_max)
			verif = true;
	}
	
	//putText(frame, "Error", Point(25,25), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,255),2);
	
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

//Détection en utilisant la touche échap
void detection_old() {
	Mat frame = getImage();
	int largeur = frame.cols;
	int hauteur = frame.rows;

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
		Mat frame = getImage();
		flip(frame, frame, 1);
    
		rectangle(frame, pt1, pt2, (127,255,0), 5);
		imshow( "Camera", frame );
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
				p.x = multiTracker->getObjects()[i].x + taille_box/2;
				p.y = multiTracker->getObjects()[i].y + taille_box/2;
				points_finaux.push_back(p);
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
