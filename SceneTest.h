//
//  SceneTest.h
//  opengl
//
//  Created by Josiah Ebhomenye on 03/12/2016.
//  Copyright (c) 2016 Josiah Ebhomenye. All rights reserved.
//

#ifndef opengl_SceneTest_h
#define opengl_SceneTest_h

#include "scene.h"
#include "Particle.h"
#include <algorithm>
#include "forces.h"
#include "Timer.h"
#include "random.h"
#include "Spring.h"

const Vector ACCELERATION_DUE_TO_GRAVTY{0, -9.8, 0};

Force* gravity = new Gravity{ACCELERATION_DUE_TO_GRAVTY};
//Force* groundForce = new GroundForce{Vector{0, 0, 0}, ACCELERATION_DUE_TO_GRAVTY};
ForceRegistry forceRegistry;

class Sphere : public ParticleSceneObject{
private:
    float radius;
    
public:
    Sphere(float r = 0):
    radius(r){
        _damping = 0.45;
    }
    
    virtual const char* name() override{
        return "Sphere";
    }
    
    virtual void update(float elapsedTime) override{
        integrate(elapsedTime);
    }
    
    virtual void draw() override{
     //   if(_position.y > 0){
            glPushMatrix();
            glTranslatef(_position.x, _position.y, _position.z);
            glColor3f(1.0, 0.0, 0.0);
            glutSolidSphere(2.5, 20, 20);
            glPopMatrix();
     //   }
        
        // shpere shadow
//        glPushMatrix();
//        glDisable(GL_LIGHTING);
//        glTranslatef(0, 0, 10);
//        glScaled(0.8, 0.1, 0.8);
//        glColor3f(0.8, 0.8, 0.8);
//        glutSolidSphere(radius, 20, 20);
//        glEnable(GL_LIGHTING);
//        glPopMatrix();
    }
};

class Cube : public SceneObject{
private:
    float length;
    
public:
    Cube(float l = 0):length(l){}
    
    virtual const char* name(){
        return "cube";
    }
    
    virtual void draw() override{
        glPushMatrix();
        glColor3f(0, 0, 1);
        glTranslatef(0, length/2, -20);
        glutSolidCube(length);
        glPopMatrix();
    }
    
    virtual void update(float elapsedTime) override{
        
    }
};

class Teapot : public SceneObject{
private:
    float size;
    // r1 = 0.25, r2 = 0.25, periodlength=3.0, cycles = 4
    Spring s{5, 0.25, 0.25, 3, 200, 20};
    
public:
    Teapot(float s = 0):size(s){}
    
    virtual const char* name(){
        return "Teapot";
    }
    
    virtual void draw() override{
        glPushMatrix();
        glColor3f(0, 1, 1);
        glTranslatef(-20, size - 1, -20);
       // glScalef(5, 5, 5);
       // glRotatef(90, 1, 0, 0);
      //  glutSolidTeapot(size);
        s.draw();
        glPopMatrix();
    }
    
    virtual void update(float elapsedTime) override{
        
    }
    
};

class Cone : public SceneObject{
private:
    float base, height;
    
public:
    Cone(float b = 0, float h = 0):base(b), height(h){}
    
    virtual const char* name(){
        return "Cone";
    }
    
    virtual void draw() override{
        glPushMatrix();
        glColor3f(1, 0, 1);
        glTranslatef(20, 0, -20);
        glRotatef(-90, 1, 0, 0);
        glutSolidCone(base, height, 20, 20);
        glPopMatrix();
    }
    
    virtual void update(float elapsedTime) override{
    }
    
};

class SceneTest : public Scene{
private:
    Teapot teapot{5.0f};
    Cube cube{5.0f};
    Sphere sphere{2.5f};
    Cone cone{5.0f, 5.0f};
    
public:
    using Scene::Scene;
    
    virtual void init() override {
        Scene::init();
        glEnable(GL_RESCALE_NORMAL);
        glEnable(GL_NORMALIZE);
        sphere.position({0, 50, 0});
        sphere.mass(1000);
        
        forceRegistry + Registration{sphere, *gravity};
  //      forceRegistry + Registration{sphere, *groundForce};
        
        addObject(&teapot);
        addObject(&cube);
        addObject(&sphere);
        addObject(&cone);
        
    }
    
    virtual void display() override {
       // gluLookAt(80, 20, 60, 78.2929, 19, 58.2929, 0, 1, 0);
        std::for_each(objects.begin(), objects.end(), [](std::pair<const char*, SceneObject*> entry){
            entry.second->draw();
        });
    }
    
    virtual void update() override {
        Scene::update();
        float time = Timer::get().lastFrameTime;
        forceRegistry.applyForces(time);
        
        sphere.update(time);
    }
};

Scene* Scene::instance = new SceneTest("Scene Test", 1080, 720);

#endif
