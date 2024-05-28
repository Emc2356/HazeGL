#pragma once

#include <chrono>

namespace HazeGL {
    class Timer {
    public:
        Timer() {
            reset();
        }

        void reset() {
            mStart = std::chrono::high_resolution_clock::now();
        }

        float elapsed() {
            return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - mStart).count() * 0.001f * 0.001f * 0.001f;
        }

        float elapsed_millis() {
            return elapsed() * 1000.0f;
        }

    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> mStart;
    };

    class ScopedTimer {
	public:
		ScopedTimer(const std::string& name)
            : mName(name) {}
		~ScopedTimer() {
			float time = mTimer.elapsed_millis();
            printf("[TIMER] %s - %fms\n", mName.c_str(), time);
		}
	private:
		std::string mName;
		Timer mTimer;
	};
} 

