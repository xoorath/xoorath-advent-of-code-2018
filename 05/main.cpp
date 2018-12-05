#include <fstream>
#include <list>
#include <iostream>
#include <vector>

using namespace std;

void ReactPolymer(string& str) {
    size_t oldLen = str.length();
    size_t newLen = 0;
    while(oldLen != newLen) {
        oldLen = str.length();
        for(size_t i = 0; i < str.length()-1 ; ++i) {
            char l = tolower(str[i]);
            char L = toupper(str[i]);
            if( (str[i]==l && str[i+1]==L) || 
                (str[i]==L && str[i+1]==l) ) {
                str.erase(str.begin()+i);
                str.erase(str.begin()+i);
                i-=2;
            }
        }
        newLen = str.length();
    }
}

void Question1() {
    fstream fs("input.txt");
    string str((istreambuf_iterator<char>(fs)), istreambuf_iterator<char>());
    ReactPolymer(str);
    cout << "Question 1: " << str.length() << endl;
}

void Question2() {
    fstream fs("input.txt");
    string str((istreambuf_iterator<char>(fs)), istreambuf_iterator<char>());
    vector<pair<char, string>> possiblePolymers;
    string reducableChars = "abcdefghijklmnopqrstuvwxyz";
    for(char x : reducableChars) {
        string tempStr = str;
        tempStr.erase(std::remove(tempStr.begin(), tempStr.end(), x), tempStr.end());
        tempStr.erase(std::remove(tempStr.begin(), tempStr.end(), toupper(x)), tempStr.end());
        ReactPolymer(tempStr);
        possiblePolymers.push_back(pair<char,string>(x, tempStr));
    }
    size_t shortest = str.length();
    char shortestChar = '_';
    for(size_t i = 0; i < possiblePolymers.size(); ++i) {
        size_t l = possiblePolymers[i].second.length();
        if(l < shortest) {
            shortest = l;
            shortestChar = possiblePolymers[i].first;
        }
    }
    cout << "Question 2: " << shortestChar  << toupper(shortestChar) << ": " << shortest << endl;
}

int main() {
    Question1();
    Question2();
}