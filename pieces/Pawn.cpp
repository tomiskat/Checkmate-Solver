#include "Pawn.h"
#include "../Chess.h"

/**
 * Check if the piece gives check
 * @param chess chess logic
 * @return true if the piece gives check
 */
bool Pawn::givesCheck(const Chess &chess) const {
    // pawn can give check only diagonally
    for (Vector2D move : captureMoves_) {
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
std::vector<Position> Pawn::getPossibleMoves(Chess &chess) const {
    std::vector<Position> positions;

    // if piece blocks check
    if (chess.pieceBlocksCheck(position_, color_, positions)) {
        return pawnBlocksCheckMoves(chess, positions);
    }
    positions.clear();

    checkRegularMoves(chess, positions);
    checkCaptureMoves(chess, positions);
    return positions;
}

/**
 * Get available moves of the piece when it blocks check
 * @param chess chess logic
 * @return moves of the piece
 */
std::vector<Position> Pawn::pawnBlocksCheckMoves(Chess &chess, const std::vector<Position> &chessBlocking) const {
    std::vector<Position> reachable;
    std::vector<Position> reachableChessBlocking;
    checkRegularMoves(chess, reachable);
    checkCaptureMoves(chess, reachable);

    for (const auto &position : reachable) {
        if (std::find(chessBlocking.begin(), chessBlocking.end(), position) != reachable.end()) {
            reachableChessBlocking.push_back(position);
        }
    }
    return reachableChessBlocking;
}


/**
 * @brief Check if pawn can be transformed to another piece, if yes, add positions with new pieceTypes to positions
 * @param position position of the piece
 * @param positions vector of positions
 */
void Pawn::checkTransformation(Position &position, std::vector<Position> &positions) const {
    if (position.x_ == 0 || position.x_ == 7) {
        position.setTransformation(PieceType::QUEEN);

        // add other transformations, queen will be added in caller function
        positions.emplace_back(position, PieceType::KNIGHT);
        positions.emplace_back(position, PieceType::ROOK);
        positions.emplace_back(position, PieceType::BISHOP);
    }
}


/**
 * @brief Check if pawn can updatePosition forward, if yes, add reachable positions to positions
 * @param chess chess logic
 * @param positions positions
 */
void Pawn::checkRegularMoves(Chess &chess, std::vector<Position> &positions) const {
    // check regular updatePosition forward by 1
    Position newPosition = position_ + regularMoves_[0];
    if (chess.isFree(newPosition)) {
        checkTransformation(newPosition, positions);
        positions.emplace_back(newPosition);

        // check regular updatePosition forward by 2
        newPosition = position_ + regularMoves_[1];
        if (position_.x_ == startingRow_ && chess.isFree(newPosition)) {
            positions.emplace_back(newPosition);
        }
    }
}


/**
 * @brief Check if pawn can capture enemy, if yes, add reachable positions to positions
 * @param chess chess logic
 * @param positions positions
 */
void Pawn::checkCaptureMoves(Chess &chess, std::vector<Position> &positions) const {
    Position newPosition;

    for (Vector2D move : captureMoves_) {
        newPosition = position_ + move;
        if (Chess::onChessboard(newPosition) && !chess.isFree(newPosition) &&
            chess.canCapture(newPosition, color_)) {
            checkTransformation(newPosition, positions);
            positions.emplace_back(newPosition);
        }
    }
}
