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

    qt::Rectangle boundary{WIDTH * 0.5f, HEIGHT * 0.5f, WIDTH * 0.5f, HEIGHT * 0.5f};
    qt::Node* quadtree = nullptr;

    bool showFPS = true;
    bool showInfo = false;
    bool showQT = false;

  private:
    void update(float dt);
    void draw();
};

#endif

