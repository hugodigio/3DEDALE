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


using namespace cv;
using namespace std;

//OBJECTIF : TROUVER L'AXE Z D'UN PLAN INCLINÉ (objet) PAR RAPPORT AU MODÈLE (scene) QUI LE CONTIENT
//ENTRÉES : - 4 points représentants les extrémités du modèle
//          - 4 points représentants les extrémités de l'objet (plan incliné sur lequel s'affiche le labyrinthe)
//          - 1 point représentant le centre du modèle
//          - 2 données représentant les dimensions du modèle (focal_x et focal_y)
//SORTIE : - 2 vecteurs (translation et rotation) relatifs à la transformation géométrique
//           correspondant à l'inclinaison de la plan dans le modèle.
//À partir de ces informations (notamment la rotation), on peut tracer l'axe Z image par image
//(c.a.d. à chaque fois que les coordonnées du plan incliné changent)

//compilation: make
//test: ./inclinaison

//VARIABLES GLOBALES
cv::Mat image ;                                   //L'image du modèle
Mat rot_vec(3,1,cv::DataType<double>::type);      //Rotation estimée de la feuille
Mat trans_vec(3,1,cv::DataType<double>::type);    //Translation estimée de la feuille (optionnel)
//Les entrées du programme
Point2f point_modele_1(95,95) ; //On leur donne des valeurs uniquement pour les tests
Point2f point_modele_2(920,115) ; 
Point2f point_modele_3(80,705) ; 
Point2f point_modele_4(965,690) ;

Point2f point_objet_1(185+11,115+80) ;
Point2f point_objet_2(795+11,150+80) ;
Point2f point_objet_3(60+11,520+80) ;
Point2f point_objet_4(975+11,500+80) ;

Point2f centre(514, 401) ;

double focal_x = 885 ;
double focal_y = 610 ;

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


int main( int argc, char **argv ) {


/*Lecture et Affichage d'une image entrée dans le terminal--------------------------------------------------*/

    image = cv::imread( "feuille.jpg" ); /* Read the file */

    if ( !image.data ) { /* Check for invalid input */
        printf("Could not open or find the image\n") ;
        return -1;
    }

    //Changement de la taille de l'image (afin qu'elle affiche ce que l'on veut voir)
    resize(image, image, Size(), 0.25, 0.25);

    cv::namedWindow( "Display window" , WINDOW_AUTOSIZE);
    cv::imshow( "Display window", image );

    //Ajout du callback onMouse à la fenêtre Display window
    setMouseCallback("Display window", onMouse, &image); //Nous allons rentrer manuellement les coordonnées des coins de la feuille et du modèle

    cv::waitKey(0); // Attente de la tappe d'une touche dans la fenêtre

/*Calcul du vecteur de rotation représentant l'inclinaison de la feuille par rapport au modèle---------------------*/

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


/*Visualisation du résultat------------------------------------------------------------------------------------------*/

    Mat image_objet = cv::imread( "feuille_inclinee.jpg" ); /* Read the file */

    //Création des matrices de rotation/translation que l'on va appliquer au point du modèle
    Mat rot_mat = Mat::zeros(3,3,CV_64F);
    Mat trans_mat = Mat::zeros(3,3,CV_64F);

    //Conversion des vecteurs en matrices
    Rodrigues(rot_vec, rot_mat) ;
    Rodrigues(trans_vec, trans_mat) ;

    //Application des matrices aux points du modèle
    std::vector<Point2f> scene2; //Contient les points du modèle modifiés par les matrices
    cv::Mat scene2_mat(3,1,cv::DataType<double>::type); //On ne se sert de cette matrice que pour l'homogénéité des calculs

    scene2 = scene ;
    scene2_mat = rot_mat * (Mat)scene2 ;
    scene2_mat = trans_mat * (Mat)scene2 ;

    scene2 = (vector<Point2f>)scene2_mat ;

    //Dessin du résultat
    line(image_objet, scene[0], scene2[0], CV_RGB(0, 0, 1), 1) ;
    line(image_objet, scene[1], scene2[1], CV_RGB(0, 0, 1), 1) ;
    line(image_objet, scene[2], scene2[2], CV_RGB(0, 0, 1), 1) ;
    line(image_objet, scene[3], scene2[3], CV_RGB(0, 0, 1), 1) ;

    //Affichage
    cv::namedWindow( "Affine Transformation" , WINDOW_AUTOSIZE);
    cv::imshow( "Affine Transformation", image_objet );

    cv::waitKey(0); // Attente de la tappe d'une touche dans la fenêtre

    //TODO : Traiter ce problème de multiplication de matrices

}

