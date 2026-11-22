/*
===============================================================================

	Backdrop.cpp

===============================================================================
*/

#include <iostream>
#include "Backdrop.h"
#include "SetClass.h"
#include "SetManager.h"
#include "TGStream.h"
#include "TGEventHandlerTable.h"
#include "TGModelManager.h"
#include "NiAlphaProperty.h"
#include "NiZBufferProperty.h"
#include "NiNode.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x0058cd20 _$E465
// FUNCTION: STBC 0x0058cd40 _$E464

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0058c430 _$E66
// FUNCTION: STBC 0x0058c440 _$E65
// FUNCTION: STBC 0x0058c450 _$E234
// FUNCTION: STBC 0x0058c460 _$E233
// FUNCTION: STBC 0x0058c470 _$E77
// FUNCTION: STBC 0x0058c480 _$E76
// FUNCTION: STBC 0x0058c490 _$E81
// FUNCTION: STBC 0x0058c4a0 _$E80
// FUNCTION: STBC 0x0058c4b0 _$E85
// FUNCTION: STBC 0x0058c4c0 _$E84
// FUNCTION: STBC 0x0058c4d0 _$E89
// FUNCTION: STBC 0x0058c4e0 _$E88
// FUNCTION: STBC 0x0058c4f0 _$E206
// FUNCTION: STBC 0x0058c500 _$E205
// FUNCTION: STBC 0x0058c510 _$E148
// FUNCTION: STBC 0x0058c520 _$E147
// FUNCTION: STBC 0x0058c530 _$E161
// FUNCTION: STBC 0x0058c540 _$E160
// FUNCTION: STBC 0x0058c550 _$E211
// FUNCTION: STBC 0x0058c560 _$E210
// FUNCTION: STBC 0x0058c570 _$E239
// FUNCTION: STBC 0x0058c580 _$E238
// FUNCTION: STBC 0x0058c590 _$E215
// FUNCTION: STBC 0x0058c5a0 _$E214
// FUNCTION: STBC 0x0058c5b0 _$E243
// FUNCTION: STBC 0x0058c5c0 _$E242
// FUNCTION: STBC 0x0058c5d0 _$E250
// FUNCTION: STBC 0x0058c5e0 _$E249
// FUNCTION: STBC 0x0058c5f0 _$E273
// FUNCTION: STBC 0x0058c600 _$E272
// FUNCTION: STBC 0x0058c610 _$E277
// FUNCTION: STBC 0x0058c620 _$E276
// FUNCTION: STBC 0x0058c630 _$E106
// FUNCTION: STBC 0x0058c640 _$E105
// FUNCTION: STBC 0x0058c650 _$E259
// FUNCTION: STBC 0x0058c660 _$E258
// FUNCTION: STBC 0x0058c670 _$E266
// FUNCTION: STBC 0x0058c680 _$E265
// FUNCTION: STBC 0x0058c690 _$E289
// FUNCTION: STBC 0x0058c6a0 _$E288
// FUNCTION: STBC 0x0058c760 _$E293
// FUNCTION: STBC 0x0058c770 _$E292
// FUNCTION: STBC 0x0058c910 _$E333
// FUNCTION: STBC 0x0058c920 _$E332
// FUNCTION: STBC 0x0058c930 _$E339
// FUNCTION: STBC 0x0058c940 _$E338
// FUNCTION: STBC 0x0058c950 _$E281
// FUNCTION: STBC 0x0058c960 _$E280
// FUNCTION: STBC 0x0058c970 _$E285
// FUNCTION: STBC 0x0058c980 _$E284
// FUNCTION: STBC 0x0058c990 _$E297
// FUNCTION: STBC 0x0058c9a0 _$E296
// FUNCTION: STBC 0x0058c9b0 _$E301
// FUNCTION: STBC 0x0058c9c0 _$E300
// FUNCTION: STBC 0x0058c9d0 _$E305
// FUNCTION: STBC 0x0058c9e0 _$E304
// FUNCTION: STBC 0x0058c9f0 _$E309
// FUNCTION: STBC 0x0058ca00 _$E308
// FUNCTION: STBC 0x0058ca10 _$E313
// FUNCTION: STBC 0x0058ca20 _$E312
// FUNCTION: STBC 0x0058ca30 _$E317
// FUNCTION: STBC 0x0058ca40 _$E316
// FUNCTION: STBC 0x0058ca50 _$E321
// FUNCTION: STBC 0x0058ca60 _$E320
// FUNCTION: STBC 0x0058ca70 _$E325
// FUNCTION: STBC 0x0058ca80 _$E324
// FUNCTION: STBC 0x0058cb60 _$E111
// FUNCTION: STBC 0x0058cb70 _$E110
// FUNCTION: STBC 0x0058cb80 _$E329
// FUNCTION: STBC 0x0058cb90 _$E328
// FUNCTION: STBC 0x0058cba0 _$E116
// FUNCTION: STBC 0x0058cbb0 _$E115
// FUNCTION: STBC 0x0058cbc0 _$E343
// FUNCTION: STBC 0x0058cbd0 _$E342
// FUNCTION: STBC 0x0058cbe0 _$E347
// FUNCTION: STBC 0x0058cbf0 _$E346
// FUNCTION: STBC 0x0058cc00 _$E351
// FUNCTION: STBC 0x0058cc10 _$E350
// FUNCTION: STBC 0x0058cc20 _$E355
// FUNCTION: STBC 0x0058cc30 _$E354
// FUNCTION: STBC 0x0058cc40 _$E125
// FUNCTION: STBC 0x0058cc50 _$E124
// FUNCTION: STBC 0x0058cc60 _$E359
// FUNCTION: STBC 0x0058cc70 _$E358
// FUNCTION: STBC 0x0058cc80 _$E363
// FUNCTION: STBC 0x0058cc90 _$E362
// FUNCTION: STBC 0x0058cca0 _$E367
// FUNCTION: STBC 0x0058ccb0 _$E366
// FUNCTION: STBC 0x0058ccc0 _$E131
// FUNCTION: STBC 0x0058ccd0 _$E130
// FUNCTION: STBC 0x0058cce0 _$E136
// FUNCTION: STBC 0x0058ccf0 _$E135
// FUNCTION: STBC 0x0058cd00 _$E166
// FUNCTION: STBC 0x0058cd10 _$E165
// FUNCTION: STBC 0x0058cd50 _$E371
// FUNCTION: STBC 0x0058cd60 _$E370
// FUNCTION: STBC 0x0058cd70 _$E171
// FUNCTION: STBC 0x0058cd80 _$E170
// FUNCTION: STBC 0x0058cd90 _$E375
// FUNCTION: STBC 0x0058cda0 _$E374
// FUNCTION: STBC 0x0058cdb0 _$E379
// FUNCTION: STBC 0x0058cdc0 _$E378
// FUNCTION: STBC 0x0058cdd0 _$E176
// FUNCTION: STBC 0x0058cde0 _$E175
// FUNCTION: STBC 0x0058cdf0 _$E383
// FUNCTION: STBC 0x0058ce00 _$E382
// FUNCTION: STBC 0x0058ce10 _$E141
// FUNCTION: STBC 0x0058ce20 _$E140
// FUNCTION: STBC 0x0058ce30 _$E387
// FUNCTION: STBC 0x0058ce40 _$E386
// FUNCTION: STBC 0x0058ce50 _$E391
// FUNCTION: STBC 0x0058ce60 _$E390
// FUNCTION: STBC 0x0058ce70 _$E181
// FUNCTION: STBC 0x0058ce80 _$E180
// FUNCTION: STBC 0x0058ce90 _$E186
// FUNCTION: STBC 0x0058cea0 _$E185
// FUNCTION: STBC 0x0058ceb0 _$E395
// FUNCTION: STBC 0x0058cec0 _$E394
// FUNCTION: STBC 0x0058ced0 _$E399
// FUNCTION: STBC 0x0058cee0 _$E398
// FUNCTION: STBC 0x0058cef0 _$E403
// FUNCTION: STBC 0x0058cf00 _$E402
// FUNCTION: STBC 0x0058cf10 _$E407
// FUNCTION: STBC 0x0058cf20 _$E406
// FUNCTION: STBC 0x0058cf30 _$E411
// FUNCTION: STBC 0x0058cf40 _$E410
// FUNCTION: STBC 0x0058cf50 _$E415
// FUNCTION: STBC 0x0058cf60 _$E414
// FUNCTION: STBC 0x0058cf70 _$E419
// FUNCTION: STBC 0x0058cf80 _$E418
// FUNCTION: STBC 0x0058cf90 _$E423
// FUNCTION: STBC 0x0058cfa0 _$E422
// FUNCTION: STBC 0x0058cfb0 _$E191
// FUNCTION: STBC 0x0058cfc0 _$E190
// FUNCTION: STBC 0x0058cfd0 _$E427
// FUNCTION: STBC 0x0058cfe0 _$E426
// FUNCTION: STBC 0x0058cff0 _$E196
// FUNCTION: STBC 0x0058d000 _$E195
// FUNCTION: STBC 0x0058d010 _$E201
// FUNCTION: STBC 0x0058d020 _$E200
// FUNCTION: STBC 0x0058d030 _$E431
// FUNCTION: STBC 0x0058d040 _$E430
// FUNCTION: STBC 0x0058d050 _$E436
// FUNCTION: STBC 0x0058d060 _$E435
// FUNCTION: STBC 0x0058d070 _$E440
// FUNCTION: STBC 0x0058d080 _$E439

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0058c3b0 _$E32
// FUNCTION: STBC 0x0058c3c0 _$E29
// FUNCTION: STBC 0x0058c3d0 _$E31
// FUNCTION: STBC 0x0058c3e0 _$E30
// FUNCTION: STBC 0x0058c3f0 _$E38
// FUNCTION: STBC 0x0058c400 _$E35
// FUNCTION: STBC 0x0058c410 _$E37
// FUNCTION: STBC 0x0058c420 _$E36

// The static initialisers of the unit's own statics: the handler table, the array of every backdrop and the map by name.
// FUNCTION: STBC 0x0058b070 _$E446
// FUNCTION: STBC 0x0058b080 _$E442
// FUNCTION: STBC 0x0058b090 _$E445
// FUNCTION: STBC 0x0058b0a0 _$E443
// FUNCTION: STBC 0x0058b270 _$E452
// FUNCTION: STBC 0x0058b280 _$E448
// FUNCTION: STBC 0x0058b2c0 _$E451
// FUNCTION: STBC 0x0058b2d0 _$E449
// FUNCTION: STBC 0x0058b300 _$E457
// FUNCTION: STBC 0x0058b310 _$E454
// FUNCTION: STBC 0x0058b320 _$E456
// FUNCTION: STBC 0x0058b330 _$E455

// DATA: STBC 0x0098d7e8
TGEventHandlerTable Backdrop::classHandlers;

/*
================
Backdrop::AddToSet

Adds the object to the set's list of its class type, and to those of its
base classes.
================
*/
// FUNCTION: STBC 0x0058b0c0
void Backdrop::AddToSet( SetClass *set ) {
	if ( set != NULL ) {
		set->objectTable.Add( this, BACKDROP_TYPE );
		ObjectClass::AddToSet( set );
	}
}

/*
================
Backdrop::RemoveFromSet
================
*/
// FUNCTION: STBC 0x0058b0f0
void Backdrop::RemoveFromSet( SetClass *set ) {
	if ( set != NULL ) {
		set->objectTable.Remove( this, BACKDROP_TYPE );
		ObjectClass::RemoveFromSet( set );
	}
}

/*
================
Backdrop::GetClassObjects

The set's objects of the class and their number; NULL and 0 when there
are none.
================
*/
// FUNCTION: STBC 0x0058b120
Backdrop **Backdrop::GetClassObjects( SetClass *set, int *count ) {
	if ( set != NULL ) {
		return (Backdrop **)set->objectTable.GetObjects( BACKDROP_TYPE, count );
	}
	*count = 0;
	return NULL;
}

/*
================
Backdrop::GetNumClassObjects

The number of the set's objects of the class.
================
*/
// FUNCTION: STBC 0x0058b150
int Backdrop::GetNumClassObjects( SetClass *set ) {
	if ( set != NULL ) {
		return set->objectTable.GetNumObjects( BACKDROP_TYPE );
	}
	return 0;
}

/*
================
Backdrop::GetObjectByID

The object with the given ID in the set, or among every object when the
set is NULL; NULL when it is none or not a Backdrop.
================
*/
// FUNCTION: STBC 0x0058b170
Backdrop *Backdrop::GetObjectByID( SetClass *set, long id ) {
	if ( set == NULL ) {
		TGObject *object;
		if ( TGObject::objectTable->GetAt( id, object ) && object->IsTypeOf( BACKDROP_TYPE ) ) {
			return (Backdrop *)object;
		}
		return NULL;
	}
	return (Backdrop *)set->GetClassObject( BACKDROP_TYPE, id );
}

/*
================
Backdrop::GetObject

The object of the class and name in the set, or in any set when the set
is NULL, the last set first.
================
*/
// FUNCTION: STBC 0x0058b1e0
Backdrop *Backdrop::GetObject( SetClass *set, char *name ) {
	if ( set == NULL ) {
		int count = g_kSetManager.GetNumSets();
		SetClass **sets = g_kSetManager.GetSets();
		BaseObjectClass *object = NULL;
		for ( int i = count - 1; i >= 0 && object == NULL; i-- ) {
			object = sets[i]->GetClassObject( BACKDROP_TYPE, name );
		}
		return (Backdrop *)object;
	}
	return (Backdrop *)set->GetClassObject( BACKDROP_TYPE, name );
}

/*
================
Backdrop::Cast

The object as a Backdrop; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0058b240
Backdrop *Backdrop::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( BACKDROP_TYPE ) ) {
		return (Backdrop *)object;
	}
	return NULL;
}

// DATA: STBC 0x0098d8d8 ?backdrops@Backdrop@@1V?$NiTArray@PAVBackdrop@@@@A
NiTArray<Backdrop *> Backdrop::backdrops;

// DATA: STBC 0x0098d868
NiTStringMap<Backdrop *> Backdrop::backdropsByName;

/*
================
Backdrop::Create
================
*/
// FUNCTION: STBC 0x0058b350
Backdrop *Backdrop::Create( void ) {
	return new Backdrop;
}

/*
================
Backdrop::Backdrop

Joins the array of every backdrop, at the end: the drawing order.
================
*/
// FUNCTION: STBC 0x0058b380
Backdrop::Backdrop( long id ) : ObjectClass( id ), modelName( NULL ) {
	collisionFlags = ObjectClass::CFB_NO_COLLISIONS;
	backdrops.Add( this );
	SetHailable( false );
	SetScannable( false );
}

/*
================
Backdrop::GetObjType
================
*/
// FUNCTION: STBC 0x0058b4f0
int Backdrop::GetObjType( void ) {
	return BACKDROP_TYPE;
}

/*
================
Backdrop::GetClassName
================
*/
// FUNCTION: STBC 0x0058b560
char *Backdrop::GetClassName( void ) {
	return "Backdrop";
}

/*
================
Backdrop::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x0058b570
char *Backdrop::GetSwigTypeName( void ) {
	return "_p_Backdrop";
}

/*
================
Backdrop::GetPythonClassName
================
*/
// FUNCTION: STBC 0x0058b580
char *Backdrop::GetPythonClassName( void ) {
	return "BackdropPtr";
}

/*
================
Backdrop::GetClassHandlers
================
*/
// FUNCTION: STBC 0x0058b590
TGEventHandlerTable *Backdrop::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
Backdrop::~Backdrop

Leaves the by-name map and the array of every backdrop.
================
*/
// FUNCTION: STBC 0x0058b5d0
Backdrop::~Backdrop( void ) {
	delete [] modelName;
	RemoveFromLists();
}

/*
================
Backdrop::SetModelName

Keeps a copy of the name, shows the model of that name, and gives it an
alpha property that the sorter leaves alone.
================
*/
// FUNCTION: STBC 0x0058b630
void Backdrop::SetModelName( char *name ) {
	delete [] modelName;
	modelName = new char[strlen( name ) + 1];
	strcpy( modelName, name );
	TGAnimNode *model = g_kModelManager.CloneModel( name );
	SetNiObject( model );
	RemovePropertyFromTree( model, NiZBufferProperty::ZBUFFER );
	NiAlphaProperty *alpha = new NiAlphaProperty;
	alpha->SetNoSorter( true );
	model->AttachProperty( alpha );
	SetNoSorterOnTree();
	model->UpdateProperties();
}

/*
================
Backdrop::SetContainingSet

Moves the backdrop's node from the old set's backdrop root to the new one's.
================
*/
// FUNCTION: STBC 0x0058b760
void Backdrop::SetContainingSet( SetClass *set ) {
	if ( containingSet != NULL ) {
		containingSet->GetBackdropRoot()->DetachChild( node );
	}
	containingSet = set;
	if ( set != NULL ) {
		set->GetBackdropRoot()->AttachChild( node, true );
		set->GetBackdropRoot()->SetAppCulled( false );
	}
}

/*
================
Backdrop::SetNoSorterOnTree
================
*/
// FUNCTION: STBC 0x0058b7e0
void Backdrop::SetNoSorterOnTree( NiAVObject *object ) {
	if ( object == NULL ) {
		object = niObject;
		if ( object == NULL ) {
			return;
		}
	}
	NiAlphaProperty *alpha = NiDynamicCast( NiAlphaProperty, object->GetProperty( NiAlphaProperty::ALPHA ) );
	if ( alpha ) {
		alpha->SetNoSorter( true );
	}
	NiNode *parent = NiDynamicCast( NiNode, object );
	if ( parent != NULL ) {
		for ( unsigned int i = 0; i < parent->GetArrayCount(); i++ ) {
			NiAVObject *child = parent->GetAt( i );
			if ( child != NULL ) {
				SetNoSorterOnTree( child );
			}
		}
	}
}

/*
================
Backdrop::RemoveFromLists
================
*/
// FUNCTION: STBC 0x0058b990
void Backdrop::RemoveFromLists( void ) {
	if ( name != NULL ) {
		backdropsByName.RemoveAt( name );
	}
	for ( unsigned int i = 0; i < backdrops.GetSize(); i++ ) {
		if ( backdrops.GetAt( i ) == this ) {
			backdrops.RemoveAt( i );
			backdrops.Compact();
			return;
		}
	}
}

/*
================
Backdrop::SetName

Renames the backdrop's entry of the by-name map.
================
*/
// FUNCTION: STBC 0x0058bb40
void Backdrop::SetName( char *name_ ) {
	if ( name != NULL ) {
		backdropsByName.RemoveAt( name );
	}
	ObjectClass::SetName( name_ );
	if ( name_ != NULL ) {
		Backdrop *existing;
		if ( !backdropsByName.GetAt( name_, existing ) ) {
			backdropsByName.SetAt( name_, this );
		}
	}
}



/*
================
Backdrop::MoveForward

Changes places with the backdrop before it, in the set's scene and in
the drawing order.
================
*/
// FUNCTION: STBC 0x0058bcf0
void Backdrop::MoveForward( void ) {
	SetClass *set = GetContainingSet();
	if ( set != NULL ) {
		set->Unknown40E6A0( this );
		for ( unsigned int i = 1; i < backdrops.GetSize(); i++ ) {
			Backdrop *current = backdrops.GetAt( i );
			if ( current == this ) {
				Backdrop *previous = backdrops.GetAt( i - 1 );
				backdrops.SetAt( i - 1, this );
				backdrops.SetAt( i, previous );
				return;
			}
		}
	}
}

/*
================
Backdrop::MoveBack

Changes places with the backdrop after it, in the set's scene and in the
drawing order.
================
*/
// FUNCTION: STBC 0x0058bdf0
void Backdrop::MoveBack( void ) {
	SetClass *set = GetContainingSet();
	if ( set != NULL ) {
		set->Unknown40E810( this );
		for ( unsigned int i = 0; i < backdrops.GetSize() - 1; i++ ) {
			Backdrop *current = backdrops.GetAt( i );
			if ( current == this ) {
				Backdrop *next = backdrops.GetAt( i + 1 );
				backdrops.SetAt( i + 1, this );
				backdrops.SetAt( i, next );
				return;
			}
		}
	}
}

/*
================
Backdrop::SaveToFile

Writes the backdrop's script lines to the file by calling its virtual slots 67, 68, and 69 in sequence.
Returns true if all slots succeed, false otherwise.
================
*/
// FUNCTION: STBC 0x0058bf10
bool Backdrop::SaveToFile( FILE *file ) {
	bool result = file != NULL;
	if ( result ) {
		result = VirtualSlot67( file );
		if ( result ) {
			result = VirtualSlot68( file );
			if ( result ) {
				result = VirtualSlot69( file );
			}
		}
	}
	return result;
}

/*
================
Backdrop::VirtualSlot67

The first lines of the backdrop's script: its name and its creation.
================
*/
// FUNCTION: STBC 0x0058bf50
bool Backdrop::VirtualSlot67( FILE *file ) {
	fprintf( file, "\t# Backdrop \"%s\"\n", name );
	fprintf( file, "\tkThis = App.Backdrop_Create()\n" );
	return true;
}

/*
================
Backdrop::VirtualSlot68

The lines that give the backdrop its name, model and placement.
================
*/
// FUNCTION: STBC 0x0058bf80
bool Backdrop::VirtualSlot68( FILE *file ) {
	fprintf( file, "\tkThis.SetName(\"%s\")\n", name );
	if ( modelName != NULL ) {
		fprintf( file, "\tkThis.SetModelName(\"%s\")\n", modelName );
	}
	NiPoint3 location = GetTranslate();
	fprintf( file, "\tkThis.SetTranslateXYZ(%f, %f, %f)\n", location.x, location.y, location.z );
	NiPoint3 forward;
	forward = GetWorldForward();
	fprintf( file, "\tkForward = App.TGPoint3()\n" );
	fprintf( file, "\tkForward.SetXYZ(%f, %f, %f)\n", forward.x, forward.y, forward.z );
	NiPoint3 up;
	up = GetWorldUp();
	fprintf( file, "\tkUp = App.TGPoint3()\n" );
	fprintf( file, "\tkUp.SetXYZ(%f, %f, %f)\n", up.x, up.y, up.z );
	fprintf( file, "\tkThis.AlignToVectors(kForward, kUp)\n" );
	return true;
}

/*
================
Backdrop::VirtualSlot69

The lines that put the backdrop in the set and close its script.
================
*/
// FUNCTION: STBC 0x0058c0b0
bool Backdrop::VirtualSlot69( FILE *file ) {
	fprintf( file, "\tpSet.AddBackdropToSet(kThis,\"%s\")\n", name );
	fprintf( file, "\tkThis.Update(0)\n" );
	fprintf( file, "\tkThis = None\n" );
	fprintf( file, "\t# End Backdrop \"%s\"\n\n", name );
	return true;
}

/*
================
Backdrop::MoveAllToSet

Moves every backdrop into the set, taking it out of any other set first.
================
*/
// FUNCTION: STBC 0x0058c100
void Backdrop::MoveAllToSet( SetClass *set ) {
	int count = backdrops.GetSize();
	Backdrop **list = backdrops.GetBase();
	for ( int i = 0; i < count; i++ ) {
		SetClass *current = list[i]->GetContainingSet();
		if ( current != NULL && current != set ) {
			current->RemoveBackdropFromSet( list[i] );
		}
		if ( current != set ) {
			set->AddBackdropToSet( list[i], list[i]->GetName() );
		}
	}
}

/*
================
Backdrop::RemoveAllFromSets

Takes every backdrop out of the set that holds it.
================
*/
// FUNCTION: STBC 0x0058c150
void Backdrop::RemoveAllFromSets( void ) {
	int count = backdrops.GetSize();
	Backdrop **list = backdrops.GetBase();
	for ( int i = 0; i < count; i++ ) {
		SetClass *set = list[i]->GetContainingSet();
		if ( set != NULL ) {
			set->RemoveBackdropFromSet( list[i] );
		}
	}
}

/*
================
Backdrop::DeleteAll

Takes every backdrop out of its set and deletes it, the last first.
================
*/
// FUNCTION: STBC 0x0058c180 ?DeleteAll@Backdrop@@SAXXZ
void Backdrop::DeleteAll( void ) {
	RemoveAllFromSets();
	int count = backdrops.GetSize();
	Backdrop **list = backdrops.GetBase();
	for ( int i = count - 1; i >= 0; i-- ) {
		if ( list[i] != NULL ) {
			delete list[i];
		}
	}
}

/*
================
Backdrop::SaveAll

Writes the LoadBackdrops function of every backdrop; false when the file
is NULL or a backdrop fails.
================
*/
// FUNCTION: STBC 0x0058c1c0
bool Backdrop::SaveAll( FILE *file ) {
	bool result = file != NULL;
	if ( result ) {
		fprintf( file, "import App\n\n" );
		fprintf( file, "def LoadBackdrops(pSet):\n\n" );
		fprintf( file, "\t#Draw order is implicit. First object gets drawn first\n\n" );
		int count = backdrops.GetSize();
		Backdrop **list = backdrops.GetBase();
		for ( int i = 0; result && i < count; i++ ) {
			result = list[i]->SaveToFile( file );
		}
	}
	return result;
}

/*
================
Backdrop::SaveSet

Writes the LoadBackdrops function of the set's backdrops; false when the
file is NULL or a backdrop fails.
================
*/
// FUNCTION: STBC 0x0058c230
bool Backdrop::SaveSet( SetClass *set, FILE *file ) {
	bool result = file != NULL;
	if ( result ) {
		fprintf( file, "import App\n\n" );
		fprintf( file, "def LoadBackdrops(pSet):\n\n" );
		fprintf( file, "\t#Draw order is implicit. First object gets drawn first\n\n" );
		int count = backdrops.GetSize();
		Backdrop **list = backdrops.GetBase();
		for ( int i = 0; result && i < count; i++ ) {
			if ( list[i]->GetContainingSet() == set ) {
				result = list[i]->SaveToFile( file );
			}
		}
	}
	return result;
}

/*
================
Backdrop::GetByName

NULL when no backdrop has the name.
================
*/
// FUNCTION: STBC 0x0058c2e0
Backdrop *Backdrop::GetByName( char *name ) {
	Backdrop *backdrop = NULL;
	backdropsByName.GetAt( name, backdrop );
	return backdrop;
}

/*
================
Backdrop::WriteToStream
================
*/
// FUNCTION: STBC 0x0058c330
bool Backdrop::WriteToStream( TGStream *stream ) {
	if ( !ObjectClass::WriteToStream( stream ) ) {
		return false;
	}
	stream->VirtualSlot11( modelName );
	return true;
}

/*
================
Backdrop::ReadFromStream
================
*/
// FUNCTION: STBC 0x0058c360
bool Backdrop::ReadFromStream( TGStream *stream ) {
	if ( !ObjectClass::ReadFromStream( stream ) ) {
		return false;
	}
	modelName = NULL;
	char *model = stream->VirtualSlot10( -1 );
	if ( model != NULL ) {
		SetModelName( model );
		delete [] model;
	}
	SetName( name );
	return true;
}
