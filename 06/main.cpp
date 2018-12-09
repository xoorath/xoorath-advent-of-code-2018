#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;

struct Coord { int x, y; };

vector<Coord> GetInput() {
    vector<Coord> output;
    output.reserve(50);
    fstream fs("input.txt");
    string line;
    char discard;
    while(getline(fs, line)) {
        stringstream ss(line);
        int x, y;
        ss >> x >> discard >> y;
        output.push_back({x, y});
    }
    return output;
}

void Question1() {
    vector<Coord> input = GetInput();
    Coord 
        min = {numeric_limits<int>::max(), numeric_limits<int>::max()}, 
        max = {numeric_limits<int>::min(), numeric_limits<int>::min()};
    struct Node {
        Coord coord;
        bool infinite;
        int area;
    };
    vector<Node> nodes;
    nodes.reserve(input.size());
    for(Coord const& coord : input) {
        nodes.push_back({coord, false, 0});
        if(coord.x < min.x) min.x = coord.x;
        if(coord.x > max.x) max.x = coord.x;
        if(coord.y < min.y) min.y = coord.y;
        if(coord.y > max.y) max.y = coord.y;
    }
    for(int y = min.y; y < max.y+1; ++y) {
        for(int x = min.x; x < max.x+1; ++x) {
            Node* closest = nullptr;
            int d = numeric_limits<int>::max();
            for(Node& node : nodes) {
                int nd = abs(x-node.coord.x) + abs(y-node.coord.y);
                if(nd == d) {
                    d = nd;
                    closest = nullptr;
                } else if(nd < d) {
                    d = nd;
                    closest = &node;
                }
            }
            if(closest) {
                closest->area++;
                closest->infinite = closest->infinite || x == min.x || x == max.x || y == min.y || y == max.y;
            }
        }
    }
    int a = numeric_limits<int>::min();
    for(Node& node : nodes) {
        if(node.infinite == false && node.area > a) {
            a = node.area;
        }
    }
    cout << "Question 1: " << a << endl;
}

void Question2() {
    constexpr int target = 10000;
    vector<Coord> input = GetInput();
    Coord 
        min = {numeric_limits<int>::max(), numeric_limits<int>::max()}, 
        max = {numeric_limits<int>::min(), numeric_limits<int>::min()};
    for(Coord const& coord : input) {
        if(coord.x < min.x) min.x = coord.x;
        if(coord.x > max.x) max.x = coord.x;
        if(coord.y < min.y) min.y = coord.y;
        if(coord.y > max.y) max.y = coord.y;
    }
    int num = 0;
    for(int y = min.y; y < max.y+1; ++y) {
        for(int x = min.x; x < max.x+1; ++x) {
            int s = 0;
            for(Coord const& coord : input) {
                s += abs(x-coord.x) + abs(y-coord.y);
            }
            if(s < target) {
                num++;
            }
        }
    }
    cout << "Question 2: " << num << endl;
}

int main() {
    Question1();
    Question2();
}