#ifndef RENDER_H
#define RENDER_H

#include <cstdint>
#include <list>
#include <string>

#include "VerletObject.hpp"

class Render {
  public:
    Render(uint16_t width, uint16_t height, std::string title);
    void run();

  private:
    sf::RenderWindow window;
    sf::Font font;
    sf::Clock clock;
    sf::Texture circleTexture;

    std::list<VerletObject> objects;
    sf::VertexArray vertices{sf::Quads};

  private:
    void update(float dt);
    void draw();
};

#endif

