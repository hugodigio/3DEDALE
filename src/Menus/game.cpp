#include "game.hpp"

labyrinthe lab;

void LoadGame(){
    lab = test();
    glutDisplayFunc(gameDisplayFunc);
    glutPostRedisplay();
}

void gameDisplayFunc(void){
    // Scale to window size
    glViewport(0, 0, 800, 600);

    // Draw stuff
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION_MATRIX);
    glLoadIdentity();
    gluPerspective( 75, (double)800 / (double)600, 0.1, 100 );
    //        eyeX,eyeY,eyeZ,centerX,centerY,centerZ,upX,upY,upZ
    gluLookAt(15.0, 5.0, 5.0, 5.0, 5.0, 0.0, 0.0, 0.0, 1.0);

    glMatrixMode(GL_MODELVIEW_MATRIX);
    glTranslatef(0,0,0);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    glPushMatrix();
        float lightPosition[4] = {2, 3, 10, 1};
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
        GLfloat Lambiant [4] = {0.4, 0.4, 0.4, 1.0};
        GLfloat Lblanche [4] = {1.0, 1.0, 1.0, 1.0};
        glLightfv (GL_LIGHT0, GL_AMBIENT, Lambiant);
        glLightfv (GL_LIGHT0, GL_DIFFUSE, Lblanche);
        glLightfv (GL_LIGHT0, GL_SPECULAR, Lblanche);
    glPopMatrix();

    creerlabyrinthe(lab);
    
    def_axes();

    glColor4f(0.0f, 0.0f, 1.0f, 1.0f);  //RGBA values of text color
    glRasterPos2i(100, 120);            //Top left corner of text
    const unsigned char* t = reinterpret_cast<const unsigned char *>("text to render");
    // Since 2nd argument of glutBitmapString must be const unsigned char*
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'c');

    //on affiche
    glutSwapBuffers();

}