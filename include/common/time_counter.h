
#pragma once

#include <chrono>
#include <string>
#include <memory>
#include <iostream>
#include <common/stl.h>

namespace util {

class time_counter_default_printer
{
	void print(const ::std::chrono::nanoseconds&) {}
};

class time_counter_stdout_printer
{
	using nanoseconds = ::std::chrono::nanoseconds;
	using rep = typename nanoseconds::rep;

public:
	void print(const nanoseconds& time)
	{
		rep ns = time.count();
		double elapsedSeconds = static_cast<double>(ns) / 1000000000.0;
		double elapsedMilliseconds = static_cast<double>(ns) / 1000.0;
		::std::cout << ::stl::string_format("Elapsed time in seconds: [%.3f] in milliseconds: [%.3f]",
			elapsedSeconds, elapsedMilliseconds).c_str() << ::std::endl;
	}
};

template <class TimeUnit, class Printer = time_counter_stdout_printer>
class time_counter : public Printer
{
public:
	using time_unit = TimeUnit;
	using time_point = ::std::chrono::time_point<::std::chrono::steady_clock>;
	using clock = ::std::chrono::high_resolution_clock;
	using duration = typename time_point::duration;
	using rep = typename time_unit::rep;

	time_counter()
		: m_startTime(clock::now())
		, m_stopTime(time_point())
	{}

	inline void start()
	{
		m_startTime = clock::now();
		m_stopTime = time_point();
	}

	inline time_unit stop()
	{
		m_stopTime = clock::now();
		time_unit elapsedTime = getElapsedTime();
		print(::std::chrono::duration_cast<::std::chrono::nanoseconds>(elapsedTime));
		return elapsedTime;
	}

	inline time_unit restart()
	{
		time_unit time = stop();
		start();
		return time;
	}

	inline time_unit getElapsedTime()
	{
		bool wasStopped = (m_stopTime != time_point());
		time_point stopTime = wasStopped ? m_stopTime : clock::now();
		return ::std::chrono::duration_cast<time_unit, rep>(stopTime - m_startTime);
	}

private:
	time_point m_startTime;
	time_point m_stopTime;
};

} // namespace util
