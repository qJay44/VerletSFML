#include "Render.hpp"
#include "Solver.hpp"

Render::Render() : solver(objects) {
  window.create(sf::VideoMode(WIDTH, HEIGHT), "VerletSFML", sf::Style::Close);
  window.setFramerateLimit(90);
  font.loadFromFile("../../src/res/Minecraft rus.ttf");
  circleTexture.loadFromFile("../../src/res/circle-512.png");

  fpsText.setString("90");
  fpsText.setFont(font);
  fpsText.setCharacterSize(20);
  fpsText.setOutlineColor(sf::Color(31, 31, 31));
  fpsText.setOutlineThickness(3.f);
  fpsText.setPosition({WIDTH - fpsText.getLocalBounds().width, 0});

  infoText.setString("count:\t100000\nsub steps:\t10");
  infoText.setFont(font);
  infoText.setCharacterSize(10);
  infoText.setOutlineColor(sf::Color(31, 31, 31));
  infoText.setOutlineThickness(3.f);

  spawner = new Spawner(objects, vertices, sf::Vector2f(circleTexture.getSize()), SPAWN_COUNT);
  quadtree = new qt::Node(boundary);
}

Render::~Render() {
  if (spawner) delete spawner;
  delete quadtree;
}

void Render::run() {
  while (window.isOpen()) {
    static float deltaTime = 0.f;
    static int hue = 0;

    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();

      if (event.type == sf::Event::KeyReleased)
        switch (event.key.code) {
          case sf::Keyboard::Q:
            window.close();
            break;
          case sf::Keyboard::F:
            showFPS = !showFPS;
            break;
          case sf::Keyboard::I:
            showInfo = !showInfo;
            break;
          case sf::Keyboard::G:
            showQT = !showQT;
            break;
          default:
            break;
        }
    }

    deltaTime = clock.restart().asSeconds();

    if (spawner && spawner->add(deltaTime)) {
      delete spawner; spawner = nullptr;
    }

    update(deltaTime);

    window.clear();
    draw();
    window.display();
  }
}

void Render::update(float dt) {
  // Reset quadtree
  delete quadtree; quadtree = new qt::Node(boundary);
  solver.setQuadTree(quadtree);

  // Update objects
  solver.update(dt);

  // Update vertices
  for (int i = 0; i < objects.size(); i ++) {
    const VerletObject& obj = objects[i];
    const sf::Vector2f& pos = obj.positionCurrent;
    const float& r = obj.radius;
    int ii = i << 2;

    vertices[ii + 0].position = {pos.x - r, pos.y - r};
    vertices[ii + 1].position = {pos.x + r, pos.y - r};
    vertices[ii + 2].position = {pos.x + r, pos.y + r};
    vertices[ii + 3].position = {pos.x - r, pos.y + r};
  }

  // Update fps text
  int fps = static_cast<int>(1.f / dt);
  fpsText.setString(std::to_string(fps));

  // Update info text
  std::string infoStr("count:\t" + std::to_string(objects.size()));
  infoStr.append("\nsub steps:\t" + std::to_string(SUB_STEPS));
  infoText.setString(infoStr);
}

void Render::draw() {
  window.draw(vertices, &circleTexture);

  if (showFPS)
    window.draw(fpsText);

  if (showInfo)
    window.draw(infoText);

  if (showQT) {
    sf::VertexArray va{sf::Lines};
    quadtree->show(va);
    window.draw(va);
  }
}

