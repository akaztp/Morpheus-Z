#include "WaveformDisplay.h"
#include "../StylesStore.h"
#include <juce_audio_utils/juce_audio_utils.h>
#include <utility>

WaveformDisplay::WaveformDisplay(
		const StylesStore& stylesStore,
		juce::AudioThumbnailCache& thumbnailCache,
		juce::AudioFormatManager& formatManager,
		DrawCallback onDrawCallback)
		: StyledComponent(stylesStore), thumbnail(1, formatManager, thumbnailCache), onDraw(std::move(onDrawCallback))
{
	thumbnail.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
	thumbnail.removeChangeListener(this);
}

void WaveformDisplay::setSource(
		const juce::AudioBuffer<float>& newSource,
		double sampleRate,
		juce::int64 hashCode)
{
	thumbnail.setSource(&newSource, sampleRate, hashCode);
}

void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster* source)
{
	juce::ignoreUnused(source);
	repaint();
}

void WaveformDisplay::paint(juce::Graphics& g)
{
	const auto width = getWidth();
	const auto height = getHeight();
	const juce::Rectangle<int> thumbnailBounds(0, 0, width, height);

	g.setColour(stylesStore.getColor(StylesStore::ColorIds::WaveformDisplay_background));
	g.fillRect(thumbnailBounds);
	g.setColour(stylesStore.getColor(StylesStore::ColorIds::WaveformDisplay_zero));
	g.drawHorizontalLine(height / 2, 0.0f, static_cast<float>(width) - 1.0f);
	g.setColour(stylesStore.getColor(StylesStore::ColorIds::WaveformDisplay_signal));
	thumbnail.drawChannel(g,
			thumbnailBounds,
			0.0,
			thumbnail.getTotalLength(),
			0,
			1.0f);
}

void WaveformDisplay::mouseDown(const juce::MouseEvent& event)
{
	if (onDraw == nullptr)
	{ return; }

	const auto pos = event.getPosition();
	onDraw(pos, pos);

	lastMousePosition = pos;
}

void WaveformDisplay::mouseDrag(const juce::MouseEvent& event)
{
	if (onDraw == nullptr)
	{ return; }

	auto pos = event.getPosition();
	forceVerticalBounds(pos);
	const auto forcedHorizontally = forceHorizontalBounds(pos);
	const auto x = pos.getX();
	const auto y = pos.getY();
	const auto lastX = lastMousePosition.getX();
	const auto lastY = lastMousePosition.getY();
	if (x == lastX)
	{
		if (y != lastY && !forcedHorizontally)
		{
			onDraw(pos, pos);
		}
	}
	else
	{
		if (x > lastX)
		{
			onDraw(lastMousePosition, pos);
		}
		else
		{
			onDraw(pos, lastMousePosition);
		}
	}

	lastMousePosition = pos;
}

bool WaveformDisplay::forceHorizontalBounds(juce::Point<int>& pos)
{
	auto forced = false;
	const auto x = pos.getX();
	if (x >= getWidth())
	{
		pos.x = getWidth() - 1;
		forced = true;
	}
	else if (x < 0)
	{
		pos.x = 0;
		forced = true;
	}
	return forced;
}

void WaveformDisplay::forceVerticalBounds(juce::Point<int>& pos)
{
	const auto y = pos.getY();
	if (y >= getHeight())
	{
		pos.y = getHeight() - 1;
	}
	else if (y < 0)
	{
		pos.y = 0;
	}
}
