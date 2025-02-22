#ifndef PIECE_H
#define PIECE_H

#include <algorithm>
#include <string>
#include <utility>
#include <vector>
#include <iostream>
#include <memory>
#include "../Types.h"

/// forward declaration
class Chess;

/**
 * @brief class to represent pieces on the chessboard
 */
class Piece {
protected:
    const Color color_;                         ///< Color of the piece
    const PieceType pieceType_;                 ///< Type of the piece
    int value_;                                 ///< value of the piece
    const std::vector<Vector2D> vectorMoves_;   ///< Vector of possible moves
    Position position_;                         ///< position of the piece

public:
    /**
     * @brief Constructor
     * @param color color of Piece
     * @param pieceType type of Piece
     */
    Piece(Color color, PieceType pieceType, int value, const std::vector<Vector2D> &moves, const Position &position) :
            color_(color), pieceType_(pieceType), value_(value), vectorMoves_(moves), position_(position) {}


    /**
     * @brief Destructor
     */
    ~Piece() = default;


    /**
     * @brief Get color of piece
     * @return color of piece
     */
    Color getColor() const {
        return color_;
    }


    /**
     * @brief Get type of piece
     * @return type of piece
     */
    PieceType getPieceType() const {
        return pieceType_;
    }


    /**
     * @brief Get moves of piece
     * @return moves of piece
     */
    const std::vector<Vector2D>& getVectorMoves() const {
        return vectorMoves_;
    }


    /**
     * @brief Get position of piece
     * @return position of piece
     */
    const Position &getPosition() const {
        return position_;
    }

    /**
     * @brief Get value of piece
     * @return value of piece
     */
    int getValue() const {
        return value_;
    }


    /**
     * @brief Check if the piece check can be blocked by another piece
     * @return true if the piece check can be blocked by another piece
     */
    virtual bool isCheckBlockAble() const {
        return true;
    }


    /**
     * @brief Check if the piece can updatePosition in the given direction
     * @param vector vector representing the direction
     * @return true if the piece can updatePosition in the given direction
     */
    bool canMoveDirection(const Vector2D &vector) const {
        return std::any_of(vectorMoves_.begin(), vectorMoves_.end(),
                           [&vector](const Vector2D& v) { return v == vector; });
    }




    /**
     * @brief Check if the piece gives check
     * @param chess chess logic
     * @return true if the piece gives check
     */
    virtual bool givesCheck(const Chess &chess) const;


    /**
     * @brief Get available positions of the piece
     * @param chess chess logic
     * @return positions of the piece
     */
    virtual std::vector<Position> getPossibleMoves(Chess &chess) const;


    /**
     * @brief printBoard piece to output stream
     * @param os output stream
     */
    void print(std::ostream& os = std::cout) const;


    /**
     * @brief Overload operator<< to print piece
     * @param os output stream
     * @return output stream with printed piece
     */
    friend std::ostream& operator<<(std::ostream& os, const Piece& piece) {
        piece.print(os);
        return os;
    }


    /**
     * @brief Update position of piece
     * @param position new position
     */
    void updatePosition(const Position &position) {
        position_ = position;
    }
};

#endif // PIECE_H
