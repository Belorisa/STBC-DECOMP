/*
===============================================================================

	TGModelContainer.h

	A model the model manager holds: the file it is read from, the node
	inside it and the texture path, the loaded camera and model, and how
	many users have asked for it. The class name is retail's: the trace
	string "TGModelContainer::ExtractModel() expects only one object, others
	will be ignored.\n" (0x95ABDC) is printed from ExtractModel (0x6C9520).
	The members are named from how the manager uses them.

	0x20 bytes, from the game's pools. Evidence: the manager's loader
	(0x6C3440) allocates one from the pool of size 0x20 and calls the
	constructor (0x6C8E50) with the manager, the file, the node name and the
	texture path; the destructor (0x6C8EE0) frees the three strings, the
	NiStream (0x74 bytes) and releases the two references; 0x6C8FA0 replaces
	the file and drops the stream. The manager's sorted array orders the
	entries by file (0x6C9780 compares two entries, 0x6C9790 compares one
	with a file name); GetCamera reads the first member and GetModel the
	member at 0x10.

===============================================================================
*/

#ifndef __TGMODELCONTAINER_H__
#define __TGMODELCONTAINER_H__

#include "NiPointer.h"
#include "NiCamera.h"
#include "TGAnimNode.h"
#include "TGPoolManager.h"

class NiStream;
class TGModelManager;
class TGStream;

class TGModelContainer {
public:
	TGPOOL_ALLOCATION

					TGModelContainer( TGModelManager *manager_, char *file_, char *nodeName_, char *texturePath_ );
					~TGModelContainer( void );

	// Replaces the file name with a copy and drops the stream.
	void			SetFile( char *file_ );
	void			SetNodeName( char *nodeName_ );
	void			SetTexturePath( char *texturePath_ );

	// The order of the manager's array: by file.
	char *			GetFile( void ) { return file; }
	int				Compare( TGModelContainer *other );
	int				CompareKey( char *key );

	// Reads the file at once, with the texture path (each replaces the
	// entry's when given); false when it can't be read (invented name).
	bool			Load( char *file_, char *texturePath_ );
	// Reads the file a step further; false once it is done.
	bool			Unknown6C9300( void );
	// Wraps the stream's first object in a new model and takes its
	// second as the camera; false without objects. Named by its trace
	// message (TGModelContainer::ExtractModel).
	bool			ExtractModel( NiStream *stream_ );

	// The save and load of the entry (invented names): the file, the
	// texture path, the node name and the users. Only the manager's
	// WriteToStream and ReadFromStream call them.
	bool			WriteToStream( TGStream *stream_ );
	bool			ReadFromStream( TGStream *stream_ );

	// The bytes a step of the incremental read reads; 0 lets the stream
	// choose (invented name; nothing sets it).
	static unsigned int	loadStepSize;

	// One more user of the model (invented name). Its existence is shown by
	// the manager's code: retail calls SortedArray::Find out of line in
	// LoadModelIncremental, which needs one more inline site than the bare
	// users++ gives, and Unknown6C3440's model load waits for the users store
	// as it does behind an inlined member call.
	void			AddUser( void ) { users++; }

	NiPointer<NiCamera>		camera;			// 0x00
	char *			file;				// 0x04 owned copy
	char *			nodeName;			// 0x08 owned copy
	char *			texturePath;		// 0x0C owned copy
	NiPointer<TGAnimNode>	model;			// 0x10
	int				users;				// 0x14
	TGModelManager *	manager;			// 0x18
	NiStream *		stream;				// 0x1C owned, while the file is being read
};

#endif /* !__TGMODELCONTAINER_H__ */
