//
//  main.cpp
//  opengl
//
//  Created by Josiah Ebhomenye on 03/12/2016.
//  Copyright (c) 2016 Josiah Ebhomenye. All rights reserved.
//

#include "RainingBalls.h"
#include <iostream>
#include "Timer.h"
#include "random.h"
#include "core.h"
Scene& scene = *Scene::instance;

void init(){
    scene.init();
}

void display(){
    scene.render();
    glFlush();
    glutSwapBuffers();
}

void resize(int w, int h){
    scene.resize(w, h);
}

void keyPress(unsigned char key, int x, int y){
    scene.keyPress(key, x, y);
}

void specialKey(int key, int x, int y){
    scene.specialKey(key, x, y);
}

void update(){
    Timer::get().update();
    scene.update();
    glutPostRedisplay();
}

void timer(int value){
    scene.updateFrameData(value);
    glutTimerFunc(1000, timer, ++value);
}

void mouse(int buton, int state, int x, int y){
    scene.mouseDown(buton, state, x, y);
}

void mouseMove(int x, int y){
    scene.mouseMove(x, y);
    glutPostRedisplay();
}

/*
int main(int argc, char * argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
    glutInitWindowSize(scene.width(), scene.height());
    glutInitWindowPosition(100, 100);
    glutCreateWindow(scene.title());
    
    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyPress);
    glutSpecialFunc(specialKey);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMove);
    
    glutTimerFunc(1000, timer, 1);
    
    glutIdleFunc(update);
    
    Timer::start();
    scene.init();
    
    
    glutMainLoop();
    
    Timer::stop();
    delete Scene::instance;
    
    return 0;
 }
*/
//3.142857142857143
int main(){
    using namespace std;
    Matrix4 R = rotateZ(30) * rotateX(30);
  //  Matrix4 R = rotate(30, {1, 0, 1});
    
//    Quaternion q{0.5, 0.5, 0.5, 0.5};
//    Quaternion q1{0.0, 0.0, 22.0f/7.0f, 0};
//    Vector av{0, 22.0f/7.0f, 0};
//    
//    q.addScaled(av, 2.0f);
    
    Quaternion q = "0.5 0.5 0.5 0.5"_q * "0 3.142857 0"_v;
    
    cout << q << endl;
    
    return 0;
}
