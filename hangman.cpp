//Code file for hangman
//change lines 60-72 to adjust based on your files with the words in them

#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <chrono> // For time delay
#include <thread> // Makes sure the thread excutes before moving on.
#include <algorithm> 

using namespace std;

class intro {
public:
    // printHangmanforintro prints out the hang man for the opening animation.
    void printHangmanforintro(int guessCount, int animationStep) {
        // Head animation
        cout << "  +---+\n";
        if (guessCount < 6) cout << "   |   |\n";
        if (guessCount < 5) cout << "   O   |\n";

        // Limb animation
        if (animationStep % 2 == 0) {
            if (guessCount < 4) cout << "   \b\b\b";
        }
        if (guessCount < 2) cout << "  /|\\  |\n";
        if (guessCount < 1) cout << "   |   |\n";
        if (guessCount == 0) cout << "  / \\  |\n";
        cout << "      ===\n";
    }
    // printHangmanforcount prints out the hang man for the incorrect guesses.
    void printHangmanforcount(int& guessesLeft) {
        cout << "  +---+\n";
        if (guessesLeft < 6) cout << "   |   |\n";
        if (guessesLeft < 5) cout << "   O   |\n";
        if (guessesLeft < 4) cout << "  /|\\  |\n";
        if (guessesLeft < 3) cout << "   |   |\n";
        if (guessesLeft == 1) cout << "  / \\  |\n";
        cout << "      ===\n";
    }
    // Animate the hangman
    void animateHangman() {
        int guessCount = 6;
        int animationStep = 0;
        while (guessCount >= 0 && animationStep <= 10) {
            printHangmanforintro(guessCount, animationStep);
            cout.flush(); // Forces the system to flush the output so time delay can be accurate.
            this_thread::sleep_for(chrono::milliseconds(500)); // Delays the loop for 500 milisecond.
            system("cls"); // Clears the screen.
            animationStep++;
            guessCount--;
        }
    }

};

class game {
public:
    // Selects file and random word for single player.
    string selectWord(const char& dif) {
        string filename;
        switch (dif) {
        case 'E': filename = "easy.txt"; break;
        case 'M': filename = "medium.txt"; break;
        case 'H': filename = "hard.txt"; break;
        default:
            cerr << "Invalid difficulty!" << endl;
            return "";
        }
        vector<string> words;
        string word;
        ifstream file(filename);
        // This catches any error that comes up due to not being able to open the file and throws out an error that the user can understand.
        try {
            if (!file.is_open()) {
                throw runtime_error("Error opening file: " + filename);
                return "";
            }
            while (getline(file, word)) {
                words.push_back(word);
            }
            file.close();

            if (words.empty()) {
                throw runtime_error("Error: File is empty.");
                return "";
            }
        } catch (const runtime_error& e)
        {
            cerr << e.what() << endl;
            return "";
        }
        srand(time(0));
        return words[rand() % words.size()];
    }
    // The simple ui for the game to tell the user what guess they have made so far and how many guesses they have left.
    void renderGame(const string& guessedWord, int guessesLeft) {
        cout << "\nCurrent guess: ";
        for (char c : guessedWord) { 
            cout << c << " ";
        }
        cout << "\nGuesses left: " << guessesLeft << endl;
    }
    // processGuess checks if the guessed letter is in the secret word.
    bool processGuess(char guess, const string& secretWord, string& guessedWord) {
        bool correct = false; // Tracks if the guess was correct
        intro guesing;
        for (size_t i = 0; i < secretWord.length(); ++i) {
            if (secretWord[i] == guess) {
                guessedWord[i] = guess; // Updates the guessedWord if correct.
                correct = true;
            }
        }
        return correct;
    }
};
int main() {
    char dif;
    char mode;
    char player;
    char playagain;
    int guessesLeft = 6;
    string secretWord;
    string guessedWord;
    intro animate;
    game play;
    vector<char> guesses; // Stores the player's previous guesses.
    //Displays a welcome screen then calls on the animation.
    cout << "****************************\n";
    cout << "**  Welcome to Hangman!  **\n";
    cout << "****************************\n";


    animate.animateHangman();
    cout << "****************************\n";

    cout << "Would you like to play single player or multiplayer? (S-Single, M-Multi)" << endl;
    cin >> player;
    // Do-while loop allows the user to select if they want to play again.
    do {
        // Allows user to select game mode.
        if (toupper(player) == 'M')
        {
            cout << "Player, please insert your word for the other person to guess." << endl;
            cin >> secretWord;
            guessedWord = string(secretWord.length(), '_');
            this_thread::sleep_for(chrono::milliseconds(200));
            system("cls");
        }
        else if (toupper(player) == 'S') {
            cout << "Please select your difficulty: (E-Easy, M-Medium, H-Hard) " << endl;
            cin >> dif;
            dif = toupper(dif);
            secretWord = play.selectWord(dif);
            guessedWord = string(secretWord.length(), '_');
            this_thread::sleep_for(chrono::milliseconds(200));
            system("cls");
        }
        else
        {
            cerr << "You inputed an incorrect player mode" << endl;
        }
        // Makes sure the player has not guessed the secret word and still has guesses left.
        while (guessesLeft > 0 && guessedWord != secretWord && !secretWord.empty()) {
            // Calles the rendergame function, to show the current guess and how many guesses there are left.
            play.renderGame(guessedWord, guessesLeft);
            char guess;
            cout << "Enter your guess: ";
            cin >> guess;
            // Finds out if the guess you have entered now is a repeat, to make sure you don't waste a guess.
            if (find(guesses.begin(), guesses.end(), guess) != guesses.end()) {
                cout << "You have already guessed that letter. Try a different one.\n";
            }
            else {
                // Adds your current guess to the vector of previous guesses.
                guesses.push_back(guess);
                // Removes a guess if you are incorrect.
                if (!play.processGuess(guess, secretWord, guessedWord)) {
                    animate.printHangmanforcount(guessesLeft);
                    guessesLeft--;
                    cout << "Wrong guess!\n";
                }
            }
            this_thread::sleep_for(chrono::milliseconds(500)); // Allows for the game to show you the guess you are at and the animation of the hangman.
            system("cls"); // Clears the screen to avoid confusion.
        }
        //Checks if you won.
        if (guessedWord == secretWord && !secretWord.empty()) {
            cout << "Congratulations! You guessed the word: " << secretWord << endl;
        }
        //checks if you lost.
        else if (!secretWord.empty()) {
            play.renderGame(guessedWord, guessesLeft);
            cout << "Game Over! The correct word was: " << secretWord << endl;
        }
        cout << "Would you like to play again? (Y-Yes, N-No)" << endl;
        cin >> playagain;
        guesses.clear(); // Clears previous guesses
        guessesLeft = 6; // Resets lives.
    } while (toupper(playagain) == 'Y');
}
