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

		void setDiagonal(real a, real b, real c){
			setInertiaTensorCoeffs(a, b, c);
		}


		void setInertiaTensorCoeffs(real ix, real iy, real iz,
			real ixy = 0, real ixz = 0, real iyz = 0){

			a = ix;
			b = d = -ixy;
			c = g = -ixz;
			e = iy;
			f = h = -iyz;
			i = iz;
		}

		void setBlockInertiaTensor(const Vector& halfSizes, real mass){
			Vector squares = halfSizes.compProduct(halfSizes);
			setInertiaTensorCoeffs(0.3f*mass*(squares.y + squares.z),
				0.3f*mass*(squares.x + squares.z),
				0.3f*mass*(squares.x + squares.y));
		}

		void setSkewSymmetric(const Vector v){
			a = e = i = 0;
			b = -v.z;
			c = v.y;
			d = v.z;
			f = -v.x;
			g = -v.y;
			h = v.x;
		}

		void setComponents(const Vector &compOne, const Vector &compTwo, const Vector &compThree){
			a = compOne.x;
			b = compTwo.x;
			c = compThree.x;
			d = compOne.y;
			e = compTwo.y;
			f = compThree.y;
			g = compOne.z;
			h = compTwo.z;
			i = compThree.z;

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
        
        Vector transform(const Vector& v) const{
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

		Vector getRowVector(int i) const {
			return { cells[i * 3], cells[i * 3 + 1], cells[i * 3 + 2] };
		}

		Vector getAxisVector(int i) const {
			return{ cells[i], cells[i + 3], cells[i + 6] };
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

		void transposeOf(const Matrix3 &m){
			a = m.a;
			b = m.d;
			c = m.g;
			d = m.b;
			e = m.e;
			f = m.h;
			g = m.c;
			h = m.f;
			i = m.i;
		}

		Matrix3 transpose() const {
			Matrix3 result;
			result.transposeOf(*this);
			return result;
		}

		Matrix3& operator*=(const real s){
			a *= s; b *= s; c *= s;
			d *= s; e *= s; f *= s;
			g *= s; h *= s; i *= s;

			return *this;
		}


		Matrix3& operator+=(const Matrix3 &o){
			a += o.a; b += o.b; c += o.c;
			d += o.d; e += o.e; f += o.f;
			g += o.g; h += o.h; i += o.i;

			return *this;
		}

		/**
		* Sets this matrix to be the rotation matrix corresponding to
		* the given quaternion.
		*/
		void setOrientation(const Quaternion &q){
			a = 1 - (2 * q.j*q.j + 2 * q.k*q.k);
			b = 2 * q.i*q.j + 2 * q.k*q.r;
			c = 2 * q.i*q.k - 2 * q.j*q.r;
			d = 2 * q.i*q.j - 2 * q.k*q.r;
			e = 1 - (2 * q.i*q.i + 2 * q.k*q.k);
			f = 2 * q.j*q.k + 2 * q.i*q.r;
			g = 2 * q.i*q.k + 2 * q.j*q.r;
			h = 2 * q.j*q.k - 2 * q.i*q.r;
			i = 1 - (2 * q.i*q.i + 2 * q.j*q.j);
		}

		static Matrix3 linearInterpolate(const Matrix3& a, const Matrix3& b, real prop) {
			Matrix3 result;
			for (unsigned i = 0; i < 9; i++) {
				result.cells[i] = a.cells[i] * (1 - prop) + b.cells[i] * prop;
			}
			return result;
		}

    };
    
}


#endif
