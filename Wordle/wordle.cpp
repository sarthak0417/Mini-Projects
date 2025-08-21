#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <ctime>

//for lowercasing every word
void toLowerCase(std::string &word) {
  for (auto &c : word) {
    c = std::tolower(c);
  }
}

//for user input
std::string getUserWord() {
  std::cout << "Enter a 5 letter word: ";

  std::string user_word;
  std::cin >> user_word;

  return user_word;
}

//for checking length of user word
bool wordLengthCheck(std::string user_word) { return user_word.size() == 5; }

//for getting word from dictionary
std::vector<std::string> getWordList() {
  std::vector<std::string> words_list;

  std::ifstream file("dictionary.txt");

  std::string word;

  while (getline(file, word)) {
    if (!word.empty()) {
      toLowerCase(word);
      words_list.push_back(word);
    }
  }

  return words_list;
}

//for getting program's word from word list(5 length words)
std::string getProgramWord(std::vector<std::string> words_list) {
  std::string program_word;

  static std::mt19937 rng(time(0));
  std::shuffle(words_list.begin(), words_list.end(), rng);
  program_word = words_list[0];

  return program_word;
}

//assigning positions for letters
void evaluateUserWord(std::string user_word, std::string program_word) {
  int program_word_freq[26] = {0};

  std::vector<std::string> reply_string(5, "Not in word");

  for (char c : program_word) {
    program_word_freq[c - 'a']++;
  }

  for (int i = 0; i < 5; i++) {
    if (user_word[i] == program_word[i]) {
      reply_string[i] = "Right Position";
      program_word_freq[user_word[i] - 'a']--;
    }
  }

  for (int i = 0; i < 5; i++) {
    if (reply_string[i] == "Not in word" && program_word_freq[user_word[i] - 'a'] > 0) {
      reply_string[i] = "Wrong position";
      program_word_freq[user_word[i] - 'a']--;
    }
  }

  for (int i = 0; i < 5; i++) {
    std::cout << user_word[i] << " - " << reply_string[i];
    std::cout << '\n';
  }

  std::cout << '\n';
}

void guessRounds() {
  int rounds = 6;

  std::vector<std::string> word_list = getWordList();
  std::string program_word = getProgramWord(word_list);

  bool userWon = false;

  while (rounds--) {
    std::string user_word = getUserWord();

    if (wordLengthCheck(user_word)) {
      evaluateUserWord(user_word, program_word);

      if (user_word == program_word) {
        std::cout << "Correct! You win!" << '\n';
        userWon = true;
        break;
      }

    } else {
      std::cout << "Word is not 5 in length!" << '\n';
      break;
    }
  }

  if (!userWon) {
    std::cout << "You lose! The word was: " << program_word << '\n';
  }
}

int main() {
  std::cout << "Welcome to the game of Wordle!" << '\n';
  std::cout << "Rules: 5 letter word | 6 chances to guess it!" << '\n';

  guessRounds();
}