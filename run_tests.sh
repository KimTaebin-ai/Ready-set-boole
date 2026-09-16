#!/usr/bin/env bash
#
# Ready, Set, Boole! — 전체 테스트 러너
#
# 각 ex 에 대해 네 가지를 확인합니다.
#   1. -Wall -Wextra -Werror 로 경고 없이 빌드되는지
#   2. 각 ex 의 main 이 정상 종료(exit 0)하는지
#   3. subject 의 기준값을 만족하는지 — tests/drivers/ 의 드라이버가 각 ex 의
#      구현 소스를 직접 링크해 단정합니다. main.cpp 출력을 스냅샷하지 않으므로
#      main 에 테스트를 추가해도 이 스크립트는 깨지지 않습니다.
#   4. ex00-ex02 가 금지된 산술 연산자를 쓰지 않는지
#
# ex03/ex05/ex06/ex07 은 예시 출력만 맞추는 것으로 부족하므로, 무작위 수식을
# 생성해 의미 동등성과 정규형 구조 조건까지 확인합니다. 정답 기준은
# tests/drivers/reference_eval.cpp — common/ast 와 코드를 공유하지 않는
# 독립 스택 평가기입니다.
#
# 사용법:
#   ./run_tests.sh              전체 실행
#   ./run_tests.sh --quick      무작위 대량 검증 생략
#   ./run_tests.sh --verbose    빌드/실행 출력까지 표시
#   ./run_tests.sh ex03 ex04    특정 ex 만 실행

set -uo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
DRIVERS="$ROOT/tests/drivers"
CXX="${CXX:-clang++}"
CXXFLAGS="-Wall -Wextra -Werror -std=c++17"

QUICK=0
VERBOSE=0
SELECTED=()

for arg in "$@"; do
	case "$arg" in
		--quick)      QUICK=1 ;;
		--verbose|-v) VERBOSE=1 ;;
		ex0[0-9]|ex1[01]) SELECTED+=("$arg") ;;
		-h|--help)
			# BSD sed 는 BRE 에서 \? 를 지원하지 않으므로 -E 로 씁니다.
			sed -n '3,26p' "${BASH_SOURCE[0]}" | sed -E 's|^# ?||'
			exit 0 ;;
		*)
			echo "unknown argument: $arg (try --help)" >&2
			exit 2 ;;
	esac
done

[ ${#SELECTED[@]} -eq 0 ] && SELECTED=(ex00 ex01 ex02 ex03 ex04 ex05 ex06 ex07 ex08
	ex09 ex10 ex11)

WORK="$(mktemp -d)"
trap 'rm -rf "$WORK"' EXIT

source "$ROOT/tests/lib.sh"

if [ "$QUICK" -eq 1 ]; then
	SMALL_BOUND=64; RANDOM_ROUNDS=0; FORMULA_ROUNDS=50; CURVE_ROUNDS=2000
	GRID_SIDE=64; VALUE_COUNT=4096
else
	SMALL_BOUND=400; RANDOM_ROUNDS=2000000; FORMULA_ROUNDS=3000
	CURVE_ROUNDS=200000; GRID_SIDE=256; VALUE_COUNT=65536
fi

# 각 ex 의 드라이버 컴파일 인자와 통과 라벨. 배열로 넘겨 경로에 공백이
# 있어도 안전하게 다룹니다.
set_driver() {
	local common=(-I"$DRIVERS" -I"$ROOT/common" "$DRIVERS/reference_eval.cpp")
	local rounds=(-DRANDOM_ROUNDS=$FORMULA_ROUNDS)

	DRIVER_LABEL=""
	DRIVER_ARGS=()
	case "$1" in
	ex00)
		DRIVER_LABEL="adder 가 subject 값 + 내장 + 와 일치"
		DRIVER_ARGS=(-DSMALL_BOUND=$SMALL_BOUND -DRANDOM_ROUNDS=$RANDOM_ROUNDS
			"$DRIVERS/arith.cpp" "$ROOT/ex00/adder.cpp") ;;
	ex01)
		DRIVER_LABEL="multiplier 가 subject 값 + 내장 * 와 일치"
		DRIVER_ARGS=(-DTEST_MULTIPLIER -DSMALL_BOUND=$SMALL_BOUND
			-DRANDOM_ROUNDS=$RANDOM_ROUNDS -I"$ROOT/ex00" "$DRIVERS/arith.cpp"
			"$ROOT/ex00/adder.cpp" "$ROOT/ex01/multiplier.cpp") ;;
	ex02)
		DRIVER_LABEL="gray_code 가 subject 값 + 그레이 코드 정의를 만족"
		DRIVER_ARGS=("$DRIVERS/gray.cpp" "$ROOT/ex02/gray_code.cpp") ;;
	ex03)
		DRIVER_LABEL="eval_formula 가 subject 예시 + 독립 평가기와 일치"
		DRIVER_ARGS=("${common[@]}" "${rounds[@]}" -I"$ROOT/ex03"
			"$DRIVERS/eval.cpp" "$ROOT/ex03/eval_formula.cpp"
			"$ROOT/common/ast.cpp") ;;
	ex04)
		DRIVER_LABEL="진리표가 subject 출력과 바이트 단위로 일치"
		DRIVER_ARGS=(-I"$ROOT/ex04" -I"$ROOT/common" "$DRIVERS/table.cpp"
			"$ROOT/ex04/print_truth_table.cpp" "$ROOT/common/ast.cpp") ;;
	ex05)
		DRIVER_LABEL="NNF 이 subject 예시 + 구조 조건 + 의미 동등성을 만족"
		DRIVER_ARGS=("${common[@]}" "${rounds[@]}" -I"$ROOT/ex05"
			"$DRIVERS/normal_form.cpp" "$ROOT/ex05/negation_normal_form.cpp"
			"$ROOT/common/ast.cpp" "$ROOT/common/nnf.cpp") ;;
	ex06)
		DRIVER_LABEL="CNF 이 subject 예시 + 구조 조건 + 의미 동등성을 만족"
		DRIVER_ARGS=("${common[@]}" "${rounds[@]}" -DTEST_CNF -I"$ROOT/ex06"
			"$DRIVERS/normal_form.cpp" "$ROOT/ex06/conjunctive_normal_form.cpp"
			"$ROOT/common/ast.cpp" "$ROOT/common/nnf.cpp") ;;
	ex07)
		DRIVER_LABEL="sat 이 subject 예시 + 독립 기준값과 일치"
		DRIVER_ARGS=("${common[@]}" "${rounds[@]}" -I"$ROOT/ex07"
			"$DRIVERS/sat.cpp" "$ROOT/ex07/sat.cpp" "$ROOT/common/ast.cpp") ;;
	ex08)
		DRIVER_LABEL="멱집합이 subject 예시 + 정의를 만족"
		DRIVER_ARGS=(-I"$ROOT/ex08" "$DRIVERS/powerset.cpp"
			"$ROOT/ex08/powerset.cpp") ;;
	ex09)
		DRIVER_LABEL="eval_set 이 subject 예시 + 독립 집합대수 기준과 일치"
		DRIVER_ARGS=("${common[@]}" "${rounds[@]}" -I"$ROOT/ex09"
			"$DRIVERS/set_eval.cpp" "$ROOT/ex09/eval_set.cpp"
			"$ROOT/common/ast.cpp") ;;
	# map 과 reverse_map 은 서로의 역함수라 함께 봐야 의미가 있으므로 같은
	# 드라이버를 양쪽에 배정합니다.
	ex10|ex11)
		DRIVER_LABEL="map/reverse_map 이 전단사이고 양방향 왕복이 정확"
		DRIVER_ARGS=(-DRANDOM_ROUNDS=$CURVE_ROUNDS -DGRID_SIDE=$GRID_SIDE
			-DVALUE_COUNT=$VALUE_COUNT -I"$ROOT/ex10"
			-I"$ROOT/ex11" "$DRIVERS/curve.cpp" "$ROOT/ex10/map.cpp"
			"$ROOT/ex11/reverse_map.cpp") ;;
	esac
}

section "빌드 ($CXXFLAGS)"

BUILT=()
for ex in "${SELECTED[@]}"; do
	if run_logged "$ex: 경고 없이 빌드" make -C "$ROOT/$ex" re; then
		BUILT+=("$ex")
	fi
done

section "각 ex 의 main 실행"

for ex in "${BUILT[@]}"; do
	binary="$(awk -F' *= *' '/^NAME/ { print $2; exit }' "$ROOT/$ex/Makefile")"
	if [ -x "$ROOT/$ex/$binary" ]; then
		run_logged "$ex: ./$binary 정상 종료" "$ROOT/$ex/$binary"
	else
		fail "$ex: 바이너리를 찾을 수 없음 ($binary)"
	fi
done

section "subject 기준값 검증"

for ex in "${BUILT[@]}"; do
	set_driver "$ex"
	[ ${#DRIVER_ARGS[@]} -eq 0 ] && continue
	if compile_driver "$ex: $DRIVER_LABEL" "$WORK/$ex" "${DRIVER_ARGS[@]}"; then
		run_logged "$ex: $DRIVER_LABEL" "$WORK/$ex"
	fi
done

section "허용 연산자 검사 (ex00-ex02)"

in_selection() {
	local ex
	for ex in "${BUILT[@]}"; do
		[ "$ex" = "$1" ] && return 0
	done
	return 1
}

in_selection ex00 && check_operators ex00 "$ROOT/ex00/adder.cpp"
in_selection ex01 && check_operators ex01 "$ROOT/ex01/multiplier.cpp"
in_selection ex02 && check_operators ex02 "$ROOT/ex02/gray_code.cpp"

for ex in "${BUILT[@]}"; do
	make -C "$ROOT/$ex" fclean >/dev/null 2>&1
done

summary
