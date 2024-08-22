#include "Piece.h"
#include "../Chess.h"


/**
 * @brief Check if the piece gives check
 * @param chess chess logic
 * @return true if the piece gives check
 */
bool Piece::givesCheck(const Chess &chess) const {
    // get vector in which piece_move is located enemy king
    Vector2D vector = chess.enemyKingPosition(color_) - position_;

    // end if the vector cannot be in the direction of the enemy king
    if (!vector.couldBlockCheck()) {
        return false;
    }
    vector.normalize();

    // end if the vector is not in the list of possible moves
    if (std::find(vectorMoves_.begin(), vectorMoves_.end(), vector) == vectorMoves_.end()) {
        return false;
    }

    // while there is no piece on the way keep moving
    Position newPosition = position_ + vector;
    while (chess.isFree(newPosition)) {
        newPosition += vector;
    }
    return newPosition == chess.enemyKingPosition(color_);
}


/**
 * @brief Get positions on which the piece can move
 * @param chess chess logic
 * @return positions of the piece
 */
std::vector<Position> Piece::getPossibleMoves(Chess &chess) const {
    std::vector<Position> positions;
    Position newPosition;

    // if piece blocks check
    if (chess.pieceBlocksCheck(position_, color_, positions)) {
        return positions;
    }
    positions.clear();

    for (const Vector2D& move : vectorMoves_) {
        newPosition = position_ + move;

        // while there is no piece on the way
        while (Chess::onChessboard(newPosition) && chess.isFree(newPosition)) {
            positions.push_back(newPosition);
            newPosition += move;
        }

        // if there is a piece on the way, and it is an enemy piece
        if (Chess::onChessboard(newPosition) && chess.canCapture(newPosition, color_)) {
            positions.push_back(newPosition);
        }
    }
    return positions;
}


/**
 * @brief printBoard piece to output stream
 * @param os output stream
 */
void Piece::print(std::ostream &os) const {
    os << position_;
    os << (color_ == Color::WHITE ? " white " : " black ");

    switch (pieceType_) {
        case PieceType::PAWN:
            os << "pawn";
            break;
        case PieceType::ROOK:
            os << "rook";
            break;
        case PieceType::KNIGHT:
            os << "knight";
            break;
        case PieceType::BISHOP:
            os << "bishop";
            break;
        case PieceType::QUEEN:
            os << "queen";
            break;
        case PieceType::KING:
            os << "king";
            break;
    }
    os << std::endl;
}
