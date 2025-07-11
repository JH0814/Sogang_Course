
double detA(double** A, int n);//행렬식 구하기
double cofactor(double** mat, int n, int i, int j);//추가, 여인수행렬에서 각각의 원소 구하는 용도
double** M_Mat(double** A, int n, int row, int col);//추가, 여인수행렬에서 M(i,j) 구하는 용도
double** turn_two(double* A, int n);//추가, 일차원 배열을 이차원 배열로 바꾸는 용도
double** cofactorMatrix(double** A, int n);//여인수 행렬 반환
double** transposeMatrix(double** A, int n);//전치행렬 반환
double** inverseMatrix(double** A, double det, int n);//역행렬 구하기
double** inverseCheckMatrix(double** A, double** B, int n);//행렬과 역행렬 곱 반환
void printMatrix(double** A, int n);//행렬 출력
void checkPrint(double** C, int n);//추가, inverseCheckMatrix에서 계산한 행렬이 항등행렬인지 확인
