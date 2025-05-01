#pragma once
#include "const.h"
class GameTime
{
public:
	static inline float getDeltaTime(bool unscaled = false) { return unscaled ? s_deltaTime : s_scaledDeltaTime; }
	static inline float getFixedDeltaTime(bool unscaled = false) { return unscaled ? s_scaledFixedDeltaTime : FIXED_DELTA_TIME; }
	static inline float getElapsedTime() { return s_elapsedTime; }
	static inline float getTimeScale() { return s_timeScale; }

	static inline void setDeltaTime(float deltaTime) { s_deltaTime = deltaTime; updateScaledDeltaTime(); }
	static inline void setElapsedTime(float elapsedTime) { s_elapsedTime = elapsedTime; }
	static inline void setTimeScale(float timeScale) { s_timeScale = timeScale; updateScaledDeltaTime(); updateScaledFixedDeltaTime(); }
private:
	GameTime() = delete;
	GameTime(const GameTime&) = delete;
	static inline void updateScaledDeltaTime()
	{
		s_scaledDeltaTime = s_deltaTime * s_timeScale;
	}
	static inline void updateScaledFixedDeltaTime()
	{
		s_scaledFixedDeltaTime = FIXED_DELTA_TIME * s_timeScale;
	}
private:
	static float s_timeScale;
	static float s_scaledDeltaTime;
	static float s_deltaTime;
	static float s_scaledFixedDeltaTime;
	static float s_elapsedTime;
};
