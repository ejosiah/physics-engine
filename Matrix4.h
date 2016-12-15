//
//  Matrix4.h
//  opengl
//
//  Created by Josiah Ebhomenye on 14/12/2016.
//  Copyright (c) 2016 Josiah Ebhomenye. All rights reserved.
//

#ifndef opengl_Matrix4_h
#define opengl_Matrix4_h

#include <iostream>
#include "vector.h"
#include "Quaternion.h"

namespace physics{
    
    class Matrix4{
    private:
        union{
            struct{
                real a, b, c, d;
                real e, f, g, h;
                real i, j, k, l;
                real m, n, o, p;
            };
            real data[16];
        };

	private:
		class _2nd {
		private:
			int i, offset;
			const real* data;

		public:
			_2nd(int i, int off, const real* d) : i(i), offset(off), data(d) {}

			real operator[](int j) {
				return data[i * offset + j];
			}

		};

    public:
		Matrix4() {
			b = c = d = e = g
			= h = i = j = l 
			= m = n = o = 0;
			a = f = k = p = 1;
		}

        Matrix4(const real* d){
            for(int i = 0; i < 16; i++){
                data[i] = d[i];
            }
        }
        
        Matrix4(std::initializer_list<std::initializer_list<real>> values){
            auto row = values.begin();
            
            for(int i = 0; i < 4; i++){
                if(row->size() != 4) throw "invalid no. of columns";
                auto col = row->begin();
                for(int j = 0; j < 4; j++, col++){
                    data[i * 4 + j] = *col;
                }
                row++;
                
            }
        }

		void setDiagonal(real d0, real d1, real d2)
		{
			a = d0;
			f = d1;
			k = d2;
		}
        
        Matrix4 operator*(const Matrix4& m1){
            return {
                {(a * m1.a + b * m1.e + c * m1.i + d * m1.m), (a * m1.b + b * m1.f + c * m1.j + d * m1.n), (a * m1.c + b * m1.g + c * m1.k + d * m1.o), (a * m1.d + b * m1.h + c * m1.l + d * m1.p)},
                {(e * m1.a + f * m1.e + g * m1.i + h * m1.m), (e * m1.b + f * m1.f + g * m1.j + h * m1.n), (e * m1.c + f * m1.g + g * m1.k + h * m1.o), (e * m1.d + f * m1.h + g * m1.l + h * m1.p)},
                {(i * m1.a + j * m1.e + k * m1.i + l * m1.m), (i * m1.b + j * m1.f + k * m1.j + l * m1.n), (i * m1.c + j * m1.g + k * m1.k + l * m1.o), (i * m1.d + j * m1.h + k * m1.l + l * m1.p)},
                {(m * m1.a + n * m1.e + o * m1.i + p * m1.m), (m * m1.b + n * m1.f + o * m1.j + p * m1.n), (m * m1.c + n * m1.g + o * m1.k + p * m1.o), (m * m1.d + n * m1.h + o * m1.l + p * m1.p)}
                
            };
        }
        
        Matrix4& operator*=(const Matrix4& m1){
            real t1, t2, t3, t4;
            t1 = (a * m1.a + b * m1.e + c * m1.i + d * m1.m);
            t2 = (a * m1.b + b * m1.f + c * m1.j + d * m1.n);
            t3 = (a * m1.c + b * m1.g + c * m1.k + d * m1.o);
            t4 = (a * m1.d + b * m1.h + c * m1.l + d * m1.p);
            
            a = t1; b = t2; c = t3, d = t4;
            
            t1 = (e * m1.a + f * m1.e + g * m1.i + h * m1.m);
            t2 = (e * m1.b + f * m1.f + g * m1.j + h * m1.n);
            t3 = (e * m1.c + f * m1.g + g * m1.k + h * m1.o);
            t4 = (e * m1.d + f * m1.h + g * m1.l + h * m1.p);
            
            e = t1; f = t2; g = t3, h = t4;
            
            t1 = (i * m1.a + j * m1.e + k * m1.i + l * m1.m);
            t2 = (i * m1.b + j * m1.f + k * m1.j + l * m1.n);
            t3 = (i * m1.c + j * m1.g + k * m1.k + l * m1.o);
            t4 = (i * m1.d + j * m1.h + k * m1.l + l * m1.p);
            
            i = t1; j = t2; k = t3, l = t4;
            
            t1 = (m * m1.a + n * m1.e + o * m1.i + p * m1.m);
            t2 = (m * m1.b + n * m1.f + o * m1.j + p * m1.n);
            t3 = (m * m1.c + n * m1.g + o * m1.k + p * m1.o);
            t4 = (m * m1.d + n * m1.h + o * m1.l + p * m1.p);
            
            m = t1; n = t2; o = t3, p = t4;
            
            return *this;
        }
        
        Vector operator*(const Vector& v) const {
            return {
                a * v.x + b * v.y + c * v.z + d,
                e * v.x + f * v.y + g * v.z + h,
                i * v.x + j * v.y + k * v.z + l,
            };
        }
        
        Vector transform(const Vector& v){
            return (*this) * v;
        }

		Vector transformDirection(const Vector &v) const {
			return Vector{
				a * v.x + b * v.y + c,
				e * v.x + f * v.y + g,
				i * v.x + j * v.y + k
			};
		}

		Vector transformInverseDirection(const Vector& v) const {
			return Vector{
				a * v.x + e * v.y + i * v.z ,
				b * v.x + f * v.y + j * v.z ,
				c * v.x + g * v.y + k * v.z 
			};
		}

		Vector transformInverse(const Vector& v) const {
			Vector v1 = v - Vector{ d, h, l };
			return Vector{
				a * v1.x + e * v1.y + i * v1.z ,
				b * v1.x + f * v1.y + j * v1.z ,
				c * v1.x + g * v1.y + k * v1.z
			};
		}

		_2nd operator[](int i) const {
			return _2nd(i, 4, data);
		}

		Vector axisVector(int i) const {
			return Vector{ data[i], data[i + 4], data[i + 8] };
		}

        
        operator real*() {
            return data;
        }

		operator real() const {
			return         
				-i*f*c +
				e*j*c +
				i*b*g -
				a*j*g -
				e*b*k +
				a*f*k;
		};

		real determinant() const {
			return (real)*this;
		}
		
		void setOrientationAndPos(const Quaternion& q, const Vector& pos)
		{
			a = 1 - (2 * q.j*q.j + 2 * q.k*q.k);
			b = 2 * q.i*q.j + 2 * q.k*q.r;
			c = 2 * q.i*q.k - 2 * q.j*q.r;
			d = pos.x;

			e = 2 * q.i*q.j - 2 * q.k*q.r;
			f = 1 - (2 * q.i*q.i + 2 * q.k*q.k);
			g = 2 * q.j*q.k + 2 * q.i*q.r;
			h = pos.y;

			i = 2 * q.i*q.k + 2 * q.j*q.r;
			j = 2 * q.j*q.k - 2 * q.i*q.r;
			k = 1 - (2 * q.i*q.i + 2 * q.j*q.j);
			l = pos.z;
		}

		void inverseOf(const Matrix4& m) {
			real det = determinant();
			if (det == 0) return;
			det = ((real)1.0) / det;

			a = (-m.j*m.g + m.f*m.k)*det;
			e = (m.i*m.g - m.e*m.k)*det;
			i = (-m.i*m.f + m.e*m.j)*det;

			b = (m.j*m.c - m.b*m.k)*det;
			f = (-m.i*m.c + m.a*m.k)*det;
			j = (m.i*m.b - m.a*m.j)*det;

			c = (-m.f*m.c + m.b*m.g)*det;
			g = (+m.e*m.c - m.a*m.g)*det;
			k = (-m.e*m.b + m.a*m.f)*det;

			d = (m.j*m.g*m.d
				- m.f*m.k*m.d
				- m.j*m.c*m.h
				+ m.b*m.k*m.h
				+ m.f*m.c*m.l
				- m.b*m.g*m.l)*det;
			h = (-m.i*m.g*m.d
				+ m.e*m.k*m.d
				+ m.i*m.c*m.h
				- m.a*m.k*m.h
				- m.e*m.c*m.l
				+ m.a*m.g*m.l)*det;
			l = (m.i*m.f*m.d
				- m.e*m.j*m.d
				- m.i*m.b*m.h
				+ m.a*m.j*m.h
				+ m.e*m.b*m.l
				- m.a*m.f*m.l)*det;

		}

		friend std::ostream& operator<<(std::ostream& out, const Matrix4 m) {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					out << m[i][j] << " ";
				}
				out << std::endl;
			}
			return out;
		}

    };
    
}

#endif
