#include <GLFW/glfw3.h>
#include "2Dto3D.hpp"

using namespace std;


void creerlabyrinthe(labyrinthe x){
    cout << "nombre de murs: " << x.lignes.size() << endl;
    for(int i=0; i<x.lignes.size();i++){
        cout << "construction du mur " << i+1 << endl;
        cv::Point2d A = cv::Point2d(x.lignes[i][0],x.lignes[i][1]);
        cv::Point2d B = cv::Point2d(x.lignes[i][2],x.lignes[i][3]);
        def_mur(A,B,EPAISSEUR_MUR,HAUTEUR_MUR);
    }
}

labyrinthe test(){
    labyrinthe lab;

    lab.PointDepart  = cv::Point2f(1,1);
    lab.PointArrivee = cv::Point2f(10,10);
    //1ere ligne
    lab.lignes.push_back(cv::Vec4f(2,3,2,6));
    //2e ligne
    lab.lignes.push_back(cv::Vec4f(3,4,5,4));
    //3e ligne
    lab.lignes.push_back(cv::Vec4f(5,7,8,7));

    //murs
    lab.lignes.push_back(cv::Vec4f(0,0,10,0));
    lab.lignes.push_back(cv::Vec4f(0,0,0,10));
    lab.lignes.push_back(cv::Vec4f(10,10,0,10));
    lab.lignes.push_back(cv::Vec4f(10,10,10,0));

    return lab;
}

void def_mur(cv::Point2f A, cv::Point2f B, float epaisseur, float hauteur){
    if(A.x == B.x){
        //               MUR VERTICAL
        //face gauche
        glBegin(GL_POLYGON);
            glNormal3f(-1.0,0.0,0.0);
            glVertex3f(A.x-(epaisseur/2), A.y, 0);
            glVertex3f(B.x-(epaisseur/2), B.y, 0);
            glVertex3f(B.x-(epaisseur/2), B.y, hauteur);
            glVertex3f(A.x-(epaisseur/2), A.y, hauteur);
        glEnd();
        //face droite
        glBegin(GL_POLYGON);
            glNormal3f(1.0,0.0,0.0);
            glVertex3f(A.x+(epaisseur/2), A.y, 0);
            glVertex3f(B.x+(epaisseur/2), B.y, 0);
            glVertex3f(B.x+(epaisseur/2), B.y, hauteur);
            glVertex3f(A.x+(epaisseur/2), A.y, hauteur);
        glEnd();
        //face de dessus
        glBegin(GL_POLYGON);
            glNormal3f(0,0,1.0);
            glVertex3f(A.x-(epaisseur/2), A.y, 0);
            glVertex3f(A.x+(epaisseur/2), A.y, 0);
            glVertex3f(B.x+(epaisseur/2), B.y, 0);
            glVertex3f(B.x-(epaisseur/2), B.y, 0);
        glEnd();
        //face arriere
        glBegin(GL_POLYGON);
            glNormal3f(0,1.0,0.0);
            glVertex3f(A.x-(epaisseur/2), A.y, 0);
            glVertex3f(A.x-(epaisseur/2), A.y, hauteur);
            glVertex3f(A.x+(epaisseur/2), A.y, hauteur);
            glVertex3f(A.x+(epaisseur/2), A.y, 0);
        glEnd();
        //face avant
        glBegin(GL_POLYGON);
            glNormal3f(0,-1.0,0.0);
            glVertex3f(B.x-(epaisseur/2), B.y,0);
            glVertex3f(B.x+(epaisseur/2), B.y,0);
            glVertex3f(B.x+(epaisseur/2), B.y,hauteur);
            glVertex3f(B.x-(epaisseur/2), B.y,hauteur);
        glEnd();
    }else if(A.y == B.y){
        // MUR HORIZONTAL
        // face avant
        glBegin(GL_POLYGON);
            glNormal3f(0,1.0,0.0);
            glVertex3f(A.x, A.y-(epaisseur/2), 0);
            glVertex3f(B.x, B.y-(epaisseur/2), 0);
            glVertex3f(B.x, B.y-(epaisseur/2), hauteur);
            glVertex3f(A.x, A.y-(epaisseur/2), hauteur);
        glEnd();
        // face arriere
        glBegin(GL_POLYGON);
            glNormal3f(0,-1.0,0.0);
            glVertex3f(A.x, A.y+(epaisseur/2), 0);
            glVertex3f(B.x, B.y+(epaisseur/2), 0);
            glVertex3f(B.x, B.y+(epaisseur/2), hauteur);
            glVertex3f(A.x, A.y+(epaisseur/2), hauteur);
        glEnd();
        // face de dessus
        glBegin(GL_POLYGON);
            glNormal3f(0,0.0,1.0);
            glVertex3f(A.x, A.y-(epaisseur/2),hauteur);
            glVertex3f(A.x, B.y-(epaisseur/2),hauteur);
            glVertex3f(A.x, B.y+(epaisseur/2),hauteur);
            glVertex3f(A.x, A.y+(epaisseur/2),hauteur);
        glEnd();
        //face gauche
        glBegin(GL_POLYGON);
            glNormal3f(-1.0,0.0,0.0);
            glVertex3f(A.x, A.y-(epaisseur/2),0);
            glVertex3f(A.x, A.y+(epaisseur/2),hauteur);
            glVertex3f(A.x, A.y+(epaisseur/2),hauteur);
            glVertex3f(A.x, A.y+(epaisseur/2),0);
        glEnd();
        //face droite
        glBegin(GL_POLYGON);
           glNormal3f(1.0,0.0,0.0);
           glVertex3f(B.x, B.y-(epaisseur/2),0);
           glVertex3f(B.x, B.y+(epaisseur/2),hauteur);
           glVertex3f(B.x, B.y+(epaisseur/2),hauteur);
           glVertex3f(B.x, B.y+(epaisseur/2),0);
        glEnd();
    } else {
        cout << "erreur: mur non vertical ou horizontal" << endl;
    }
    //code pour faire un mur
}

/* ----------------------------------------------------------
// PLUS TARD DANS LE MAIN  QUAND ON AURA REUSSI A LINK SA MER
------------------------------------------------------------*/

void def_carre(void){

    glBegin(GL_POLYGON);
        glNormal3f(0., 0., 1.) ;
        glVertex3f(-0.5,0.5,0.);
        glVertex3f(0.5,0.5,0.);
        glVertex3f(0.5,-0.5,0.);
        glVertex3f(-0.5 ,-0.5,0.);
    glEnd();

}

void def_axes(){

    glBegin(GL_LINES);
        //x
        glColor3f(1.0, 0.0, 0.0);
        glVertex3f(0.0,0.0,0.0);
        glVertex3f(1,0.0,0.0);
        //y
        glColor3f(0.0, 1.0, 0.0);
        glVertex3f(0.0,0.0,0.0);
        glVertex3f(0.0,1,0.0);
        //z
        glColor3f(0.0, 0.0, 1.0);
        glVertex3f(0.0,0.0,0.0);
        glVertex3f(0.0,0.0,1);
    glEnd();

}