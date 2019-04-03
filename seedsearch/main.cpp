#include <cstdint>
#include <algorithm>
#include <functional>
#include <iostream>
#include <random>
#include <thread>
#include <vector>

#include "minarg/minarg.hpp"

#include "Manager.hpp"
#include "Worker.hpp"


int main(int argc, char* argv[])
{
	unsigned int prngInit{0u};
	unsigned int seedCount{10u};
	unsigned int maxThreads{8u};

	minarg::Parser parser{
		"This tool tests pseudo-random noise seeds for the hashnoise library.\n"
		"For each seed, it generates a noise image, compresses it, and prints\n"
		"the resulting size."};

	parser.addSignal(            'h', "help" ,           "Print help and exit");
	parser.addOption(prngInit,   'i', "init" ,   "UINT", "Value to initialize PRNG");
	parser.addOption(seedCount,  's', "seeds",   "UINT", "Number of hash seeds");
	parser.addOption(maxThreads, 't', "threads", "UINT", "Maximum number of threads");

	try
	{
		parser.parse(argc, argv);
	}
	catch (const minarg::Signal&)
	{
		std::cout << parser;
		return EXIT_SUCCESS;
	}
	catch (const std::exception& e)
	{
		std::cerr << "ERROR: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	// Determine thread count
	unsigned int availableThreads{std::thread::hardware_concurrency()};
	maxThreads = std::max(1u, std::min(maxThreads, availableThreads));

	// Prepare PRNG to generate seeds pseudo-randomly
	std::mt19937 engine{prngInit};
	std::uniform_int_distribution<std::uint32_t> distribution{};
	auto random = std::bind(distribution, engine);

	// Prepare hash seeds
	std::vector<std::uint32_t> seeds(seedCount);
	std::generate(seeds.begin(), seeds.end(), random);
	Manager manager{std::move(seeds)};

	// Run worker threads
	std::vector<std::thread> threads{};
	for(unsigned int i{0}; i < maxThreads; ++i)
		threads.emplace_back(Worker{manager});

	// Wait for threads to finish
	for (auto& thread : threads)
		thread.join();
}
