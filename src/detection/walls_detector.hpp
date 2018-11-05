#ifndef _WALLS_DETECTOR_
#define _WALLS_DETECTOR_

bool compris(int a, int b, int inter);

double isVectorsCollinear(cv::Vec4i segment_1, cv::Vec4i segment_2);

bool estCompris(double a, double b, double inter);

bool equalLines (cv::Vec4i vec_a, cv::Vec4i vec_b);

void normalizedLines ( std::vector<cv::Vec4i> lines);

bool mixLines ( std::vector<cv::Vec4i> lines);

bool deleteLineMin ( std::vector<cv::Vec4i> lines);

bool mixAlignedLines ( std::vector<cv::Vec4i> lines);

void findWalls(cv::Mat src);
                          
#endif
