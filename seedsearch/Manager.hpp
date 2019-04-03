#pragma once

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <vector>


class Manager
{
	public:

		Manager(std::vector<std::uint32_t> jobs) :
			jobs_{std::move(jobs)}
		{}

		bool loadJob(std::uint32_t& job)
		{
			std::unique_lock<std::mutex> lock{mutex_};

			if (next_ >= jobs_.size())
				return false;

			job = jobs_[next_++];
			return true;
		}

		void submitResult(std::uint32_t job, std::size_t result)
		{
			std::unique_lock<std::mutex> lock{mutex_};
			std::cout <<
				std::hex << std::setw(8) << std::setfill('0') << job << ' ' <<
				std::dec << std::setw(6) << std::setfill(' ') << result << '\n';
		}

	private:

		std::mutex mutex_{};
		std::vector<std::uint32_t> jobs_{};
		std::size_t next_{0};
};
