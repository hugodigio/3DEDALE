//
//  TestVidYout.cpp
//  
//
//  Created by Alzahra Hassanein on 12/12/2018.
//

#include "TestVidYout.hpp"

#include <iostream>
//#include "camera.h"
//#include <GLUT/gl.h>
#include <GLUT/glut.h>
#include <vector>
#include "./lib/bullet3/src/btBulletDynamicsCommon.h"    //you may need to change this


camera cam;
GLUquadricObj* quad;
btDynamicsWorld* world;    //every physical object go to the world
btDispatcher* dispatcher;    //what collision algorithm to use?
btCollisionConfiguration* collisionConfig;    //what collision algorithm to use?
btBroadphaseInterface* broadphase;    //should Bullet examine every object, or just what close to each other
btConstraintSolver* solver;                    //solve collisions, apply forces, impulses
std::vector<btRigidBody*> bodies;

btRigidBody* addSphere(float rad,float x,float y,float z,float mass)
{
    btTransform t;    //position and rotation
    t.setIdentity();
    t.setOrigin(btVector3(x,y,z));    //put it to x,y,z coordinates
    btSphereShape* sphere=new btSphereShape(rad);    //it's a sphere, so use sphereshape
    btVector3 inertia(0,0,0);    //inertia is 0,0,0 for static object, else
    if(mass!=0.0)
        sphere->calculateLocalInertia(mass,inertia);    //it can be determined by this function (for all kind of shapes)
    
    btMotionState* motion=new btDefaultMotionState(t);    //set the position (and motion)
    btRigidBody::btRigidBodyConstructionInfo info(mass,motion,sphere,inertia);    //create the constructioninfo, you can create multiple bodies with the same info
    btRigidBody* body=new btRigidBody(info);    //let's create the body itself
    world->addRigidBody(body);    //and let the world know about it
    bodies.push_back(body);    //to be easier to clean, I store them a vector
    return body;
}

void renderSphere(btRigidBody* sphere)
{
    if(sphere->getCollisionShape()->getShapeType()!=SPHERE_SHAPE_PROXYTYPE)    //only render, if it's a sphere
        return;
    glColor3f(1,0,0);
    float r=((btSphereShape*)sphere->getCollisionShape())->getRadius();
    btTransform t;
    sphere->getMotionState()->getWorldTransform(t);    //get the transform
    float mat[16];
    t.getOpenGLMatrix(mat);    //OpenGL matrix stores the rotation and orientation
    glPushMatrix();
    glMultMatrixf(mat);    //multiplying the current matrix with it moves the object in place
    gluSphere(quad,r,20,20);
    glPopMatrix();
}

//similar then renderSphere function
void renderPlane(btRigidBody* plane)
{
    if(plane->getCollisionShape()->getShapeType()!=STATIC_PLANE_PROXYTYPE)
        return;
    glColor3f(0.8,0.8,0.8);
    btTransform t;
    plane->getMotionState()->getWorldTransform(t);
    float mat[16];
    t.getOpenGLMatrix(mat);
    glPushMatrix();
    glMultMatrixf(mat);    //translation,rotation
    glBegin(GL_QUADS);
    glVertex3f(-1000,0,1000);
    glVertex3f(-1000,0,-1000);
    glVertex3f(1000,0,-1000);
    glVertex3f(1000,0,1000);
    glEnd();
    glPopMatrix();
}


void init(float angle)
{
    //pretty much initialize everything logically
    collisionConfig=new btDefaultCollisionConfiguration();
    dispatcher=new btCollisionDispatcher(collisionConfig);
    broadphase=new btDbvtBroadphase();
    solver=new btSequentialImpulseConstraintSolver();
    world=new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfig);
    world->setGravity(btVector3(0,-10,0));    //gravity on Earth
    
    //similar to createSphere
    btTransform t;
    t.setIdentity();
    t.setOrigin(btVector3(0,0,0));
    btStaticPlaneShape* plane=new btStaticPlaneShape(btVector3(0,1,0),0);
    btMotionState* motion=new btDefaultMotionState(t);
    btRigidBody::btRigidBodyConstructionInfo info(0.0,motion,plane);
    btRigidBody* body=new btRigidBody(info);
    world->addRigidBody(body);
    bodies.push_back(body);
    
    addSphere(1.0,0,20,0,1.0);    //add a new sphere above the ground
    
    glClearColor(0,0,0,1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(angle,640.0/480.0,1,1000);
    glMatrixMode(GL_MODELVIEW);
    quad=gluNewQuadric();
    //initskybox();
    glEnable(GL_DEPTH_TEST);
    cam.setLocation(vector3d(10,10,10));    //the player will be top of the terrain
}


void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    cam.Control();
    //drawSkybox(50);
    cam.UpdateCamera();
    for(int i=0;i<bodies.size();i++)
    {
        if(bodies[i]->getCollisionShape()->getShapeType()==STATIC_PLANE_PROXYTYPE)
            renderPlane(bodies[i]);
        else if(bodies[i]->getCollisionShape()->getShapeType()==SPHERE_SHAPE_PROXYTYPE)
            renderSphere(bodies[i]);
    }
}


int main()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_SetVideoMode(640,480,32,SDL_OPENGL);
    Uint32 start;
    SDL_Event event;
    bool running=true;
    float angle=50;
    init(angle);
    while(running)
    {
        start=SDL_GetTicks();
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                    running=false;
                    break;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        running=false;
                        break;
                    case SDLK_y:
                        cam.mouseIn(false);
                        break;
                    case SDLK_SPACE:
                        //if space is pressed, shoot a ball
                        btRigidBody* sphere=addSphere(1.0,cam.getLocation().x,cam.getLocation().y,cam.getLocation().z,1.0);
                        vector3d look=cam.getVector()*20;
                        sphere->setLinearVelocity(btVector3(look.x,look.y,look.z));
                        break;
                }
                    break;
                case SDL_KEYUP:
                    switch(event.key.keysym.sym)
                {
                        
                }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    cam.mouseIn(true);
                    break;
                    
            }
        }
        world->stepSimulation(1/60.0);    //you have to call this function, to update the simulation (with the time since last time in seconds), you can actually call this with varying variables, so you can actually mease the time since last update
        display();
        SDL_GL_SwapBuffers();
        if(1000.0/60>SDL_GetTicks()-start)
            SDL_Delay(1000.0/60-(SDL_GetTicks()-start));
    }
    //killskybox();
    for(int i=0;i<bodies.size();i++)
    {
        world->removeCollisionObject(bodies[i]);
        btMotionState* motionState=bodies[i]->getMotionState();
        btCollisionShape* shape=bodies[i]->getCollisionShape();
        delete bodies[i];
        delete shape;
        delete motionState;
    }
    delete dispatcher;
    delete collisionConfig;
    delete solver;
    delete broadphase;
    delete world;
    SDL_Quit();
    gluDeleteQuadric(quad);
}
