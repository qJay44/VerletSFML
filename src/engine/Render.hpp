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
    sf::View view;
    sf::View viewStatic;
    sf::Vector2i mouse{};
    sf::Vector2i mousePressed{};

    Solver solver;
    Spawner spawner;
    std::vector<VerletObject> objects;
    sf::VertexArray vertices{sf::Quads};

    int spawnAtOnce = 1;
    float zoomFactor = 0.3f;
    float dragFactor = 1.2f;

    bool showFPS = false;
    bool showInfo = true;

  private:
    void update(float dt);
    void updateView();
    void draw();
    void reset();
    void zoom(float dir);
    void handleKeyReleased(int);
};

#endif

