#include <SFML/Graphics.hpp>
#include "Renderer.hpp"
#include "Solver.hpp"

int main() {
	constexpr int32_t height = 840;
	constexpr int32_t width = 840;

	const uint32_t frame_rate = 800;
	const float physics_timestep = 1.0f / 60.0f;
	float accumulator = 0.0f;
	int substeps = 8;
	sf::Clock clock;

	sf::RenderWindow window(sf::VideoMode({ width,height }), "VerletParticleSimulation");
	sf::ContextSettings settings = window.getSettings();
	window.setFramerateLimit(frame_rate);
	settings.antiAliasingLevel = 1;

	Renderer renderer{ window };
	Solver solver;
	solver.setBoundry({ (float)height / 2, (float)width / 2 }, 300.0f);


	while (window.isOpen()) {

		float frameTime = std::min(clock.restart().asSeconds(), 0.025f);
		accumulator += frameTime;
		
		while (const std::optional event = window.pollEvent()) {
			if (event->is < sf::Event::Closed>())
				window.close();
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
				sf::Vector2i mousePos = sf::Mouse::getPosition(window);
				sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
				solver.addObject(mousePosF, 10.0f);
			}
		}

		while (accumulator >= physics_timestep) {
			float subDt = physics_timestep / substeps;
			for (int i = 0; i < substeps; i++) {
				solver.update(subDt);
			}
			accumulator -= physics_timestep;

		}

		window.clear(sf::Color(156, 153, 152));
		renderer.render(solver);
		window.display();
	}
}