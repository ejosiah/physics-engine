#ifndef PHYSICS_RIGID_BODY_H
#define  PHYSICS_RIGID_BODY_H

#include "precision.h"
#include "core.h"

namespace physics {
    class RigidBody : Particle {
	protected:
        real motion;
        real _angularDamping;
        
        bool isAwake;
        bool canSleep;
        
		Velocity _angularVelocity;
        Acceleration _lastFrameAcceleration;
		Torque _accumulatedTorque;
        

		Quaternion _orientation;
		Matrix4 _transformMatrix;
		Matrix3 _inverseInertiaTensorWorld;
		Matrix3 _inverseInertiaTensor;
		

	protected:
        static void _transformInertiaTensor(Matrix3& iitWorld, const Quaternion& q, const Matrix3& iitBody, const Matrix4& rotmat) {
            real t4 = rotmat[0] * iitBody[0] +rotmat[1] * iitBody[3] + rotmat[2] * iitBody[6];
            real t9 = rotmat[0] * iitBody[1] + rotmat[1] * iitBody[4] + rotmat[2] * iitBody[7];
            real t14 = rotmat[0] * iitBody[2] + rotmat[1] * iitBody[5] + rotmat[2] * iitBody[8];
            real t28 = rotmat[4] * iitBody[0] + rotmat[5] * iitBody[3] + rotmat[6] * iitBody[6];
            real t33 = rotmat[4] * iitBody[1] + rotmat[5] * iitBody[4] + rotmat[6] * iitBody[7];
            real t38 = rotmat[4] * iitBody[2] + rotmat[5] * iitBody[5] + rotmat[6] * iitBody[8];
            real t52 = rotmat[8] * iitBody[0] + rotmat[9] * iitBody[3] + rotmat[10] * iitBody[6];
            real t57 = rotmat[8] * iitBody[1] + rotmat[9] * iitBody[4] + rotmat[10] * iitBody[7];
            real t62 = rotmat[8] * iitBody[2] + rotmat[9] * iitBody[5] + rotmat[10] * iitBody[8];
            
            iitWorld[0] = t4 * rotmat[0] + t9 * rotmat[1] + t14 * rotmat[2];
            iitWorld[1] = t4 * rotmat[4] + t9 * rotmat[5] + t14 * rotmat[6];
            iitWorld[2] = t4 * rotmat[8] + t9 * rotmat[9] + t14 * rotmat[10];
            iitWorld[3] = t28 * rotmat[0] + t33 * rotmat[1] + t38 * rotmat[2];
            iitWorld[4] = t28 * rotmat[4] + t33 * rotmat[5] + t38 * rotmat[6];
            iitWorld[5] = t28 * rotmat[8] + t33 * rotmat[9] + t38 * rotmat[10];
            iitWorld[6] = t52 * rotmat[0] + t57 * rotmat[1] + t62 * rotmat[2];
            iitWorld[7] = t52 * rotmat[4] + t57 * rotmat[5] + t62 * rotmat[6];
            iitWorld[8] = t52 * rotmat[8] + t57 * rotmat[9] + t62 * rotmat[10];
        }

    public:
        void calculateDerivedData() {
            _orientation.normailize();
            _transformMatrix.setOrientationAndPos(_orientation, _position);
            _transformInertiaTensor(_inverseInertiaTensorWorld, _orientation, _inverseInertiaTensor, _transformMatrix);
        }
        
		void addForce(const Force& force) {
			_accumulatedForce += force;
			isAwake = true;
		}

		virtual void clearAccumulators() override {
			_accumulatedForce.clear();
			_accumulatedTorque.clear();
		}

        void addForceAtPoint(const Force& force, const Vector& point){
            Vector pt = point - _position;
            
            _accumulatedForce += force;
            _accumulatedTorque += pt * force;
            
            isAwake = true;
        }

        void addForceAtBodyPoint(const Force& force, const Vector& point){
            Vector pt = pointInWorldSpace(point);
            addForceAtPoint(force, pt);
            isAwake = true;
        }
        
        Point pointInWorldSpace(const Point& point){
            return _transformMatrix.transform(point);
        }
        
        virtual void integrate(real time) override{
            if(!isAwake) return;
            
            _lastFrameAcceleration = _acceleration;
            _lastFrameAcceleration.addScaled(_accumulatedForce, _inverseMass);
            
            Acceleration angularAcc = _inverseInertiaTensorWorld.transform(_accumulatedTorque);
            
            _velocity.addScaled(_lastFrameAcceleration, time);
            _angularVelocity.addScaled(angularAcc, time);
            
            _velocity *= real_pow(_damping, time);
            _angularVelocity *= real_pow(_angularDamping, time);
            
            _position.addScaled(_velocity, time);
            _orientation.addScaled(_angularVelocity, time);
            
            calculateDerivedData();
            clearAccumulators();
            
        }

	};
}

#endif
