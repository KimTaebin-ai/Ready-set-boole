// ex02 gray_code: subject 참조값 + 그레이 코드의 정의(인접 값은 1비트 차이).
#include <cstdint>
#include <cstdio>

uint32_t gray_code(uint32_t n);

static int failures = 0;

static void check(uint32_t n, uint32_t want) {
	uint32_t got = gray_code(n);

	if (got != want) {
		std::printf("gray_code(%u): got %u, want %u\n", n, got, want);
		++failures;
	}
}

static unsigned int popcount(uint32_t v) {
	unsigned int bits = 0;

	while (v != 0) {
		bits += v & 1u;
		v >>= 1;
	}
	return bits;
}

int main() {
	// subject 의 예시 전체.
	check(0, 0);
	check(1, 1);
	check(2, 3);
	check(3, 2);
	check(4, 6);
	check(5, 7);
	check(6, 5);
	check(7, 4);
	check(8, 12);

	// 정의: 연속한 두 값의 코드는 정확히 한 비트만 다릅니다.
	for (uint32_t n = 0; n < 1u << 20; ++n) {
		if (popcount(gray_code(n) ^ gray_code(n + 1)) != 1) {
			if (failures < 5)
				std::printf("gray_code(%u) and gray_code(%u) differ by "
					"more than one bit\n", n, n + 1);
			++failures;
		}
	}

	// 정의: 단사(injective) 이어야 합니다. 16비트 구간에서 확인.
	static bool seen[1 << 16] = {};

	for (uint32_t n = 0; n < 1u << 16; ++n) {
		uint32_t code = gray_code(n);

		if (code >= 1u << 16 || seen[code]) {
			if (failures < 5)
				std::printf("gray_code(%u) = %u is not a bijection over "
					"16 bits\n", n, code);
			++failures;
		} else
			seen[code] = true;
	}
	std::printf("subject values + 1M adjacency checks + 65536 bijection "
		"checks, %d mismatches\n", failures);
	return failures != 0;
}
