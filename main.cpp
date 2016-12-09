//
//  main.cpp
//  opengl
//
//  Created by Josiah Ebhomenye on 03/12/2016.
//  Copyright (c) 2016 Josiah Ebhomenye. All rights reserved.
//
#include "canonTest.h"
#include <iostream>
#include "Timer.h"
#include "random.h"
#include "VectorTest.h"
#include <cpptest-textoutput.h>

Scene& scene = *Scene::instance;

void init(){
    scene.init();
}

void display(){
    scene.render();
    glFlush();
}

void resize(int w, int h){
    scene.resize(w, h);
}

void keyPress(unsigned char key, int x, int y){
    scene.keyPress(key, x, y);
    glutPostRedisplay();
}

void specialKey(int key, int x, int y){
    scene.specialKey(key, x, y);
    glutPostRedisplay();
}

void update(){
    Timer::get().update();
    scene.update();
    glutPostRedisplay();
}

void mouse(int buton, int state, int x, int y){
    scene.mouseDown(buton, state, x, y);
    glutPostRedisplay();
}

void mouseMove(int x, int y){
    scene.mouseMove(x, y);
    glutPostRedisplay();
}

int main(int argc, char * argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_DEPTH | GLUT_RGBA);
    glutInitWindowSize(scene.width(), scene.height());
    glutInitWindowPosition(100, 100);
    glutCreateWindow(scene.title());
    
    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyPress);
    glutSpecialFunc(specialKey);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMove);
    
    glutIdleFunc(update);
    
    scene.init();
    Timer::start();
    
    glutMainLoop();
    
    Timer::stop();
    delete Scene::instance;
    
    return 0;
    
    VectorTestSuite vecTest;
    Test::TextOutput output(Test::TextOutput::Verbose);
    
    return vecTest.run(output, false) ? 0 : 1;
    
}



//#include "core.h"
//#include <glm/vec3.hpp>
//#include <glm/vec4.hpp>
//#include <glm/mat4x4.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//
//glm::mat4 GetMatrixUsingYawPitchRoll(const float yaw, const float pitch, const float roll) {
//    
//    glm::mat4 R=glm::mat4(1);
//    
//    R = glm::rotate(R, roll, glm::vec3(0,0,1));
//    R = glm::rotate(R,  yaw, glm::vec3(0,1,0));
//    R = glm::rotate(R, pitch, glm::vec3(1,0,0));
//    
//    return R;
//}
//
//
//
//int main(){
//    using namespace physics;
//    using namespace std;
//    using namespace glm;
//    
//    vec3 position{80.0f, 20.0f, 60.0};
//    vec3 translation{0};
//    vec3 right{0};
//    
//    mat4 R = GetMatrixUsingYawPitchRoll(0.0f,0.0f,0.0f);
//    position += translation;
//    translation= vec3(0);
//    
//    
//    
//    vec3 look = vec3(R*glm::vec4(0,0,1,0));
//    vec3 tgt  = position+look;
//    vec3 up   = glm::vec3(R*glm::vec4(0,1,0,0));
//    right = glm::cross(look, up);
//    
//    cout << "rotation:" << endl;
//    for(int i = 0; i < 4; i++){
//        for(int j = 0; j < 4; j++){
//            cout << R[i][j] << " ";
//        }
//        cout << endl;
//    }
//    
//    cout << "\nposition: " << Vector{position.x, position.y, position.z} << endl;
//    cout << "look: " << Vector{look.x, look.y, look.z} << endl;
//    cout << "tgt: " << Vector{tgt.x, tgt.y, tgt.z} << endl;
//    cout << "up: " << Vector{up.x, up.y, up.z} << endl;
//    cout << "right: " << Vector{right.x, right.y, right.z} << endl;
//    
//    return 0;
//}

