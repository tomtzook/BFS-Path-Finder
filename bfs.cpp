
#include "utils.h"
#include "bfs.h"


bfs_node::bfs_node()
    : m_parent_bfs(nullptr)
    , m_tile(nullptr)
    , m_parent(nullptr)
{ }

const bfs* bfs_node::parent_bfs() const {
    return m_parent_bfs;
}

const tile& bfs_node::maze_tile() const {
    return *m_tile;
}

tile& bfs_node::maze_tile() {
    return *m_tile;
}

const bfs_node* bfs_node::parent() const {
    return m_parent;
}

bfs_node* bfs_node::parent() {
    return m_parent;
}

void bfs_node::parent_bfs(const bfs* bfs) {
    m_parent_bfs = bfs;
}

void bfs_node::maze_tile(tile& tile) {
    m_tile = &tile;
}

void bfs_node::parent(bfs_node* parent) {
    m_parent = parent;
}

bfs::bfs(bfs_node* bfs_map, size_t bfs_map_size) noexcept
        : m_bfs_map(bfs_map)
        , m_bfs_map_size(bfs_map_size)
{}

bool bfs::has_more_fringes() const {
    return !m_fringe_nodes.empty();
}

void bfs::clear() {
    m_fringe_nodes.clear();
}

void bfs::add_node(tile& tile, bfs_node* parent) {
    bfs_node& node = LINEAR_MAT(m_bfs_map, tile.row(), tile.col(), m_bfs_map_size);
    node.parent_bfs(this);
    node.parent(parent);

    m_fringe_nodes.push_back(&node);
}

bfs_node* bfs::dequeue_fringe_node() {
    if (m_fringe_nodes.empty()) {
        return nullptr;
    }

    bfs_node* node = m_fringe_nodes.front();
    m_fringe_nodes.erase(m_fringe_nodes.cbegin());

    return node;
}

void bfs_map_initialize(bfs_node* bfs_map, tile* tiles, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            auto& tile = LINEAR_MAT(tiles, i, j, size);
            LINEAR_MAT(bfs_map, i, j, size).maze_tile(tile);
        }
    }
}

void bfs_map_clear(bfs_node* bfs_map, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            LINEAR_MAT(bfs_map, i, j, size).parent_bfs(nullptr);
            LINEAR_MAT(bfs_map, i, j, size).parent(nullptr);
        }
    }
}
