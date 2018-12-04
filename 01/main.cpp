#include <iostream>
#include <fstream>
#include <set>

using namespace std;

// sum all the values in input.txt
void Question1() {
    int x, a=0;
    fstream fs("input.txt");
    if(!fs.is_open()) {
        cerr << "01 Question1 - failed to open input.txt" << endl;
        return;
    }

    while(fs >> x) {
        a += x;
    }
    cout << "Question 1: " << a << endl;
}

// as you sum values, the result of the sum is called a frequency.
// find the first duplicate frequency. It may take many runs through the file.
void Question2() {
    set<int> collection;
    int x, a=0;
    int failsafe = 1024;
    bool first = true;
    while(failsafe--) {
        fstream fs("input.txt");
        if(!fs.is_open()) {
            cerr << "01 Question2 - failed to open input.txt" << endl;
            return;
        }
        while(fs >> x) {
            x += a;
            a = x;
            if(!first && collection.find(x) != collection.end()) {
                cout << "Question 2: " << x << endl;
                return;
            } else if(first) {
                first = false;
            } else {
                collection.insert(x);
            }
        }
    }
    cerr << "01 Question2 - failsafe triggered, we didn't find an answer." << endl;
}

int main() {
    Question1();
    Question2();
}