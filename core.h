//
//  core.h
//  opengl
//
//  Created by Josiah Ebhomenye on 03/12/2016.
//  Copyright (c) 2016 Josiah Ebhomenye. All rights reserved.
//

#ifndef openglcoreh
#define openglcoreh
#include <cmath>
#include <initializer_list>
#include <iostream>
#include <stdexcept>
#include <glm/vec4.hpp>

#include "precision.h"

namespace physics {
    
    class Vector{
    public:
        union{
            struct { real x, y, z, w; };
            real data[4];
        };
        
    public:
        Vector(real x = 0, real y = 0, real z = 0, real w = 0):
        x(x), y(y), z(z), w(w){}
        
        Vector(std::initializer_list<real> v){
            auto itr = v.begin();
            x = *(itr++);
            y = *(itr++);
            z = *itr;
            w = 0;
        }

		real squreLength() const {
			return x * x + y * y + z * z;
		}
    
        real length() const{
            return real_sqrt(x * x + y * y + z * z);
        }
    
        real  operator()(){
            return length();
        }
        
        Vector& operator+=(const Vector& v){
            x += v.x;
            y += v.y;
            z += v.z;
            return *this;
        }
        
        Vector operator+(const Vector& v) const{
            return Vector(x + v.x, y + v.y, z + v.z);
        }
        
        Vector& operator-=(const Vector& v){
            x -= v.x;
            y -= v.y;
            z -= v.z;
            
            return *this;
        }
        
        
        Vector operator-(const Vector& v) const{
            return Vector(x - v.x, y - v.y, z - v.z);
        }
        
        Vector& operator*=(real s){
            x *= s;
            y *= s;
            z *= s;
            return *this;
        }
        
        Vector operator*(const real s) const{
            return Vector(x * s, y * s, z * s);
        }
        
        Vector& addScaled(const Vector& v, real s){
            x += v.x * s;
            y += v.y * s;
            z += v.z * s;
            return *this;
        }
        
        bool operator==(const Vector& v){
            if (this == &v) {
                return true;
            }
            
            return real_equals(x, v.x) && real_equals(y, v.y) && real_equals(z, v.z);
        }
        
        bool operator<=(const Vector& v){
            if(this == &v){
                return true;
            }
            
            return x <= v.x && y <= v.y && z <= v.z;
        }
    
        Vector& normalize(){
            real l = length();
        
            *this *= 1/l;
            
            return *this;
        }
        
        real operator[](const char key){
            switch(key){
                case 'x': return x;
                case 'y': return y;
                case 'z': return z;
                default: throw "Illegal argument";
            }
        }
        
        real dot(const Vector& v) const{
            return x * v.x + y * v.y + z * v.z;
        }
        
        Vector cross(const Vector& v) const{
            return Vector{
                y * v.z - z * v.y,
                z * v.x - x * v.z,
                x * v.y - y * v.x
            };
        }
        
        Vector& crossProductUpdate(const Vector& v){
            *this = cross(v);
            
            return *this;
        }
        
        real operator%(const Vector& v){
            return dot(v);
        }
        
        Vector operator*(const Vector& v) const{
            return cross(v);
        }
        
        Vector& operator*=(const Vector& v){
            return crossProductUpdate(v);
        }
        
        void invert(){
            x *= -1;
            y *= -1;
            z *= -1;
        }
        
        Vector& operator-(){
            invert();
            return *this;
        }
        
        operator real*(){
            return data;
        }
        
        void clear(){
            x = 0;
            y = 0;
            z = 0;
            w = 0;
        }
        
        static real angleBetween(Vector a, Vector b){
            a.normalize();
            b.normalize();
            real dot = a.dot(b);
            return real_acos(dot)/DEG_TO_RAD;
            
        }
        
        friend std::ostream& operator<<(std::ostream& out, const Vector& v){
            out << "v(" << v.x << ", " << v.y << ", " << v.z;
            if(!(fabs(v.w - 0) < epsilon)) out << "," << v.w;
            out << ")";
            return out;
        }
        
        operator glm::vec4(){
            return glm::vec4{x, y, z, w};
        }
    };
    
    using Position = Vector;
    using Velocity = Vector;
    using Acceleration = Vector;
    using Force = Vector;
    
    template<size_t N, size_t M>
    class Matrix{
    private:
        real cell[N][M];
        
    public:
        Matrix(){
            for(int i = 0; i < N; i++){
                for(int j =0; j < M; j++){
                    cell[i][j] = 0;
                }
            }
        };
        
        Matrix(std::initializer_list<std::initializer_list<real>> values){
            if(values.size() != N) throw "invaid no of rows";
            auto row = values.begin();

            for(int i = 0; i < N; i++){
                if(row->size() != M) throw "invalid no. of columns";
                auto col = row->begin();
                for(int j = 0; j < M; j++, col++){
                    cell[i][j] = *col;
                }
                row++;
               
            }
        }
        
        Matrix<N, M>& operator*=(const real s){
            for(int i = 0; i < N; i++){
                for(int j = 0; j < M; j++){
                    cell[i][j] *= s;
                }
            }
            return *this;
        }
        
        Matrix<N, M> operator*(const real s){
            Matrix<N, M> res;
            for(int i = 0; i < N; i++){
                for(int j = 0; j < M; j++){
                    res[i][j] = cell[i][j] * s;
                }
            }
            return res;
            
        }
        
        template<size_t N1, size_t M1>
        Matrix<N, M1> operator*(const Matrix<N1, M1>& m){
            Matrix<N, M1> res;
            if (M != N1) {
                throw std::logic_error("dont product is not difined ");
            }
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < M1 ; j++) {
                    for (int k = 0; k < N1; k++) {
                        res[i][j] += cell[i][k] * m[k][j];;
                    }
                }
            }
            return res;
        }
        
        real* operator[](int i){
            return cell[i];
        }
        
        const real* operator[](int i) const{
            return cell[i];
        }
        
        Matrix<4, 1> toMatrix(const Vector& v){
            return Matrix<4, 1>{{v.x}, {v.y}, {v.z}, {v.w}};
        }
        
        operator Vector(){
            return Vector{cell[0][0], cell[0][1], cell[0][2], cell[0][3]};
        }
        
        Vector operator*(const Vector& v){
            Matrix<4, 1> vm = toMatrix(v);
            Matrix<N, M> me = *this;
            
            return me * vm;
        }
        
        
        friend std::ostream& operator<<(std::ostream& out, const Matrix<N, M>& m){
            for(int i = 0; i < N; i++){
                for(int j = 0; j < M; j++){
                    out << m.cell[i][j] << " ";
                }
                out << std::endl;
            }
            return out;
        }
        
        // return the determinant of this matrix
        operator real() {
            if (N != M) {
                throw std::logic_error("Not a square matrix");
            }
            if (N == 2) {
                return (*this)[0][0] * (*this)[1][1] - (*this)[0][1] * (*this)[1][0];
            }
            real result;
            Matrix<N - 1, N - 1> minor;
            for (int i = 0; i < N; i++) { // for each element in the top row
                int j = 0, k = 0;
                for (int p = 0; p < N - 1; p++) { // for every other row
                    for (int q = 0; q < N; q++) { // scan each element for current row
                        if (q == i) { // skip elements in same colunm as element [0][i]
                            continue;
                        }
                        minor[j][k] = (*this)[p][q];
                        k++;
                    }
                    j++;
                }
                if (i % 2 != 0) { // every second element
                    result -= (*this)[0][i] * T(minor);
                }
                else {
                    result += (*this)[0][i] * T(minor);
                }
                
            }
            return result;
        }
    };
    
    using Matrix2 = Matrix<2, 2>;
    using Matrix3 = Matrix<3, 3>;
    using Matrix4 = Matrix<4, 4>;
    using Vector_Matrix4 = Matrix<4, 1>;
    
    Matrix3 IdentityMatrix3{
        {1, 0, 0},
        {0, 1, 0},
        {0, 1, 0}
    };
    
    Matrix4 IdentityMatrix4{
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };
    
    Matrix4 rotate(const real angle, const Vector& axis){
        real s = real_sin(angle * DEG_TO_RAD);
        real c = real_cos(angle * DEG_TO_RAD);
        real t = 1 - c;
        real x = axis.x;
        real y = axis.y;
        real z = axis.z;
        
        return {
            {t * x * x + c,      t * x * y + s * z,  t * x * z - s * y,  0},
            {t * x * y - s * z,  t * y * y + c,      t * y * z + s * x,  0},
            {t * x * z + s * y,  t * y * z - s * x,  t * z * z + c,      0},
            {        0,                  0,                   0,         1}
        };
    }
    
    Matrix4 rotateZ(const real angle){
        return rotate(angle, {0, 0, 1});
    }
    
    Matrix4 rotateX(const real angle){
        return rotate(angle, {1, 0, 0});
    }
    
    Matrix4 rotateY(const real angle){
        return rotate(angle, {0, 1, 0});
    }
    
    
    
    Matrix4 RotationMatrix(const real yaw, const real pitch, const real roll){
        return rotateY(yaw) * rotateX(pitch) * rotateZ(roll);
    }
}

#endif
