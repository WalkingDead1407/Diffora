#include "comparator.h"

class Merger {
protected:
    static int mergesDone;      //static member counts all merges

public:
    virtual TextFile merge(const TextFile& a, const TextFile& b) = 0;   // pure virtual
    virtual string name() = 0;      // pure virtual
    virtual ~Merger() {}        // virtual destructor

    static int getMergesDone() { return mergesDone; }
};

int Merger::mergesDone = 0;

// all of A then all of B
class AppendMerger : public Merger {
public:
    TextFile merge(const TextFile& a, const TextFile& b) {
        mergesDone++;
        return a + b;
    }
    string name() { return "Append"; }
};

//common lines appear once and lines unique to A or B are all kept
class SmartMerger : public Merger {
public:
    TextFile merge(const TextFile& a, const TextFile& b) {
        mergesDone++;
        Comparator comparator;
        TextFile result;
        for (const DiffLine& d : comparator.compare(a, b))
            result.addLine(d.text);
        return result;
    }
    string name() { return "Smart merge"; }
};

class ConflictMerger : public Merger {
private:
    //writes the pending different lines. returns 1 if it was a conflict.
    int writeBlock(TextFile& out, vector<string>& onlyA, vector<string>& onlyB) {
        int conflict = 0;
        if (!onlyA.empty() && !onlyB.empty()) {    // both files changed here
            conflict = 1;
            out.addLine("<<<<<<< FILE A");
            for (const string& s : onlyA) out.addLine(s);
            out.addLine("=======");
            for (const string& s : onlyB) out.addLine(s);
            out.addLine(">>>>>>> FILE B");
        } else {                 // only one file changed then keep it
            for (const string& s : onlyA) out.addLine(s);
            for (const string& s : onlyB) out.addLine(s);
        }
        onlyA.clear();
        onlyB.clear();
        return conflict;
    }

public:
    TextFile merge(const TextFile& a, const TextFile& b) {
        mergesDone++;
        Comparator comparator;
        TextFile result;
        vector<string> onlyA, onlyB;
        int conflicts = 0;

        for (const DiffLine& d : comparator.compare(a, b)) {
            if (d.type == '-')      onlyA.push_back(d.text);
            else if (d.type == '+') onlyB.push_back(d.text);
            else {      // a common line ends the block
                conflicts += writeBlock(result, onlyA, onlyB);
                result.addLine(d.text);
            }
        }
        conflicts += writeBlock(result, onlyA, onlyB);

        cout << conflicts << " conflict(s) found.\n";
        return result;
    }
    string name() { return "Merge with conflict markers"; }
};
