#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp> //line
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>

#include <iostream>
#include <vector>
#include <limits>
#include <queue>

using namespace cv;
using namespace std;

/// Global Variables
Mat img; Mat templ; Mat result;

int match_method;
int max_Trackbar = 5;


cv::Mat image;
cv::Mat model;
Mat image_rect, image_circle;

int treshold_value = 0;
int cpt_frame = 0;

Rect contoursConvexHull( Mat image, int tresh) {
    Mat image_convex;
    vector<vector<Point> > vec_contours;

    cvtColor(image, image_convex, CV_BGR2GRAY);
    threshold( image_convex, image_convex, tresh, 255, 0 ); //Seuillage
    blur(image_convex, image_convex, Size(3, 3));
    
    Canny(image_convex, image_convex, 50, 150, 3, true);
    
    findContours( image_convex, vec_contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE );
 
    vector<Point> ConvexHullPoints;
    vector<Point> pts;
    
    for ( size_t i = 0; i< vec_contours.size(); i++)
        for ( size_t j = 0; j< vec_contours[i].size(); j++)
            pts.push_back(vec_contours[i][j]);
    
    if(pts.size() > 0)
        convexHull( pts, ConvexHullPoints );
    
    Rect bounding_rect = boundingRect( ConvexHullPoints );
    
    return bounding_rect;

    /* // Peut-etre pour contour
    Mat drawing = Mat::zeros(image_convex.size(), CV_8UC3);
    for (int i = 0; i< contours.size(); i++)
    {
        Scalar color = Scalar( 255,0,255);
        drawContours( drawing, contours, i, color, 2 );
    }
    polylines( drawing, ConvexHullPoints, true, Scalar(0,0,255), 2 );
    imshow("Contours", drawing);
     */
}


void Contour ( int, void* ) {
  
    /*
     image_lines = image.clone();
     image_fill = image.clone();
     
     vector<vector<Point> > = ....
     polylines( image_lines, pts_contours, true, Scalar(0,0,255), 2 );
     fillConvexPoly(image_fill, pts_contours, Scalar(255,0,255));

     imshow("Convex", image_lines);
     imshow("fill", image_fill);
    */
    image_rect = image.clone();
    
    Rect rect = contoursConvexHull(image_rect, treshold_value);
    
    cout << rect << endl;
    
    rectangle(image_rect, rect, Scalar(0,255,0), 3);
    
    imshow( "Rectangle", image_rect );
    
}

void findCircle(Mat image) {
    
    Mat img_gray;
    image_circle = image.clone();
    cvtColor( image, img_gray, CV_BGR2GRAY );
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

    imshow( "Circle", image_circle );
}


void findPoly(Mat image) {
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
    Canny(blur, bw, 0, 50);
    
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

        if(approx.size()==3 ) { //triangle
            
            if(fabs(contourArea(approx) > area)) {
                area = fabs(contourArea(approx));
                
                polylines( image, approx, true, Scalar(0,0,255), 2 );
                
                double area1 = contourArea(approx);
                
                cout << "area0 =" << endl <<
                "area1 =" << area1 << endl <<
                "approx poly vertices" << approx.size() << endl <<
                "area : " << fabs(contourArea(approx)) << endl;
            }
        }
    }
    namedWindow("a", 0);
    imshow("a", image);
    resizeWindow("a", 900, 600);
}
               
int main( int argc, char **argv ) {
    
    if ( argc < 2 ) {
        printf( "Usage: analyseur ImageToLoadAndDisplay\n" );
        return -1;
    }
    
    image       = imread( argv[1]);
    image_rect  = Mat(image.rows, image.cols, CV_8UC3, cv::Scalar(255,0,0));
    image_circle  = Mat(image.rows, image.cols, CV_8UC3, cv::Scalar(255,0,0));
    
    if ( !image.data ) { /* Check for invalid input */
        printf("Could not open or find the image\n") ;
        return -1;
    }
    
    // Trouver les contours externes du labyrinthe et les affichers
    
    // utilisation : Decommenter et :
    //  ./analyseur lab3.jpg
    //  ./analyseur ...
    
    findPoly(image);
    findCircle(image);
    
    treshold_value = 40;
    
    Contour( 0,0);

    namedWindow("Image", 0);
    imshow( "Image", image );
    resizeWindow("Image", 900, 600);
    
    namedWindow("Rectangle", 0);
    imshow( "Rectangle", image_rect );
    resizeWindow("Rectangle", 900, 600);
    
    namedWindow("Circle", 0);
    imshow( "Circle", image_circle );
    resizeWindow("Circle", 900, 600);
    
    createTrackbar( "Seuil : ", "Rectangle", &treshold_value, 255, Contour );

    cv::waitKey(0);  // Wait for a keystroke in the window
    return 0;
}





