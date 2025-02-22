#ifndef CHESS_H
#define CHESS_H

#include <fstream>
#include <sstream>
#include <climits>
#include "pieces/Piece.h"
#include "pieces/King.h"
#include "pieces/Queen.h"
#include "pieces/Rook.h"
#include "pieces/Bishop.h"
#include "pieces/Knight.h"
#include "pieces/Pawn.h"
#include "pieces/PawnWhite.h"
#include "pieces/PawnBlack.h"
#include "Exception.h"

using piece_ptr = std::shared_ptr<Piece>;
using piece_move = std::pair<Position, Position>;
using piece_moves = std::vector<piece_move>;

// check bonus
static const size_t GIVES_CHECK_BONUS = 5;      ///< Bonus for giving check

// minimax settings
static const size_t PRUNING_SIZE = INT_MAX;     ///< Number of best moves to consider in minimax
static const size_t SEARCH_DEPTH = 3;           ///< Number of moves to search in minimax
static const bool ADD_CHECKMATE_MOVES = false;  ///< Add checkmate moves to the list


/**
 * @brief Class representing a chess game.
 */
class Chess {
private:
    piece_ptr chessBoard_[8][8];  ///< 2D array representing the chess board
    piece_ptr whiteKing_;         ///< White king on the board
    piece_ptr blackKing_;         ///< Black king on the board

    // moves
    piece_move bestStartingMove_;             ///< Best updatePosition for the computer
    piece_moves minimaxMoves_;                ///< Vector of moves searched in minimax tree
    std::vector<piece_moves> checkMateList_;  ///< Vector of vector off pairs positions leading to dealCheckmateFound

    // minimax settings
    size_t searchDepth_ = 2 * SEARCH_DEPTH;         ///< User search depth
    size_t pruningSize_ = PRUNING_SIZE;             ///< Search depth
    bool addCheckmateMoves_ = ADD_CHECKMATE_MOVES;  ///< Add checkmate moves to the list


public:
    /**
     * @brief Default constructor
     */
    Chess() = default;


    /**
     * @brief Check that given position is on the chessboard
     * @param position position to check
     * @return true if given position is on the chessboard
     */
    static bool onChessboard(const Position &position) {
        int x = position.x_;
        int y = position.y_;
        return x >= 0 && x <= 7 && y >= 0 && y <= 7;
    }


    /**
     * @brief Check if given position is free
     * @param position position to check
     * @return true if given position is free
     */
    bool isFree(const Position &position) const {
        int x = position.x_;
        int y = position.y_;
        return chessBoard_[x][y] == nullptr;
    }


    /**
     * @brief Check if given position is occupied by capturable enemy
     * @param newPosition position to check
     * @param myColor color of moving piece
     * @return true if given position is occupied by capturable enemy
     */
    bool canCapture(const Position &newPosition, Color myColor) const {
        return isEnemy(newPosition, myColor) && enemyKingPosition(myColor) != newPosition;
    }


    /**
     * @brief Check if piece blocks check on given king color (piece can also block check of own color)
     * @param piecePosition position of piece
     * @param kingColor color of king to give check
     * @param positions empty vector of positions to fill by positions which might be check-blocking
     * @return true if piece blocks check
     */
    bool pieceBlocksCheck(const Position &piecePosition, Color kingColor, std::vector<Position> &positions) const;


    /**
     * @brief Check if there is an enemy piece on given position
     * @param position position to check
     * @param myColor my color
     * @return true if there is an enemy piece on given position
     */
    bool isEnemy(const Position &position, Color myColor) const {
        int x = position.x_;
        int y = position.y_;
        return chessBoard_[x][y]->getColor() != myColor;
    }


    /**
     * @brief Get the position of the player's own king.
     * @param myColor The color of the player's pieces.
     * @return A constant reference to the position of the player's own king.
     */
    const Position &myKingPosition(Color myColor) const {
        return myColor == Color::WHITE ? whiteKing_->getPosition() : blackKing_->getPosition();
    }


    /**
     * @brief Get the position of the enemy's king.
     * @param myColor The color of the player's pieces.
     * @return A constant reference to the position of the enemy's king.
     */
    const Position &enemyKingPosition(Color myColor) const {
        return myColor != Color::WHITE ? whiteKing_->getPosition() : blackKing_->getPosition();
    }


    /**
     * @brief Check if the kings are neighbors.
     * @return True if kings neighbor
     */
    bool kingsAreNeighbours() const {
        Position whitePosition = whiteKing_->getPosition();
        Position blackPosition = blackKing_->getPosition();
        return (abs(blackPosition.x_ - whitePosition.x_) <= 1 && abs(blackPosition.y_ - whitePosition.y_) <= 1);
    }


    /**
     * @brief Check if kings become neighbours or king gets checked after move to new position.
     * @param newPosition The new position for the king.
     * @param myColor The color of the player's pieces.
     * @return True if kings become neighbors or king gets checked.
     */
    bool kingsNeighboursOrCheck(const Position &newPosition, Color myColor);


    /**
     * @brief Check if king is in check
     * @param myColor king color
     * @return true if king is in check
     */
    bool kingHasCheck(Color myColor);


    /**
     * @brief Check if both kings exist on the chessboard.
     */
    void validateKings();


    /**
     * @brief Check if a given position is occupied by the enemy's king.
     * @param position The position to check.
     * @param color The color of the player's pieces.
     * @return True if the given position is occupied by the enemy's king, false otherwise.
     */
    bool isEnemyKing(const Position &position, Color color) const {
        return position == enemyKingPosition(color);
    }


    /**
     * load a game from file
     * @param fileName name of file
     */
    void loadGame(const std::string &fileName);


    /**
     * @brief Load a game from inputs stream.
     * @param inputStream The input stream to read from.
     */
    void loadGame(std::istream &inputStream = std::cin);


    /**
     * @brief load game from FEN code
     * @param FENCode FEN code of game
     */
    void loadFENGame(const std::string &FENCode);


    /**
     * @brief load color of piece
     * @param color string color of piece
     * @return enum value of color
     */
    static Color loadColor(const std::string &color);


    /**
     * @brief load position of piece
     * @param position string position of piece
     * @return Position of piece
     */
    Position loadPosition(const std::string &position) const;


    /**
     * @brief Create a piece on the chess board.
     * @param position The position of the piece.
     * @param color The color of the piece.
     * @param pieceType The type of the piece.
     */
    void createPiece(const Position &position, Color color, const std::string &pieceType);


     /**
      * @brief Check if color can give checkmate in searchDepth moves
      * @param colorOnMove color of player on move
      * @param searchDepth number of one color moves to check in minimax
      * @param addCheckMateMoves if true, checkmate moves will be add checkMateList_
      * @param pruningSize number of moves to consider in minimax
      * @return true if colorOnMove can give checkmate in searchDepth moves
      */
    bool findCheckMate(Color colorOnMove, size_t searchDepth = SEARCH_DEPTH, bool addCheckMateMoves = ADD_CHECKMATE_MOVES,
                       size_t pruningSize = PRUNING_SIZE);


    /**
     * @minimax algorithm to find checkmate move, more here @url https://www.youtube.com/watch?v=l-hh51ncgDI&t=294s
     * @param colorOnMove color of player on move
     * @param searchDepth search depth
     * @param alpha best reached value of maximizing player
     * @param beta best reached value of minimizing player
     * @return value of best move
     */
    int minimax(Color colorOnMove, size_t searchDepth, int alpha = INT_MIN, int beta = INT_MAX);


    /**
     * @brief Find best move for maximizing player == white
     * @param searchDepth search depth
     * @param alpha current best reached value of maximizing playerr
     * @param beta best possible value for maximizing player
     * @return value of best move
     */
    int maximizer(size_t searchDepth, int alpha, int beta);


    /**
     * @brief Find best move for minimizing player == black
     * @param searchDepth search depth
     * @param alpha best possible value for minimizing player
     * @param beta current best reached value of minimizing player
     * @return
     */
    int minimizer(size_t searchDepth, int alpha, int beta);


    /**
     * @brief Move piece on chessboard
     * @param move move to do
     */
    void movePiece(const piece_move &move);


    /**
     * @brief Get all pieces of color
     * @param color color if pieces to get
     * @return vector of pieces
     */
    std::vector<piece_ptr> getPieces(Color color) const;


    /**
     * @brief Get best moves for color -> they are chosen using quickEvaluation
     * @param color color of player on move
     * @return vector of best moves
     */
    std::vector<piece_move> getBestMoves(Color color);


    /**
     * @brief Get all moves for color
     * @param color color of player on move
     * @return vector of all moves
     */
    std::vector<piece_move> getAllMoves(Color color);


    /**
     * @brief quick evaluation of move, capturing moves or moves giving checkmate are preferred
     * @param move move to evaluate
     * @return value of move
     */
    int quickEvaluation(const piece_move &move);


    /**
     * @brief bonus for capturing enemy piece
     * @param position position to move
     * @return value of captured piece or 0 if no piece is captured
     */
    int captureEnemyBonus(const Position &position) {
        if (!isFree(position)) {
            return chessBoard_[position.x_][position.y_]->getValue();
        }
        return 0;
    };


    /**
     * @brief bonus for moving piece to better position, we prefer positions closer to enemy king
     * @param move move to evaluate
     * @return value of move
     */
    int betterPositionBonus(const piece_move &move);


    /**
     * @brief bonus for color on move, used in deepEvaluation
     * @param color color of player on move
     * @return 1 if white is on move, -1 if black is on move
     */
    int colorOnMoveBonus(Color color) {
        return color == Color::WHITE ? 1 : -1;
    }


    /**
     * @brief count distance between two positions
     * @param position1 position of first piece
     * @param position2 position of second piece
     * @return distance between two positions
     */
    int countDistance(const Position &position1, const Position &position2) const {
        return std::abs(position2.x_ - position1.x_) + std::abs(position2.y_ - position1.y_);
    }


    /**
     * @brief evaluate game position if we reached end of searchDepth_
     * @param colorOnMove
     * @return
     */
    int deepEvaluation(Color colorOnMove);


    /**
     * @brief evaluate piece based on its value and number of possible moves, method called by deepEvaluation
     * @param piece piece to evaluate
     * @return value of piece
     */
    int evaluatePiece(const piece_ptr &piece) {
        int value = 0;
        value += piece->getValue();
        value += piece->getPossibleMoves(*this).size();

        return (piece->getColor() == Color::WHITE) ? value : value * -1;
    }


    /**
     * @brief bonus for giving check
     * @param piece piece to evaluate
     * @return GIVES_CHECK_BONUS if piece gives check, 0 otherwise
     */
    int givesCheckBonus(const piece_ptr &piece) const {
        if (piece->givesCheck(*this)) {
            return GIVES_CHECK_BONUS;
        }
        return 0;
    }


    /**
     * @brief deal situation when checkmate was found
     */
    void dealCheckmateFound() const;


    /**
     * @brief deal situation when checkmate was not found
     * @param evaluation evaluation of game position
     */
    void dealCheckmateNotFound(int evaluation) const;


    /**
     * @brief print game analysis
     * @param evaluation evaluation of game position
     */
    static void printGameAnalysis(int evaluation);


    /**
     * @brief print recorded moves that lead to checkmate
     */
    void printCheckMateMoves() const;


    /**
     * @brief transform pawn to another piece, position holds pieceType to transform to
     * @param position position of pawn to transform
     */
    void transformPawn(const Position &position);


    /**
     * @brief check if position is valid
     * @param position position to check
     */
    void validatePosition(const Position &position) const;

    
    /**
     * @brief check if FEN code is valid
     * @param column column in FEN code
     */
    void checkFENColumn(int column) {
        if (column != 8) {
            throw InvalidFENFormat();
        }
    }


    /**
     * @brief check if FEN code is valid
     * @param row row in FEN code
     */
    void checkFENRow(int row) {
        if (row != 7) {
            throw InvalidFENFormat();
        }
    }


    /**
     * @brief return opposite color
     * @param color color to get opposite
     * @return opposite color
     */
    Color getOppositeColor(Color color) const {
        return color == Color::WHITE ? Color::BLACK : Color::WHITE;
    }


    /**
     * @brief check if king is checked by enemy knight
     * @param kingColor color of king
     * @param checkingPiece empty position of checking piece -> will be set if king is checked
     * @return true if king is checked by enemy knight
     */
    bool isCheckedByKnight(Color kingColor, Position &checkingPiece);


    /**
     * @brief check if king is checked by enemy pawn
     * @param kingColor color of king
     * @param checkingPiece empty position of checking piece -> will be set if king is checked
     * @return true if king is checked by enemy knight
     */
    bool isCheckedByPawn(Color kingColor, Position &checkingPiece);


    /**
     * @brief check if king is checked by enemy bishop, queen or rook
     * @param kingColor color of king
     * @param checkCount number of checking pieces -> will be increment for each checking piece
     * @param checkingPiece empty position of checking piece -> will be set if king is checked (we only use this
     *                      position if total number of pieces that gives check == 1, more in needsToBlockCheck)
     *
     * @return true if king is checked by enemy bishop, queen or rook
     */
    bool hasBlockAbleCheck(Color kingColor, int &checkCount, Position &checkingPiece);


    /**
     * @brief check that piece can move in given vector -> called always on pieces that have blockable
     * check (queen, rook, bishop)
     */
    static bool canMoveDirection(const piece_ptr &piece, const Vector2D &vector) {
        std::vector<Vector2D> pieceMoves = piece->getVectorMoves();
        return std::find(pieceMoves.begin(), pieceMoves.end(), vector) != pieceMoves.end();
    }


    /**
     * @brief check if king is checked by enemy
     * @param colorOnMove color of player on move
     * @param moves empty vector of moves -> will be filled by moves that can block check
     * @return true if king is checked by enemy
     */
    bool needsToBlockCheck(Color colorOnMove, std::vector<piece_move> &moves);


    /**
     * @brief add moves of king to vector of moves
     * @param kingColor color of king
     * @param moves vector of moves -> will be filled by possible moves of king
     */
    void addKingMoves(Color kingColor, std::vector<piece_move> &moves);


    /**
     * @brief add moves of pieces of given color to enemy checking piece position
     * @param colorOnMove color of player on move
     * @param enemyCheckingPiece position of enemy checking piece
     * @param moves vector of moves -> will be filled by possible moves of pieces of given color
     */
    void addMovesAtPosition(Color colorOnMove, const Position &enemyCheckingPiece, std::vector<piece_move> &moves);


    /**
     * @brief add moves of pieces to enemy checking piece and between player´s king and enemy checking piece
     * @param myColor color of player on move
     * @param checkingPiece position of enemy checking piece
     * @param moves vector of moves -> will be filled by possible moves of pieces of player´s color
     */
    void addMovesAtPositionAndBetween(Color myColor, const Position &checkingPiece, std::vector<piece_move> &moves);


    /**
     * @brief add current minimaxMoves_ to checkMateList_
     */
    void addCheckMateMove() {
        checkMateList_.push_back(minimaxMoves_);
    }


    /**
     * @brief clear checkMateList_ -> called if there is higher leaf in minimax which does not lead to checkmate
     */
    void resetCheckMateMove() {
        checkMateList_.clear();
    }


    /**
     * @brief print chessBoard_ to file
     * @param fileName name of file to print to
     */
    void printBoard(const std::string &fileName);


    /**
     * @brief print chessBoard_ to output stream
     * @param outputStream output stream to print to
     */
    void printBoard(std::ostream &outputStream = std::cout) const;


    /**
     * @brief Overload operator<< to print chess to output stream
     * @param os output stream to print to
     * @param chess chess to print
     * @return output stream
     */
    friend std::ostream& operator<<(std::ostream& os, const Chess& chess) {
        chess.printBoard(os);
        return os;
    }


    /**
     * @brief check if move will give check
     * @param move move to try
     * @return true if move will give check
     */
    int willBeCheckBonus(const piece_move &move);


    /**
     * @brief setup minimax, if parameter is not specified by user, default value will be used
     * @param searchDepth number of one color moves to check in minimax
     * @param addCheckmateMoves if true, checkmate moves will be added to checkMateList_
     * @param pruningSize number of moves to consider in minimax
     */
    void setupMinimax(size_t searchDepth, bool addCheckmateMoves, size_t pruningSize);


    /**
     * @brief Overload operator<< to print piece move to output stream
     * @param os output stream to print to
     * @param move move to print
     * @return output stream
     */
    friend std::ostream& operator<<(std::ostream& os, const piece_move& move);


    /**
     * @brief return vector piece_moves that lead to checkmate
     * @return
     */
    const std::vector<piece_moves> &getCheckmateMoves() const {
        return checkMateList_;
    }


    /**
     * @brief return best move
     * @return best move found by minimax
     */
    const piece_move &getBestMove() const {
        return bestStartingMove_;
    }
};


#endif //CHESS_H
