//  Created by HASSANEIN Alzahra
//

#include <GLUT/glut.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <vector>
//#include <Box2D/Box2D.h>

/** Pensez a faire :
 
 FAIT Tourner l'affichage pour une initialisation a plat
 Faire une fonction init
 
 FAIT  Matrices pour la boule
 FAIT  Matrice pour le plateau
 Voir si le soucis de couleur de la boule est regle, par defaut la couleur verte
 du dernier objet dessine
 RESOLU  Nouveau Problème : Disparition de l'axe Z (bleu) a l'affichage
 
 RESOLU Voir si on peut faire un axe qui fait moins "attacher" au plan
 Il sera enlever pour la fin
 
 RESOLU Nouveau probleme : Translation de la boule sur le plateau ne fonctionne pas
 On dirait que la translation sur Z pose un soucis, on a l'effet de zoom mais
 pas le rapprochement par rapport au plateau
 La ligne MouvementM(M_boule,0,(0.3-SECTION/4.)); diminue la taille de la boule,
 au lieu de la deplacer
 
 FAIT  Enlevez les a et remplacer par SECTION
 Uniformisez la création du plateau (et déplacer par Translation et non pas
 recreer par coordonnées)
 
 FAIT Creer des variables pour les angles de rotations du plateau,
 cf Fontion Rotation
 
 FAIT Creer une fonction mouvement pour la boule,
 REDEFINI Definir la vitesse a partir de l'angle de Rotation du plateau
 Faire un deplacecment plus naturel :  en fonction de la circonference
 
 RESOLU Definir a partir de quel angle la boule ne bougera plus (plus de 90 la boule
 devrait "tomber" du plateau,
 On ne bouge pas en Z pour le moment, la balle reste collé et se deplace en fonction meme
 a l'envers, prendre apres 180 degre, inverser les deplacements
 
 Etudier BOX2D pour
 la physique
 et le deplacement coulissant
 et la gestion des collisions (premiere collision mur du bord donc largeur (ou section)
 plus ou moins le rayon esst la distance maximale, suite collision au mur, il faut detecter
 si on est pas sur une coordoonées d'une ligne d'un mur, donc avec le tableau de ligne
 
 Faire des murs avec une epaisseur,
 Faire un tableau de ligne pour dessiner les murs internes
 
 Voir la latence
 Mouvemet diagonale
 Regler le soucis de bidouillage du mur x
 Revoir pour les murs ce ne sera pas max et min
 
 Commenter
 
 **/


#define LONGUEUR 5
#define SECTION 3

using namespace std;
float M_plateau[16];
float M_boule[16];

///
float rotx = 10.;
float roty = 10.;

float rotationActuelleX = 0.;
float rotationActuelleY = 0.;

float deplacementX = 0.1;
float deplacementY = 0.1;
int valeurChangement = 5;

///
int etatMouvement = 0;
int direction = 4;

float bouleX = 0.;
float bouleY = 0.;

vector<float> murCoordX;
vector<float> murCoordY;

double Xmax, Ymax;
double Xmin, Ymin;

void remplissageCoordMur(){
    //Murs
    murCoordX.push_back(-LONGUEUR*1./2.);
    murCoordY.push_back(SECTION*1./2.);
    murCoordX.push_back(LONGUEUR*1./2.);
    murCoordY.push_back(SECTION*1./2.);
    murCoordX.push_back(LONGUEUR*1./2.);
    murCoordY.push_back(-SECTION*1./2.);
    murCoordX.push_back(-LONGUEUR*1./2.);
    murCoordY.push_back(-SECTION*1./2.);
    
    bouleX =-0.3-SECTION/2.;
    bouleY =-0.3+SECTION/2.;
    
    cout << "MUR COORD X" << endl;
    vector <float>::const_iterator i;
    for( i=murCoordX.begin(); i!=murCoordX.end(); ++i){
        cout<<(*i)<<endl;
    }
    
    cout << "MUR COORD Y" << endl;
    for(i=murCoordY.begin(); i!=murCoordY.end(); ++i){
        cout<<(*i)<<std::endl;
    }
    
    Xmax = *max_element(murCoordX.begin(), murCoordX.end());
    cout<<"XMax value: "<<Xmax<<endl;
    Ymax = *max_element(murCoordY.begin(), murCoordY.end());
    cout<<"XMax value: "<<Ymax<<endl;
    
    Xmin = *min_element(murCoordX.begin(), murCoordX.end());
    cout<<"XMin value: "<<Xmin<<endl;
    Ymin = *min_element(murCoordY.begin(), murCoordY.end());
    cout<<"XMin value: "<<Ymin<<endl;
}


//Trace les axes, X rouge, Y Vert, Z Bleu
void def_axes(void){
    
    
    glBegin(GL_LINES);
    //x
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-1.,0.0,0.0);
    glVertex3f(1.,0.0,0.0);
    //y
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0.0,-1,0.0);
    glVertex3f(0.0,1.,0.0);
    //z
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0.0,0.0,-1.);
    glVertex3f(0.0,0.0,1.);
    glEnd();
    
}


void def_carre(void){
    int a = 1;
    glBegin(GL_POLYGON);
    glVertex2f(-a/2.0,a/2.0);
    glVertex2f(a/2.0,a/2.0);
    glVertex2f(a/2.0,-a/2.0);
    glVertex2f(-a/2.0 ,-a/2.0);
    glEnd();
    
    
}

void def_boule(void){
    
    glPushMatrix();
    //float blanc[] = { 1.0F,1.0F,1.0F,1.0F };
    //glMaterialfv(GL_FRONT,GL_SPECULAR,blanc);
    glutSolidSphere(0.3,36,36);
    
    glPopMatrix();
    
    /*cout << "INITIALISATION" <<endl;
     cout << bouleX << endl;
     cout << bouleY << endl;
     */
}

void def_plateau(void){
    
    glPushMatrix();
    glColor3f(0.0, 0.5, 1.0);
    glTranslatef(0.0, 0.0, -SECTION/4.0);
    glRotatef(180, 1, 0, 0);
    glScalef(SECTION*3,SECTION*2.,SECTION);
    def_carre();
    glPopMatrix();
}


void def_boite(void){
    
    
    
    // face bleu, le fond
    //on le met devant en translatant selon z
    glPushMatrix();
    glColor3f(0.0, 0.1, 1.0);
    glTranslatef(0.0, 0.0, -SECTION/4.0);
    glRotatef(180, 1, 0, 0);
    glScalef(LONGUEUR,SECTION,SECTION);
    def_carre();
    glPopMatrix();
    
    // face rouge, de droite
    //rotation de 90 selon l'axe des y et translation sur les x et z
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glTranslatef(LONGUEUR/2.0, 0.0, 0.0);
    glRotatef(90, 0, 1, 0);
    glScalef(SECTION/2.,SECTION,SECTION);
    //glScalef(SECTION,SECTION,SECTION/2.0);
    //glScalef(LONGUEUR*a,SECTION,SECTION);
    def_carre();
    
    glPopMatrix();
    
    // face jaune, de gauche
    glPushMatrix();
    glColor3f(1.0, 1.0, 0.0);
    glTranslatef(-LONGUEUR/2.0, 0.0,0.0);
    glRotatef(-90, 0, 1, 0);
    glScalef(SECTION/2.,SECTION,SECTION);
    //glScalef(SECTION,LONGUEUR*a,SECTION);
    //glScalef(LONGUEUR*a,SECTION,SECTION);
    def_carre();
    glPopMatrix();
    
    // face blanche, de haut
    
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glTranslatef(0.0, SECTION/2.0, 0.0);
    glRotatef(-90, 1, 0, 0);
    glScalef(LONGUEUR,SECTION/2.,SECTION);
    //glScalef(SECTION,LONGUEUR*a,SECTION);
    def_carre();
    glPopMatrix();
    
    //face de bas, verte
    glPushMatrix();
    glColor3f(0.0, 1.0, 0.0);
    glTranslatef(0.0, -SECTION/2.0, 0.0);
    glRotatef(90, 1, 0, 0);
    glScalef(LONGUEUR,SECTION/2.,SECTION);
    //glScalef(SECTION,LONGUEUR*a,SECTION);
    def_carre();
    glPopMatrix();
    
}


//Axes : X rouge, Y Vert, - X, | Y
void RotationM(float M[16], int valaxe, float angle) {
    glPushMatrix();
    glLoadIdentity();
    glLoadMatrixf(M);
    
    switch (valaxe) {
        case 1: //x
            glRotatef(angle, 1.0, 0.0, 0.0);
            rotationActuelleX += angle;
            cout << "ROTATION SUR XXXXXXXXXXXXXXXXX " << angle << " X " <<rotationActuelleX << endl;
            break;
        case 2: //y
            glRotatef(angle, 0.0, 1.0, 0.0);
            rotationActuelleY += angle;
            cout <<"ROTATION SUR YYYYYYYYYYYYYYYYYY " <<  angle << " Y "<<   rotationActuelleY << endl;
            break;
            //        case 3://z
            //            glRotatef(angle, 0.0, 0.0, 1.0);
            //            break;
    }
    glGetFloatv(GL_MODELVIEW_MATRIX, M);
    glPopMatrix();
}

void MouvementM(float M[16], int valaxe, float distance) {
    glPushMatrix();
    glLoadIdentity();
    glLoadMatrixf(M);
    
    switch (valaxe) {
        case 1: //x
            glTranslatef(distance, 0.0, 0.0);
            break;
        case 2: //y
            glTranslatef( 0.0, distance , 0.0);
            break;
            //        case 3://z
            //            glTranslatef(0.0, 0.0, distance);
            //            break;
            
    }
    glGetFloatv(GL_MODELVIEW_MATRIX, M);
    glPopMatrix();
}


void changerEtat(){
    etatMouvement = 1;
}

bool estChanger(){
    if (etatMouvement == 1){
        etatMouvement = 0;
        return true;
    }
    return false;
}

int donnerCas(int cas){
    direction = cas;
    return direction;
}

bool collisionMurs(int cas){
    
    if (cas ==0) // gauche
        if ( bouleX <= Xmin +0.4)
            return true;
    if (cas==1) // droite
        if (bouleX>= Xmax - 0.3)
            return true;
    if (cas==2) //bas
        if (bouleY>= Ymax -0.3)
            return true;
    if (cas==3) //haut
        if ( bouleY<= Ymin + 0.3)
            return true;
    
    
    //    if (find(murCoordX.begin(), murCoordX.end(), bouleX) != murCoordX.end())
    //        return true;
    //    else if(find(murCoordY.begin(), murCoordY.end(), bouleY) != murCoordY.end())
    //        return true;
    return false;
}

//Axes : X rouge, Y Vert, - X, | Y
int checkSigne(int axis){
    
    if (axis == 1){ //haut, bas
        if (rotationActuelleX == 0.)
            return 0;
        else if (rotationActuelleX > 0. && rotationActuelleX <=90.){
            if (collisionMurs(3)){
                //                cout << "COLLISION MUR DU BAS"<< endl;
                return 0;}
            return -1;
        }else if (rotationActuelleX <0. && rotationActuelleX >= -90.){
            if (collisionMurs(2)){
                //               cout << "COLLISION MUR DU HAUT"<< endl;
                return 0;}
            return 1;
            
        }
    }
    
    else if(axis == 2){ // droite, gauche
        if (rotationActuelleY == 0.)
            return 0;
        else if (rotationActuelleY > 0. && rotationActuelleY <=90.){
            if (collisionMurs(1)){
                //                cout << "COLLISION MUR DE DROITE" << endl;
                return 0;}
            return 1;
        }else if (rotationActuelleY <0. && rotationActuelleY >= -90.){
            if (collisionMurs(0)){
                //                cout << "COLLISION MUR DE GAUCHE" <<endl;
                return 0;}
            return -1;
            
        }
    }
}


void affichageCoordBoule(int val){
    cout << val << endl;
    cout << bouleX << endl;
    cout << bouleY << endl;
    cout << "---------------------" <<endl;
}

void Timer(int value){
    
    //affichageCoordBoule(value);
    
    //on fait les deplacements en fonction de l'angle de rotation x ou y
    
    //Axes : X rouge, Y Vert, - X, | Y
    if (value==0){ //Va vers la gauche // q
        if(collisionMurs(0) == 0){ //tant que la boule est entre 0 et Longueur - 36
            MouvementM(M_boule,1,deplacementX * checkSigne(2));
            bouleX = bouleX + (deplacementX * checkSigne(2));
            valeurChangement = 5;
        }else
            valeurChangement = 4;
    }else if(value==1){ // va vers la droite // d
        if(collisionMurs(1) == 0){
            MouvementM(M_boule,1,deplacementX * checkSigne(2));
            bouleX = bouleX + (deplacementX * checkSigne(2));
            valeurChangement = 5;
        }else
            valeurChangement = 4;
    }else if(value==2){ //va vers le haut // z
        if(collisionMurs(2)==0){
            MouvementM(M_boule,2,deplacementY * checkSigne(1));
            bouleY = bouleY + (deplacementY * checkSigne(1));
            valeurChangement = 5;
        }else
            valeurChangement = 4;
    }else if(value==3){ //va vers le bas // s
        if(collisionMurs(3)==0){
            MouvementM(M_boule,2,deplacementY * checkSigne(1));
            bouleY = bouleY + (deplacementY * checkSigne(1));
            valeurChangement = 5;
        }else
            valeurChangement = 4;
    }else if(value==4){
        MouvementM(M_boule,2,0);
        valeurChangement = 5;
    }
    
    glutPostRedisplay();
    
    if (valeurChangement == 5) {
        if(estChanger) //si on detecte une autre positions
            glutTimerFunc(20, Timer, direction);
        //        else {//On ne rentre jamais ici
        //            cout << "on ne rentre jamais iciiiiiiiiiiiiiiiiiiiiiiii"<<endl;
        //            glutTimerFunc(20, Timer, value);}
    }else
        
        glutTimerFunc(20,Timer,valeurChangement);
    
}

//Axes : X rouge, Y Vert, - X, | Y
void clavier(unsigned char key, int x, int y){
    switch (key){
        case 'q':
        case 'Q':
            RotationM(M_plateau,2,-roty); //-10
            donnerCas(0);
            changerEtat();
            Timer(0);
            break;
        case 'd':
        case 'D':
            RotationM(M_plateau,2,roty); //10
            donnerCas(1);
            changerEtat();
            Timer(1);
            break;
        case 'z':
        case 'Z':
            RotationM(M_plateau,1,-rotx); //-10
            donnerCas(2);
            changerEtat();
            Timer(2);
            break;
        case 's':
        case 'S':
            RotationM(M_plateau,1,rotx); //10
            donnerCas(3);
            changerEtat();
            Timer(3);
            break;
        case 'p':
        case 'P':
            donnerCas(4);
            changerEtat();
            Timer(4);
            break;
            
            /*
             case 'i':
             case 'I':
             //Initialisation();
             break;
             */
        case 'a':
        case'A' :
            exit (0);
            break;
        default:
            break;
    }glutPostRedisplay();
}

//void clavierBoule(int key, int x, int y){
//    switch (key){
//        case GLUT_KEY_UP:
//            MouvementM(M_boule,2,0.1);
//            break;
//        case GLUT_KEY_DOWN:
//            MouvementM(M_boule,2,-0.1);
//            break;
//        case GLUT_KEY_LEFT:
//            MouvementM(M_boule,2,-0.1);
//            break;
//        case GLUT_KEY_RIGHT:
//            MouvementM(M_boule,1,0.1);
//            break;
//    }glutPostRedisplay();
//
//}

//Axes : X rouge, Y Vert, - X, | Y
void dessinScene(){
    glPushMatrix();
    glMultMatrixf(M_plateau);
    //glTranslatef(-3.0,-3.0,0.0);
    def_axes();
    glPopMatrix();
    
    glPushMatrix();
    
    glMultMatrixf(M_plateau);
    def_plateau();
    def_boite();
    //glRotatef(-90,0,1,0);
    glPopMatrix();
    
    glPushMatrix();
    //
    glMultMatrixf(M_plateau);
    glTranslatef(-0.3-SECTION/2., -0.3+SECTION/2., 0.3-SECTION/4.0);
    glMultMatrixf(M_boule);
    def_boule();
    glPopMatrix();
}

//Axes : X rouge, Y Vert, - X, | Y
void affichage(void){
    //on efface le buffer d'ecran
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_BACK, GL_LINE); // a enlever par la suite
    
    
    glPushMatrix();
    glRotatef(-80.0,1,0,0);
    //rotationActuelleX = 10.;
    dessinScene();
    glPopMatrix();
    
    
    
    
    //on affiche
    glutSwapBuffers();
}



void redim(int width, int height){
    glEnable(GL_DEPTH_TEST);
    glViewport(0,0,width, height);
    
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    //          left,right,bot,top,nearVal,farVal
    //glFrustum (-1.0, 1.0, -1.0, 1.0, 1.0, 20.0);
    //ou
    gluPerspective(75,(float)width/height, 1.0,100.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    //Initialisation des matrices a Identite
    glGetFloatv(GL_MODELVIEW_MATRIX,M_plateau);
    glGetFloatv(GL_MODELVIEW_MATRIX,M_boule);
    
    //         eyeX,eyeY,eyeZ,centerX,centerY,centerZ,upX,upY,upZ
    //gluLookAt (1.0, -2.5, -0.5, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    gluLookAt    (0.0, 0.0, 9.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}


void init (){
    
    //Taille et emplacemet de la fenetre
    glutInitWindowSize(600,600);
    glutInitWindowPosition(200,100);
    
    //Type d'affichage, Tampon de profondeur, Double buffer
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    
    //Creation de la premiere fenetre
    glutCreateWindow("3Dedale");
    remplissageCoordMur();
    //Association des callback
    glutDisplayFunc(affichage);
    glutReshapeFunc(redim);
    
    //Action Clavier
    glutKeyboardFunc(clavier);
    //glutSpecialFunc(clavierBoule);
    
    glutMainLoop();

    
}
void load_boule(int argc, char **argv){
    //Initialisation de GLUT
    glutInit(&argc, argv);
    init();
}


