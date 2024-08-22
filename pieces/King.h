#ifndef KING_H
#define KING_H

#include <vector>
#include "Piece.h"

/**
 * @brief Class to represent King on chessboard, extends Piece
 */
class King final : public Piece {
private:
    /// Vectors representing the possible king moves
    static const std::vector<Vector2D> vectorMoves_;
    static const int value_ = 1;  // Value of the King is not important

public:
    /**
     * Constructor
     * @param color color of King
     */
    explicit King(Color color, const Position &position) : Piece(color, PieceType::KING, value_, vectorMoves_, position) {}


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
    bool givesCheck(const Chess &chess) const override {
        return false;
    }


    /**
     * Get available positions of the piece
     * @param chess chess logic
     * @return positions of the piece
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


#endif //KING_H
