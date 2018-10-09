#include "mainMenu.hpp"

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

void LoadMainMenu(){
    mainMenu();
    glutReshapeFunc(redim);
    glutPostRedisplay();
}


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
    cout << "w:" << window_w << " h:" << window_h << endl;
    char buf[100] = {0};
    printstring(0.0,0.0,GLUT_BITMAP_TIMES_ROMAN_24,"X",1.0,1.0,1.0,1.0);
    printstring(window_w,0.0,GLUT_BITMAP_TIMES_ROMAN_24,"X",1.0,1.0,1.0,1.0);
    sprintf(buf, "3DEDALE");
    printstring((window_w/2)-((7*15)/2),50.0,GLUT_BITMAP_TIMES_ROMAN_24,buf,1.0,1.0,1.0,1.0);
    for(int i = 0; i<NB_OPTIONS; i++){
        float r=1.0,g=1.0 ,b=1.0;
        if(current_option_selected == i){ g=0.0; b=0.0;}
        printstring(30.0, window_h-(10+((NB_OPTIONS-i)*30)), GLUT_BITMAP_HELVETICA_18,MainMenu[i],r,g,b,1.0);
        
    }
}

void clavier(unsigned char key, int x, int y){
    switch(key){
        case 27: //touche echap
            LoadMainMenu();
            break; 
        case 13: //touche entree
            switch(current_option_selected){
                case 0: //JOUER
                    LoadGame();
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
}
void specialKey(int keycode, int x, int y){
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

void mainMenu(){
    cout << "affichage" << endl;
    display_menu();
    //Action Clavier
    glutKeyboardFunc(clavier);
    glutSpecialFunc(specialKey);

    glutSwapBuffers();
}

void redim(int width, int height){
    glViewport(0, 0, width, height);
    window_w = width;
    window_h = height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, window_w, window_h, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glutSwapBuffers();
}