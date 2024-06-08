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
          default:
            break;
        }
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
      addObject(hue++);

    deltaTime = clock.restart().asSeconds();
    update(deltaTime);

    window.clear();
    draw();
    window.display();
  }
}

void Render::addObject(int hue) {
  constexpr float r = 5.f;

  sf::Vector2f mouse(sf::Mouse::getPosition(window));
  const VerletObject obj{mouse, mouse, {0.f, 0.f}, r, hsv2rgb(hue, 1.f, 1.f, 255.f)};
  objects.push_back(obj);

  const sf::Vector2f texSize(circleTexture.getSize());
  sf::Vertex topLeft;
  sf::Vertex topRight;
  sf::Vertex bottomRight;
  sf::Vertex bottomLeft;

  topLeft.position     = {mouse.x - r, mouse.y - r};
  topRight.position    = {mouse.x + r, mouse.y - r};
  bottomRight.position = {mouse.x + r, mouse.y + r};
  bottomLeft.position  = {mouse.x - r, mouse.y + r};

  topLeft.color     = obj.color;
  topRight.color    = obj.color;
  bottomRight.color = obj.color;
  bottomLeft.color  = obj.color;

  topLeft.texCoords     = {0.f, 0.f};
  topRight.texCoords    = {texSize.x, 0.f};
  bottomRight.texCoords = {texSize.x, texSize.y};
  bottomLeft.texCoords  = {0.f, texSize.y};

  vertices.append(topLeft);
  vertices.append(topRight);
  vertices.append(bottomRight);
  vertices.append(bottomLeft);
}

void Render::update(float dt) {
  // Update fps text
  int fps = static_cast<int>(1.f / dt);
  fpsText.setString(std::to_string(fps));

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
}

void Render::draw() {
  window.draw(vertices, &circleTexture);

  if (showFPS)
    window.draw(fpsText);
}

