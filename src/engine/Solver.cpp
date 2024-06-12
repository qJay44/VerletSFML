#include "Solver.hpp"

#define IX(x, y) ((x) + (y) * (COLUMNS))

Solver::Solver(std::vector<VerletObject>& objects)
  : objects(objects) {
    cells.resize(COLUMNS * ROWS);
    cells.reserve(COLUMNS * ROWS);

    for (int i = 0; i < COLUMNS; i++) {
      for (int j = 0; j < ROWS; j++) {
        Cell& cell = cells[IX(i, j)];

        // Along left side
        if (i > 0) {
          cell.addNeighbour(&cells[IX(i - 1, j)]); // Middle-left

          if (j > 0)        cell.addNeighbour(&cells[IX(i - 1, j - 1)]); // Top-left
          if (j < ROWS - 1) cell.addNeighbour(&cells[IX(i - 1, j + 1)]); // Bottom-left
        }

        // Along right side
        if (i < COLUMNS - 1) {
          cell.addNeighbour(&cells[IX(i + 1, j)]); // Middle-right

          if (j > 0)        cell.addNeighbour(&cells[IX(i + 1, j - 1)]); // Top-left
          if (j < ROWS - 1) cell.addNeighbour(&cells[IX(i + 1, j + 1)]); // Bottom-left
        }

        // Above and under
        if (j > 0)        cell.addNeighbour(&cells[IX(i, j - 1)]); // Top
        if (j < ROWS - 1) cell.addNeighbour(&cells[IX(i, j + 1)]); // Bottom
      }
    }
    tp.start();
  }

Solver::~Solver() {
  tp.stop();
}

void Solver::update(float dt) {
  float dtSub = dt / SUB_STEPS;
  for (int i = 0; i < SUB_STEPS; i++) {
    for (Cell& cell : cells)
      cell.clearObjects();

    int slice = objects.size() / tp.size();
    for (int i = 0; i < tp.size(); i++) {
      int begin = i * slice;
      int end = begin + slice;

      if (i == tp.size() - 1)
        end += objects.size() % tp.size();

      tp.queueJob([this, begin, end, dtSub] {
        for (int i = begin; i < end; i++) {
          VerletObject& obj = objects[i];
          // Apply gravity
          obj.accelerate(gravity);

          // Apply vertical constraint
          if (obj.positionCurrent.y - obj.radius <= 0.f) obj.positionCurrent.y = obj.radius;
          else if (obj.positionCurrent.y + obj.radius >= HEIGHT) obj.positionCurrent.y = HEIGHT - obj.radius;

          // Apply horizontal constraint
          if (obj.positionCurrent.x - obj.radius <= 0.f) obj.positionCurrent.x = obj.radius;
          else if (obj.positionCurrent.x + obj.radius >= WIDTH) obj.positionCurrent.x = WIDTH - obj.radius;

          // Update cells
          int column = obj.positionCurrent.x / CELL_SIZE;
          int row = obj.positionCurrent.y / CELL_SIZE;
          cells[IX(column, row)].addObject(&obj);

          // Update position
          obj.updatePosition(dtSub);
        }
      });
    }
    tp.waitForCompletion();

    solveCollisions();
  }
}

 const Cell* Solver::getCellAt(int x, int y) const {
  std::list<const Cell*> list;
  int column = x / CELL_SIZE;
  int row = y / CELL_SIZE;

  return &cells[IX(column, row)];
}

void Solver::solveCollisions() {
  int sliceCount = tp.size() * 2;
  int slice = COLUMNS / sliceCount;

  // First half
  for (int i = 0; i < tp.size(); i++) {
    int begin = i * slice;
    int end = begin + slice;

    tp.queueJob([this, begin, end] {
      for (int i = begin; i < end; i++)
        for (int j = 0; j < ROWS; j++)
          cells[IX(i, j)].checkCollisions();
    });
  }
  tp.waitForCompletion();

  // Second half
  for (int i = tp.size(); i < sliceCount; i++) {
    int begin = i * slice;
    int end = begin + slice;

    if (i == sliceCount - 1)
      end += COLUMNS % sliceCount;

    tp.queueJob([this, begin, end] {
      for (int i = begin; i < end; i++)
        for (int j = 0; j < ROWS; j++)
          cells[IX(i, j)].checkCollisions();
    });
  }
  tp.waitForCompletion();
}

