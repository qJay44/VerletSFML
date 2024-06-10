#include "quadtree.hpp"

#define CAPACITY 4

// Rectangle
namespace qt {
  bool Rectangle::contains(const Point& p) const {
    return
      p.x >= left  &&
      p.x <= right &&
      p.y >= top   &&
      p.y <= bottom;
  }

  bool Rectangle::intersects(const Rectangle& r) const {
    return !(
      // Checking given rectangle is it off the quad by each side
      top    > r.bottom || // True: The rectangle under the current quad
      right  < r.left   || // True: The rectangle to the right of the current quad
      left   > r.right  || // True: The rectangle to the left the current quad
      bottom < r.top       // True: The rectangle above the current quad
    );
  }
};

// Node
namespace qt {
  Node::Node(Rectangle boundary) : boundary(boundary) {}

  Node::~Node() {
    delete northWest;
    delete northEast;
    delete southWest;
    delete southEast;
  }

  bool Node::insert(Point p) {
    if (!boundary.contains(p)) return false;

    if (points.size() < CAPACITY) {
      points.push_back(p);
      return true;
    } else {
      subdivide();
      return
        northWest->insert(p) ||
        northEast->insert(p) ||
        southWest->insert(p) ||
        southEast->insert(p);
    }
  }

  void Node::query(std::list<void*>& found, const Rectangle& range) {
    if (boundary.intersects(range)) {
      for (Point& p : points)
        if (range.contains(p))
          found.push_back(p.data);

      if (divided) {
        northWest->query(found, range);
        northEast->query(found, range);
        southWest->query(found, range);
        southEast->query(found, range);
      }
    }
  }

  void Node::show(sf::VertexArray& va) {
    static const sf::Color color{30, 30, 30};
    const float& x = boundary.x;
    const float& y = boundary.y;
    const float& w = boundary.w;
    const float& h = boundary.h;

    va.append({{boundary.left, boundary.top}, color});
    va.append({{boundary.right, boundary.top}, color});
    va.append({{boundary.right, boundary.top}, color});
    va.append({{boundary.right, boundary.bottom}, color});
    va.append({{boundary.right, boundary.bottom}, color});
    va.append({{boundary.left, boundary.bottom}, color});
    va.append({{boundary.left, boundary.bottom}, color});
    va.append({{boundary.left, boundary.top}, color});

    if (divided) {
      northWest->show(va);
      northEast->show(va);
      southWest->show(va);
      southEast->show(va);
    }
  }

  void Node::subdivide() {
    if (!divided) {
      const float& x = boundary.x;
      const float& y = boundary.y;
      const float& w = boundary.w;
      const float& h = boundary.h;

      Rectangle nwRect{x - w / 2.f, y - h / 2.f, w / 2.f, h / 2.f};
      Rectangle neRect{x + w / 2.f, y - h / 2.f, w / 2.f, h / 2.f};
      Rectangle swRect{x - w / 2.f, y + h / 2.f, w / 2.f, h / 2.f};
      Rectangle seRect{x + w / 2.f, y + h / 2.f, w / 2.f, h / 2.f};

      northWest = new Node(nwRect);
      northEast = new Node(neRect);
      southWest = new Node(swRect);
      southEast = new Node(seRect);

      divided = true;
    }
  }
};

