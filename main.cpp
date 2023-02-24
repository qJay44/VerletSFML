#include "SFML/Window.hpp"
#include "SFML/System.hpp"
#include "solver.hpp"
#include "link.hpp"
#include "verlet_object.hpp"
#include <iostream>

float randBetween(int min, int max) {
  return static_cast<float>(min + rand() % ((max + 1) - min));
}

static sf::Color getColor(float t) {
  const float r = sin(t);
  const float g = sin(t + 0.33f * 2.f * 3.14f);
  const float b = sin(t + 0.66f * 2.f * 3.14f);

  return {
    static_cast<uint8_t>(255.f * r * r),
    static_cast<uint8_t>(255.f * g * g),
    static_cast<uint8_t>(255.f * b * b),
  };
}

int main() {

  constexpr int32_t windowWidth  = 1200;
  constexpr int32_t windowHeight = 900;
  const float objectSpawnDelay = 0.025f;

  sf::ContextSettings settings;
  settings.antialiasingLevel = 1;

  sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Primitive cellular automata");
  const uint32_t frameRate = 75;
  window.setFramerateLimit(frameRate);

  vec2 screenCenter = { windowWidth / 2.f, windowHeight / 2.f };

  Solver solver;

  const float constraintRadius = 400.f;
  solver.setConstraintPos(screenCenter);
  solver.setConstraintRadius(constraintRadius);

  sf::Clock clock;
  sf::Clock clock2;
  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
          window.close();

      if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && clock2.getElapsedTime().asSeconds() >= objectSpawnDelay) {
        const float mouseX = (float) sf::Mouse::getPosition(window).x;
        const float mouseY = (float) sf::Mouse::getPosition(window).y;

        VerletObject obj{
          { mouseX, mouseY },
          { mouseX, mouseY },
          { 0.f, 0.f },
          randBetween(1, 20),
          getColor(clock2.getElapsedTime().asSeconds())
        };

        solver.addObject(obj);
      }
    }

    window.clear(sf::Color(55, 55, 55));

    sf::CircleShape constraint{constraintRadius};
    constraint.setOrigin(constraintRadius, constraintRadius);
    constraint.setPosition(screenCenter.x, screenCenter.y);
    constraint.setFillColor(sf::Color::Black);
    constraint.setPointCount(128);

    window.draw(constraint);

    sf::CircleShape circle{ 1.f };
    circle.setOrigin(1.f, 1.f);
    circle.setPointCount(32);

    for (VerletObject &obj : solver.getObjects()) {
      circle.setPosition(obj.position_current.x , obj.position_current.y);
      circle.setScale(obj.radius, obj.radius);
      circle.setFillColor(obj.color);

      window.draw(circle);
    }

    solver.update(clock.restart().asSeconds());

    window.display();
  }

  return 0;
}

