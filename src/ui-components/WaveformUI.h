#pragma once

#include <juce_audio_utils/juce_audio_utils.h>

class WaveformUI : public juce::Component, juce::ChangeListener
{
public:
	explicit WaveformUI(
			juce::AudioThumbnailCache& thumbnailCache,
			juce::AudioFormatManager& formatManager,
			std::function<void(const juce::Point<int>& from, const juce::Point<int>& to)> onDrawCallback);

	~WaveformUI() override;

	void paint(juce::Graphics&) override;

	void mouseDown(const juce::MouseEvent& event) override;

	void mouseDrag(const juce::MouseEvent& event) override;

	void setSource(const juce::AudioBuffer<float>& newSource, double sampleRate, juce::int64 hashCode);

	void changeListenerCallback(juce::ChangeBroadcaster* source) override;

private:
	juce::AudioThumbnail thumbnail;
	std::function<void(const juce::Point<int>& from, const juce::Point<int>& to)> onDraw;
	juce::Point<int> lastMousePosition;

	bool forceBounds(juce::Point<int>& pos);
};