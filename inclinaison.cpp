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


using namespace cv;
using namespace std;

//compilation: make
//test: ./inclinaison feuille_inclinee.jpg

//VARIABLES GLOBALES
cv::Mat image ;                                   //L'image chargé dans le terminal
cv::Mat H ;                                       //La matrice contenant l'homographie
GLsizei WIDTH = 400, HEIGHT = 400;                //Largeur et Hauteur de la fenetre
GLfloat colors[][3] = { { 1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f }, {0.0f, 0.0f, 1.0f }, {0.0f, 0.0f, 0.0f },  {1.0f, 1.0f, 1.0f }  }; //Contient les couleurs que l'on veut utiliser en RGB
//Index des couleurs
int red = 0 ;
int green = 1 ;
int blue = 2 ;
int black = 3 ;
int white = 4 ;
int cpt=0 ;


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

void affichage(void){

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ; //On efface le buffer d'ecran

  glLoadIdentity() ;

  glClearColor(colors[black][0], colors[black][1], colors[black][2], 1.0f); //Couleur de fond en RGBA

  //Construction d'un rectangle blanc représentant la feuille
  glColor3f(colors[white][0], colors[white][1], colors[white][2]) ;
  glBegin(GL_QUADS);
    
    glVertex2f(100.0, 100.0);
    glVertex2f(300.0, 100.0);
    glVertex2f(300.0, 200.0);
    glVertex2f(100.0, 200.0);
  glEnd() ;

  //L'axe Z
  glBegin(GL_LINES) ;
    glColor3f(colors[red][0], colors[red][1], colors[red][2]) ;
    glVertex3f(200, 150, -1000); //On le place au centre du rectangle
    glVertex3f(200, 150, +1000);
  glEnd() ;

  glFlush();


  glutSwapBuffers() ; //On indique ce qu'il faut afficher
}

//callback appelé à chaque fois que le fenetre est redimensionnee (une fois la fenetre crée)
void redim(int width, int height){
  WIDTH = glutGet(GLUT_WINDOW_WIDTH) ;
  HEIGHT = glutGet(GLUT_WINDOW_HEIGHT) ; //Recuperation de la taille de la fenetre
  glViewport(0, 0, (GLsizei)width, (GLsizei)height) ; //"On affiche toute la fenetre"
  //Les 3 lignes de code suivantes me permettent de remplacer les coordonees par leur equivalent en pixels...
  //(...ce qui est fort pratique puisque l'on ne sait pas vraiment comment glVertex interprete nos coordonees...)
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0,WIDTH,HEIGHT,0,-1.0,1.0);//On adapte les coordonees par rapport a la dimension actuelle de la fenetre
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}


int main( int argc, char **argv ) {

    if ( argc != 2 ) {
       printf( "Usage: analyse_image ImageToLoadAndDisplay\n" );
       return -1;
    }

/*Lecture et Affichage d'une image entrée dans le terminal--------------------------------------------------*/

    image = cv::imread( argv[1] ); /* Read the file */

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

    //Les coordonnées des coins du modèle
    Point point_modele_1 = Point(0,0) ;
    Point point_modele_2 = Point(991,0) ;
    Point point_modele_3 = Point(0,715) ;
    Point point_modele_4 = Point(991,715) ;

    //Les coordonnées des coins de la feuille inclinée
    Point point_feuille_1 = Point(185,114) ;
    Point point_feuille_2 = Point(799,149) ;
    Point point_feuille_3 = Point(61,519) ;
    Point point_feuille_4 = Point(977,501) ;

    //Création de vecteurs de points
    std::vector<Point> obj; //Contient les points de la feuille
    obj.push_back(point_feuille_1) ;
    obj.push_back(point_feuille_2) ;
    obj.push_back(point_feuille_3) ;
    obj.push_back(point_feuille_4) ;

    std::vector<Point2f> scene; //Contient les points du modèle
    scene.push_back(point_modele_1) ;
    scene.push_back(point_modele_2) ;
    scene.push_back(point_modele_3) ;
    scene.push_back(point_modele_4) ; 

    //Calcul de l'homographie (=> Inclinaison)
    H = findHomography(obj, scene, CV_RANSAC) ; //H contient l'homographie

    //Conversion de la matrice de rotation en un vecteur de rotation
    Mat rot_vec = Mat::zeros(1,3,CV_32F) ;
    Rodrigues(H, rot_vec) ; //Conversion


/*Visualisation de la feuille dans un plan (OpenGL)-------------------------------------------------------------*/

    glutInit(&argc, argv); //initialisation de GLUT

    //Taille et emplacement de la fenetre
    glutInitWindowSize(WIDTH, HEIGHT) ;
    glutInitWindowPosition(0, 0) ;

    //Choix du type et d'affichage RGBA (mode couleur le plus fréquent), tampon de profondeur et d'un double buffer
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);

    //Creation de la fenetre
    glutCreateWindow("GLUTFenetre");

    glutPostRedisplay();

    //Association des callback pour cette fenetre
    glutDisplayFunc(affichage) ;
    glutReshapeFunc(redim) ;

    glutMainLoop(); //On entre dans la boucle d'evenements

}

