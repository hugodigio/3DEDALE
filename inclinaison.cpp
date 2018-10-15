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

//compilation: make
//test: ./inclinaison feuille_inclinee.jpg

//VARIABLES GLOBALES
cv::Mat image ;                                   //L'image chargé dans le terminal
Mat rot_vec(3,1,cv::DataType<double>::type);      //Rotation estimée de la feuille
Mat trans_vec(3,1,cv::DataType<double>::type);    //Translation estimée de la feuille (optionnel)
GLsizei WIDTH = 400, HEIGHT = 400;                //Largeur et Hauteur de la fenetre
GLfloat colors[][3] = { { 1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f }, {0.0f, 0.0f, 1.0f }, {0.0f, 0.0f, 0.0f },  {1.0f, 1.0f, 1.0f }  }; //Contient les couleurs que l'on veut utiliser en RGB
//Index des couleurs
int red = 0 ;
int green = 1 ;
int blue = 2 ;
int black = 3 ;
int white = 4 ;
int cpt=0 ;
int rotation_globale_y = 0 ; //Possibilité de faire tourner l'ensemble des objets sur l'axe y pour vérifier l'inclinaison


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

void clavier(unsigned char key, int x, int y){

  switch (key) {
    case 'y': 
      rotation_globale_y += 5 ; 
      rotation_globale_y = rotation_globale_y % 360 ;
      cout << rotation_globale_y << "\n" ;
      glutPostRedisplay();
      break ;
  }

}

void def_axes(){
  glBegin(GL_LINES) ;
    glColor3f(colors[red][0], colors[red][1], colors[red][2]) ; //Axe X
    glVertex3f(200, 150, 0); 
    glVertex3f(2000, 150, 0);
    glColor3f(colors[green][0], colors[green][1], colors[green][2]) ; //Axe Y
    glVertex3f(200, 150, 0);
    glVertex3f(200, 2000, 0);
    glColor3f(colors[blue][0], colors[blue][1], colors[blue][2]) ; //Axe Z
    glVertex3f(200, 150, 0);
    glVertex3f(200, 150, 2000);
  glEnd() ;

}

void affichage(void){

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ; //On efface le buffer d'ecran
  glEnable(GL_DEPTH_TEST) ; //On active le z-buffer

  glLoadIdentity() ;

  glClearColor(colors[black][0], colors[black][1], colors[black][2], 1.0f); //Couleur de fond en RGBA

  glPushMatrix() ;
    glRotatef(rotation_globale_y, 0, 1, 0) ;
    //Rotation/Translation du repère
    glRotated(rot_vec.at<double>(0,0), 1, 0, 0) ; //Axe X
    glRotated(rot_vec.at<double>(1,0), 0, 1, 0) ; //Axe Y
    glRotated(rot_vec.at<double>(2,0), 0, 0, 1) ; //Axe Z
    //glTranslated(trans_vec.at<double>(0,0) , trans_vec.at<double>(1,0) , trans_vec.at<double>(2,0) ) ;
    //Les axes
    def_axes() ;
  glPopMatrix() ;

  //Construction d'un rectangle blanc représentant la feuille
  glColor3f(colors[white][0], colors[white][1], colors[white][2]) ;
  glPushMatrix() ;
    glRotatef(rotation_globale_y, 0, 1, 0) ;
    //Rotation/Translation du repère
    glRotated(rot_vec.at<double>(0,0), 1, 0, 0) ; //Axe X
    glRotated(rot_vec.at<double>(1,0), 0, 1, 0) ; //Axe Y
    glRotated(rot_vec.at<double>(2,0), 0, 0, 1) ; //Axe Z
    //glTranslated(trans_vec.at<double>(0,0) , trans_vec.at<double>(1,0) , trans_vec.at<double>(2,0) ) ;
    glBegin(GL_QUADS);
        //Le rectangle
        glVertex2f(100.0, 100.0);
        glVertex2f(300.0, 100.0);
        glVertex2f(300.0, 200.0);
        glVertex2f(100.0, 200.0);
    glEnd() ;
  glPopMatrix() ;

  glFlush();


  glutSwapBuffers() ; //On indique ce qu'il faut afficher
}

//callback appelé à chaque fois que le fenetre est redimensionnee (une fois la fenetre crée)
void redim(int width, int height){
  glViewport(0, 0, (GLsizei)width, (GLsizei)height) ; //"On affiche toute la fenetre"
  glMatrixMode(GL_PROJECTION); //Projection sur un autre plan
  glLoadIdentity();
  gluOrtho2D(0, width, 0, height) ;
  glMatrixMode(GL_MODELVIEW);
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
    Point2f point_modele_1(0,0) ;
    Point2f point_modele_2(991,0) ;
    Point2f point_modele_3(0,715) ;
    Point2f point_modele_4(991,715) ;

    //Les coordonnées des coins de la feuille inclinée
    Point3f point_feuille_1(185,114,0) ;
    Point3f point_feuille_2(799,149,0) ;
    Point3f point_feuille_3(61,519,0) ;
    Point3f point_feuille_4(977,501,0) ;

    //Création de vecteurs de points
    std::vector<Point3f> obj; //Contient les points de la feuille
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
    cv::Mat cameraMatrix(3,3,cv::DataType<double>::type);
    cv::setIdentity(cameraMatrix); //Valeur par défaut

    cv::Mat distCoeffs(4,1,cv::DataType<double>::type);
    distCoeffs.at<double>(0) = 0;
    distCoeffs.at<double>(1) = 0;
    distCoeffs.at<double>(2) = 0;
    distCoeffs.at<double>(3) = 0; //Valeur par défaut

    cv::solvePnP(obj, scene, cameraMatrix, distCoeffs, rot_vec, trans_vec);

    cout << rot_vec << "\n" ;
    cout << trans_vec << "\n" ;

    //NB: rot_vec et trans_vec sont le résultat attendus par le programmme
    //La partie visualisation ci-dessous sert de vérification


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
    glutKeyboardFunc(clavier);

    glutMainLoop(); //On entre dans la boucle d'evenements

}

