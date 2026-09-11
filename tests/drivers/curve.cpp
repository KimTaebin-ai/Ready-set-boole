// ex10 map / ex11 reverse_map: 전단사성과 양방향 왕복을 검사합니다.
//
// subject 는 f 가 전단사이기만 하면 방법은 자유라고 하므로, 특정 값이 아니라
// 성질을 검사합니다.
//   - map 의 결과는 [0, 1] 안에 있고 서로 달라야 합니다 (단사)
//   - (f^-1 . f)(x, y) == (x, y)
//   - (f . f^-1)(n) == n
#include "map.hpp"
#include "reverse_map.hpp"

#include <cstdio>
#include <random>
#include <set>
#include <stdexcept>

static int failures = 0;

// map 이 낼 수 있는 가장 큰 값, (2^32 - 1) / 2^32.
static const double index_count = 4294967296.0;
static const double largest = (index_count - 1.0) / index_count;

static void check_point(uint16_t x, uint16_t y)
{
	double value = map(x, y);

	if (!(value >= 0.0 && value <= 1.0))
	{
		std::printf("map(%u, %u) = %.17g is outside [0, 1]\n", x, y, value);
		++failures;
	}

	std::pair<uint16_t, uint16_t> back = reverse_map(value);

	if (back.first != x || back.second != y)
	{
		std::printf("reverse_map(map(%u, %u)) = (%u, %u)\n", x, y,
			back.first, back.second);
		++failures;
	}
}

static void check_throws(double n)
{
	try
	{
		std::pair<uint16_t, uint16_t> point = reverse_map(n);

		std::printf("reverse_map(%.17g) = (%u, %u), want a throw\n", n,
			point.first, point.second);
		++failures;
	}
	catch (const std::out_of_range&)
	{
	}
}

int main()
{
	// 경계값.
	check_point(0, 0);
	check_point(1, 0);
	check_point(0, 1);
	check_point(65535, 65535);
	check_point(65535, 0);
	check_point(0, 65535);

	// 두 좌표가 뒤바뀌면 안 됩니다. 그러면 쌍이 한 값으로 뭉개집니다.
	if (map(1, 0) == map(0, 1))
	{
		std::printf("map(1, 0) and map(0, 1) collide\n");
		++failures;
	}

	// 256x256 구석을 전수 검사: 65536 개 좌표가 65536 개의 서로 다른 값을
	// 내고, 모두 원래 좌표로 되돌아와야 합니다.
	std::set<double> values;

	for (uint32_t x = 0; x < 256; ++x)
	{
		for (uint32_t y = 0; y < 256; ++y)
		{
			uint16_t narrow_x = static_cast<uint16_t>(x);
			uint16_t narrow_y = static_cast<uint16_t>(y);

			values.insert(map(narrow_x, narrow_y));
			check_point(narrow_x, narrow_y);
		}
	}
	if (values.size() != 256 * 256)
	{
		std::printf("256x256 grid gave %zu distinct values, want 65536\n",
			values.size());
		++failures;
	}

	// u16 전 범위에서 무작위로 뽑아 왕복과 단사성을 확인합니다.
	std::mt19937 rng(20260910);
	std::set<double> sampled;
	int collisions = 0;

	for (int i = 0; i < RANDOM_ROUNDS; ++i)
	{
		uint16_t x = static_cast<uint16_t>(rng() & 0xFFFF);
		uint16_t y = static_cast<uint16_t>(rng() & 0xFFFF);

		check_point(x, y);
		if (!sampled.insert(map(x, y)).second)
		{
			// 같은 좌표를 두 번 뽑았을 수 있으므로, 되돌린 좌표가 다르면
			// 그때만 충돌입니다.
			std::pair<uint16_t, uint16_t> back = reverse_map(map(x, y));

			if (back.first != x || back.second != y)
				++collisions;
		}
	}
	if (collisions != 0)
	{
		std::printf("%d distinct coordinates shared a value\n", collisions);
		++failures;
	}

	// (f . f^-1)(n) == n : map 이 실제로 내는 값들에 대해.
	size_t value_mismatches = 0;

	for (uint32_t i = 0; i < 65536; ++i)
	{
		double n = static_cast<double>(i) / index_count;
		std::pair<uint16_t, uint16_t> point = reverse_map(n);

		if (map(point.first, point.second) != n)
			++value_mismatches;
	}
	if (value_mismatches != 0)
	{
		std::printf("map(reverse_map(n)) != n for %zu values\n",
			value_mismatches);
		++failures;
	}

	// 정의역 끝은 받아들이고, 그 밖은 거부해야 합니다.
	std::pair<uint16_t, uint16_t> top = reverse_map(largest);

	if (top.first != 65535 || top.second != 65535)
	{
		std::printf("reverse_map(largest) = (%u, %u), want (65535, 65535)\n",
			top.first, top.second);
		++failures;
	}
	// 1.0 은 [0, 1] 안이지만 map 이 내지 않는 값이라 대응하는 쌍이 없습니다.
	check_throws(1.0);
	check_throws(-0.5);
	check_throws(2.0);
	check_throws(1.5);

	std::printf("65536 grid points + %d random coordinates + 65536 values, "
		"%d mismatches\n", RANDOM_ROUNDS, failures);
	return failures != 0;
}
