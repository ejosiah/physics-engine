//
//  main.cpp
//  opengl
//
//  Created by Josiah Ebhomenye on 03/12/2016.
//  Copyright (c) 2016 Josiah Ebhomenye. All rights reserved.
//
/*
#include "RainingBalls.h"
#include <iostream>
#include "Timer.h"
#include "random.h"

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

#include "Matrix4.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

int main() {
	using namespace physics;
	using namespace std;
	using namespace glm;

	mat4 m{ 
		2.0f, 0.0f, -1.0f, 0.0f, 
		0.0f, 1.0f, 5.0f, 1.0f, 
		-1.0f, 3.0f, -2.0f, 0.0f, 
		0.0f, 0.0f, 0.0f, 1.0f };

	mat4 im = inverse(m);

	mat4 m2 = m * im;


	Matrix4 M = {
		{2, 0, -1, 0},
		{0, 1, 5, 1},
		{-1, 3, -2, 0},
		{0, 0, 0, 1}
	};

	Matrix4 IM;
	IM.inverseOf(M);

	Matrix4 M2 = M * IM;
    
    cout << "Mine:" << endl;
	cout << M << endl << endl;
    
     cout << "Mine: inverse" << endl;
	cout << IM << endl << endl;
    
     cout << "Mine: M * M-1" << endl;
	cout << M2 << endl << endl;
    
     cout << "glm: inverse" << endl;
    cout << Matrix4(glm::value_ptr(im)) << endl;

    cout << "glm m * m-1:" << endl;
	cout << Matrix4(glm::value_ptr(m2)) << endl;
    
    cout << M.determinant() << endl;
    cout << determinant(m) << endl;
	cin.get();
	return 0;
}