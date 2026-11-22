/*
===============================================================================

	TGModelPropertyInstance.cpp

===============================================================================
*/

#include <string.h>

#include "TGModelPropertyInstance.h"
#include "TGModelProperty.h"
#include "TGModelPropertyManager.h"
#include "TGStream.h"

/*
================
TGModelPropertyInstance::TGModelPropertyInstance

Takes a reference on the template and places a copy of it.
================
*/
// FUNCTION: STBC 0x006ed380 ??0TGModelPropertyInstance@@QAE@PAVTGModelProperty@@PAD@Z
TGModelPropertyInstance::TGModelPropertyInstance( TGModelProperty *template_, char *hardpointName_ ) {
	property = NULL;
	hardpointName = NULL;
	if ( hardpointName_ != NULL ) {
		hardpointName = new char[strlen( hardpointName_ ) + 1];
		strcpy( hardpointName, hardpointName_ );
	}

	originalTemplate = template_;
	if ( template_ != NULL ) {
		template_->IncRefCount();
	}
	property = template_->Duplicate();
}

/*
================
TGModelPropertyInstance::TGModelPropertyInstance
================
*/
// FUNCTION: STBC 0x006ed420 ??0TGModelPropertyInstance@@QAE@XZ
TGModelPropertyInstance::TGModelPropertyInstance( void ) {
	property = NULL;
	originalTemplate = NULL;
	hardpointName = NULL;
}

/*
================
TGModelPropertyInstance::~TGModelPropertyInstance
================
*/
// FUNCTION: STBC 0x006ed440 ??1TGModelPropertyInstance@@UAE@XZ
TGModelPropertyInstance::~TGModelPropertyInstance( void ) {
	if ( originalTemplate != NULL ) {
		originalTemplate->DecRefCount();
	}
	if ( hardpointName != NULL ) {
		delete[] hardpointName;
		hardpointName = NULL;
	}
	property->DecRefCount();
}

/*
================
TGModelPropertyInstance::UseOriginalTemplate

Goes back to the template the instance started from.
================
*/
// FUNCTION: STBC 0x006ed480 ?UseOriginalTemplate@TGModelPropertyInstance@@QAEXXZ
void TGModelPropertyInstance::UseOriginalTemplate( void ) {
	property->DecRefCount();
	property = originalTemplate;
	property->IncRefCount();
}

/*
================
TGModelPropertyInstance::SetProperty
================
*/
// FUNCTION: STBC 0x006ed4a0 ?SetProperty@TGModelPropertyInstance@@QAEXPAVTGModelProperty@@@Z
void TGModelPropertyInstance::SetProperty( TGModelProperty *property_ ) {
	if ( property != NULL ) {
		property->DecRefCount();
	}
	property = property_;
	if ( property != NULL ) {
		property->IncRefCount();
	}
}

/*
================
TGModelPropertyInstance::SetOriginalTemplate
================
*/
// FUNCTION: STBC 0x006ed4d0 ?SetOriginalTemplate@TGModelPropertyInstance@@QAEXPAVTGModelProperty@@@Z
void TGModelPropertyInstance::SetOriginalTemplate( TGModelProperty *template_ ) {
	if ( originalTemplate != NULL ) {
		originalTemplate->DecRefCount();
	}
	originalTemplate = template_;
	if ( originalTemplate != NULL ) {
		originalTemplate->IncRefCount();
	}
}

/*
================
TGModelPropertyInstance::WriteToStream

A template the manager keeps is written by its ID, any other whole; the
property follows the same way unless it is the template itself.
================
*/
// FUNCTION: STBC 0x006ed500
bool TGModelPropertyInstance::WriteToStream( TGStream *stream ) {
	if ( g_kModelPropertyManager.IsGlobalTemplate( originalTemplate ) || g_kModelPropertyManager.IsLocalTemplate( originalTemplate ) ) {
		stream->WriteBool( true );
		stream->WriteID( TGObject::GetSavedID( originalTemplate ) );
	} else {
		stream->WriteBool( false );
		if ( !originalTemplate->WriteToStream( stream ) ) {
			return false;
		}
	}

	if ( property == originalTemplate ) {
		stream->WriteBool( true );
	} else {
		stream->WriteBool( false );
		property->WriteToStream( stream );
	}
	stream->VirtualSlot11( hardpointName );
	return true;
}

/*
================
TGModelPropertyInstance::ReadFromStream

The template is kept as its ID until PostFixUp.
================
*/
// FUNCTION: STBC 0x006ed5c0
bool TGModelPropertyInstance::ReadFromStream( TGStream *stream ) {
	if ( stream->ReadBool() == true ) {
		originalTemplate = (TGModelProperty *)stream->ReadID();
	} else {
		originalTemplate = (TGModelProperty *)TGObject::LoadObject( stream, 0 );
		originalTemplate = (TGModelProperty *)originalTemplate->GetObjID();
	}

	if ( stream->ReadBool() == true ) {
		property = originalTemplate;
	} else {
		property = (TGModelProperty *)TGObject::LoadObject( stream, 0 );
	}
	hardpointName = stream->VirtualSlot10( -1 );
	return true;
}

/*
================
TGModelPropertyInstance::FixUpPointers
================
*/
// FUNCTION: STBC 0x006ed630
bool TGModelPropertyInstance::FixUpPointers( void ) {
	if ( originalTemplate != NULL && TGObject::GetSavedObject( (long)originalTemplate ) == NULL ) {
		return false;
	}
	return true;
}

/*
================
TGModelPropertyInstance::PostFixUp

Turns the template's ID back into the template.
================
*/
// FUNCTION: STBC 0x006ed650
void TGModelPropertyInstance::PostFixUp( void ) {
	bool isTemplate = false;
	if ( property == originalTemplate ) {
		isTemplate = true;
	}
	originalTemplate = (TGModelProperty *)TGObject::GetSavedObject( (long)originalTemplate );
	if ( isTemplate ) {
		property = originalTemplate;
	}
}
