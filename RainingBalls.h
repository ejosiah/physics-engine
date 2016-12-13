//
//  RainingBalls.h
//  opengl
//
//  Created by Josiah Ebhomenye on 12/12/2016.
//  Copyright (c) 2016 Josiah Ebhomenye. All rights reserved.
//

#ifndef opengl_RainingBalls_h
#define opengl_RainingBalls_h

#include <string>
#include "scene.h"
#include "random.h"
#include <iostream>

static int id = 0;

RandomColor rngColor;
Random RNG;

class Ball : public ParticleSceneObject{
private:
    std::string _name;
    real radius;
    Vector color;
    
    
public:
    bool onScene = false;
    real lifetime;
 
public:
    Ball(real r = 0): radius(r){
        _name = "ball" + std::to_string(id++);
        color = rngColor();
        lifetime = 0;
    }
    
    virtual const char* name() override{
        return _name.c_str();
    }
    
    virtual void update(float elapsedTime) override{
        lifetime += elapsedTime;
        if(lifetime > 30){
            lifetime = 0;
            Scene::remove(this);
        }
    }
    
    virtual void draw() override{
        glPushMatrix();
        glTranslatef(x(), y(), z());
        glColor3fv(color);
        glutSolidSphere(radius, 20, 20);
        glPopMatrix();
        
    }
};

class RaningBalls : public Scene{
private:
    const static int row = 20;
    const static int col = 20;
    const static int nBalls = 1;
    Ball clouds[row][col][nBalls];
    real radius = 2;
    real currentTime;
    Gravity* gravity = new Gravity({0, -20, 0});
public:
    RaningBalls(const char* title):Scene(title){
        using namespace std;
        float r = radius;
        real w = r * row;
        real h = r * col;
        float x = 0;
        float z = 0;
        
        for(int i = 0; i < row; i++){
            for(int j = 0; j < col; j++){
                Ball ball{r};
                ball.mass(1);
                ball.damping(0.99);
                ball.position({x, 200, -z});
                clouds[i][j][0] = ball;
                x += 2 * r + 0.05;
            }
            x = 0;
            z += 2 * r + 1;
        }
    }
    
    virtual void update() override {
        using namespace std;
        currentTime += Timer::get().lastFrameTime;
        if(currentTime >= 0.1){
            int n = 1;
            for(int i = 0; i < n; i++){
                int r = RNG._int(row - 1);
                int c = RNG._int(col - 1);
                int n = RNG._int(nBalls - 1);
                
                world.forceRegistry() + Registration{clouds[r][c][n], *gravity};
                
                if(!clouds[r][c][n].onScene){
                    addObject(&clouds[r][c][n]);
                    clouds[r][c][n].onScene = true;
                }
            }
            currentTime = 0;
        }
        Scene::update();
    }
    
    virtual void init() override {
        Scene::init();
    }
    
    virtual void display() override {
        glTranslatef(-(row * radius), 0, (col * radius));
        Scene::display();
    }
    
    virtual void onKeyPress(unsigned char key, int x, int y) override{
        using namespace std;
    }
    
    virtual void onMouseDown(int button, int state, int x, int y) override{
        
    }
    
};

Scene* Scene::instance = new RaningBalls("Raining Balls");


#endif
