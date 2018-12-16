#include "game.hpp"
#include "mainMenu.hpp"
#include "../OPENCV/camera.hpp"
#include "../CONVERT/inclinaison.hpp"

labyrinthe lab;
int window_w_game;
int window_h_game;

void clavier_game(unsigned char key, int x, int y){
    switch(key){
        case 27: //touche echap
			closeCamera();
            LoadMainMenu(window_w_game, window_h_game);
            break;
    }
}
void specialKey_game(int keycode, int x, int y){
    //code
    
}

void souris_game(int button, int state, int x, int y){
    //code
}

void souris_mouvement_game(int x, int y){
    //code
}

//BOUCLE DE JEU
void display_game(void){
	cout<< "affichage" << endl;
	
	//OPENCV
	vector<cv::Point2f> points;
    
    cv::Point2f point_modele_1(250, 100) ;     // !! Point en haut à gauche !!
    cv::Point2f point_modele_2(1030, 100) ;   // !! Point en haut à droite !!
    cv::Point2f point_modele_3(250, 620) ;    // !! Point en bas à gauche !!
    cv::Point2f point_modele_4(1030, 620) ;   // !! Point en bas à droite !!

    vector<cv::Point2f> modele ;
    modele.push_back(point_modele_1) ;
    modele.push_back(point_modele_2) ;
    modele.push_back(point_modele_3) ;
    modele.push_back(point_modele_4) ;
    
    cv::Mat rot_mat ;
    cv::Mat trans_mat ;
    cv::Vec3f euler_angles , euler_angles_param;
    cv::Vec3f euler_angles_zero;
    cv::Vec3f inclinaison_angles;
    
    
	points = tracking(getImage());
	euler_angles_zero = inclinaison(modele, modele, rot_mat, trans_mat, euler_angles_param) ;
	euler_angles = inclinaison(modele, points, rot_mat, trans_mat, euler_angles_param) ;
	
	inclinaison_angles = euler_angles_zero - euler_angles ;
	int scale_x = 6 ;
	int scale_y = 6 ;
	int scale_z = 2 ;
	
	inclinaison_angles[0] *= scale_x ;
	inclinaison_angles[1] *= scale_y ;	
	inclinaison_angles[2] *= scale_z ;
		
	cout << inclinaison_angles << "\n" ;
	rotationlabyrinthe(inclinaison_angles);
	
	
	
    // Scale to window size
    glViewport(0, 0, window_w_game, window_h_game);

    // Draw stuff
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION_MATRIX);
    glLoadIdentity();
    gluPerspective( 50, (double)800 / (double)600, 0.1, 1000 );

    glMatrixMode(GL_MODELVIEW_MATRIX);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    
    glPushMatrix();
        float lightPosition[4] = {2, 3, 10, 1};
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
        GLfloat Lambiant [4] = {0.4, 0.4, 0.4, 1.0};
        GLfloat Lblanche [4] = {1.0, 1.0, 1.0, 1.0};
        glLightfv (GL_LIGHT0, GL_AMBIENT, Lambiant);
        glLightfv (GL_LIGHT0, GL_DIFFUSE, Lblanche);
        glLightfv (GL_LIGHT0, GL_SPECULAR, Lblanche);
    glPopMatrix();
    //        eyeX,eyeY,eyeZ,centerX,centerY,centerZ,upX,upY,upZ
    gluLookAt(0.0, 0.1, 17.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);

    glRotatef(180,0.0,0.0,1.0); //on pivote l'axe pour qu'il corresponde a nos besoins
    glPushMatrix();
    creerlabyrinthe(lab);
    glPopMatrix();
    
    //on affiche
    glutSwapBuffers();
    
    glutPostRedisplay();

}

void redim_game(int window_width, int window_height){
    window_w_game = window_width;
    window_h_game = window_height;
    glViewport(0, 0, window_w_game, window_h_game);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, window_w_game, window_h_game, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glutSwapBuffers();
}

void LoadGame(int window_width, int window_height){

    //lancement des fonctions OpenCV

    start_camera();
    vector<cv::Vec4f> lines = detection();
    lab.PointDepart = cv::Point2d(1,1);
    lab.PointArrivee = cv::Point2d(10,10);
    lab.lignes = lines;
    init_tracking(getImage());
    
    //lancement des fonctions OpenGL
	
    window_w_game = window_width;
    window_h_game = window_height;
    glutDisplayFunc(display_game);
    glutReshapeFunc(redim_game);
    
    //clavier, souris, motion
    glutKeyboardFunc(clavier_game);
    glutSpecialFunc(specialKey_game);
    glutMouseFunc(souris_game);
    glutPassiveMotionFunc(souris_mouvement_game);
	
	glutPostRedisplay();
    

    
}
