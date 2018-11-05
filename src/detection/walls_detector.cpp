
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

#include "walls_detector.hpp"

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


vector<Vec4i> lines;

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
void normalizedLines () {
    
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
}

bool mixLines () {
    
    int fontFace = FONT_HERSHEY_COMPLEX_SMALL;
    double fontScale=3.5;
    int cpt = 0;
    int largeur = 50;
    int x0a, y0a, x1a, y1a, x0b, y0b, x1b, y1b;
    bool changement = false;
    
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
                    changement = true;
                    lines.push_back(ligne);
                    break;
                }
                
                //Verticale
                if( (y0b >= y0a && y0b <= y1a && y1b >= y1a) && estCompris(x0a, x0b, largeur) && estCompris(x1a, x1b, largeur)) {
                    
                    cpt ++;
                    
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
                    changement = true;
                    lines.push_back(ligne);
                    break;
                }
            }
        }
    }
    return changement;
}


bool deleteLineMin () {
    int fontFace = FONT_HERSHEY_COMPLEX_SMALL;
    double fontScale=3.5;
    int cpt = 0;
    int largeur = 50;
    int x0a, y0a, x1a, y1a, x0b, y0b, x1b, y1b;
    
    bool changement = false;
    
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
                    
                    line( image2, Point(x0b, y0b), Point(x1b, y1b), Scalar(127,255,200), 3, 8 );
                    
                    lines.erase(lines.begin() + j);

                    j--;
                    if(j<=i) {
                        i--;
                    }
                    changement = true;
                    continue;
                }
            }
        }
    }
    return changement;
}

bool mixAlignedLines () {
    
    int fontFace = FONT_HERSHEY_COMPLEX_SMALL;
    double fontScale=3.5;
    int cpt = 0;
    int largeur = 50; //espacement entre 2 lignes
    int espace = 100;
    int x0a, y0a, x1a, y1a, x0b, y0b, x1b, y1b;
    bool changement = false;
    
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
                    
                    changement = true;
                    lines.push_back(ligne);
                    break;
                }
                
                //Verticale
                if( (y0b >= y1a && y0b <= y1a + espace && y1b >= y1a) && estCompris(x0a, x1b, largeur) && estCompris(x1a, x0b, largeur)) {
                    
                    cpt ++;
                   
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
                    
                    changement = true;
                    lines.push_back(ligne);
                    break;
                }
            }
        }
    }
    return changement;
}

void findWalls(Mat src) {
    //src est en niveaux de gris
    Mat gray;
    int precision = 20;
    
    morphologyEx( src, src, 2, getStructuringElement(MORPH_RECT, Size(7,7)) );
    
    image_walls = src.clone();
    
    cvtColor(image_walls, image_walls, CV_GRAY2BGR);
    cvtColor(image_walls, gray, CV_BGR2GRAY);

    medianBlur(gray, gray, 3);
    Canny(gray, gray, 50, 150, 3);

    vector<Vec4i> l;
    
    HoughLinesP( gray, lines, 1, CV_PI/180, 80, src.rows/20, src.rows/15);
    
    // On garde les lignes verticales et horizontales
    for( size_t i = 0; i < lines.size(); i++ )
    {
        // x1, y1, x2, y2
        double angle = atan2(lines[i][3] - lines[i][1], lines[i][2] - lines[i][0]) * 180.0 / CV_PI;
        
        if((angle <= 0 + precision && angle >= 0 - precision) || angle <= -90 + precision || angle >= 90 -precision) {
            l.push_back(lines[i]);
        }
    }
    
    normalizedLines();
    
    image2 = image_walls.clone();
    
    bool continuer = mixLines();
    
    while(continuer) {
		cout << "a" << endl;
		continuer = mixLines();
	}
    
    continuer = deleteLineMin();
    
    while(continuer) {
		cout << "b" << endl;
		continuer = deleteLineMin();
	}
	
	
	continuer = mixAlignedLines();
    
    while(continuer) {
		cout << "c" << endl;
		continuer = mixAlignedLines();
	}
	
	mixLines();
	mixAlignedLines();
	mixLines();
	mixAlignedLines();
	mixLines();
	mixAlignedLines();
	mixLines();
	mixAlignedLines();

   
    for( size_t i = 0; i < lines.size(); i++ ) {
        line( image_walls, Point(lines[i][0], lines[i][1]),
             Point(lines[i][2], lines[i][3]), Scalar(255,0,127), 3, 8 );
        line( image_walls, Point(lines[i][0], lines[i][1]),
             Point(lines[i][0], lines[i][1]), Scalar(0,0,255), 10, 8 );
    }

    namedWindow("Walls", 0);
    imshow( "Walls", image_walls );
    imshow( "Walls2", image2 );
    resizeWindow("Walls", 1300, 900);
    cout << l.size() << endl;
}

/*
int main( int argc, char **argv ) {
    Mat dst = imread ("image.jpg");
    cvtColor(dst, dst, CV_BGR2GRAY);
    findWalls(dst);
    
    namedWindow("Walls", 0);
    imshow( "Walls", image_walls );
    
    cv::waitKey(0);  // Wait for a keystroke in the window
    return 0;
}*/
