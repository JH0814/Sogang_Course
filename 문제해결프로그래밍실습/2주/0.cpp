#include <iostream>
#include <vector>
#include <queue>
using namespace std;

vector<int> findNLargestElements(const vector<vector<int>>& matrix, int n) {
    vector<int> largestElements;
    // TODO:
    priority_queue<int, vector<int>, greater<int>> pq;
    for(int i = 0; i<n; i++){
        for(int j = 0; j<n; j++){
            if(pq.size() < n){
                pq.push(matrix[i][j]);
            }
            else if(pq.top() < matrix[i][j]){
                pq.pop();
                pq.push(matrix[i][j]);
            }
        }
    }
    vector<int> semi;
    for(int i = 0; i<n; i++){
        semi.push_back(pq.top());
        pq.pop();
    }
    for(int i = semi.size() - 1; i>=0; i--){
        largestElements.push_back(semi[i]);
    }
    return largestElements;
}

int main() {
    // Do NOT delete these lines unless you know what you are doing:
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<int>> M(n, vector<int>(n));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> M[i][j];
        }
    }

    vector<int> largestElements = findNLargestElements(M, n);

    for (int element : largestElements) {
        cout << element << "\n";
    }

    return 0;
}
