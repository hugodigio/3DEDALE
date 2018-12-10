#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <bits/stdc++.h>
#include <math.h>


using namespace cv;
using namespace std;

//OBJECTIF : TROUVER L'AXE Z D'UN PLAN INCLINÉ (objet) PAR RAPPORT AU MODÈLE (scene) QUI LE CONTIENT
//ENTRÉES : - 4 points représentants les extrémités du modèle
//          - 4 points représentants les extrémités de l'objet (plan incliné sur lequel s'affiche le labyrinthe)
//SORTIE : - 2 vecteurs (translation et rotation) relatifs à la transformation géométrique
//           correspondant à l'inclinaison de la plan dans le modèle.
//À partir de ces informations (notamment la rotation), on peut tracer l'axe Z image par image
//(c.a.d. à chaque fois que les coordonnées du plan incliné changent)

//compilation: make
//test: ./inclinaison

//VARIABLES GLOBALES
Mat rot_vec(3,1,cv::DataType<double>::type);      //Rotation estimée de la feuille
Mat trans_vec(3,1,cv::DataType<double>::type);    //Translation estimée de la feuille (optionnel)
//Les entrées du programme
//On leur donne des valeurs uniquement pour les tests
Point2f point_modele_1(95,95) ;     // !! Point en haut à gauche !!
Point2f point_modele_2(920,115) ;   // !! Point en haut à droite !!
Point2f point_modele_3(80,705) ;    // !! Point en bas à gauche !!
Point2f point_modele_4(965,690) ;   // !! Point en bas à droite !!

Point2f point_objet_1(185+11,115+80) ;
Point2f point_objet_2(795+11,150+80) ;
Point2f point_objet_3(60+11,520+80) ;
Point2f point_objet_4(975+11,500+80) ;

//Calcul du centre du modèle (on en a besoin pour le changement de repère)
Point2f centre((point_modele_1.x+point_modele_2.x+point_modele_3.x+point_modele_4.x/4),
               (point_modele_1.y+point_modele_2.y+point_modele_3.y+point_modele_4.y/4)) ;

//Calcul des dimensions du modèle (on en a besoin pour cameraMatrix)
double focal_x = point_modele_2.x - point_modele_1.x ;
double focal_y = point_modele_2.y - point_modele_1.y ;

//Callback souris récupérant les coordonnées (x,y) d'un clic
static void onMouse(int event, int x, int y, int flags, void* param)
{
    Mat &image = *((Mat*)param); //cast and deref the param

    if (event == EVENT_LBUTTONDOWN)
    {
        short val = image.at< short >(y,x); // opencv is row-major ! 
        cout << "x= " << x << " y= " << y << "val= "<<val<< endl;
    }
}

// Checks if a matrix is a valid rotation matrix.
bool isRotationMatrix(Mat &R)
{
    Mat Rt;
    transpose(R, Rt);
    Mat shouldBeIdentity = Rt * R;
    Mat I = Mat::eye(3,3, shouldBeIdentity.type());
     
    return  norm(I, shouldBeIdentity) < 1e-6;
     
}

// Calculates rotation matrix to euler angles
// The result is the same as MATLAB except the order
// of the euler angles ( x and z are swapped ).
Vec3f rotationMatrixToEulerAngles(Mat &R)
{
 
    assert(isRotationMatrix(R));
     
    float sy = sqrt(R.at<double>(0,0) * R.at<double>(0,0) +  R.at<double>(1,0) * R.at<double>(1,0) );
 
    bool singular = sy < 1e-6; // If
 
    float x, y, z;
    if (!singular)
    {
        x = atan2(R.at<double>(2,1) , R.at<double>(2,2));
        y = atan2(-R.at<double>(2,0), sy);
        z = atan2(R.at<double>(1,0), R.at<double>(0,0));
    }
    else
    {
        x = atan2(-R.at<double>(1,2), R.at<double>(1,1));
        y = atan2(-R.at<double>(2,0), sy);
        z = 0;
    }
    return Vec3f(x, y, z);

}

/*
int main( int argc, char **argv ) {


    //Changement de repère (Déplacement de l'origine au centre du modèle)
    point_modele_1 -= centre ;
    point_modele_2 -= centre ;
    point_modele_3 -= centre ;
    point_modele_4 -= centre ;

    point_objet_1 -= centre ;
    point_objet_2 -= centre ;
    point_objet_3 -= centre ;
    point_objet_4 -= centre ;

    centre -= centre ;

    //Création de vecteurs de points
    Point3f point_objet_1_3f(point_objet_1.x,point_objet_1.y,0) ;
    Point3f point_objet_2_3f(point_objet_1.x,point_objet_1.y,0) ;
    Point3f point_objet_3_3f(point_objet_1.x,point_objet_1.y,0) ;
    Point3f point_objet_4_3f(point_objet_1.x,point_objet_1.y,0) ;

    std::vector<Point3f> obj; //Contient les points de l'objet (Point3f car c'est ce qu'il faut fournir à solvePnP() )
    obj.push_back(point_objet_1_3f) ;
    obj.push_back(point_objet_2_3f) ;
    obj.push_back(point_objet_3_3f) ;
    obj.push_back(point_objet_4_3f) ;

    std::vector<Point2f> scene; //Contient les points du modèle
    scene.push_back(point_modele_1) ;
    scene.push_back(point_modele_2) ;
    scene.push_back(point_modele_3) ;
    scene.push_back(point_modele_4) ; 

    //Calcul de l'homographie (=> Inclinaison)
    cv::Mat cameraMatrix(3,3,cv::DataType<double>::type); //Conventional Camera Matrix
    cameraMatrix.at<double>(0,0) = focal_x ;
    cameraMatrix.at<double>(0,1) = 0 ; 
    cameraMatrix.at<double>(0,2) = 0 ; //center_x (dans le nouveau repère)
    cameraMatrix.at<double>(1,0) = 0 ; 
    cameraMatrix.at<double>(1,1) = focal_y ;
    cameraMatrix.at<double>(1,2) = 0 ; //center_y (dans le nouveau repère)
    cameraMatrix.at<double>(2,0) = 0 ; 
    cameraMatrix.at<double>(2,1) = 0 ; 
    cameraMatrix.at<double>(2,2) = 1 ; 

    cv::Mat distCoeffs(4,1,cv::DataType<double>::type);
    distCoeffs.at<double>(0) = 0;
    distCoeffs.at<double>(1) = 0;
    distCoeffs.at<double>(2) = 0;
    distCoeffs.at<double>(3) = 0; //Valeur par défaut

    cv::solvePnP(obj, scene, cameraMatrix, distCoeffs, rot_vec, trans_vec);

    cout << rot_vec << "\n" ;
    cout << trans_vec << "\n" ;

    //NB: rot_vec et trans_vec sont les résultats attendus par le programmme
    //Grâce à ces données relatives à la transformation géometrique actuelle,
    //on est capable de trouver l'axe Z de l'objet (=> plan incliné sur lequel figure le labyrinthe).

    //Conversion des vecteurs en matrices
    Mat rot_mat , trans_mat ;
    Rodrigues(rot_vec, rot_mat) ;
    Rodrigues(trans_vec, trans_mat) ;

    //Calcul des angles d'Euler de la matrice de rotation
    Vec3f euler_angles = rotationMatrixToEulerAngles(rot_mat) ;
    //Conversion radians ---> degrés
    euler_angles[0] = euler_angles[0] * (180.0/3.141592653589793238463) ;
    euler_angles[1] = euler_angles[1] * (180.0/3.141592653589793238463) ;
    euler_angles[2] = euler_angles[2] * (180.0/3.141592653589793238463) ;

    cout << euler_angles << "\n" ;

}*/

