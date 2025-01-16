#include "Array.h"
#include <cstdlib>
#include<iostream>
using namespace std;
Array::Array(int size)
{
	//사이즈를 확인하고 양수이면 new를 사용하여 배열 data를 할당, len값 초기화
	if(size<=0)
	{
		cout << "Size can not be negative integer!" << endl; // 사이즈가 0 이하이면 오류 출력
	}
	else
	{
		data = new int[size]; // 전달된 사이즈만큼 배열 동적할당
		len = size; // 배열의 크기 변수에 저장
	}
}
Array::~Array()
{
	// 소멸자; 할당된 메모리 해제
	delete[] data;
}
int Array::length() const
{
	// 배열의 크기 리턴
	return len; // 배열의 크기 저장된 len 리턴
}

// 배열에 원소를 대입하거나 값을 반환하는 부분으로 []연산자의 오버로딩이다
int& Array::operator[](int i) // 배열에 원소 삽입
{
	static int tmp;

	// 배열의 인덱스가 범위 내에 있으면 값 리턴, 그렇지 않으면 에러메세지 출력하고 tmp리턴
	if(i >= 0 && i < len)
	{
		return data[i]; // 배열의 값 리턴
	}
	else
	{
		cout << "Array bound error!" << endl; // 출력하는 오류 메시지
		return tmp; // 에러 메시지 출력 후 tmp 리턴
	}
}

int Array::operator[](int i) const // 배열의 원소값 반환
{
	//배열의 인덱스가 범위 내에 잇으면 값을 리턴, 그렇지 않으면 에러메세지 출력하고 0을 리턴
	if(i >= 0 && i<len)
	{
		return data[i]; // 배열의 값 리턴
	}
	else
	{
		cout << "Array bound error!" << endl; // 오류 메시지 출력
		return 0; // 에러 메시지 출력 후 0 리턴
	}
}
void Array::print() //배열의 모든 내용을 출력해주는 함수
{
	int i;
	cout<<"["; // [ 로 시작하도록 출력
	for(i = 0; i<len; i++){ // 0부터 len-1까지 저장되어 있으므로 전체 출력위해 0부터 len-1까지 반복
		cout << data[i]; // 배열의 값 출력
		if(i != len - 1){ // 마지막 값일 때와 아닐 때 구분하여 뒤 문자 처리
			cout << " "; // 마지막이 아닐 때는 띄어쓰기로 구분
		}
		else{
			cout << "]"; // 마지막에는 앞에서 출력한 [가 닫히도록 ] 출력
		}
	}

		
	cout<<endl; // 마지막에 줄 바뀌도록함
}
