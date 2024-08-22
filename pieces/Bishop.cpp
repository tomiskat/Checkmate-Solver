#include "Bishop.h"

/**
 * Vectors representing the possible moves of the Bishop
 */
const std::vector<Vector2D> Bishop::vectorMoves_ = {
        Vector2D(1, 1),   // piece_move diagonally up-right
        Vector2D(1, -1),  // piece_move diagonally down-right
        Vector2D(-1, 1),  // piece_move diagonally up-left
        Vector2D(-1, -1)  // piece_move diagonally down-left
};
