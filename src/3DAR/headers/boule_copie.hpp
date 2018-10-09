//  Created by HASSANEIN Alzahra

#ifndef BOULE_H
#define BOULE_H

#include <GLUT/glut.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <vector>
//#include <Box2D/Box2D.h>

#define LONGUEUR 5
#define SECTION 3

using namespace std;


class Boule{
    
    private :
    
    //ATTRIBUTS
    
    float M_plateau[16];
    float M_boule[16];
    
    ///
    float rotx ;
    float roty ;
    
    float rotationActuelleX;
    float rotationActuelleY ;
    
    float deplacementX ;
    float deplacementY ;
    int valeurChangement;
    
    ///
    int etatMouvement;
    int direction;
    
    float bouleX ;
    float bouleY ;
    
    vector<float> murCoordX;
    vector<float> murCoordY;
    
    double Xmax, Ymax;
    double Xmin, Ymin;
    
    
    //FONCTIONS
    
public:
    
    Boule();
    ~Boule();
    
    
    void remplissageCoordMur();
    //void def_axes(void);
    
    void def_carre(void);
    void def_boule(void);
    void def_plateau(void);
    //void def_boite(void);
    
    void RotationM(float M[16], int valaxe, float angle);
    void MouvementM(float M[16], int valaxe, float distance);
    
    void changerEtat();
    bool estChanger();
    int donnerCas(int cas);
    int checkSigne(int axis);
    
    bool collisionMurs(int cas);
    
    
    void affichageCoordBoule(int val);
    
    void Timer(int value);
    
    void clavier(unsigned char key, int x, int y);
    
    void dessinScene();
    void affichage(void);
    void redim(int width, int height);
    
    void init();
    void load_boule(int argc, char **argv);
    
};


/** Problèmes:
 
 FAIT Tourner l'affichage pour une initialisation a plat
 FAIT Faire une fonction init
 FAIT  Matrices pour la boule
 FAIT  Matrice pour le plateau
 RESOLU  Nouveau Problème : Disparition de l'axe Z (bleu) a l'affichage
 RESOLU Voir si on peut faire un axe qui fait moins "attacher" au plan
 Il sera enlever pour la fin
 
 RESOLU Nouveau probleme : Translation de la boule sur le plateau ne fonctionne pas
 On dirait que la translation sur Z pose un soucis, on a l'effet de zoom mais
 pas le rapprochement par rapport au plateau
 La ligne MouvementM(M_boule,0,(0.3-SECTION/4.)); diminue la taille de la boule,
 au lieu de la deplacer
 
 FAIT  Enlevez les a et remplacer par SECTION
 FAIT Creer des variables pour les angles de rotations du plateau,
 cf Fontion Rotation
 FAIT Creer une fonction mouvement pour la boule,
 
 RESOLU Definir a partir de quel angle la boule ne bougera plus (plus de 90 la boule
 devrait "tomber" du plateau,
 On ne bouge pas en Z pour le moment, la balle reste collé et se deplace en fonction meme
 a l'envers, prendre apres 180 degre, inverser les deplacements
 
 Etudier BOX2D pour la physique et le deplacement coulissant et la gestion des collisions
 (premiere collision mur du bord donc largeur (ou section) plus ou moins le rayon esst la
 distance maximale, suite collision au mur, il faut detecter si on est pas sur une coordoonées
 d'une ligne d'un mur, donc avec le tableau de ligne
 
 **/

#endif

