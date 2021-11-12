#include <algorithm>
#include <iostream>
#include <utility>
#include <GL/glew.h>
#include <GL/glut.h>

#include "maze.h"
#include "bfs.h"

static constexpr size_t MAZE_SIZE = 50;

maze_matrix<MAZE_SIZE> maze;
bfs_map<MAZE_SIZE> bfs_node_map;

bfs first_bfs(bfs_node_map);
bfs second_bfs(bfs_node_map);

bool run_bfs = false;
bool bfs_run_requested = false;

static tile& get_random_tile() {
    size_t row = rand() % MAZE_SIZE;
    size_t col = rand() % MAZE_SIZE;

    return maze[row][col];
}

static void reset() {
    run_bfs = false;

    reset_maze(maze);
    bfs_map_clear(bfs_node_map);

    first_bfs.clear();
    second_bfs.clear();

    tile& start_tile = get_random_tile();
    start_tile.type(tile_type::START);
    first_bfs.add_node(start_tile);

    tile& target_tile = get_random_tile();
    target_tile.type(tile_type::TARGET);
    second_bfs.add_node(target_tile);

    std::cout << "Start ("
              << start_tile.row() << ", " << start_tile.col() << ")" << std::endl;
    std::cout << "Target ("
              << target_tile.row() << ", " << target_tile.col() << ")" << std::endl;
}

static void bfs_path_found(bfs_node& start_node) {
    bfs_node* node = &start_node;
    while (node != nullptr) {
        node->maze_tile().type(tile_type::PATH);
        node = node->parent();
    }
}

static bool bfs_iteration(bfs& current_bfs, bfs& other_bfs) {
    bfs_node* current_node = current_bfs.dequeue_fringe_node();
    if (nullptr == current_node) {
        return false;
    }

    size_t row = current_node->maze_tile().row();
    size_t col = current_node->maze_tile().col();

    std::cout << "Running from ("
              << row << ", " << col << ")"
              << std::endl;

    std::pair<size_t, size_t> points[] = {
            {row, col - 1},
            {row, col + 1},
            {row - 1, col},
            {row + 1, col},
    };

    for (auto& point : points) {
        if (point.first >= MAZE_SIZE || point.second >= MAZE_SIZE) {
            continue;
        }

        std::cout << "\tAt node ("
                  << point.first << ", " << point.second << ")" << std::endl;

        tile& neighbor = maze[point.first][point.second];
        bfs_node& neighbor_node = bfs_node_map[point.first][point.second];

        if (neighbor_node.parent_bfs() == &other_bfs) {
            // intersection
            std::cout << "\t\tIntersection" << std::endl;

            bfs_path_found(*current_node);
            bfs_path_found(neighbor_node);
            return true;
        } else if (neighbor.type() == tile_type::SPACE) {
            neighbor.type(tile_type::FRINGE);
            current_bfs.add_node(neighbor, current_node);

            std::cout << "\t\tMoving" << std::endl;
        }
    }

    return false;
}

static bool bfs2_iteration(bfs& bfs1, bfs& bfs2) {
    return bfs_iteration(bfs1, bfs2) || bfs_iteration(bfs2, bfs1);
}

void init() {
    glClearColor(0.8, 0.7, 0.5, 0);// color of window background
    srand(time(nullptr));

    generate_maze(maze);
    bfs_map_initialize(bfs_node_map, maze);

    reset_maze(maze);
    bfs_map_clear(bfs_node_map);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer
    draw_maze(maze);
    glutSwapBuffers(); // show all
}

void idle() {
    if (bfs_run_requested) {
        bfs_run_requested = false;
        reset();
        run_bfs = true;
    }

    if (run_bfs) {
        if (!first_bfs.has_more_fringes() || !second_bfs.has_more_fringes()) {
            run_bfs = false;
            std::cout << "No more fringes" << std::endl;
        } else {
            run_bfs = !bfs2_iteration(first_bfs, second_bfs);

            if (!run_bfs) {
                std::cout << "Done" << std::endl;
            }
        }
    }

    glutPostRedisplay();
}

void menu(int choice) {
    switch (choice) {
        case 1: //BFS
            bfs_run_requested = true;
            break;
    }

}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(200, 100);
    glutCreateWindow("Window");

    glutDisplayFunc(display);
    glutIdleFunc(idle);

    glutCreateMenu(menu);
    glutAddMenuEntry("2direct-BFS", 1);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    init();

    glutMainLoop();
}
