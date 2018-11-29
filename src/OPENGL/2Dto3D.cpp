#include "2Dto3D.hpp"
#include <limits>

using namespace std;

float anglex=0.0f, angley=0.0f, anglez=0.0f;

void rotationlabyrinthe(float x, float y, float z){
    anglex = x;
    angley = y;
    anglez = z;
}

labyrinthe normaliselabyrinthe(labyrinthe x){
    cout << "normalisation du labyrinthe" << endl << "---------------------------" << endl;
    labyrinthe normalized;
    float minX = std::numeric_limits<float>::max(), minY = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::min(),maxY = std::numeric_limits<float>::min();
    cv::Point2f transform = cv::Point2f(0,0);
    
    for (int i=0; i<x.lignes.size();i++){
        //trouver le min en x
        if(x.lignes[i][0] < minX) minX = x.lignes[i][0];
        if(x.lignes[i][2] < minX) minX = x.lignes[i][2];
        //trouver le min en y
        if(x.lignes[i][1] < minY) minY = x.lignes[i][1];
        if(x.lignes[i][3] < minY) minY = x.lignes[i][3];
        //trouver le max en x
        if(x.lignes[i][0] > maxX) maxX = x.lignes[i][0];
        if(x.lignes[i][2] > maxX) maxX = x.lignes[i][2];
        //trouver le max en y
        if(x.lignes[i][1] > maxY) maxY = x.lignes[i][1];
        if(x.lignes[i][3] > maxY) maxY = x.lignes[i][3];
    }
    cout << "min:(" << minX << "," << minY << ") max:(" << maxX << "," << maxY << ")" << endl;
    transform = cv::Point2f( (minX-maxX)/2 , (minY-maxY)/2 );
    cout << "transformation: " << transform << endl;

    //formation du labyrinthe normalise
    normalized.PointDepart  = x.PointDepart;
    normalized.PointArrivee = x.PointArrivee;
    for(int i=0; i < x.lignes.size(); i++){
        float x1 = x.lignes[i][0]+transform.x;
        float y1 = x.lignes[i][1]+transform.y;
        float x2 = x.lignes[i][2]+transform.x;
        float y2 = x.lignes[i][3]+transform.y;
        normalized.lignes.push_back(cv::Vec4f(x1,y1,x2,y2));
    }
    return normalized;
}
void creerlabyrinthe(labyrinthe x){
    glRotatef(0.0,1.0,0.0,0.0);
    glRotatef(0.0,0.0,1.0,0.0);
    glRotatef(0,0.0,0.0,1.0);
    labyrinthe lab = normaliselabyrinthe(x);

    GLfloat white[4] = {1.0f,1.0f,1.0f,1.0f};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
    cout << "nombre de murs: " << lab.lignes.size() << endl;
    for(int i=0; i<lab.lignes.size();i++){
        cout << "construction du mur " << i+1 << endl;
        cv::Point2d A = cv::Point2d(lab.lignes[i][0],lab.lignes[i][1]);
        cv::Point2d B = cv::Point2d(lab.lignes[i][2],lab.lignes[i][3]);
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
        glDisable(GL_CULL_FACE);
            glNormal3f(-1.0,0.0,0.0);
            glVertex3f(A.x-(epaisseur/2), A.y, 0);
            glVertex3f(B.x-(epaisseur/2), B.y, 0);
            glVertex3f(B.x-(epaisseur/2), B.y, hauteur);
            glVertex3f(A.x-(epaisseur/2), A.y, hauteur);
        glEnd();
        //face droite
        glBegin(GL_POLYGON);
        glDisable(GL_CULL_FACE);
            glNormal3f(1.0,0.0,0.0);
            glVertex3f(A.x+(epaisseur/2), A.y, 0);
            glVertex3f(B.x+(epaisseur/2), B.y, 0);
            glVertex3f(B.x+(epaisseur/2), B.y, hauteur);
            glVertex3f(A.x+(epaisseur/2), A.y, hauteur);
        glEnd();
        //face de dessus
        glBegin(GL_POLYGON);
            glNormal3f(0,0,1.0);
            glVertex3f(A.x-(epaisseur/2), A.y, hauteur);
            glVertex3f(A.x+(epaisseur/2), A.y, hauteur);
            glVertex3f(B.x+(epaisseur/2), B.y, hauteur);
            glVertex3f(B.x-(epaisseur/2), B.y, hauteur);
        glEnd();
        //face arriere
        glBegin(GL_POLYGON);
        glDisable(GL_CULL_FACE);
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
        //face de dessus
        glBegin(GL_POLYGON);
            glNormal3f(0,0,1.0);
            glVertex3f(A.x, A.y-(epaisseur/2), hauteur);
            glVertex3f(A.x, A.y+(epaisseur/2), hauteur);
            glVertex3f(B.x, B.y+(epaisseur/2), hauteur);
            glVertex3f(B.x, B.y-(epaisseur/2), hauteur);
        glEnd();
        //face droite
        glBegin(GL_POLYGON);
            glNormal3f(-1.0,0.0,0.0);
            glVertex3f(A.x, A.y+(epaisseur/2),0);
            glVertex3f(A.x, A.y-(epaisseur/2),0);
            glVertex3f(A.x, A.y-(epaisseur/2),hauteur);
            glVertex3f(A.x, A.y+(epaisseur/2),hauteur);
        glEnd();
        //face gauche
        glBegin(GL_POLYGON);
           glNormal3f(1.0,0.0,0.0);
           glVertex3f(B.x, B.y+(epaisseur/2),0);
           glVertex3f(B.x, B.y-(epaisseur/2),0);
           glVertex3f(B.x, B.y-(epaisseur/2),hauteur);
           glVertex3f(B.x, B.y+(epaisseur/2),hauteur);
        glEnd();
    } else {
        cout << "erreur: mur non vertical ou horizontal" << endl;
    }
    //code pour faire un mur
}

/* ----------------------------------------------------------
// PLUS TARD DANS LE MAIN  QUAND ON AURA REUSSI A LINK SA MER
------------------------------------------------------------*/

void def_axes(){

    glBegin(GL_LINES);
        //x
        GLfloat red[4] = {1.0f,0.0f,0.0f,1.0f};
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
        glVertex3f(0.0,0.0,0.0);
        glVertex3f(1,0.0,0.0);
        //y
        GLfloat green[4] = {0.0f,1.0f,0.0f,1.0f};
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
        glVertex3f(0.0,0.0,0.0);
        glVertex3f(0.0,1,0.0);
        //z
        GLfloat blue[4] = {0.0f,0.0f,1.0f,1.0f};
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
        glColor3f(0.0, 0.0, 1.0);
        glVertex3f(0.0,0.0,0.0);
        glVertex3f(0.0,0.0,1);
    glEnd();

}