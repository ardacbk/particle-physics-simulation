#include <SFML/Graphics.hpp>

int main() {

	sf::ContextSettings settings;
	settings.antiAliasingLevel = 1;
	sf::RenderWindow window(sf::VideoMode({ 840,840 }), "VerletParticleSimulation");
	sf::CircleShape shape(5.0f);
	shape.setFillColor(sf::Color::Magenta);
	shape.setPosition({ 420.0f,420.0f });
	while (window.isOpen()) {
		
		while (const std::optional event = window.pollEvent()) {
			if (event->is < sf::Event::Closed>())
				window.close();
		}
		window.clear();
		window.draw(shape);
		window.display();
	}
}