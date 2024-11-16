#include "WaveformUI.h"
#include "../StylesStore.h"
#include <juce_audio_utils/juce_audio_utils.h>
#include <utility>

WaveformUI::WaveformUI(
		const StylesStore& stylesStore,
		juce::AudioThumbnailCache& thumbnailCache,
		juce::AudioFormatManager& formatManager,
		DrawCallback onDrawCallback)
		: stylesStore(stylesStore),
		  thumbnail(1, formatManager, thumbnailCache),
		  onDraw(std::move(onDrawCallback))
{
	thumbnail.addChangeListener(this);
}

WaveformUI::~WaveformUI()
{
	thumbnail.removeChangeListener(this);
}

void WaveformUI::setSource(
		const juce::AudioBuffer<float>& newSource,
		double sampleRate,
		juce::int64 hashCode)
{
	thumbnail.setSource(&newSource, sampleRate, hashCode);
}

void WaveformUI::changeListenerCallback(juce::ChangeBroadcaster* source)
{
	juce::ignoreUnused(source);
	repaint();
}

void WaveformUI::paint(juce::Graphics& g)
{
	const auto width = getWidth();
	const auto height = getHeight();
	const juce::Rectangle<int> thumbnailBounds(0, 0, width, height);

	g.setColour(stylesStore.getColor(StylesStore::ColorIds::WaveformUI_background));
	g.fillRect(thumbnailBounds);
	g.setColour(stylesStore.getColor(StylesStore::ColorIds::WaveformUI_zero));
	g.drawHorizontalLine(height / 2, 0.0f, static_cast<float>(width) - 1.0f);
	g.setColour(stylesStore.getColor(StylesStore::ColorIds::WaveformUI_signal));
	thumbnail.drawChannel(g,
			thumbnailBounds,
			0.0,
			thumbnail.getTotalLength(),
			0,
			1.0f);
}

void WaveformUI::mouseDown(const juce::MouseEvent& event)
{
	if (onDraw == nullptr)
	{ return; }

	const auto pos = event.getPosition();
	onDraw(pos, pos);

	lastMousePosition = pos;
}

void WaveformUI::mouseDrag(const juce::MouseEvent& event)
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

bool WaveformUI::forceHorizontalBounds(juce::Point<int>& pos)
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

void WaveformUI::forceVerticalBounds(juce::Point<int>& pos)
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
