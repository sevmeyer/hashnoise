#pragma once

#include <cmath>
#include <cstdint>
#include <array>
#include <zlib.h>

#include "hashnoise/hashnoise.hpp"

#include "Manager.hpp"


class Worker
{
	public:

		Worker(Manager& manager) :
			manager_{manager},
			zbuffer_(compressBound(area_))
		{}

		void operator()()
		{
			std::uint32_t seed;
			while (manager_.loadJob(seed))
			{
				fill(seed);

				std::size_t upright{compress(uprightPixels_)};
				std::size_t rotated{compress(rotatedPixels_)};
				std::size_t minimum{upright < rotated ? upright : rotated};

				manager_.submitResult(seed, minimum);
			}
		}

	private:

		Manager& manager_;

		static constexpr double period{0.25};
		static constexpr std::size_t size_{512};
		static constexpr std::size_t area_{size_*size_};

		std::array<std::uint8_t, area_> uprightPixels_;
		std::array<std::uint8_t, area_> rotatedPixels_;
		std::vector<std::uint8_t> zbuffer_;

		void fill(std::uint32_t seed)
		{
			hashnoise::Noise noise{seed};

			for (std::size_t i{0}; i < area_; ++i)
			{
				std::size_t x{i % size_};
				std::size_t y{i / size_};
				std::uint8_t color{static_cast<std::uint8_t>(
					noise.getNorm(x*period, y*period)*255.0 + 0.5)};

				uprightPixels_[i] = color;
				rotatedPixels_[x*size_ + size_-1-y] = color;
			}
		}

		std::size_t compress(const std::array<std::uint8_t, area_>& pixels)
		{
			// Using zlib, should be thread-safe
			constexpr int level{5}; // 1(fast) - 9(best)

			uLong size{zbuffer_.size()};
			int status{compress2(
				zbuffer_.data(), &size,
				pixels.data(), pixels.size(),
				level)};

			return status == Z_OK ? size : 0;
		}
};
