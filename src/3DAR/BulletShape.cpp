//
//  TestWorld.cpp
//
//
//  Created by Alzahra Hassanein on 12/12/2018.
//


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


float DtoR = M_PI/180; // constante pour transformer les degres en radiant
///Les rigidbodys et leurs transfo:
btRigidBody* Sphere = NULL;
btTransform tSphere;
float angleX = 0,angleY=0,angleZ=0; //angles de rotations

btCompoundShape * CShape = new btCompoundShape();



void clavier(unsigned char key, int x, int y){
    switch (key){
        case 'q':
        case 'Q':
                angleY -=0.1;
            break;
        case 'd':
        case 'D':
                angleY+=0.1;
            break;
        case 's':
        case 'S':
                angleX -= 0.1;
            break;
        case 'z':
        case 'Z':
                angleX+= 0.1;
            break;
        case 'a':
        case'A' :
            exit (0);
            break;
        default:
            break;
    }glutPostRedisplay();
}

//Fonction de Redimensionnement
void red(int width, int height){
    glEnable(GL_DEPTH_TEST);
    glViewport(0,0,width, height);
    
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    //          left,right,bot,top,nearVal,farVal
    //glFrustum (-1.0, 1.0, -1.0, 1.0, 1.0, 20.0);
    //ou
    gluPerspective(75,(float)width/height, 1.0,100.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //         eyeX,eyeY,eyeZ,centerX,centerY,centerZ,upX,upY,upZ
    //gluLookAt (1.0, -2.5, -0.5, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    gluLookAt    (0.0, 0.0, 17.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}



void renderSphere(btRigidBody* sphere){
    
    float mat[16];
    btTransform t;
    sphere->getMotionState()->getWorldTransform(t);
    t.getOpenGLMatrix(mat);
    glColor3f(1,0,0);
    glPushMatrix();
    glMultMatrixf(mat);
    //gluSphere(quad,r,20,20);
    glutSolidSphere(0.3,36,36);
    glPopMatrix();
    
    glutPostRedisplay();
}

/*
 Fonctions d'affichage des objets en prenant en compte leur box de collision
 */


void display(void){
    
    //on efface le buffer d'ecran
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glPushMatrix();
    
    /// Do some simulation
    btQuaternion qx,qy,qz,qres;
    qx = btQuaternion(1,0,0,btScalar(angleX*DtoR));
    qy = btQuaternion(0,1,0,btScalar(angleY*DtoR));
    qz = btQuaternion(0,0,1,btScalar(0 *DtoR));
    
    qres = qx*qy*qz;
    
    
    ///-----stepsimulation_start-----
    dynamicsWorld->stepSimulation(1.f / 60.f, 10);
        
    //print positions of all objects
    for (int j = dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--){ //sphere,sol,mur
    
        btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
        btRigidBody* body = btRigidBody::upcast(obj);
        btTransform trans;
        float mat[16];
        
        if (body && body->getMotionState()){
            body->getMotionState()->getWorldTransform(trans);
        }else{
            trans = obj->getWorldTransform();
        }
        
        trans.getOpenGLMatrix(mat);
        
        if (j ==0){
            btQuaternion tmp(trans.getRotation());
            trans.setRotation(qres*tmp);
            body->getMotionState()->setWorldTransform(trans);
        }
        if(j==1){ //sphere
            renderSphere(body);
            
            
        }/*else if((j+1)==1){ //sol
            
        
          
            glPushMatrix();
                glMultMatrixf(mat);    //translation,rotation
                //renderPlane(body,1);
                glColor3f(0.1,0.5,0.8);
                glBegin(GL_QUADS);
                    glVertex3f(-5,3.35,0);
                    glVertex3f(-5,-3.35,0);
                    glVertex3f(5,-3.35,0);
                    glVertex3f(5,3.35,0);
                glEnd();
            glPopMatrix();
            
        }else if ((j+1)==2 || (j+1)==3){ //murs verticaux
           
          
       
            glPushMatrix();
                glMultMatrixf(mat);
                glColor3f(0.5,0,0.8);
                glBegin(GL_QUADS);
                    glVertex3f(-2,3.35,0);
                    glVertex3f(-2,-3.35,0);
                    glVertex3f(2,-3.35,0);
                    glVertex3f(2,3.35,0);
                glEnd();
            glPopMatrix();
            
        }else if((j+1)==4|| (j+1)==5){ //murs horizontaux
            
         
            glPushMatrix();
                glMultMatrixf(mat);
                glColor3f(0.5,0,0.8);
                glBegin(GL_QUADS);
                    glVertex3f(-2,5,0);
                    glVertex3f(-2,-5,0);
                    glVertex3f(2,-5,0);
                    glVertex3f(2,5,0);
                glEnd();
            glPopMatrix();
        }else if((j+1)==6){
            
            
       
            glPushMatrix();
            glMultMatrixf(mat);
            glColor3f(0.5,0.5,0);
            glBegin(GL_QUADS);
                glVertex3f(-2,2.06,0);
                glVertex3f(-2,-2.06,0);
                glVertex3f(2,-2.06,0);
                glVertex3f(2,2.06,0);
            glEnd();
            glPopMatrix();
        }else if((j+1)==7){
           
            
        
            glPushMatrix();
            glMultMatrixf(mat);
            glColor3f(0.5,0.5,0);
            glBegin(GL_QUADS);
                glVertex3f(-2,1.25,0);
                glVertex3f(-2,-1.25,0);
                glVertex3f(2,-1.25,0);
                glVertex3f(2,1.25,0);
            glEnd();
            glPopMatrix();
        }else if((j+1)==8){
            
          
            glPushMatrix();
            glMultMatrixf(mat);
            glColor3f(0.5,0.5,0);
            glBegin(GL_QUADS);
                glVertex3f(-2,1.29,0);
                glVertex3f(-2,-1.29,0);
                glVertex3f(2,-1.29,0);
                glVertex3f(2,1.29,0);
            glEnd();
            glPopMatrix();
        }else if((j+1)==9){
 
            glPushMatrix();
            glMultMatrixf(mat);
            glColor3f(0.5,0.5,0);
            glBegin(GL_QUADS);
                glVertex3f(-2,1.265,0);
                glVertex3f(-2,-1.265,0);
                glVertex3f(2,-1.265,0);
                glVertex3f(2,1.265,0);
            glEnd();
            glPopMatrix();
        }else if((j+1)==10){
           
            

            glPushMatrix();
            glMultMatrixf(mat);
            glColor3f(0.5,0.5,0);
            glBegin(GL_QUADS);
                glVertex3f(-2,1.53,0);
                glVertex3f(-2,-1.53,0);
                glVertex3f(2,-1.53,0);
                glVertex3f(2,1.53,0);
            glEnd();
            glPopMatrix();
        }else if ((j+1)==11){ //murs verticaux
          
            glPushMatrix();
            glMultMatrixf(mat);
            glColor3f(0,0.3,0.3);
            glBegin(GL_QUADS);
                glVertex3f(-2,1,0);
                glVertex3f(-2,-1,0);
                glVertex3f(2,-1,0);
                glVertex3f(2,1,0);
            glEnd();
            glPopMatrix();
            
        }else if ((j+1)==12){ //murs verticaux
          
            glPushMatrix();
            glMultMatrixf(mat);
            glColor3f(0,0.3,0.3);
            glBegin(GL_QUADS);
            glVertex3f(-2,1.02,0);
            glVertex3f(-2,-1.02,0);
            glVertex3f(2,-1.02,0);
            glVertex3f(2,1.02,0);
            glEnd();
            glPopMatrix();
            
        }else if ((j+1)==13){ //murs verticaux
            
          
            glPushMatrix();
            glMultMatrixf(mat);
            glColor3f(0,0.3,0.3);
            glBegin(GL_QUADS);
                glVertex3f(-2,1.73,0);
                glVertex3f(-2,-1.73,0);
                glVertex3f(2,-1.73,0);
                glVertex3f(2,1.73,0);
            glEnd();
            glPopMatrix();
            
        }else if ((j+1)==14){ //murs verticaux
          
            glPushMatrix();
            glMultMatrixf(mat);
            glColor3f(0,0.3,0.3);
            glBegin(GL_QUADS);
                glVertex3f(-2,2.5,0);
                glVertex3f(-2,-2.5,0);
                glVertex3f(2,-2.5,0);
                glVertex3f(2,2.5,0);
            glEnd();
            glPopMatrix();
            
        }
        
    }*/
   
    //on affiche
    glutSwapBuffers();
    ///-----stepsimulation_end-----
    
}


/*
 Initialise la fenetre GLUT
 */
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
    glutKeyboardFunc(clavier);
    
    glutMainLoop();
}


/*
 Initialise la gravite, la box de collision de :
 - la boule
 - des murs externes
 */
void init_bull(){
    
    dynamicsWorld->setGravity(btVector3(0, 0, -4));
    ///create a few basic rigid bodies
    
    //float x = (0*DtoR); //angle de test pour pencher
    {
        btTransform CShapeTransform;
        CShapeTransform.setIdentity();
        CShapeTransform.setOrigin(btVector3(0,0, 0));
        
        
        //CShapeTransform.setRotation(btQuaternion (btVector3(0,0,1) , btScalar(0*DtoR)));
        
        btScalar mass(0.);
        
        //rigidbody is dynamic if and only if mass is non zero, otherwise static
        bool isDynamic = (mass != 0.f);
        
        btVector3 localInertia(0, 0, 0);
        if (isDynamic)
            CShape->calculateLocalInertia(mass, localInertia);
        
        //using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
        btDefaultMotionState* myMotionState = new btDefaultMotionState(CShapeTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, CShape, localInertia);
        btRigidBody* body = new btRigidBody(rbInfo);
        body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
        body->setActivationState(DISABLE_DEACTIVATION);
        //add the body to the dynamics world
        dynamicsWorld->addRigidBody(body);
    }
    
    //Sphere
    {
        //create a dynamic rigidbody
        btCollisionShape* sphShape = new btSphereShape(btScalar(0.15));
        collisionShapes.push_back(sphShape);
        
        /// Create Dynamic Objects
        //btTransform startTransform;
        tSphere.setIdentity();
        
        btScalar mass(1.f);
        
        //rigidbody is dynamic if and only if mass is non zero, otherwise static
        bool isDynamic = (mass != 0.f);
        
        btVector3 localInertia(0, 0, 0);
        if (isDynamic)
            sphShape->calculateLocalInertia(mass, localInertia);
        
        tSphere.setOrigin(btVector3(-4, 3, 3));
        
        
        
        //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
        btDefaultMotionState* myMotionState = new btDefaultMotionState(tSphere);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, sphShape, localInertia);
        //btRigidBody* body = new btRigidBody(rbInfo);
        //dynamicsWorld->addRigidBody(body);
        
        Sphere = new btRigidBody(rbInfo);
        dynamicsWorld->addRigidBody(Sphere);
    }
    
    
    //Sol
    {
        btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(5), btScalar(5), btScalar(0.2)));
        
        collisionShapes.push_back(groundShape);
        
        btTransform groundTransform;
        groundTransform.setIdentity();
        groundTransform.setOrigin(btVector3(0,0, 0));
        
        
        groundTransform.setRotation(btQuaternion (btVector3(0,0,1) , btScalar(0*DtoR)));
        
        btScalar mass(0.);
        
        //rigidbody is dynamic if and only if mass is non zero, otherwise static
        bool isDynamic = (mass != 0.f);
        
        btVector3 localInertia(0, 0, 0);
        if (isDynamic)
            groundShape->calculateLocalInertia(mass, localInertia);
        
        //using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
        btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
        btRigidBody* body = new btRigidBody(rbInfo);
       // body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
       // body->setActivationState(DISABLE_DEACTIVATION);
        //add the body to the dynamics world
        dynamicsWorld->addRigidBody(body);
        CShape->addChildShape(groundTransform, groundShape);
    
    }
    

    //mur verticaux : epaisseur x 0.2, hauteur y 2, longueur z (10)
    //mur  ->setOrigin(positionCentrex, 0, positionCentrez)
    //mur horizontaux : epaisseur z 0.2, hauteur y 2, longueur x 10
    //mur ->setOrigin(positionCentrex, 0, positionCentrez)
    
    //Mur Gauche
    {
        btCollisionShape* wallShape1 = new btBoxShape(btVector3(btScalar(4), btScalar(6.7), btScalar(0.2)));
        
        collisionShapes.push_back(wallShape1);
        
        btTransform wallTransform;
        wallTransform.setIdentity();
        wallTransform.setOrigin(btVector3(-5, 0, 0));
        
        wallTransform.setRotation(btQuaternion (btVector3(0,1,0) , btScalar(90*DtoR) ));
        
        btScalar mass(0.);
        
        //rigidbody is dynamic if and only if mass is non zero, otherwise static
        bool isDynamic = (mass != 0.f);
        
        btVector3 localInertia(0, 0, 0);
        if (isDynamic)
            wallShape1->calculateLocalInertia(mass, localInertia);
        
        //using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
        btDefaultMotionState* myMotionState = new btDefaultMotionState(wallTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, wallShape1, localInertia);
        btRigidBody* body = new btRigidBody(rbInfo);
        
        //add the body to the dynamics world
        dynamicsWorld->addRigidBody(body);
        CShape->addChildShape(wallTransform, wallShape1);
    }
    
    
    //Mur droit
    {
        btCollisionShape* wallShape2 = new btBoxShape(btVector3(btScalar(4), btScalar(6.7), btScalar(0.2)));
        
        collisionShapes.push_back(wallShape2);
        
        btTransform wallTransform;
        wallTransform.setIdentity();
        wallTransform.setOrigin(btVector3(5, 0, 0));
        
        wallTransform.setRotation(btQuaternion (btVector3(0,1,0) , btScalar(-90*DtoR) ));
        
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
        CShape->addChildShape(wallTransform, wallShape2);
    }
    
    //mur arriere-haut
    {
        btCollisionShape* wallShape3 = new btBoxShape(btVector3(btScalar(4.), btScalar(10.), btScalar(0.2)));
        
        collisionShapes.push_back(wallShape3);
        
        btTransform wallTransform, wallTransformBis;
        wallTransform.setIdentity();
        wallTransformBis.setIdentity();
        
        wallTransform.setOrigin(btVector3(0, 3.35, 0));
        
        wallTransformBis.setRotation(btQuaternion (btVector3(1,0,0) , btScalar(90*DtoR) ));
        wallTransform.setRotation(btQuaternion (btVector3(0,1,0) , btScalar(90*DtoR) ));
        wallTransform *= wallTransformBis;
        
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
        CShape->addChildShape(wallTransform, wallShape3);
    }
    
    //mur avant-bas
    {
        btCollisionShape* wallShape4 = new btBoxShape(btVector3(btScalar(4.), btScalar(10.), btScalar(0.2)));
        
        collisionShapes.push_back(wallShape4);
        
        btTransform wallTransform, wallTransformBis;
        wallTransform.setIdentity();
        wallTransformBis.setIdentity();
        wallTransform.setOrigin(btVector3(0, -3.35, 0));
        
        wallTransformBis.setRotation(btQuaternion (btVector3(1,0,0) , btScalar(90*DtoR) ));
        wallTransform.setRotation(btQuaternion (btVector3(0,1,0) , btScalar(90*DtoR) ));
        wallTransform *= wallTransformBis;
        
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
        CShape->addChildShape(wallTransform, wallShape4);
    }

    //Mur 1 Vertical
    {
        btCollisionShape* wallShape1 = new btBoxShape(btVector3(btScalar(2), btScalar(2.06), btScalar(0.2)));
        
        collisionShapes.push_back(wallShape1);
        
        btTransform wallTransform;
        wallTransform.setIdentity();
        wallTransform.setOrigin(btVector3((-5+1.5), (3.35-2.06), 0));
        
        wallTransform.setRotation(btQuaternion (btVector3(0,1,0) , btScalar(90*DtoR) ));
        
        btScalar mass(0.);
        
        //rigidbody is dynamic if and only if mass is non zero, otherwise static
        bool isDynamic = (mass != 0.f);
        
        btVector3 localInertia(0, 0, 0);
        if (isDynamic)
            wallShape1->calculateLocalInertia(mass, localInertia);
        
        //using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
        btDefaultMotionState* myMotionState = new btDefaultMotionState(wallTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, wallShape1, localInertia);
        btRigidBody* body = new btRigidBody(rbInfo);
        
        //add the body to the dynamics world
        dynamicsWorld->addRigidBody(body);
        CShape->addChildShape(wallTransform, wallShape1);
    }
    
    //Mur 2 Vertical
    {
        btCollisionShape* wallShape1 = new btBoxShape(btVector3(btScalar(2), btScalar(1.25), btScalar(0.2)));
        
        collisionShapes.push_back(wallShape1);
        
        btTransform wallTransform;
        wallTransform.setIdentity();
        wallTransform.setOrigin(btVector3((-5+3.5), (3.35-1.25), 0));
        
        wallTransform.setRotation(btQuaternion (btVector3(0,1,0) , btScalar(90*DtoR) ));
        
        btScalar mass(0.);
        
        //rigidbody is dynamic if and only if mass is non zero, otherwise static
        bool isDynamic = (mass != 0.f);
        
        btVector3 localInertia(0, 0, 0);
        if (isDynamic)
            wallShape1->calculateLocalInertia(mass, localInertia);
        
        //using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
        btDefaultMotionState* myMotionState = new btDefaultMotionState(wallTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, wallShape1, localInertia);
        btRigidBody* body = new btRigidBody(rbInfo);
        
        //add the body to the dynamics world
        dynamicsWorld->addRigidBody(body);
        CShape->addChildShape(wallTransform, wallShape1);
    }
    
    //Mur 3 Vertical
    {
        btCollisionShape* wallShape1 = new btBoxShape(btVector3(btScalar(2), btScalar(1.29), btScalar(0.2)));
        
        collisionShapes.push_back(wallShape1);
        
        btTransform wallTransform;
        wallTransform.setIdentity();
        wallTransform.setOrigin(btVector3((-5+5.3), (3.35-1.03-(1.29)), 0));
        
        wallTransform.setRotation(btQuaternion (btVector3(0,1,0) , btScalar(90*DtoR) ));
        
        btScalar mass(0.);
        
        //rigidbody is dynamic if and only if mass is non zero, otherwise static
        bool isDynamic = (mass != 0.f);
        
        btVector3 localInertia(0, 0, 0);
        if (isDynamic)
            wallShape1->calculateLocalInertia(mass, localInertia);
        
        //using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
        btDefaultMotionState* myMotionState = new btDefaultMotionState(wallTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, wallShape1, localInertia);
        btRigidBody* body = new btRigidBody(rbInfo);
        
        //add the body to the dynamics world
        dynamicsWorld->addRigidBody(body);
        CShape->addChildShape(wallTransform, wallShape1);
    }
    
    //Mur 4 Vertical
    {
        btCollisionShape* wallShape1 = new btBoxShape(btVector3(btScalar(2), btScalar(1.265), btScalar(0.2)));
        
        collisionShapes.push_back(wallShape1);
        
        btTransform wallTransform;
        wallTransform.setIdentity();
        wallTransform.setOrigin(btVector3((-5+3.58), (-3.35+1.265), 0));
        
        wallTransform.setRotation(btQuaternion (btVector3(0,1,0) , btScalar(90*DtoR) ));
        
        btScalar mass(0.);
        
        //rigidbody is dynamic if and only if mass is non zero, otherwise static
        bool isDynamic = (mass != 0.f);
        
        btVector3 localInertia(0, 0, 0);
        if (isDynamic)
            wallShape1->calculateLocalInertia(mass, localInertia);
        
        //using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
        btDefaultMotionState* myMotionState = new btDefaultMotionState(wallTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, wallShape1, localInertia);
        btRigidBody* body = new btRigidBody(rbInfo);
        
        //add the body to the dynamics world
        dynamicsWorld->addRigidBody(body);
        CShape->addChildShape(wallTransform, wallShape1);
    }
    
    //Mur 5 Vertical
    {
        btCollisionShape* wallShape1 = new btBoxShape(btVector3(btScalar(2), btScalar(1.53), btScalar(0.2)));
        
        collisionShapes.push_back(wallShape1);
        
        btTransform wallTransform;
        wallTransform.setIdentity();
        wallTransform.setOrigin(btVector3((-5+6.15), (-3.35+(1.53)), 0));
        
        wallTransform.setRotation(btQuaternion (btVector3(0,1,0) , btScalar(90*DtoR) ));
        
        btScalar mass(0.);
        
        //rigidbody is dynamic if and only if mass is non zero, otherwise static
        bool isDynamic = (mass != 0.f);
        
        btVector3 localInertia(0, 0, 0);
        if (isDynamic)
            wallShape1->calculateLocalInertia(mass, localInertia);
        
        //using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
        btDefaultMotionState* myMotionState = new btDefaultMotionState(wallTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, wallShape1, localInertia);
        btRigidBody* body = new btRigidBody(rbInfo);
        
        //add the body to the dynamics world
        dynamicsWorld->addRigidBody(body);
        CShape->addChildShape(wallTransform, wallShape1);
    }
    
    
    //Mur Horizontal 1
    
    {
        btCollisionShape* wallShape4 = new btBoxShape(btVector3(btScalar(2.), btScalar(1), btScalar(0.2)));
        
        collisionShapes.push_back(wallShape4);
        
        btTransform wallTransform, wallTransformBis;
        wallTransform.setIdentity();
        wallTransformBis.setIdentity();
        wallTransform.setOrigin(btVector3(-5+1.5+1, 3.35-2.5, 0));
        
        wallTransformBis.setRotation(btQuaternion (btVector3(1,0,0) , btScalar(90*DtoR) ));
        wallTransform.setRotation(btQuaternion (btVector3(0,1,0) , btScalar(90*DtoR) ));
        wallTransform *= wallTransformBis;
        
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
        CShape->addChildShape(wallTransform, wallShape4);
    }
    //Mur Horizontal 2
    
    {
        btCollisionShape* wallShape4 = new btBoxShape(btVector3(btScalar(2.), btScalar(1.02), btScalar(0.2)));
        
        collisionShapes.push_back(wallShape4);
        
        btTransform wallTransform, wallTransformBis;
        wallTransform.setIdentity();
        wallTransformBis.setIdentity();
        wallTransform.setOrigin(btVector3(-5+1.5+1.02, 3.35-4.83, 0));
        
        wallTransformBis.setRotation(btQuaternion (btVector3(1,0,0) , btScalar(90*DtoR) ));
        wallTransform.setRotation(btQuaternion (btVector3(0,1,0) , btScalar(90*DtoR) ));
        wallTransform *= wallTransformBis;
        
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
        CShape->addChildShape(wallTransform, wallShape4);
    }
    
    
    //Mur Horizontal 3
    
    {
        btCollisionShape* wallShape4 = new btBoxShape(btVector3(btScalar(2.), btScalar(1.73), btScalar(0.2)));
        
        collisionShapes.push_back(wallShape4);
        
        btTransform wallTransform, wallTransformBis;
        wallTransform.setIdentity();
        wallTransformBis.setIdentity();
        wallTransform.setOrigin(btVector3(5-1.73, 3.35-1.79, 0));
        
        wallTransformBis.setRotation(btQuaternion (btVector3(1,0,0) , btScalar(90*DtoR) ));
        wallTransform.setRotation(btQuaternion (btVector3(0,1,0) , btScalar(90*DtoR) ));
        wallTransform *= wallTransformBis;
        
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
        CShape->addChildShape(wallTransform, wallShape4);
    }
    
    
    //Mur Horizontal 4
    
    {
        btCollisionShape* wallShape4 = new btBoxShape(btVector3(btScalar(2.), btScalar(2.5), btScalar(0.2)));
        
        collisionShapes.push_back(wallShape4);
        
        btTransform wallTransform, wallTransformBis;
        wallTransform.setIdentity();
        wallTransformBis.setIdentity();
        wallTransform.setOrigin(btVector3(-5+3.08+2.5, 3.35-3.61, 0));
        
        wallTransformBis.setRotation(btQuaternion (btVector3(1,0,0) , btScalar(90*DtoR) ));
        wallTransform.setRotation(btQuaternion (btVector3(0,1,0) , btScalar(90*DtoR) ));
        wallTransform *= wallTransformBis;
        
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
        CShape->addChildShape(wallTransform, wallShape4);
    }
}


int test(int argc, char** argv)
{
    
    
    init_bull();
    init_fenetre(argc,argv);
    
    
    //free_mem();
    
    return 0;
}

/*
 
 void free_mem(){
 
 //cleanup in the reverse order of creation/initialization
 
 ///-----cleanup_start-----
 
 //remove the rigidbodies from the dynamics world and delete them
 for (i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
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

