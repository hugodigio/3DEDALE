//
//  TestWorld.cpp
//  
//
//  Created by Alzahra Hassanein on 12/12/2018.
//

#include "TestWorld.hpp"
#include <GLUT/glut.h>

#include <cmath>

///-----includes_start-----
#include "./lib/bullet3/src/btBulletDynamicsCommon.h"
#include <stdio.h>

using namespace std;
///-----includes_end-----

int i;
///-----initialization_start-----

///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();

///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();

///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

///-----initialization_end-----

//keep track of the shapes, we release memory at exit.
//make sure to re-use collision shapes among rigid bodies whenever possible!
btAlignedObjectArray<btCollisionShape*> collisionShapes;


///
float angleX = 0,angleY = 0,angleZ; //angles de rotations
float DtoR = M_PI/180; // constante pour transformer les degres en radiant


float x = (angleX*DtoR); //angle de test pour pencher
float y = angleY*DtoR;

float HAUTEUR_MUR = 1.;
float EPAISSEUR_MUR = 0.1;

btVector3 localInertia(0, 0, 0);
btScalar mass(0.);

///Les rigidbodys et leurs transfo:
btRigidBody* Sphere = NULL;
btTransform tSphere;

btRigidBody* Sol = NULL;
btRigidBody* MurExtG = NULL;
btRigidBody* MurExtD = NULL;
btRigidBody* MurExtArr = NULL;
btRigidBody* MurExtAv = NULL;
btTransform tSol;
btTransform tMurExtG;
btTransform tMurExtD;
btTransform tMurExtArr;
btTransform tMurExtAv;

btRigidBody* Mur1 = NULL;
btRigidBody* Mur2 = NULL;
btRigidBody* Mur3 = NULL;
btRigidBody* Mur4 = NULL;
btRigidBody* Mur5 = NULL;
btRigidBody* Mur6 = NULL;
btRigidBody* Mur7 = NULL;
btRigidBody* Mur8 = NULL;
btRigidBody* Mur9 = NULL;
btTransform tMur1;
btTransform tMur2;
btTransform tMur3;
btTransform tMur4;
btTransform tMur5;
btTransform tMur6;
btTransform tMur7;
btTransform tMur8;
btTransform tMur9;

void display();
void red(int a,int b);
// Initialise la fenetre GLUT

void init_fenetre(int argc, char** argv){
    
    glutInit(&argc, argv);
    //Taille et emplacemet de la fenetre
    glutInitWindowSize(700,800);
    glutInitWindowPosition(200,100);
    
    //Type d'affichage, Tampon de profondeur, Double buffer
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    
    //Creation de la premiere fenetre
    glutCreateWindow("3Dedale");
    
    glutDisplayFunc(display);
    glutReshapeFunc(red);
    
    //Action Clavier
    //glutKeyboardFunc(clavier);

    glutMainLoop();
}

//Fonction de Redimensionnement
void red(int width, int height){
    glEnable(GL_DEPTH_TEST);
    glViewport(0,0,width, height);
    
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    gluPerspective(75,(float)width/height, 1.0,100.0);
    
    //         eyeX,eyeY,eyeZ,centerX,centerY,centerZ,upX,upY,upZ
    gluLookAt    (0.0, 7.0, 19.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}


/////////////////////////////////////////////////////////////////

//Fonction de construction de la sphere
void renderSphere(btRigidBody* sphere)
{
   
    glColor3f(1,0,0);
    //float r=((btSphereShape*)sphere->getCollisionShape())->getRadius();
    btTransform t;
    float mat[16];
    
    sphere->getMotionState()->getWorldTransform(t);    //get the transform
    
    t.getOpenGLMatrix(mat);    //OpenGL matrix stores the rotation and orientation
    glPushMatrix();
        glMultMatrixf(mat);    //multiplying the current matrix with it moves the object in place
        glutSolidSphere(0.3,36,36);
    glPopMatrix();
}

void BoxSphere(float x){
    
}

void BoxWall (float x){
    
}


int once = 0;
//Fonction de construction des plans (murs)
void renderPlane(btRigidBody* plane, int type)
{

    //
    once += 1;
    
    glColor3f(0.5,0,0.8);
    btTransform t;
    float mat[16];
    btQuaternion qa, qx,qy,qz, res;
    
    //cout << "ADIEU : " << endl << (plane->getLinearVelocity()).getX() <<endl<< (plane->getLinearVelocity()).getY() << endl << (plane->getLinearVelocity()).getZ() << endl;
    //plane->forceActivationState(DISABLE_DEACTIVATION);
   
    if (once == 5){
        plane->getMotionState()->getWorldTransform(t);
        qa = t.getRotation();
        
        //angle*DtoR
        //axeX,axeY,axeZ
        
        qx = btQuaternion (btVector3(1,0,0) , btScalar(10*DtoR));
        qy = btQuaternion (btVector3(0,1,0) , btScalar(0*DtoR));
        qz = btQuaternion (btVector3(0,0,1) , btScalar(00*DtoR));
        res = qa*qx*qy*qz;
        t.setRotation(res);
        //r = btQuaternion(btVector3(0,0,1) , btScalar(y));
        //d = r*q;
        //t.setRotation(d);
 
        plane->getMotionState()->setWorldTransform(t);
    }
    plane->getMotionState()->getWorldTransform(t);
    t.getOpenGLMatrix(mat);

    
    glPushMatrix();
        glMultMatrixf(mat);    //translation,rotation
    
        if (type == 1){ //sol
            glColor3f(0.1,0.5,0.8);
            glBegin(GL_QUADS);
                glVertex3f(-5,0,5);
                glVertex3f(-5,0,-5);
                glVertex3f(5,0,-5);
                glVertex3f(5,0,5);
            glEnd();
        }else{ //murs
            glBegin(GL_QUADS);
                glVertex3f(-1,0,5);
                glVertex3f(-1,0,-5);
                glVertex3f(1,0,-5);
                glVertex3f(1,0,5);
            glEnd();
        }
    glPopMatrix();
}


/*
 Fonctions d'affichage des objets en prenant en compte leur box de collision
 */

void display(void){
    
    /*int width,height;
    width = glutGet(GLUT_WINDOW_WIDTH);
    height = glutGet(GLUT_WINDOW_HEIGHT);*/
    
    //on efface le buffer d'ecran
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glPushMatrix();
    
    ///-----stepsimulation_start-----
    for (i = 0; i < 2; i++)
    {
        dynamicsWorld->stepSimulation(1.f / 60.f, 10);
        
        //print positions of all objects
        for (int j = dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--) //sphere,sol,mur
        {
            btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
            btRigidBody* body = btRigidBody::upcast(obj);

            if(body->getCollisionShape()->getShapeType()!=SPHERE_SHAPE_PROXYTYPE){
                if(j > 1)//que ce n'est pas le sol
                   renderPlane(body,2);
                else
                   renderPlane(body,1);
            }
            else if(body->getCollisionShape()->getShapeType()==SPHERE_SHAPE_PROXYTYPE)
                renderSphere(body);
        }
       
        
    }
    glPopMatrix();
    //on affiche
    
    glutSwapBuffers();
    glutPostRedisplay();
    ///-----stepsimulation_end-----
    
}





/*
 Initialise la gravite, la box de collision de :
  - la boule
  - des murs externes
 */
void init_bull(btVector3 angle){ //ici passer le Vec3f
    
    dynamicsWorld->setGravity(btVector3(0, -10, 0));
    ///create a few basic rigid bodies
    
    /*Vec3f to btVector3
     btVector3 angles;
     angles.setX(btScalar(angle.x));
     angles.setY(btScalar(angle.y));
     angles.setZ(btScalar(angle.z));
     */
   
    
    //Sphere
    //BoxSphere(angle);
    //BoxWall(angle);
    
    {
        //create a dynamic rigidbody
        
        btCollisionShape* colShape = new btSphereShape(btScalar(0.2));
        collisionShapes.push_back(colShape);
        
        /// Create Dynamic Objects
        btTransform startTransform;
        startTransform.setIdentity();
        
        btScalar massB(1.f);
        
        //rigidbody is dynamic if and only if mass is non zero, otherwise static
        bool isDynamic = (massB != 0.f);
        
        
        if (isDynamic)
            colShape->calculateLocalInertia(massB, localInertia);
        
        
        //startPoint
        startTransform.setOrigin(btVector3(2, 2, -2));
        
        
        
        //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
        btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(massB, myMotionState, colShape, localInertia);
        //btRigidBody* body = new btRigidBody(rbInfo);
        //dynamicsWorld->addRigidBody(body);
        Sphere = new btRigidBody(rbInfo);
        dynamicsWorld->addRigidBody(Sphere);
        
    }
    
    
    //Sol
    {
        
        //Taille du sol,  largeur
        btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(5.), btScalar(EPAISSEUR_MUR), btScalar(5.)));
        
        collisionShapes.push_back(groundShape);
        
        //btTransform groundTransform;
        tSol.setIdentity();
        tSol.setOrigin(btVector3(0, 0, 0));
        //Test
        
        /*btQuaternion qx,qy,qz, res;
        qx = btQuaternion (btVector3(1,0,0) , btScalar(0*DtoR));
        qy = btQuaternion (btVector3(0,1,0) , btScalar(0*DtoR));
        qz = btQuaternion (btVector3(0,0,1) , btScalar(0*DtoR));
        res = qx*qy*qz;
        groundTransform.setRotation(res);
        */
        
        btQuaternion qx,qy,qz, res;
        qx = btQuaternion (btVector3(1,0,0) , btScalar((angle.getX())*DtoR));
        qy = btQuaternion (btVector3(0,1,0) , btScalar(angle.getY()*DtoR));
        qz = btQuaternion (btVector3(0,0,1) , btScalar((angle.getZ())*DtoR));
        res = qx*qy*qz;
        tSol.setRotation(res);

        
        //rigidbody is dynamic if and only if mass is non zero, otherwise static
        bool isDynamic = (mass != 0.f);
        
       
        if (isDynamic)
            groundShape->calculateLocalInertia(mass, localInertia);
        
        //using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
        btDefaultMotionState* myMotionState = new btDefaultMotionState(tSol);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
        //btRigidBody* body = new btRigidBody(rbInfo);
        //dynamicsWorld->addRigidBody(body);
        
        Sol = new btRigidBody(rbInfo);
        dynamicsWorld->addRigidBody(Sol);
    }
    

    
    
    //mur verticaux : epaisseur x 0.2, hauteur y 2, longueur z (10)
    //mur  ->setOrigin(positionCentrex, 0, positionCentrez)
    //mur horizontaux : epaisseur z 0.2, hauteur y 2, longueur x 10
    //mur ->setOrigin(positionCentrex, 0, positionCentrez)
    
    //Mur Gauche
    {
        btCollisionShape* wallShape1 = new btBoxShape(btVector3(btScalar(0.5), btScalar(EPAISSEUR_MUR), btScalar(5.)));
        
        collisionShapes.push_back(wallShape1);
        
        btTransform wallTransform;
        wallTransform.setIdentity();
        wallTransform.setOrigin(btVector3(-5, 1, 0));
        
        btQuaternion qx,qy,qz, res;
        qx = btQuaternion (btVector3(1,0,0) , btScalar(angle.getX()*DtoR));
        qy = btQuaternion (btVector3(0,1,0) , btScalar(angle.getY()*DtoR));
        qz = btQuaternion (btVector3(0,0,1) , btScalar((90+angle.getZ())*DtoR));
        res = qx*qy*qz;
        wallTransform.setRotation(res);
        

        //rigidbody is dynamic if and only if mass is non zero, otherwise static
        bool isDynamic = (mass != 0.f);
    
        if (isDynamic)
            wallShape1->calculateLocalInertia(mass, localInertia);
        
        //using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
        btDefaultMotionState* myMotionState = new btDefaultMotionState(wallTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, wallShape1, localInertia);
        btRigidBody* body = new btRigidBody(rbInfo);
        
        //add the body to the dynamics world
        dynamicsWorld->addRigidBody(body);
    }
    
    
    //Mur droit
    {
        btCollisionShape* wallShape2 = new btBoxShape(btVector3(btScalar(0.5), btScalar(EPAISSEUR_MUR), btScalar(5.)));
        
        collisionShapes.push_back(wallShape2);
        
        btTransform wallTransform;
        wallTransform.setIdentity();
        wallTransform.setOrigin(btVector3(5, 1, 0));
        
        //Rotation 90D
        btQuaternion qx,qy,qz, res;
        qx = btQuaternion (btVector3(1,0,0) , btScalar(angle.getX()*DtoR));
        qy = btQuaternion (btVector3(0,1,0) , btScalar(angle.getY()*DtoR));
        qz = btQuaternion (btVector3(0,0,1) , btScalar((90+angle.getZ())*DtoR));
        res = qx*qy*qz;
        wallTransform.setRotation(res);
        
        btScalar mass(0.);
        
        //rigidbody is dynamic if and only if mass is non zero, otherwise static
        bool isDynamic = (mass != 0.f);
        
        btVector3 localInertia(0, 0, 0);
        if (isDynamic)
            wallShape2->calculateLocalInertia(mass, localInertia);
        
        //using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
        btDefaultMotionState* myMotionState = new btDefaultMotionState(wallTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, wallShape2, localInertia);
        btRigidBody* body = new btRigidBody(rbInfo);
        
        //add the body to the dynamics world
        dynamicsWorld->addRigidBody(body);
    }
    
    //mur arriere
    {
        btCollisionShape* wallShape3 = new btBoxShape(btVector3(btScalar(5.), btScalar(EPAISSEUR_MUR), btScalar(0.5)));
        
        collisionShapes.push_back(wallShape3);
        
        btTransform wallTransform, wallTransformBis;
        wallTransform.setIdentity();
        wallTransformBis.setIdentity();
        
        wallTransform.setOrigin(btVector3(0, 1, -5));
        
        btQuaternion qx,qy,qz, res;
        qx = btQuaternion (btVector3(1,0,0) , btScalar(angle.getX()*DtoR));
        qy = btQuaternion (btVector3(0,1,0) , btScalar((90+angle.getY())*DtoR));
        qz = btQuaternion (btVector3(0,0,1) , btScalar((90+angle.getZ())*DtoR));
        res = qx*qy*qz;
        wallTransform.setRotation(res);
    
        /* ou
        wallTransformBis.setRotation(btQuaternion (btVector3(0,0,1) , btScalar(1.5708 +x) ));
        
        wallTransform.setRotation(btQuaternion (btVector3(0,1,0) , btScalar(1.5708 +x)));
        wallTransform *= wallTransformBis;
        */
        btScalar mass(0.);
        
        //rigidbody is dynamic if and only if mass is non zero, otherwise static
        bool isDynamic = (mass != 0.f);
        
        btVector3 localInertia(0, 0, 0);
        if (isDynamic)
            wallShape3->calculateLocalInertia(mass, localInertia);
        
        //using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
        btDefaultMotionState* myMotionState = new btDefaultMotionState(wallTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, wallShape3, localInertia);
        btRigidBody* body = new btRigidBody(rbInfo);
        
        //add the body to the dynamics world
        dynamicsWorld->addRigidBody(body);
    }
    
    //mur avant
    {
        btCollisionShape* wallShape4 = new btBoxShape(btVector3(btScalar(5.), btScalar(EPAISSEUR_MUR), btScalar(0.5)));
        
        collisionShapes.push_back(wallShape4);
        
        btTransform wallTransform, wallTransformBis;
        wallTransform.setIdentity();
        wallTransformBis.setIdentity();
        wallTransform.setOrigin(btVector3(0, 1, 5));
        
        btQuaternion qx,qy,qz, res;
        qx = btQuaternion (btVector3(1,0,0) , btScalar(angle.getX()*DtoR));
        qy = btQuaternion (btVector3(0,1,0) , btScalar((90+angle.getY())*DtoR));
        qz = btQuaternion (btVector3(0,0,1) , btScalar((90+angle.getZ())*DtoR));
        res = qx*qy*qz;
        wallTransform.setRotation(res);
        /*
        wallTransformBis.setRotation(btQuaternion (btVector3(0,0,1) , btScalar((90*DtoR) +x) ));
        wallTransform.setRotation(btQuaternion (btVector3(0,1,0) , btScalar((90*DtoR) + x) ));
        wallTransform *= wallTransformBis;
        */
        btScalar mass(0.);
        
        //rigidbody is dynamic if and only if mass is non zero, otherwise static
        bool isDynamic = (mass != 0.f);
        
        btVector3 localInertia(0, 0, 0);
        if (isDynamic)
            wallShape4->calculateLocalInertia(mass, localInertia);
        
        //using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
        btDefaultMotionState* myMotionState = new btDefaultMotionState(wallTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, wallShape4, localInertia);
        btRigidBody* body = new btRigidBody(rbInfo);
        
        //add the body to the dynamics world
        dynamicsWorld->addRigidBody(body);
    }
    
}


int test(int argc, char** argv)
{
    
    
    init_bull(btVector3(0,0,0));
    init_fenetre(argc,argv);
    
    
    //free_mem();
    
    return 0;
}

/*
 
 void free_mem(){
 
 //cleanup in the reverse order of creation/initialization
 
 ///-----cleanup_start-----
 
 //remove the rigidbodies from the dynamics world and delete them
 for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
 {
 btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
 btRigidBody* body = btRigidBody::upcast(obj);
 if (body && body->getMotionState())
 {
 delete body->getMotionState();
 }
 dynamicsWorld->removeCollisionObject(obj);
 delete obj;
 }
 
 
 //delete collision shapes
 for (int j = 0; j < collisionShapes.size(); j++)
 {
 btCollisionShape* shape = collisionShapes[j];
 collisionShapes[j] = 0;
 delete shape;
 }
 
 //delete dynamics world
 delete dynamicsWorld;
 
 //delete solver
 delete solver;
 
 //delete broadphase
 delete overlappingPairCache;
 
 //delete dispatcher
 delete dispatcher;
 
 delete collisionConfiguration;
 
 //next line is optional: it will be cleared by the destructor when the array goes out of scope
 collisionShapes.clear();
 
 }*/
