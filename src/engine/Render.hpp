#ifndef RENDER_H
#define RENDER_H

#include "Solver.hpp"
#include "Spawner.hpp"

class Render {
  public:
    Render();
    ~Render();
    void run();

  private:
    sf::RenderWindow window;
    sf::Font font;
    sf::Clock clock;
    sf::Texture circleTexture;
    sf::Text fpsText;
    sf::Text infoText;

    Solver solver;
    Spawner* spawner = nullptr;
    std::vector<VerletObject> objects;
    sf::VertexArray vertices{sf::Quads};
    int spawnAtOnce = 1;

    bool showFPS = false;
    bool showInfo = true;

  private:
    void handleKeyReleased(int);
    void update(float dt);
    void draw();
};

#endif

