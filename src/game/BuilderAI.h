/*
===============================================================================

	BuilderAI.h

	The members of BuilderAI that the SWIG wrappers BuilderAI_* call, the
	data members the AI manager reads, and the first of its overrides.

	0x88 bytes: the load creator (0x4779B0) allocates 0x88 and calls the load
	constructor (0x477160), which reads a string into 0x4C, builds the list
	at 0x50 (two items a block) and the string maps at 0x68 and 0x78 (37
	buckets), and stores the vtable 0x88BBE0. AddAIBlock (0x4766C0) files a
	0x38-byte block record under its name in the map at 0x68 and appends
	the name to the list at 0x50; AIManager::UpdateAI (0x4722D0) updates a
	BuilderAI of a building AI tree for as long as that list holds names.

	The vtable 0x88BBE0 (23 slots) overrides PreprocessingAI's slots 0
	(0x476140), 15 to 20 (0x476100, 0x476F30, 0x477590, 0x4777D0, 0x476110,
	0x476120) and 22 (0x476B80); GetAIType returns 12, the type the AI
	manager's building pass looks for.

===============================================================================
*/

#ifndef __BUILDERAI_H__
#define __BUILDERAI_H__

#include "PreprocessingAI.h"
#include "NiTList.h"
#include "NiTStringMap.h"

// An AI block of a BuilderAI, defined in BuilderAI.cpp.
struct BuilderAIBlock;

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00476140 ??_GBuilderAI@@UAEPAXI@Z
class BuilderAI : public PreprocessingAI {
public:
					BuilderAI( PhysicsObjectClass *object, char *name_, char *moduleName_ );
					BuilderAI( TGStream *stream, bool *ok );
	virtual			~BuilderAI( void );

	// ArtificialIntelligence
	virtual PyObject *	VirtualSlot15( void );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	FixUpPointers( void );
	virtual void	PostFixUp( void );
	virtual int		GetAIType( void );
	virtual bool	VirtualSlot20( int type );

	// PreprocessingAI
	virtual int		CallPreprocessingMethod( double time );

	void	AddAIBlock( char *, char * );
	void	AddDependency( char *, char * );
	// Passes the Python object of the name to the block's function, after
	// the AIs of its dependencies (invented name: the Python method
	// BuilderAI_AddDependencyObject).
	void	AddDependencyObject( char *blockName, char *objectName, PyObject *object );

	// The stream constructor, for CreateFromStream.
	static ArtificialIntelligence *	Create( TGStream *stream );
	// The script-side constructor (the SWIG wrapper BuilderAI_Create calls it).
	// FUNCTION: STBC 0x00606a10 ?Create@BuilderAI@@SAPAV1@PAVPhysicsObjectClass@@PAD1@Z
	static BuilderAI *	Create( PhysicsObjectClass *object, char *name, char *moduleName ) { return new BuilderAI( object, name, moduleName ); }

	// Whether blocks remain to be built (invented name).
	bool	IsBuilding( void ) { return !pendingBlocks.IsEmpty(); }

protected:
	// Deletes the blocks and the trees of the AIs built for them, and
	// empties the block lists and the function map (invented name).
	void			DeleteBlocks( void );
	// Calls the block's function, passing the object's shadow, the AIs of
	// the block's dependencies and its dependency objects; the AI built,
	// NULL when a dependency is not built yet or the call fails (invented
	// name).
	ArtificialIntelligence *	BuildBlock( char *blockName );

	char *			moduleName;				// 0x4C owned: the script module of the block functions (invented name)
	NiTList<char *>	pendingBlocks;			// 0x50 owned names of the blocks added, still to be built (invented name)
	NiTStringMap<BuilderAIBlock *>	blocks;	// 0x68 owned, by name (invented name)
	NiTStringMap<PyObject *>	dependencyObjects;	// 0x78 by name; a reference held on each (invented name)
};

// The Python method BuilderAI_AddDependencyObject: the AI's shadow, the
// block's name, the object's name and the object.
PyObject *		BuilderAI_AddDependencyObject( PyObject *self, PyObject *args );

#endif /* !__BUILDERAI_H__ */
