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
            real data[9];
        };
    public:
        Matrix3(){
            for(int i = 0; i < 9; i++){
                data[i] = 0;
            }
        }
        
        Matrix3(std::initializer_list<std::initializer_list<real>> values){
            auto row = values.begin();
            
            for(int i = 0; i < 3; i++){
                if(row->size() != 3) throw "invalid no. of columns";
                auto col = row->begin();
                for(int j = 0; j < 3; j++, col++){
                    data [i * 3 + j] = *col;
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
        
        Matrix3& invert(){
            inverseOf(*this);
			return *this;
        }

		Vector getRowVector(int i) const {
			return { data[i * 3], data[i * 3 + 1], data[i * 3 + 2] };
		}

		Vector getAxisVector(int i) const {
			return{ data[i], data[i + 3], data[i + 6] };
		}
        
        void inverseOf(const Matrix3& m){
			real t4 = m.a * m.e;
			real t6 = m.a * m.f;
			real t8 = m.b * m.d;
			real t10 = m.c * m.d;
			real t12 = m.b * m.g;
			real t14 = m.c * m.g;

			// Calculate the determinant
			real t16 = (t4*m.i - t6*m.h - t8*m.i +
				t10*m.h + t12*m.f - t14*m.e);

			// Make sure the determinant is non-zero.
			if (t16 == (real)0.0f) return;
			real t17 = 1 / t16;

			a = (m.e * m.i - m.f * m.h)*t17;
			b = -(m.b * m.i - m.c * m.h)*t17;
			c = (m.b * m.f - m.c * m.e)*t17;
			d = -(m.d * m.i - m.f * m.g)*t17;
			e = (m.a * m.i - t14)*t17;
			f = -(t6 - t10)*t17;
			g = (m.d * m.h - m.e * m.g)*t17;
			h = -(m.a * m.h - t12)*t17;
			i = (t4 - t8)*t17;

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

		real operator[](const int i) const {
			return data[i];
		}

		real& operator[](const int i) {
			return data[i];
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
				result.data[i] = a.data[i] * (1 - prop) + b.data[i] * prop;
			}
			return result;
		}

		friend std::ostream& operator<<(std::ostream& out, const Matrix3 m) {
			for (int i = 0; i < 9; i++) {
				if(i % 3 == 0) out << std::endl;
				out << m.data[i] << " ";
				
			}
			return out;
		}

    };
    
}


#endif
