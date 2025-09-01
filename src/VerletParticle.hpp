#pragma once
#include <SFML/Graphics.hpp>

struct VerletParticle {
	sf::Vector2f position;
	sf::Vector2f position_last;
	sf::Vector2f acceleration;

	float		radius		= 10.0f;
	sf::Color	color		= sf::Color::White;

	VerletParticle() = default;
	VerletParticle(sf::Vector2f position_, float radius_) :
		position{ position_ },
		position_last{ position_ },
		acceleration{ 10.0f,10.0f },
		radius{radius_} 
	{}

	void update(float dt) {
		const sf::Vector2f displacement = position - position_last;
		position_last = position;
		position = position + displacement + acceleration * (dt * dt);
		acceleration = {}; // reset acceleration

	}

	void accelerate(sf::Vector2f a) {
		acceleration += a;
	}

	void setVelocity(sf::Vector2f vel, float dt) {
		position_last = position - (vel * dt); // vel  = (position - position_last) / dt
	}

	void addVelocity(sf::Vector2f vel, float dt) {
		position_last -= vel * dt;
	}

	sf::Vector2f getVelocity(float dt = 1) const {
		return (position - position_last) / dt;
	}
};