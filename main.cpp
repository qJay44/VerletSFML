#include "SFML/Window.hpp"
#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "solver.hpp"
#include <iostream>

int main() {

  constexpr int32_t windowWidth  = 1000;
  constexpr int32_t windowHeight = 1000;

  sf::ContextSettings settings;
  settings.antialiasingLevel = 1;

  sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Primitive cellular automata");
  const uint32_t frameRate = 75;
  window.setFramerateLimit(frameRate);

  vec2 screenCenter = { windowWidth / 2.f, windowHeight / 2.f };

  Solver solver;
  solver.addObject({
      screenCenter + 50.f,
      screenCenter + 50.f,
      { 0.f, 0.f }
  });

  const float constraintRadius = 400.f;
  solver.setConstraintPos(screenCenter);
  solver.setConstraintRadius(constraintRadius);

  sf::Clock clock;
  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
            window.close();
    }

    window.clear(sf::Color(55, 55, 55));

    solver.update(clock.restart().asSeconds());

    sf::CircleShape constraint{400.f};
    constraint.setOrigin(constraintRadius, constraintRadius);
    constraint.setPosition(screenCenter.x, screenCenter.y);
    constraint.setFillColor(sf::Color::Black);
    constraint.setPointCount(128);

    window.draw(constraint);


    sf::CircleShape circle{ 1.f };
    circle.setOrigin(1.f, 1.f);
    circle.setPointCount(32);
    circle.setFillColor(sf::Color::White);

    for (VerletObject obj : solver.getObjects()) {
      circle.setPosition(obj.position_current.x , obj.position_current.y);
      circle.setScale(obj.radius, obj.radius);

      window.draw(circle);
    }

    window.display();
  }

  return 0;
}

