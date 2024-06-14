#include <algorithm>
#include <format>

#include "Render.hpp"
#include "Solver.hpp"

Render::Render()
  : solver(objects) {
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

  view.setSize(WIDTH, HEIGHT);
  view.setCenter(WIDTH * 0.5f, HEIGHT * 0.5f);
  viewStatic.setSize(WIDTH, HEIGHT);
  viewStatic.setCenter(WIDTH * 0.5f, HEIGHT * 0.5f);

  spawner = Spawner(&objects, &vertices, sf::Vector2f(circleTexture.getSize()));
}

Render::~Render() {}

void Render::run() {
  while (window.isOpen()) {
    static float deltaTime = 0.f;
    static int hue = 0;

    sf::Event event;
    while (window.pollEvent(event)) {
      switch (event.type) {
        case sf::Event::Closed:
          window.close();
          break;
        case sf::Event::KeyReleased:
          handleKeyReleased(event.key.code);
          break;
        case sf::Event::MouseMoved:
          mouse = sf::Mouse::getPosition(window);
          break;
        case sf::Event::MouseButtonPressed:
          if (event.mouseButton.button == sf::Mouse::Left)
            mousePressed = sf::Mouse::getPosition(window);
          break;
        case sf::Event::MouseWheelScrolled:
          zoom(-event.mouseWheelScroll.delta);
          break;
        default:
          break;
      }
    }

    if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
      mousePressed = mouse;

    deltaTime = clock.restart().asSeconds();

    spawner.add(spawnAtOnce);

    update(deltaTime);
    updateView();

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
  std::string infoStr(std::format("count:\t{}\n", objects.size()));
  infoStr.append(std::format("sub steps:\t{}\n", SUB_STEPS));
  infoStr.append(std::format("{:.2f}ms", dt * 1000.f));
  infoText.setString(infoStr);
}

void Render::updateView() {
  view.move(sf::Vector2f(mousePressed - mouse) * dragFactor);
  mousePressed = mouse;
  window.setView(view);
}

void Render::draw() {
  window.draw(vertices, &circleTexture);
  window.setView(viewStatic);

  if (showFPS)
    window.draw(fpsText);

  if (showInfo)
    window.draw(infoText);
}

void Render::reset() {
  solver.reset();
  spawner.reset();
}

void Render::zoom(float dir) {
  sf::Vector2f prevSize = view.getSize();
  view.zoom(1.f + dir * zoomFactor);
  sf::Vector2f currSize = view.getSize();

  float diff = (currSize.x + currSize.y) / (prevSize.x + prevSize.y);
  dragFactor *= diff;
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
    case sf::Keyboard::Num0:
      spawnAtOnce = 0;
      break;
    case sf::Keyboard::J:
      spawnAtOnce -= 10;
      spawnAtOnce = std::max(0, spawnAtOnce);
      break;
    case sf::Keyboard::K:
      spawnAtOnce += 10;
      break;
    case sf::Keyboard::C:
      solver.switchGravity();
      break;
    case sf::Keyboard::R:
      reset();
      break;
    default:
      break;
  }
}

