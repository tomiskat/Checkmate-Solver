#include "Knight.h"
#include "../Chess.h"

/**
 * Vectors representing the possible knight moves
 */
const std::vector<Vector2D> Knight::vectorMoves_ = {
        Vector2D(2, 1),   // Move 2 steps forward and 1 step to the right
        Vector2D(1, 2),   // Move 1 step forward and 2 steps to the right
        Vector2D(-2, 1),  // Move 2 steps backward and 1 step to the right
        Vector2D(-1, 2),  // Move 1 step backward and 2 steps to the right
        Vector2D(2, -1),  // Move 2 steps forward and 1 step to the left
        Vector2D(1, -2),  // Move 1 step forward and 2 steps to the left
        Vector2D(-2, -1), // Move 2 steps backward and 1 step to the left
        Vector2D(-1, -2)  // Move 1 step backward and 2 steps to the left
};


/**
 * Check if the piece gives check
 * @param chess chess logic
 * @return true if the piece gives check
 */
bool Knight::givesCheck(const Chess &chess) const {
    for (const auto &move: vectorMoves_) {
        Position newPosition = position_ + move;
        if (Chess::onChessboard(newPosition) && chess.isEnemyKing(newPosition, color_)) {
            return true;
        }
    }
    return false;
}


/**
 * Get available moves of the piece
 * @param chess chess logic
 * @return moves of the piece
 */
std::vector<Position> Knight::getPossibleMoves(Chess &chess) const {
    std::vector<Position> positions;
    Position newPosition;

    // if knight is blocking check, it can't updatePosition
    if (chess.pieceBlocksCheck(position_, color_, positions)) {
        return {};
    }
    positions.clear();

    for (const auto &move: vectorMoves_) {
        newPosition = position_ + move;
        if (Chess::onChessboard(newPosition) &&
            (chess.isFree(newPosition) || chess.canCapture(newPosition, color_))) {
                positions.push_back(newPosition);
        }
    }
    return positions;
}
