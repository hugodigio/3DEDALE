#include <iostream>
#include <GL/freeglut.h>
#include <GL/glu.h>
#include <string>

#define NB_OPTIONS 4

using namespace std;

int window_h = 600;
int window_w = 800;

int menustart_x = 0;
int menustart_y = 0;

//variable du menu
int current_option_selected = 0;
string MainMenu[NB_OPTIONS] = {
    "JOUER",
    "FULLSCREEN ON/OFF",
    "TEST VIDEO",
    "QUITTER"
};


void init();
void affichage();
void redim(int width, int height);

int main(int arg, char **argv){
    glutInit(&arg,argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(window_w, window_h);
    glutInitWindowPosition(10,10);
    glutCreateWindow("test MENU");
    
    init();

    glutDisplayFunc(affichage);
    glutReshapeFunc(redim);

    glutMainLoop();

    return 0;
}

void init(){
    glClearColor(0.0,0.0,0.0,0.0);
    glMatrixMode(GLUT_SINGLE | GLUT_RGB);
    glLoadIdentity();
    glOrtho(0.0,window_w,window_h,0.0,0.0,1.0);
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
    char buf[100] = {0};
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
        case 13: //touche entree
            switch(current_option_selected){
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

void affichage(){
    display_menu();
    //Action Clavier
    glutKeyboardFunc(clavier);
    glutSpecialFunc(specialKey);

    glutSwapBuffers();
}

void redim(int width, int height){
    cout << "w:" << width << " h:" << height << endl;
    width = window_w;
    height = window_h;
    glLoadIdentity();
    glOrtho(-0.0,window_w,window_h,0.0,0.0,1.0);
    glutSwapBuffers();
}