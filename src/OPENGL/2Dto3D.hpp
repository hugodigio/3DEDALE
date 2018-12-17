#ifndef DTO3D_H
#define DTO3D_H

#include <iostream>
#include <opencv/highgui.h>
#include <vector>
#include <GL/glut.h>
#include "../MASTER/labyrinthe.hpp"

#define HAUTEUR_MUR 1.0
#define EPAISSEUR_MUR 0.1

using namespace std;

/*s'occupe de la rotation du labyrinthe */
void rotationlabyrinthe(cv::Vec3f angles);
/*permet de normaliser les données du labyrinthe.
les coordonnées du labyrinthe sont modifiées de facon à ce que le centre du labyrinthe soit au centre du repère*/
labyrinthe normaliselabyrinthe(labyrinthe x);
/* initialise le modele 3D du labyrinthe en creant tout les murs
contenu dans la variable x ainsi que le point de depart et d'arrivee */
void creerlabyrinthe(labyrinthe x);
labyrinthe test();   //DEBUG
labyrinthe bf();     //DEBUG
labyrinthe bf_mir(); //DEBUG
/*cree un mur 3D a partir des coordonnees 2D donnees en parametre*/
void creermur(cv::Point2f x, cv::Point2f y);
void def_mur(cv::Point2f A, cv::Point2f B, float epaisseur, float hauteur);
void def_axes();

#endif

