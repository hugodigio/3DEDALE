/*
La fonction fond() est pensée pour une caméra se déplacant sur le plan xz du coté des y négatifs et un background
suivant ses déplacements sur le plan xz mais du coté des y positifs.
*/

#include <iostream>
#include <string>
#include <opencv2/highgui.hpp>
#include <stdio.h>
#include <GLUT/glut.h>

#include "background.hpp"

using namespace cv;
using namespace std;

// Crée et texture le background en fonction de l'image et des parametres de la projection et de la position de la camera
void fond(Mat image, Vec3f position, float far, float fov)
{
	TextureImage texture;
	chargeTextureMat(&texture, image);

	float longueur=2*far*tan(fov*(M_PI/180)/2);
	
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_TEXTURE_2D);
    dessiner_background((float)image.cols/image.rows, longueur, position.val[0], position.val[1]+far-0.01, position.val[2], 90, 0, -90);
    glDisable(GL_TEXTURE_2D);
    
}

//Dessine le rectangle du background a la taille, position et orientation demandées
void dessiner_background(float rapport, float a, float x, float y, float z, float r, float t, float l)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glTranslatef(x, y, z);
		glRotatef(r, 1.0, 0.0, 0.0);
		glRotatef(t, 0.0, 1.0, 0.0);
		glRotatef(l, 0.0, 0.0, 1.0);
		glScaled(a, a, a);
        def_rectangle(rapport);
	glPopMatrix();
}

//Dessine un rectangle de taille unitaire
void def_rectangle(float rapport)
{
    glBegin(GL_POLYGON);
    
    glNormal3f(0, 0, 1);
    glTexCoord2f(1.0f, 1.0f);glVertex2f(0.5, 0.5*rapport); //bas droite
    glTexCoord2f(1.0f, 0.0f);glVertex2f(-0.5, 0.5*rapport);
    glTexCoord2f(0.0f, 0.0f);glVertex2f(-0.5, -0.5*rapport);
    glTexCoord2f(0.0f, 1.0f);glVertex2f(0.5, -0.5*rapport);
    
    glEnd();
}

//Charge une texture en fonction d'une Mat
int chargeTextureMat(Mat image)
{
    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    
    
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGB,
                 image.cols,
                 image.rows,
                 0,
                 GL_BGR,
                 GL_UNSIGNED_BYTE,
                 image.ptr());
    
    glGenerateMipmap(GL_TEXTURE_2D);
}
