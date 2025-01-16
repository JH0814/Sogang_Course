cc=g++ #컴파일러를 g++로 한다
target = main # 생성되는 실행파일을 main으로 한다

objects = main.o array.o rangearray.o # object 파일 그룹을 변수로 선언한다

$(target) : $(objects) # 변수로 선언한 target과 objects 변수들을 대상으로 한다
	$(cc) -o $(target) $(objects) # 변수들을 대상으로 원하는 명령어 실행한다.

$.o : $.cpp # 디렉터리 내에 해당 확장자 가진 모든 파일 대상으로 한다
	$(cc) -c -o $@ $< # 위에서 정한 확장자의 파일 대상으로 명령어 실행한다

main.o array.o rangearray.o : RangeArray.h Array.h #object파일에 묶어줄 헤더 파일 정해준다

.PHONY : clean # clean 명령어 추가한다
clean :
	rm $(target) $(objects) # clean 실행시 원하는 명령어
