#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp> //line
#include <opencv2/opencv.hpp>

#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

cv::Mat image;
cv::Mat model;
Mat image_lines, image_fill;
int     treshold_value = 0;

///Fonction permettant la détection des lignes
vector<vector<Point2i>> linesDetection(Mat img, vector<Point2i> coordCorner){
    /// détection des contours avec Canny
    Mat imgCanny;
    
    cvtColor(img, imgCanny, COLOR_RGB2GRAY);
    blur( imgCanny, imgCanny, Size(3,3) );
    Canny(imgCanny, imgCanny, 50, 200, 3);
    
    /// detection des lignes dans le vect lines
    /// vecteur dans lequel sont stockées les lignes
    ///     lignes stockées sous la forme (x1,y1,x2,y2)
    vector<Vec4i> lines;
    /// houghLinesP(imgsource,
    /// vectdest,
    /// distance resolution en pixels
    /// angle resolution en rad
    /// seuil :The minimum number of intersections to “detect” a line
    /// longueur min d'une ligne détectée
    /// max ecart entre pixels de la ligne)
    
    HoughLinesP(imgCanny, lines, 1, CV_PI/180, 30, 15, 10);
    
    /*
    for( size_t i = 0; i < lines.size(); i++ )
    {
        Vec4i l = lines[i];
        line( imgCanny, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255,0,0), 3, LINE_AA);
    }
    
    namedWindow("canny1", WINDOW_AUTOSIZE);
    imshow("canny1", imgCanny);
    */
    /// tableau de couples de points
    vector<vector<Point2i>> vectLines;
    
    ///Initialisation du mask
    Mat mask = Mat::zeros(img.size(), CV_8UC1);
    
    ///Si on a 4 points alors
    ///On déssine un polygone avec ces 4 points dans le mask
    if(coordCorner.size() == 4) {
        ///Conversion des données pour utiliser la fonction fillPoly
        Point coord[1][4];
        coord[0][0] = coordCorner[0];
        coord[0][1] = coordCorner[1];
        coord[0][2] = coordCorner[2];
        coord[0][3] = coordCorner[3];
        ///Nombre de points
        int npt[] = {4};
        ///Pointeur de points
        const Point *ppt[1] = {coord[0]};
        
        fillPoly(mask, ppt, npt, 1, Scalar(255, 255, 255), 8);
    }
    
    for(Vec4i l : lines){
        
        /// couple de points
        vector<Point2i> vectPoints ;
        vectPoints.emplace_back(l[0], l[1]);
        vectPoints.emplace_back(l[2], l[3]);

        ///tracé de la ligne
        if((int)mask.at<uchar>(vectPoints[0].y, vectPoints[0].x) == 255 && (int)mask.at<uchar>(vectPoints[1].y, vectPoints[1].x) == 255) {
            /// ajout du couple au tableau
            vectLines.push_back(vectPoints) ;
            line( imgCanny, vectPoints[0], vectPoints[1], Scalar(255,0,0), 3, LINE_AA);
        }
    }

    namedWindow("canny",WINDOW_AUTOSIZE);
    imshow("canny", imgCanny);
    
    
    //return(filterDouble(vectLines,10));
    return(vectLines);
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



void Contour ( int, void* )
{
    image_lines = image.clone();
    image_fill = image.clone();
    
    vector<Point> pts_contours = contoursConvexHull(image, treshold_value);
    
    polylines( image_lines, pts_contours, true, Scalar(0,0,255), 2 );
    
    fillConvexPoly(image_fill, pts_contours, Scalar(255,0,255));
    
    
    cout << treshold_value << endl;
    imshow("Convex", image_lines);
    imshow("fill", image_fill);
}

int main( int argc, char **argv ) {
    
    if ( argc != 2 ) {
        printf( "Usage: analyseur ImageToLoadAndDisplay\n" );
        return -1;
    }
    
    image       = cv::imread( argv[1]);
    model       = cv::Mat(image.rows, image.cols, CV_8UC3, cv::Scalar(255,0,0));
    
    if ( !image.data ) { /* Check for invalid input */
        printf("Could not open or find the image\n") ;
        return -1;
    }
    
    image_lines = image.clone();
    image_fill = image.clone();
    treshold_value = 40;
    
    
    Contour( 0,0);

    //linesDetection(image, coordCorner);

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
    
    cv::waitKey(0);  /* Wait for a keystroke in the window */
    return 0;
}





