#ifndef PHYSICS_ENGINE_CONTACTS
#define PHYSICS_ENGINE_CONTACTS
#include "core.h"
#include "Particle.h"
#include <Vector>
#include <algorithm>
#include <queue>

namespace physics {

	class Contact {
		friend class ContactResolver;
		friend struct MostSevereContact;
	public:
		Particle* particle[2];
		Vector	  movement[2];
		Vector contactNormal;
		real resitution;
		real penetration;

	protected:
		void resolve(real duration) {
			resolveVelocity(duration);
			resolveInterpenetration(duration);
		}

		real separatingVelocity() const {
			Vector v1 = particle[0]->velocity();
            Vector v2 = !particle[1] ? Vector{0, 0, 0} : particle[0]->velocity();
			Vector n = contactNormal;

			return  (v1 - v2).dot(n);
		}

	private:
		void resolveVelocity(real duration) {
			real t = duration;
			Particle* pa = particle[0];
			Particle* pb = particle[1];
			const Vector& va = pa->velocity();
			const Vector& n = contactNormal;
			real ima = pa->inverseMass();
			real imb = pb == nullptr ? 0 : pb->inverseMass();

			real sv = separatingVelocity();
			real c = resitution;

			if (sv > 0)  return;	// particles are moving apart no need for resolution

			real svi = -c * sv;

			Vector vDueToAcc = pa->acceleration();
			if (pb) vDueToAcc -= pb->acceleration();
			real svDueToAcc = vDueToAcc.dot(n) * t;

			if (svDueToAcc < 0) {	// if closing velocity due to acceleration, remove it from 
				svi += c * svDueToAcc;
			}

			real dv = svi - sv;

			real totalImass = ima + imb;

			if (totalImass <= 0) return;	// infinite mass, impulses have no effect

			real impluse = dv / totalImass;

			Vector implusePerImass = n * impluse;

			pa->velocity(va + implusePerImass * ima);

			if (pb) {
				Vector vb = pb->velocity();
				pb->velocity(vb + implusePerImass * imb);
			}
		}

		void resolveInterpenetration(real duration) {
			Particle* pa = particle[0];
			Particle* pb = particle[1];
			const Vector& n = contactNormal;
			real d = penetration;
			real ima = pa->inverseMass();
			real imb = pb == nullptr ? 0 : pb->inverseMass();
			real totalIMass = ima + imb;

			if (d <= 0 || totalIMass <= 0) return;

			Vector movePerImass = n * (d / totalIMass);

			movement[0] = movePerImass * ima;
			pa->position(pa->position() + movement[0]);

			if (pb) {
				movement[1] = movePerImass * -imb;
				pb->position(pb->position() + movement[1]);
			}else {
				movement[0].clear();
			}
			


		}
	};

	struct MostSevereContact {

		bool operator()(const Contact& ca, const Contact& cb) {
			real svA = ca.separatingVelocity();
			real svB = cb.separatingVelocity();
			return svA < svB && (svA < 0 || ca.penetration > 0);
		}
	};
    
    using ContactsByPriority =  std::priority_queue<Contact, std::vector<Contact>, MostSevereContact>;

	class ContactResolver {
	protected:
		unsigned int _iterations;
		unsigned int _iterationsUsed;

	public:
		ContactResolver(int itr) : _iterations(itr) {

		}

		void iterations(const int iterations) {
			_iterations = iterations;
		}

		void resolveContacts(std::vector<Contact> contacts, real duration) {
			size_t n = contacts.size();
			_iterationsUsed = 0;

			while (_iterationsUsed < _iterations) {
				real maxSv = REAL_MAX;
				int maxIndex = n;
				for (int i = 0; i < n; i++) {
					Contact& contact = contacts[i];
					real sv = contact.separatingVelocity();
					if (sv < maxSv && (sv < 0 || contact.penetration > 0)) {	// use a list sorted by separating velocity and penetration
						maxSv = sv;
						maxIndex = i;
					}
				}

				if (maxIndex == n) break;	// nothing to resovle
				Contact& currentContact = contacts[maxIndex];
				currentContact.resolve(duration);
			//	updateInterpenetrations(contacts, currentContact);

				_iterationsUsed++;
			}
		}

	protected:
		void updateInterpenetrations(std::vector<Contact>& contacts, Contact currentContact) {
			size_t n = contacts.size();
			Vector* move = currentContact.movement;
			for (int i = 0; i < n; i++){
				Contact& otherContact = contacts[i];
				if (otherContact.particle[0] == currentContact.particle[0]){
					otherContact.penetration -= move[0].dot(otherContact.contactNormal);
				}
				else if (otherContact.particle[0] == currentContact.particle[1]){
					otherContact.penetration -= move[1].dot(otherContact.contactNormal);
				}

				if (otherContact.particle[1]){
					if (otherContact.particle[1] == currentContact.particle[0]){
						otherContact.penetration += move[0].dot(otherContact.contactNormal);
					}
					else if (otherContact.particle[1] == currentContact.particle[1]){
						otherContact.penetration += move[1].dot(otherContact.contactNormal);
					}
				}
			}
		}
	};

	class ContactGenerator {
	public:
		virtual void add(std::vector<Contact>& contacts, int limit) const = 0;
	};
}

#endif // PHYSICS_ENGINE_CONTACTS

