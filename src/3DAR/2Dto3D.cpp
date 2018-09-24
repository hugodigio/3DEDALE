#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include "2Dto3D.hpp"

using namespace std;

void creerlabyrinthe(labyrinthe x){
    cout << "nombre de murs: " << x.x.size() << endl;
}

void test(){
    cout << "test OK" << endl;
}


/* ----------------------------------------------------------
// PLUS TARD DANS LE MAIN  QUAND ON AURA REUSSI A LINK SA MER
------------------------------------------------------------*/

void def_axes(){

    glBegin(GL_LINES);
        //x
        glColor3f(1.0, 0.0, 0.0);
        glVertex3f(-1,0.0,0.0);
        glVertex3f(1,0.0,0.0);
        //y
        glColor3f(0.0, 1.0, 0.0);
        glVertex3f(0.0,-1,0.0);
        glVertex3f(0.0,1,0.0);
        //z
        glColor3f(0.0, 0.0, 1.0);
        glVertex3f(0.0,0.0,-1);
        glVertex3f(0.0,0.0,1);
    glEnd();

}

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
        gluPerspective( 60, (double)windowWidth / (double)windowHeight, 0.1, 100 );

        glMatrixMode(GL_MODELVIEW_MATRIX);
        glTranslatef(0,0,-5);

        def_axes();

        // Update Screen
        glfwSwapBuffers(window);

        // Check for any input, or window movement
        glfwPollEvents();
        /* Poll for and process events */
        glfwPollEvents();
    }
}

int main(int argc, char const *argv[]) {
    labyrinthe lab;

    test();

    lab.PointDepart  = cv::Point2f(1,1);
    lab.PointArrivee = cv::Point2f(10,10);
    //1ere ligne
    lab.x.push_back(cv::Point2f(2,3));
    lab.y.push_back(cv::Point2f(2,6));

    //2e ligne
    lab.x.push_back(cv::Point2f(3,4));
    lab.y.push_back(cv::Point2f(5,4));

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
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    affichage(window);
   
    glfwTerminate();
    return 0;
}

