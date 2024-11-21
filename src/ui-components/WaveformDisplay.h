#pragma once

#include <juce_audio_utils/juce_audio_utils.h>
#include "../StylesStore.h"

typedef std::function<void(const juce::Point<int>& from, const juce::Point<int>& to)> DrawCallback;

class WaveformDisplay : public juce::Component, juce::ChangeListener
{
public:
	explicit WaveformDisplay(
			const StylesStore& stylesStore,
			juce::AudioThumbnailCache& thumbnailCache,
			juce::AudioFormatManager& formatManager,
			DrawCallback onDrawCallback);

	~WaveformDisplay() override;

	void paint(juce::Graphics&) override;

	void mouseDown(const juce::MouseEvent& event) override;

	void mouseDrag(const juce::MouseEvent& event) override;

	void setSource(const juce::AudioBuffer<float>& newSource, double sampleRate, juce::int64 hashCode);

	void changeListenerCallback(juce::ChangeBroadcaster* source) override;

private:
	juce::AudioThumbnail thumbnail;
	std::function<void(const juce::Point<int>& from, const juce::Point<int>& to)> onDraw;
	juce::Point<int> lastMousePosition;
	const StylesStore& stylesStore;

	bool forceHorizontalBounds(juce::Point<int>& pos);

	void forceVerticalBounds(juce::Point<int>& pos);
};
