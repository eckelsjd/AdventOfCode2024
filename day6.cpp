/**
 * @file day6.cpp
 * @brief Solution for Advent of Code 2024, Day 6 challenge.
 * 
 * This file contains the implementation of the solution for the Day 6 challenge of Advent of Code 2024.
 * The challenge involves navigating a grid with a guard and counting specific positions and obstacles.
 * 
 * The main functions in this file are:
 * - count_token: Counts the occurrences of a specific token in the grid.
 * - get_token_location: Finds the location of a specific token in the grid.
 * - get_guard_path: Simulates the guard's path through the grid, marking positions and detecting loops.
 * - part_one: Solves the first part of the challenge by counting the number of guard positions.
 * - part_two: Solves the second part of the challenge by counting obstacles that create loops.
 * - main: Reads the input grid from a file and calls the functions to solve both parts of the challenge.
 * 
 * @param grid The grid representing the area the guard navigates.
 * @param token The character token to search for in the grid.
 * @return Various return types depending on the function, including counts and modified grids.
 */
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <unordered_map>
#include <array>

using Grid = std::vector<std::string>;
using Coordinate = std::pair<int, int>;
using BarrierHitCount = std::unordered_map<std::string, std::array<size_t, 4>>;

int count_token(Grid grid, char token) {
    int cnt = 0;

    for (int i = 0; i < grid.size(); i++) {
        std::string line = grid[i];

        for (int j = 0; j < line.size(); j++) {
            char c = line[j];
            if (c == token) {
                cnt++;
            }
        }
    }

    return cnt;
}

Coordinate get_token_location(Grid grid, char token) {
    int token_pos_row;
    int token_pos_col;

    // Get starting position
    bool found_token = false;
    for (int i = 0; i < grid.size(); i++) {
        std::string line = grid[i];

        for (int j = 0; j < line.size(); j++) {
            char c = line[j];
            if (c == token) {
                token_pos_row = i;
                token_pos_col = j;
                found_token = true;
                break;
            }
        }

        if (found_token) {
            break;
        }
    }

    Coordinate token_pos = {token_pos_row, token_pos_col};

    return token_pos;
}

Grid get_guard_path(Grid grid) {
    int guard_horizontal = 0;
    int guard_vertical = -1;  // Always starts walking vertically up
    Grid guard_path;
    BarrierHitCount barrier_cnt;  // Track number of times hitting a barrier from each direction [0->top, 1->right, 2->bottom, 3->left]
    bool stuck_in_loop = false;

    std::copy(grid.begin(), grid.end(), std::back_inserter(guard_path));

    Coordinate guard_pos = get_token_location(grid, '^');
    guard_path[guard_pos.first].at(guard_pos.second) = 'X';

    // Walk the guard until they exit the grid (or get stuck in a loop)
    while (true) {
        Coordinate next_pos = {guard_pos.first + guard_vertical, guard_pos.second + guard_horizontal};

        // About to walk off the grid
        if (next_pos.first < 0 || next_pos.first >= grid.size() || next_pos.second < 0 || next_pos.second >= grid[0].size()) {
            break;
        }
        
        // About to hit a barrier (turn right)
        else if (grid[next_pos.first].at(next_pos.second) == '#') {
            std::string barrier_key = std::to_string(next_pos.first) + std::to_string(next_pos.second);
            if (!barrier_cnt.contains(barrier_key)) {
                std::fill(barrier_cnt[barrier_key].begin(), barrier_cnt[barrier_key].end(), 0);
            }

            if (guard_horizontal == 0) {
                if (guard_vertical > 0) {
                    guard_horizontal = -1;
                    barrier_cnt[barrier_key][0] += 1;
                } else {
                    guard_horizontal = 1;
                    barrier_cnt[barrier_key][2] += 1;
                }
                guard_vertical = 0;
            } else {
                if (guard_horizontal > 0) {
                    guard_vertical = 1;
                    barrier_cnt[barrier_key][3] += 1;
                } else {
                    guard_vertical = -1;
                    barrier_cnt[barrier_key][1] += 1;
                }
                guard_horizontal = 0;
            }

            // Break if we are stuck in a loop (more than one hit on a single side)
            for (size_t hit_cnt : barrier_cnt[barrier_key]) {
                if (hit_cnt > 1) {
                    guard_path[0].at(0) = 'O';  // Mark this guard path as a loop
                    stuck_in_loop = true;
                    break;
                }
            }

            if (stuck_in_loop) {
                break;
            }

        }

        // Update guard position and path
        guard_pos.first += guard_vertical;
        guard_pos.second += guard_horizontal;
        guard_path[guard_pos.first].at(guard_pos.second) = 'X';
    }

    return guard_path;
}

int part_one(Grid grid) {
    Grid guard_path = get_guard_path(grid);
    
    // for (auto line : guard_path) {
    //     std::cout << line << std::endl;
    // }

    int num_positions = count_token(guard_path, 'X');
    std::cout << "Part 1) Number of guard positions: " << num_positions << std::endl;

    return 0;
}


int part_two(Grid grid) {
    int loop_cnt = 0;
    Grid grid_cpy;
    std::copy(grid.begin(), grid.end(), std::back_inserter(grid_cpy));

    for (int i = 0; i < grid.size(); i++) {
        std::string line = grid[i];

        for (int j = 0; j < line.size(); j++) {
            char c = line.at(j);

            if (c != '#' && c != '^') {
                char temp = grid_cpy[i].at(j);
                grid_cpy[i].at(j) = '#'; // Add a barrier
                Grid guard_path = get_guard_path(grid_cpy);

                if (guard_path[0].at(0) == 'O') {
                    loop_cnt += 1;
                }

                grid_cpy[i].at(j) = temp;
            }
        }
    }

    std::cout << "Part 2) Number of loop obstacles: " << loop_cnt << std::endl;

    return 0;
}

int main() {
    std::ifstream file("data/day6.txt");
    std::string line;
    Grid grid;

    while (std::getline(file, line)) {
        grid.push_back(line);
    }

    part_one(grid); // Count guard positions
    part_two(grid); // Count obstacles that create loops

    return 0;
}
