#pragma once

#include <cstddef>
#include <cstdint>


enum class tile_type {
    SPACE,
    WALL,
    START,
    TARGET,
    BLACK,
    FRINGE,
    PATH
};

class tile {
public:
    tile(size_t row = 0, size_t col = 0);

    size_t row() const;
    size_t col() const;
    tile_type type() const;

    void row(size_t row);
    void col(size_t col);
    void type(tile_type type);

private:
    size_t m_row;
    size_t m_col;
    tile_type m_type;
};

void generate_maze(tile* matrix, size_t size);
void draw_maze(tile* matrix, size_t size);
void reset_maze(tile* matrix, size_t size);

template<size_t size>
using maze_matrix = tile[size][size];

template<size_t size>
void generate_maze(maze_matrix<size>& matrix) {
    generate_maze(reinterpret_cast<tile*>(matrix), size);
}

template<size_t size>
void draw_maze(maze_matrix<size>& matrix) {
    draw_maze(reinterpret_cast<tile*>(matrix), size);
}

template<size_t size>
void reset_maze(maze_matrix<size>& matrix) {
    reset_maze(reinterpret_cast<tile*>(matrix), size);
}
