#pragma once

#include <vector>

#include "maze.h"


class bfs;

class bfs_node {
public:
    bfs_node();

    const bfs* parent_bfs() const;

    const tile& maze_tile() const;
    tile& maze_tile();

    const bfs_node* parent() const;
    bfs_node* parent();

    void parent_bfs(const bfs* type);
    void maze_tile(tile& tile);
    void parent(bfs_node* parent);

private:
    const bfs* m_parent_bfs;
    tile* m_tile;
    bfs_node* m_parent;
};

void bfs_map_initialize(bfs_node* bfs_map, tile* tiles, size_t size);
void bfs_map_clear(bfs_node* bfs_map, size_t size);

template<size_t size>
using bfs_map = bfs_node[size][size];

template<size_t size>
void bfs_map_initialize(bfs_map<size>& map, maze_matrix<size>& maze) {
    bfs_map_initialize(
            reinterpret_cast<bfs_node*>(map),
            reinterpret_cast<tile*>(maze),
            size);
}
template<size_t size>
void bfs_map_clear(bfs_map<size>& map) {
    bfs_map_clear(reinterpret_cast<bfs_node*>(map), size);
}

class bfs {
public:
    explicit bfs(bfs_node* bfs_map, size_t bfs_map_size) noexcept;

    template<size_t size>
    explicit bfs(bfs_map<size>& bfs_map) noexcept;

    bool has_more_fringes() const;

    void clear();
    void add_node(tile& tile, bfs_node* parent = nullptr);
    bfs_node* dequeue_fringe_node();

private:
    bfs_node* m_bfs_map;
    size_t m_bfs_map_size;
    std::vector<bfs_node*> m_fringe_nodes;
};

template<size_t size>
bfs::bfs(bfs_map<size>& bfs_map) noexcept
    : m_bfs_map(reinterpret_cast<bfs_node*>(bfs_map))
    , m_bfs_map_size(size)
{}
