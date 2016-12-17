//
//  QuaternionTest.h
//  opengl
//
//  Created by Josiah Ebhomenye on 16/12/2016.
//  Copyright © 2016 Josiah Ebhomenye. All rights reserved.
//

#ifndef QuaternionTest_h
#define QuaternionTest_h

#include "scene.h"
#include <iostream>

class QuaternionTest : public Scene{
private:
    bool once = true;
public:
    using Scene::Scene;
    
    Vector axis{1, 1, 1};
    Vector point{50, 0, 0};
    
    virtual void init() override {
        Scene::init();
        glDisable(GL_LIGHTING);
    }
    
    virtual void display() override{
        using namespace std;
        glColor3f(0.8, 0.8, 0.8);
        real x = -100, z = 100, w = 20;
        
        glPushMatrix();
        glBegin(GL_LINES);
        for(int i = 0; i < 11; i++){
            glVertex3f(x + i * w, 0, z);
            glVertex3f(x + i * w, 0, -z);
        }
        glEnd();
        
        glBegin(GL_LINES);
        for(int i = 10; i > -1; i--){
            glVertex3f(x, 0, z - i * w);
            glVertex3f(-x, 0, z - i * w);
        }
        glEnd();
        glPopMatrix();

        axis = axis.normalize() * cl;
        
        Vector axisI = axis * -1;
        
        
        glPushMatrix();
        glColor3f(0.2, 0.267, 0);
        glBegin(GL_LINES);
        glVertex3fv(axis);
        glVertex3fv(axisI);
        glEnd();
        glPopMatrix();
        
        int n = 20;
        int i;
        Quaternion q;
        for(i = 0; i < n; i++){
            glColor3f(0.2, 0.73, 1.0);
            real t = i * 2 * PI/n;
            real cos0 = real_cos(t/2);
            real sin0 = real_sin(t/2);
            axis.normalize();
            q = Quaternion{ cos0, axis.x * sin0, axis.y * sin0, axis.z * sin0 };
            q.normailize();
            Matrix4 m;
        
            
            glBegin(GL_LINE_LOOP);
            m = Matrix4(q) * translate(point);
            
            if(once){
                cout << m << endl;
            }
            
            Vector v = m * Vector();
            
            glVertex3fv(v);
        
        }
        glEnd();
        once = false;
    }
    
    void printCurrentMatrix(){
        real matrix[16];
        glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
        for(int i = 0; i < 16; i++){
            if(i%4 == 0) std::cout << std::endl;
            std::cout << matrix[i] << " ";
        }
    }
    
    virtual void onKeyPress(unsigned char key, int x, int y) override{
        using namespace std;
    }
    
    virtual void onMouseDown(int button, int state, int x, int y) override{
        
    }
    
};

Scene* Scene::instance = new QuaternionTest("Quaternion Test");


#endif /* QuaternionTest_h */
