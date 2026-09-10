# 테스트 러너가 공유하는 보고/실행 도우미. run_tests.sh 에서 source 합니다.

if [ -t 1 ]; then
	RED=$'\033[31m'; GREEN=$'\033[32m'; YELLOW=$'\033[33m'
	BOLD=$'\033[1m'; DIM=$'\033[2m'; OFF=$'\033[0m'
else
	RED=''; GREEN=''; YELLOW=''; BOLD=''; DIM=''; OFF=''
fi

PASSED=0
FAILED=0
FAILED_NAMES=()

section() {
	printf '\n%s== %s%s\n' "$BOLD" "$1" "$OFF"
}

pass() {
	PASSED=$((PASSED + 1))
	printf '  %s✓%s %s\n' "$GREEN" "$OFF" "$1"
}

# $1 라벨, $2 (선택) 들여쓰기해서 보여줄 상세 내용.
fail() {
	FAILED=$((FAILED + 1))
	FAILED_NAMES+=("$1")
	printf '  %s✗%s %s\n' "$RED" "$OFF" "$1"
	# 명령 치환이 끝의 개행을 지우므로 직접 붙여줍니다.
	if [ -n "${2:-}" ]; then
		printf '%s\n' "$2" | sed "s|^|      $DIM|; s|\$|$OFF|"
	fi
}

# 명령을 돌리고, 실패했을 때만 출력을 보여줍니다. VERBOSE=1 이면 항상.
run_logged() {
	local label="$1" log="$WORK/log"
	shift
	if "$@" >"$log" 2>&1; then
		[ "${VERBOSE:-0}" -eq 1 ] && sed "s|^|      $DIM|; s|\$|$OFF|" "$log"
		pass "$label"
		return 0
	fi
	fail "$label" "$(cat "$log")"
	return 1
}

# $1 라벨, $2 출력 바이너리, 그 뒤는 컴파일 인자.
compile_driver() {
	local label="$1" out="$2"
	shift 2
	if "$CXX" $CXXFLAGS -O2 -o "$out" "$@" >"$WORK/cc" 2>&1; then
		return 0
	fi
	fail "$label (드라이버 컴파일 실패)" "$(cat "$WORK/cc")"
	return 1
}

summary() {
	section "요약"
	if [ "$FAILED" -eq 0 ]; then
		printf '  %s%d개 통과, 실패 없음%s' "$GREEN" "$PASSED" "$OFF"
		[ "${QUICK:-0}" -eq 1 ] && printf ' %s(--quick: 무작위 대량 검증 생략)%s' \
			"$YELLOW" "$OFF"
		printf '\n'
		return 0
	fi
	printf '  %s%d개 통과, %d개 실패%s\n' "$RED" "$PASSED" "$FAILED" "$OFF"
	local name
	for name in "${FAILED_NAMES[@]}"; do
		printf '    - %s\n' "$name"
	done
	return 1
}

# ex00-ex02 는 & | ^ << >> = 와 비교 연산자만 쓸 수 있고, ++ 는 루프 인덱스에만
# 허용됩니다. 주석을 지운 뒤 산술 연산자를 찾고 ++ 를 쓰는 for 헤더는 제외하는
# 어림 검사입니다(같은 줄에 연산까지 적으면 놓칠 수 있습니다).
check_operators() {
	local ex="$1" file offenders found=0
	shift
	for file in "$@"; do
		[ -f "$file" ] || continue
		offenders="$(sed 's|//.*||' "$file" \
			| grep -n '[-+*/%]' \
			| grep -v 'for *(.*++' || true)"
		if [ -n "$offenders" ]; then
			fail "$ex: $(basename "$file") 에 금지된 산술 연산자" "$offenders"
			found=1
		fi
	done
	[ "$found" -eq 0 ] && pass "$ex: 비트/비교 연산자만 사용"
}
