#ifndef QUADTREE_H
#define QUADTREE_H

#include <list>

namespace qt {
  struct Point {
    float x, y;
    void* data;
  };

  struct Rectangle {
    // NOTE: Coords must point to the center of a rectangle
    float x, y, w, h;

    float top    = y - h;
    float right  = x + w;
    float bottom = y + h;
    float left   = x - w;

    bool contains(const Point& p) const;
    bool intersects(const Rectangle& r) const;
  };

  class Node {
    public:
      Node(Rectangle boundary);
      ~Node();

      bool insert(Point p);
      void query(std::list<void*>& found, const Rectangle& range);
      void show(sf::VertexArray& va);

    private:
      Rectangle boundary;
      std::list<Point> points;
      bool divided = false;

      Node* northWest = nullptr;
      Node* northEast = nullptr;
      Node* southWest = nullptr;
      Node* southEast = nullptr;

    private:
      void subdivide();
  };
}

#endif

