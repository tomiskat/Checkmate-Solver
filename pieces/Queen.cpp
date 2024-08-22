#include "Queen.h"

/**
 * @brief Vectors representing the possible moves of the Queen
 */
const std::vector<Vector2D> Queen::vectorMoves_ = {
        Vector2D(1, 0),   // piece_move right
        Vector2D(-1, 0),  // piece_move left
        Vector2D(0, 1),   // piece_move up
        Vector2D(0, -1),  // piece_move down
        Vector2D(1, 1),   // piece_move diagonally up-right
        Vector2D(1, -1),  // piece_move diagonally down-right
        Vector2D(-1, 1),  // piece_move diagonally up-left
        Vector2D(-1, -1)  // piece_move diagonally down-left
};