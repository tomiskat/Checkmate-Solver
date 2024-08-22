#ifndef BISHOP_H
#define BISHOP_H

#include "Piece.h"
#include <vector>

/**
 * @brief Class to represent Bishop on chessboard, extends Piece
 */
class Bishop final : public Piece {
private:
    /// Vectors representing the possible bishop moves
    static const std::vector<Vector2D> vectorMoves_;
    static const int value_ = 3;

public:
    /**
     * Constructor
     * @param color color of Bishop
     * @param name name of Bishop
     * @param pieceName used in toString method
     */
    explicit Bishop(Color color, const Position &position) : Piece(color, PieceType::BISHOP, value_, vectorMoves_, position) {}


    /**
     * @brief Get vector of possible moves
     * @return vector of possible moves
     */
    static const std::vector<Vector2D>& getVectorMoves() {
        return vectorMoves_;
    }
};


#endif //BISHOP_H