// SudokuBlitz.cpp

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <windows.h>

using namespace std;

// Console color setup
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
const int textColors[] = {
    FOREGROUND_RED, FOREGROUND_GREEN, FOREGROUND_BLUE,
    FOREGROUND_RED | FOREGROUND_GREEN,       // Yellow
    FOREGROUND_GREEN | FOREGROUND_BLUE,      // Cyan
    FOREGROUND_RED | FOREGROUND_BLUE,        // Magenta
    FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE // White
};
const int bgColors[] = {
    BACKGROUND_RED, BACKGROUND_GREEN, BACKGROUND_BLUE,
    BACKGROUND_RED | BACKGROUND_GREEN,       // Yellow
    BACKGROUND_GREEN | BACKGROUND_BLUE,      // Cyan
    BACKGROUND_RED | BACKGROUND_BLUE,        // Magenta
    BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE // White
};

void setConsoleColorWin(int textIndex, int bgIndex = -1) {
    WORD attr = textColors[textIndex % 7];
    if (bgIndex >= 0) attr |= bgColors[bgIndex % 7];
    SetConsoleTextAttribute(hConsole, attr);
}
void resetColorWin() {
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void showTitleScreen() {
    setConsoleColorWin(4); // Cyan
    cout << R"(
  ____                                           ___           ____ ______  _____
 / ___|              ||       ||               ||   \  ||       ||    ||    ||  /
 \(__    ||   ||   __||  __   || /  ||   ||    ||___/  ||       ||    ||       /
  ___)|  ||   ||  (__|| /  \\ ||/   ||   ||    ||   \  ||       ||    ||      /    _|_ _|_
 |____/  \_-_-_/  \___| \\__/ || \\ \_-_-_/    ||___/  ||____  _||_   ||     /__||  |   |
)";
    resetColorWin();
    cout << "\n";
    setConsoleColorWin(3); // Yellow
    cout << "\nBeat the clock.\n"
         << "You only get 3 strikes.\n"
         << "Think fast. Think right.\n\n"
         << "Game Instructions:\n"
         << "- Fill in the missing numbers from 1 to 9 in the grid.\n"
         << "- You can only use each number once per row, column, and 3x3 box.\n"
         << "- You have 3 chances to make mistakes.\n"
         << "- You can use up to 2 hints during the game.\n"
         << "- The faster and more accurate you are, the higher your score.\n";
    resetColorWin();
}
string getShrekJoke(int count) {
    string jokes[] = {
        "If mistakes were gold, you'd be a billionaire.",
        "You're solving like a swamp snail on vacation.",
        "Ogres do better with less layers of failure.",
        "I thought snails were bad at Sudoku. Then I met you.",
        "You're the reason Sudoku comes with erasers.",
        "Every time you mess up, a fairy tale dies.",
        "Your logic has more holes than Swiss cheese.",
        "This grid is starting to look like your GPA.",
        "Have you considered taking up knitting instead?"
    };
    return jokes[count % 9];
}
void showShrekFace(int count) {
    setConsoleColorWin(0); // Red
    cout << R"(
          _______
        /        \\
       /  DON'T   \\
       \\  CARE!  /
        \\_______/
         (o _ o)
        (      ) < )" << getShrekJoke(count) << R"(
         \_||_/
)" << endl;
    resetColorWin();
}

void displayBoard(int grid[9][9], bool fixed[9][9], const string& level) {
    setConsoleColorWin(1);
    cout << "\nLevel: " << level << "\n";
    resetColorWin();

    cout << "   ";
    for (int j = 0; j < 9; ++j) {
        cout << " " << j + 1;
        if ((j + 1) % 3 == 0 && j != 8) cout << " |";
    }
    cout << "\n";

    for (int i = 0; i < 9; ++i) {
        if (i % 3 == 0) {
            cout << "   +-------+-------+-------+\n";
        }
        cout << " " << i + 1 << " |";
        for (int j = 0; j < 9; ++j) {
            if (grid[i][j] == 0) {
                setConsoleColorWin(0); // Red for empty
                cout << " .";
            } else {
                setConsoleColorWin(fixed[i][j] ? 3 : 2); // Yellow or Green
                cout << " " << grid[i][j];
            }
            resetColorWin();
            if ((j + 1) % 3 == 0 && j != 8) cout << " |";
        }
        cout << "|\n";
    }
    cout << "   +-------+-------+-------+\n";
}

class User {
    string name;
    int mistakes = 0, score = 0, timeTaken = 0, hintsUsed = 0;
public:
    User(string uname) : name(uname) {}
    void reset() { mistakes = score = timeTaken = hintsUsed = 0; }
    void addMistake() { mistakes++; showShrekFace(mistakes); }
    void addHint() { hintsUsed++; }
    void setTime(int t) { timeTaken = t; }
    void setScore(int s) { score = s; }
    string getName() const { return name; }
    int getMistakes() const { return mistakes; }
    int getScore() const { return score; }
    int getTime() const { return timeTaken; }
    int getHintsUsed() const { return hintsUsed; }
};

class PuzzleGenerator {
    vector<vector<string>> puzzles;
public:
    void loadFromFile(const string& filename, const string& level) {
        puzzles.clear();
        ifstream fin(filename);
        if (!fin) {
            cerr << "Error loading puzzle file!" << endl;
            exit(1);
        }
        string line, tag = "# " + level;
        vector<string> puzzle;
        bool reading = false;

        while (getline(fin, line)) {
            if (line.find(tag) == 0) {
                reading = true;
                puzzle.clear();
            } else if (line.find("#") == 0) {
                reading = false;
            } else if (reading && !line.empty()) {
                puzzle.push_back(line);
                if (puzzle.size() == 9) {
                    puzzles.push_back(puzzle);
                    puzzle.clear();
                }
            }
        }
    }

    void getRandomPuzzle(int grid[9][9], bool fixed[9][9], int solution[9][9]) {
        if (puzzles.empty()) {
            cerr << "No puzzles were loaded for this level!" << endl;
            exit(1);
        }
        int idx = rand() % puzzles.size();
        auto& puzzle = puzzles[idx];
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j) {
                char ch = puzzle[i][j];
                grid[i][j] = (ch >= '1' && ch <= '9') ? ch - '0' : 0;
                fixed[i][j] = (grid[i][j] != 0);
            }
        memcpy(solution, grid, sizeof(int) * 81);
        solveSudoku(solution);
    }

private:
    bool isSafe(int grid[9][9], int row, int col, int num) {
        for (int x = 0; x < 9; ++x)
            if (grid[row][x] == num || grid[x][col] == num)
                return false;
        int sr = row / 3 * 3, sc = col / 3 * 3;
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                if (grid[sr + i][sc + j] == num)
                    return false;
        return true;
    }

    bool solveSudoku(int grid[9][9]) {
        for (int row = 0; row < 9; ++row)
            for (int col = 0; col < 9; ++col)
                if (grid[row][col] == 0)
                    for (int num = 1; num <= 9; ++num)
                        if (isSafe(grid, row, col, num)) {
                            grid[row][col] = num;
                            if (solveSudoku(grid)) return true;
                            grid[row][col] = 0;
                        }
        return true;
    }
};

class Board {
    int grid[9][9], solution[9][9];
    bool fixed[9][9];
    string level;
public:
    void load(PuzzleGenerator& pg, const string& lvl) {
        level = lvl;
        pg.getRandomPuzzle(grid, fixed, solution);
    }

    void display() { displayBoard(grid, fixed, level); }
    bool isEditable(int r, int c) { return !fixed[r][c]; }
    bool isValidMove(int r, int c, int val) { return isEditable(r, c) && solution[r][c] == val; }
    void makeMove(int r, int c, int val) { grid[r][c] = val; }
    bool isComplete() {
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j)
                if (grid[i][j] != solution[i][j])
                    return false;
        return true;
    }
    bool giveHint() {
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j)
                if (grid[i][j] == 0 && !fixed[i][j]) {
                    grid[i][j] = solution[i][j];
                    return true;
                }
        return false;
    }
};

class SudokuGame {
    User player;
    Board board;
    PuzzleGenerator puzzleGen;
    string level;
    time_t startTime;
public:
    SudokuGame(string name) : player(name) {}

    void play() {
                int diff;
        do {
            setConsoleColorWin(0); // Yellow
            cout << "\nSelect Difficulty:\n1. Easy\n2. Medium\n3. Hard\nEnter choice (1-3): ";
            cin >> diff;
            if(diff < 1 || diff > 3)
            cout << "Invalid input: Enter again.";
            resetColorWin();
        } while (diff < 1 || diff > 3);
        level = (diff == 1 ? "Easy" : (diff == 2 ? "Medium" : "Hard"));


        puzzleGen.loadFromFile("sudoku_puzzles.txt", level);
        board.load(puzzleGen, level);
        player.reset();
        startTime = time(nullptr);
        int baseScore = (level == "Easy" ? 1000 : (level == "Medium" ? 1500 : 2000));

        while (true) {
            board.display();

            int elapsed = time(nullptr) - startTime;
            setConsoleColorWin(5);
            cout << "Time: " << elapsed << "s  ";
            setConsoleColorWin(0);
            cout << "Mistakes: " << player.getMistakes() << "/3  ";
            setConsoleColorWin(3);
            cout << "Hints: " << player.getHintsUsed() << "/2\n";
            resetColorWin();

            if (player.getMistakes() >= 3) break;
            if (board.isComplete()) {
                player.setTime(elapsed);
                int finalScore = baseScore - elapsed - (player.getMistakes() * 100);
                player.setScore(max(finalScore, 0));
                setConsoleColorWin(2);
                cout << "\nPuzzle Complete!\n";
                resetColorWin();
                cout << "Player: " << player.getName()
                     << " | Score: " << player.getScore()
                     << " | Time: " << player.getTime() << "s\n";
                break;
            }

                        int choice;
            do {
                cout << "\n1. Make Move  2. Use Hint  3. Exit\nEnter choice: ";
                cin >> choice;
                if(choice < 1 || choice > 3)
                cout << "Invalid input: Enter again.";
            } while (choice < 1 || choice > 3);


            if (choice == 1) {
                int r, c, val;
                while (true) {
                    cout << "Enter Row (1-9), Column (1-9), Value (1-9): ";
                    cin >> r >> c >> val;
                    if (r >= 1 && r <= 9 && c >= 1 && c <= 9 && val >= 1 && val <= 9)
                        break;
                    cout << "Invalid input. Please enter values between 1 and 9.\n";
                }
                r--; c--;

                if (!board.isEditable(r, c)) {
                    cout << "Cell not editable.\n";
                } else if (board.isValidMove(r, c, val)) {
                    board.makeMove(r, c, val);
                    cout << "Move accepted!\n";
                } else {
                    player.addMistake();
                }
            } else if (choice == 2) {
                if (player.getHintsUsed() < 2 && board.giveHint()) {
                    player.addHint();
                    cout << "Hint applied!\n";
                } else {
                    cout << "No hints left.\n";
                }
            } else {
                cout << "Exiting game...\n";
                break;
            }

            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
        }
    }
};

int main() {
    srand(time(0));
    showTitleScreen();
    cout << "Enter your name: ";
    string name;
    cin.ignore();
    getline(cin, name);
    SudokuGame game(name);
    game.play();
    cout << "\nThanks for playing, " << name << "!\n";
    return 0;
}
