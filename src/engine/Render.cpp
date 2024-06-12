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

  spawner = new Spawner(objects, vertices, sf::Vector2f(circleTexture.getSize()));
}

Render::~Render() {
  if (spawner) delete spawner;
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
        handleKeyReleased(event.key.code);
    }

    deltaTime = clock.restart().asSeconds();

    spawner->add(spawnAtOnce, deltaTime);

    update(deltaTime);

    window.clear();
    draw();
    window.display();
  }
}

void Render::update(float dt) {
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

  if (showCells) {
    sf::Vector2 mouse{sf::Mouse::getPosition(window)};
    const Cell* cell = solver.getCellAt(mouse.x, mouse.y);
    int rectX = static_cast<int>(mouse.x / CELL_SIZE) * CELL_SIZE;
    int rectY = static_cast<int>(mouse.y / CELL_SIZE) * CELL_SIZE;
    sf::RectangleShape rect({CELL_SIZE, CELL_SIZE});

    // Center
    rect.setPosition(sf::Vector2f(rectX, rectY));
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(sf::Color::Magenta);
    rect.setOutlineThickness(1.f);
    window.draw(rect);

    // Along left side
    rect.setPosition(sf::Vector2f(rectX - CELL_SIZE, rectY - CELL_SIZE));
    window.draw(rect);
    rect.setPosition(sf::Vector2f(rectX - CELL_SIZE, rectY));
    window.draw(rect);
    rect.setPosition(sf::Vector2f(rectX - CELL_SIZE, rectY + CELL_SIZE));
    window.draw(rect);

    // Along right side
    rect.setPosition(sf::Vector2f(rectX + CELL_SIZE, rectY - CELL_SIZE));
    window.draw(rect);
    rect.setPosition(sf::Vector2f(rectX + CELL_SIZE, rectY));
    window.draw(rect);
    rect.setPosition(sf::Vector2f(rectX + CELL_SIZE, rectY + CELL_SIZE));
    window.draw(rect);

    // Above and under
    rect.setPosition(sf::Vector2f(rectX, rectY - CELL_SIZE));
    window.draw(rect);
    rect.setPosition(sf::Vector2f(rectX, rectY + CELL_SIZE));
    window.draw(rect);
  }
}

void Render::handleKeyReleased(int key) {
  switch (key) {
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
      showCells = !showCells;
      break;
    case sf::Keyboard::Num0:
      spawnAtOnce = 0;
      break;
    case sf::Keyboard::Num1:
      spawnAtOnce = 1;
      break;
    case sf::Keyboard::Num2:
      spawnAtOnce = 2;
      break;
    case sf::Keyboard::Num3:
      spawnAtOnce = 3;
      break;
    case sf::Keyboard::Num4:
      spawnAtOnce = 4;
      break;
    case sf::Keyboard::Num5:
      spawnAtOnce = 5;
      break;
    case sf::Keyboard::Num6:
      spawnAtOnce = 6;
      break;
    case sf::Keyboard::Num7:
      spawnAtOnce = 7;
      break;
    case sf::Keyboard::Num8:
      spawnAtOnce = 8;
      break;
    case sf::Keyboard::Num9:
      spawnAtOnce = 9;
      break;
    default:
      break;
  }
}

