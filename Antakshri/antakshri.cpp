#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <vector>
#include <ctime>

std::pair<char, char> giveFirstAndLastWord(std::string user_string,
                                           int word_size) {
  char start_letter = user_string[0];
  char end_letter = user_string[word_size - 1];

  std::pair<char, char> letters = {start_letter, end_letter};

  return letters;
}

char nextChar(char c) {
  if (c == 'z') {
    return 'z' + 1;
  } else {
    return c + 1;
  }
}

std::vector<std::string>
searchWordsHelperFunc(char ch, std::vector<std::string> &words) {
  std::string lower(1, ch);
  std::string upper(1, nextChar(ch));

  auto start = lower_bound(words.begin(), words.end(), lower);
  auto end = lower_bound(words.begin(), words.end(), upper);

  return std::vector<std::string>(start, end);
}

std::vector<std::string> searchWords(std::string user_string, int word_size) {
  std::ifstream file("dictionary.txt");

  std::vector<std::string> words;
  std::string word;

  while (getline(file, word)) {
    if (!word.empty())
      words.push_back(word);
  }

  sort(words.begin(), words.end());

  std::pair<char, char> letters = giveFirstAndLastWord(user_string, word_size);

  std::vector<std::string> words_array =
      searchWordsHelperFunc(letters.second, words);

  return words_array;
}

std::string chooseWord(std::vector<std::string> words,
                       std::string user_string) {
  static std::mt19937 rng(time(0));
  std::shuffle(words.begin(), words.end(), rng);

  for (std::string &word : words) {
    if (word != user_string) {
      return word;
    }
  }

  if (words.empty()) {
    return "";
  } else {
    return words[0];
  }
}

int getRandomWeight(int min, int max) {
  static std::mt19937 rng(time(0));
  std::uniform_int_distribution<int> dist(min, max);
  return dist(rng);
}

std::map<char, int> getTrumpLetters() {
  std::vector<char> alphabets;

  for (char c = 'a'; c <= 'z'; c++) {
    alphabets.push_back(c);
  }

  std::shuffle(alphabets.begin(), alphabets.end(), std::mt19937(0));

  std::map<char, int> alphabet_weights;

  for (char c = 'a'; c <= 'z'; c++) {
    alphabet_weights[c] = 1;
  }
  
  for (int i = 0; i < 3; i++) {
    char chosen_alphabet = alphabets[i];
    int bonus_weight = getRandomWeight(1, 4);
    alphabet_weights[chosen_alphabet] = bonus_weight;
  }

  return alphabet_weights;
}

void trumpLetters(std::map<char, int> alphabet_weights) {
  std::cout << "X === X === X === X" << '\n';
  std::cout << "Trump Letters (Bonus Points):" << '\n';

  for (auto it : alphabet_weights) {
    if (it.second > 1) {
      std::cout << "Alphabet: " << it.first << " | Weight: " << it.second
                << '\n';
    }
  }

  std::cout << "All other letters have weight 1" << '\n';
  std::cout << "X === X === X === X" << '\n';
}

int calculateScore(std::string user_string, int word_size,
                   std::map<char, int> &alphabet_weights) {
  int score = 0;

  for (int i = 0; i < word_size; i++) {
    char letter = user_string[i];
    int weight = (alphabet_weights.find(letter) != alphabet_weights.end())
                     ? alphabet_weights[letter]
                     : 1;
    score += weight;
  }

  return score;
}

void gameRounds(int rounds) {
  int user_score = 0;
  int program_score = 0;

  std::map<char, int> alphabet_weights;
  alphabet_weights = getTrumpLetters();
  trumpLetters(alphabet_weights);

  while (rounds--) {
    std::cout << "Enter your word: ";

    std::string user_string;
    std::cin >> user_string;

    int word_size = user_string.size();

    std::vector<std::string> words = searchWords(user_string, word_size);

    std::string reply_word = chooseWord(words, user_string);
    int reply_word_size = reply_word.size();

    user_score += calculateScore(user_string, word_size, alphabet_weights);
    program_score +=
        calculateScore(reply_word, reply_word_size, alphabet_weights);

    std::cout << "Program Word: " << reply_word << '\n';

    std::cout << "Current user score: " << user_score << '\n';
    std::cout << "Current program score: " << program_score << '\n';
  }

  std::cout << "Final user score: " << user_score << '\n';
  std::cout << "Final program score: " << program_score << '\n';

  if (user_score > program_score) {
    std::cout << "You win!" << '\n';
  } else if (user_score < program_score) {
    std::cout << "You lost." << '\n';
  } else {
    std::cout << "Game draw!" << '\n';
  }
}

int main() {
  int rounds = 0;

  std::cout << "Enter the number of rounds: ";
  std::cin >> rounds;

  gameRounds(rounds);

  return 0;
}
