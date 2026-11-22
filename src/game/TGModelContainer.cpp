/*
===============================================================================

	TGModelContainer.cpp

===============================================================================
*/

#include <iostream>
#include <string.h>

#include "TGModelContainer.h"
#include "NiStream.h"
#include "NiCamera.h"
#include "NiNode.h"
#include "TGModelManager.h"
#include "TGTrace.h"
#include "TGStream.h"

// The standard streams' initialisers and exit-time destructors (<iostream>),
// and the locale-id pair: retail lays them out after the unit's code.
// FUNCTION: STBC 0x006c9970 _$E32
// FUNCTION: STBC 0x006c9980 _$E29
// FUNCTION: STBC 0x006c9990 _$E31
// FUNCTION: STBC 0x006c99a0 _$E30
// FUNCTION: STBC 0x006c99b0 _$E38
// FUNCTION: STBC 0x006c99c0 _$E35
// FUNCTION: STBC 0x006c99d0 _$E37
// FUNCTION: STBC 0x006c99e0 _$E36
// FUNCTION: STBC 0x006c99f0 _$E231
// FUNCTION: STBC 0x006c9a10 _$E230

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006c9a20 _$E45
// FUNCTION: STBC 0x006c9a30 _$E44
// FUNCTION: STBC 0x006c9a40 _$E60
// FUNCTION: STBC 0x006c9a50 _$E59
// FUNCTION: STBC 0x006c9a60 _$E64
// FUNCTION: STBC 0x006c9a70 _$E63
// FUNCTION: STBC 0x006c9a80 _$E68
// FUNCTION: STBC 0x006c9a90 _$E67
// FUNCTION: STBC 0x006c9aa0 _$E72
// FUNCTION: STBC 0x006c9ab0 _$E71
// FUNCTION: STBC 0x006c9ac0 _$E196
// FUNCTION: STBC 0x006c9ad0 _$E195
// FUNCTION: STBC 0x006c9b00 _$E210
// FUNCTION: STBC 0x006c9b10 _$E209
// FUNCTION: STBC 0x006c9b20 _$E49
// FUNCTION: STBC 0x006c9b30 _$E48
// FUNCTION: STBC 0x006c9b40 _$E138
// FUNCTION: STBC 0x006c9b50 _$E137
// FUNCTION: STBC 0x006c9b60 _$E151
// FUNCTION: STBC 0x006c9b70 _$E150
// FUNCTION: STBC 0x006c9b80 _$E206
// FUNCTION: STBC 0x006c9b90 _$E205
// FUNCTION: STBC 0x006c9ba0 _$E214
// FUNCTION: STBC 0x006c9bb0 _$E213
// FUNCTION: STBC 0x006c9bc0 _$E221
// FUNCTION: STBC 0x006c9bd0 _$E220

/*
================
TGModelContainer::TGModelContainer

Keeps copies of the file, the node name and the texture path it is given.
================
*/
// FUNCTION: STBC 0x006c8e50 ??0TGModelContainer@@QAE@PAVTGModelManager@@PAD11@Z
TGModelContainer::TGModelContainer( TGModelManager *manager_, char *file_, char *nodeName_, char *texturePath_ )
	: camera( NULL ), model( NULL ) {
	manager = manager_;
	file = NULL;
	nodeName = NULL;
	texturePath = NULL;
	stream = NULL;
	users = 0;
	if ( file_ != NULL ) {
		SetFile( file_ );
	}
	if ( nodeName_ != NULL ) {
		SetNodeName( nodeName_ );
	}
	if ( texturePath_ != NULL ) {
		SetTexturePath( texturePath_ );
	}
}

/*
================
TGModelContainer::~TGModelContainer
================
*/
// FUNCTION: STBC 0x006c8ee0 ??1TGModelContainer@@QAE@XZ
TGModelContainer::~TGModelContainer( void ) {
	delete [] file;
	delete [] nodeName;
	delete [] texturePath;
	delete stream;
}

/*
================
TGModelContainer::SetFile
================
*/
// FUNCTION: STBC 0x006c8fa0 ?SetFile@TGModelContainer@@QAEXPAD@Z
void TGModelContainer::SetFile( char *file_ ) {
	delete [] file;
	if ( file_ != NULL ) {
		file = new char[strlen( file_ ) + 1];
		strcpy( file, file_ );
	} else {
		file = NULL;
	}
	delete stream;
	stream = NULL;
}

/*
================
TGModelContainer::SetTexturePath
================
*/
// FUNCTION: STBC 0x006c9020
void TGModelContainer::SetTexturePath( char *texturePath_ ) {
	delete [] texturePath;
	if ( texturePath_ != NULL ) {
		texturePath = new char[strlen( texturePath_ ) + 1];
		strcpy( texturePath, texturePath_ );
	} else {
		texturePath = NULL;
	}
}

/*
================
TGModelContainer::SetNodeName
================
*/
// FUNCTION: STBC 0x006c9090
void TGModelContainer::SetNodeName( char *nodeName_ ) {
	delete [] nodeName;
	if ( nodeName_ != NULL ) {
		nodeName = new char[strlen( nodeName_ ) + 1];
		strcpy( nodeName, nodeName_ );
	} else {
		nodeName = NULL;
	}
}

// DATA: STBC 0x009979f0
unsigned int TGModelContainer::loadStepSize;

/*
================
TGModelContainer::Load
================
*/
// FUNCTION: STBC 0x006c9100
bool TGModelContainer::Load( char *file_, char *texturePath_ ) {
	if ( file_ != NULL ) {
		SetFile( file_ );
	}
	if ( file == NULL ) {
		return false;
	}
	if ( texturePath_ != NULL ) {
		SetTexturePath( texturePath_ );
	}

	NiStream loader;
	if ( manager != NULL ) {
		loader.SetImageMap( &manager->unknown00 );
	}
	if ( texturePath != NULL && ( NiStream::searchPath == NULL || strcmp( NiStream::searchPath, texturePath ) != 0 ) ) {
		NiStream::SetSearchPath( texturePath );
	}
	if ( !loader.Load( file ) || !ExtractModel( &loader ) ) {
		return false;
	}
	if ( nodeName != NULL ) {
		model->SetRootNode( nodeName );
	}
	return true;
}

/*
================
TGModelContainer::Unknown6C9300

The first call opens the stream; each later one reads a step, and the
last makes the model.
================
*/
// FUNCTION: STBC 0x006c9300 ?Unknown6C9300@TGModelContainer@@QAE_NXZ
bool TGModelContainer::Unknown6C9300( void ) {
	if ( file != NULL ) {
		if ( stream == NULL ) {
			stream = new NiStream;
			if ( texturePath != NULL && ( NiStream::searchPath == NULL || strcmp( NiStream::searchPath, texturePath ) != 0 ) ) {
				NiStream::SetSearchPath( texturePath );
			}
			if ( manager != NULL ) {
				stream->SetImageMap( &manager->unknown00 );
			}
			return stream->BeginLoad( file );
		}

		if ( stream->LoadStep( loadStepSize ) == NiStream::LOAD_DONE ) {
			ExtractModel( stream );
			delete stream;
			stream = NULL;
			if ( nodeName != NULL && model != NULL ) {
				model->SetRootNode( nodeName );
			}
			return false;
		}
	}
	return true;
}

/*
================
TGModelContainer::ExtractModel

The model holds the first object, or the first node under it named
"Scene Root" or with more than one child slot.
================
*/
// FUNCTION: STBC 0x006c9520
bool TGModelContainer::ExtractModel( NiStream *stream_ ) {
	if ( stream_->GetObjectCount() <= 0 ) {
		return false;
	}
	if ( stream_->GetObjectCount() != 1 ) {
		TGTrace( TGTRACE_TYPE_MODEL, "TGModelContainer::ExtractModel() expects only one object, others will be ignored.\n" );
	}

	NiAVObject *object = NiDynamicCast( NiAVObject, stream_->GetObjectAt( 0 ) );
	if ( object == NULL ) {
		return false;
	}
	while ( true ) {
		// BUG: a node without children ends the walk with object NULL, and the next pass calls GetName on it
		if ( object->GetName() != NULL && strcmp( "Scene Root", object->GetName() ) == 0 ) {
			break;
		}
		NiNode *node = NiDynamicCast( NiNode, object );
		if ( node == NULL || node->GetArrayCount() > 1 ) {
			break;
		}
		object = node->GetAt( 0 );
	}

	model = new TGAnimNode;
	model->AttachChild( object, false );
	model->Unknown6C5680();
	NiObject::SetDefaultCopyType( NiObject::COPY_EXACT );
	model->Update( 0.0f );
	model->UpdateProperties();

	if ( stream_->GetObjectCount() > 1 ) {
		camera = NiDynamicCast( NiCamera, stream_->GetObjectAt( 1 ) );
		if ( camera != NULL ) {
			camera->SetTranslate( camera->GetWorldLocation() );
			NiMatrix3 rotate;
			rotate.SetCol( 0, camera->GetWorldDirection() );
			rotate.SetCol( 1, camera->GetWorldUpVector() );
			rotate.SetCol( 2, camera->GetWorldRightVector() );
			camera->SetRotate( rotate );
		}
	}
	return true;
}

/*
================
TGModelContainer::Compare

Orders two entries by their files.
================
*/
// FUNCTION: STBC 0x006c9780 ?Compare@TGModelContainer@@QAEHPAV1@@Z
int TGModelContainer::Compare( TGModelContainer *other ) {
	return CompareKey( other->GetFile() );
}

/*
================
TGModelContainer::CompareKey

Orders the entry's file against a file name.
================
*/
// FUNCTION: STBC 0x006c9790 ?CompareKey@TGModelContainer@@QAEHPAD@Z
int TGModelContainer::CompareKey( char *key ) {
	return strcmp( key, file );
}

/*
================
TGModelContainer::WriteToStream
================
*/
// FUNCTION: STBC 0x006c97d0 ?WriteToStream@TGModelContainer@@QAE_NPAVTGStream@@@Z
bool TGModelContainer::WriteToStream( TGStream *stream_ ) {
	stream_->VirtualSlot11( file );
	stream_->VirtualSlot11( texturePath );
	stream_->VirtualSlot11( nodeName );
	stream_->WriteLong( users );
	return true;
}

/*
================
TGModelContainer::ReadFromStream

Reads what WriteToStream wrote and loads the model, placing it by the node.
================
*/
// FUNCTION: STBC 0x006c9810 ?ReadFromStream@TGModelContainer@@QAE_NPAVTGStream@@@Z
bool TGModelContainer::ReadFromStream( TGStream *stream_ ) {
	// BUG: the file name is read and freed unused, so the entry loads with no file
	char *file_ = stream_->VirtualSlot10( -1 );
	// the stream's string is kept as is, not copied as SetTexturePath would
	texturePath = stream_->VirtualSlot10( -1 );
	char *nodeName_ = stream_->VirtualSlot10( -1 );
	SetNodeName( nodeName_ );
	Load( NULL, NULL );
	if ( nodeName_ != NULL && model != NULL ) {
		model->SetRootNode( nodeName_ );
	}
	delete nodeName_;
	delete file_;
	users = stream_->ReadLong();
	return true;
}
