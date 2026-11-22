/*
===============================================================================

	TGAnimationManagerClass.h

	The animations of the game, each an entry (TGAnimationEntry) in an array
	sorted by name, and the file each loaded animation came from. There is one,
	the global g_kAnimationManager.

	Evidence: the dynamic initialiser (0x6C7DA0) runs the constructor
	(0x6C7E40), which clears the array at 0x00 and builds a string map of 37
	buckets at 0x0C (the vtable 0x889554). The SWIG wrappers
	TGAnimationManagerClass_* name the members. The array is invented.

===============================================================================
*/

#ifndef __TGANIMATIONMANAGERCLASS_H__
#define __TGANIMATIONMANAGERCLASS_H__

#include "NiTStringMap.h"
#include "SortedArray.h"
#include "TGAnimationEntry.h"

class NiNode;
class TGAnimBlender;
class TGStream;

class TGAnimationManagerClass {
public:
	// FUNCTION: STBC 0x006c7e40 ??0TGAnimationManagerClass@@QAE@XZ
					TGAnimationManagerClass( void ) {}
					~TGAnimationManagerClass( void );

	// What an animation does to the model's root node (the SWIG constants
	// TGAnimationManagerClass_SAT_*; the values follow their order in the
	// constant table).
	enum {
		SAT_NORMAL,
		SAT_SPECIAL,
		SAT_MOVES_PELVIS
	};

	int	LoadAnimation( char *, char * );
	int	MapDefaultAnimation( NiNode *, char * );
	void	FreeAnimation( char * );
	int	UseAnimation( TGAnimBlender *blender, char *name, bool unknown, float start, float end );
	int	BlendAnimation( TGAnimBlender *blender, float unknown, float unknown2, bool flag, bool flag2, bool flag3, char *name );
	int	UseDefault( TGAnimBlender *blender, char *name );
	float	GetAnimationLength( char *name );
	bool	IsLoaded( char *name );
	void	ClearAnimations( void );
	int	PositionUsingAnimation( TGAnimBlender *blender, char *name );
	// The SAT_* type of the animation; SAT_NORMAL when it is not loaded
	// (name invented).
	int	GetAnimationType( char *name );
	// The saved game: each animation's name and file, and the animations
	// loaded again from them (invented names).
	bool	WriteToStream( TGStream *stream );
	bool	ReadFromStream( TGStream *stream );

protected:
	SortedArray<TGAnimationEntry, char *>	animations;		// 0x00 owned, by name (invented name)
	NiTStringMap<char *>					files;				// 0x0C owned copies of the loaded animations' files, by name (invented name)
};

// The game's animation manager (the SWIG variable of that name).
extern TGAnimationManagerClass	g_kAnimationManager;

#endif /* !__TGANIMATIONMANAGERCLASS_H__ */
