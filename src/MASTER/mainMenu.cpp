#include "mainMenu.hpp"
#include "game.hpp"
#include "../OPENCV/camera.hpp"
#include "labyrinthe.hpp"
#define NB_OPTIONS 4

using namespace std;

int window_h = 600;
int window_w = 800;

//variable du menu
int current_option_selected = 0;
string MainMenu[NB_OPTIONS] = {
    "JOUER",
    "FULLSCREEN ON/OFF",
    "TEST VIDEO",
    "QUITTER"
};

void printstring(float x, float y, void *font, string str, float r, float g, float b, float a){
    glColor3f(r,g,b);
    char * writable = new char[str.size() + 1];
    std::copy(str.begin(), str.end(), writable);
    writable[str.size()] = '\0';
    glRasterPos2f(x,y);
    for(writable; *writable != '\0'; writable++){
        glutBitmapCharacter(font,*writable);
    }
}

void display_menu(){
    char buf[100] = {0};
    sprintf(buf, "3DEDALE");
    printstring((window_w/2)-((7*15)/2),50.0,GLUT_BITMAP_TIMES_ROMAN_24,buf,1.0,1.0,1.0,1.0);
    for(int i = 0; i<NB_OPTIONS; i++){
        float r=1.0,g=1.0 ,b=1.0;
        if(current_option_selected == i){ g=0.0; b=0.0;}
        printstring(30.0, window_h-(10+((NB_OPTIONS-i)*30)), GLUT_BITMAP_HELVETICA_18,MainMenu[i],r,g,b,1.0);
        
    }
}

void option_clicked(int current_option_selected){
    switch(current_option_selected){
        case 0: //JOUER
			start_camera();
			detection();
            LoadGame(window_w,window_h);
            break;
        case 1: //fullscreen
            glutFullScreenToggle();
            break;
        case 2: //test video
            break;
        case 3: //QUITTER
            exit(0);
            break;
    }
}

void clavier_menu(unsigned char key, int x, int y){
    switch(key){
        case 13: //touche entree
            option_clicked(current_option_selected);
    }
}
void specialKey_menu(int keycode, int x, int y){
    switch(keycode){
        case GLUT_KEY_UP:
            current_option_selected--;
            break;
        case GLUT_KEY_DOWN:
            current_option_selected++;
            break;
    }
    if(current_option_selected > NB_OPTIONS-1) current_option_selected = 0;
    if(current_option_selected < 0) current_option_selected = NB_OPTIONS-1;
    glutPostRedisplay();
    
}

void souris_menu(int button, int state, int x, int y){
    cout << "souris: x=" << x << " y=" << y << endl;
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        option_clicked(current_option_selected);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        display_menu();
    } 

}

void souris_mouvement_menu(int x, int y){
    for(int i = 0; i<NB_OPTIONS; i++){
        if(y >= window_h-(10+((NB_OPTIONS-i)*30))-15 && y <= window_h-(10+((NB_OPTIONS-i)*30))){
            current_option_selected = i;
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            display_menu();
            glutSwapBuffers();
        }
    }

}

void redim_menu(int width, int height){
    cout << "redim" << endl;
    window_w = width;
    window_h = height;
    glutPostRedisplay();
}

void mainMenu(){
    glViewport(0, 0, window_w, window_h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, window_w, window_h, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    display_menu();
    //Type d'affichage, Tampon de profondeur, Double buffer
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glMatrixMode(GLUT_SINGLE | GLUT_RGB);
    glutSwapBuffers();
}

void LoadMainMenu(int window_width, int window_height){
    window_w = window_width;
    window_h = window_height;
    //mainMenu();
    glutDisplayFunc(mainMenu);
    glutReshapeFunc(redim_menu);


    glDisable(GL_LIGHTING);

    //Action Clavier
    glutKeyboardFunc(clavier_menu);
    glutSpecialFunc(specialKey_menu);
    glutMouseFunc(souris_menu);
    glutPassiveMotionFunc(souris_mouvement_menu);

    cout << window_w << "," << window_h << endl;

    //redim_menu(window_w,window_h);

    glutPostRedisplay();
}

