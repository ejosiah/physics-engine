//
//  rotation.h
//  opengl
//
//  Created by Josiah Ebhomenye on 12/12/2016.
//  Copyright (c) 2016 Josiah Ebhomenye. All rights reserved.
//

#ifndef opengl_rotation_h
#define opengl_rotation_h

#include <string>
#include <cmath>
#include "scene.h"

class Rotation : public Scene{
private:
    real yaw = 0;
    real pitch = 0;
    real roll = 0;
    real delta = 5;
    real r = 20;
    int n = 20;
    
public:
    using Scene::Scene;

    
    virtual void init() override {
        Scene::init();
    }
    
    bool once = true;
    
    void printCurrentMatrix(){
        using namespace std;
        float matrix[16];
        glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
        for(int i = 0; i < 16; i++){
            if(i%4 == 0) cout << endl;
            cout << matrix[i] << " ";
            
        }
    }
    
    virtual void display() override {

        
        if(once){
            glPushMatrix();
            glLoadIdentity();
            glRotatef(30, 0, 0, 1);
            printCurrentMatrix();
            glPopMatrix();
            once = false;
        }
        
        
        
        glRotatef(yaw, 0, 1, 0);
        
        glLineWidth(5);
                    
        
//        glBegin(GL_LINES);
//        glColor3f(1, 0, 0);
//        glVertex3f(0, 0, 0);
//        glVertex3f(10, 0, 0);
//        
//        glColor3f(0, 1, 0);
//        glVertex3f(0, 0, 0);
//        glVertex3f(0, 10, 0);
//        
//        glColor3f(0, 0, 1);
//        glVertex3f(0, 0, 0);
//        glVertex3f(0, 0, -10);
//        glEnd();
        
                    
        drawCircleY();
                    
        glPushMatrix();
        glRotatef(pitch, 1, 0, 0);
        drawCircleX();
        
        glPushMatrix();
        glRotatef(roll, 0, 0, 1);
        drawCircleZ();
        glPopMatrix();
        glPopMatrix();
        
        
        glLineWidth(1);
        
        std::string msg = "Yaw: " + std::to_string((int)roundf(yaw));
        renderText(10, 10, msg.c_str());
        msg = "Pitch: " + std::to_string((int)roundf(pitch));
        renderText(10, 25, msg.c_str());
        msg = "Roll: " + std::to_string((int)roundf(roll));
        renderText(10, 40, msg.c_str());
    }
    
    void drawCircleZ(){
        glColor3f(0, 0, 1);
        glLineWidth(1.5);
        glBegin(GL_LINE_LOOP);
        real t = 0;
        for(int i = 0; i < n; i++){
            t = 2*PI*i/n;
            glVertex3f(r * cos(t), r * sin(t), 0);
        }
        glEnd();
    }
    
    void drawCircleX(){
        glColor3f(1, 0, 0);
        glLineWidth(1.5);
        glBegin(GL_LINE_LOOP);
        real t = 0;
        for(int i = 0; i < n; i++){
            t = 2*PI*i/n;
            glVertex3f(0, r * sin(t), r * cos(t));
        }
        glEnd();
    }
    
    void drawCircleY(){
        glColor3f(0, 1, 0);
        glLineWidth(1.5);
        glBegin(GL_LINE_LOOP);
        real t = 0;
        for(int i = 0; i < n; i++){
            t = 2*PI*i/n;
            glVertex3f(r * cos(t), 0, r * sin(t));
        }
        glEnd();
    }
    
    
    virtual void onKeyPress(unsigned char key, int x, int y) override{
        switch(key){
            case 'p':
                pitch += delta;
                if(pitch > 360) pitch -= 360;
                break;
            case 'y':
                yaw += delta;
                if(yaw > 360) yaw -= 360;
                break;
            case 'r':
                roll += delta;
                if(roll > 360) roll -= 360;
                break;
            case 'P':
                pitch -= delta;
                if(pitch < -360) pitch += 360;
                break;
            case 'Y':
                yaw -= delta;
                if(yaw < -360) yaw += 360;
                break;
            case 'R':
                roll -= delta;
                if(roll < -360) roll += 360;
                break;
            default:
                break;
                
        }
    }
    
    virtual void onMouseDown(int button, int state, int x, int y) override{
        
    }
    
};

Scene* Scene::instance = new Rotation("Rotation");

#endif
