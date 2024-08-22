#include "Chess.h"

#include <ranges>

/**
 * @brief Check if piece blocks check on given king color (piece can also block check of own color)
 * @param piecePosition position of piece
 * @param kingColor color of king to give check
 * @param positions empty vector of positions to fill by positions which might be check-blocking
 * @return true if piece blocks check
 */
bool Chess::pieceBlocksCheck(const Position &piecePosition, Color kingColor, std::vector<Position> &positions) const {
    // get vector defined by my piece and my king
    Position kingPosition = this->myKingPosition(kingColor);
    Vector2D vector = piecePosition - kingPosition;

    if (!vector.couldBlockCheck()) {
        return false;
    }
    vector.normalize();

    // Check there is no piece between piece and king
    Position newPosition = kingPosition + vector;
    while (onChessboard(newPosition) && piecePosition != newPosition) {
        if (!isFree(newPosition)) {
            return false;
        }
        positions.push_back(newPosition);
        newPosition += vector;
    }

    // Check that piece is between king and checking-color piece
    newPosition += vector;
    positions.push_back(newPosition);
    while (onChessboard(newPosition)) {
        if (!isFree(newPosition)) {
            if (isEnemy(newPosition, kingColor)) {
                piece_ptr piece = chessBoard_[newPosition.x_][newPosition.y_];
                return piece->isCheckBlockAble() && piece->canMoveDirection(vector);
            }
        }
        newPosition += vector;
        positions.push_back(newPosition);
    }
    return false;
}


/**
 * @brief Check if kings become neighbours or king gets checked after move to new position.
 * @param newPosition The new position for the king.
 * @param myColor The color of the player's pieces.
 * @return True if kings become neighbors or king gets checked.
 */
bool Chess::kingsNeighboursOrCheck(const Position &newPosition, Color myColor) {
    // get king position
    Position myKingPosition = this->myKingPosition(myColor);
    piece_ptr backup = chessBoard_[newPosition.x_][newPosition.y_];

    // temporary updatePosition king
    movePiece(std::make_pair(myKingPosition, newPosition));
    bool kingGetsChecked = kingHasCheck(myColor);
    bool kingsNeighbours = kingsAreNeighbours();

    // restore previous state
    movePiece(std::make_pair(newPosition, myKingPosition));
    chessBoard_[newPosition.x_][newPosition.y_] = backup;
    return kingGetsChecked || kingsNeighbours;
}


bool Chess::kingHasCheck(Color myColor) {
    int checkCount;
    Position checkingPiece;

    return isCheckedByKnight(myColor, checkingPiece) || isCheckedByPawn(myColor, checkingPiece) ||
            hasBlockAbleCheck(myColor, checkCount, checkingPiece);
}


/**
 * load a game from file
 * @param fileName name of file
 */
void Chess::loadGame(const std::string &fileName) {
    std::ifstream ifs;
    ifs.open(fileName);

    if (!ifs.good()) {
        throw std::runtime_error("Failed to open file");
    }

    loadGame(ifs);
    ifs.close();
}


/**
 * @brief Load a game from inputs stream.
 * @param inputStream The input stream to read from.
 */
void Chess::loadGame(std::istream &inputStream) {
    std::string line;      // line read from file
    std::string position;  // position of piece
    std::string color;     // color of piece
    std::string pieceType; // type of piece

    while(std::getline(inputStream, line)) {
        // create a string stream from line
        std::istringstream values(line);

        // read values from line
        std::getline(values, position, ' ');
        std::getline(values, color, ' ');
        std::getline(values, pieceType, ' ');

        // load position and value
        createPiece(loadPosition(position), loadColor(color), pieceType);
    }
    validateKings();
}


/**
 * @brief load game from FEN code
 * @param FENCode FEN code of game
 */
void Chess::loadFENGame(const std::string &FENCode) {
    int row = 0;
    int col = 0;
    Color color;
    std::string pieceType;

    for (char i : FENCode) {
        if (i == '/') {
            ++row;
            checkFENColumn(col);
            col = 0;
        }
        else if (isdigit(i)) {
            col += i - '0';
        }
        else {
            Position position(row, col);
            color = std::isupper(i) ? Color::WHITE : Color::BLACK;
            pieceType = std::string(1, std::toupper(i));

            // check if position is valid
            validatePosition(position);
            createPiece(position, color, pieceType);
            ++col;
        }
    }
    checkFENRow(row);
    checkFENColumn(col);
    validateKings();
}


/**
 * @brief load color of piece
 * @param color string color of piece
 * @return enum color of piece
 * @see Color
 */
Color Chess::loadColor(const std::string &color) {
    if (color == "white") {
        return Color::WHITE;
    }
    else if (color == "black") {
        return Color::BLACK;
    }
    else {
        throw InvalidColor();
    }
}


/**
 * @brief load position of piece
 * @param position string position of piece
 * @return Position of piece
 * @see Position
 */
Position Chess::loadPosition(const std::string &position) const {
    int x = 7 - (position[1] - '1');
    int y = position[0] - 'A';
    Position piecePosition(x, y);

    validatePosition(piecePosition);
    return piecePosition;
}


/**
 * @brief Create a piece on the chess board.
 * @param position The position of the piece.
 * @param color The color of the piece.
 * @param pieceType The type of the piece.
 */
void Chess::createPiece(const Position &position, Color color, const std::string &pieceType) {
    // get x and y coordinates
    size_t x = position.x_;
    size_t y = position.y_;

    // create piece
    if (pieceType == "king" || pieceType == "K") {
        chessBoard_[x][y] = std::make_shared<King>(color, position);
        (color == Color::WHITE) ? whiteKing_ = chessBoard_[x][y] : blackKing_ = chessBoard_[x][y];
    }
    else if (pieceType == "queen" || pieceType == "Q") {
        chessBoard_[x][y] = std::make_shared<Queen>(color, position);
    }
    else if (pieceType == "rook" || pieceType == "R") {
        chessBoard_[x][y] = std::make_shared<Rook>(color, position);
    }
    else if (pieceType == "bishop" || pieceType == "B") {
        chessBoard_[x][y] = std::make_shared<Bishop>(color, position);
    }
    else if (pieceType == "knight" || pieceType == "N") {
        chessBoard_[x][y] = std::make_shared<Knight>(color, position);
    }
    else if (pieceType == "pawn" || pieceType == "P") {
        if (color == Color::WHITE) {
            chessBoard_[x][y] = std::make_shared<PawnWhite>(position);
        }
        else {
            chessBoard_[x][y] = std::make_shared<PawnBlack>(position);
        }
    }
    else {
        throw InvalidPieceType();
    }
}


/**
 * @brief Check if both kings exist on the chessboard.
 */
void Chess::validateKings() {
    if (whiteKing_ == nullptr) {
        throw WhiteKingDoesNotExist();
    }
    else if (blackKing_ == nullptr) {
        throw BlackKingDoesNotExist();
    }
    else if (kingsAreNeighbours()) {
        throw KingsNeighbours();
    }
}


/**
  * @brief Check if color can give checkmate in searchDepth moves
  * @param colorOnMove color of player on move
  * @param searchDepth search depth
  * @param addCheckMateMoves whether to trace checkmate moves in checkMateList_
  * @param pruningSize number of best moves to consider
  * @return true if colorOnMove can give checkmate in searchDepth moves
  */
bool Chess::findCheckMate(Color colorOnMove, size_t searchDepth, bool addCheckMateMoves, size_t pruningSize) {
    // setup and call minimax
    setupMinimax(searchDepth, addCheckMateMoves, pruningSize);
    int evaluation = minimax(colorOnMove, searchDepth_);

    // deal results
    if (evaluation == INT_MAX || evaluation == INT_MIN) {
        dealCheckmateFound();
        return true;
    }
    dealCheckmateNotFound(evaluation);
    return false;
}


/**
 * @minimax algorithm to find checkmate move, more here @url https://www.youtube.com/watch?v=l-hh51ncgDI&t=294s
 * @param colorOnMove color of player on move
 * @param searchDepth search depth
 * @param alpha best reached value of maximizing player
 * @param beta best reached value of minimizing player
 * @return value of best move
 */
int Chess::minimax(Color colorOnMove, size_t searchDepth, int alpha, int beta) {
    if (searchDepth == 0) {
        return deepEvaluation(colorOnMove);
    }

    // maximizing player
    if (colorOnMove == Color::WHITE) {
        return maximizer(searchDepth, alpha, beta);
    }
    else {
        return minimizer(searchDepth, alpha, beta);
    }
}


/**
 * @brief Find best move for maximizing player == white
 * @param searchDepth search depth
 * @param alpha current best reached value of maximizing playerr
 * @param beta best possible value for maximizing player
 * @return value of best move
 */
int Chess::maximizer(size_t searchDepth, int alpha, int beta) {
    std::vector<piece_move> moves = getBestMoves(Color::WHITE);
    int maxEval = INT_MIN;

    for (const piece_move &move : moves) {
        Position positionFrom = move.first;
        Position positionTo = move.second;

        // create piece backups -> at positionFrom might be pawn which will be promoted to queen, rook, etc.
        piece_ptr pieceBackupFrom = chessBoard_[positionFrom.x_][positionFrom.y_];
        piece_ptr pieceBackupTo = chessBoard_[positionTo.x_][positionTo.y_];

        // update state
        movePiece(move);

        // evaluate position
        minimaxMoves_.push_back(move);
        int eval = minimax(Color::BLACK, searchDepth - 1, alpha, beta);
        minimaxMoves_.pop_back();

        // restore state -> make reverse updatePosition and restore piece at positionTo
        chessBoard_[positionTo.x_][positionTo.y_] = pieceBackupFrom;
        movePiece({positionTo, positionFrom});
        chessBoard_[positionTo.x_][positionTo.y_] = pieceBackupTo;

        // reset checkmate moves if it is not checkmate + save starting updatePosition if better than previous
        if (searchDepth == searchDepth_) {
            (eval != INT_MAX) ? resetCheckMateMove() : void(0);
            bestStartingMove_ = (eval > maxEval) ? move : bestStartingMove_;
        }

        // apply alpha-beta pruning
        maxEval = std::max(maxEval, eval);
        alpha = std::max(alpha, eval);
        if (beta <= alpha) {
            break;
        }
    }

    // if it is draw or checkmate
    if (moves.empty() && !kingHasCheck(Color::WHITE)) {
        return 0;
    }
    else if (moves.empty() && addCheckmateMoves_) {
        addCheckMateMove();
    }
    return maxEval;
}


/**
 * @brief Find best move for minimizing player == black
 * @param searchDepth search depth
 * @param alpha best possible value for minimizing player
 * @param beta current best reached value of minimizing player
 * @return
 */
int Chess::minimizer(size_t searchDepth, int alpha, int beta) {
    std::vector<piece_move> moves = getBestMoves(Color::BLACK);
    int minEval = INT_MAX;

    for (const piece_move &move : moves) {
        Position positionFrom = move.first;
        Position positionTo = move.second;

        // create piece backups -> at positionFrom might be pawn which will be promoted to queen, rook, etc.
        piece_ptr pieceBackupFrom = chessBoard_[positionFrom.x_][positionFrom.y_];
        piece_ptr pieceBackupTo = chessBoard_[positionTo.x_][positionTo.y_];

        // update state
        movePiece(move);

        // evaluate position
        minimaxMoves_.push_back(move);
        int eval = minimax(Color::WHITE, searchDepth - 1, alpha, beta);
        minimaxMoves_.pop_back();

        // restore state -> make reverse updatePosition and restore piece at positionTo
        chessBoard_[positionTo.x_][positionTo.y_] = pieceBackupFrom;
        movePiece({positionTo, positionFrom});
        chessBoard_[positionTo.x_][positionTo.y_] = pieceBackupTo;

        // reset checkmate moves if it is not checkmate + save starting updatePosition if better than previous
        if (searchDepth == searchDepth_) {
            (eval != INT_MIN) ? resetCheckMateMove() : void(0);
            bestStartingMove_ = (eval < minEval) ? move : bestStartingMove_;
        }

        // evaluate returned value
        minEval = std::min(minEval, eval);
        beta = std::min(beta, eval);
        if (beta <= alpha) {
            break;
        }
    }

    // if it is draw or checkmate
    if (moves.empty() && !kingHasCheck(Color::BLACK)) {
        return 0;
    }
    else if (moves.empty() && addCheckmateMoves_) {
        addCheckMateMove();
    }
    return minEval;
}


/**
 * @brief Move piece on chessboard
 * @param move move to do
 */
void Chess::movePiece(const piece_move &move) {
    // get positions
    Position fromPosition = move.first;
    Position toPosition = move.second;

    size_t fromX = fromPosition.x_;
    size_t fromY = fromPosition.y_;
    size_t toX = toPosition.x_;
    size_t toY = toPosition.y_;

    // updatePosition piece
    chessBoard_[fromX][fromY]->updatePosition(toPosition);
    chessBoard_[toX][toY] = chessBoard_[fromX][fromY];
    chessBoard_[fromX][fromY] = nullptr;

    // update king position
    if (chessBoard_[toX][toY]->getPieceType() == PieceType::KING) {
        (chessBoard_[toX][toY]->getColor() == Color::WHITE) ? whiteKing_ = chessBoard_[toX][toY] : blackKing_ = chessBoard_[toX][toY];
    }
    // check if pawn should be transformed
    else if (chessBoard_[toX][toY]->getPieceType() == PieceType::PAWN && (toX == 0 || toX == 7)) {
        transformPawn(toPosition);
    }
}


/**
 * @brief Get all pieces of color
 * @param color color if pieces to get
 * @return vector of pieces
 */
std::vector<piece_ptr> Chess::getPieces(Color color) const {
    std::vector<piece_ptr> pieces;
    for (const auto & row : chessBoard_) {
        for (const auto & piece : row) {
            if (piece != nullptr && piece->getColor() == color) {
                pieces.push_back(piece);
            }
        }
    }
    return pieces;
}


/**
 * @brief Get best moves for color -> they are chosen using quickEvaluation
 * @param color color of player on move
 * @return vector of best moves
 */
std::vector<piece_move> Chess::getBestMoves(Color myColor) {
    std::vector<piece_move> moves = getAllMoves(myColor);

    // sort moves by quick evaluation -> we want to check first moves which are more likely to be good
    std::sort(moves.begin(), moves.end(), [this](const piece_move &move1, const piece_move &move2) {
        return quickEvaluation(move1) > quickEvaluation(move2);
    });

    if (moves.size() > pruningSize_) {
        return {moves.begin(), moves.begin() + pruningSize_};
    }
    return moves;
}


/**
 * @brief Get all moves for color
 * @param color color of player on move
 * @return vector of all moves
 */
std::vector<piece_move> Chess::getAllMoves(Color color) {
    std::vector<piece_move> allMoves;
    Position originalPosition;

    // if king is in check -> we can updatePosition only king or block check, these moves are returned
    if (needsToBlockCheck(color, allMoves)) {
        return allMoves;
    }
    for (const piece_ptr &piece: getPieces(color)) {
        originalPosition = piece->getPosition();

        // get all possible moves for piece
        for (Position position: piece->getPossibleMoves(*this)) {
            allMoves.emplace_back(originalPosition, position);
        }
    }
    return allMoves;
}


/**
 * @brief quick evaluation of move, capturing moves or moves giving checkmate are preferred
 * @param move move to evaluate
 * @return value of move
 */
int Chess::quickEvaluation(const piece_move &move) {
    int evaluation = 0;
    evaluation += captureEnemyBonus(move.second);
    evaluation += betterPositionBonus(move);
    evaluation += willBeCheckBonus(move);
    return evaluation;
}


/**
 * @brief bonus for moving piece to better position, we prefer positions closer to enemy king
 * @param move move to evaluate
 * @return value of move
 */
int Chess::betterPositionBonus(const piece_move &move) {
    Position positionFrom = move.first;
    Position positionTo = move.second;
    piece_ptr piece = chessBoard_[positionFrom.x_][positionFrom.y_];

    Color myColor = piece->getColor();
    PieceType pieceType = piece->getPieceType();
    Position enemyKingPosition = this->enemyKingPosition(myColor);

    if (pieceType == PieceType::KING) {
        return countDistance(positionFrom, enemyKingPosition) - countDistance(positionTo, enemyKingPosition);
    }
    else if (pieceType == PieceType::ROOK || pieceType == PieceType::QUEEN) {
        int prevDistance = countDistance(positionFrom, enemyKingPosition);
        int newDistance  = countDistance(positionTo, enemyKingPosition);
        return prevDistance - newDistance;
    }
    return 0;
}


/**
 * @brief evaluate game position if we reached end of searchDepth_
 * @param colorOnMove
 * @return
 */
int Chess::deepEvaluation(Color colorOnMove) {
    int evaluation = 0;

    for (const auto & row : chessBoard_) {
        for (const auto & piece : row) {
            if (piece != nullptr) {
                evaluation += evaluatePiece(piece);
            }
        }
    }
    evaluation += colorOnMoveBonus(colorOnMove);
    return evaluation;
}


/**
 * @brief overloaded operator for printing move
 * @param os output stream to print to
 * @param move move to print
 * @return output stream
 */
std::ostream& operator<<(std::ostream& os, const piece_move & move) {
    os << move.first << " -> " << move.second;
    return os;
}


/**
 * @brief deal situation when checkmate was found
 */
void Chess::dealCheckmateFound() const {
    std::cout << "Checkmate founded!" << std::endl;
    std::cout << "Start: " << bestStartingMove_ << std::endl;
    std::cout << std::endl;
}


/**
 * @brief deal situation when checkmate was not found
 * @param evaluation evaluation of game position
 */
void Chess::dealCheckmateNotFound(int evaluation) const {
    std::cout << "No checkmate founded, ";
    printGameAnalysis(evaluation);
    std::cout << "Good updatePosition could be: " << bestStartingMove_;
    std::cout << std::endl;
}


/**
 * @brief print game analysis
 * @param evaluation evaluation of game position
 */
void Chess::printGameAnalysis(int evaluation) {
    if (evaluation > 0) {
        std::cout << "WHITE has material advantage." << std::endl;
    }
    else if (evaluation < 0) {
        std::cout << "BLACK has material advantage." << std::endl;
    }
    else {
        std::cout << "NO player has material advantage" << std::endl;
    }
}


/**
 * @brief print recorded moves that lead to checkmate
 */
void Chess::printCheckMateMoves() const {
    std::cout << "Checkmate moves: " << std::endl;
    for (const auto & checkMateMoves : checkMateList_) {
        for (const auto &move: checkMateMoves) {
            std::cout << move;
            std::cout << (move == checkMateMoves.back() ? "" : ", ");
        }
        std::cout << std::endl;
    }
}


/**
 * @brief transform pawn to another piece, position holds pieceType to transform to
 * @param position position of pawn to transform
 */
void Chess::transformPawn(const Position &position) {
    int x = position.x_;
    int y = position.y_;
    PieceType pieceType = position.transformTo_;

    switch (pieceType) {
        case PieceType::QUEEN:
            chessBoard_[x][y] = std::make_shared<Queen>(chessBoard_[x][y]->getColor(), position);
            break;
        case PieceType::ROOK:
            chessBoard_[x][y] = std::make_shared<Rook>(chessBoard_[x][y]->getColor(), position);
            break;
        case PieceType::BISHOP:
            chessBoard_[x][y] = std::make_shared<Bishop>(chessBoard_[x][y]->getColor(), position);
            break;
        case PieceType::KNIGHT:
            chessBoard_[x][y] = std::make_shared<Knight>(chessBoard_[x][y]->getColor(), position);
            break;
    }
}


/**
 * @brief check if position is valid
 * @param position position to check
 */
void Chess::validatePosition(const Position &position) const {
    if (!onChessboard(position)) {
        throw InvalidPosition();
    }
    else if (!isFree(position)) {
        throw PositionAlreadyOccupied();
    }
}


/**
 * @brief check if king is checked by enemy knight
 * @param kingColor color of king
 * @param checkingPiece empty position of checking piece -> will be set if king is checked
 * @return true if king is checked by enemy knight
 */
bool Chess::isCheckedByKnight(Color kingColor, Position &checkingPiece) {
    std::vector<Vector2D> knightMoves = Knight::getVectorMoves();
    Position myKingPosition = this->myKingPosition(kingColor);

    for (const auto &move : knightMoves) {
        Position newPosition = myKingPosition + move;
        if (onChessboard(newPosition) && !isFree(newPosition)) {

            // if there is a knight of the opposite color
            piece_ptr piece = chessBoard_[newPosition.x_][newPosition.y_];
            if (piece->getPieceType() == PieceType::KNIGHT && piece->getColor() != kingColor) {
                checkingPiece = newPosition;
                return true;
            }
        }
    }
    return false;
}


/**
 * @brief check if king is checked by enemy pawn
 * @param kingColor color of king
 * @param checkingPiece empty position of checking piece -> will be set if king is checked
 * @return true if king is checked by enemy knight
 */
bool Chess::isCheckedByPawn(Color kingColor, Position &checkingPiece) {
    Position newPosition;
    Position myKingPosition = this->myKingPosition(kingColor);
    std::vector<Vector2D> captureMoves = (kingColor == Color::WHITE) ? PawnWhite::getCaptureMoves() :
                                         PawnBlack::getCaptureMoves();

    for (const auto &captureMove : captureMoves) {
        newPosition = myKingPosition + captureMove;
        if (onChessboard(newPosition) && !isFree(newPosition)) {

            // if there is a pawn of the opposite color
            piece_ptr piece = chessBoard_[newPosition.x_][newPosition.y_];
            if (piece->getPieceType() == PieceType::PAWN && piece->getColor() != kingColor) {
                checkingPiece = newPosition;
                return true;
            }
        }
    }
    return false;
}


/**
 * @brief check if king is checked by enemy bishop, queen or rook
 * @param kingColor color of king
 * @param checkCount number of checking pieces -> will be increment for each checking piece
 * @param checkingPiece empty position of checking piece -> will be set if king is checked (we only use this
 *                      position if total number of pieces that gives check == 1, more in needsToBlockCheck)
 *
 * @return true if king is checked by enemy bishop, queen or rook
 */
bool Chess::hasBlockAbleCheck(Color kingColor, int &checkCount, Position &checkingPiece) {
    Position myKingPosition = this->myKingPosition(kingColor);
    std::vector<Vector2D> kingMoves = King::getVectorMoves();
    Position newPosition;

    for (const auto &move : kingMoves) {
        newPosition = myKingPosition + move;

        // while there is no piece on the way
        while (Chess::onChessboard(newPosition) && isFree(newPosition)) {
            newPosition += move;
        }
        // if there is a piece on the way, and it is an enemy piece
        if (Chess::onChessboard(newPosition) && isEnemy(newPosition, kingColor)) {
            piece_ptr piece = chessBoard_[newPosition.x_][newPosition.y_];

            // if the piece can block the check => it is queen, rook or bishop
            if (piece->isCheckBlockAble() && canMoveDirection(piece, move)) {
                checkingPiece = newPosition;
                checkCount++;
            }
        }
    }
    return checkCount > 0;
}


/**
 * @brief check if king is checked by enemy
 * @param colorOnMove color of player on move
 * @param moves empty vector of moves -> will be filled by moves that can block check
 * @return true if king is checked by enemy
 */
bool Chess::needsToBlockCheck(Color colorOnMove, std::vector<piece_move> &moves) {
    std::vector<Position> positions;
    Position checkingPiece;
    int blockAble = 0;
    int unblockAble = 0;
    int checkCount;

    // check unblockable checks -> it is impossible to be checked by both knight and pawn
    if (isCheckedByKnight(colorOnMove, checkingPiece) || isCheckedByPawn(colorOnMove, checkingPiece)) {
        unblockAble = 1;
    }
    // check blockable checks -> there can be more than one check
    hasBlockAbleCheck(colorOnMove, blockAble, checkingPiece);
    checkCount = unblockAble + blockAble;

    if (checkCount > 0) {
        addKingMoves(colorOnMove, moves);

        // if there is only one check => we block the check or capture the checking piece
        if (checkCount == 1) {
            unblockAble == 1 ? addMovesAtPosition(colorOnMove, checkingPiece, moves) :
                               addMovesAtPositionAndBetween(colorOnMove, checkingPiece, moves);
        }
        return true;
    }
    return false;
}


/**
 * @brief add moves of king to vector of moves
 * @param kingColor color of king
 * @param moves vector of moves -> will be filled by possible moves of king
 */
void Chess::addKingMoves(Color kingColor, std::vector<piece_move> &moves) {
    Position myKingPosition = this->myKingPosition(kingColor);
    std::vector<Position> positions = (kingColor == Color::WHITE) ? whiteKing_->getPossibleMoves(*this) :
                                                                    blackKing_->getPossibleMoves(*this);

    for (const auto &position : positions) {
        moves.emplace_back(myKingPosition, position);
    }
}


/**
 * @brief add moves of pieces of given color to enemy checking piece position
 * @param colorOnMove color of player on move
 * @param enemyCheckingPiece position of enemy checking piece
 * @param moves vector of moves -> will be filled by possible moves of pieces of given color
 */
void Chess::addMovesAtPosition(Color colorOnMove, const Position &enemyCheckingPiece, std::vector<piece_move> &moves) {
    std::vector<Position> positions;

    for (const auto &piece : getPieces(colorOnMove)) {
        if (piece->getPieceType() != PieceType::KING) {
            positions = piece->getPossibleMoves(*this);

            // if the piece can updatePosition to the position of the checking piece => add it to the moves
            for (const auto & position : positions) {
                if (position == enemyCheckingPiece) {
                    moves.emplace_back(piece->getPosition(), position);
                }
            }
        }
    }
}


/**
 * @brief add moves of pieces to enemy checking piece and between player´s king and enemy checking piece
 * @param myColor color of player on move
 * @param checkingPiece position of enemy checking piece
 * @param moves vector of moves -> will be filled by possible moves of pieces of player´s color
 */
void Chess::addMovesAtPositionAndBetween(Color myColor, const Position &checkingPiece, std::vector<piece_move> &moves) {
    // add moves at the position of the checking piece
    std::vector<Position> blockPositions;
    blockPositions.push_back(checkingPiece);

    // get vector between the checking piece and my king
    Position myKingPosition = this->myKingPosition(myColor);
    Vector2D vector = checkingPiece - myKingPosition;
    vector.normalize();

    // add moves between the checking piece and my king
    Position newPosition = myKingPosition + vector;
    while (newPosition != checkingPiece) {
        blockPositions.push_back(newPosition);
        newPosition += vector;
    }

    std::vector<Position> possibleMoves;
    for (const auto &piece : getPieces(myColor)) {
        if (piece->getPieceType() != PieceType::KING) {
            possibleMoves = piece->getPossibleMoves(*this);

            // if the piece can move to the check blocking position => add updatePosition
            for (const auto &position : possibleMoves) {
                if (std::find(blockPositions.begin(), blockPositions.end(), position) != blockPositions.end()) {
                    moves.emplace_back(piece->getPosition(), position);
                }
            }
        }
    }
}


/**
 * @brief print chessBoard_ to file
 * @param fileName name of file to print to
 */
void Chess::printBoard(const std::string &fileName) {
    std::ofstream ofs;
    ofs.open(fileName);

    if (!ofs.good()) {
        throw std::runtime_error("Failed to open file");
    }

    printBoard(ofs);
    ofs.close();
}


/**
 * @brief print chessBoard_
 * @param outputStream output stream to print to
 */
void Chess::printBoard(std::ostream &os) const {
    for (const auto & row : chessBoard_) {
        for (const auto & piece : row) {
            if (piece != nullptr) {
                os << *piece;
            }
        }
    }
}


/**
 * @brief check if move will give check
 * @param move move to try
 * @return true if move will give check
 */
int Chess::willBeCheckBonus(const piece_move &move) {
    Position positionFrom = move.first;
    Position positionTo = move.second;

    int fromX = positionFrom.x_;
    int fromY = positionFrom.y_;
    int toX = positionTo.x_;
    int toY = positionTo.y_;
    int bonus = 0;

    // create piece backups -> at positionFrom might be pawn which will be promoted to queen, rook, etc.
    piece_ptr pieceBackupFrom = chessBoard_[fromX][fromY];
    piece_ptr pieceBackupTo = chessBoard_[toX][toY];
    Color pieceColor = pieceBackupFrom->getColor();

    // check whether piece blocks own color check
    std::vector<Position> blockingPositions;
    bool inspectBlockedCheck = pieceBlocksCheck(positionFrom, getOppositeColor(pieceColor), blockingPositions);

    // updatePosition piece
    movePiece(move);

    // get bonus for giving check
    bonus += givesCheckBonus(chessBoard_[toX][toY]);
    if (inspectBlockedCheck) {
        // at the last check-blocking position is the checking piece
        Position position = blockingPositions.back();
        bonus += givesCheckBonus(chessBoard_[position.x_][position.y_]);
    }

    // updatePosition piece back
    chessBoard_[toX][toY] = pieceBackupFrom;
    movePiece({positionTo, positionFrom});
    chessBoard_[toX][toY] = pieceBackupTo;
    return bonus;
}


/**
 * @brief setup minimax, if parameter is not specified by user, default value will be used
 * @param searchDepth number of one color moves to check in minimax
 * @param addCheckmateMoves if true, checkmate moves will be added to checkMateList_
 * @param pruningSize number of moves to consider in minimax
 */
void Chess::setupMinimax(size_t searchDepth, bool addCheckmateMoves, size_t pruningSize) {
    searchDepth_ = 2 * searchDepth;
    addCheckmateMoves_ = addCheckmateMoves;
    pruningSize_ = pruningSize;

    if (pruningSize_ != PRUNING_SIZE) {
        std::cout << "You have defined pruning size, only limited number of moves will be searched." << std::endl;
        std::cout << "This can lead to invalid results in some cases." << std::endl;
        std::cout << std::endl;
    }
}

