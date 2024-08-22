#ifndef QUEEN_H
#define QUEEN_H

#include "Piece.h"
#include <vector>

/**
 * @brief Class to represent Queen on chessboard, extends Piece
 */
class Queen final : public Piece {
private:
    /// Vectors representing the possible queen moves
    static const std::vector<Vector2D> vectorMoves_;
    const static int value_ = 9;

public:
    /**
     * Constructor
     * @param color color of Queen
     */
    explicit Queen(Color color, Position position) : Piece(color, PieceType::QUEEN, value_, vectorMoves_, position) {}


    /**
     * @brief Get vector of possible moves
     * @return vector of possible moves
     */
    static const std::vector<Vector2D>& getVectorMoves() {
        return vectorMoves_;
    }
};


#endif // QUEEN_H
