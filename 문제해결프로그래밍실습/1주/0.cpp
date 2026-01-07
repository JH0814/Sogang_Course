#include <iostream>
#include <cassert>

/**
 * @brief Asks person A if they know person B.
 * @param a The number of person A.
 * @param b The number of person B.
 * @return true if A knows B, otherwise returns false.
 */
bool ask_a_to_know_b(int a, int b) {
    int result;
    std::cout << "? " << a << ' ' << b << std::endl;
    std::cin >> result;
    assert(result == 0 || result == 1);
    return result;
}

/**
 * @brief Verifies if person x is a celebrity.
 * @param x The number of the person to verify, or -1 if there is no celebrity.
 * @return true if the answer is correct, otherwise returns false.
 */
bool answer(int x) {
    int result;
    std::cout << "! " << x << std::endl;
    std::cin >> result;
    assert(result == 0 || result == 1);
    return result;
}

int main(){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    // TODO: write your logic here!
    // You can use the functions `ask_a_to_know_b` and `answer`.
    // Any invocation of print functions rather than `ask_a_to_know_b`
    // and `answer` will result in 0 points for this question.
    int cand = 1;
    for(int i = 2; i<=n; i++){
        if(ask_a_to_know_b(cand, i)){
            cand = i;
        }
    }
    bool is_cel = true;
    for(int i = 1; i<=n; i++){
        if(cand == i) continue;
        if(ask_a_to_know_b(cand, i) || !ask_a_to_know_b(i, cand)){
            is_cel = false;
            break;
        }
    }
    if(is_cel){
        answer(cand);
    }
    else{
        answer(-1);
    }
    return 0;
}
