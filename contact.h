#pragma once
#include <vector>
#include "RigidBody.h"

namespace physics {

	struct PotentialContact {
		RigidBody* body[2];
	};

	using PotentialContacts = std::vector<PotentialContact>;

	template<class BV>
	class BVHNode {
	public:
		using BoundingVolume = BV;

		BVHNode* parent;
		BVHNode* left;
		BVHNode* right;

		BoundingVolume volume;
		RigidBody* body;

	public:
		BVHNode(BVHNode* parent, const BoundingVolume& volume, RigidBody* body = nullptr) 
			: parent(parent), volume(volume), body(body) {
			left = right = nullptr;
		}

		~BVHNode() {
			if (parent != nullptr) {
				BVHNode<BoundingVolume>* sibling;
				if (parent->left == this) {
					sibling = parent->right;
				}
				else {
					sibling = parent->left;
				}

				parent->volume = sibling->volume;
				parent->body = sibling->body;
				parent->left = sibling->left;
				parent->right = sibling->right;

				sibling->parent = nullptr;
				sibling->body = nullptr;
				sibling->left = nullptr;
				sibling->right = nullptr;
				delete sibling;

				parent->recalculateboundingVolume();
			}
		}

		bool isLeaf() const { return body != nullptr; }

		int getPotentialContacts(PotentialContacts& contacts, int limit) {
			if (isLeaf() || limit = 0) return;

			return left->getPotentialContactsWith(right, contacts, limit);
		}

		void insert(RigidBody* newBody, const BoundingVolume& newVolume) {
			if (isLeaf()) {
				left = new BVHNode<BoundingVolume>(this, volume, body);
				right = new BVHNode<BoundingVolume>(this, newVolume, newBody);
				this->body = nullptr;
				recalculateboundingVolume();
			}
			else {
				if (left->volume.getGrowth(newVolume) < right->volume.getGrowth(newVolume)) {
					left->insert(newBody, newVolume);
				}
				else {
					right->insert(newBody, newVolume);
				}
			}
		}

	protected:
		bool overlaps(const BVHNode<BoundingVolume>& other) const {
			return volume->overlaps(other.volume);
		}

		int getPotentialContactsWith(const BVHNode<BoundingVolume>& other, PotentialContacts& contacts, int limit) const {
			if (!overlaps(other) || limit == 0) return;

			if (isLeaf() && other.isLeaf()) {
				PotentialContact contact;
				contact.body[0] = body;
				contact.body[1] = other.body;
			}


			// TODO based on binary tree right child should be the bigger child
			if (other.isLeaf() || (!isLeaf() && volume->getSize() >= other.volume->getSize())) {
				
				count = left->getPotentialContactsWith(other, contacts, limit);

				if (limit > count) {
					return count + right->getPotentialContactsWith(other, contacts + count, limit - count);
				}
				else {
					return count;
				}
			}
			else {
				int count = getPotentialContactsWith(other.left, contacts, limit);

				if (limit > count) {
					return count + getPotentialContactsWith(other.right, contacts, limit - count);
				}
				else {
					return count;
				}
			}
		}

		void recalculateboundingVolume();
	};

}