/*
===============================================================================

	GraphicsModeInfo.h

	A graphics mode: the device name and description, the window flag, the
	screen size, the colour depth and the Z buffer depth. The members are
	named after the SWIG wrappers GraphicsModeInfo_*; the class has no
	virtual functions.

===============================================================================
*/

#ifndef __GRAPHICSMODEINFO_H__
#define __GRAPHICSMODEINFO_H__

class Ni3DDeviceDesc;

class GraphicsModeInfo {
public:
					GraphicsModeInfo( void );
					~GraphicsModeInfo( void );

	// Replaces the device description, deleting the old one; NULL only
	// deletes it (invented name, after GetDesc).
	void			SetDesc( Ni3DDeviceDesc *newDesc );
	// A copy of the device's name (invented name, after GetDevice).
	void			SetDevice( char *newDevice );

	GraphicsModeInfo &	operator=( const GraphicsModeInfo &mode );

	char *	GetDevice( void ) const { return device; }
	bool	IsHighZBuffer( void ) const { return highZBuffer; }
	int	GetColorDepth( void ) const { return colorDepth; }
	Ni3DDeviceDesc *	GetDesc( void ) const { return desc; }
	bool	IsWindowed( void ) const { return windowed; }
	// The setters beside the getters (invented): operator= and
	// UpdateCurrentMode spend retail's inline budget on one site per member
	// stored, as calls to these give.
	void	SetWindowed( bool windowed_ ) { windowed = windowed_; }
	void	SetWidth( unsigned int width_ ) { width = width_; }
	void	SetHeight( int height_ ) { height = height_; }
	void	SetColorDepth( int colorDepth_ ) { colorDepth = colorDepth_; }
	void	SetHighZBuffer( bool highZBuffer_ ) { highZBuffer = highZBuffer_; }
	// The mode in use (GraphicsModeInfo_GetCurrentMode reads it).
	static GraphicsModeInfo *	GetCurrentMode( void ) { return currentMode; }
	// Makes the current mode anew from the renderer's (640 by 480 in 16
	// bits without one) and loads its LCARS icon group (invented name).
	static void		UpdateCurrentMode( void );
	// Deletes the current mode (invented name).
	static void		DeleteCurrentMode( void );

	int	GetCurrentResolution( void );
	// The name of the LCARS icon group drawn at the current resolution
	// (GraphicsModeInfo_GetLcarsString).
	char *	GetLcarsString( void ) { return lcarsStrings[GetCurrentResolution()]; }

	// The module of the LCARS icon group drawn at the current resolution
	// (GraphicsModeInfo_GetLcarsModule).
	char *	GetLcarsModule( void ) { return lcarsModules[GetCurrentResolution()]; }

	// The screen width in pixels (GraphicsModeInfo_GetWidth reads 0x0C).
	int		GetWidth( void ) const { return width; }
	// The screen height in pixels (GraphicsModeInfo_GetHeight reads 0x10).
	int		GetHeight( void ) const { return height; }
	// One pixel in screen widths and heights (invented names, from the SWIG wrappers GetPixelWidth and GetPixelHeight).
	float	GetPixelWidth( void ) { return 1.0f / (unsigned int)GetWidth(); }
	float	GetPixelHeight( void ) { return 1.0f / (unsigned int)GetHeight(); }

	static GraphicsModeInfo *	currentMode;	// invented name
	static char *	lcarsStrings[];				// "LCARS_640" and on, by resolution; invented name
	static char *	lcarsModules[];				// "Icons.LCARS_640" and on, by resolution; invented name

	char *			device;						// 0x00 (GraphicsModeInfo_GetDevice)
	Ni3DDeviceDesc *	desc;					// 0x04 (GraphicsModeInfo_GetDesc)
	bool			windowed;					// 0x08 (GraphicsModeInfo_IsWindowed)
	unsigned int	width;						// 0x0C the screen width in pixels (GetCurrentResolution reads it; invented name)
	int				height;						// 0x10
	int				colorDepth;					// 0x14 (GraphicsModeInfo_GetColorDepth)
	bool			highZBuffer;				// 0x18 (GraphicsModeInfo_IsHighZBuffer)
};

#endif /* !__GRAPHICSMODEINFO_H__ */
