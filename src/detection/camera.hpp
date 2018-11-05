#ifndef _CAMERA_
#define _CAMERA_

void initCamera();

void closeCamera();

cv::Mat getImage ();

cv::Mat image_contours(cv::Mat frame);

bool findCircle(cv::Mat image);

void findStartFinish(cv::Mat image);

void detection();

void tracking();
                          
#endif
