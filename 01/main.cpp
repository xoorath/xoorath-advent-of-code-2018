#include <iostream>
#include <fstream>
#include <set>

using namespace std;

// sum all the values in input.txt
void Question1() {
    int x, a=0;
    fstream fs("input.txt");
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
    collection.insert(0);
    while(true) {
        fstream fs("input.txt");
        while(fs >> x) {
            x += a;
            a = x;
            if(collection.find(x) != collection.end()) {
                cout << "Question 2: " << x << endl;
                return;
            }
            collection.insert(x);
        }
    }
}

int main() {
    Question1();
    Question2();
}