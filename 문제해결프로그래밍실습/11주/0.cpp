#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

bool cmp_meet(pair<int, int>& a, pair<int, int>& b){
    if(a.second == b.second){
        return a.first < b.first;
    }
    return a.second < b.second;
}

vector<pair<int, int>> salesman(int N, vector<pair<int, int>> meetings) {
    vector<pair<int, int>> answer;

    // TODO: Implement the salesman function
    sort(meetings.begin(), meetings.end(), cmp_meet);
    int end_time = 0;
    for(auto& meeting : meetings){
        if(meeting.first >= end_time){
            answer.push_back(meeting);
            end_time = meeting.second;
        }
    }

    return answer;
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;
    vector<pair<int, int>> meetings(N);

    for (int i = 0; i < N; i++) {
        cin >> meetings[i].first >> meetings[i].second;
    }

    vector<pair<int, int>> ans = salesman(N, meetings);

    for (auto meeting: ans) {
        cout << meeting.first << " " << meeting.second << " ";
    }
    cout << '\n';

    return 0;
}