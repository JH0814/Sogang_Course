#include<stdio.h>
#include<stdlib.h>
#include "20211522.h"


int main() {
	int n;//행렬의 행, 열의 크기
	char ch;//n값의 문자 형태의 임시 저장
	double det;//행렬식 변수
	FILE* fp = fopen("matrix.txt", "r");//'matrix.txt' 열기
	if (fp == NULL) {//파일 열기 에러 확인
		printf("file open error!\n");
		return 1;
	}
	ch = fgetc(fp);//txt 파일의 첫번째 즉 n을 문자로 받음
	n = (int)ch-'0';//n에 크기 형 변환하여 저장
	if (n <= 0) {//n<=0 일 시, 프로그램 종료
		printf("n<0!\n");
		return 1;
	}
	double** A = (double**)malloc(sizeof(double*) * n);//입력받을 행렬 A 선언
	if (A == NULL) {
		printf("memory error\n");
		return 1;
	}
	for (int i = 0; i < n; i++) {//A를 이차원 배열 형태로 선언
		*(A + i) = (double*)malloc(sizeof(double) * n);
		if (A+i == NULL) {
			printf("memory error\n");
			return 1;
		}
	}
	for (int i = 0; i < n; i++) {//nXn 형태의 행렬 입력받음
		for (int j = 0; j < n; j++) {
			fscanf(fp, "%lf", (*(A + i) + j));
		}
	}
	printf("A\n");
	printMatrix(A, n);//A 행렬 출력
	det = detA(A, n);//행렬식 값 구하기
	if (det == 0) {//행렬의 singular 여부 판단
		printf("singular\n");
		return 0;
	}
	double** B = (double**)malloc(sizeof(double*) * n);//A의 역행렬 B 선언
	if (B == NULL) {
		printf("memory error\n");
		return 1;
	}
	for (int i = 0; i < n; i++) {//B를 이차원 배열 형태로 선언
		*(B + i) = (double*)malloc(sizeof(double) * n);
		if (B + i == NULL) {
			printf("memory error\n");
			return 1;
		}
	}
	printf("Inverse Matrix\n");
	B = inverseMatrix(A, det, n);//역행렬 구하기
	
	printMatrix(B, n);//B 출력

	double** C = (double**)malloc(sizeof(double*) * n);//A와 B의 곱을 저장하는 C 선언
	if (C == NULL) {
		printf("memory error\n");
		return 1;
	}
	for (int i = 0; i < n; i++) {//C를 이차원 배열로 선언
		*(C + i) = (double*)malloc(sizeof(double) * n);
		if (C + i == NULL) {
			printf("memory error\n");
			return 1;
		}
	}
	printf("InverseCheck Matrix\n");
	C = inverseCheckMatrix(A, B, n);//A와 B의 곱 구해서 저장
	printMatrix(C, n);//C 출력
	//flag 변수를 이용하여 C가 항등 행렬인지 확인 맞으면 True 아니면 false 출력
	checkPrint(C, n);

	//메모리 해제 및 파일 종료
	free(A);
	fclose(fp);
	free(B);
	free(C);
}
double detA(double** A, int n) {
	double det = 0;
	if (n == 2) {
		return *(*A) * (*(*(A + 1) + 1)) - *(*A + 1) * *(*(A + 1));//2*2 행렬일 때 det값
	}
	else {//그외에서 det값
		for (int i = 0; i < n; i++) {
			det += *(*A + i) * cofactor(A, n, 1, i + 1);
		}
		return det;
	}
}
double cofactor(double** mat, int n, int i, int j) {//i행 j열에 대한 여인수 구하기
	if ((i + j) % 2 == 0) {
		return detA(M_Mat(mat, n, i, j), n - 1);//-1^(i+j)이 양수
	}
	else {
		return -(detA(M_Mat(mat, n, i, j), n - 1));//-1^(i+j)이 음수
	}
}
double** cofactorMatrix(double** A, int n) {
	double** con = (double**)malloc(sizeof(double*) * n);
	for (int i = 0; i < n; i++) {
		*(con + i) = (double*)malloc(sizeof(double) * n);
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			*(*(con + i) + j) = cofactor(A, n, i + 1, j + 1);//i,j에 따른 cofactor 함수를 이용하여 여인수 행렬 구함
		}
	}
	return con;//여인수 행렬 반환
}
double** M_Mat(double** A, int n, int row, int col) {//3*3 이상의 행렬에서 M(row,col) 구한다
	double* m_mat = (double*)malloc(sizeof(double) * (n - 1) * (n - 1));
	double** mat = (double**)malloc(sizeof(double*) * (n - 1));
	for (int i = 0; i < n - 1; i++) {
		*(mat + i) = (double*)malloc(sizeof(double) * (n - 1));
	}
	int cnt = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (i != row - 1 && j != col - 1) {
				*(m_mat + cnt) = *(*(A + i) + j);//우선 일차원 배열에 순서대로 저장
				cnt++;
			}
		}
	}
	mat = turn_two(m_mat, n - 1);//해당 일차원 배열을 이차원 배열로 변환
	return mat;//M(row, col) 반환
}
double** turn_two(double* A, int n) {//일차원 배열을 이차원 배열로 변환
	double** mat = (double**)malloc(sizeof(double*) * n);
	for (int i = 0; i < n; i++) {
		*(mat + i) = (double*)malloc(sizeof(double) * n);
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			*(*(mat + i) + j) = *(A + i * n + j);
		}
	}
	return mat;
}
double** transposeMatrix(double** A, int n) {//전치행렬 구하기
	double** trans = (double**)malloc(sizeof(double*) * n);
	for (int i = 0; i < n; i++) {
		*(trans + i) = (double*)malloc(sizeof(double) * n);
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			*(*(trans + i) + j) = *(*(A + j) + i);//trans에 i,j 서로 바꿔서 저장
		}
	}
	return trans;
}
double** inverseMatrix(double** A, double det, int n) {
	double** inv = cofactorMatrix(A, n);//inv에 우선 여인수 행렬 저장
	inv = transposeMatrix(inv, n);//여인수 행렬의 전치 행렬 구하기
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			*(*(inv + i) + j) /= det;//각각을 det 값으로 나눠 역행렬 구함
		}
	}
	return inv;//역행렬 반환
}
double** inverseCheckMatrix(double** A, double** B, int n) {//행렬과 역행렬 곱을 반환
	double** mul = (double**)malloc(sizeof(double*) * n);
	for (int i = 0; i < n; i++) {
		*(mul + i) = (double*)malloc(sizeof(double) * n);
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			*(*(mul + i) + j) = 0;
			for (int k = 0; k < n; k++) {
				*(*(mul + i) + j) += *(*(A + i) + k) * *(*(B + k) + j);//행렬 곱 식
			}
		}
	}
	return mul;//곱 반환
}
void printMatrix(double** A, int n) {//행렬 출력 함수
	double e = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (-0.01 < *(*(A + i) + j) && *(*(A + i) + j) < 0.01) {//0근처의 절댓값 0.01 범위 내의 수들 0으로 출력(-0 표기 오류 수정용)
				printf("%7.2lf", e);
			}
			else {//일반 출력
				printf("%7.2lf", *(*(A + i) + j));
			}
		}
		printf("\n");
	}
}
void checkPrint(double** C, int n) {//항등행렬인지 확인하고 맞으면 True, 아니면 False 출력
	int flag = 1;//항등행렬에서 벗어나는 것이 있으면 0으로 바꾸고 반복문 종료
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (i == j) {
				if (!(*(*(C + i) + j) < 1.01 && 0.99 < *(*(C + i) + j))) {
					flag = 0;
					break;
				}
			}
			else {
				if (!(*(*(C + i) + j) < 0.01 && -0.01 < *(*(C + i) + j))) {
					flag = 0;
					break;
				}
			}
		}
	}
	printf("Corectness of InverseMatrix\n");
	if (flag == 1) {
		printf("True\n");
	}
	else {
		printf("False\n");
	}
}

