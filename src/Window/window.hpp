#ifndef WINDOW_H
#define WINDOW_H

#include <GL/freeglut.h>
#include "../3DAR/2Dto3D.hpp"

void createWindow(int argc, char **argv);
void affichage(void);
void redim(int width, int height);
void clavier(unsigned char key, int x, int y);



#endif