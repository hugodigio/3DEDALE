#include <GLFW/glfw3.h>

void def_carre(void){
    int a = 1;
    glBegin(GL_POLYGON);
        glVertex2f(-a/2.0,a/2.0);
        glVertex2f(a/2.0,a/2.0);
        glVertex2f(a/2.0,-a/2.0);
        glVertex2f(-a/2.0 ,-a/2.0);
    glEnd();
    
}

void affichage(void){
    //on efface le buffer d'ecran
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_BACK, GL_LINE); // a enlever par la suite
        
}

//fonction pour la gestion des touches
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    /*code*/
}

//fonction pour la gestion de la souris
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    /*code*/
}

int main(int argc, char const *argv[])
{
    GLFWwindow* window;

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

        affichage();

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
