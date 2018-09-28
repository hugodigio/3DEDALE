#include <iostream>
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include "./src/3DAR/2Dto3D.hpp"

/*
A FAIRE:
- un makefile qui lie tout les fichiers pour devoir executer seulement le main.cpp
- creer les murs
*/

//fonction pour la gestion des touches
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
//fonction pour la gestion de la souris
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

labyrinthe lab;

void affichage(GLFWwindow* window){
     /*Boucle jusqu'a fermuture de la fenetre*/
    while (!glfwWindowShouldClose(window))
    {
        
        // Scale to window size
        GLint windowWidth, windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        glViewport(0, 0, windowWidth, windowHeight);

        // Draw stuff
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION_MATRIX);
        glLoadIdentity();
        gluPerspective( 75, (double)windowWidth / (double)windowHeight, 0.1, 100 );
        //        eyeX,eyeY,eyeZ,centerX,centerY,centerZ,upX,upY,upZ
        gluLookAt(15.0, 5.0, 5.0, 5.0, 5.0, 0.0, 0.0, 0.0, 1.0);

        glMatrixMode(GL_MODELVIEW_MATRIX);
        glTranslatef(0,0,0);

        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glPushMatrix();
            float lightPosition[4] = {2, 3, 10, 1};
            glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
        glPopMatrix();
    
        
        def_axes();
        creerlabyrinthe(lab);

        // Update Screen
        glfwSwapBuffers(window);

        // Check for any input, or window movement
        glfwPollEvents();
        /* Poll for and process events */
        glfwPollEvents();
    }
}

int main(int argc, char const *argv[]) {
    

    test();

    lab.PointDepart  = cv::Point2f(1,1);
    lab.PointArrivee = cv::Point2f(10,10);
    //1ere ligne
    lab.x.push_back(cv::Point2f(2,3));
    lab.y.push_back(cv::Point2f(2,6));

    //2e ligne
    lab.x.push_back(cv::Point2f(3,4));
    lab.y.push_back(cv::Point2f(5,4));

    //3e ligne
    lab.x.push_back(cv::Point2f(5,7));
    lab.y.push_back(cv::Point2f(8,7));

    lab.x.push_back(cv::Point2f(0,0));
    lab.y.push_back(cv::Point2f(10,0));


    lab.x.push_back(cv::Point2f(0,0));
    lab.y.push_back(cv::Point2f(0,10));


    lab.x.push_back(cv::Point2f(10,10));
    lab.y.push_back(cv::Point2f(0,10));


    lab.x.push_back(cv::Point2f(10,10));
    lab.y.push_back(cv::Point2f(10,0));

    creerlabyrinthe(lab);

    cout << "" << lab.PointDepart << endl;

    //OpenGL
    GLFWwindow* window;

    glfwInit();
    //creer une funetre
    window = glfwCreateWindow(640, 480, "3DEDALE", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current 
    glfwMakeContextCurrent(window);
    
    affichage(window);
   
    glfwTerminate();
    return 0;
}


//fonction pour la gestion des touches
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    /*code*/
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos){
    /*code*/
}