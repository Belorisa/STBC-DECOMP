/*
===============================================================================

	TGModelManager.h

	The models of the game, read from their files once and shared: each is
	an entry (TGModelContainer) in an array sorted by file, with a count of its
	users. Models can be queued to be read a step at a time. There is one,
	the global g_kModelManager.

	0x34 bytes, not polymorphic.

	Evidence: the dynamic initialiser (0x6C3210) runs the constructor
	(0x6C3240), which builds a string map of 37 buckets at 0x00 (the vtable
	0x88B6A0 it shares with the icon manager's map), clears the array at
	0x10 and the queue's list at 0x1C and sets that list's block size, 2, at
	0x30. The map is emptied by FreeAllModels; nothing the wrappers reach
	puts anything into it. The SWIG wrappers TGModelManager_* name the
	members. The array and the queue are invented names.

===============================================================================
*/

#ifndef __TGMODELMANAGER_H__
#define __TGMODELMANAGER_H__

#include "NiTStringMap.h"
#include "NiTList.h"
#include "NiPointer.h"
#include "NiObject.h"
#include "SortedArray.h"
#include "TGModelContainer.h"

class NiImage;

class NiCamera;
class TGAnimNode;
class TGStream;

class TGModelManager {
public:
	// FUNCTION: STBC 0x006c3240 ??0TGModelManager@@QAE@XZ
					TGModelManager( void ) : queue( 2 ) {}
					~TGModelManager( void );

	// Loads the model file with the node of the name in it and its
	// textures from the path; false when it fails (the SWIG wrapper's
	// format "OO|OO").
	bool	LoadModel( char *file, char *name = NULL, char *texturePath = NULL );
	// LoadModel's queued form: the file joins the incremental load queue
	// (DoIncrementalLoad reads it), or its entry gains a user when it is
	// loaded or queued already (the SWIG wrapper's format "OO|OO").
	void	LoadModelIncremental( char *file, char *name = NULL, char *texturePath = NULL );
	bool	IsModelLoaded( char *file );
	bool	AddModel( TGAnimNode *model, char *file );
	TGAnimNode *	GetModel( char *file );
	NiCamera *	GetCamera( char *file );
	TGAnimNode *	CloneModel( char *file );
	NiCamera *	CloneCamera( char *file );
	TGAnimNode *	CopyModel( char *file );
	void	FreeModel( char *file );
	void	FreeAllModels( void );
	void	Refer( char *file );
	void	Unrefer( char *file );
	void	UnreferAll( char *file );
	void	Purge( void );
	bool	DoIncrementalLoad( void );
	void	ClearIncrementalLoadQueue( void );

	// Finds or reads the model of the file; the model goes to *model.
	bool	Unknown6C3440( char *file, char *name, TGAnimNode **model, char *texturePath );

	// Counts a change on the texture images of every loaded model
	// (TouchTextureImages); nothing in retail calls it.
	void	Unknown6C41A0( void );

	// The save and load of the queue and the loaded entries (invented
	// names, after the other managers'); nothing in retail calls them.
	bool	WriteToStream( TGStream *stream );
	bool	ReadFromStream( TGStream *stream );

	NiTStringMap<NiPointer<NiImage> >	unknown00;	// 0x00 the images the models' streams share, by file name (NiStream::SetImageMap)
	SortedArray<TGModelContainer, char *>	entries;	// 0x10 owned, by file
	NiTList<TGModelContainer *>		queue;			// 0x1C entries waiting to be read, owned
};


extern TGModelManager	g_kModelManager;

#endif /* !__TGMODELMANAGER_H__ */
