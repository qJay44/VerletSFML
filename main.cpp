#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"

int main() {

  constexpr int32_t windowWidth  = 1000;
  constexpr int32_t windowHeight = 1000;

  sf::ContextSettings settings;
  settings.antialiasingLevel = 1;

  sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Primitive cellular automata");
  const uint32_t frameRate = 75;
  window.setFramerateLimit(frameRate);

  sf::Clock clock;
  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
            window.close();
    }

    window.display();
  }

  return 0;
}

