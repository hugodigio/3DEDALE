#ifndef LABYRINTHE
#define LABYRINTHE

class labyrinthe{
    public: 
    cv::Point2f PointDepart;
    cv::Point2f PointArrivee;
    std::vector<cv::Point2f> x;
    std::vector<cv::Point2f> y;
    //un mur est tracé de x[0] à y[0]
};

#endif