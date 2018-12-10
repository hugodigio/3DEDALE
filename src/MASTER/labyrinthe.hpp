#ifndef LABYRINTHE
#define LABYRINTHE

class labyrinthe{
    public:
    cv::Point2d PointDepart;
    cv::Point2d PointArrivee;
    std::vector<cv::Vec4f> lignes;
};

#endif