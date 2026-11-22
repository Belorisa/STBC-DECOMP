// NiObjectNET.cpp -- named objects with extra data and time controllers

#include "NiRTLib.h"
#include <assert.h>
#include <string.h>

#include "NiSystem.h"
#include "NiObjectNET.h"
#include "NiCloneExtraData.h"
#include "NiStream.h"
#include "NiAVObject.h"
#include "NiDynamicEffect.h"
#include "NiNode.h"
#include "NiProperty.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007db9f0 _$E200
// FUNCTION: STBC 0x007db990 _$E17
// FUNCTION: STBC 0x007db9b0 _$E15
// FUNCTION: STBC 0x007db9c0 _$E23
// FUNCTION: STBC 0x007db9e0 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007dba10 _$E30
// FUNCTION: STBC 0x007dba20 _$E41
// FUNCTION: STBC 0x007dba30 _$E45
// FUNCTION: STBC 0x007dba70 _$E49

// FUNCTION: STBC 0x007daaf0 _$E193
// FUNCTION: STBC 0x007dab10 _$E190
// DATA: STBC 0x9A1500 ?rtti@NiObjectNET@@2VNiRTTI@@B
const NiRTTI NiObjectNET::rtti( "NiObjectNET", &NiObject::rtti );

/*
================
NiObjectNET::Register
================
*/
// FUNCTION: STBC 0x7DAB30 ?Register@NiObjectNET@@SAHXZ
NiImplementRegister( NiObjectNET )

/*
================
NiObjectNET::NiObjectNET
================
*/
// FUNCTION: STBC 0x7DAC80
NiObjectNET::NiObjectNET() {
	name = 0;
	extra = 0;
	controllers = 0;
}

/*
================
NiObjectNET::~NiObjectNET
================
*/
// FUNCTION: STBC 0x7DAD10
NiObjectNET::~NiObjectNET() {
	delete [] name;
	RemoveAllExtraData();
	RemoveAllControllers();
}

/*
================
NiObjectNET::CreateClone

NiObjectNET itself is never cloned; the derived classes create the clone.
================
*/
// FUNCTION: STBC 0x7DAE00
NiObject *NiObjectNET::CreateClone() {
	assert( false );
	return 0;
}

/*
================
NiObjectNET::CopyMembers

The name by the copy type, clones of the extra data blocks (in reverse
order) and of the controllers. The original is marked with its clone unless
it already carries the mark.
================
*/
// FUNCTION: STBC 0x7DAE10
void NiObjectNET::CopyMembers( NiObjectNET *dest ) {
	NiObject::CopyMembers( dest );
	if ( defaultCopyType == COPY_EXACT ) {
		dest->SetName( GetName() );
	} else if ( defaultCopyType == COPY_UNIQUE && GetName() ) {
		unsigned int length = strlen( GetName() ) + 2;
		char *buffer = new char[length];	// (name invented; fits the /Od frame order)
		assert( buffer );
		strcpy( buffer, GetName() );
		buffer[length - 2] = defaultAppendCharacter;
		buffer[length - 1] = 0;
		dest->SetName( buffer );
		delete [] buffer;
	}

	// AnimOnly.exe 0x4133B5 (NiObjectNET.cpp) walks the list with a for
	// loop; retail's inline budget needs the while loop's cost
	if ( extra ) {
		NiExtraData *extraData = extra;
		while ( extraData ) {
			dest->AddExtraData( (NiExtraData *)extraData->CreateClone() );
			extraData = extraData->GetNext();
		}
	}

	NiExtraData *marker = GetExtraData();
	if ( !marker || !NiIsExactKindOf( NiCloneExtraData, marker ) ) {
		marker = new NiCloneExtraData( dest );
		assert( marker );
		AddExtraData( marker );
	}

	if ( controllers ) {
		dest->controllers = (NiTimeController *)controllers->CreateClone();
	}
}

/*
================
NiObjectNET::ProcessClone

Lets the controllers fix up their clones.
================
*/
// FUNCTION: STBC 0x7DB060
void NiObjectNET::ProcessClone( NiObject *clone ) {
	assert( NiIsKindOf( NiObjectNET, clone ) );
	NiObject::ProcessClone( clone );
	if ( controllers ) {
		NiObjectNET *netClone = (NiObjectNET *)clone;	// (name invented; fits the /Od frame order)
		NiTimeController *controllerClone = netClone->GetControllers();
		assert( controllerClone );
		controllers->ProcessClone( controllerClone );
	}
}

/*
================
NiObjectNET::VirtualSlot03

Drops the NiCloneExtraData that cloning left at the head of the extra
data list, then passes the call on to the controllers.
================
*/
// FUNCTION: STBC 0x7DB080
void NiObjectNET::VirtualSlot03() {
	NiObject::VirtualSlot03();
	NiExtraData *extraData = GetExtraData();
	if ( extraData && NiIsExactKindOf( NiCloneExtraData, extraData ) ) {
		assert( extraData->GetRefCount() == 1 );
		RemoveExtraData( extraData );
	}
	if ( controllers ) {
		controllers->VirtualSlot03();
	}
}

/*
================
NiObjectNET::SetName

Keeps its own copy of the name; NULL clears it.
================
*/
// FUNCTION: STBC 0x7DB0B0
void NiObjectNET::SetName( const char *name_ ) {
	delete [] name;
	if ( name_ ) {
		name = new char[strlen( name_ ) + 1];
		assert( name );
		strcpy( name, name_ );
	} else {
		name = 0;
	}
}

/*
================
NiObjectNET::GetName
================
*/
// FUNCTION: STBC 0x7DB120
const char *NiObjectNET::GetName() const {
	return name;
}

/*
================
NiObjectNET::AddExtraData

Puts the block at the head of the list.
================
*/
// FUNCTION: STBC 0x7DB130
void NiObjectNET::AddExtraData( NiExtraData *extraData ) {
	assert( extraData );
	if ( extraData ) {
		extraData->SetNext( extra );
		extra = extraData;
	}
}

/*
================
NiObjectNET::SetExtraData

Replaces the whole list.
================
*/
// FUNCTION: STBC 0x7DB1A0
void NiObjectNET::SetExtraData( NiExtraData *extraData ) {
#ifdef _DEBUG
	if ( extra ) {
		if ( extra->GetRefCount() == 1 ) {
			NiOutputDebugString( "WARNING.  SetExtraData destroying list.\n" );
		}
	}
#endif
	extra = extraData;
}

/*
================
NiObjectNET::GetExtraData
================
*/
// FUNCTION: STBC 0x7DB1E0
NiExtraData *NiObjectNET::GetExtraData() const {
	return extra;
}

/*
================
NiObjectNET::RemoveExtraData

Unlinks the block from the list. The block may be released here when the
list held its last reference.
================
*/
// FUNCTION: STBC 0x7DB1F0
void NiObjectNET::RemoveExtraData( NiExtraData *extraData ) {
	if ( extraData ) {
		if ( extra == extraData ) {
			NiPointer<NiExtraData> save = extraData;
			extra = extraData->GetNext();
			extraData->SetNext( 0 );
		} else {
			NiExtraData *prev = extra;
			NiExtraData *curr = prev ? prev->GetNext() : 0;
			while ( curr && curr != extraData ) {
				prev = curr;
				curr = curr->GetNext();
			}
			if ( curr ) {
				NiPointer<NiExtraData> save = extraData;
				prev->SetNext( extraData->GetNext() );
				extraData->SetNext( 0 );
			}
		}
	}
}

/*
================
NiObjectNET::RemoveAllExtraData
================
*/
// FUNCTION: STBC 0x7DB360
void NiObjectNET::RemoveAllExtraData() {
	extra = 0;
}

/*
================
NiObjectNET::PrependController

Puts the controller at the head of the list.
================
*/
// FUNCTION: STBC 0x7DB390
void NiObjectNET::PrependController( NiTimeController *controller ) {
	assert( controller );
	if ( controller ) {
		controller->SetNext( controllers );
		controllers = controller;
	}
}

/*
================
NiObjectNET::SetControllers

Replaces the whole list.
================
*/
// FUNCTION: STBC 0x7DB400
void NiObjectNET::SetControllers( NiTimeController *controller ) {
#ifdef _DEBUG
	if ( controllers ) {
		if ( controllers->GetRefCount() == 1 ) {
			NiOutputDebugString( "WARNING.  SetControllers destroying list.\n" );
		}
	}
#endif
	controllers = controller;
}

/*
================
NiObjectNET::GetControllers
================
*/
// FUNCTION: STBC 0x7DB440
NiTimeController *NiObjectNET::GetControllers() const {
	return controllers;
}

/*
================
NiObjectNET::RemoveController

Unlinks the controller from the list. The controller may be released here
when the list held its last reference.
================
*/
// FUNCTION: STBC 0x7DB450
void NiObjectNET::RemoveController( NiTimeController *controller ) {
	if ( controller ) {
		if ( controllers == controller ) {
			NiPointer<NiTimeController> save = controller;
			controllers = controller->GetNext();
			controller->SetNext( 0 );
	#ifdef _DEBUG
			if ( controller->GetRefCount() == 1 ) {
				NiOutputDebugString( "WARNING.  RemoveController is destroying a controller.\n" );
			}
	#endif
		} else {
			// BUG: unlike RemoveExtraData, no test for an empty list: removing a
			// controller from an object without controllers reads through NULL
			NiTimeController *prev = controllers;
			NiTimeController *curr = prev->GetNext();
			while ( curr && curr != controller ) {
				prev = curr;
				curr = curr->GetNext();
			}
			if ( curr ) {
				NiPointer<NiTimeController> save = controller;
				prev->SetNext( controller->GetNext() );
				controller->SetNext( 0 );
	#ifdef _DEBUG
				if ( controller->GetRefCount() == 1 ) {
					NiOutputDebugString( "WARNING.  RemoveController is destroying a controller.\n" );
				}
	#endif
			}
		}
	}
}

/*
================
NiObjectNET::RemoveAllControllers
================
*/
// FUNCTION: STBC 0x7DB5B0
void NiObjectNET::RemoveAllControllers() {
	controllers = 0;
}

/*
================
NiObjectNET::CreateObject

NiObjectNET is never loaded on its own: no object is created.
================
*/
// FUNCTION: STBC 0x7DB5E0
void NiObjectNET::CreateObject( NiStream &stream, NiObject *&object ) {
}

/*
================
NiObjectNET::LoadBinary

NiObject's members, the name, then the link IDs of the list heads.
================
*/
// FUNCTION: STBC 0x7DB5F0
void NiObjectNET::LoadBinary( NiStream &stream ) {
	NiObject::LoadBinary( stream );
	stream.LoadCString( name );

	NiObjectNETLoadRecord *record = (NiObjectNETLoadRecord *)stream.GetLoadRecord();
	assert( record );
	NiStreamLoadBinary( stream, record->extraLinkID );
	NiStreamLoadBinary( stream, record->controllersLinkID );
}

/*
================
NiObjectNET::LinkObject

Resolves the link IDs of the list heads.
================
*/
// FUNCTION: STBC 0x7DB630
void NiObjectNET::LinkObject( NiStream &stream ) {
	NiObject::LinkObject( stream );

	NiObjectNETLoadRecord *record = (NiObjectNETLoadRecord *)stream.GetLoadRecord();
	assert( record );
	if ( record->extraLinkID ) {
		extra = (NiExtraData *)stream.GetObjectFromLinkID( record->extraLinkID );
	}
	if ( record->controllersLinkID ) {
		controllers = (NiTimeController *)stream.GetObjectFromLinkID( record->controllersLinkID );
	}
}

/*
================
NiObjectNET::RegisterStreamables

Registers the extra data and controller lists after the object.
================
*/
// FUNCTION: STBC 0x7DB6C0
bool NiObjectNET::RegisterStreamables( NiStream &stream ) {
	if ( !NiObject::RegisterStreamables( stream ) ) {
		return false;
	}
	if ( extra ) {
		extra->RegisterStreamables( stream );
	}
	if ( controllers ) {
		controllers->RegisterStreamables( stream );
	}
	return true;
}

/*
================
NiObjectNET::SaveBinary

The name, then the link IDs (addresses) of the list heads.
================
*/
// FUNCTION: STBC 0x7DB700
void NiObjectNET::SaveBinary( NiStream &stream ) {
	NiObject::SaveBinary( stream );
	stream.SaveCString( name );
	NiStreamSaveBinary( stream, extra );
	NiStreamSaveBinary( stream, controllers );
}

/*
================
NiObjectNET::IsEqual

The names, then the extra data blocks and the controllers pair by pair.
================
*/
// FUNCTION: STBC 0x7DB740
bool NiObjectNET::IsEqual( NiObject *object ) {
	if ( !NiObject::IsEqual( object ) ) {
		return false;
	}

	NiObjectNET *net = (NiObjectNET *)object;
	if ( ( name && !net->name ) || ( !name && net->name ) ) {
		return false;
	}
	if ( name && strcmp( name, net->name ) != 0 ) {
		return false;
	}

	NiExtraData *pExtra0 = extra;	// (name invented; fits the /Od frame order)
	NiExtraData *pExtra1 = net->extra;	// (name invented; fits the /Od frame order)
	if ( pExtra0->Count() != pExtra1->Count() ) {
		return false;
	}
	while ( pExtra0 && pExtra1 ) {
		if ( !pExtra0->IsEqual( pExtra1 ) ) {
			return false;
		}
		pExtra0 = pExtra0->GetNext();
		pExtra1 = pExtra1->GetNext();
	}

	NiTimeController *pControl0 = controllers;	// (name invented; fits the /Od frame order)
	NiTimeController *pControl1 = net->controllers;	// (name invented; fits the /Od frame order)
	if ( pControl0->ItemsInList() != pControl1->ItemsInList() ) {
		return false;
	}
	while ( pControl0 && pControl1 ) {
		if ( !pControl0->IsEqual( pControl1 ) ) {
			return false;
		}
		pControl0 = pControl0->GetNext();
		pControl1 = pControl1->GetNext();
	}
	return true;
}

/*
================
NiObjectNET::GetViewerStrings

NiObject's strings, the name, then the strings of every extra data block
and controller.
================
*/
// FUNCTION: STBC 0x7DB860
void NiObjectNET::GetViewerStrings( NiTArray<char *> *strings ) {
	NiObject::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
	strings->Add( NiGetViewerString( "m_pName", name ) );

	if ( extra ) {
		for ( NiExtraData *extraData = extra; extraData; extraData = extraData->GetNext() ) {
			extraData->GetViewerStrings( strings );
		}
	}
	if ( controllers ) {
		for ( NiTimeController *controller = controllers; controller; controller = controller->GetNext() ) {
			controller->GetViewerStrings( strings );
		}
	}
}
