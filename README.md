# Ready, Set, Boole!

42 과제 — 불 대수, 집합론, 공간 채움 곡선 입문.
각 연습은 독립된 폴더에 들어있고, 폴더로 들어가 `make`로 바로 빌드/실행할 수 있습니다.

## Exercises

| ex   | 바이너리                    | 주제 |
|------|----------------------------|------|
| ex00 | `adder`                    | 비트 연산으로 덧셈 구현 |
| ex01 | `multiplier`               | 비트 연산으로 곱셈 구현 |
| ex02 | `gray_code`                | 그레이 코드 변환 |
| ex03 | `eval_formula`             | RPN 불 수식 평가 |
| ex04 | `print_truth_table`        | 진리표 출력 |
| ex05 | `negation_normal_form`     | NNF (부정 정규형) 변환 |
| ex06 | `conjunctive_normal_form`  | CNF (논리곱 정규형) 변환 |
| ex07 | `sat`                      | 충족 가능성(SAT) 판별 |
| ex08 | `powerset`                 | 멱집합 |
| ex09 | `eval_set`                 | RPN 집합 연산 평가 |
| ex10 | `map`                      | 2D → 1D 매핑 (공간 채움 곡선) |
| ex11 | `reverse_map`              | 1D → 2D 역매핑 |

ex00–ex08 구현 완료. ex09–ex11은 미구현(ex10/ex11은 subject상 필수 아님).

## 디렉터리 구조

```
common/          여러 ex가 공유하는 모듈 + 공통 빌드 규칙
  Makefile.inc     모든 ex가 include 하는 빌드 규칙
  ast.hpp/.cpp     RPN 파서, AST, RPN 직렬화, 평가기, 변수 수집
  nnf.hpp/.cpp     AST 단계의 NNF 재작성 (ex05, ex06 공유)
exNN/            각 연습. <함수명>.hpp/.cpp + main.cpp + Makefile
tests/           테스트 러너 (제출물 아님)
  lib.sh           보고/실행 도우미
  drivers/         ex별 검증 드라이버 + 독립 기준 평가기
run_tests.sh     전체 테스트 진입점
```

subject는 turn-in 디렉터리나 파일 목록을 규정하지 않습니다(Chapter XIX는
"Git 저장소에 제출, 파일명을 확인하라"가 전부). 강제되는 것은 각 ex가 명시된
함수 시그니처를 제공하고, 실행 가능한 `main`을 함께 제출하며, 복잡도 제한을
지키는 것입니다.

## 빌드 / 실행

```sh
cd ex00
make          # 바이너리 빌드 (예: ./adder)
./adder       # 실행
make clean    # 오브젝트 파일 제거
make fclean   # 오브젝트 + 바이너리 제거
make re       # fclean 후 재빌드
```

각 ex의 Makefile은 자기에게 특수한 것만 선언하고 나머지는 `common/Makefile.inc`가
처리합니다:

```make
NAME = negation_normal_form

SRCS = negation_normal_form.cpp main.cpp
SHARED = ../common/ast.cpp ../common/nnf.cpp

include ../common/Makefile.inc
```

`SHARED`에 적은 경로는 `vpath %.cpp`와 `-I`로 연결되므로, 폴더에 들어가
`make`만 해도 공유 모듈이 함께 빌드됩니다. 파일을 복사해 두지 않습니다.

## 테스트

```sh
./run_tests.sh              # 전체 (약 18초)
./run_tests.sh --quick      # 무작위 대량 검증 생략 (약 1초)
./run_tests.sh --verbose    # 빌드/실행 출력까지 표시
./run_tests.sh ex03 ex04    # 특정 ex 만
```

실패 시 exit 1입니다. 러너는 각 ex를 `-Werror`로 빌드하고, `main`이 정상
종료하는지 확인한 뒤, `tests/drivers/`의 드라이버로 subject 기준값을 단정하고,
마지막으로 ex00–ex02의 허용 연산자를 검사합니다.

드라이버는 각 ex의 구현 소스를 직접 링크하며 `main.cpp`의 출력을 스냅샷하지
않습니다. 따라서 `main`에 테스트를 추가해도 러너가 깨지지 않습니다.

ex03·ex05·ex06·ex07은 예시 출력만 맞추는 것으로 부족하므로, 무작위 수식을
생성해 **의미 동등성**(원식과 결과식의 진리표 전량 비교)과 **정규형 구조
조건**까지 확인합니다. 이때 정답 기준은 `tests/drivers/reference_eval.cpp` —
`common/ast`와 코드를 한 줄도 공유하지 않는 독립 스택 평가기입니다.

## 컴파일 옵션 / 규칙

- C++17 (`-std=c++17`)
- `-Wall -Wextra -Werror`
- **수학 라이브러리 사용 금지** — `<cmath>` 등 표준 수학 함수를 호출하지 않고, 필요한 수학 연산은 모두 직접 구현
- ex00–ex02는 비트 연산(`& | ^ << >>`)과 비교 연산자만 사용. `++`는 루프 인덱스에만 허용
- STL 컨테이너(`std::vector`, `std::string`, `std::set` 등)는 허용
- 각 ex의 시간/공간 복잡도 제한은 해당 폴더의 헤더 파일 주석에 명시
