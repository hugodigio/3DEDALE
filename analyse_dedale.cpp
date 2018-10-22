#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp> //line
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>

#include <iostream>
#include <vector>
#include <list>
#include <limits>
#include <queue>

using namespace cv;
using namespace std;

void findWalls(Mat image, int tresh_angle);

/// Global Variables
Mat img; Mat templ; Mat result;
Mat image2;
int match_method;
int max_Trackbar = 5;
int tresh_ang = 0;

cv::Mat image;
cv::Mat model;
Mat image_rect, image_circle, image_walls;

int treshold_value = 0;
int cpt_frame = 0;


static void onMouse(int event,int x,int y,int,void*)
{
    //this function will be called every time you move your mouse over the image
    // the coordinates will be in x and y variables
    Mat image_mouse      = Mat(500, 500, CV_8UC3, cv::Scalar(255,0,0));
    string str = "a : " + to_string(x) + ", " + to_string(y) +  " .";
    putText(image_mouse, str, Point(100,100),FONT_HERSHEY_COMPLEX_SMALL,1,Scalar(10,10,255));

    imshow("mouse",image_mouse);
}


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
    namedWindow("Triangle", 0);
    imshow("Triangle", image);
    resizeWindow("Triangle", 900, 600);
}

//Si a est compris dans l'interface [b +- inter]
bool compris(int a, int b, int inter) {
    if( b - inter <= a && b + inter >= a)
        return true;
    return false;
}


double isVectorsCollinear(Vec4i segment_1, Vec4i segment_2)
{
    Point vec_1 = Point(segment_1[2], segment_1[3]) - Point(segment_1[0], segment_1[1]);
    Point vec_2 = Point(segment_2[2], segment_2[3]) - Point(segment_2[0], segment_2[1]);
    return fabs(norm(vec_1) * norm(vec_2) - vec_1.dot(vec_2));
}

bool estCompris(double a, double b, double inter) {
    
    if(fabs(a-b) <= 0 + inter) {
        return true;
    }
    return false;
}

//x0, y0, x1, y1
//0,  1,  2,  3
bool equalLines (Vec4i vec_a, Vec4i vec_b) {
    
    double collinear = isVectorsCollinear(vec_a, vec_b);
    int eps = 100;
    if( collinear >= - 5 && collinear <= 5 &&
        ((estCompris(vec_a[0], vec_b[0], eps) && // axe X
        (   (vec_a[1] >= vec_b[1] - eps && vec_a[1] <= vec_b[1] + eps) || // y0 de a : y0 de b
            (vec_a[3] >= vec_b[3] - eps && vec_a[3] <= vec_b[3] + eps)    // y1 de a : y1 de b
        )) ||
        ((estCompris(vec_a[1], vec_b[1], eps) && // axe Y
        (   (vec_a[0] >= vec_b[0] - eps && vec_a[0] <= vec_b[0] + eps) || // x0 de a : x0 de b
            (vec_a[2] >= vec_b[2] - eps && vec_a[2] <= vec_b[2] + eps)    // x1 de a : x1 de b
        )))) ) {
             return true;
    }
    return false;
}

//Donner le meme sens a toutes les lignes ( v ^ )
vector<Vec4i> normalizedLines ( vector<Vec4i> lines) {
    
    int x0, y0, x1, y1;
    
    for( size_t i = 0; i < lines.size(); i++ ) {
        
        x0 = lines[i][0];
        y0 = lines[i][1];
        x1 = lines[i][2];
        y1 = lines[i][3];
        
        if(y0 > y1 + 100) {
            lines[i][0] = x1;
            lines[i][1] = y1;
            lines[i][2] = x0;
            lines[i][3] = y0;
        }
    }
    return lines;
}

vector<Vec4i> mixLines ( vector<Vec4i> lines) {
    
    int fontFace = FONT_HERSHEY_COMPLEX_SMALL;
    double fontScale=3.5;
    int cpt = 0;
    int largeur = 200;
    int x0a, y0a, x1a, y1a, x0b, y0b, x1b, y1b;
    
    for( size_t i = 0; i < lines.size(); i++ )
    {
        x0a = lines[i][0];
        y0a = lines[i][1];
        x1a = lines[i][2];
        y1a = lines[i][3];
        for( size_t j = 0; j < lines.size(); j++ ) {
            
            if( i != j) {
                x0b = lines[j][0];
                y0b = lines[j][1];
                x1b = lines[j][2];
                y1b = lines[j][3];
                
                //Horizontale
                if( (x0b >= x0a && x0b <= x1a && x1b >= x1a) && estCompris(y0a, y0b, largeur) && estCompris(y1a, y1b, largeur)) {
                    
                    cpt ++;
                    putText(image2, to_string(cpt), Point(x0b,y0b),fontFace,fontScale,Scalar(10,50,255));
                    
                    Vec4i ligne;
                    ligne[0] = x0a;    //x0
                    ligne[1] = y0a;    //y0
                    ligne[2] = x1b;    //x1
                    ligne[3] = y1a;    //y1
                    
                    line( image2, Point(x0a, y0a), Point(x1a, y1a), Scalar(127,255,100), 3, 8 );
                    line( image2, Point(x0b, y0b), Point(x1b, y1b), Scalar(127,255,100), 3, 8 );
                    
                    if(i > j) {
                        lines.erase(lines.begin() + i);
                        lines.erase(lines.begin() + j);
                    } else {
                        lines.erase(lines.begin() + j);
                        lines.erase(lines.begin() + i);
                    }
                    
                    i = 0;
                    j = 0;
                    
                    lines.push_back(ligne);
                    break;
                }
                
                //Verticale
                if( (y0b >= y0a && y0b <= y1a && y1b >= y1a) && estCompris(x0a, x0b, largeur) && estCompris(x1a, x1b, largeur)) {
                    
                    cpt ++;
                    putText(image2, to_string(cpt), Point(x0b,y0b),fontFace,fontScale,Scalar(10,50,255));
                    
                    Vec4i ligne;
                    ligne[0] = x0a;    //x0
                    ligne[1] = y0a;    //y0
                    ligne[2] = x1a;    //x1
                    ligne[3] = y1b;    //y1
                    
                    line( image2, Point(x0a, y0a), Point(x1a, y1a), Scalar(127,255,100), 3, 8 );
                    line( image2, Point(x0b, y0b), Point(x1b, y1b), Scalar(127,255,100), 3, 8 );
                    
                    if(i > j) {
                        lines.erase(lines.begin() + i);
                        lines.erase(lines.begin() + j);
                    } else {
                        lines.erase(lines.begin() + j);
                        lines.erase(lines.begin() + i);
                    }
                    
                    i = 0;
                    j = 0;
                    
                    lines.push_back(ligne);
                    break;
                }
            }
        }
    }
    return lines;
}


vector<Vec4i> deleteLineMin ( vector<Vec4i> lines) {
    int fontFace = FONT_HERSHEY_COMPLEX_SMALL;
    double fontScale=3.5;
    int cpt = 0;
    int largeur = 200;
    int x0a, y0a, x1a, y1a, x0b, y0b, x1b, y1b;
    
    // Suppression des lignes proches
    for( size_t i = 0; i < lines.size(); i++ )
    {
        x0a = lines[i][0];
        y0a = lines[i][1];
        x1a = lines[i][2];
        y1a = lines[i][3];
        for( size_t j = 0; j < lines.size(); j++ )
        {
            if( i != j) {
                x0b = lines[j][0];
                y0b = lines[j][1];
                x1b = lines[j][2];
                y1b = lines[j][3];
                
                // Horizontale
                if( (x0b >= x0a && x1b <= x1a) && estCompris(y0a, y0b, largeur) && estCompris(y1a, y1b, largeur)) {
                    cpt ++;
                    putText(image2, to_string(cpt), Point(x0b,y0b),fontFace,fontScale,Scalar(10,10,255));
                    
                    line( image2, Point(x0b, y0b), Point(x1b, y1b), Scalar(127,255,200), 3, 8 );
                    
                    lines.erase(lines.begin() + j);

                    j--;
                    if(j<=i) {
                        i--;
                    }
                    
                    continue;
                }
            }
        }
    }
    return lines;
}




vector<Vec4i> mixAlignedLines ( vector<Vec4i> lines) {
    
    int fontFace = FONT_HERSHEY_COMPLEX_SMALL;
    double fontScale=3.5;
    int cpt = 0;
    int largeur = 50;
    int espace = 200;
    int x0a, y0a, x1a, y1a, x0b, y0b, x1b, y1b;
    
    for( size_t i = 0; i < lines.size(); i++ )
    {
        x0a = lines[i][0];
        y0a = lines[i][1];
        x1a = lines[i][2];
        y1a = lines[i][3];
        for( size_t j = 0; j < lines.size(); j++ ) {
            
            if( i != j) {
                x0b = lines[j][0];
                y0b = lines[j][1];
                x1b = lines[j][2];
                y1b = lines[j][3];
                
                //Horizontale
                if( (x0b >= x1a && x0b <= x1a + espace && x1b >= x1a) && estCompris(y0a, y1b, largeur) && estCompris(y1a, y0b, largeur)) {
                    
                    cpt ++;
                    putText(image2, to_string(cpt), Point(x0b,y0b),fontFace,fontScale,Scalar(10,50,255));
                    
                    Vec4i ligne;
                    ligne[0] = x0a;    //x0
                    ligne[1] = y0a;    //y0
                    ligne[2] = x1b;    //x1
                    ligne[3] = y1a;    //y1
                    
                    line( image2, Point(x0a, y0a), Point(x1a, y1a), Scalar(127,255,100), 3, 8 );
                    line( image2, Point(x0b, y0b), Point(x1b, y1b), Scalar(127,255,100), 3, 8 );
                    
                    if(i > j) {
                        lines.erase(lines.begin() + i);
                        lines.erase(lines.begin() + j);
                    } else {
                        lines.erase(lines.begin() + j);
                        lines.erase(lines.begin() + i);
                    }
                    
                    i = 0;
                    j = 0;
                    
                    lines.push_back(ligne);
                    break;
                }
                
                //Verticale
                if( (y0b >= y1a && y0b <= y1a + espace && y1b >= y1a) && estCompris(x0a, x1b, largeur) && estCompris(x1a, x0b, largeur)) {
                    
                    cpt ++;
                    putText(image2, to_string(cpt), Point(x0b,y0b),fontFace,fontScale,Scalar(10,50,255));
                    
                    Vec4i ligne;
                    ligne[0] = x0a;    //x0
                    ligne[1] = y0a;    //y0
                    ligne[2] = x0a;    //x1
                    ligne[3] = y1b;    //y1
                    
                    line( image2, Point(x0a, y0a), Point(x1a, y1a), Scalar(127,255,100), 3, 8 );
                    line( image2, Point(x0b, y0b), Point(x1b, y1b), Scalar(127,255,100), 3, 8 );
                    
                    if(i > j) {
                        lines.erase(lines.begin() + i);
                        lines.erase(lines.begin() + j);
                    } else {
                        lines.erase(lines.begin() + j);
                        lines.erase(lines.begin() + i);
                    }
                    
                    i = 0;
                    j = 0;
                    
                    lines.push_back(ligne);
                    break;
                }
            }
        }
    }
    return lines;
}

void findWalls(Mat src) {
    
    Mat gray;
    int inter = 40;
    int precision = 10;
    
    imshow("test", src);
    
    image_walls = src.clone();
    
    cvtColor(image_walls, image_walls, CV_GRAY2BGR);
    cvtColor(image_walls, gray, CV_BGR2GRAY);

    medianBlur(gray, gray, 3);
    Canny(gray, gray, 50, 150, 3);
    
    // dilate to ensure there is no cut off lines
    //dilate(gray, gray, Mat(), Point(-1,-1));
    
    vector<Vec4i> lines;
    vector<Vec4i> l;
    
    HoughLinesP( gray, lines, 1, CV_PI/180, 120, image.rows/20, image.rows/15);
    //cout << lines[0][0] << endl;
    
    // On garde les lignes verticales et horizontales
    for( size_t i = 0; i < lines.size(); i++ )
    {
        // x1, y1, x2, y2
        double angle = atan2(lines[i][3] - lines[i][1], lines[i][2] - lines[i][0]) * 180.0 / CV_PI;
        
        if((angle <= 0 + precision && angle >= 0 - precision) || angle <= -90 + precision || angle >= 90 -precision) {
            l.push_back(lines[i]);
        }
    }
    
    l = normalizedLines(l);
    
    image2 = image_walls.clone();
    
    l = mixLines(l);
    
    l = deleteLineMin(l);
    
    l = mixAlignedLines(l);
    
    l = mixLines(l);
    
    l = mixAlignedLines(l);
    
    for( size_t i = 0; i < l.size(); i++ ) {
        line( image_walls, Point(l[i][0], l[i][1]),
             Point(l[i][2], l[i][3]), Scalar(255,0,127), 3, 8 );
        line( image_walls, Point(l[i][0], l[i][1]),
             Point(l[i][0], l[i][1]), Scalar(0,0,255), 10, 8 );
    }

    /*
    //Dessin des lignes
    for( size_t i = 0; i < l.size(); i++ ) {
        line( image_walls, Point(finale[i][0], finale[i][1]),
             Point(finale[i][2], finale[i][3]), Scalar(255,255,0), 3, 8 );
        line( image_walls, Point(finale[i][0], finale[i][1]),
             Point(finale[i][0], finale[i][1]), Scalar(0,0,200), 10, 8 );
    }
    */
    /*
    //Dessin des lignes supprime
    for( size_t i = 0; i < vec.size(); i++ ) {
        line( image2, Point(l[i][0], l[i][1]),
             Point(l[i][2], l[i][3]), Scalar(127,255,127), 3, 8 );
    }
    */
    namedWindow("Walls2", 0);
    imshow( "Walls2", image2 );
    resizeWindow("Walls2", 1300, 900);
    moveWindow("Walls2", 30, 30);
    cout << l.size() << endl;
}


int main( int argc, char **argv ) {
    
    if ( argc < 2 ) {
        printf( "Usage: analyseur ImageToLoadAndDisplay\n" );
        return -1;
    }
    
    image           = imread( argv[1]);
    image_rect      = Mat(image.rows, image.cols, CV_8UC3, cv::Scalar(255,0,0));
    image_circle    = Mat(image.rows, image.cols, CV_8UC3, cv::Scalar(255,0,0));
    image_walls     = Mat(image.rows, image.cols, CV_8UC3, cv::Scalar(255,0,0));
    
    if ( !image.data ) { /* Check for invalid input */
        printf("Could not open or find the image\n") ;
        return -1;
    }
    
    // Gray
    Mat gray;
    if (image.channels() == 3) {
        cvtColor(image, gray, CV_BGR2GRAY);
    } else {
        gray = image;
    }
    Mat dst;
    morphologyEx( gray, dst, 2, getStructuringElement(MORPH_RECT, Size(7,7)) );
    
    findWalls(dst);
    
    
    // Trouver les contours externes du labyrinthe et les affichers
    
    // utilisation : Decommenter et :
    //  ./analyseur lab3.jpg
    //  ./analyseur ...
    
    //findPoly(image);
    //findCircle(image);
    
    treshold_value = 40;
    
    //Contour( 0,0);
    /*
    namedWindow("Image", 0);
    imshow( "Image", image );
    resizeWindow("Image", 900, 600);
    
    namedWindow("Rectangle", 0);
    imshow( "Rectangle", image_rect );
    resizeWindow("Rectangle", 900, 600);
    
    namedWindow("Circle", 0);
    imshow( "Circle", image_circle );
    resizeWindow("Circle", 900, 600);
    
    namedWindow("Mask", 0);
    imshow( "Mask", mask );
    resizeWindow("Mask", 900, 600);
    */
    namedWindow("Walls", 0);
    imshow( "Walls", image_walls );
    resizeWindow("Walls", 1300, 900);
    moveWindow("Walls", 30, 30);
    
    setMouseCallback("Walls",onMouse);
    
    //createTrackbar( "Seuil : ", "Rectangle", &treshold_value, 255, Contour );
    
    cv::waitKey(0);  // Wait for a keystroke in the window
    return 0;
}
