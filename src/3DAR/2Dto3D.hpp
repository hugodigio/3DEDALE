#include <iostream>
#include <opencv/highgui.h>
#include <vector>
#include "../DataStruct/labyrinthe.hpp"

#define HAUTEUR_MUR 1.0
#define EPAISSEUR_MUR 0.1

using namespace std;

/* initialise le modele 3D du labyrinthe en creant tout les murs
contenu dans la variable x ainsi que le point de depart et d'arrivee */
void creerlabyrinthe(labyrinthe x);
void test();

/*cree un mur 3D a partir des coordonnees 2D donnees en parametre*/
void creermur(cv::Point2f x, cv::Point2f y);
void def_mur(cv::Point2f A, cv::Point2f B, float epaisseur, float hauteur);

