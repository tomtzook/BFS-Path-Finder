#include <cstdlib>
#include <GL/glut.h>

#include "utils.h"
#include "maze.h"


tile::tile(size_t row, size_t col)
    : m_row(row)
    , m_col(col)
    , m_type(tile_type::SPACE)
{}

size_t tile::row() const {
    return m_row;
}

size_t tile::col() const {
    return m_col;
}

tile_type tile::type() const {
    return m_type;
}

void tile::row(size_t row) {
    m_row = row;
}

void tile::col(size_t col) {
    m_col = col;
}

void tile::type(tile_type type) {
    m_type = type;
}

static void init_tiles(tile* matrix, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            LINEAR_MAT(matrix, i, j, size).row(i);
            LINEAR_MAT(matrix, i, j, size).col(j);
            LINEAR_MAT(matrix, i, j, size).type(tile_type::SPACE);
        }
    }
}

static void init_boundaries(tile* matrix, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        LINEAR_MAT(matrix, 0, i, size).type(tile_type::WALL);
        LINEAR_MAT(matrix, size - 1, i, size).type(tile_type::WALL);
        LINEAR_MAT(matrix, i, 0, size).type(tile_type::WALL);
        LINEAR_MAT(matrix, i, size - 1, size).type(tile_type::WALL);
    }
}

static void generate_maze_content(tile* matrix, size_t size) {
    for (size_t i = 1; i < size - 1; ++i) {
        for (size_t j = 1; j < size - 1; ++j) {
            if (i % 2 == 1) {
                if (rand() % 100 < 10) {
                    LINEAR_MAT(matrix, i, j, size).type(tile_type::WALL);
                } else {
                    LINEAR_MAT(matrix, i, j, size).type(tile_type::SPACE);
                }
            } else {
                if (rand() % 100 < 40) {
                    LINEAR_MAT(matrix, i, j, size).type(tile_type::SPACE);
                } else {
                    LINEAR_MAT(matrix, i, j, size).type(tile_type::WALL);
                }
            }
        }
    }
}

void generate_maze(tile* matrix, size_t size) {
    init_tiles(matrix, size);
    init_boundaries(matrix, size);
    generate_maze_content(matrix, size);
}

static void draw_tile(double x, double y, double size_x, double size_y) {
    glBegin(GL_POLYGON);
    glVertex2d(x, y);
    glVertex2d(x, y + size_y);
    glVertex2d(x + size_x, y + size_y);
    glVertex2d(x + size_x, y);
    glEnd();
}

void draw_maze(tile* matrix, size_t size) {
    double size_x = 2.0 / static_cast<double>(size);
    double size_y = 2.0 / static_cast<double>(size);

    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            switch (LINEAR_MAT(matrix, i, j, size).type()) {
                case tile_type::SPACE:
                    glColor3d(1, 1, 1);
                    break;
                case tile_type::WALL:
                    glColor3d(0.7, 0.4, 0);
                    break;
                case tile_type::START:
                    glColor3d(0, 1, 1);
                    break;
                case tile_type::TARGET:
                    glColor3d(1, 0, 0);
                    break;
                case tile_type::BLACK:
                    glColor3d(0.8, 1, 0.8);
                    break;
                case tile_type::FRINGE:
                    glColor3d(1, 1, 0);
                    break;
                case tile_type::PATH:
                    glColor3d(0.8, 0.4, 1);
                    break;
            }

            double x = 2 * (j / static_cast<double>(size)) - 1;
            double y = 2 * (i / static_cast<double>(size)) - 1;

            draw_tile(x, y, size_x, size_y);
        }
    }
}

void reset_maze(tile* matrix, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            switch (LINEAR_MAT(matrix, i, j, size).type()) {
                case tile_type::START:
                case tile_type::TARGET:
                case tile_type::BLACK:
                case tile_type::FRINGE:
                case tile_type::PATH:
                    LINEAR_MAT(matrix, i, j, size).type(tile_type::SPACE);
                    break;
            }
        }
    }
}
