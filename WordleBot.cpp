#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <set>
#include <time.h>
#include <map>

using namespace std;
class wordle {
    public:
    wordle() {
        srand(time(NULL));
        infile.open("database.csv");
        outfile.open("database.csv",ios::app);
        seedFile.open("WordleList.csv");
        
        string line1;
        string line2;
        int i = 0;
        string line;
        while (getline(seedFile,line)) {
            seedList.insert(line);
        }
    
        while (!infile.eof()) {
            if ((i % 2 == 0) ) {
                getline(infile, line1);
                
            }
            else {
                getline(infile, line2);

                database.insert({line1, line2});
            }
            i++;

        
    }

    }
    set<string> solver(string guess, string feedback) {
        string currentWord;
        set<string> newPossibleWords;
        for (set<string>::iterator itr = possibleWords.begin(); itr != possibleWords.end(); itr++) {
            currentWord = *itr;
            for (int i = 0; i < feedback.length(); i++) {
                if (feedback.at(i) == 'g') {
                    if (currentWord.at(i) != guess.at(i)) {
                        break;

                    }
                }
                else if(feedback.at(i) == 'y') {
                    if(currentWord.find(guess.at(i)) == string::npos ) {
                        break;
                    }
                    if (currentWord.at(i) == guess.at(i)) {
                        break;
                    } 
                }
                else {
                    for (int j = 0; j < guess.length(); j++ ) {
                        if (feedback.at(j) == 'g') {
                    
                            if (guess.at(i) == currentWord.at(j)) {
                                goto jump;
                            }

                        }
                    }
                    
                    if(currentWord.find(guess.at(i)) != string::npos ) {
                        break;
                    }
        
                }
                jump:
                if (i == feedback.length() - 1) {
                    newPossibleWords.insert(currentWord);
                
                }

            }
        }
        return newPossibleWords;
    }
    void play() {
        
        string guess;
        string feedback;
        cin >> guess >> feedback;
        string output;
        possibleWords = seedList;
        for (int i = 0; i < 6; i++) {
            
            possibleWords = solver(guess, feedback);

            guess = lookup(guess,feedback);
            cout << guess << endl;
            cin >> feedback;
            if (feedback == "done") {
                return;
            }
    }

    }
    double simulate(string startword) {
        
        int seed = (rand() + time(NULL)) % seedList.size();
        
        set<string>::iterator itr = seedList.begin();
        for (int i = 0; i < seed; i++) {
            itr++;
        }
        string key = *itr;
        string inputWord;
        string output;
        string firstGuess = startword;
        possibleWords = seedList;
        for (int i = 0; i < 7; i++) {
            if (comp(firstGuess, key) == "ggggg") {
                //cout << "Found the answer " << firstGuess << " in " << i + 1 << " tries" << endl;
                return i + 1;
            }
            possibleWords = solver(firstGuess, comp(firstGuess, key));
            firstGuess = lookup(firstGuess, comp(firstGuess, key));
            //cout << "Guessing " << firstGuess << endl;

        }
        //cout << "Could not solve. Answer was " << key << endl;
        return 7;

    }
    double findAverage(string startword, int iterations) {
        double average = 0;
        for (int i = 0; i < iterations; i++) {
            average += simulate(startword);
        }
        return average/iterations;
    }
    private:
    map<string, string> database;
    set<string> possibleWords;
    ifstream infile;
    ofstream outfile;
    ifstream seedFile;
    set<string> seedList;
    
    string comp(string guess, string key) {
        string response;
        string progress;
        for (int i = 0; i < guess.length(); i++) {
            progress = response;
            if (guess.at(i) == key.at(i)) {
                response = response + "g";
            }
            else {
                for (int j = 0; j < guess.length(); j++) {
                    if (guess.at(i) == key.at(j)) {
                        response = response + "y";
                        j = guess.length() - 1;
                    }

                }
            }
            if (response == progress) {
                response = response + "w";
            }
        }
        return response;

    }
    string guesser() {
        string testWord;
        string guessWord;
        string winningWord = "unable to find";
        int guessPoolSize = 0;
        int averagePoolSizeOld = possibleWords.size();
        int averagePoolSizeNew = 0;
        for (set<string>::iterator itr1 = possibleWords.begin(); itr1 != possibleWords.end(); itr1++) {
            guessWord = *itr1;
            for (set<string>::iterator itr2 = possibleWords.begin(); itr2 != possibleWords.end(); itr2++) {
                testWord = *itr2;
                guessPoolSize += solver(guessWord, comp(guessWord, testWord)).size();
            }
            averagePoolSizeNew = guessPoolSize / possibleWords.size();
            guessPoolSize = 0;
            if (averagePoolSizeNew <= averagePoolSizeOld) {
                averagePoolSizeOld = averagePoolSizeNew;
                winningWord = *itr1;
            }
            cout << "Simulating " << guessWord << endl;

        }
        return winningWord;

    }
    string lookup(string guess, string feedback) {
        
        string wordGuess;
        string databaseKey = guess + feedback;
        if(database.find(databaseKey) == database.end()) {
            wordGuess = guesser();

            database.insert({databaseKey,wordGuess});

            outfile << databaseKey << endl << wordGuess << endl;
            
        }
        else {
            
            wordGuess = database.at(databaseKey);   
        }
        return wordGuess;
    }
    
};

void findBestWord(wordle* wordleBot) {
    ifstream masterList("WordleList.csv");
    set<string> dictionary;
    string line;
    set<string> leaderboard;
    while(getline(masterList,line)) {
        dictionary.insert(line);
    }
    
    string boardEntry;
    int n = 0;
    int end = dictionary.size();
    for (set<string>::iterator itr = dictionary.begin(); itr != dictionary.end(); itr++) {
        n++;
        cout << n << "/" << end << endl;
        cout << "Testing " << *itr << endl;
        double average = 0;
        for (int i = 0; i < 1000; i++) {
            average += wordleBot->simulate(*itr);
        }
        boardEntry = to_string(average/1000) + " " + *itr;
        //cout << boardEntry << endl;
        leaderboard.insert(boardEntry);
        set<string>::iterator itr2 = leaderboard.begin();
        for (int i = 0; i < 3; i++) {
            if (itr2!= leaderboard.end()) {
                cout << *itr2 << endl;
                itr2++;
            }
        }
    }
    for (set<string>::iterator itr = leaderboard.begin(); itr != leaderboard.end(); itr++) {
        cout << *itr << endl;
    }

}



int main() {
    wordle wordleBot;
    wordleBot.play();
      
    

    return 0;
}


