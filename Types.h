#ifndef TYPES_H
#define TYPES_H

#include <cstdlib>

/**
 * @brief Enum class for representing colors.
 * @details The enum class has two values, WHITE and BLACK.
 */
enum class Color {
    WHITE,
    BLACK
};


/**
 * @brief Enum class for representing piece types.
 * @details The enum class has six values, PAWN, BISHOP, KNIGHT, ROOK, QUEEN and KING.
 */
enum class PieceType {
    PAWN,
    BISHOP,
    KNIGHT,
    ROOK,
    QUEEN,
    KING
};


/**
 * @brief Struct representing 2D vector.
 */
struct Vector2D {
    int moveX; ///< X direction of the vector
    int moveY; ///< Y direction of the vector


    /**
     * @brief Constructor for Vector2D.
     * @param x moveX of the vector.
     * @param y moveY of the vector.
     */
    Vector2D(int x, int y) : moveX(x), moveY(y) {}


    /**
     * @brief Equality operator for comparing two Vector2D objects.
     * @param other Vector2D object to compare with.
     * @return True if the two Vector2D objects are equal, false otherwise.
     */
    bool operator==(const Vector2D& other) const {
        return moveX == other.moveX && moveY == other.moveY;
    }


    /**
     * @brief Multiplication operator for Vector2D with a scalar.
     * @param scalar Scalar value to multiply the vector with.
     * @return A new Vector2D object that is the result of the multiplication.
     */
    Vector2D operator*(int scalar) const {
        return {scalar * moveX, scalar * moveY};
    }


    /**
     * @brief Checks if the vector could block a check.
     * @return True if the vector is aligned with X or Y axis or diagonal.
     */
    bool couldBlockCheck() const {
        return moveX == 0 || moveY == 0 || abs(moveX) == abs(moveY);
    }


    /**
     * @brief modify vector to have unit length. Incompatible with Knight.
     */
    void normalize() {
        moveX = (moveX == 0) ? 0 : moveX / abs(moveX);
        moveY = (moveY == 0) ? 0 : moveY / abs(moveY);
    }
};


/**
 * @brief Struct for representing a position on a 2D board.
 */
struct Position {
    int x_; ///< X coordinate of the position
    int y_; ///< Y coordinate of the position
    PieceType transformTo_; ///< Piece type to transform to


    /**
     * @brief Constructor for Position.
     * @param x X coordinate of the position.
     * @param y Y coordinate of the position.
     */
    Position(int x, int y) : x_(x), y_(y) {}


    /**
     * @brief Default constructor for Position.
     * Initializes the position with -1 for invalid position.
     */
    Position() : x_(-1), y_(-1) {}


    /**
     * @brief Constructor for Position with transformation.
     * @param other The position to transform to.
     * @param transformTo The type of piece to transform to.
     */
    Position(const Position& other, PieceType transformTo) :
            x_(other.x_), y_(other.y_), transformTo_(transformTo) {}


    /**
     * @brief Adds a vector to the position.
     * @param vector The vector to add to the position.
     * @return updated Position.
     */
    Position& operator+=(const Vector2D &vector) {
        x_ += vector.moveX;
        y_ += vector.moveY;
        return *this;
    }


    /**
     * @brief Sets the transformation for the position.
     * @param pieceType The type of piece to transform to.
     */
    void setTransformation(PieceType pieceType) {
        transformTo_ = pieceType;
    }


    /**
     * @brief Equality operator for comparing two Positions.
     * @param other Position to compare with.
     * @return True if the two Positions are equal.
     */
    bool operator==(const Position &other) const {
        return x_ == other.x_ && y_ == other.y_;
    }


    /**
     * @brief Adds a Vector2D to the current position.
     * @param vector The Vector2D to be added.
     * @return The resulting Position after addition.
     */
    Position operator+(const Vector2D &vector) const {
        return {x_ + vector.moveX, y_ + vector.moveY};
    }


    /**
     * @brief Subtracts a Position from the current position.
     * @param position The Position to be subtracted.
     * @return The resulting Vector2D after subtraction.
     */
    Vector2D operator-(const Position &position) const {
        return {x_ - position.x_, y_ - position.y_};
    }


    /**
     * @brief Prints the position to the given output stream.
     * @param os The output stream to print to.
     */
    void print(std::ostream& os = std::cout) const {
        os << static_cast<char>(y_ + 'A');
        os << (8 - x_);
    }


    /**
     * @brief Overloaded operator for printing the position to the given output stream.
     * @param os output stream to print to
     * @param position position to print
     * @return output stream
     */
    friend std::ostream& operator<<(std::ostream& os, const Position& position) {
        position.print(os);
        return os;
    }
};

#endif //TYPES_H
