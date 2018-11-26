#ifndef _WALLS_DETECTOR_
#define _WALLS_DETECTOR_

double isVectorsCollinear(cv::Vec4i segment_1, cv::Vec4i segment_2);

bool estCompris(double a, double b, double inter);

void normalizedLines ( std::vector<cv::Vec4i> &lines);

bool mixLines ( std::vector<cv::Vec4i> &lines);

bool deleteLineMin ( std::vector<cv::Vec4i> &lines);

bool mixAlignedLines ( std::vector<cv::Vec4i> &lines);

std::vector<cv::Vec4i> findWalls(cv::Mat src);
                          
#endif
