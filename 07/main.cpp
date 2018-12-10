#include <string>
#include <algorithm>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <set>
#include <list>

using namespace std;

struct Instruction {
    string name;
    bool complete, inProgress;
    vector<Instruction*> dependedOnBy;
    vector<Instruction*> dependsOn;
    bool IsAvailable() const {
        if(complete || inProgress) return false;
        for(auto inst : dependsOn) {
            if(!inst->complete) return false;
        }
        return true;
    }
};

vector<Instruction*> GetInstructions() {
    struct InstructionDescription {
        string blocker, task;
    };
    vector<Instruction*> instructions;
    vector<InstructionDescription> descriptions;
    string blocker, task, discard, line;
    fstream fs("input.txt");
    while(getline(fs, line)) {
        // Step A must be finished before step B can begin.
        stringstream ss(line);
        ss 
            >> discard // step
            >> blocker // A
            >> discard // must
            >> discard // be
            >> discard // finished
            >> discard // before
            >> discard // step
            >> task;   // B
        descriptions.push_back({blocker, task});
    }
    set<string> taskNames;
    for(auto const& desc : descriptions) {
        taskNames.insert(desc.blocker);
        taskNames.insert(desc.task);
    }
    for(auto const& task : taskNames) {
        instructions.push_back(new Instruction({task, false, false, vector<Instruction*>()}));
    }
    for(auto const& desc : descriptions) {
        auto foundTask = find_if(instructions.begin(), instructions.end(), [&](Instruction*const& inst)->bool{
            return desc.task == inst->name;
        });
        if(foundTask == instructions.end()) {
            cerr << "couldn't find task: " << desc.task << endl;
            return instructions;
        }
        auto foundBlocker = find_if(instructions.begin(), instructions.end(), [&](Instruction*const& inst)->bool{
            return desc.blocker == inst->name;
        });
        if(foundBlocker == instructions.end()) {
            cerr << "couldn't find blocker: " << desc.blocker << endl;
            return instructions;
        }
        (*foundBlocker)->dependedOnBy.push_back((*foundTask));
        (*foundTask)->dependsOn.push_back((*foundBlocker));
    }
    return instructions;
}

void Question1() {
    auto instructions = GetInstructions();

    sort(instructions.begin(), instructions.end(), [](Instruction*const&l, Instruction*const&r) -> bool {
        return l->dependsOn.size() < r->dependsOn.size();
    });

    cout << "Question 1: ";
    vector<Instruction*> available = {instructions[0]};
    while(available.size() != 0) {
        sort(available.begin(), available.end(), [](Instruction*const& l, Instruction*const& r)->bool{
            return l->name < r->name;
        });
        auto front = available[0];
        available[0] = available[available.size()-1];
        available.resize(available.size()-1);
        cout << front->name;
        front->complete = true;
        // add all newly available instructions
        for(auto inst : instructions) {
            if(inst->IsAvailable() && find(available.begin(), available.end(), inst) == available.end()) {
                available.push_back(inst);
            }
        }
    }
    cout << endl;

    for(auto inst : instructions) {
        delete inst;
    }
}

void Question2() {
    auto instructions = GetInstructions();

    auto TryTakeNextJob = [&](Instruction** outInstruction) -> bool {
        vector<Instruction*> available;
        // add all available tasks
        for(auto inst : instructions) {
            if(inst->IsAvailable()) {
                available.push_back(inst);
            }
        }
        if(available.size() == 0) return false;
        sort(available.begin(), available.end(), [](Instruction*const& l, Instruction*const& r)->bool{
            return l->name < r->name;
        });
        auto front = available[0];
        available[0] = available[available.size()-1];
        available.resize(available.size()-1);
        front->inProgress = true;
        *outInstruction = front;
        return true;
    };
    
    struct Job {
        int timeRemaining;
        Instruction* task;
    };
    vector<Job> elves = {
        {0, nullptr}, 
        {0, nullptr}, 
        {0, nullptr}, 
        {0, nullptr}
    };
    int duration = -1;
    int incomplete = instructions.size();
    while(incomplete != 0) {
        // tick down and complete each task
        duration++;
        for(Job& job : elves) {
            if(job.task != nullptr) {
                --job.timeRemaining;
                if(job.timeRemaining <= 0) {
                    job.task->complete = true;
                    --incomplete;
                    job.task = nullptr;
                }
            }
        }
        
        // give each available job a task, if we can.
        for(Job& job : elves) {
            if(job.task == nullptr) {
                Instruction* inst;
                if(TryTakeNextJob(&inst)) {
                    job.task = inst;
                    job.timeRemaining = 60 + (int)(job.task->name[0] - 'A') + 1;
                }
            }
        }
    }
    cout << "Question 2: " << duration << endl;

    for(auto inst : instructions) {
        delete inst;
    }
}

int main() {
    Question1();
    Question2();
}