#pragma once

#include <juce_audio_utils/juce_audio_utils.h>
#include <functional>
#include <vector>

template<typename T>
class ValueMonitor : private juce::Timer {
public:
    using Callback = std::function<void(const T&)>;

    ValueMonitor(int interval) : interval(interval) {
        startTimer(interval);
    }

    void setValue(const T& val) {
        const juce::ScopedLock lock(valueLock);
        value = val;
    }

    void addObserver(Callback callback) {
        const juce::ScopedLock lock(callbacksLock);
        callbacks.push_back(callback);
    }

private:
    void timerCallback() override {
        T currentValue;
        {
            const juce::ScopedLock lock(valueLock);
            currentValue = value;
        }

        if (currentValue != lastValue) {
            lastValue = currentValue;
            notifyObservers(currentValue);
        }
    }

    void notifyObservers(const T& newValue) {
        const juce::ScopedLock lock(callbacksLock);
        for (const auto& callback : callbacks) {
            callback(newValue);
        }
    }

    T value;
    T lastValue;
    int interval;
    std::vector<Callback> callbacks;

    juce::CriticalSection valueLock;
    juce::CriticalSection callbacksLock;
};
