#include "window.hpp"

void createWindow(int argc, char **argv){
    //Initialisation de GLUT
    glutInit(&argc, argv);
    
    //Taille et emplacemet de la fenetre
    glutInitWindowSize(800,600);
    glutInitWindowPosition(800,100);
    
    //Type d'affichage, Tampon de profondeur, Double buffer
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);

    glMatrixMode(GLUT_SINGLE | GLUT_RGB);
    
    //Creation de la premiere fenetre
    glutCreateWindow("3DEDALE");

    //Association des callback
    LoadMainMenu(800, 600);
    
    glutMainLoop();
}
