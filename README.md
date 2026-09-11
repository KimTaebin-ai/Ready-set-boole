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
cd ex05
make          # 바이너리 빌드 (여기서는 ./negation_normal_form)
./negation_normal_form
make clean    # 오브젝트 파일(obj/) 제거
make fclean   # 오브젝트 + 바이너리 제거
make re       # fclean 후 재빌드
```

바이너리 이름은 위 Exercises 표와 같고, 해당 ex 폴더 안에 생성됩니다. 각
Makefile의 `NAME`이 그 이름이므로 확인이 필요하면 다음으로 볼 수 있습니다:

```sh
grep '^NAME' ex*/Makefile
```

각 Makefile은 자기에게 특수한 것만 선언하고 빌드 규칙은 전부
`common/Makefile.inc`가 처리합니다:

```make
NAME = negation_normal_form

SRCS = negation_normal_form.cpp main.cpp
SHARED = ../common/ast.cpp ../common/nnf.cpp

include ../common/Makefile.inc
```

`SHARED`에 적은 경로는 `vpath %.cpp`와 `-I`로 연결되므로, 폴더에 들어가
`make`만 해도 공유 모듈이 함께 빌드됩니다. 파일을 복사해 두지 않습니다.

> **주의** — 이 구조에서 ex 폴더는 단독으로 들고 나갈 수 없습니다.
> `common/Makefile.inc`(그리고 `SHARED`에 적힌 소스)가 상대 경로로 필요하므로,
> 폴더 하나만 복사하면 `No rule to make target '../common/Makefile.inc'`로
> 실패합니다. 저장소 전체를 함께 두면 됩니다 — subject도 저장소 단위로
> 평가하므로 문제되지 않습니다.

## 테스트

저장소 루트에서 실행합니다.

```sh
./run_tests.sh              # 전체
./run_tests.sh --quick      # 무작위 대량 검증을 줄여서 실행
./run_tests.sh --verbose    # 각 검사의 상세 출력까지 표시
./run_tests.sh ex03 ex06    # 특정 ex 만 (ex00-ex11)
./run_tests.sh --help       # 사용법
```

러너는 다음 순서로 검사하고, 하나라도 실패하면 exit 1 과 함께 실패 목록을
요약합니다. 통과한 검사는 한 줄로만 보고하고, 실패한 검사만 상세 출력을
펼칩니다(`--verbose`를 주면 전부 펼침).

1. 각 ex 를 `make re` 로 `-Werror` 빌드
2. 각 ex 의 `main` 실행 — 정상 종료(exit 0) 확인
3. `tests/drivers/` 의 드라이버로 subject 기준값 단정
4. ex00–ex02 의 허용 연산자 검사(금지된 산술 연산자 사용 여부)

두 가지 알아둘 점:

- 러너는 **끝에 `make fclean` 을 돌려 빌드 산물을 지웁니다.** 테스트 후
  바이너리가 사라진 것처럼 보이는 게 정상이고, 다시 쓰려면 해당 ex 폴더에서
  `make` 하면 됩니다.
- 실행 시간은 **대부분 컴파일**입니다(ex 12개 `make re` + 드라이버 12개를
  `-O2` 로 컴파일). 그래서 `--quick` 은 무작위 검증량만 줄이므로 체감상
  3분의 1 정도만 짧아집니다 — 큰 차이를 기대할 옵션은 아닙니다.

드라이버는 각 ex의 구현 소스를 직접 링크하며 `main.cpp`의 출력을 스냅샷하지
않습니다. 따라서 `main`에 테스트를 추가해도 러너가 깨지지 않습니다.

### 예시값만으로 부족한 경우

subject 의 예시를 맞추는 것은 약한 검증이라, 다음은 성질까지 확인합니다.
무작위 입력을 만들어 **검증 대상과 코드를 공유하지 않는 별도 기준 구현**과
대조하는 방식입니다.

- **ex00·ex01** — 내장 `+`, `*` 와 대조
- **ex02** — 그레이 코드의 정의(인접 값이 정확히 1비트 차이, 전단사)
- **ex03** — `tests/drivers/reference_eval.cpp` 와 대조. 이 파일은 리팩토링
  전 ex03 의 스택 기반 평가기로, `common/ast` 와 한 줄도 공유하지 않습니다
- **ex05·ex06** — **의미 동등성**(원식과 결과식의 진리표 전량 비교)과
  **정규형 구조 조건**(NNF: `!` 이 변수 바로 뒤에만, CNF: `|` 안에 `&` 없음)
- **ex07** — 위 독립 평가기로 모든 대입을 훑은 기준값과 대조
- **ex08** — 멱집합의 정의(크기 2ⁿ, 전부 서로 다름, 전부 부분집합, 각 원소가
  2ⁿ⁻¹ 개에 등장)
- **ex09** — 집합을 스택에 쌓아 집합 연산으로 직접 계산하는 기준 구현과
  대조. ex09 본체는 전체집합의 원소를 하나씩 판정하므로 알고리즘이 다릅니다
- **ex10·ex11** — 서로의 역함수라 함께 봐야 의미가 있어 같은 드라이버를
  씁니다. subject 가 "전단사이면 방법은 자유"라고 하므로 특정 값이 아니라
  성질을 봅니다 — 결과가 `[0, 1]` 안에 있고 서로 다른지, 그리고
  `reverse_map(map(x, y)) == (x, y)` 와 `map(reverse_map(n)) == n` 인지

## 컴파일 옵션 / 규칙

- C++17 (`-std=c++17`)
- `-Wall -Wextra -Werror`
- **수학 라이브러리 사용 금지** — `<cmath>` 등 표준 수학 함수를 호출하지 않고, 필요한 수학 연산은 모두 직접 구현
- ex00–ex02는 비트 연산(`& | ^ << >>`)과 비교 연산자만 사용. `++`는 루프 인덱스에만 허용
- STL 컨테이너(`std::vector`, `std::string`, `std::set` 등)는 허용
- 각 ex의 시간/공간 복잡도 제한은 해당 폴더의 헤더 파일 주석에 명시
