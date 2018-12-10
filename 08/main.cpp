#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int sumMetadata(int* tree, int* sum) {
    int children = tree[0];
    int md = tree[1];
    int* curr = tree+2;
    for(int i = 0; i < children; ++i) {
        curr += sumMetadata(curr, sum);
    }
    for(int i = 0; i < md; ++i) {
        *sum += *curr;
        curr++;
    }
    return curr-tree;
}

void Question1() {
    fstream fs("input.txt");
    vector<int> tree;
    int x;
    tree.reserve(16623);
    while(fs >> x) {
        tree.push_back(x);
    }
    x = 0;
    sumMetadata(tree.data(), &x);
    cout << "Question 1: " << x << endl;
}

struct Entry {
    vector<int> meta;
    vector<Entry*> children;
    int value;
};

int BuildEntries(int* tree, vector<Entry*>& entries, vector<Entry*>& parentsChildren) {
    Entry* entry = new Entry();
    entry->value = -1;
    entries.push_back(entry);
    parentsChildren.push_back(entry);

    int children = tree[0];
    int md = tree[1];
    int* curr = tree+2;
    for(int i = 0; i < children; ++i) {
        curr += BuildEntries(curr, entries, entry->children);
    }
    int sum = 0;
    for(int i = 0; i < md; ++i) {
        sum += *curr;
        entry->meta.push_back(*curr);
        curr++;
    }
    if(children == 0) {
        entry->value = sum;
    }
    return curr-tree;
}

void Question2() {
    fstream fs("input.txt");
    vector<int> tree;
    int x;
    tree.reserve(16623);
    while(fs >> x) {
        tree.push_back(x);
    }
    vector<Entry*> entries;
    vector<Entry*> rootParent;
    BuildEntries(tree.data(), entries, rootParent);
    bool foundAll = false;
    while(foundAll == false) {
        foundAll = true;
        for(auto* entry : entries) {
            if(entry->value == -1) {
                bool metaSatisfied = true;
                int metaSum = 0;
                for(int m : entry->meta) {
                    if(m > 0 && m <= entry->children.size()) {
                        if(entry->children[m-1]->value != -1) {
                            metaSum += entry->children[m-1]->value;
                        } else {
                            metaSatisfied = false;
                        }
                    }
                }
                if(metaSatisfied) {
                    entry->value = metaSum;
                }
            }
        }
        int remaining = 0;
        for(auto* entry : entries) {
            if(entry->value == -1) {
                remaining++;
            }
        }
        foundAll = remaining == 0;
    }
    cout << "Question 2: " << rootParent[0]->value << endl;
    for(auto* entry : entries) {
        delete entry;
    }
}

int main() {
    Question1();
    Question2();
}