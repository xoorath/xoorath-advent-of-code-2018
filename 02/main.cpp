#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

// each line in inputs.txt has an id. Keep track of how many lines
// contain letters that occur twice, and how many lines contain letters
// that occur three times. Multiply the number of double and tripple containing lines.
void Question1() {
    fstream fs("input.txt");
    string s;
    int twos = 0;
    int threes = 0;
    while(getline(fs, s)) {
        sort(s.begin(), s.end());
        char lastCharacter = s[0];
        int charOccurance = 1;
        bool hasTwos = false;
        bool hasThrees = false;

        for(size_t i = 1; i < s.length(); ++i) {
            if(lastCharacter == s[i]) {
                charOccurance++;
            } else {
                if(charOccurance == 2) {
                    hasTwos = true;
                } else if(charOccurance == 3) {
                    hasThrees = true;
                }
                // Since we didn't find a chain of characters here,
                // start a new chain starting on this char
                charOccurance = 1;
                lastCharacter = s[i];
            }
        }
        // make sure we count the last letter
        if(charOccurance == 2) {
            hasTwos = true;
        } else if(charOccurance == 3) {
            hasThrees = true;
        }

        if(hasTwos) 
            twos++;
        if(hasThrees) 
            threes++;
    }
    cout << "Question 1: " << (twos * threes) << endl;
}

// Returns the number of characters that differ between the left and right input strings
// only considers characters up to the shorter of the two strings.
int DiffStr(char const* l, char const* r) {
    int d = 0;
    while(*l != '\0' && *r != '\0') {
        if(*l != *r) {
            d++;
        }
        l++;
        r++;
    }
    return d;
}

// Prints each character that is different in the left and right strings.
// only considers characters up to the shorter of the two strings.
void PrintDiff(char const* l, char const* r) {
    int d = 0;
    while(*l != '\0' && *r != '\0') {
        if(*l == *r) {
            cout << *l;
        }
        l++;
        r++;
    }
}

// In the input lines, find the two 
void Question2() {
    fstream fs("input.txt");
    string s;
    vector<string> strs;
    strs.reserve(250);
    while(getline(fs, s)) {   
        strs.push_back(s);
    }
    for(size_t i = 0; i < strs.size(); ++i) {
        for(size_t j = 0; j < strs.size(); ++j) {
            if(i==j) continue;
            if(DiffStr(strs[i].c_str(), strs[j].c_str()) == 1) {
                cout << "Question 2: ";
                PrintDiff(strs[i].c_str(), strs[j].c_str());
                cout  << endl;
                return;
            }
        }
    }
}

int main() {
    Question1();
    Question2();
}