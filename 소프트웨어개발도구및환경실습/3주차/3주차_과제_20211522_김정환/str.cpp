#include "str.h"
Str::Str(int leng){// leng 은 string의 길이.
    if(leng < 0){ // 문자열 길이가 음수일 경우 오류 출력
        cout << "length can not be negative number" << endl; // 오류 메시지 출력
    }
    else if(leng == 0){ // 문자열 길이가 0일 경우 NULL만 들어있도록 초기화
        len = 0; // 길이는 0
        str = new char[1]; // str에 들어갈 문자는 '\0' 하나이므로 크기가 char 1개인 배열 할당
        str[0] = '\0'; // '\0' 대입
    }
    else{
        len = leng; // 변수 len에 주어진 길이 저장
        str = new char[leng + 1]; // '\0'을 포함하여 길이보다 1 큰 배열 할당해준다.
    }
} 
Str::Str(char *neyong){// neyong은 초기화할 내용이 들어감.
    if(neyong == NULL){ // neyong이 NULL일 경우
        len = 0; // len은 0으로 초기화
        str = new char[1]; // 크기가 1인 배열로 할당
        str[0] = '\0'; // '\0' 문자만 넣어준다.
    }
    else{ // NULL이 아닌 모든 경우
        str = new char[strlen(neyong) + 1]; // '\0'을 포함하기 위해 크기보다 1 큰 배열을 할당
        strcpy(str, neyong); // strcpy를 이용하여 str 배열에 복사
        len = strlen(neyong); // neyong의 길이로 len을 초기화
    }
} 
Str::~Str(){// 소멸자.
    delete[] str; // delete로 str 배열의 메모리 해제
} 
int Str::length(void){// string의 길이를 리턴하는 함수.
    return len; // string의 길이가 저장된 len을 리턴
} 
char *Str::contents(void){// string의 내용을 리턴하는 함수.
    return str; // string의 내용이 저장된 str을 리턴
} 
int Str::compare(class Str& a){// a의 내용과 strcmp.
    return strcmp(str, a.contents()); // Str 클래스의 값으로 들어왔을 때이므로 클래스의 멤버 함수로 값을 받아온다. 그 값을 strcmp로 str과 비교한 값을 리턴
} 
int Str::compare(char *a){ // a의 내용과 strcmp.
    return strcmp(str, a); // char 배열로 들어왔으므로 그냥 strcmp에 str과 비교하여 리턴
}
void Str::operator=(char *a){// string의 값을 대입.
    if(a == NULL){ // a 문자열이 NULL인 경우
        len = 0; // 길이를 0으로 수정
        delete[] str; // 기존 str 메모리 해제
        str = new char[1]; // 크기 char 1개 크기의 배열로 새로 할당
        str[0] = '\0'; // '\0' 문자만 넣어주기
    }
    else{
        delete[] str; // 기존 str 메모리 해제
        str = new char[strlen(a) + 1]; // 새로 a의 길이 + 1 크기의 배열로 새로 할당
        len = strlen(a); // 길이는 a의 길이로 저장
        strcpy(str, a); // a 문자열 복사하기
    }
} 
void Str::operator=(class Str& a){// Str의 내용을 대입. 기존 클래스는 NULL인 경우도 처리되어 있으므로 둘 다 상관 없음
    delete[] str; // 기존 문자열 메모리 해제
    str = new char[a.length() + 1]; // a.length() + 1의 크기의 배열로 새로 할당
    len = a.length(); // a.length()의 값으로 저장
    strcpy(str, a.contents()); // a.contents()로 a의 str값 받아와서 복사
} 