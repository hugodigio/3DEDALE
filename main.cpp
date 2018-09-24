#include <iostream>
#include <GLFW/glfw3.h>
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

int main(int argc, char const *argv[])
{
    GLFWwindow* window;

    test();

    glfwInit();
    //creer une funetre
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /*Boucle jusqu'a fermuture de la fenetre*/
    while (!glfwWindowShouldClose(window))
    {

        //affichage();

        //input clavier
        glfwSetKeyCallback(window, key_callback);
        //position souris
        glfwSetCursorPosCallback(window, cursor_position_callback);

        /* Poll for and process events */
        glfwPollEvents();
    }
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