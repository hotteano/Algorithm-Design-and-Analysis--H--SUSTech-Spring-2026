#include <bits/stdc++.h>
using namespace std;

int N, M;

struct college {
    int cap;
    vector<int> comment;
};

struct student {
    vector<int> pref;
};

void shapley_gale(vector<student> &s, vector<college> &c) {
    int n = s.size();
    int m = c.size();

    vector<int> nxt(n, 0);

    using P = pair<int, int>;
    vector<priority_queue<P, vector<P>, greater<P>>> college_heaps(m);

    queue<int> free_students;
    for (int i = 0; i < n; i++) {
        free_students.push(i);
    }

    while (!free_students.empty()) {
        int stu = free_students.front();
        free_students.pop();

        if (nxt[stu] >= s[stu].pref.size()) continue;

        int col = s[stu].pref[nxt[stu]] - 1;
        nxt[stu]++;

        int score = c[col].comment[stu];

        if (college_heaps[col].size() < c[col].cap) {
            if (score > 0) {
                college_heaps[col].push({score, stu});
            } else {
                free_students.push(stu);
            }
        } else if (!college_heaps[col].empty()) {
            int worst_score = college_heaps[col].top().first;
            int worst_stu = college_heaps[col].top().second;
            if (score > worst_score) {
                college_heaps[col].pop();
                college_heaps[col].push({score, stu});
                free_students.push(worst_stu);
            } else {
                free_students.push(stu);
            }
        } else {
            free_students.push(stu);
        }
    }

    for (int i = 0; i < m; i++) {
        vector<int> admitted;
        while (!college_heaps[i].empty()) {
            admitted.push_back(college_heaps[i].top().second);
            college_heaps[i].pop();
        }

        sort(admitted.begin(), admitted.end());

        cout << admitted.size();
        for (int stu : admitted) {
            cout << " " << stu + 1;
        }
        cout << endl;
    }
}

int main() {
    cin >> N >> M;

    vector<student> s(N);
    vector<college> c(M);

    for (int i = 0; i < M; i++) {
        cin >> c[i].cap;
    }

    vector<vector<int>> student_scores(N, vector<int>(M));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            cin >> student_scores[i][j];
        }
    }

    for (int i = 0; i < N; i++) {
        vector<pair<int, int>> scores;
        for (int j = 0; j < M; j++) {
            if (student_scores[i][j] > 0) {
                scores.push_back({student_scores[i][j], j + 1});
            }
        }
        sort(scores.rbegin(), scores.rend());
        for (auto &[score, col] : scores) {
            s[i].pref.push_back(col);
        }
    }

    for (int i = 0; i < M; i++) {
        c[i].comment.resize(N);
        for (int j = 0; j < N; j++) {
            cin >> c[i].comment[j];
        }
    }

    shapley_gale(s, c);

    return 0;
}
