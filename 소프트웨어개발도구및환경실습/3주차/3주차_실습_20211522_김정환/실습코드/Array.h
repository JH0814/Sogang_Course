#ifndef __ARRAY__
#define __ARRAY__

class Array{
	protected:
		int *data; // 배열 생성할 포인터 변수
		int len; // 배열의 크기 저장할 변수
	public:
			Array(int size); // Array의 생성자
			~Array(); // Array의 소멸자

			int length() const; // Array의 크기 리턴

			int& operator[](int i); // 배열의 값 대입할 수 있는 operator []
			int operator[](int i)const; // 배열의 값 반환하는 operator []

			void print(); // 배열의 모든 값 출력하는 함수
};
#endif
