#ifndef RENDER_H
#define RENDER_H

#include "Solver.hpp"

class Render {
  public:
    Render();
    void run();

  private:
    sf::RenderWindow window;
    sf::Font font;
    sf::Clock clock;
    sf::Texture circleTexture;
    sf::Text fpsText;

    Solver solver;
    std::vector<VerletObject> objects;
    sf::VertexArray vertices{sf::Quads};

    bool showFPS = true;

  private:
    void addObject(int hue);
    void update(float dt);
    void draw();
};

#endif

