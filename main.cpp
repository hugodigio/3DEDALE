//  Created by HASSANEIN Alzahra
//

#include <GLUT/glut.h>
#include <stdlib.h>


#define LONGUEUR 5
#define SECTION 3

float WIDTH = 400;
float HEIGHT = 400;


void def_carre(void){
    int a = 1;
    glBegin(GL_POLYGON);
    glVertex2f(-a/2.0,a/2.0);
    glVertex2f(a/2.0,a/2.0);
    glVertex2f(a/2.0,-a/2.0);
    glVertex2f(-a/2.0 ,-a/2.0);
    glEnd();
    
}

void def_plateau(void){
    
    //on le met devant en translatant selon z
    glPushMatrix();
    glColor3f(0.0, 0.5, 1.0);
    glTranslatef(0.0, 0.0, -SECTION/4.0);
    glRotatef(180, 1, 0, 0);
    glScalef(SECTION*3,SECTION*2.,SECTION);
    def_carre();
    glPopMatrix();
}

void def_mur(){
    //on le met devant en translatant selon z
    glPushMatrix();
    glColor3f(0.0, 0.5, 1.0);
    glTranslatef(0.0, 0.0, -SECTION/2.0);
    glRotatef(180, 1, 0, 0);
    glScalef(9.0,SECTION*3.,SECTION);
    def_carre();
    glPopMatrix();
}

void def_boite(int a){
    
    //Par rapport au carre vert de base
    
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

//Trace les axes, X rouge, Y Vert, Z Bleu
void def_axes(void){
    
    
    glBegin(GL_LINES);
    //x
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-1,0.0,0.0);
    glVertex3f(1,0.0,0.0);
    //y
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0.0,-1,0.0);
    glVertex3f(0.0,1,0.0);
    //z
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0.0,0.0,-1);
    glVertex3f(0.0,0.0,1);
    glEnd();
}

void def_boule(void){
    glPushMatrix();
    
    float blanc[] = { 1.0F,1.0F,1.0F,1.0F };
    glMaterialfv(GL_FRONT,GL_SPECULAR,blanc);
    glutSolidSphere(0.3,36,36);
    glPopMatrix();
}


void affichage(void){
    //on efface le buffer d'ecran
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_BACK, GL_LINE);
    
    def_axes();
    
    def_plateau();
    def_boite(SECTION);
    
    
    def_boule();
    
    
    
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
    gluPerspective(75,1.0, 1.0,20.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //         eyeX,eyeY,eyeZ,centerX,centerY,centerZ,upX,upY,upZ
    //gluLookAt (1.0, -2.5, -0.5, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    gluLookAt    (0.0, 0.0, 6.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}




void clavier(unsigned char key, int x, int y){
    switch (key){
        case 'q':
        case 'Q':
            glRotatef(-10, 0,1,0);
            break;
        case 'd':
        case 'D':
            glRotatef(10, 0,1,0);
            break;
        case 'z':
        case 'Z':
            glRotatef(-10, 1,0,0);
            break;
        case 's':
        case 'S':
            glRotatef(10, 1,0,0);
            break;
        case 'a':
        case'A' :
            exit (0);
            break;
        default:
            break;
    }glutPostRedisplay();
}



int main (int argc, char **argv){
    
    //Initialisation de GLUT
    glutInit(&argc, argv);
    
    //Taille et emplacemet de la fenetre
    glutInitWindowSize(400,400);
    glutInitWindowPosition(200,100);
    
    //Type d'affichage, Tampon de profondeur, Double buffer
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    
    //Creation de la premiere fenetre
    glutCreateWindow("Polygon");
    
    //Association des callback
    glutDisplayFunc(affichage);
    glutReshapeFunc(redim);
    
    //Action Clavier
    glutKeyboardFunc(clavier);
    //glutMouseFunc(souris);
    
    glutMainLoop();
    
    return 0;
    
}
