#pragma once

#include <functional>
#include <map>
#include <vector>

#include <SDL.h>

class EventHandler {
public:
	using Callback = std::function<void()>;
	using MouseCallback = std::function<void(int, int)>;

	inline void addKeyBinding(int key, const Callback& callback) {
		m_keyCallbacks[key].push_back(callback);
		m_usedKeys.push_back(key);
	}

	inline void addMouseBinding(const MouseCallback& callback) {
		m_mouseMoveCallbacks.push_back(callback);
	}

	inline void onKeyPress(int key) {
		for (Callback& callback : m_keyCallbacks[key]) {
			callback();
		}
	}

	inline void onMouseMove(int x, int y) {
		for (MouseCallback& callback : m_mouseMoveCallbacks) {
			callback(x, y);
		}
	}

	inline void handleKeyboardInput() {
		const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
		for (auto key : m_usedKeys) {
			if (keyboardState[key]) {
				onKeyPress(key);
			}
		}
	}

	inline void handleMouseInput() {
		int dx, dy;
		SDL_GetRelativeMouseState(&dx, &dy);
		onMouseMove(dx, dy);
	}

private:
	std::map<int, std::vector<Callback>> m_keyCallbacks;
	std::vector<int> m_usedKeys;

	std::vector<MouseCallback> m_mouseMoveCallbacks;
};