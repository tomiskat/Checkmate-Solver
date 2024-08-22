#ifndef PAWN_H
#define PAWN_H

#include "Piece.h"
#include <vector>


/**
 * @brief Class to represent Pawn on chessboard, extends Piece
 */
class Pawn : public Piece {
private:
    /// Vectors representing the possible pawn moves
    const std::vector<Vector2D> regularMoves_;
    const std::vector<Vector2D> captureMoves_;

    const int startingRow_;
    const static int value_ = 1;

public:
    /**
     * Constructor
     * @param color color of Pawn
     */
    explicit Pawn(Color color, const std::vector<Vector2D> &regularMoves, const std::vector<Vector2D> &captureMoves,
                  int startingRow, const Position &position) :
                  Piece(color, PieceType::PAWN, value_, regularMoves, position), regularMoves_(regularMoves),
                  captureMoves_(captureMoves), startingRow_(startingRow) {}


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
     * Get available moves of the piece when it blocks check
     * @param chess chess logic
     * @param chessBlocking vector of positions that block check
     * @return moves at chess-blocking positions
     */
    std::vector<Position> pawnBlocksCheckMoves(Chess &chess, const std::vector<Position> &chessBlocking) const;


    /**
     * @brief Check if pawn can be transformed to another piece, if yes, add positions with new pieceTypes to positions
     * @param position position of the piece
     * @param positions vector of positions
     */
    void checkTransformation(Position &position, std::vector<Position> &positions) const;


    /**
     * @brief Check if pawn can updatePosition forward, if yes, add reachable positions to positions
     * @param chess chess logic
     * @param positions positions
     */
    void checkRegularMoves(Chess &chess, std::vector<Position> &positions) const;


    /**
     * @brief Check if pawn can capture enemy, if yes, add reachable positions to positions
     * @param chess chess logic
     * @param positions positions
     */
    void checkCaptureMoves(Chess &chess, std::vector<Position> &positions) const;
};

#endif //PAWN_H
