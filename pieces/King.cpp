#include "King.h"
#include "../Chess.h"

/**
 * Vectors representing the possible moves of the King
 */
const std::vector<Vector2D> King::vectorMoves_ = {
        Vector2D(1, 0),   // Move 1 step to the right
        Vector2D(-1, 0),  // Move 1 step to the left
        Vector2D(0, 1),   // Move 1 step forward
        Vector2D(0, -1),  // Move 1 step backward
        Vector2D(1, 1),   // Move 1 step to the right and 1 step forward (diagonal)
        Vector2D(1, -1),  // Move 1 step to the right and 1 step backward (diagonal)
        Vector2D(-1, 1),  // Move 1 step to the left and 1 step forward (diagonal)
        Vector2D(-1, -1)  // Move 1 step to the left and 1 step backward (diagonal)
};


/**
 * Get available positions of the piece
 * @param chess chess logic
 * @return positions of the piece
 */
std::vector<Position> King::getPossibleMoves(Chess &chess) const {
    std::vector<Position> positions;

    for (const auto &move: vectorMoves_) {
        Position newPosition = position_ + move;
        if (Chess::onChessboard(newPosition) &&
            (chess.isFree(newPosition) || chess.canCapture(newPosition, color_)) &&
            !chess.kingsNeighboursOrCheck(newPosition, color_)) {
                positions.push_back(newPosition);
        }
    }
    return positions;
}
