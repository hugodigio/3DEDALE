#include "game.hpp"
#include "mainMenu.hpp"

labyrinthe lab;
int window_w_game;
int window_h_game;

void clavier_game(unsigned char key, int x, int y){
    switch(key){
        case 27: //touche echap
            LoadMainMenu(window_w_game, window_h_game);
            break;
    }
}
void specialKey_game(int keycode, int x, int y){
    //code
    
}

void souris_game(int button, int state, int x, int y){
    //code
}

void souris_mouvement_game(int x, int y){
    //code
}

//BOUCLE DE JEU
void display_game(void){
    // Scale to window size
    glViewport(0, 0, window_w_game, window_h_game);

    // Draw stuff
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION_MATRIX);
    glLoadIdentity();
    gluPerspective( 75, (double)800 / (double)600, 0.1, 100 );

    glMatrixMode(GL_MODELVIEW_MATRIX);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    
    glPushMatrix();
        float lightPosition[4] = {2, 3, 10, 1};
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
        GLfloat Lambiant [4] = {0.4, 0.4, 0.4, 1.0};
        GLfloat Lblanche [4] = {1.0, 1.0, 1.0, 1.0};
        glLightfv (GL_LIGHT0, GL_AMBIENT, Lambiant);
        glLightfv (GL_LIGHT0, GL_DIFFUSE, Lblanche);
        glLightfv (GL_LIGHT0, GL_SPECULAR, Lblanche);
    glPopMatrix();
    //        eyeX,eyeY,eyeZ,centerX,centerY,centerZ,upX,upY,upZ
    gluLookAt(2.0, 5.0, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);

    def_axes();
    creerlabyrinthe(lab);
    


    //on affiche
    glutSwapBuffers();

}

void redim_game(int window_width, int window_height){
    window_w_game = window_width;
    window_h_game = window_height;
    glViewport(0, 0, window_w_game, window_h_game);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, window_w_game, window_h_game, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glutSwapBuffers();
}

void LoadGame(int window_width, int window_height){
    window_w_game = window_width;
    window_h_game = window_height;
    lab = test();
    glutDisplayFunc(display_game);
    glutReshapeFunc(redim_game);


    //clavier, souris, motion
    glutKeyboardFunc(clavier_game);
    glutSpecialFunc(specialKey_game);
    glutMouseFunc(souris_game);
    glutPassiveMotionFunc(souris_mouvement_game);

    glutPostRedisplay();
}