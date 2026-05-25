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

## 빌드 / 실행

```sh
cd ex00
make          # 바이너리 빌드 (예: ./adder)
./adder       # 실행
make clean    # 오브젝트 파일 제거
make fclean   # 오브젝트 + 바이너리 제거
make re       # fclean 후 재빌드
```

## 컴파일 옵션 / 규칙

- C++17 (`-std=c++17`)
- `-Wall -Wextra -Werror`
- **수학 라이브러리 사용 금지** — `<cmath>` 등 표준 수학 함수를 호출하지 않고, 필요한 수학 연산은 모두 직접 구현
- STL 컨테이너(`std::vector`, `std::string`, `std::set` 등)는 허용
- 각 ex의 시간/공간 복잡도 제한은 해당 폴더의 헤더 파일 주석에 명시

## 공통 모듈에 대한 메모

ex03부터는 RPN(역폴란드 표기법) 기반 불 수식 파서를 공유합니다.
평가 시 각 ex가 독립적으로 컴파일되어야 하므로, 공통 코드(파서 / AST 노드 / 평가기 등)는 각 ex 폴더로 **복사**하여 사용합니다.
