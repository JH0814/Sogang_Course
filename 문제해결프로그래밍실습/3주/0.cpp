#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

struct query {
    int R1, R2, C1, C2;
    double V;
};

void matrix_addition(int N, vector<vector<double>>& A, int Q, vector<query>& queries) {
    //TODO:
    vector<vector<double>> temp(N + 1, vector<double>(N + 1, 0.0));
    for(int i = 0; i<Q; i++){
        int R1 = queries[i].R1 - 1;
        int R2 = queries[i].R2 - 1;
        int C1 = queries[i].C1 - 1;
        int C2 = queries[i].C2 - 1;
        temp[R1][C1] += queries[i].V;
        temp[R2 + 1][C1] -= queries[i].V;
        temp[R1][C2 + 1] -= queries[i].V;
        temp[R2 + 1][C2 + 1] += queries[i].V;
    }
    for(int i = 0; i<N; i++){
        for(int j = 1; j<N; j++){
            temp[i][j] += temp[i][j - 1];
        }
    }
    for(int j = 0; j<N; j++){
        for(int i = 1; i<N; i++){
            temp[i][j] += temp[i - 1][j];
        }
    }
    for(int i = 0; i<N; i++){
        for(int j = 0; j<N; j++){
            A[i][j] += temp[i][j];
        }
    }
}

int main() {
    // Do NOT delete these lines unless you know what you are doing:
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    vector<vector<double>> A(N, vector<double>(N));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> A[i][j];
        }
    }

    int Q;
    cin >> Q;

    vector<query> queries(Q);
    for (int i = 0; i < Q; i++) {
        cin >> queries[i].R1 >> queries[i].C1 >> queries[i].R2 >> queries[i].C2 >> queries[i].V;
    }

    matrix_addition(N, A, Q, queries);

    cout << fixed << setprecision(12);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << A[i][j] << ' ';
        }
        cout << '\n';
    }

    return 0;
}