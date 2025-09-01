#pragma once
#include <SFML/Graphics.hpp>
#include "Solver.hpp"

class Renderer {
public:
	explicit Renderer(sf::RenderTarget& target) : m_Target{ target } {}

	void render(Solver solver) {

		sf::Vector3f boundry = solver.getBoundry();
		//Draw boundry
		sf::CircleShape boundryCircle{ boundry.z };
		boundryCircle.setOrigin({ boundry.z, boundry.z });
		boundryCircle.setFillColor(sf::Color::Black);
		boundryCircle.setPosition({ boundry.x,boundry.y });
		boundryCircle.setPointCount(128);
		m_Target.draw(boundryCircle);

		sf::CircleShape shape{ 1.0f };
		shape.setPointCount(32);
		const std::vector<VerletParticle> objects = solver.getObjects();
		for (const auto &obj : objects)
		{
			shape.setScale({obj.radius, obj.radius });
			shape.setPosition(obj.position);
			shape.setFillColor(sf::Color(100, 250, 50));
			m_Target.draw(shape);
		}

	}

private:
	sf::RenderTarget& m_Target;
};