#pragma once
#include <chrono>
#include <string>
#include <functional>

namespace Instrumentalization
{
	class FPSCounter
	{
	public:
		void start(const std::string& name = "Unnamed");

		void update(float timeStep);
		~FPSCounter();
	private:
		std::string m_name = "Unnamed";
		bool m_started = false;
		float m_counter = 0.0f;
		int m_frames = 0;
		int m_totalSeconds = 0;
		int m_totalFrames = 0;
	};

	class ScopeTimer
	{
	public:
		ScopeTimer(const std::string& name = "Unnamed");

		~ScopeTimer();

	private:
		std::string m_name;
		std::chrono::high_resolution_clock::time_point m_startTime;
	};

	class EventTimer
	{
	public:
		void start(float delaySeconds, std::function<void()> callback);

		void update(float timeStep);
	private:
		std::function<void()> m_callback = {};
		float m_countDown = 0.0f;
		bool m_onCountDown = false;
	};
}