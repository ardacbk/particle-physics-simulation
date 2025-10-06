#include <SFML/Graphics.hpp>
#include "Renderer.hpp"
#include "Solver.hpp"
#include <random>
#include <iostream>
#include <numbers>

static sf::Color getRainbow(float t)
{
	const float r = sin(t);
	const float g = sin(t + 0.33f * 2.0f * std::numbers::pi_v<float>);
	const float b = sin(t + 0.66f * 2.0f * std::numbers::pi_v<float>);
	return { static_cast<uint8_t>(255.0f * r * r),
			static_cast<uint8_t>(255.0f * g * g),
			static_cast<uint8_t>(255.0f * b * b) };
}


std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<float> radiusDist(5.0f, 10.0f);
std::uniform_int_distribution<int> colorDist(128, 255);


int main() {
	constexpr int32_t height = 840;
	constexpr int32_t width = 840;
	static float seed = 0.0f;
	const sf::Vector2f spawn_position = { 420.0f,120.0f };

	const uint32_t frame_rate = 60;
	const float physics_timestep = 1.0f / 60.0f;
	float accumulator = 0.0f;
	sf::Clock spawnClock;
	sf::Time spawn_cooldown = sf::seconds(0.05f);
	const int substeps = 8;
	sf::Clock clock;
	sf::Clock timer;
	const size_t max_particle = 200;

	sf::RenderWindow window(sf::VideoMode({ width,height }), "VerletParticleSimulation");
	sf::ContextSettings settings = window.getSettings();
	window.setFramerateLimit(frame_rate);
	settings.antiAliasingLevel = 1;

	Renderer renderer{ window };
	Solver solver(width, height);
	solver.setBoundry({ (float)height / 2, (float)width / 2 }, 350.0f);

	while (window.isOpen()) {

		float frameTime = std::min(clock.restart().asSeconds(), 0.025f);
		accumulator += frameTime;
		
		while (const std::optional event = window.pollEvent()) {
			if (event->is < sf::Event::Closed>())
				window.close();
		}


		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && spawnClock.getElapsedTime() > spawn_cooldown) {
			sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
			sf::Vector2f mousePosF = window.mapPixelToCoords(pixelPos);

			float radius = radiusDist(gen);
			seed += 1.0f;

			sf::Color color = getRainbow(seed);

			solver.addObject(mousePosF, radius, color);

			spawnClock.restart();
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
			sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
			sf::Vector2f mousePosF = window.mapPixelToCoords(pixelPos);
			solver.pullObjects(mousePosF);
		}

		if (solver.getObjects().size() < max_particle && spawnClock.getElapsedTime() > spawn_cooldown) {
			float t = timer.getElapsedTime().asSeconds();
			float radius = radiusDist(gen);
			float angle = std::numbers::pi_v<float> * 0.5f +  1.2f * sin(3 * t);
			sf::Color color = getRainbow(t);
			auto& obj = solver.addObject(spawn_position, radius,color);
			solver.setObjectVelocity(obj, 1.0f * sf::Vector2f{ cos(angle), sin(angle) });
			spawnClock.restart();
			
		}


		while (accumulator >= physics_timestep) {
			float subDt = physics_timestep / substeps;
			for (int i = 0; i < substeps; i++) {
				solver.update(subDt);
			}
			accumulator -= physics_timestep;

		}

		window.clear(sf::Color::Black);
		renderer.render(solver);
		window.display();
	}
}