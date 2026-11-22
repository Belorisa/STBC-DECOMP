// NiFrameRate.cpp -- the application framework's frame-rate overlay

#include "NiRTLib.h"
#include <math.h>
#include <string.h>

#include "NiCamera.h"
#include "NiFrameRate.h"
#include "NiPoint2.h"
#include "NiSystem.h"
#include "renderer/NiImage.h"
#include "NiRawImageData.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007bc8c0 _$E30
// FUNCTION: STBC 0x007bc8d0 _$E45
// FUNCTION: STBC 0x007bc8e0 _$E49
// FUNCTION: STBC 0x007bc8f0 _$E53
// FUNCTION: STBC 0x007bc900 _$E57
// FUNCTION: STBC 0x007bc910 _$E81
// FUNCTION: STBC 0x007bc940 _$E111
// FUNCTION: STBC 0x007bc950 _$E166
// FUNCTION: STBC 0x007bc960 _$E181
// FUNCTION: STBC 0x007bc970 _$E34
// FUNCTION: STBC 0x007bc980 _$E123
// FUNCTION: STBC 0x007bc9b0 _$E136
// FUNCTION: STBC 0x007bc9c0 _$E186
// FUNCTION: STBC 0x007bc9d0 _$E191
// FUNCTION: STBC 0x007bc9e0 _$E195
// FUNCTION: STBC 0x007bc9f0 _$E199
// FUNCTION: STBC 0x007bca00 _$E206
// FUNCTION: STBC 0x007bcf40 _$E156

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007bc920 _$E219
// FUNCTION: STBC 0x007bc840 _$E17
// FUNCTION: STBC 0x007bc860 _$E15
// FUNCTION: STBC 0x007bc870 _$E23
// FUNCTION: STBC 0x007bc890 _$E21

// the glyphs' rows, top first; bit 0 is the left column
// DATA: STBC 0x97654C
static unsigned short digitGlyphs[NiFrameRate::GLYPHS][16] = {
	{ 0x003C, 0x007E, 0x00E7, 0x00C3, 0x00C3, 0x00C3, 0x00C3, 0x00C3, 0x00C3, 0x00C3, 0x00C3, 0x00C3, 0x00C3, 0x00E7, 0x007E, 0x003C },
	{ 0x0018, 0x0038, 0x0078, 0x0018, 0x0018, 0x0018, 0x0018, 0x0018, 0x0018, 0x0018, 0x0018, 0x0018, 0x0018, 0x0018, 0x0018, 0x003C },
	{ 0x001C, 0x003E, 0x00E7, 0x00C3, 0x0083, 0x0003, 0x0003, 0x0003, 0x0006, 0x000C, 0x001C, 0x0030, 0x0070, 0x00E0, 0x00FF, 0x00FF },
	{ 0x003C, 0x007E, 0x00E7, 0x00C3, 0x0003, 0x0003, 0x0006, 0x001C, 0x001C, 0x0006, 0x0003, 0x0003, 0x00C3, 0x00E7, 0x007E, 0x003C },
	{ 0x000E, 0x001E, 0x001E, 0x0036, 0x0066, 0x0066, 0x00C6, 0x00FF, 0x00FF, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x000F },
	{ 0x00FF, 0x00FF, 0x00C0, 0x00C0, 0x00C0, 0x00C0, 0x00FC, 0x00FE, 0x0087, 0x0003, 0x0003, 0x0003, 0x0003, 0x00C6, 0x00FE, 0x0078 },
	{ 0x003C, 0x007E, 0x00E7, 0x00C3, 0x00C0, 0x00C0, 0x00C0, 0x00C0, 0x00FC, 0x00FE, 0x00E7, 0x00C3, 0x00C3, 0x00E7, 0x007E, 0x003C },
	{ 0x00FF, 0x00FF, 0x0083, 0x0003, 0x0006, 0x0006, 0x000C, 0x000C, 0x0018, 0x0018, 0x0030, 0x0030, 0x0060, 0x0060, 0x00C0, 0x00C0 },
	{ 0x003C, 0x007E, 0x00E7, 0x00C3, 0x00C3, 0x00C3, 0x0066, 0x003C, 0x003C, 0x0066, 0x00C3, 0x00C3, 0x00C3, 0x00E7, 0x007E, 0x003C },
	{ 0x003C, 0x007E, 0x00C7, 0x00C3, 0x00C3, 0x00C3, 0x00E7, 0x007F, 0x003B, 0x0003, 0x0003, 0x0003, 0x0003, 0x0003, 0x0003, 0x0003 },
	{ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 }
};

// the texture coordinates of a polygon's corners; the glyphs' bit order
// mirrors them
// FUNCTION: STBC 0x007bb3e0 _$E212
// DATA: STBC 0x9A0C90
static NiPoint2 digitTextureCoords[4] = {
	NiPoint2( 1.0f, 1.0f ),
	NiPoint2( 0.0f, 1.0f ),
	NiPoint2( 0.0f, 0.0f ),
	NiPoint2( 1.0f, 0.0f )
};

/*
================
NiFrameRate::NiFrameRate
================
*/
// FUNCTION: STBC 0x7BB440
NiFrameRate::NiFrameRate() {
	int i;
	for ( i = 0; i < GLYPHS; i++ ) {
		glyphs[i] = 0;
	}
	for ( i = 0; i < DIGITS; i++ ) {
		polygons[i] = 0;
	}
	display = false;
	alphaProperty = 0;
	textureModeProperty = 0;
	vertexColorProperty = 0;

	samples = 0;
	sumTime = 0.0;
	sumTimeSquared = 0.0;
	firstSample = true;
	startTime = 0.0;
	lastTime = 0.0;
	recentIndex = 0;
	frameRate = 0;
	histogramSize = 0;
	histogram = NULL;

	left = 0.9f;
	top = 0.935f;
	digitWidth = 0.025f;
	digitHeight = 0.05f;
	digitSpacing = 0.005f;
	SetColor( NiColorA( 1.0f, 1.0f, 1.0f, 1.0f ) );
}

/*
================
NiFrameRate::~NiFrameRate
================
*/
// FUNCTION: STBC 0x7BB630
NiFrameRate::~NiFrameRate() {
	if ( histogram ) {
		delete [] histogram;
	}
	DeleteScreenPolys();
	DeleteProperties();
}

/*
================
NiFrameRate::Init
================
*/
// FUNCTION: STBC 0x7BB7C0
void NiFrameRate::Init( bool display_, int histogramSize_ ) {
	DeleteScreenPolys();
	DeleteProperties();
	if ( histogram ) {
		delete [] histogram;
	}
	histogram = NULL;

	if ( display_ ) {
		SetupProperties();
		SetScreenPolyCoords();
		CreateScreenPolys();
	}

	histogramSize = histogramSize_;
	if ( histogramSize_ ) {
		histogram = new unsigned int[histogramSize_];
		if ( !histogram ) {
			histogramSize = 0;
		} else {
			memset( histogram, 0, histogramSize * sizeof( unsigned int ) );
		}
	}

	display = display_;
	samples = 0;
	sumTime = 0.0;
	sumTimeSquared = 0.0;
	firstSample = true;
	startTime = 0.0;
	lastTime = 0.0;
	recentIndex = 0;
	frameRate = 0;
	minTime = 100000.0;
	maxTime = -100000.0;
}

/*
================
NiFrameRate::Shutdown
================
*/
// FUNCTION: STBC 0x7BB9A0
void NiFrameRate::Shutdown() {
	if ( histogram ) {
		delete [] histogram;
	}
	histogram = NULL;
	DeleteScreenPolys();
	DeleteProperties();
}

/*
================
NiFrameRate::AddToCamera

Hands the polygons to the camera, when Init built them.
================
*/
// FUNCTION: STBC 0x7BBAA0
void NiFrameRate::AddToCamera( NiCamera *camera ) {
	if ( display ) {
		for ( int i = 0; i < DIGITS; i++ ) {
			camera->AttachScreenPolygon( polygons[i] );
		}
	}
}

/*
================
NiFrameRate::RemoveFromCamera
================
*/
// FUNCTION: STBC 0x7BBAF0
void NiFrameRate::RemoveFromCamera( NiCamera *camera ) {
	if ( display ) {
		for ( int i = 0; i < DIGITS; i++ ) {
			camera->DetachScreenPolygon( polygons[i] );
		}
	}
}

/*
================
NiFrameRate::SetScreenPolyCoords

Lays the digits out from the left, most significant first: the last
polygon is the leftmost.
================
*/
// FUNCTION: STBC 0x7BBB40
// PARKED: schedule only: in the last digit's first corner retail loads x into ecx before the y store and stores z after x; this build the reverse
void NiFrameRate::SetScreenPolyCoords() {
	float x = left;

	vertices[2][0] = NiPoint3( x, top, 0.0f );
	vertices[2][1] = NiPoint3( x + digitWidth, top, 0.0f );
	vertices[2][2] = NiPoint3( x + digitWidth, top + digitHeight, 0.0f );
	vertices[2][3] = NiPoint3( x, top + digitHeight, 0.0f );
	x += digitSpacing + digitWidth;

	vertices[1][0] = NiPoint3( x, top, 0.0f );
	vertices[1][1] = NiPoint3( x + digitWidth, top, 0.0f );
	vertices[1][2] = NiPoint3( x + digitWidth, top + digitHeight, 0.0f );
	vertices[1][3] = NiPoint3( x, top + digitHeight, 0.0f );
	x += digitSpacing + digitWidth;

	vertices[0][0] = NiPoint3( x, top, 0.0f );
	vertices[0][1] = NiPoint3( x + digitWidth, top, 0.0f );
	vertices[0][2] = NiPoint3( x + digitWidth, top + digitHeight, 0.0f );
	vertices[0][3] = NiPoint3( x, top + digitHeight, 0.0f );
}

/*
================
NiFrameRate::SetupProperties

Draws each glyph into an 8x16 image, white where its bits are set and
clear elsewhere, and makes the properties the polygons share.
================
*/
// FUNCTION: STBC 0x7BBD00
// PARKED: inline budget: retail calls NiRawImageData::GetPixel (0x7BC990), NiTextureProperty's constructor (0x809FE0), NiPointer's constructor (0x7BC8A0) and NiPointer<NiImage>::operator= (0x45D9A0) out of line; this build inlines GetPixel and the constructor
void NiFrameRate::SetupProperties() {
	for ( int i = 0; i < GLYPHS; i++ ) {
		NiRawImageData *raw = new NiRawImageData( NiRawImageData::FORMAT_RGBA, 8, 16 );
		if ( raw ) {
			for ( int y = 0; y < 16; y++ ) {
				unsigned short row = digitGlyphs[i][y];
				unsigned short bit = 1;
				for ( int x = 0; x < 8; x++ ) {
					unsigned char *pixel = ( *raw )( x, y );
					if ( row & bit ) {
						pixel[0] = pixel[1] = pixel[2] = pixel[3] = 0xFF;
					} else {
						pixel[0] = pixel[1] = pixel[2] = pixel[3] = 0;
					}
					bit <<= 1;
				}
			}
		}

		NiTextureProperty *texture = NULL;
		if ( raw ) {
			texture = new NiTextureProperty;
			if ( texture ) {
				NiImage *image = NiImage::Create( raw, 0, 7, 2 );
				if ( image ) {
					texture->SetImage( image );
				} else {
					delete texture;
					texture = NULL;
				}
			}
		}
		glyphs[i] = texture;
	}

	alphaProperty = new NiAlphaProperty;
	if ( alphaProperty ) {
		alphaProperty->SetAlphaBlending( true );
		alphaProperty->SetSrcBlendMode( NiAlphaProperty::ALPHA_SRCALPHA );
		alphaProperty->SetDestBlendMode( NiAlphaProperty::ALPHA_INVSRCALPHA );
	}

	textureModeProperty = new NiTextureModeProperty;
	if ( textureModeProperty ) {
		textureModeProperty->SetFilterMode( NiTextureModeProperty::FILTER_NEAREST );
	}

	vertexColorProperty = new NiVertexColorProperty;
	if ( vertexColorProperty ) {
		vertexColorProperty->SetSourceMode( NiVertexColorProperty::SOURCE_EMISSIVE );
		vertexColorProperty->SetLightingMode( NiVertexColorProperty::LIGHTING_E );
	}
}

/*
================
NiFrameRate::DeleteProperties
================
*/
// FUNCTION: STBC 0x7BC0A0
void NiFrameRate::DeleteProperties() {
	for ( int i = 0; i < GLYPHS; i++ )
		glyphs[i] = 0;
	alphaProperty = 0;
	textureModeProperty = 0;
	vertexColorProperty = 0;
}

/*
================
NiFrameRate::CreateScreenPolys

The polygons start out blank.
================
*/
// FUNCTION: STBC 0x7BC160
void NiFrameRate::CreateScreenPolys() {
	NiColorA colors[4];
	int i;
	for ( i = 0; i < 4; i++ ) {
		colors[i] = color;
	}

	for ( i = 0; i < DIGITS; i++ ) {
		NiScreenPolygon *polygon = new NiScreenPolygon( 4, vertices[i], digitTextureCoords, colors );
		if ( polygon ) {
			polygon->GetPropertyState()->Reset();
			polygon->GetPropertyState()->SetProperty( glyphs[BLANK_GLYPH] );
			polygon->GetPropertyState()->SetProperty( alphaProperty );
			polygon->GetPropertyState()->SetProperty( textureModeProperty );
			polygon->GetPropertyState()->SetProperty( vertexColorProperty );
			polygons[i] = polygon;
		}
	}
}

/*
================
NiFrameRate::DeleteScreenPolys
================
*/
// FUNCTION: STBC 0x7BC350
void NiFrameRate::DeleteScreenPolys() {
	for ( int i = 0; i < DIGITS; i++ ) {
		if ( polygons[i] ) {
			polygons[i]->GetPropertyState()->Reset();
		}
		polygons[i] = 0;
	}
}

/*
================
NiFrameRate::TakeSample

The displayed rate counts the frames of the last RECENT_SAMPLES samples;
the histogram bins the rate of this frame alone.
================
*/
// FUNCTION: STBC 0x7BC3A0
void NiFrameRate::TakeSample() {
	double time = NiGetCurrentTimeInSec();
	if ( firstSample ) {
		startTime = time;
		lastTime = time;
		firstSample = false;
		return;
	}

	samples++;
	recentTimes[recentIndex] = time;
	recentIndex = ( recentIndex + 1 ) % RECENT_SAMPLES;
	if ( samples >= RECENT_SAMPLES ) {
		frameRate = (int)( ( RECENT_SAMPLES - 1 ) / ( time - recentTimes[recentIndex] ) + 0.5 );
	}

	double frameTime = time - lastTime;
	sumTime += frameTime;
	sumTimeSquared += frameTime * frameTime;
	lastTime = time;
	if ( frameTime < minTime ) {
		minTime = frameTime;
	}
	if ( frameTime > maxTime ) {
		maxTime = frameTime;
	}

	if ( histogram ) {
		int bin = (int)( 1.0 / frameTime );
		if ( bin >= (int)histogramSize ) {
			bin = histogramSize - 1;
		}
		if ( bin < 0 ) {
			bin = 0;
		}
		histogram[bin]++;
	}
}

/*
================
NiFrameRate::GetStats

The deviation of the rate is the spread between the rates one deviation
of the frame time either side of the average frame time.
================
*/
// FUNCTION: STBC 0x7BC4C0
// PARKED: schedule only: retail stores the divisor count * ( count - 1 ) for its fild before the numerator's subtraction, this build after it; a divisor local and its other spellings do not move it
void NiFrameRate::GetStats( double &averageRate, double &rateDeviation, double &minRate, double &maxRate,
							int *histogram_ ) {
	rateDeviation = 0.0;
	averageRate = 0.0;

	int count = samples;
	if ( count == 0 ) {
		return;
	}

	double average = count / ( lastTime - startTime );
	double deviation = 0.0;
	if ( count > 1 ) {
		double averageTime = 1.0 / average;
		double timeDeviation = sqrt( ( count * sumTimeSquared - sumTime * sumTime ) / ( count * ( count - 1 ) ) );
		deviation = ( timeDeviation + timeDeviation ) / ( ( averageTime - timeDeviation ) * ( averageTime + timeDeviation ) );
	}
	averageRate = average;
	rateDeviation = deviation;
	minRate = 1.0 / maxTime;
	maxRate = 1.0 / minTime;

	if ( histogram_ ) {
		memcpy( histogram_, histogram, histogramSize * sizeof( unsigned int ) );
	}
}

/*
================
NiFrameRate::Update

Shows the rate's last three digits; leading zeros are blank, and so is a
rate of zero.
================
*/
// FUNCTION: STBC 0x7BC5D0
void NiFrameRate::Update() {
	if ( !display ) {
		return;
	}

	int digits[DIGITS];
	int value = frameRate;
	int i;
	for ( i = 0; i < DIGITS; i++ ) {
		digits[i] = value % 10;
		value /= 10;
	}

	bool blank[DIGITS];
	blank[0] = blank[1] = blank[2] = false;
	for ( i = DIGITS - 1; i >= 0; i-- ) {
		if ( digits[i] != 0 ) {
			break;
		}
		blank[i] = true;
	}

	for ( i = 0; i < DIGITS; i++ ) {
		if ( polygons[i] ) {
			polygons[i]->GetPropertyState()->Reset();
			if ( blank[i] ) {
				polygons[i]->SetProperty( glyphs[BLANK_GLYPH] );
			} else {
				polygons[i]->SetProperty( glyphs[digits[i]] );
			}
			polygons[i]->SetProperty( alphaProperty );
			polygons[i]->SetProperty( textureModeProperty );
			polygons[i]->SetProperty( vertexColorProperty );
		}
	}
}
