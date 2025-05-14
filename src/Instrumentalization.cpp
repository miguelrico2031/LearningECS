#include "Instrumentalization.h"
#include <iostream>
#include <fstream> // Added for file handling
#include <unordered_map>
#include <filesystem> // Added for filesystem utilities

namespace Instrumentalization
{
	void FPSCounter::start(const std::string& name, const std::string& path)
	{
		m_name = name;
		m_path = path;
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

			// Calculate average FPS
			float avgFPS = static_cast<float>(m_totalFrames) / m_totalSeconds;

			// Create JSON string manually
			std::string jsonResult = "{\n";
			jsonResult += "    \"name\": \"" + m_name + "\",\n";
			jsonResult += "    \"total_frames\": " + std::to_string(m_totalFrames) + ",\n";
			jsonResult += "    \"total_seconds\": " + std::to_string(m_totalSeconds) + ",\n";
			jsonResult += "    \"avg_fps\": " + std::to_string(avgFPS) + "\n";
			jsonResult += "}";

			 // Ensure the directory exists
			try
			{
				if (!std::filesystem::exists(m_path))
				{
					std::filesystem::create_directories(m_path);
				}
			}
			catch (const std::filesystem::filesystem_error& e)
			{
				std::cerr << "INSTR ERR: Failed to create directory: " << m_path << "\n";
				std::cerr << "INSTR ERR: " << e.what() << "\n";
				return;
			}

			// Write JSON to file
			static std::unordered_map<std::string, unsigned int> fileCounter;

			std::string fileName = m_name + "_fps_results_" + std::to_string(fileCounter[m_name]++) + ".json";
			std::string fullPath = m_path + fileName;
			std::ofstream outFile(fullPath);
			if (outFile.is_open())
			{
				outFile << jsonResult;
				outFile.close();
			}
			else
			{
				std::cerr << "INSTR ERR: Failed to open file.\n";
			}

			// Log to console
			std::cout << "INSTR LOG: " << "FPS Counter results: --------------------------------------------" << "\n";
			std::cout << "INSTR LOG: " << "Name: [" << m_name << "]" << "\n";
			std::cout << "INSTR LOG: " << "Total frames: " << m_totalFrames << "\n";
			std::cout << "INSTR LOG: " << "Average FPS: " << avgFPS << "\n";
			std::cout << "INSTR LOG: " << "-----------------------------------------------------------------" << "\n";

			std::cout << "INSTR LOG: " << "FPS Counter results written to " << fullPath << "\n";
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