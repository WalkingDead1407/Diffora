#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;

class TextFile {
private:
    string name;
    vector<string> lines;
    static int filesLoaded;       // static member shared by all file object

public:
    TextFile() {}
    TextFile(string fileName) { load(fileName); }  // parameterized constructor

    // Read a file into memory. Throws an exception if it can't be opened.
    void load(string fileName) {
        ifstream in(fileName);
        if (!in) throw runtime_error("Cannot open file: " + fileName);
        lines.clear();
        string line;
        while (getline(in, line)) {
            if (!line.empty() && line.back() == '\r') line.pop_back();
            lines.push_back(line);
        }
        name = fileName;
        filesLoaded++;
    }

    //write the lines to a filr
    void save(string fileName) const {
        ofstream out(fileName);
        if (!out) throw runtime_error("Cannot write file: " + fileName);
        for (const string& line : lines) out << line << "\n";
    }

    void addLine(string line) { lines.push_back(line); }
    const vector<string>& getLines() const { return lines; }
    int size() const { return lines.size(); }

    static int getFilesLoaded() { return filesLoaded; }   //static member function

    //file1 == file2 is true if all lines are the same
    bool operator==(const TextFile& other) const { return lines == other.lines; }

    //file1 + file2 gives all lines of file1 followed by file2
    TextFile operator+(const TextFile& other) const {
        TextFile result = *this;
        for (const string& line : other.lines) result.lines.push_back(line);
        return result;
    }

    // Friend function: cout << file prints it (needs access to private lines)
    friend ostream& operator<<(ostream& os, const TextFile& f) {
        for (const string& line : f.lines) os << line << "\n";
        return os;
    }
};
int TextFile::filesLoaded = 0;
