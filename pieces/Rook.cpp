#include "Rook.h"

/**
 * Vectors representing the possible moves of the Rook
 */
const std::vector<Vector2D> Rook::vectorMoves_ = {
        Vector2D(1, 0),   // piece_move right
        Vector2D(-1, 0),  // piece_move left
        Vector2D(0, 1),   // piece_move up
        Vector2D(0, -1),  // piece_move down
};