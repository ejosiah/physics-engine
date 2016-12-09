//
//  templateScene.h
//  opengl
//
//  Created by Josiah Ebhomenye on 08/12/2016.
//  Copyright (c) 2016 Josiah Ebhomenye. All rights reserved.
//

#ifndef opengl_templateScene_h
#define opengl_templateScene_h

#include "scene.h"

class Object : public SceneObject{
    
    virtual const char* name() override{
        return "Object";
    }
    
    virtual void update(float elapsedTime) override{
        integrate(elapsedTime);
    }
    
    virtual void draw() override{
    }
};

class templateScene : public Scene{
public:
    using Scene::Scene;
    
    
    virtual void init() override {
        Scene::init();
    }
    
    
    virtual void onKeyPress(unsigned char key, int x, int y) override{
        using namespace std;
    }
    
    virtual void onMouseDown(int button, int state, int x, int y) override{

    }
    
};

Scene* Scene::instance = new templateScene("Template");

#endif

