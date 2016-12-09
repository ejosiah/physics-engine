//
//  SpringTest.h
//  opengl
//
//  Created by Josiah Ebhomenye on 07/12/2016.
//  Copyright (c) 2016 Josiah Ebhomenye. All rights reserved.
//

#ifndef opengl_SpringTest_h
#define opengl_SpringTest_h

#include "scene.h"
#include "Spring.h"
#include "SpringForces.h"

class Sphere : public SceneObject{
public:
    Spring s{20, 0.25, 0.25, 3, 200, 20};
    real l = 30;
    real newL = 0;    // by 30 for every whole scala
    real dl = 30;
    real scalaY = 1.0f;
    real dScala = 0.5f;
    Particle pa;
    Particle pb;
    Vector ancor;
    Force* forceA;
    Force* forceB;
    bool pulling;
    
    Sphere(){
        pb.mass(50);
        pb.damping(1);
        pb.position({0, l, 0});
        
        pa.mass(50);
        pa.damping(1);
        forceA = new SpringForce(pb, 0.05, l);
        forceB = new SpringForce(pa, 0.05, l);
    }
    
public:
    
    virtual const char* name(){ return "sphere"; }
        
    virtual void update(float duration){
        using namespace std;
        pa.integrate(duration);
        pb.integrate(duration);
        real newL = (pa.position() - pb.position()).length();
        if (pulling && newL < l*2) {
            Vector d = pa.position() - Vector{0, 1.0f, 0};
            pa.position(d);
        }
        
        scalaY = (newL != 0 ? newL/dl : 1);
//            cout << "newL" << newL << endl;
//            cout << "scaleY" << scalaY << endl;
        
    }
    
    void addForce(){
        forceRegistry + Registration{pa, *forceA};
        forceRegistry + Registration{pb, *forceB};
    }
    
    void removeForce(){
        forceRegistry - Registration{pa, *forceA};
        forceRegistry - Registration{pb, *forceB};
    }
    
    virtual void draw() {
        
        glPushMatrix();
        glColor3f(1, 0, 0);
        glTranslatef(pb.x(), pb.y(), pb.z());
        glutSolidSphere(5, 50, 50);
      //  glScalef(2, 0.2, 2);
       // glutSolidCube(5);
        glPopMatrix();
        
        
        glPushMatrix();
        glColor3f(0, 0, 1);
        glTranslatef(pa.x(), pa.y(), pa.z());
        glutSolidSphere(5, 50, 50);
        glColor3f(0, 1, 0);
        glScalef(2, scalaY, 2);
        glPushMatrix();
        glRotatef(-90, 1, 0, 0);
        s.draw();
        glPopMatrix();
        glPopMatrix();
        

    }
};

class SpringTest : public Scene{
    using Scene::Scene;
    
    Sphere sphere;
    
    virtual void init() override {
        Scene::init();
        addObject(&sphere);
        
    }
    
    virtual void onKeyPress(unsigned char key, int x, int y){
        using namespace std;
    }
    
    virtual void onMouseDown(int button, int state, int x, int y){
        if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
            sphere.removeForce();
            sphere.pulling = true;
        }else if(button == GLUT_RIGHT_BUTTON && state == GLUT_UP){
            sphere.addForce();
            sphere.pulling = false;
            std::cout << "stopped pulling" << std::endl;
        }
    }

};

Scene* Scene::instance = new SpringTest("Spring test");

#endif
