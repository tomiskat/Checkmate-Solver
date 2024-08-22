#ifndef PAWNWHITE_H
#define PAWNWHITE_H

#include "Pawn.h"

/**
 * @brief Class to represent White Pawn on chessboard, extends Pawn
 */
class PawnWhite final : public Pawn {
private:
    /// Vectors representing the possible white pawn moves
    static const std::vector<Vector2D> regularMoves_;
    static const std::vector<Vector2D> captureMoves_;
    static const int startingRow_ = 6;

public:
    /**
     * Constructor
     */
    PawnWhite(const Position &position) : Pawn(Color::WHITE, regularMoves_, captureMoves_, startingRow_, position) {}


    /**
     * @brief Get vector of regular moves
     * @return vector of regular moves
     */
    static const std::vector<Vector2D>& getCaptureMoves() {
        return captureMoves_;
    }


    /**
     * @brief Get vector of capture moves
     * @return vector of capture moves
     */
    static const std::vector<Vector2D>& getRegularMoves() {
        return regularMoves_;
    }
};




#endif // PAWNWHITE_H