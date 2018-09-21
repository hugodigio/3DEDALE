#include <GLFW/glfw3.h>
#include "2Dto3D.hpp"

using namespace std;

void creerlabyrinthe(labyrinthe x){
    cout << "nombre de murs: " << x.x.size() << endl;
}

int main(int argc, char const *argv[]) {
    labyrinthe test;

    test.PointDepart  = cv::Point2f(1,1);
    test.PointArrivee = cv::Point2f(10,10);
    //1ere ligne
    test.x.push_back(cv::Point2f(2,3));
    test.y.push_back(cv::Point2f(2,6));

    //2e ligne
    test.x.push_back(cv::Point2f(3,4));
    test.y.push_back(cv::Point2f(5,4));


    cout << "" << test.PointDepart << endl;

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

    /*Boucle jusqu'a fermuture de la fenetre*/
    while (!glfwWindowShouldClose(window))
    {

        creerlabyrinthe(test);

        /* Poll for and process events */
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

