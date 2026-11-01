#include <iomanip>
#include "textfile.h"

struct DiffLine {
    char type;      // ' ' = same in both files, '-' = only in A, '+' = only in B
    string text;
    int lineA;      // line number in file A (0 if not there)
    int lineB;      // line number in file B (0 if not there)
};

class Comparator {
public:
    //compare two file objects
    vector<DiffLine> compare(const TextFile& a, const TextFile& b) {
        const vector<string>& A = a.getLines();
        const vector<string>& B = b.getLines();
        int n = A.size(), m = B.size();

        // dp[i][j] = number of lines in common between A[i..] and B[j..]
        vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
        for (int i = n - 1; i >= 0; i--)
            for (int j = m - 1; j >= 0; j--)
                if (A[i] == B[j]) dp[i][j] = dp[i + 1][j + 1] + 1;
                else              dp[i][j] = max(dp[i + 1][j], dp[i][j + 1]);

        // read the table to build the result
        vector<DiffLine> result;
        int i = 0, j = 0;
        while (i < n && j < m) {
            if (A[i] == B[j]) {
                result.push_back({' ', A[i], i + 1, j + 1});
                i++; j++;
            } else if (dp[i + 1][j] >= dp[i][j + 1]) {
                result.push_back({'-', A[i], i + 1, 0});
                i++;
            } else {
                result.push_back({'+', B[j], 0, j + 1});
                j++;
            }
        }
        while (i < n) { result.push_back({'-', A[i], i + 1, 0}); i++; }
        while (j < m) { result.push_back({'+', B[j], 0, j + 1}); j++; }
        return result;
    }

    vector<DiffLine> compare(string pathA, string pathB) {
        TextFile a(pathA), b(pathB);
        return compare(a, b);
    }

    //prints only the differences then a summmary
    void print(const vector<DiffLine>& diff) {
        int same = 0, removed = 0, added = 0;
        cout << "\n";
        for (const DiffLine& d : diff) {
            if (d.type == ' ')      same++;
            else if (d.type == '-') { removed++; cout << "- [A:" << setw(3) << d.lineA << "] " << d.text << "\n"; }
            else                    { added++;   cout << "+ [B:" << setw(3) << d.lineB << "] " << d.text << "\n"; }
        }
        if (removed == 0 && added == 0) cout << "Files are IDENTICAL.\n";
        double similarity = diff.empty() ? 100.0 : 100.0 * same / diff.size();
        cout << "\nSummary: " << same << " same, " << removed << " only in A, "
             << added << " only in B | similarity " << fixed << setprecision(1)
             << similarity << "%\n";
    }
};
