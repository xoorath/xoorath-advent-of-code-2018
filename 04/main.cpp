#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <locale>
#include <sstream>
#include <unordered_map>
#include <vector>

using namespace std;
constexpr int MinsInDay = 1440;
enum LogKind {
    WAKE, SLEEP, START
};

struct LogEntry {
    unsigned id;
    tm time;
    LogKind kind;
};

struct Shift {
    unsigned id;
    int asleepMinutes[MinsInDay];
    int NumMinutesAsleep() const {
        int num=0;
        for(int i = 0; i < MinsInDay; ++i) {
            if(asleepMinutes[i] != 0) num++;
        }
        return num;
    }
};

vector<LogEntry> GetEntries() {
    fstream fs("input.txt");
    string s;
    char discard;
    unsigned id, year, month, day, hour, minute;
    vector<LogEntry> logs;
    LogKind kind;
    tm time;
    string remainder;
    while(getline(fs, s)) {
        stringstream ss(s);
        ss >> get_time(&time, "[%Y-%m-%d %H:%M]");
        getline(ss, remainder);
        if(remainder.find("begins shift") != string::npos) {
            kind = START;
            remainder = remainder.substr(remainder.find("#")+1);
            stringstream sss(remainder);
            sss >> id;
        } else if(remainder.find("falls asleep") != string::npos) {
            kind = SLEEP;
            id = -1;

        } else if(remainder.find("wakes up") != string::npos) {
            kind = WAKE;
            id = -1;
        } else {
            cerr << "found line we don't understand:" << s << endl;
            cerr << "remainder: " << remainder << endl;
            return logs;
        }
        // difftime wont work properly with time before 1970, our dataset is 
        time.tm_year += 1000;

        logs.push_back({id, time, kind});
    }
    sort(logs.begin(), logs.end(), [](LogEntry const& l, LogEntry const& r) -> bool {
        tm lt = l.time, rt = r.time;
        return difftime(mktime(&lt), mktime(&rt)) < 0.0;
    });
    unsigned lastid = -1;
    for(auto& log : logs) {
        if(log.id == -1)
            log.id = lastid;
        else 
            lastid = log.id;
    }
    return logs;
}

vector<Shift> GetShifts(vector<LogEntry> const& entries)
{
    vector<Shift> shifts;
    int asleep = 0;
    for(auto const& entry : entries) {
        switch(entry.kind) {
            case WAKE: 
                {
                    Shift s = {entry.id,  {0}};
                    for(int i = asleep; i < entry.time.tm_min; ++i) {
                        s.asleepMinutes[i]++;
                    }
                    shifts.push_back(s);
                }
            break;
            case SLEEP: asleep = entry.time.tm_min; break;
            default: break;
        }
    }
    return shifts;
}

unsigned GuardIdWithMostMinutesAsleep(vector<Shift> const& shifts) {
    unordered_map<unsigned, int> mp;
    for(auto const& shift : shifts) {
        mp[shift.id] += shift.NumMinutesAsleep();
    }
    unsigned sleepiestGuard = -1;
    int timeAsleep = 0;
    for(auto const& kvp : mp) {
        if(kvp.second > timeAsleep) {
            sleepiestGuard = kvp.first;
            timeAsleep = kvp.second;
        }
    }
    return sleepiestGuard;
}

int SleepiestMinute(unsigned guardId, vector<Shift> const& shifts) {
    int minutes[MinsInDay] = {0};
    for(auto const& shift : shifts) {
        if(guardId == shift.id) {
            for(int i = 0; i < MinsInDay; ++i) {
                minutes[i] += shift.asleepMinutes[i];
            }
        }
    }
    int sleepIdx = 0;
    int sleepVal = 0;
    for(int i = 0; i < MinsInDay; ++i) {
        if(minutes[i] > sleepVal) {
            sleepVal = minutes[i];
            sleepIdx = i;
        }
    }
    return sleepIdx;
}

void Question1() {
    auto logs = GetEntries();
    auto shifts = GetShifts(logs);
    unsigned sleepiestGuard = GuardIdWithMostMinutesAsleep(shifts);
    int sleepiestMinute = SleepiestMinute(sleepiestGuard, shifts);
    cout << "Question 1: " << (sleepiestGuard * sleepiestMinute) << endl;
}

void Question2() {
    auto logs = GetEntries();
    auto shifts = GetShifts(logs);
    unordered_map<unsigned, int[MinsInDay]> mp;
    for(auto const& shift : shifts) {
        int* arr = mp[shift.id];
        for(int i = 0; i < MinsInDay; ++i) {
            arr[i] += shift.asleepMinutes[i];
        }
    }
    unsigned sleepiestGuard = -1;
    int sleepiestVal = 0;
    int sleepiestMinute = 0;
    for(auto const& kvp : mp) {
        for(int i = 0; i < MinsInDay; ++i) {
            if(kvp.second[i] > sleepiestVal) {
                sleepiestVal = kvp.second[i];
                sleepiestGuard = kvp.first;
                sleepiestMinute = i;
            }
        }
    }
    cout << "Question 2: " << (sleepiestGuard * sleepiestMinute) << endl;
}

int main() {
    Question1();
    Question2();
}