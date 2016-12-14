//
//  Matrix3X4.h
//  opengl
//
//  Created by Josiah Ebhomenye on 14/12/2016.
//  Copyright (c) 2016 Josiah Ebhomenye. All rights reserved.
//

#ifndef opengl_Matrix3X4_h
#define opengl_Matrix3X4_h

namespace physics{
    
    class Matrix3X4{
    private:
        union{
            struct{
                real a, b, c, d;
                real e, f, g, h;
                real i, j, k, l;
            };
            real cells[12];
        };
    public:
        Matrix3X4(){
            for(int i = 0; i < 12; i++){
                cells[i] = 0;
            }
        }
        
        Matrix3X4(std::initializer_list<std::initializer_list<real>> values){
            auto row = values.begin();
            
            for(int i = 0; i < 3; i++){
                if(row->size() != 4) throw "invalid no. of columns";
                auto col = row->begin();
                for(int j = 0; j < 4; j++, col++){
                    cells [i * 4 + j] = *col;
                }
                row++;
                
            }
        }
        
        Vector operator*(const Vector& v) const {
            auto self = *this;
            return Vector{
                a * v.x + b * v.y + c * v.z + d,
                e * v.x + f * v.y + g * v.z + h,
                i * v.x + j * v.y + k * v.z + l,
            };
        }
        
        Vector transform(const Vector& v){
            return (*this) * v;
        }
    };
    
}

#endif
