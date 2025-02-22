# Checkmate Solver

Student project to find out whether player can give checkmate in given number moves. The algorithm, which is used
is [minimax](https://www.youtube.com/watch?v=l-hh51ncgDI&t=586s) with alpha-beta pruning. The base concept is to 
try all moves and if there is move leading to checkmate, return it. If there is no checkmate move, then the
best move is returned. The best move is such move, that leads to bigger material advantage and mobility.

- Material advantage - Each piece is given a value queen = 9, rook = 5, bishop = 3, knight = 3, pawn = 1, king = 1)
- Mobility - Each piece has a number of possible moves. The more moves, the better.

Program also uses heuristic to determine which moves try first. The following moves are preferred:

- capturing moves - moves which capture opponent's piece
- check moves - moves which give check to opponent's king
- better position moves - moves which lead to better position
  - moving queen and rook closer to enemy king -> reduces number of opponent's king moves
  - moving king closer to enemy king -> suitable for endgame situations

Program also tries to be flexible, it supports more ways to load game:
- FEN - [Forsyth–Edwards Notation](https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation)
- defined [positions](inputs/positions)

## Run program
```bash
mkdir -p build
cmake -S . -B build
make -C build
./build/checkmate_solver
```
- For more info ckeck [Usage](./USAGE.md)
