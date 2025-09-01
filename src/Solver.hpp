#pragma once
#include "VerletParticle.hpp"
#include <vector>

class Solver {
public:
	Solver() = default;
	
	VerletParticle& addObject(sf::Vector2f position, float radius) {

		VerletParticle particle = VerletParticle(position, radius);

		return m_objects.emplace_back(particle);
		
	}

	void update() {
		applyGravity();
		updateObjects(m_dt);
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

private:
	std::vector<VerletParticle> m_objects;
	sf::Vector2f m_gravity = { 0.0f, 10.0f };
	float m_dt = 1.0f / 60.0f;
	sf::Vector2f m_boundryCenter = { 420.0f,420.0f };
	float m_boundryRadius = 100.0f;

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
};