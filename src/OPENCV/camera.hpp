#ifndef _CAMERA_
#define _CAMERA_

void initCamera();

void closeCamera();

cv::Mat getImage ();

cv::Mat image_contours(cv::Mat frame);

bool findCircle(cv::Mat image);

void findStartFinish(cv::Mat image);

std::vector<cv::Vec4f> detection();

void tracking();

void start_camera();
                          
#endif
