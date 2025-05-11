#include "Instrumentalization.h"
#include <iostream>


namespace Instrumentalization
{
	void FPSCounter::start(const std::string& name)
	{
		m_name = name;
		m_started = true;
	}
	void FPSCounter::update(float timeStep)
	{
		if (!m_started) return;
		m_counter += timeStep;
		m_frames++;
		if (m_counter >= 1.0f)
		{
			std::cout << "INSTR LOG: " << "FPS Counter [" << m_name << "] FPS: " << m_frames << "\n";
			m_totalFrames += m_frames;
			m_totalSeconds++;
			m_counter = 0.0f;
			m_frames = 0;
		}
	}
	FPSCounter::~FPSCounter()
	{
		if (m_totalSeconds > 0)
		{
			std::cout << "INSTR LOG: " << "FPS Counter results: --------------------------------------------" << "\n";
			std::cout << "INSTR LOG: " << "Name: [" << m_name << "]" << "\n";
			std::cout << "INSTR LOG: " << "Total frames: " << m_totalFrames << "\n";
			std::cout << "INSTR LOG: " << "Average FPS: " << (float)m_totalFrames / m_totalSeconds << "\n";
			std::cout << "INSTR LOG: " << "-----------------------------------------------------------------" << "\n";
		}
	}


	ScopeTimer::ScopeTimer(const std::string& name)
		: m_name(name), m_startTime(std::chrono::high_resolution_clock::now())
	{
	}
	ScopeTimer::~ScopeTimer()
	{
		auto endTime = std::chrono::high_resolution_clock::now();
		auto durationMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(endTime - m_startTime).count();
		auto durationMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - m_startTime).count();
		auto durationSeconds = std::chrono::duration_cast<std::chrono::seconds>(endTime - m_startTime).count();

		std::cout << "INSTR LOG: " << "Scope Timer Results: ------------------------------------------------" << "\n";
		std::cout << "INSTR LOG: " << "Name: [" << m_name << "]" << "\n";
		std::cout << "INSTR LOG: " << "Elapsed time: "
			<< durationMicroseconds << " microseconds /  "
			<< durationMilliseconds << " milliseconds / "
			<< durationSeconds << " seconds." << "\n";
	}


	void EventTimer::start(float delaySeconds, std::function<void()> callback)
	{
		m_countDown = delaySeconds;
		m_callback = callback;
		m_onCountDown = true;
	}

	void EventTimer::update(float timeStep)
	{
		if (!m_onCountDown) return;
		m_countDown -= timeStep;
		if (m_countDown <= 0.0f)
		{
			m_onCountDown = false;
			m_callback();
		}
	}
}