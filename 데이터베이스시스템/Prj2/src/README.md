## 테스트 환경
* **OS** : Windows 11
* **DBMS** : MySQL Server 8.0.46 / MySQL Workbench 8.0.47
* **Compiler** : MSVC
* **Connectivity** : **MySQL Native C API**
* **IDE** : vscode

## 환경 세팅 (실행 전 필수 확인)
1. **데이터베이스 초기화**
   * MySQL Workbench를 열고 `database/schema.sql` 파일을 불러와 전체 실행합니다. (테이블 및 무결성 제약조건 생성)
   * 이어서 `database/sample_data.sql` 파일을 불러와 전체 실행합니다. (시나리오 테스트용 데이터 105개 삽입)
2. **DLL 파일 배치**
   * MySQL C API를 사용하므로, 컴파일 후 실행 시 `libmysql.dll` 파일이 `main.exe`와 동일한 폴더에 위치해야 합니다. (MySQL 설치 경로의 `lib` 폴더 내에 존재합니다.)
   * 환경변수 : set PATH=%PATH%;C:\Program Files\MySQL\MySQL Server 8.0\bin 필요
3. **Compiler**
   * call "vcvars64.bat 위치"

## 컴파일 명령어
```
cl /EHsc src/main.cpp /I"C:\Program Files\MySQL\MySQL Server 8.0\include" /link /LIBPATH:"C:\Program Files\MySQL\MySQL Server 8.0\lib" libmysql.lib
```

## 실행
```
main.exe
```

## 사용 방법
[1] Sales Trends: 1 입력 후 Enter (최근 3년간의 판매 트렌드를 브랜드, 연월, 고객 성별/소득별로 출력)

[2] Defective Part Tracking: 2 입력 후 Enter Supplier, 두 날짜 구간 입력 (결함부품 장착 차량 및 고객 추적)

[3] Top 2 Brands by Revenue: 3 입력 후 Enter (최근 1년 기준 매출 1, 2위 브랜드 출력)

[4] Top 2 Brands by Unit Sales: 4 입력 후 Enter (최근 1년 기준 판매 대수 1, 2위 브랜드 출력)

[5] Seasonal Sales Patterns: 5 입력 후 Enter style 입력 (입력된 스타일 차량이 가장 많이 팔린 계절/월 분석)

[6] Dealer Inventory Efficiency: 6 입력 후 Enter (현재 재고 차량을 가장 오래 방치하고 있는 재고 회전율 최하위 딜러 출력)

[7] Supplier Coverage Analysis: 7 입력 후 Enter (가장 많은 종류의 차량 모델에 부품을 공급하는 1위 공급사 출력)

[8] Exit: 8 입력 후 Enter (프로그램 종료)
