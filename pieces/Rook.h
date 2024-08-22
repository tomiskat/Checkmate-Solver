#ifndef ROOK_H
#define ROOK_H

#include "Piece.h"
#include <vector>


/**
 * @brief Class to represent Rook on chessboard, extends Piece
 */
class Rook final : public Piece {
private:
    /// Vectors representing the possible rook moves
    static const std::vector<Vector2D> vectorMoves_;
    static const int value_ = 5;

public:
    /**
     * Constructor
     * @param color color of Rook
     */
    explicit Rook(Color color, Position position) : Piece(color, PieceType::ROOK, value_, vectorMoves_, position) {}


    /**
     * @brief Get vector of possible moves
     * @return vector of possible moves
     */
    static const std::vector<Vector2D>& getVectorMoves() {
        return vectorMoves_;
    }
};




#endif //ROOK_H
