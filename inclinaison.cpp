#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include <GL/gl.h>
#include <GL/glut.h>

#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <bits/stdc++.h>
#include <math.h>

#include "inclinaison.hpp"

using namespace cv;
using namespace std;

//OBJECTIF : TROUVER L'AXE Z D'UN PLAN INCLINÉ (objet) PAR RAPPORT AU MODÈLE (scene) QUI LE CONTIENT
//ENTRÉES : - 4 points représentants les extrémités du modèle.
//          - 4 points représentants les extrémités de l'objet (plan incliné sur lequel s'affiche le labyrinthe).
//          NB : Le calcul principal se fait dans la fonction inclinaison() .
//               Pour l'utiliser il faut déclarer préalablement les sorties,
//               puis les passer par adresse en paramètres de la fonction.
//               Les sorties préalablement déclarées s'en retrouveront donc modifiées.
//SORTIE : - 2 matrices (translation et rotation) relatives à la transformation géométrique
//           correspondante à l'inclinaison de la plan dans le modèle.
//         - 3 valeurs (stockés dans un Vec3b euler_angles) correspondant aux angles de rotations sur les 3 axes.
//           Ces valeurs sont calculés à partir du vecteur de rotation.
//À partir de ces informations (notamment la rotation), on peut tracer l'axe Z image par image
//(c.a.d. à chaque fois que les coordonnées du plan incliné changent).

//compilation: make
//test: ./inclinaison

// Checks if a matrix is a valid rotation matrix.
bool isRotationMatrix(Mat &R){
    Mat Rt;
    transpose(R, Rt);
    Mat shouldBeIdentity = Rt * R;
    Mat I = Mat::eye(3,3, shouldBeIdentity.type());
     
    return  norm(I, shouldBeIdentity) < 1e-6;
     
}

// Calculates rotation matrix to euler angles
// The result is the same as MATLAB except the order
// of the euler angles ( x and z are swapped ).
Vec3f rotationMatrixToEulerAngles(Mat &R){
 
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

void inclinaison(vector<Point2f> modele, vector<Point2f> objet, Mat* rot_mat, Mat* trans_mat, Vec3f* euler_angles){

    assert((modele.size() == 4) && (objet.size() == 4) ) ;

    //Calcul du centre du modèle (on en a besoin pour le changement de repère)
    Point2f centre((modele[0].x+modele[1].x+modele[2].x+modele[3].x/4),
                (modele[0].y+modele[1].y+modele[2].y+modele[3].y/4)) ;

    //Calcul des dimensions du modèle (on en a besoin pour cameraMatrix)
    double focal_x = modele[1].x - modele[0].x ;
    double focal_y = modele[1].y - modele[0].y ;

    //Changement de repère (Déplacement de l'origine au centre du modèle)
    modele[0] -= centre ;
    modele[1] -= centre ;
    modele[2] -= centre ;
    modele[3] -= centre ;

    objet[0] -= centre ;
    objet[1] -= centre ;
    objet[2] -= centre ;
    objet[3] -= centre ;

    centre -= centre ;

    //Création de vecteurs de points
    Point3f point_objet_1_3f(objet[0].x,objet[0].y,0) ;
    Point3f point_objet_2_3f(objet[1].x,objet[1].y,0) ;
    Point3f point_objet_3_3f(objet[2].x,objet[2].y,0) ;
    Point3f point_objet_4_3f(objet[3].x,objet[3].y,0) ;

    std::vector<Point3f> obj; //Contient les points de l'objet (Point3f car c'est ce qu'il faut fournir à solvePnP() )
    obj.push_back(point_objet_1_3f) ;
    obj.push_back(point_objet_2_3f) ;
    obj.push_back(point_objet_3_3f) ;
    obj.push_back(point_objet_4_3f) ; 

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

    Mat rot_vec(3,1,cv::DataType<double>::type);      //Stockage de la rotation estimée de la feuille
    Mat trans_vec(3,1,cv::DataType<double>::type);    //Stockage de la translation estimée de la feuille

    cv::solvePnP(obj, modele, cameraMatrix, distCoeffs, rot_vec, trans_vec);

    //Conversion des vecteurs en matrices
    Rodrigues(rot_vec, *rot_mat) ;
    Rodrigues(trans_vec, *trans_mat) ;

    //Calcul des angles d'Euler de la matrice de rotation
    Vec3f euler_angles_val ;
    euler_angles_val = rotationMatrixToEulerAngles(*rot_mat) ;
    //Conversion radians ---> degrés
    euler_angles_val[0] = euler_angles_val[0] * (180.0/3.141592653589793238463) ;
    euler_angles_val[1] = euler_angles_val[1] * (180.0/3.141592653589793238463) ;
    euler_angles_val[2] = euler_angles_val[2] * (180.0/3.141592653589793238463) ;
    //Echange de la rotation sur x et z (à cause de la fonction rotationMatrixToEulerAngles...)
    float swap ;
    swap = euler_angles_val[0] ;
    euler_angles_val[0] = euler_angles_val[1] ;
    euler_angles_val[1] = swap ;

    *euler_angles = euler_angles_val ;

}

int main( int argc, char **argv ) {

    //VARIABLES DE TEST
    Mat rot_vec(3,1,cv::DataType<double>::type);      //Rotation estimée de la feuille
    Mat trans_vec(3,1,cv::DataType<double>::type);    //Translation estimée de la feuille (optionnel)

    //Les entrées du programme
    Point2f point_modele_1(95,95) ;     // !! Point en haut à gauche !!
    Point2f point_modele_2(920,115) ;   // !! Point en haut à droite !!
    Point2f point_modele_3(80,705) ;    // !! Point en bas à gauche !!
    Point2f point_modele_4(965,690) ;   // !! Point en bas à droite !!

    vector<Point2f> modele ;
    modele.push_back(point_modele_4) ;
    modele.push_back(point_modele_3) ;
    modele.push_back(point_modele_2) ;
    modele.push_back(point_modele_1) ;

    Point2f point_objet_1(185+11,115+80) ;
    Point2f point_objet_2(795+11,150+80) ;
    Point2f point_objet_3(60+11,520+80) ;
    Point2f point_objet_4(975+11,500+80) ;

    vector<Point2f> objet ;
    objet.push_back(point_objet_4) ;
    objet.push_back(point_objet_3) ;
    objet.push_back(point_objet_2) ;
    objet.push_back(point_objet_1) ;

    //Les sorties du programmes
    Mat rot_mat ;
    Mat trans_mat ;
    Vec3f euler_angles ;

    //Le calcul
    inclinaison(modele, objet, &rot_mat, &trans_mat, &euler_angles) ;

    //Les résultats
    cout << rot_mat << "\n" ;
    cout << trans_mat << "\n" ;
    cout << euler_angles << "\n" ;

    return 0 ;

}
