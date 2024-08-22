#include "PawnBlack.h"

/**
 * Vectors whose directions Pawn can updatePosition forward
 */
const std::vector<Vector2D> PawnBlack::regularMoves_ = {
        Vector2D(1, 0),  // Move forward one step
        Vector2D(2, 0),  // Move forward two steps (only allowed for initial piece_move)
};


/**
 * Vectors whose directions Pawn can capture enemy piece
 */
const std::vector<Vector2D> PawnBlack::captureMoves_ = {
        Vector2D(1, 1),  // Move diagonally to capture opponent's piece (right)
        Vector2D(1, -1)  // Move diagonally to capture opponent's piece (left)
};


