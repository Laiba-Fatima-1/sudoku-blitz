# 🟩 SudokuBlitz

> Beat the clock. You only get 3 strikes. Think fast. Think right.

A fast-paced, colorful Sudoku game for the Windows console, built in C++. Race against the timer, spend your hints wisely, and don't let Shrek catch you making mistakes.

---

## 🎮 Features

- **3 Difficulty Levels** — Easy, Medium, and Hard puzzles loaded from a file
- **Live Timer & Scoring** — The faster and cleaner you play, the higher your score
- **Strike System** — 3 wrong moves and it's game over
- **Hint System** — Up to 2 hints per game to bail you out
- **Color-coded Console UI** — Fixed cells, user entries, and errors displayed in distinct colors
- **Shrek's Commentary** — A snarky response every time you make a mistake

---

## 🛠️ Tech Stack

| Component | Details |
|-----------|---------|
| Language | C++17 |
| Platform | Windows (uses `windows.h` for console colors) |
| Puzzle Storage | External `.txt` file with tagged sections |
| Build | Any C++ compiler (g++, MSVC) |

---

## 🚀 Getting Started

### Prerequisites
- Windows OS
- A C++ compiler (MinGW / MSVC / Visual Studio)

### Build & Run

```bash
# Using g++ (MinGW)
g++ SudokuBlitz.cpp -o SudokuBlitz.exe
./SudokuBlitz.exe
```

Or open in Visual Studio and hit **Run**.

> ⚠️ Make sure `sudoku_puzzles.txt` is in the **same directory** as the executable.

---

## 📁 Project Structure

```
sudoku-blitz/
├── SudokuBlitz.cpp       # Main game source code
├── sudoku_puzzles.txt    # Puzzle bank (Easy / Medium / Hard)
└── README.md
```

---

## 🎯 How to Play

1. Enter your name
2. Select a difficulty (Easy / Medium / Hard)
3. The board displays with numbered rows and columns
4. Each turn, choose to:
   - **Make a move** — enter row, column, and value (1–9)
   - **Use a hint** — reveals one correct cell (max 2 per game)
   - **Exit** — quit the current game
5. 3 wrong moves = game over
6. Complete the board to see your final score

### Scoring Formula
```
Score = Base Score - Time Elapsed (seconds) - (Mistakes × 100)
```
| Difficulty | Base Score |
|------------|-----------|
| Easy       | 1000      |
| Medium     | 1500      |
| Hard       | 2000      |

---

## 📌 Known Limitations

- Windows-only (uses `windows.h` for console colors)
- No save/load functionality
- Leaderboard not yet persistent across sessions

---

## 🔮 Planned Improvements

- [ ] Cross-platform color support (ANSI escape codes for Linux/macOS)
- [ ] Persistent high score leaderboard saved to file
- [ ] Timer displayed live without requiring input
- [ ] Puzzle generator (procedural, instead of file-based)
- [ ] Undo last move feature

---

## 👩‍💻 Author

Built as a 2nd semester C++ project.  
Feel free to fork, improve, and make Shrek proud.
