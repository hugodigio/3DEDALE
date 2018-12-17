#include <iostream>

/***/
#include <GLUT/glut.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <vector>
/***/

//
//#include "TestWorldNew.cpp"
//#include "TestWorld.cpp"
#include "TestWorldAncien.cpp"
//#include "TestVidYout.cpp"


/**/
#include "./lib/bullet3/src/btBulletDynamicsCommon.h"
#include "./lib/bullet3/src/btBulletCollisionCommon.h"

using namespace std;
/**/
// DONNEES POUR BULLETPHYSICS

//declaration du nom du monde physique
btDiscreteDynamicsWorld *myWorld;

//interface de detection des objets AABB
btBroadphaseInterface *myBroadphase;

//btCollisionDispatcher supporte des algorithmes qui peuvent gérer des pairs de collisions ConvexConvex et ConvexConcave
btCollisionDispatcher *myDispatcher;

//btCollisionConfiguration permet de configurer les allocataires de mémoire.
btDefaultCollisionConfiguration *myCollisionConfiguration;

//btSequentialImpulseConstraintSolver est une implémentation SIMD rapide de la méthode Projected Gauss Seidel (iterative LCP)
btSequentialImpulseConstraintSolver *mySequentialImpulseConstraintSolver;

//Position, Rotation
btTransform myTransform;

//btDefaultMotionState fournit une implémentation pour synchroniser les transformations
btDefaultMotionState *myMotionState,*myMotionState_Sol;

//Une matrice OpenGL, pour récupérer la position, rotation d'un objet
btScalar matrix[16];

//Corps d'une boite
btRigidBody *body,*body_sol;


void myBox(float x, float y, float z) //
{
    glPushMatrix();
    glScalef(x,y,z);
    glBegin (GL_QUADS);
    //face 1
    glNormal3i(-1, 1,-1);
    glVertex3i(-1, 1,-1); glVertex3i( 1, 1,-1);
    glVertex3i( 1,-1,-1); glVertex3i(-1,-1,-1);
    //face 2
    glNormal3i(-1,-1,-1);
    glVertex3i(-1,-1,-1); glVertex3i( 1,-1,-1);
    glVertex3i( 1,-1, 1); glVertex3i(-1,-1, 1);
    // face 3
    glNormal3i( 1,-1, 1);
    glVertex3i( 1,-1, 1); glVertex3i( 1,-1,-1);
    glVertex3i( 1, 1,-1); glVertex3i( 1, 1, 1);
    //face 4
    glNormal3i( 1, 1,-1);
    glVertex3i( 1, 1,-1); glVertex3i(-1, 1,-1);
    glVertex3i(-1, 1, 1); glVertex3i( 1, 1, 1);
    //face 5
    glNormal3i(-1, 1, 1);
    glVertex3i(-1, 1, 1); glVertex3i(-1, 1,-1);
    glVertex3i(-1,-1,-1); glVertex3i(-1,-1, 1);
    //face 6
    glNormal3i( 1,-1, 1);
    glVertex3i( 1,-1, 1); glVertex3i( 1, 1, 1);
    glVertex3i(-1, 1, 1); glVertex3i(-1,-1, 1);
    glEnd();
    glPopMatrix();
}


void init_bullet(){
    
    /// Bullet physics
    
    ///collision configuration contains default setup for memory, collision setup
    myCollisionConfiguration = new btDefaultCollisionConfiguration();
    
    ///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
    myDispatcher = new    btCollisionDispatcher(myCollisionConfiguration);
    
    myBroadphase = new btDbvtBroadphase();
    
    ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    mySequentialImpulseConstraintSolver = new btSequentialImpulseConstraintSolver;
    
    // initialisation du monde bullet
    myWorld = new btDiscreteDynamicsWorld(myDispatcher,myBroadphase,mySequentialImpulseConstraintSolver,myCollisionConfiguration);
    
    // On définit la gravité, de façon à ce que les objets tombent vers le bas (-Y).
    myWorld->setGravity( btVector3(0,-10,0) );
    
    /// box ///////////////////////////////////////////
    // create a shape
    btCollisionShape* shape = new btBoxShape( btVector3(1,1,1) );
    
    myTransform.setIdentity();
    myTransform.setOrigin( btVector3(10,5,0) );
    
    btVector3 localInertia(0,0,0);
    
    btScalar mass = .5f;
    
    shape->calculateLocalInertia( mass, localInertia );
    
    //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
    myMotionState = new btDefaultMotionState(myTransform);
    btRigidBody::btRigidBodyConstructionInfo myBoxRigidBodyConstructionInfo( mass, myMotionState, shape, localInertia );
    body = new btRigidBody(myBoxRigidBodyConstructionInfo);
    
    //add the body to the dynamics world
    myWorld->addRigidBody(body);
    
    /// SOL ///////////////////////////////////////////
    // create a shape
    btCollisionShape* shape_sol = new btBoxShape( btVector3(10,1,10) );
    
    myTransform.setIdentity();
    myTransform.setOrigin( btVector3(0,0,0) );
    
    btVector3 localInertiaSol(0,0,0);
    
    mass = 0;
    
    // Using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
    myMotionState_Sol = new btDefaultMotionState(myTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo_sol( mass, myMotionState_Sol, shape_sol, localInertiaSol );
    body_sol = new btRigidBody(rbInfo_sol);
    
    // Add the body to the dynamics world
    myWorld->addRigidBody(body_sol);
    
}

/***/

void affichage(void)
{

    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Update dynamics
    if (myWorld)
    {
        myWorld->stepSimulation( 0.0001 );
    }
    
    // Semblant de caméra
    glTranslatef( 0,0,-20);
    glRotatef(40,1,0,0);
    
    // On recupère la matrice OpenGL transformée par Bullet qu'on appliquera à notre boite
    myMotionState->m_graphicsWorldTrans.getOpenGLMatrix( matrix );
    
    // On affiche notre boite avec les transformations applqiuées grâce à la matrice
    glPushMatrix();
    glMultMatrixf( matrix );
    myBox(1,1,1);
    glPopMatrix();
    
    // On affiche le sol;
    myBox(10,1,10);
    
    
    
    //on affiche
    glutSwapBuffers();
}


void redim(int width, int height)
{
    
    // On active la lumière 0
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    
    // Quelques autres options OpenGL
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR);
    
    // Couleur de fond d'écran
    //glClearColor(0.7,0.7,0.7,0);
    
    // l'espace d'affichage
    glViewport( 0, 0, 800, 500 );
    
    
    // Mode perspective
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective( 45.0f, 800.0/500.0, 0.1f, 1000.0f );
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    
    //         eyeX,eyeY,eyeZ,centerX,centerY,centerZ,upX,upY,upZ
    //gluLookAt (1.0, -2.5, -0.5, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    gluLookAt    (0.0, 0.0, 9.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}



void init_glut(){
    //Taille et emplacemet de la fenetre
    glutInitWindowSize(700,800);
    glutInitWindowPosition(200,100);
    
    //Type d'affichage, Tampon de profondeur, Double buffer
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    
    //Creation de la premiere fenetre
    glutCreateWindow("3Dedale");
   
    glutDisplayFunc(affichage);
    glutReshapeFunc(redim);
    
    //Action Clavier
    //glutKeyboardFunc(clavier);
    //glutSpecialFunc(clavierBoule);
    
    glutMainLoop();
}


int main(int argc, char **argv) {
    
    /*
    init_bullet();
  
    glutInit(&argc, argv);
    init_glut();
    

   */
    
    test(argc,argv);
    //testYout(argc,argv);
    

    return 0;
}


