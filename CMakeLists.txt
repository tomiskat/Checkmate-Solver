cmake_minimum_required(VERSION 3.24)
project(checkmate_solver)

set(CMAKE_CXX_STANDARD 20)

add_executable(checkmate_solver Main.cpp pieces/Piece.h Types.h pieces/Bishop.h pieces/Pawn.h pieces/Rook.h Chess.h pieces/King.h pieces/Queen.h pieces/Knight.h pieces/PawnBlack.h pieces/PawnWhite.h Exception.h pieces/Piece.cpp pieces/Pawn.cpp pieces/Knight.cpp pieces/King.cpp Chess.cpp pieces/Rook.cpp pieces/Queen.cpp pieces/Bishop.cpp pieces/PawnBlack.cpp pieces/PawnWhite.cpp)