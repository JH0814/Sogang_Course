#include <iostream>
using namespace std;
#include "RangeArray.h"

RangeArray::RangeArray(int i,int j):Array(j-i+1) // 생성자에서 생성하면서 size를 부모 클래스인 Array에 전달
{
	low = i; // 가장 낮은 인덱스 저장
	high = j;  // 가장 높은 인덱스 저장
}

RangeArray::~RangeArray()
{
	// 아무 것도 없을 시 부모 클래스의 소멸자 실행
}

int RangeArray::baseValue()
{
	return low; // 가장 낮은 인덱스 저장한 low 리턴
}
int RangeArray::endValue()
{
	return high; // 가장 높은 인덱스 저장한 high 리턴
}
int& RangeArray::operator [](int i)
{
	return Array::operator [](i-low); // 부모클래스에서 생성된 operator에 인자만 +로 될 수 있도록 i-low 전달
}
int RangeArray::operator [](int i) const
{	
	return Array::operator [](i-low); // 부모클래스에서 생성된 operator에 인자만 +로 될 수 있도록 i-low 전달
}
