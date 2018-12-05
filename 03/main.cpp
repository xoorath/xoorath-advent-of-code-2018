#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>

using namespace std;

constexpr int FabricDimension = 1000;

struct Claim {
    int id, x, y, w, h;
    Claim(int id, int x, int y, int w, int h) 
        : id(id)
        , x(x)
        , y(y)
        , w(w)
        , h(h)
        {}

    void WriteOverlap(char fabric[]) const {
        for(int yy = y; yy < y+h; ++yy) {
            for(int xx = x; xx < x+w; ++xx) {
                switch(fabric[yy*FabricDimension+xx]) {
                    case 0:
                        // indicate that this part of the fabric is used.
                        fabric[yy*FabricDimension+xx] = 1;
                    break;
                    case 1:
                        // indicate an overlap in some way, doesn't really matter what we write
                        fabric[yy*FabricDimension+xx] = 2;
                    break;
                }
            }
        }
    }

    bool Overlaps(Claim const& other) const {
        return
            id != other.id &&
            x < other.x+other.w &&
            x+w > other.x &&
            y+h > other.y &&
            y < other.y+other.h;
    }
};

vector<Claim> ParseClaims() {
    fstream fs("input.txt");
    string s;
    vector<Claim> claims;
    claims.reserve(1233);
    int id, x, y, w, h;
    char discard;
    while(getline(fs, s)) {
        stringstream ss(s);
        // #1233 @ 923,156: 16x21
        ss 
            >> discard // #
            >> id      // 1223
            >> discard // '@'
            >> x       // 923
            >> discard // ,
            >> y       // 156
            >> discard // :
            >> w       // 16
            >> discard // x
            >> h;      // 21

        claims.emplace_back(id, x, y, w, h);
    }
    return claims;
}

void Question1() {
    vector<Claim> claims = ParseClaims();
    size_t overlapping = 0;
    char fabric[FabricDimension*FabricDimension] = {0};
    for(size_t i = 0; i < claims.size(); ++i) {
        claims[i].WriteOverlap(fabric);
    }
    for(size_t y = 0; y < FabricDimension; ++y) {
        for(size_t x = 0; x < FabricDimension; ++x) {
            if(fabric[y*FabricDimension+x] == 2) overlapping++;
        }
    }
    cout << "Question 1: " << overlapping << endl;
}

void Question2() {
    vector<Claim> claims = ParseClaims();
    for(size_t i = 0; i < claims.size(); ++i) {
        bool overlapped = false;
        for(size_t j = 0; j < claims.size(); ++j) {
            if(claims[i].Overlaps(claims[j])) {
                overlapped = true; break;
            }
        }
        if(!overlapped) {
            cout << "Question 2: " << claims[i].id << endl;
            return;
        }
    }
    cerr << "Couldn't find a non-overlapping claim for question 2!" << endl;
}

int main() {
    Question1();
    Question2();
}