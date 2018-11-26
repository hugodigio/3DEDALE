
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


//Donner le meme sens a toutes les lignes ( v ^ )
void normalizedLines (vector<Vec4i> &lines) {
    
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

/**   -------
 * 		--------
 * 
 * */
bool mixLines (vector<Vec4i> &lines) {
    
    int fontFace = FONT_HERSHEY_COMPLEX_SMALL;
    double fontScale=3.5;
    int cpt = 0;
    int espacement = 15;
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
                if( (x0b >= x0a && x0b <= x1a && x1b >= x1a) && estCompris(y0a, y0b, espacement) && estCompris(y1a, y1b, espacement)) {
                    
                    cpt ++;
                    
                    Vec4i ligne;
                    ligne[0] = x0a;    			//x0
                    ligne[1] = (y0a+y0b)/2;    	//y0
                    ligne[2] = x1b;    			//x1
                    ligne[3] = (y1a+y1b)/2;    	//y1
                    
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
                if( (y0b >= y0a && y0b <= y1a && y1b >= y1a) && estCompris(x0a, x0b, espacement) && estCompris(x1a, x1b, espacement)) {
                    
                    cpt ++;
                    
                    Vec4i ligne;
                    ligne[0] = (x0a+x0b)/2;    	//x0
                    ligne[1] = y0a;    			//y0
                    ligne[2] = (x1a+x1b)/2;    	//x1
                    ligne[3] = y1b;    			//y1
                    
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




/** 
 *  ----------
 * 		----
 * 
 * */
bool deleteLineMin (vector<Vec4i> &lines) {
    int fontFace = FONT_HERSHEY_COMPLEX_SMALL;
    double fontScale=3.5;
    int cpt = 0;
    int espacement = 30;
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
                if( (x0b >= x0a && x1b <= x1a) && estCompris(y0a, y0b, espacement) && estCompris(y1a, y1b, espacement)) {
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
                
                // Verticale
                if( (y0b >= y0a && y1b <= y1a) && estCompris(x0a, x0b, espacement) && estCompris(x1a, x1b, espacement)) {
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



/**
 * -----  --- = --------
 * 
 * */
bool mixAlignedLines (vector<Vec4i> &lines) {
    
    int fontFace = FONT_HERSHEY_COMPLEX_SMALL;
    double fontScale=3.5;
    int cpt = 0;
    int espacement = 15; //espacement entre 2 lignes
    int espace = 50; //espace entre fin d'une ligne et debut d'une autre
    int x0a, y0a, x1a, y1a, x0b, y0b, x1b, y1b;
    bool changement = false;
    
    for( size_t i = 0; i < lines.size(); i++ ) {
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
                if( (x0b >= x1a && x0b <= x1a + espace && x1b >= x1a) && estCompris(y0a, y0b, espacement) && estCompris(y1a, y1b, espacement)) {
                    
                    cpt ++;
                   
                    Vec4i ligne;
                    ligne[0] = x0a;    			//x0
                    ligne[1] = y0a;    	//y0
                    ligne[2] = x1b;    			//x1
                    ligne[3] = y1b;    	//y1
                   
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
                
                //BUG ICI LIGNE DIAGONALE
                //Verticale
                if( (y0b >= y1a && y0b <= y1a + espace && y1b >= y1a) && estCompris(x0a, x0b, espacement) && estCompris(x1a, x1b, espacement)) {

                    cpt ++;
                   
                    Vec4i ligne;
                    ligne[0] = x0a;    	//x0
                    ligne[1] = y0a;    			//y0
                    ligne[2] = x1b;    	//x1
                    ligne[3] = y1b;    			//y1
                    
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

bool orthoLines(vector<Vec4i> &lines) {
	
	int precision = 5; // precision de l'angle
	bool changement = false;
	vector<Vec4i> linesResult;
	
	// On garde les lignes verticales et horizontales
    for( size_t i = 0; i < lines.size(); i++ )
    {
        // x1, y1, x2, y2
        double angle = atan2(lines[i][3] - lines[i][1], lines[i][2] - lines[i][0]) * 180.0 / CV_PI;
        
        if((angle <= 0 + precision && angle >= 0 - precision) || angle <= -90 + precision || angle >= 90 -precision) {
            linesResult.push_back(lines[i]);
        }
    }
	
	lines = linesResult;
	
	return changement;
}

vector<Vec4i> findWalls(Mat src) {
	
	bool mix, min, align;
	
    //src est en niveaux de gris
    Mat copy;
    vector<Vec4i> lines;
    
    copy = src.clone();
    
    morphologyEx( copy, copy, 2, getStructuringElement(MORPH_RECT, Size(7,7)) );
    
    cvtColor(copy, copy, CV_GRAY2BGR);
    cvtColor(copy, copy, CV_BGR2GRAY);
    
    medianBlur(copy, copy, 3);
    Canny(copy, copy, 50, 150, 3);
    HoughLinesP( copy, lines, 1, CV_PI/180, 50, src.rows/20, src.rows/15);
    
    orthoLines(lines); //On ne garde que les lignes orthogonales
    
    normalizedLines(lines); //Toutes les lignes dans le meme sens
    
    int cpt = 0;
    
    while(cpt < 10) {
		mix = mixLines(lines);
		min = deleteLineMin(lines);
		align = mixAlignedLines(lines);
		cpt++ ;
		
	}

	return lines;
}
