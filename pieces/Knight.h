#ifndef KNIGHT_H
#define KNIGHT_H

#include "Piece.h"
#include <vector>

/**
 * @brief Class to represent Knight on chessboard, extends Piece
 */
class Knight final : public Piece {
private:
    /// Vectors representing the possible knight moves
    static const std::vector<Vector2D> vectorMoves_;
    static const int value_ = 3;

public:
    /**
     * Constructor
     * @param color color of Knight
     */
    explicit Knight(Color color, const Position &position) : Piece(color, PieceType::KNIGHT, value_, vectorMoves_, position) {}


    /**
     * Check if the piece check can be blocked by another piece
     * @return true if the piece check can be blocked by another piece
     */
    bool isCheckBlockAble() const override {
        return false;
    }


    /**
     * Check if the piece gives check
     * @param chess chess logic
     * @return true if the piece gives check
     */
    bool givesCheck(const Chess &chess) const override;


    /**
     * Get available moves of the piece
     * @param chess chess logic
     * @return moves of the piece
     */
    std::vector<Position> getPossibleMoves(Chess &chess) const override;


    /**
     * @brief Get vector of possible moves
     * @return vector of possible moves
     */
    static const std::vector<Vector2D>& getVectorMoves() {
        return vectorMoves_;
    }
};



#endif // KNIGHT_H
