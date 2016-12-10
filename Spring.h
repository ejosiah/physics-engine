//
//  Spring.h
//  opengl
//
//  Created by Josiah Ebhomenye on 05/12/2016.
//  Copyright (c) 2016 Josiah Ebhomenye. All rights reserved.
//

#ifndef opengl_Spring_h
#define opengl_Spring_h

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#else
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/glext.h>
#pragma comment(lib, "glew32.lib")
#endif
#include "precision.h"
#include <vector>

class Spring{
private:
    int cycles;
    real r1;
    real r2;
    int length;
    int nu;
    int nv;
    std::vector<Vector> faces;
    std::vector<Vector> normals;
    
    
public:
    Spring(int c, real r1, real r2, int l, int nu, int nv)
    :cycles(c), r1(r1)
    , r2(r2), length(l)
    , nu(nu), nv(nv){
        
        real u,v,du,dv;
        
        du = cycles * TWOPI / (real)nu;
        dv = TWOPI / (real)nv;
        
        for (int i=0;i<nu;i++) {
            
            Vector q[4],n[4];
            u = i * du;
            for (int j=0;j<nv;j++) {
                v = j * dv;
                q[0] = eval(u,v);
                n[0] = calcNormal(q[0],eval(u+du/10,v),eval(u,v+dv/10));
                q[1] = eval(u+du,v);
                n[1] = calcNormal(q[1],eval(u+du+du/10,v),eval(u+du,v+dv/10));
                q[2] = eval(u+du,v+dv);
                n[2] = calcNormal(q[2],eval(u+du+du/10,v+dv),eval(u+du,v+dv+dv/10));
                q[3] = eval(u,v+dv);
                n[3] = calcNormal(q[3],eval(u+du/10,v+dv),eval(u,v+dv+dv/10));
                
                faces.push_back(q[0]);
                faces.push_back(q[1]);
                faces.push_back(q[2]);
                faces.push_back(q[3]);
                normals.push_back(n[0]);
                normals.push_back(n[1]);
                normals.push_back(n[2]);
                normals.push_back(n[3]);
                
            }
        }
        
    }
    
    void draw(){
        auto face = faces.begin();
        auto normal = normals.begin();
        
        for (int i=0;i<nu;i++) {
            for (int j=0;j<nv;j++) {
                
                /* Write the facet out in your favorite format */
                glBegin(GL_QUADS);
                for (int k=0;k<4;k++){
                    glVertex3fv(*(face++));
                    glNormal3fv(*(normal++));
                }
                glEnd();
            
            }
        }
    }
    
    Vector eval(real u,real v)
    {
        Vector q;
        
        q.x = (1 - r1 * real_cos(v)) * real_cos(u);
        q.y = (1 - r1 * real_cos(v)) * real_sin(u);
        q.z = r2 * (real_sin(v) + u*length/PI);
        
        return q;
    }
    
    Vector calcNormal(Vector p,Vector p1,Vector p2)
    {
        
        Vector pa = p1 - p;
        Vector pb = p2 - p;
        
        pa.normalize();
        pb.normalize();
        
        Vector n = pb * pa;
        
        return n.normalize();
        
    }
    
};

#endif
