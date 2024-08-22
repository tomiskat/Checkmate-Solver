#include <iostream>
#include "Chess.h"

// simple main function for testing, see USAGE.md for more info
int main() {
    Chess chess = Chess();
    chess.loadFENGame("5rk1/1b3ppp/8/2RN4/8/8/2Q2PPP/6K1");
    bool check = chess.findCheckMate(Color::WHITE, 4, true, 10);
    chess.printCheckMateMoves();
    return 0;
}
