#include <cctype>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

// To lower case the direction input
void toLowerCase(std::string &word) {
  for (auto &c : word) {
    c = std::tolower(c);
  }
}

// Fixed board dimensions (8x8)
std::pair<int, int> boardDimensions() {
    return {8, 8};
}

// Calculating total >= 3 matches both horizontally and vertically
std::vector<std::vector<std::pair<int, int>>>
calculateMatches(std::vector<std::vector<char>> &board,
                 std::pair<int, int> dimensions) {
  std::vector<std::vector<std::pair<int, int>>> matches;

  int rows = dimensions.first;
  int cols = dimensions.second;

// Horizontal matches
  for (int i = 0; i < rows; i++) {
    char match_symbol = board[i][0];
    int match_cnt = 1;

    for (int j = 1; j < cols; j++) {
      if (board[i][j] == match_symbol) {
        match_cnt++;
      } else {
        if (match_cnt >= 3) {
          std::vector<std::pair<int, int>> match;

          for (int x = j - match_cnt; x < j; x++) {
            match.push_back({i, x});
          }

          matches.push_back(match);
        }

        match_symbol = board[i][j];
        match_cnt = 1;
      }
    }

    // To check if a match exists after the rows end
    if (match_cnt >= 3) {
      std::vector<std::pair<int, int>> match;

      for (int x = cols - match_cnt; x < cols; x++) {
        match.push_back({i, x});
      }

      matches.push_back(match);
    }
  }

  // Vertical matches
  for (int j = 0; j < cols; j++) {
    char match_symbol = board[0][j];
    int match_cnt = 1;

    for (int i = 1; i < rows; i++) {
      if (board[i][j] == match_symbol) {
        match_cnt++;
      } else {
        if (match_cnt >= 3) {
          std::vector<std::pair<int, int>> match;

          for (int y = i - match_cnt; y < i; y++) {
            match.push_back({y, j});
          }

          matches.push_back(match);
        }

        match_symbol = board[i][j];
        match_cnt = 1;
      }
    }

    // To check if a match exists after the columns end
    if (match_cnt >= 3) {
      std::vector<std::pair<int, int>> match;

      for (int y = rows - match_cnt; y < rows; y++) {
        match.push_back({y, j});
      }

      matches.push_back(match);
    }
  }

  return matches;
}

// To set the stored matches to null
void removeMatches(std::vector<std::vector<char>> &board,
                   std::vector<std::vector<std::pair<int, int>>> &matches) {
  for (auto &match : matches) {
    for (auto &position : match) {
      int row = position.first;
      int col = position.second;

      board[row][col] = '\0';
    }
  }
}

// To fill the spaces after removing the tiles
void getFallingTiles(std::vector<std::vector<char>> &board,
                     std::pair<int, int> dimensions) {
  int rows = dimensions.first;
  int cols = dimensions.second;

  char symbols[5] = {'*', '-', '+', '&', '#'};

  for (int j = 0; j < cols; j++) {
    int ptr1 = rows - 1;

    while (ptr1 >= 0) {
      while (ptr1 >= 0 && board[ptr1][j] != '\0') {
        ptr1--;
      }

      if (ptr1 < 0) {
        break;
      }

      int ptr2 = ptr1 - 1;

      while (ptr2 >= 0 && board[ptr2][j] == '\0') {
        ptr2--;
      }

      if (ptr2 < 0) {
        break;
      }

      std::swap(board[ptr1][j], board[ptr2][j]);

      ptr1--;
    }

    for (int i = 0; i < rows; i++) {
      if (board[i][j] == '\0') {
        int random_index = rand() % 5;
        board[i][j] = symbols[random_index];
      }
    }
  }
}

// To show what moved (for debugging)
void printFallingInfo(std::vector<std::vector<char>> &board,
                      std::pair<int, int> dimensions) {
  std::cout << "New tiles have been generated!" << '\n';
}

// To swap two adjacent tiles
std::pair<int, int> elementToSwap() {
  std::pair<int, int> element;

  std::cout << "Enter the row of the element to swap: ";
  std::cin >> element.first;

  std::cout << "Enter the column of the element to swap: ";
  std::cin >> element.second;

  return element;
}

// To get the swap direction
std::string swapDirection() {
  std::string direction;

  std::cout << "Swap direction (up, down, left, right): ";

  std::cin >> direction;

  toLowerCase(direction);

  return direction;
}

// Adding tile validation check to prevent out of bound access
bool isValidSwap(int row, int col, std::pair<int, int> dimensions,
                 std::string direction) {
  row -= 1;
  col -= 1;

  if (direction == "left") {
    return col - 1 >= 0;
  }

  if (direction == "right") {
    return col + 1 < dimensions.second;
  }

  if (direction == "up") {
    return row - 1 >= 0;
  }

  if (direction == "down") {
    return row + 1 < dimensions.first;
  }

  return false;
}

// To swap adjacent elements in the board
bool swapBoardElements(int row, int col, std::string direction,
                       std::vector<std::vector<char>> &board,
                       std::pair<int, int> dimensions) {
  std::pair<int, int> adjacent_el;

  row -= 1;
  col -= 1;

  if (direction == "left" &&
      isValidSwap(row + 1, col + 1, dimensions, direction)) {
    adjacent_el.first = row;
    adjacent_el.second = col - 1;
  }

  else if (direction == "right" &&
           isValidSwap(row + 1, col + 1, dimensions, direction)) {
    adjacent_el.first = row;
    adjacent_el.second = col + 1;
  }

  else if (direction == "up" &&
           isValidSwap(row + 1, col + 1, dimensions, direction)) {
    adjacent_el.first = row - 1;
    adjacent_el.second = col;
  }

  else if (direction == "down" &&
           isValidSwap(row + 1, col + 1, dimensions, direction)) {
    adjacent_el.first = row + 1;
    adjacent_el.second = col;
  }

  else {
    std::cout << '\n' << "Please enter valid input!" << '\n';

    return false;
  }

  std::swap(board[row][col], board[adjacent_el.first][adjacent_el.second]);

  return true;
}

// To print the current state of the board
void printBoard(std::vector<std::vector<char>> &board,
                std::pair<int, int> dimensions) {
  int rows = dimensions.first;
  int cols = dimensions.second;

  std::cout << '\n';

  std::cout << "   ";

  for (int j = 0; j < cols; j++) {
    std::cout << j + 1 << "   ";
  }

  std::cout << '\n';

  for (int i = 0; i < rows; i++) {
    if (i + 1 < 10) {
      std::cout << i + 1 << "  ";
    } else {
      std::cout << i + 1 << " ";
    }

    for (int j = 0; j < cols; j++) {
      if (board[i][j] == '\0') {
        std::cout << "  | ";
      } else {
        std::cout << board[i][j] << " | ";
      }
    }

    std::cout << '\n';
  }

  std::cout << '\n';
}

// To generate game board
std::vector<std::vector<char>> generateBoard(std::pair<int, int> dimensions) {
  int rows = dimensions.first;
  int cols = dimensions.second;

  char symbols[5] = {'*', '-', '+', '&', '#'};

  std::vector<std::vector<char>> board(rows, std::vector<char>(cols));

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      int random_index = rand() % 5;
      board[i][j] = symbols[random_index];
    }
  }

  return board;
}

// Calulating score based on tiles removed
int calculateScore(std::vector<std::vector<std::pair<int, int>>> matches) {
  int score = 0;

  for (auto &match : matches) {
    score += match.size();
  }

  return score;
}

// Checking potential matches and removing tiles
int wholeBoardCheck(std ::vector<std::vector<char>> &board,
                    std::pair<int, int> dimensions) {
  std::vector<std::vector<std::pair<int, int>>> matches =
      calculateMatches(board, dimensions);

  int score = 0;

  while (!matches.empty()) {
    std::cout << '\n' << "Found: " << matches.size() << " matches!" << '\n';

    removeMatches(board, matches);
    // printRemovedTiles(board, dimensions);
    printBoard(board, dimensions);

    score += calculateScore(matches);

    std::cout << '\n' << "Current score: " << score << '\n';

    getFallingTiles(board, dimensions);
    printFallingInfo(board, dimensions);

    printBoard(board, dimensions);

    matches = calculateMatches(board, dimensions);
  }

  return score;
}

int main() {
  srand(time(nullptr));

  std::cout << "Welcome to Match Three!" << '\n';

  // Initializing the board with given dimensions
  std::pair<int, int> dimensions = boardDimensions();
  std::vector<std::vector<char>> board = generateBoard(dimensions);

  printBoard(board, dimensions);

  // To keep track of score across function calls
  int total_score = 0;

  // Checking possible matches by default board generation
  wholeBoardCheck(board, dimensions);

  // Moves left to play
  int moves = 10;

  while (true) {
    std::cout << "Moves left: " << moves << '\n';
    std::cout << "Total score: " << total_score << '\n';

    std::pair<int, int> element = elementToSwap();
    std::string direction = swapDirection();

    bool canSwap = swapBoardElements(element.first, element.second, direction,
                                     board, dimensions);

    printBoard(board, dimensions);

    // To ask for input again after invalid direction
    while (!canSwap) {
      element = elementToSwap();
      direction = swapDirection();

      canSwap = swapBoardElements(element.first, element.second, direction,
                                  board, dimensions);

      printBoard(board, dimensions);
    }

    // Score gained in this function call
    int new_score = wholeBoardCheck(board, dimensions);

    // Adding to the total score
    total_score += new_score;

    if (new_score > 0) {
      std::cout << "Points gained: " << new_score << '\n';
      std::cout << "Total score: " << total_score << '\n';
    }

    // One swap = -1 move
    moves--;

    if (moves <= 0) {
      std::cout << "Game over! Final score: " << total_score << '\n';
      break;
    }
  }
}