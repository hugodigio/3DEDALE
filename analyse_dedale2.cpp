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
Mat image_lines, image_fill;

int treshold_value = 0;
int cpt_frame = 0;

vector<Point> contoursConvexHull( Mat image, int tresh) {
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
    if(pts.size() > 0)
        convexHull( pts, ConvexHullPoints );
    
    return ConvexHullPoints;

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

bool initialisation (Mat frame) {
    
    bool initalized = true;
    while(cpt_frame < 50 && initalized) {
        vector<Point> ConvexHullPoints = contoursConvexHull(frame, 40);
        //if()
            initalized = false;
        cpt_frame++;
    }
    
    return initalized;
}

void Contour ( int, void* ) {
    image_lines = image.clone();
    image_fill = image.clone();
    
    vector<Point> pts_contours = contoursConvexHull(image, treshold_value);
    
    polylines( image_lines, pts_contours, true, Scalar(0,0,255), 2 );
    
    fillConvexPoly(image_fill, pts_contours, Scalar(255,0,255));
    
    imshow("Convex", image_lines);
    imshow("fill", image_fill);
}

void maxLocs(const Mat& src, queue<Point>& dst, size_t size)
{
    float maxValue = -1.0f * numeric_limits<float>::max();
    float* srcData = reinterpret_cast<float*>(src.data);
    
    for(int i = 0; i < src.rows; i++)
    {
        for(int j = 0; j < src.cols; j++)
        {
            if(srcData[i*src.cols + j] > maxValue)
            {
                maxValue = srcData[i*src.cols + j];
                
                dst.push(Point(j, i));
                
                // pop the smaller one off the end if we reach the size threshold.
                if(dst.size() > size)
                {
                    dst.pop();
                }
            }
        }
    }
}

/**
 * @function MatchingMethod
 * @brief Trackbar callback
 */
void MatchingMethod( int, void* )
{
    /// Source image to display
    Mat img_display;
    img.copyTo( img_display );
    
    /// Create the result matrix
    int result_cols =  img.cols - templ.cols + 1;
    int result_rows = img.rows - templ.rows + 1;
    
    result.create( result_rows, result_cols, CV_32FC1 );
    
    /// Do the Matching and Normalize
    matchTemplate( img, templ, result, match_method );
    normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );
    
    /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
    if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED )
    {
        result = 1.0 - result;
    }
    
    // get the top 100 maximums...
    queue<Point> locations;
    maxLocs(result, locations, 100);
    
    /// Show me what you got
    while(!locations.empty())
    {
        Point matchLoc = locations.front();
        rectangle( img_display, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(127), 2, 8, 0 );
        rectangle( result, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(127), 2, 8, 0 );
        locations.pop();
    }
    
    imshow( "image_window", img_display );
    imshow( "result_window", result );
    resizeWindow("image_window", 900, 600);
    resizeWindow("result_window", 900, 600);
    
    return;
}

void matching(Mat image, Mat templt)
{
    /// Load image and template
    img = image;
    templ = templt;
    
    /// Create windows
    namedWindow( "image_window", 0 );
    namedWindow( "result_window", 0 );
    
    /// Create Trackbar

    createTrackbar( "Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED",
                   "image_window", &match_method, max_Trackbar, MatchingMethod );
    
    MatchingMethod( 0, 0 );
}


int main( int argc, char **argv ) {
    
    if ( argc < 2 ) {
        printf( "Usage: analyseur ImageToLoadAndDisplay\n" );
        return -1;
    }
    
    image       = imread( argv[1]);
    model       = Mat(image.rows, image.cols, CV_8UC3, cv::Scalar(255,0,0));
    
    if ( !image.data ) { /* Check for invalid input */
        printf("Could not open or find the image\n") ;
        return -1;
    }
    
    // needed matrix
    Mat gray;
    Mat blur;
    Mat bw;
    Mat dil;
    Mat er;
    
    vector<vector<Point> > squares;
    vector<vector<Point> > contours;
    vector<Point> approx;
    
    // convert to grayscale
    cvtColor(image, gray, CV_BGR2GRAY);
    
    // blur for reducing noise
    medianBlur(gray, blur, 3);
    
    // edge detection with Canny
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
        
        
        if(approx.size()==4 ) {
            
            if(fabs(contourArea(approx) > area) {
               area = fabs(contourArea(approx));
                
               
            polylines( image, approx, true, Scalar(0,0,255), 2 );
            
            double area1 = contourArea(approx);
            
            cout << "area0 =" << endl <<
            "area1 =" << area1 << endl <<
            "approx poly vertices" << approx.size() << endl <<
            "area : " << fabs(contourArea(approx)) << endl;
        }
        
    }
    namedWindow("a", 0);
    imshow("a", image);
    resizeWindow("a", 900, 600);
    
    
    
    // Methode de matching pour trouver un motif dans une image
    
    // utilisation : Decommenter et :
    //  ./analyseur lab4.jpg motig.jpg
    //  ./analyseur motifs.jpg motif.jpg
    
    //matching(imread(argv[1]), imread(argv[2]));
    
    
    
    
    // Trouver les contours externes du labyrinthe et les affichers
    
    // utilisation : Decommenter et :
    //  ./analyseur lab3.jpg
    //  ./analyseur ...
    
    /*
    image_lines = image.clone();
    image_fill = image.clone();
    treshold_value = 40;
    
    Contour( 0,0);

    namedWindow("Image", 0);
    imshow( "Image", image );
    resizeWindow("Image", 900, 600);
   
    namedWindow("Convex", 0);
    imshow("Convex", image_lines);
    resizeWindow("Convex", 900, 600);
    moveWindow("Convex", 20,20);
    
    namedWindow("fill", 0);
    imshow("fill", image_fill);
    resizeWindow("fill", 900, 600);
    moveWindow("fill", 20,20);
    
    createTrackbar( "Seuil : ", "Convex", &treshold_value, 255, Contour );
    createTrackbar( "Seuil : ", "fill", &treshold_value, 255, Contour );
    */
    cv::waitKey(0);  // Wait for a keystroke in the window
    return 0;
}





