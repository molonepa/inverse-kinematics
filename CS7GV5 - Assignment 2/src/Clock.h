#pragma once

#include <SDL.h>

class Clock {
public:
	void Tick() {
		m_DeltaTime = SDL_GetPerformanceCounter() - m_LastTime;
		m_ElapsedTime += m_DeltaTime;
		m_LastTime = SDL_GetPerformanceCounter();
	}

	inline Uint64 GetDeltaTime() { return m_DeltaTime; }
	inline Uint64 GetElapsedTime() { return m_ElapsedTime; }

private:
	Uint64 m_DeltaTime;
	Uint64 m_LastTime;
	Uint64 m_ElapsedTime = 0.0f;
};