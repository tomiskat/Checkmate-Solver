#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <string>


/**
 * @brief Exception class for indicating that the white king does not exist.
 */
class WhiteKingDoesNotExist : public std::exception {
public:
    /**
     * @brief Constructor for WhiteKingDoesNotExist.
     */
    WhiteKingDoesNotExist() : message("Error: White king does not exist.") {}


    /**
     * @brief Returns the error message associated with the exception.
     * @return A const char pointer to the error message.
     */
    const char* what() const noexcept override {
        return message.c_str();
    }

private:
    std::string message;
};


/**
 * @brief Exception class for indicating that the black king does not exist.
 */
class BlackKingDoesNotExist : public std::exception {
public:
    /**
     * @brief Constructor for BlackKingDoesNotExist.
     */
    BlackKingDoesNotExist() : message("Error: Black king does not exist.") {}


    /**
     * @brief Returns the error message associated with the exception.
     * @return A const char pointer to the error message.
     */
    const char* what() const noexcept override {
        return message.c_str();
    }

private:
    std::string message;
};


/**
 * @brief Exception class for indicating an invalid color.
 */
class InvalidColor : public std::exception {
public:
    /**
     * @brief Constructor for InvalidColor.
     */
    InvalidColor() : message("Error: Invalid color.") {}


    /**
     * @brief Returns the error message associated with the exception.
     * @return A const char pointer to the error message.
     */
    const char* what() const noexcept override {
        return message.c_str();
    }

private:
    std::string message;
};

/**
 * @brief Exception class for indicating an invalid piece type.
 */
class InvalidPieceType : public std::exception {
public:
    /**
     * @brief Constructor for InvalidPieceType.
     */
    InvalidPieceType() : message("Error: Invalid piece type.") {}

    /**
     * @brief Returns the error message associated with the exception.
     * @return A const char pointer to the error message.
     */
    const char* what() const noexcept override {
        return message.c_str();
    }

private:
    std::string message;
};


/**
 * @brief Exception class for indicating an invalid position.
 */
class InvalidPosition : public std::exception {
public:
    /**
     * @brief Constructor for InvalidPosition.
     */
    InvalidPosition() : message("Error: Invalid position.") {}


    /**
     * @brief Returns the error message associated with the exception.
     * @return A const char pointer to the error message.
     */
    const char* what() const noexcept override {
        return message.c_str();
    }

private:
    std::string message;
};


/**
 * @brief Exception class for indicating that a position is already occupied.
 */
class PositionAlreadyOccupied : public std::exception {
public:
    /**
     * @brief Constructor for PositionAlreadyOccupied.
     */
    PositionAlreadyOccupied() : message("Error: Position already occupied.") {}


    /**
     * @brief Returns the error message associated with the exception.
     * @return A const char pointer to the error message.
     */
    const char* what() const noexcept override {
        return message.c_str();
    }

private:
    std::string message;
};


/**
 * @brief Exception class for indicating that kings are neighbours.
 */
class KingsNeighbours : public std::exception {
public:
    /**
     * @brief Constructor for KingsNeighbours.
     */
    KingsNeighbours() : message("Error: Kings cannot be neighbours") {}


    /**
     * @brief Returns the error message associated with the exception.
     * @return A const char pointer to the error message.
     */
    const char* what() const noexcept override {
        return message.c_str();
    }

private:
    std::string message;
};


/**
 * @brief Exception class for indicating error in the FEN string.
 */
class InvalidFENFormat : public std::exception {
public:
    /**
     * @brief Constructor for KingsNeighbours.
     */
    InvalidFENFormat() : message("Error: Invalid FEN format") {}


    /**
     * @brief Returns the error message associated with the exception.
     * @return A const char pointer to the error message.
     */
    const char* what() const noexcept override {
        return message.c_str();
    }

private:
    std::string message;
};


/**
 * @brief Exception class for invalid search depth.
 */
class InvalidSearchDepth : public std::exception {
public:
    /**
     * @brief Constructor for KingsNeighbours.
     */
    InvalidSearchDepth() : message("Error: Search depth cannot be 0") {}


    /**
     * @brief Returns the error message associated with the exception.
     * @return A const char pointer to the error message.
     */
    const char* what() const noexcept override {
        return message.c_str();
    }

private:
    std::string message;
};
#endif //EXCEPTION_H
