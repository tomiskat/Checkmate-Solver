# Checkmate Solver - Usage

User creates object of class Chess, then loads game and then calls `findCheckMate` method with color on
move. The method returns true if it is possible to give checkmate in given number of moves, otherwise false.

```c++
Chess chess = Chess();
chess.loadFENGame("2r1r1k1/5ppp/8/8/Q7/8/5PPP/4R1K1");
bool check = chess.findCheckMate(Color::WHITE);
```

Method `findCheckMate` has 3 optional parameters:
- `size_t searchDepth`     - number of one color moves to search for checkmate. (default 3)
- `bool addCheckMateMoves` - if true, then all moves that lead to checkmate are added to `moves` vector. (default false)
- `size_t pruningSize`     - number of moves to try from each position. (default all)

```c++
bool check = chess.findCheckMate(Color::WHITE, 4, true, 10);
```

To load game from FEN string, use `loadFENGame` method. To load game from file, use `loadGame` method.
Following code transforms FEN string to game defined by position.

```c++
chess.loadFENGame("2r1r1k1/5ppp/8/8/Q7/8/5PPP/4R1K1");
chess.printGame();

// to print to file
chess.printGame("output file - absolute path");
```