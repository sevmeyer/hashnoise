#include <cstdint>
#include <exception>
#include <iostream>
#include <limits>
#include <string>
#include <stdexcept>
#include <vector>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write/stb_image_write.h"
#include "minarg/minarg.hpp"

#include "hashnoise/hashnoise.hpp"


int main(int argc, char* argv[])
{
	int size{512};
	int freq{32};
	std::uint32_t seed{0x332ff52d};
	std::string target{};

	minarg::Parser parser{"Generates a noise image with the hashnoise library."};

	parser.addSignal(      'h', "help",           "Print help and exit");
	parser.addOption(seed, 's', "seed", "UINT32", "Hash multiplier seed");
	parser.addOption(size, 'z', "size", "INT",    "Image width and height");
	parser.addOption(freq, 'f', "freq", "INT",    "Noise frequency");

	parser.addOperand(target, "PNG", "Target PNG image, overwrites existing", true);

	try
	{
		parser.parse(argc, argv);

		// Sanitize arguments
		if (size < 1 || size > std::numeric_limits<int>::max()/size)
			throw std::runtime_error{"Invalid size"};

		// Prepare noise
		hashnoise::Noise noise{seed};
		double period{1.0*freq / size};

		// Generate image
		std::vector<unsigned char> pixels(size*size);
		for (int y{0}; y < size; ++y)
		for (int x{0}; x < size; ++x)
			pixels[y*size + x] = static_cast<unsigned char>(
				noise.getNorm(x*period, y*period)*255.0 + 0.5);

		// Save image
		if (stbi_write_png(target.c_str(), size, size, 1, pixels.data(), 0) == 0)
			throw std::runtime_error{"Could not write image"};
	}
	catch (const minarg::Signal&)
	{
		std::cout << parser;
		return EXIT_SUCCESS;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
}
