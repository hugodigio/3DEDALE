#ifndef _INCLINAISON_
#define _INCLINAISON_

bool isRotationMatrix(cv::Mat &R) ;

cv::Vec3f rotationMatrixToEulerAngles(cv::Mat &R) ;

void inclinaison(std::vector<cv::Point2f> modele, std::vector<cv::Point2f> objet, cv::Mat rot_mat, cv::Mat trans_mat, cv::Vec3f euler_angles) ;
                          
#endif
