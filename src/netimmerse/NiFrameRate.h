/*
===============================================================================

	NiFrameRate.h

	The application framework's frame-rate overlay: three screen polygons
	that show the frame rate as digits over the camera's view, and the
	frame time statistics behind it.

	Evidence: NiApplication::Initialize (0x7B7C70) allocates it (0x1F0
	bytes), calls the constructor (0x7BB440), sets the colour at 0x1DC to
	opaque red and calls Init (0x7BB7C0) with ( true, 0 ); Terminate
	(0x7B7DD0) calls Shutdown (0x7BB9A0) and then the destructor (0x7BB630).
	EnableFrameRate (0x7B7EB0) calls AddToCamera (0x7BBAA0) or
	RemoveFromCamera (0x7BBAF0). The idle frame (0x7B8840) calls TakeSample
	(0x7BC3A0) and then Update (0x7BC5D0). Init builds eleven texture
	properties from the 8x16 glyphs at 0x97654C (the digits and a blank,
	0x7BBD00), the polygons' corners (0x7BBB40) and the three polygons
	(0x7BC160); the polygons share an alpha, a texture mode and a vertex
	colour property (their vtables 0x888A08, 0x8896CC, 0x889694 in
	0x7BBD00). TakeSample keeps the last 20 sample times and the count, sum
	and sum of squares of the frame times; 0x7BC4C0 turns them into rates.

	The method names, their signatures and the helpers' private access are
	DamageTool.exe's symbols (the SDK tool links the same NiFrameRate:
	?Init@NiFrameRate@@QAEX_NH@Z, ?SetupProperties@NiFrameRate@@AAEXXZ,
	?GetStats@NiFrameRate@@QAEXAAN000PAH@Z...); SetColor is inline there too,
	its signature after Gamebryo 1.1.2's. The members' names are invented.

===============================================================================
*/

#ifndef __NIFRAMERATE_H__
#define __NIFRAMERATE_H__

#include "NiAlphaProperty.h"
#include "NiColor.h"
#include "NiPoint3.h"
#include "NiPointer.h"
#include "NiScreenPolygon.h"
#include "NiTextureModeProperty.h"
#include "NiTextureProperty.h"
#include "NiVertexColorProperty.h"

class NiCamera;

class NiFrameRate {
public:
	enum {
		DIGITS				= 3,	// digits shown
		GLYPHS				= 11,	// the digits 0 to 9 and a blank
		BLANK_GLYPH			= 10,
		RECENT_SAMPLES		= 20	// sample times the displayed rate averages over
	};

							NiFrameRate();
							~NiFrameRate();

	// builds the polygons when display is set, and a histogram of the
	// frame rates of histogramSize_ one-frame-per-second bins
	void					Init( bool display_, int histogramSize_ );
	void					Shutdown();

	// once per rendered frame: measures, then shows the rate
	void					TakeSample();
	void					Update();

	void					AddToCamera( NiCamera *camera );
	void					RemoveFromCamera( NiCamera *camera );

	// by reference, as Gamebryo 1.1.2's SetColor( const NiColor & ): the callers'
	// temporaries are inline sites retail's inline budget counts
	void					SetColor( const NiColorA &color_ ) { color = color_; }

	// the average rate, the deviation of the rate, the lowest and highest
	// rates, and a copy of the histogram when histogram_ is not NULL
	void					GetStats( double &averageRate, double &rateDeviation, double &minRate, double &maxRate,
									  int *histogram_ );

private:
	void					SetScreenPolyCoords();
	void					SetupProperties();
	void					DeleteProperties();
	void					CreateScreenPolys();
	void					DeleteScreenPolys();

protected:
	int						frameRate;				// 0x00 the displayed rate, frames per second
	NiPoint3				vertices[DIGITS][4];	// 0x04 each polygon's corners, in screen fractions
	NiPointer<NiScreenPolygon>	polygons[DIGITS];	// 0x94 the digits, least significant first; handed to the camera
	NiPointer<NiTextureProperty>	glyphs[GLYPHS];	// 0xA0 one texture per glyph
	NiPointer<NiAlphaProperty>	alphaProperty;		// 0xCC shared by the polygons
	NiPointer<NiTextureModeProperty>	textureModeProperty;	// 0xD0
	NiPointer<NiVertexColorProperty>	vertexColorProperty;	// 0xD4
	bool					display;				// 0xD8 Init's first argument; no polygons without it
	int						samples;				// 0xDC frame times measured
	double					sumTime;				// 0xE0 seconds
	double					sumTimeSquared;			// 0xE8
	bool					firstSample;			// 0xF0 the next sample only starts the clock
	double					startTime;				// 0xF8 seconds
	double					lastTime;				// 0x100
	double					recentTimes[RECENT_SAMPLES];	// 0x108 a ring of the last sample times
	int						recentIndex;			// 0x1A8 the oldest entry of recentTimes
	double					minTime;				// 0x1B0 the shortest frame time
	double					maxTime;				// 0x1B8 the longest frame time
	unsigned int			histogramSize;			// 0x1C0
	unsigned int *			histogram;				// 0x1C4 frames per rate, NULL without bins
	float					left;					// 0x1C8 the digits' layout, in screen fractions
	float					top;					// 0x1CC
	float					digitWidth;				// 0x1D0
	float					digitHeight;			// 0x1D4
	float					digitSpacing;			// 0x1D8
	NiColorA				color;					// 0x1DC the polygons' vertex colour
};

#endif /* !__NIFRAMERATE_H__ */
