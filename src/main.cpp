#include <SFML/Graphics.hpp>
#include "Renderer.hpp"
#include "Solver.hpp"

int main() {
	constexpr int32_t height = 840;
	constexpr int32_t width = 840;

	const uint32_t frame_rate = 60;

	sf::RenderWindow window(sf::VideoMode({ width,height}), "VerletParticleSimulation");
	sf::ContextSettings settings = window.getSettings();
	window.setFramerateLimit(frame_rate);
	settings.antiAliasingLevel = 1;

	Renderer renderer{ window };
	Solver solver;
	solver.addObject({ 420.0f,420.0f }, 10.0f);

	while (window.isOpen()) {
		
		while (const std::optional event = window.pollEvent()) {
			if (event->is < sf::Event::Closed>())
				window.close();
		}
		solver.update();
		window.clear(sf::Color::White);
		renderer.render(solver);
		window.display();
	}
}