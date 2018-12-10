#ifndef _WALLS_DETECTOR_
#define _WALLS_DETECTOR_

double isVectorsCollinear(cv::Vec4f segment_1, cv::Vec4f segment_2);

bool estCompris(double a, double b, double inter);

void normalizedLines ( std::vector<cv::Vec4f> &lines);

bool mixLines ( std::vector<cv::Vec4f> &lines);

bool deleteLineMin ( std::vector<cv::Vec4f> &lines);

bool mixAlignedLines ( std::vector<cv::Vec4f> &lines);

std::vector<cv::Vec4f> findWalls(cv::Mat src);
                          
#endif
