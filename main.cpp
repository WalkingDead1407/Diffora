#include "merger.h"

string ask(string prompt) {
    string s;
    cout << prompt;
    getline(cin, s);
    return s;
}

void doCompare() {
    string pathA = ask("File A: ");
    string pathB = ask("File B: ");
    Comparator comparator;
    comparator.print(comparator.compare(pathA, pathB));
}

void doMerge() {
    string pathA = ask("File A: ");
    string pathB = ask("File B: ");
    TextFile a(pathA), b(pathB);

    cout << "\nMerge mode:\n 1. Append (A then B)\n 2. Smart merge (no duplicate lines)\n"
            " 3. Merge with conflict markers\n";
    string mode = ask("Choose: ");

    //base class pointer, derived-class object (runtime polymorphism)
    Merger* merger = NULL;
    if (mode == "1")      merger = new AppendMerger();
    else if (mode == "2") merger = new SmartMerger();
    else if (mode == "3") merger = new ConflictMerger();
    else { cout << "Invalid mode.\n"; return; }

    TextFile result = merger->merge(a, b);
    cout << "Used: " << merger->name() << "\n";
    delete merger;

    string outPath = ask("Output file: ");
    result.save(outPath);
    cout << "Merged " << result.size() << " lines into '" << outPath << "'.\n";
}

int main() {
    while (true) {
        cout << "\n===== File Merge & Comparator =====\n"
                "1. Compare two files\n2. Merge two files\n3. Show statistics\n4. Exit\n";
        string choice = ask("Choice: ");

        try {
            if (choice == "1")      doCompare();
            else if (choice == "2") doMerge();
            else if (choice == "3") {
                cout << "\nFiles loaded: " << TextFile::getFilesLoaded()
                     << "\nMerges done : " << Merger::getMergesDone() << "\n";
            }
            else if (choice == "4") break;
            else cout << "Invalid choice.\n";
        } catch (exception& e) {
            cout << "Error: " << e.what() << "\n";
        }
    }
    return 0;
}
