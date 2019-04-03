// hashnoise 1.0.0
// https://github.com/sevmeyer/hashnoise
//
// A custom 2D noise implementation for C++, based on the reference
// implementation by Ken Perlin (https://mrl.nyu.edu/~perlin/noise/).
//
// Instead of a permutation table, the gradient vectors are derived
// from a hash function, based on an algorithm by Thomas Mueller
// (https://stackoverflow.com/a/12996028).


#ifndef HASHNOISE_HASHNOISE_HPP_INCLUDED
#define HASHNOISE_HASHNOISE_HPP_INCLUDED


#include <cstdint>
#include <cmath>


namespace hashnoise {


class Noise
{
	public:

		using Uint = std::uint32_t;

		// Constructor
		Noise(Uint seed = 0x332ff52d) : seed_{seed} {}

		// Returns value in [-M_SQRT1_2, M_SQRT1_2]
		// for positive coordinates
		double get(double x, double y) const
		{
			// Find coordinates of integer cell
			const Uint xFloor{static_cast<Uint>(x)};
			const Uint yFloor{static_cast<Uint>(y)};

			// Find relative position within integer cell
			const double xFrac{x - xFloor};
			const double yFrac{y - yFloor};

			// Compute gradient value for each cell corner
			const double grad00{grad(xFloor,   yFloor,   xFrac,     yFrac    )};
			const double grad10{grad(xFloor+1, yFloor,   xFrac-1.0, yFrac    )};
			const double grad01{grad(xFloor,   yFloor+1, xFrac,     yFrac-1.0)};
			const double grad11{grad(xFloor+1, yFloor+1, xFrac-1.0, yFrac-1.0)};

			// Compute smooth transition between cell edges
			const double xFade{fade(xFrac)};
			const double yFade{fade(yFrac)};

			// Interpolate gradient values
			return lerp(lerp(grad00, grad10, xFade),
			            lerp(grad01, grad11, xFade), yFade);
		}

		// Returns value in [0.0, 1.0]
		// for positive coordinates
		double getNorm(double x, double y) const
		{
			return 0.5 + get(x, y)*M_SQRT1_2;
		}

	private:

		const Uint seed_;

		double grad(Uint xCorner, Uint yCorner, double xDist, double yDist) const
		{
			constexpr double cosPi4{std::cos(M_PI / 4.0)};
			constexpr double cosPi8{std::cos(M_PI / 8.0)};
			constexpr double sinPi8{std::sin(M_PI / 8.0)};

			// Select a gradient unit-vector and return
			// its dot product with the distance vector
			switch(hash(xCorner, yCorner) & 0xf)
			{
				// 90 degrees
				case 0x0: return +xDist;
				case 0x1: return +yDist;
				case 0x2: return -xDist;
				case 0x3: return -yDist;
				// 45 degrees
				case 0x4: return +xDist*cosPi4 +yDist*cosPi4;
				case 0x5: return -xDist*cosPi4 +yDist*cosPi4;
				case 0x6: return -xDist*cosPi4 -yDist*cosPi4;
				case 0x7: return +xDist*cosPi4 -yDist*cosPi4;
				// 22.5 degrees
				case 0x8: return +xDist*cosPi8 +yDist*sinPi8;
				case 0x9: return +xDist*sinPi8 +yDist*cosPi8;
				case 0xa: return -xDist*sinPi8 +yDist*cosPi8;
				case 0xb: return -xDist*cosPi8 +yDist*sinPi8;
				case 0xc: return -xDist*cosPi8 -yDist*sinPi8;
				case 0xd: return -xDist*sinPi8 -yDist*cosPi8;
				case 0xe: return +xDist*sinPi8 -yDist*cosPi8;
				case 0xf: return +xDist*cosPi8 -yDist*sinPi8;
				default: return 0.0;
			}
		}

		Uint hash(Uint x, Uint y) const
		{
			// Combine lower halves of coordinates
			x = (x << 16) | (y & 0xffff);

			// Based on algorithm by Thomas Mueller
			x ^= x >> 16;
			x *= seed_;
			x ^= x >> 16;
			x *= seed_;
			x ^= x >> 16;
			return x;
		}

		double fade(double a) const
		{
			return a*a*a*(a*(a*6.0 - 15.0) + 10.0);
		}

		double lerp(double a, double b, double weight) const
		{
			return (1.0 - weight)*a + weight*b;
		}
};


} // namespace hashnoise

#endif // HASHNOISE_HASHNOISE_HPP_INCLUDED
