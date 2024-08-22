#ifndef PAWNBLACK_H
#define PAWNBLACK_H

#include "Pawn.h"

/**
 * @brief Class to represent Black Pawn on chessboard, extends Pawn
 */
class PawnBlack final : public Pawn {
private:
    /// Vectors representing the possible black pawn moves
    static const std::vector<Vector2D> regularMoves_;
    static const std::vector<Vector2D> captureMoves_;
    static const int startingRow_ = 1;

public:
    /**
     * Constructor
     */
    PawnBlack(const Position &position) : Pawn(Color::BLACK, regularMoves_, captureMoves_, startingRow_, position) {}


    /**
     * @brief Get vector of regular moves
     * @return vector of regular moves
     */
    static const std::vector<Vector2D>& getRegularMoves() {
        return regularMoves_;
    }


    /**
     * @brief Get vector of capture moves
     * @return vector of capture moves
     */
    static const std::vector<Vector2D>& getCaptureMoves() {
        return captureMoves_;
    }
};

#endif // PAWNBLACK_H