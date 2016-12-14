//
//  Matrix3.h
//  opengl
//
//  Created by Josiah Ebhomenye on 14/12/2016.
//  Copyright (c) 2016 Josiah Ebhomenye. All rights reserved.
//

#ifndef opengl_Matrix3_h
#define opengl_Matrix3_h

namespace physics{

    class Matrix3{
    private:
        union{
            struct{
                real a, b, c;
                real d, e, f;
                real g, h, i;
            };
            real cells[9];
        };
    public:
        Matrix3(){
            for(int i = 0; i < 9; i++){
                cells[i] = 0;
            }
        }
        
        Matrix3(std::initializer_list<std::initializer_list<real>> values){
            auto row = values.begin();
            
            for(int i = 0; i < 3; i++){
                if(row->size() != 3) throw "invalid no. of columns";
                auto col = row->begin();
                for(int j = 0; j < 3; j++, col++){
                    cells [i * 3 + j] = *col;
                }
                row++;
                
            }
        }
        
        Matrix3 operator*(const Matrix3& m){
            return {
                {(a * m.a + b * m.d + c * m.g), (a * m.b + b * m.e + c * m.h), (a * m.c + b * m.f + c * m.i)},
                {(d * m.a + e * m.d + f * m.g), (d * m.b + e * m.e + f * m.h), (d * m.c + e * m.f + f * m.i)},
                {(g * m.a + h * m.d + i * m.g), (g * m.b + h * m.e + i * m.h), (g * m.c + h * m.f + i * m.i)}
            };
        }
        
        Matrix3& operator*=(const Matrix3& m){
            real t1, t2, t3;
            t1 = (a * m.a + b * m.d + c * m.g);
            t2 = (a * m.b + b * m.e + c * m.h);
            t3 = (a * m.c + b * m.f + c * m.i);
            
            a = t1; b = t2; c = t3;
            
            t1 = (d * m.a + e * m.d + f * m.g);
            t2 = (d * m.b + e * m.e + f * m.h);
            t3 = (d * m.c + e * m.f + f * m.i);
            
            d = t1; e = t2; f = t3;
            
            t1 = (g * m.a + h * m.d + i * m.g);
            t2 = (g * m.b + h * m.e + i * m.h);
            t3 = (g * m.c + h * m.f + i * m.i);
            
            g = t1; h = t2, i = t3;
            
            return *this;
        }
        
        Vector operator*(const Vector& v) const {
            return Vector{
                a * v.x + b * v.y + c * v.z,
                d * v.x + e * v.y + f * v.z,
                g * v.x + h * v.y + i * v.z
            };
        }
        
        Vector transform(const Vector& v){
            return (*this) * v;
        }
        
        operator real() const{
            return (a * e * i) + (d * h * c) + (g * b * f) - (a * h * f) - (g * e * c) - (d * b * i);
        }
        
        real determinant() const{
            return (real)*this;
        }
        
        Matrix3 inverse() const{
            Matrix3 res;
            res.inverseOf(*this);
            return res;
        }
        
        void invert(){
            inverseOf(*this);
        }
        
        void inverseOf(const Matrix3& m1){
            real d = m1.determinant();
            
            if(d == real(0)){
                throw "no inverse";
            }
            real di = real(1.0)/d;
            
            real t1 = m1.a * m1.e;
            real t2 = m1.a * m1.f;
            real t3 = m1.b * m1.d;
            real t4 = m1.c * m1.d;
            real t5 = m1.b * m1.g;
            real t6 = m1.c * m1.g;
            
            a = (m1.e * m1.i - m1.f* m1.h) * di;
            b = (m1.c * m1.h - m1.b * m1.i) * di;
            c = (m1.b * m1.f - m1.c * m1.e) * di;
            d = (m1.f * m1.g - m1.d * m1.i) * di;
            e = (m1.a * m1.i - t6) * di;
            f = (t4 - t2) * di;
            g = (m1.d * m1.h - m1.e * m1.g) * di;
            h = (t5 - m1.a * m1.h) * di;
            i = (t1 - t3) * di;
        }
    };
    
}


#endif
