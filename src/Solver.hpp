#pragma once
#include "VerletParticle.hpp"
#include <vector>
#include "Math.hpp"

class Solver {
public:

	Solver(int32_t width, int32_t height){
		m_width = width;
		m_height = height;
	}
	
	VerletParticle& addObject(sf::Vector2f position, float radius, sf::Color color = sf::Color::Cyan) {

		VerletParticle particle = VerletParticle(position, radius);
		particle.color = color;

		return m_objects.emplace_back(particle);
		
	}

	void update(float dt) {
		applyGravity();
		applySquareConstraint();
		handleCollisions();
		updateObjects(dt);
	}

	const std::vector<VerletParticle>& getObjects() const {
		return m_objects;
	}

	void setBoundry(sf::Vector2f center, float radius) {
		m_boundryCenter = center;
		m_boundryRadius = radius;
	}


	sf::Vector3f getBoundry() const {
		return {m_boundryCenter.x, m_boundryCenter.y, m_boundryRadius};
	}

	void pullObjects(sf::Vector2f pullPos) {
		for (auto& obj : m_objects) {
			const sf::Vector2f forceDir = pullPos - obj.position;
			const float dist = sqrt(forceDir.x * forceDir.x + forceDir.y * forceDir.y);
			sf::Vector2f pullForce = 1000000.0f * (forceDir / (dist*dist));
			obj.accelerate(pullForce);
		}
	}

	void setObjectVelocity(VerletParticle& obj, sf::Vector2f vel) {
		obj.setVelocity(vel,1.0f);
	}


private:
	std::vector<VerletParticle> m_objects;
	sf::Vector2f m_gravity = { 0.0f, 1000.0f };
	float m_dt = 1.0f / 60.0f;
	sf::Vector2f m_boundryCenter = { 420.0f,420.0f };
	float m_boundryRadius = 100.0f;
	int32_t m_width;
	int32_t m_height;

	void applyGravity() {
		for (auto& obj : m_objects) {
			obj.accelerate(m_gravity);
		}
	}

	void updateObjects(float _dt) {
		for (auto& obj : m_objects) {
			obj.update(_dt);
		}
	}

	void applySquareConstraint(float dt = 1) {

		for (auto& obj : m_objects) {
			if (obj.position.y - obj.radius < 0 ) {
				obj.position.y = obj.radius;
				sf::Vector2f newVel = obj.getVelocity();
				newVel.y *= -1;
				obj.setVelocity(newVel, dt);
			}
			else if (obj.position.y + obj.radius > m_height) {
				obj.position.y = m_height - obj.radius;
				sf::Vector2f newVel = obj.getVelocity();
				newVel.y *= -1;
				obj.setVelocity(newVel, dt);
			}
			if (obj.position.x - obj.radius < 0) {
				obj.position.x = obj.radius;
				sf::Vector2f newVel = obj.getVelocity();
				newVel.x *= -1;
				obj.setVelocity(newVel,dt);
			}
			else if (obj.position.x + obj.radius > m_width) {
				obj.position.x = m_width - obj.radius;
				sf::Vector2f newVel = obj.getVelocity();
				newVel.x *= -1;
				obj.setVelocity(newVel, dt);
			}

		}
	}

	void applyCircularConstraint(float dt) {

		for (auto& obj : m_objects) {

			// particle and boundry distance vector 
			const sf::Vector2f dVec = obj.position - m_boundryCenter;

			// length of the distance
			const float dist = sqrt(dVec.x * dVec.x + dVec.y * dVec.y);

			// obj is in outside of the boundry
			if (dist > (m_boundryRadius - obj.radius)) {
				const sf::Vector2f normal = dVec / dist;
				sf::Vector2f vel = obj.getVelocity();
				float dot = Math::dot(vel, normal);
				sf::Vector2f newVel = vel - 2.0f * dot * normal;
				obj.position = m_boundryCenter + normal * (m_boundryRadius - obj.radius);
				obj.setVelocity(newVel, 1.0f);
			}
		}
	}

	void handleCollisions() {
		size_t objCount = m_objects.size();
		for (size_t i = 0; i < objCount; i++) {
			VerletParticle& obj1 = m_objects[i];
			for (size_t j = 0; j < objCount; j++) {
				if (i == j) continue;
				VerletParticle& obj2 = m_objects[j];
				sf::Vector2f distVec = obj1.position - obj2.position;
				float dist = sqrt(distVec.x * distVec.x + distVec.y * distVec.y);
				float minDist = obj1.radius + obj2.radius;

				if (dist < minDist && dist != 0) {
					// Normal vector pointing obj1 <---- obj2
					sf::Vector2f normal = distVec / dist;

					//offset
					float delta = 0.5f * (minDist - dist);
					float totalMass = (obj1.radius * obj1.radius) + (obj2.radius * obj2.radius);
					float massRatio = obj1.radius * obj2.radius / totalMass;

					// larger mass moves slower
					obj1.position += normal * (1 - massRatio) * delta;
					obj2.position -= normal * ( massRatio) * delta;

				}
			}
		}
	}

};