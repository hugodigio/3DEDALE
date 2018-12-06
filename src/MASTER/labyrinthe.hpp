#ifndef LABYRINTHE
#define LABYRINTHE

class labyrinthe{
    public:
    cv::Point2f PointDepart;
    cv::Point2f PointArrivee;
    std::vector<cv::Vec4f> lignes;
};

#endif