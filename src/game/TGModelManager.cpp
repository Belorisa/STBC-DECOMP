/*
===============================================================================

	TGModelManager.cpp

===============================================================================
*/

#include <iostream>
#include <string.h>

#include "TGModelManager.h"
#include "ModelHelpers.h"
#include "TGStream.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006c4700 _$E45
// FUNCTION: STBC 0x006c4710 _$E44
// FUNCTION: STBC 0x006c4720 _$E60
// FUNCTION: STBC 0x006c4730 _$E59
// FUNCTION: STBC 0x006c4740 _$E64
// FUNCTION: STBC 0x006c4750 _$E63
// FUNCTION: STBC 0x006c4790 _$E68
// FUNCTION: STBC 0x006c47a0 _$E67
// FUNCTION: STBC 0x006c47b0 _$E72
// FUNCTION: STBC 0x006c47c0 _$E71
// FUNCTION: STBC 0x006c47d0 _$E196
// FUNCTION: STBC 0x006c47e0 _$E195
// FUNCTION: STBC 0x006c47f0 _$E201
// FUNCTION: STBC 0x006c4800 _$E200
// FUNCTION: STBC 0x006c4810 _$E210
// FUNCTION: STBC 0x006c4820 _$E209
// FUNCTION: STBC 0x006c4830 _$E49
// FUNCTION: STBC 0x006c4840 _$E48
// FUNCTION: STBC 0x006c4850 _$E138
// FUNCTION: STBC 0x006c4860 _$E137
// FUNCTION: STBC 0x006c4870 _$E151
// FUNCTION: STBC 0x006c4880 _$E150
// FUNCTION: STBC 0x006c4890 _$E206
// FUNCTION: STBC 0x006c48a0 _$E205
// FUNCTION: STBC 0x006c48b0 _$E214
// FUNCTION: STBC 0x006c48c0 _$E213
// FUNCTION: STBC 0x006c48d0 _$E221
// FUNCTION: STBC 0x006c48e0 _$E220

// The standard streams' initialisers and exit-time destructors (<iostream>):
// retail lays them out after the unit's code, among its template copies.
// FUNCTION: STBC 0x006c4430 _$E32
// FUNCTION: STBC 0x006c4440 _$E29
// FUNCTION: STBC 0x006c4450 _$E31
// FUNCTION: STBC 0x006c4460 _$E30
// FUNCTION: STBC 0x006c44e0 _$E38
// FUNCTION: STBC 0x006c44f0 _$E35
// FUNCTION: STBC 0x006c4500 _$E37
// FUNCTION: STBC 0x006c4510 _$E36

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x006c4760 _$E235
// FUNCTION: STBC 0x006c4780 _$E234

// Its construction and exit-time destruction.
// FUNCTION: STBC 0x006c3200 _$E228
// FUNCTION: STBC 0x006c3210 _$E225
// FUNCTION: STBC 0x006c3220 _$E227
// FUNCTION: STBC 0x006c3230 _$E226
// DATA: STBC 0x009976b0
TGModelManager g_kModelManager;

/*
================
TGModelManager::~TGModelManager

Deletes the queued entries; the members' destructors delete the loaded
ones.
================
*/
// FUNCTION: STBC 0x006c32b0 ??1TGModelManager@@QAE@XZ
TGModelManager::~TGModelManager( void ) {
	ClearIncrementalLoadQueue();
}

/*
================
TGModelManager::Unknown6C3440

A loaded model gains a user; a queued one is read to the end now;
otherwise a new entry reads the file. The model goes to *model.
================
*/
// FUNCTION: STBC 0x006c3440 ?Unknown6C3440@TGModelManager@@QAE_NPAD0PAPAVTGAnimNode@@0@Z
bool TGModelManager::Unknown6C3440( char *file, char *name, TGAnimNode **model, char *texturePath ) {
	// MATCH-PENDING: R17, exact only through the invented inline TGModelContainer::AddUser; its existence rests on the inline budget and the schedule alone, no outside evidence (user's ruling, 2026-11-22, as TGObject's sprintf helpers)
	TGModelContainer *entry = entries.Get( file );
	if ( entry != NULL ) {
		entry->AddUser();
		*model = entry->model;
		return true;
	}

	NiTListIterator pos = queue.GetHeadPos();
	while ( pos ) {
		TGModelContainer *queued = queue.Get( pos );
		if ( strcmp( queued->GetFile(), file ) == 0 ) {
			queue.RemovePos( pos );
			while ( queued->Unknown6C9300() == true ) {
			}
			if ( queued->model != NULL ) {
				queued->AddUser();
				*model = queued->model;
				entries.Insert( queued );
				return true;
			}
			delete queued;
			return false;
		}
		pos = queue.GetNextPos( pos );
	}

	entry = new TGModelContainer( this, file, name, texturePath );
	if ( entry->Load( NULL, NULL ) ) {
		entries.Insert( entry );
		entry->AddUser();
		*model = entry->model;
		return true;
	}
	delete entry;
	*model = NULL;
	return false;
}

/*
================
TGModelManager::LoadModelIncremental
================
*/
// FUNCTION: STBC 0x006c37c0 ?LoadModelIncremental@TGModelManager@@QAEXPAD00@Z
void TGModelManager::LoadModelIncremental( char *file, char *name, char *texturePath ) {
	// MATCH-PENDING: R17, exact only through the invented inline TGModelContainer::AddUser; its existence rests on the inline budget and the schedule alone, no outside evidence (user's ruling, 2026-11-22, as TGObject's sprintf helpers)
	TGModelContainer *entry = entries.Get( file );
	if ( entry != NULL ) {
		entry->AddUser();
		return;
	}

	NiTListIterator pos = queue.GetHeadPos();
	while ( pos ) {
		TGModelContainer *queued = queue.GetNext( pos );
		if ( strcmp( queued->GetFile(), file ) == 0 ) {
			queued->AddUser();
			return;
		}
	}

	entry = new TGModelContainer( this, file, name, texturePath );
	entry->AddUser();
	queue.AddTail( entry );
}

/*
================
TGModelManager::IsModelLoaded
================
*/
// FUNCTION: STBC 0x006c3900 ?IsModelLoaded@TGModelManager@@QAE_NPAD@Z
bool TGModelManager::IsModelLoaded( char *file ) {
	return entries.Get( file ) != NULL;
}

/*
================
TGModelManager::LoadModel

Reads the model file, or adds a user to it when it is already loaded.
================
*/
// FUNCTION: STBC 0x006c3970 ?LoadModel@TGModelManager@@QAE_NPAD00@Z
bool TGModelManager::LoadModel( char *file, char *name, char *texturePath ) {
	TGAnimNode *model = NULL;
	return Unknown6C3440( file, name, &model, texturePath );
}

/*
================
TGModelManager::Unrefer

Takes a user off the model; the count stops at zero.
================
*/
// FUNCTION: STBC 0x006c39a0 ?Unrefer@TGModelManager@@QAEXPAD@Z
void TGModelManager::Unrefer( char *file ) {
	TGModelContainer *entry = entries.Get( file );
	if ( entry != NULL && entry->users > 0 ) {
		entry->users--;
	}
}

/*
================
TGModelManager::Refer

Adds a user to a model that has one.
================
*/
// FUNCTION: STBC 0x006c3a10 ?Refer@TGModelManager@@QAEXPAD@Z
void TGModelManager::Refer( char *file ) {
	TGModelContainer *entry = entries.Get( file );
	if ( entry != NULL && entry->users > 0 ) {
		entry->AddUser();
	}
}

/*
================
TGModelManager::UnreferAll

Lets every user of the model go.
================
*/
// FUNCTION: STBC 0x006c3a80 ?UnreferAll@TGModelManager@@QAEXPAD@Z
void TGModelManager::UnreferAll( char *file ) {
	TGModelContainer *entry = entries.Get( file );
	if ( entry != NULL && entry->users > 0 ) {
		entry->users = 0;
	}
}

/*
================
TGModelManager::AddModel

Files a model the game built under the file name, with one user.
================
*/
// FUNCTION: STBC 0x006c3af0 ?AddModel@TGModelManager@@QAE_NPAVTGAnimNode@@PAD@Z
bool TGModelManager::AddModel( TGAnimNode *model, char *file ) {
	TGModelContainer *entry = new TGModelContainer( NULL, NULL, NULL, NULL );
	entry->SetFile( file );
	entry->model = model;
	entry->AddUser();
	entries.Insert( entry );
	return true;
}

/*
================
TGModelManager::FreeModel

Deletes the model of the file, whoever still uses it.
================
*/
// FUNCTION: STBC 0x006c3c30 ?FreeModel@TGModelManager@@QAEXPAD@Z
void TGModelManager::FreeModel( char *file ) {
	int index = entries.Find( file );
	if ( index >= 0 ) {
		entries.DeleteAt( index );
	}
}

/*
================
TGModelManager::Purge

Deletes the models that have no user left.
================
*/
// FUNCTION: STBC 0x006c3cf0 ?Purge@TGModelManager@@QAEXXZ
void TGModelManager::Purge( void ) {
	int count = entries.GetCount();
	TGModelContainer **items = entries.GetItems();
	for ( int i = 0; i < count; i++ ) {
		if ( items[i]->users <= 0 ) {
			entries.DeleteAt( i );
			i--;
			count--;
		}
	}
}

/*
================
TGModelManager::GetModel

The model of the file, read first when it is not loaded.
================
*/
// FUNCTION: STBC 0x006c3da0 ?GetModel@TGModelManager@@QAEPAVTGAnimNode@@PAD@Z
TGAnimNode *TGModelManager::GetModel( char *file ) {
	TGModelContainer *entry = entries.Get( file );
	if ( entry != NULL ) {
		return entry->model;
	}
	TGAnimNode *model;
	Unknown6C3440( file, NULL, &model, NULL );
	return model;
}

/*
================
TGModelManager::GetCamera

The camera of the file's model, read first when it is not loaded.
================
*/
// FUNCTION: STBC 0x006c3e20 ?GetCamera@TGModelManager@@QAEPAVNiCamera@@PAD@Z
NiCamera *TGModelManager::GetCamera( char *file ) {
	TGModelContainer *entry = entries.Get( file );
	if ( entry != NULL ) {
		return entry->camera;
	}
	LoadModel( file );
	entry = entries.Get( file );
	if ( entry != NULL ) {
		return entry->camera;
	}
	return NULL;
}

/*
================
TGModelManager::CloneCamera
================
*/
// FUNCTION: STBC 0x006c3ee0 ?CloneCamera@TGModelManager@@QAEPAVNiCamera@@PAD@Z
NiCamera *TGModelManager::CloneCamera( char *file ) {
	NiCamera *camera = GetCamera( file );
	if ( camera != NULL ) {
		return (NiCamera *)camera->Clone();
	}
	return NULL;
}

/*
================
TGModelManager::CloneModel
================
*/
// FUNCTION: STBC 0x006c3f00 ?CloneModel@TGModelManager@@QAEPAVTGAnimNode@@PAD@Z
TGAnimNode *TGModelManager::CloneModel( char *file ) {
	TGAnimNode *model = GetModel( file );
	if ( model != NULL ) {
		return (TGAnimNode *)model->Clone();
	}
	return NULL;
}

/*
================
TGModelManager::CopyModel

A clone of the model whose animation is copied from the original.
================
*/
// FUNCTION: STBC 0x006c3f20 ?CopyModel@TGModelManager@@QAEPAVTGAnimNode@@PAD@Z
TGAnimNode *TGModelManager::CopyModel( char *file ) {
	TGAnimNode *model = GetModel( file );
	if ( model != NULL ) {
		TGAnimNode *copy = (TGAnimNode *)model->Clone();
		UnshareTriShapeData( copy, false );
		return copy;
	}
	return NULL;
}

/*
================
TGModelManager::FreeAllModels

Deletes every model and empties the queue.
================
*/
// FUNCTION: STBC 0x006c3f50 ?FreeAllModels@TGModelManager@@QAEXXZ
void TGModelManager::FreeAllModels( void ) {
	entries.DeleteAll();
	unknown00.RemoveAll();
	ClearIncrementalLoadQueue();
}

/*
================
TGModelManager::ClearIncrementalLoadQueue

Deletes the entries still waiting to be read.
================
*/
// FUNCTION: STBC 0x006c4010 ?ClearIncrementalLoadQueue@TGModelManager@@QAEXXZ
void TGModelManager::ClearIncrementalLoadQueue( void ) {
	NiTListIterator pos = queue.GetHeadPos();
	while ( pos != NULL ) {
		TGModelContainer *entry = queue.GetNext( pos );
		delete entry;
	}
	queue.RemoveAll();
}

/*
================
TGModelManager::DoIncrementalLoad

Reads a step of the model at the head of the queue. A model that is done
joins the array. True while there is more to read.
================
*/
// FUNCTION: STBC 0x006c4080 ?DoIncrementalLoad@TGModelManager@@QAE_NXZ
bool TGModelManager::DoIncrementalLoad( void ) {
	if ( queue.GetSize() == 0 ) {
		return false;
	}
	TGModelContainer *entry = queue.GetHead();
	if ( !entry->Unknown6C9300() ) {
		queue.RemoveHead();
		entries.Insert( entry );
	}
	return queue.GetSize() > 0;
}

/*
================
TGModelManager::Unknown6C41A0
================
*/
// FUNCTION: STBC 0x006c41a0 ?Unknown6C41A0@TGModelManager@@QAEXXZ
void TGModelManager::Unknown6C41A0( void ) {
	TGModelContainer **items = entries.GetItems();
	int count = entries.GetCount();
	for ( int i = 0; i < count; i++ ) {
		if ( items[i] != NULL && items[i]->model != NULL ) {
			TouchTextureImages( items[i]->model );
		}
	}
}

/*
================
TGModelManager::WriteToStream

Writes the queue, then the loaded entries: a count, then a flag per entry
and the entry when the flag is set.
================
*/
// FUNCTION: STBC 0x006c41d0 ?WriteToStream@TGModelManager@@QAE_NPAVTGStream@@@Z
bool TGModelManager::WriteToStream( TGStream *stream ) {
	stream->WriteLong( queue.GetSize() );
	NiTListIterator pos = queue.GetHeadPos();
	while ( pos != NULL ) {
		TGModelContainer *entry = queue.GetNext( pos );
		if ( entry != NULL ) {
			stream->WriteChar( 1 );
			entry->WriteToStream( stream );
		} else {
			stream->WriteChar( 0 );
		}
	}

	TGModelContainer **items = entries.GetItems();
	int count = entries.GetCount();
	stream->WriteLong( count );
	for ( int i = 0; i < count; i++ ) {
		if ( items[i] != NULL ) {
			stream->WriteChar( 1 );
			items[i]->WriteToStream( stream );
		} else {
			stream->WriteChar( 0 );
		}
	}
	return true;
}

/*
================
TGModelManager::ReadFromStream

Reads what WriteToStream wrote; false when an entry fails to read.
================
*/
// FUNCTION: STBC 0x006c4260 ?ReadFromStream@TGModelManager@@QAE_NPAVTGStream@@@Z
bool TGModelManager::ReadFromStream( TGStream *stream ) {
	int count = stream->ReadLong();
	int i;
	for ( i = 0; i < count; i++ ) {
		if ( stream->ReadChar() ) {
			// BUG: the queued entry is read and then dropped: it joins no list and is never deleted
			TGModelContainer *entry = new TGModelContainer( this, NULL, NULL, NULL );
			if ( !entry->ReadFromStream( stream ) ) {
				return false;
			}
		}
	}

	count = stream->ReadLong();
	for ( i = 0; i < count; i++ ) {
		if ( stream->ReadChar() ) {
			TGModelContainer *entry = new TGModelContainer( this, NULL, NULL, NULL );
			if ( !entry->ReadFromStream( stream ) ) {
				return false;
			}
			entries.Insert( entry );
		}
	}
	return true;
}
