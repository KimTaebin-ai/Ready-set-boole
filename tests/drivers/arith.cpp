// ex00 adder / ex01 multiplier: subject 예시값 + 내장 연산자와의 대조 검증.
#include <cstdint>
#include <cstdio>
#include <random>

uint32_t adder(uint32_t a, uint32_t b);
#ifdef TEST_MULTIPLIER
uint32_t multiplier(uint32_t a, uint32_t b);
#endif

static int failures = 0;

static void check(const char *what, uint32_t got, uint32_t want) {
	if (got != want) {
		std::printf("%s: got %u, want %u\n", what, got, want);
		++failures;
	}
}

int main() {
	check("adder(0, 0)", adder(0, 0), 0);
	check("adder(1, 2)", adder(1, 2), 3);
	check("adder(42, 58)", adder(42, 58), 100);
	check("adder(123, 456)", adder(123, 456), 579);
	// u32 는 2^32 로 되돌아갑니다(wraparound).
	check("adder(0xFFFFFFFF, 1)", adder(0xFFFFFFFFu, 1), 0);
	check("adder(0xFFFFFFFF, 0xFFFFFFFF)",
		adder(0xFFFFFFFFu, 0xFFFFFFFFu), 0xFFFFFFFEu);
#ifdef TEST_MULTIPLIER
	check("multiplier(0, 0)", multiplier(0, 0), 0);
	check("multiplier(1, 2)", multiplier(1, 2), 2);
	check("multiplier(6, 7)", multiplier(6, 7), 42);
	check("multiplier(42, 58)", multiplier(42, 58), 2436);
	check("multiplier(123, 456)", multiplier(123, 456), 56088);
	check("multiplier(0xFFFFFFFF, 2)", multiplier(0xFFFFFFFFu, 2),
		0xFFFFFFFEu);
#endif

	// 작은 값 전수 + 무작위 대량 비교. 구현과 무관한 내장 + / * 가 기준입니다.
	long long tested = 0;
	const uint32_t small = SMALL_BOUND;

	for (uint32_t a = 0; a < small; ++a) {
		for (uint32_t b = 0; b < small; ++b) {
			++tested;
			if (adder(a, b) != a + b) {
				if (failures < 5)
					std::printf("adder(%u, %u) != %u\n", a, b, a + b);
				++failures;
			}
#ifdef TEST_MULTIPLIER
			if (multiplier(a, b) != a * b) {
				if (failures < 5)
					std::printf("multiplier(%u, %u) != %u\n", a, b, a * b);
				++failures;
			}
#endif
		}
	}

	std::mt19937 rng(42);

	for (long long i = 0; i < RANDOM_ROUNDS; ++i) {
		uint32_t a = rng();
		uint32_t b = rng();

		++tested;
		if (adder(a, b) != static_cast<uint32_t>(a + b)) {
			if (failures < 5)
				std::printf("adder(%u, %u) != %u\n", a, b,
					static_cast<uint32_t>(a + b));
			++failures;
		}
#ifdef TEST_MULTIPLIER
		if (multiplier(a, b) != static_cast<uint32_t>(a * b)) {
			if (failures < 5)
				std::printf("multiplier(%u, %u) != %u\n", a, b,
					static_cast<uint32_t>(a * b));
			++failures;
		}
#endif
	}
	std::printf("%lld pairs vs. built-in operators, %d mismatches\n",
		tested, failures);
	return failures != 0;
}
