#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#define max(a, b) (a > b ? a : b)

using namespace std;

int main() {
    ifstream file("count_big.txt");
    string line;
    vector<pair<string, uint64_t>> voc;
    int d;
    while (getline(file, line)) {
        d = 0;
        for (size_t l = 0, sz = line.length(); l < sz; ++l) {
            if (isspace(line[l])) {
                voc.push_back(make_pair(line.substr(0, l), stoi(line.substr(l + 1, sz - l + 1))));
                break;
            }
        }
        ++d;
    }
    file.close();

    vector<unordered_set<string>> id_bigram(voc.size());
    unordered_map<string, unordered_set<uint64_t>> bigram;
    string bt;
    for (size_t i = 0, sz = voc.size(); i < sz; ++i) {
        if (voc[i].first.size() == 1) {
            bigram[voc[i].first].insert(i);
            id_bigram[i].insert(voc[i].first);
        }
        for (size_t j = 0, size = voc[i].first.size() - 1; j < size; ++j) {
            bt = voc[i].first.substr(j, 2);
            bigram[bt].insert(i);
            id_bigram[i].insert(bt);
        }
    }

    string word;
    unordered_set<string> bword;
    vector<pair<uint64_t, double>> score(voc.size());
    while (cin >> word) {
        fill(score.begin(), score.end(), make_pair(0, 0));
        if (word.length() == 1) {
            cout << word << '\n';
            continue;
        }
        for (size_t j = 0, len = word.length() - 1; j < len; ++j) {
            bword.insert(word.substr(j, 2));
        }
        for (auto b : bword) {
            for (auto it : bigram[b]) {
                score[it].first = it;
                score[it].second += 1;
            }
        }
        for (auto &it : score) {
            it.second = static_cast<double>(it.second) / (bword.size() - it.second + id_bigram[it.first].size());
        }
        bword.clear();
        sort(score.begin(), score.end(), [&voc](const auto &a, const auto &b) -> bool {
            if (a.second == b.second) {
                if (voc[a.first].second == voc[b.first].second) {
                    return voc[a.first].first < voc[b.first].first;
                }
                else {
                    return voc[a.first].second > voc[b.first].second;
                }
            }
            else {
                return a.second > b.second;
            }
        });
        cout << voc[score[0].first].first << '\n';
/*        cout << score[0].second << ' ' << voc[score[0].first].first << ' ' << voc[score[0].first].second << '\n';
        cout << score[1].second << ' ' << voc[score[1].first].first << ' ' << voc[score[1].first].second << '\n';
        cout << score[2].second << ' ' << voc[score[2].first].first << ' ' << voc[score[2].first].second << '\n'; */
    }
    return 0;
}   