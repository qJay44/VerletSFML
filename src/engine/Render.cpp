#include "Render.hpp"

Render::Render(uint16_t width, uint16_t height, std::string title) {
  window.create(sf::VideoMode(width, height), title, sf::Style::Close);
  window.setFramerateLimit(90);
  font.loadFromFile("../../src/res/Minecraft rus.ttf");
  circleTexture.loadFromFile("../../src/res/circle-512.png");
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
          default:
            break;
        }
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
      sf::Vector2f mouse(sf::Mouse::getPosition(window));
      objects.push_back({mouse, mouse, {0.f, 0.f}, 15.f, hsv2rgb(hue, 1.f, 1.f, 255.f)});
      hue++;
    }

    deltaTime = clock.restart().asSeconds();
    update(deltaTime);

    window.clear();
    draw();
    window.display();
  }
}

void Render::update(float dt) {
  // Create vertices of the objects
  vertices.clear();
  for (const VerletObject& obj : objects) {
    static const sf::Vector2f texSize(circleTexture.getSize());
    const sf::Vector2f& pos = obj.positionCurrent;
    float halfSize = obj.size * 0.5f;
    sf::Vertex topLeft;
    sf::Vertex topRight;
    sf::Vertex bottomRight;
    sf::Vertex bottomLeft;

    topLeft.position     = {pos.x - halfSize, pos.y - halfSize};
    topRight.position    = {pos.x + halfSize, pos.y - halfSize};
    bottomRight.position = {pos.x + halfSize, pos.y + halfSize};
    bottomLeft.position  = {pos.x - halfSize, pos.y + halfSize};

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
}

void Render::draw() {
  window.draw(vertices, &circleTexture);
}

