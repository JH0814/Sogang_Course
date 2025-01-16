#ifndef MTT
#define MTT
#include "Array.h" // Array.h 헤더파일 불러온다.

class RangeArray : public Array{ // Array로부터 상속받은 클래스
	protected : 
		int low; // 생성시 가장 낮은 인덱스 저장하는 변수
		int high;// 생성시 가장 높은 인덱스 저장하는 변수
	public :
		RangeArray(int,int); // RangeArray 생성자
		~RangeArray(); // RangeArray 소멸자

		int baseValue(); // 가장 낮은 인덱스 리턴
		int endValue(); // 가장 높은 인덱스 리턴

		int& operator[](int); // 배열에 값 대입할 수 있는 operator []
		int operator[](int) const; // 배열에 값 대입할 수 있는 operator []
	
};
#endif
