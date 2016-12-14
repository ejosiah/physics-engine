//
//  Matrix4.h
//  opengl
//
//  Created by Josiah Ebhomenye on 14/12/2016.
//  Copyright (c) 2016 Josiah Ebhomenye. All rights reserved.
//

#ifndef opengl_Matrix4_h
#define opengl_Matrix4_h

#include "vector.h"
#include "Quarternion.h"

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
            return Vector{
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
			return -data[8] * data[5] * data[2] +
				data[4] * data[9] * data[2] +
				data[8] * data[1] * data[6] -
				data[0] * data[9] * data[6] -
				data[4] * data[1] * data[10] +
				data[0] * data[5] * data[10];
		};

		real determinant() const {
			return (real)*this;
		}
		
		void setOrientationAndPos(const Quarternion& q, const Vector& pos)
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
			real det = (real)*this;
			if (det == 0) return;
			det = ((real)1.0) / det;

			data[0] = (-m.data[9] * m.data[6] + m.data[5] * m.data[10])*det;
			data[4] = (m.data[8] * m.data[6] - m.data[4] * m.data[10])*det;
			data[8] = (-m.data[8] * m.data[5] + m.data[4] * m.data[9])*det;

			data[1] = (m.data[9] * m.data[2] - m.data[1] * m.data[10])*det;
			data[5] = (-m.data[8] * m.data[2] + m.data[0] * m.data[10])*det;
			data[9] = (m.data[8] * m.data[1] - m.data[0] * m.data[9])*det;

			data[2] = (-m.data[5] * m.data[2] + m.data[1] * m.data[6])*det;
			data[6] = (+m.data[4] * m.data[2] - m.data[0] * m.data[6])*det;
			data[10] = (-m.data[4] * m.data[1] + m.data[0] * m.data[5])*det;

			data[3] = (m.data[9] * m.data[6] * m.data[3]
				- m.data[5] * m.data[10] * m.data[3]
				- m.data[9] * m.data[2] * m.data[7]
				+ m.data[1] * m.data[10] * m.data[7]
				+ m.data[5] * m.data[2] * m.data[11]
				- m.data[1] * m.data[6] * m.data[11])*det;
			data[7] = (-m.data[8] * m.data[6] * m.data[3]
				+ m.data[4] * m.data[10] * m.data[3]
				+ m.data[8] * m.data[2] * m.data[7]
				- m.data[0] * m.data[10] * m.data[7]
				- m.data[4] * m.data[2] * m.data[11]
				+ m.data[0] * m.data[6] * m.data[11])*det;
			data[11] = (m.data[8] * m.data[5] * m.data[3]
				- m.data[4] * m.data[9] * m.data[3]
				- m.data[8] * m.data[1] * m.data[7]
				+ m.data[0] * m.data[9] * m.data[7]
				+ m.data[4] * m.data[1] * m.data[11]
				- m.data[0] * m.data[5] * m.data[11])*det;
		}

    };
    
}

#endif
