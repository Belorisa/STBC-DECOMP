/*
===============================================================================

	FocusManager.cpp

===============================================================================
*/

#include <iostream>
#include "FocusManager.h"
#include "TGPane.h"
#include "TGUIObject.h"
#include "TGStream.h"
#include "TGRootPane.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x0044dd60 _$E438
// FUNCTION: STBC 0x0044dd80 _$E437

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0044d8a0 _$E32
// FUNCTION: STBC 0x0044d8b0 _$E29
// FUNCTION: STBC 0x0044d8c0 _$E31
// FUNCTION: STBC 0x0044d8d0 _$E30
// FUNCTION: STBC 0x0044d8e0 _$E38
// FUNCTION: STBC 0x0044d8f0 _$E35
// FUNCTION: STBC 0x0044d900 _$E37
// FUNCTION: STBC 0x0044d910 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0044d920 _$E57
// FUNCTION: STBC 0x0044d930 _$E56
// FUNCTION: STBC 0x0044d940 _$E61
// FUNCTION: STBC 0x0044d950 _$E60
// FUNCTION: STBC 0x0044d960 _$E72
// FUNCTION: STBC 0x0044d970 _$E71
// FUNCTION: STBC 0x0044d980 _$E76
// FUNCTION: STBC 0x0044d990 _$E75
// FUNCTION: STBC 0x0044d9a0 _$E80
// FUNCTION: STBC 0x0044d9b0 _$E79
// FUNCTION: STBC 0x0044d9c0 _$E84
// FUNCTION: STBC 0x0044d9d0 _$E83
// FUNCTION: STBC 0x0044d9e0 _$E208
// FUNCTION: STBC 0x0044d9f0 _$E207
// FUNCTION: STBC 0x0044da00 _$E150
// FUNCTION: STBC 0x0044da10 _$E149
// FUNCTION: STBC 0x0044da20 _$E163
// FUNCTION: STBC 0x0044da30 _$E162
// FUNCTION: STBC 0x0044da40 _$E213
// FUNCTION: STBC 0x0044da50 _$E212
// FUNCTION: STBC 0x0044da60 _$E218
// FUNCTION: STBC 0x0044da70 _$E217
// FUNCTION: STBC 0x0044da80 _$E222
// FUNCTION: STBC 0x0044da90 _$E221
// FUNCTION: STBC 0x0044daa0 _$E226
// FUNCTION: STBC 0x0044dab0 _$E225
// FUNCTION: STBC 0x0044dac0 _$E233
// FUNCTION: STBC 0x0044dad0 _$E232
// FUNCTION: STBC 0x0044dae0 _$E258
// FUNCTION: STBC 0x0044daf0 _$E257
// FUNCTION: STBC 0x0044db00 _$E262
// FUNCTION: STBC 0x0044db10 _$E261
// FUNCTION: STBC 0x0044db20 _$E108
// FUNCTION: STBC 0x0044db30 _$E107
// FUNCTION: STBC 0x0044db40 _$E244
// FUNCTION: STBC 0x0044db50 _$E243
// FUNCTION: STBC 0x0044db60 _$E251
// FUNCTION: STBC 0x0044db70 _$E250
// FUNCTION: STBC 0x0044db80 _$E274
// FUNCTION: STBC 0x0044db90 _$E273
// FUNCTION: STBC 0x0044dba0 _$E278
// FUNCTION: STBC 0x0044dbb0 _$E277
// FUNCTION: STBC 0x0044dbc0 _$E318
// FUNCTION: STBC 0x0044dbd0 _$E317
// FUNCTION: STBC 0x0044dbe0 _$E324
// FUNCTION: STBC 0x0044dbf0 _$E323
// FUNCTION: STBC 0x0044dc00 _$E266
// FUNCTION: STBC 0x0044dc10 _$E265
// FUNCTION: STBC 0x0044dc20 _$E270
// FUNCTION: STBC 0x0044dc30 _$E269
// FUNCTION: STBC 0x0044dc80 _$E282
// FUNCTION: STBC 0x0044dc90 _$E281
// FUNCTION: STBC 0x0044dcc0 _$E286
// FUNCTION: STBC 0x0044dcd0 _$E285
// FUNCTION: STBC 0x0044dce0 _$E290
// FUNCTION: STBC 0x0044dcf0 _$E289
// FUNCTION: STBC 0x0044dd00 _$E294
// FUNCTION: STBC 0x0044dd10 _$E293
// FUNCTION: STBC 0x0044dd20 _$E298
// FUNCTION: STBC 0x0044dd30 _$E297
// FUNCTION: STBC 0x0044dd40 _$E302
// FUNCTION: STBC 0x0044dd50 _$E301
// FUNCTION: STBC 0x0044dd90 _$E306
// FUNCTION: STBC 0x0044dda0 _$E305
// FUNCTION: STBC 0x0044ddb0 _$E310
// FUNCTION: STBC 0x0044ddc0 _$E309
// FUNCTION: STBC 0x0044ddd0 _$E113
// FUNCTION: STBC 0x0044dde0 _$E112
// FUNCTION: STBC 0x0044ddf0 _$E314
// FUNCTION: STBC 0x0044de00 _$E313
// FUNCTION: STBC 0x0044de10 _$E118
// FUNCTION: STBC 0x0044de20 _$E117
// FUNCTION: STBC 0x0044de30 _$E328
// FUNCTION: STBC 0x0044de40 _$E327
// FUNCTION: STBC 0x0044de50 _$E332
// FUNCTION: STBC 0x0044de60 _$E331
// FUNCTION: STBC 0x0044de70 _$E336
// FUNCTION: STBC 0x0044de80 _$E335
// FUNCTION: STBC 0x0044de90 _$E340
// FUNCTION: STBC 0x0044dea0 _$E339
// FUNCTION: STBC 0x0044deb0 _$E127
// FUNCTION: STBC 0x0044dec0 _$E126
// FUNCTION: STBC 0x0044ded0 _$E344
// FUNCTION: STBC 0x0044dee0 _$E343
// FUNCTION: STBC 0x0044def0 _$E348
// FUNCTION: STBC 0x0044df00 _$E347
// FUNCTION: STBC 0x0044df10 _$E352
// FUNCTION: STBC 0x0044df20 _$E351
// FUNCTION: STBC 0x0044df30 _$E133
// FUNCTION: STBC 0x0044df40 _$E132
// FUNCTION: STBC 0x0044df50 _$E138
// FUNCTION: STBC 0x0044df60 _$E137
// FUNCTION: STBC 0x0044df70 _$E168
// FUNCTION: STBC 0x0044df80 _$E167
// FUNCTION: STBC 0x0044df90 _$E356
// FUNCTION: STBC 0x0044dfa0 _$E355
// FUNCTION: STBC 0x0044dfb0 _$E173
// FUNCTION: STBC 0x0044dfc0 _$E172
// FUNCTION: STBC 0x0044dfd0 _$E360
// FUNCTION: STBC 0x0044dfe0 _$E359
// FUNCTION: STBC 0x0044dff0 _$E364
// FUNCTION: STBC 0x0044e000 _$E363
// FUNCTION: STBC 0x0044e010 _$E178
// FUNCTION: STBC 0x0044e020 _$E177
// FUNCTION: STBC 0x0044e030 _$E368
// FUNCTION: STBC 0x0044e040 _$E367
// FUNCTION: STBC 0x0044e050 _$E143
// FUNCTION: STBC 0x0044e060 _$E142
// FUNCTION: STBC 0x0044e070 _$E372
// FUNCTION: STBC 0x0044e080 _$E371
// FUNCTION: STBC 0x0044e090 _$E376
// FUNCTION: STBC 0x0044e0a0 _$E375
// FUNCTION: STBC 0x0044e0b0 _$E183
// FUNCTION: STBC 0x0044e0c0 _$E182
// FUNCTION: STBC 0x0044e0d0 _$E188
// FUNCTION: STBC 0x0044e0e0 _$E187
// FUNCTION: STBC 0x0044e0f0 _$E380
// FUNCTION: STBC 0x0044e100 _$E379
// FUNCTION: STBC 0x0044e110 _$E384
// FUNCTION: STBC 0x0044e120 _$E383
// FUNCTION: STBC 0x0044e130 _$E388
// FUNCTION: STBC 0x0044e140 _$E387
// FUNCTION: STBC 0x0044e150 _$E392
// FUNCTION: STBC 0x0044e160 _$E391
// FUNCTION: STBC 0x0044e170 _$E396
// FUNCTION: STBC 0x0044e180 _$E395
// FUNCTION: STBC 0x0044e190 _$E400
// FUNCTION: STBC 0x0044e1a0 _$E399
// FUNCTION: STBC 0x0044e1b0 _$E404
// FUNCTION: STBC 0x0044e1c0 _$E403
// FUNCTION: STBC 0x0044e1d0 _$E408
// FUNCTION: STBC 0x0044e1e0 _$E407
// FUNCTION: STBC 0x0044e1f0 _$E193
// FUNCTION: STBC 0x0044e200 _$E192
// FUNCTION: STBC 0x0044e210 _$E412
// FUNCTION: STBC 0x0044e220 _$E411
// FUNCTION: STBC 0x0044e230 _$E198
// FUNCTION: STBC 0x0044e240 _$E197
// FUNCTION: STBC 0x0044e250 _$E203
// FUNCTION: STBC 0x0044e260 _$E202
// FUNCTION: STBC 0x0044e270 _$E416
// FUNCTION: STBC 0x0044e280 _$E415
// FUNCTION: STBC 0x0044e290 _$E421
// FUNCTION: STBC 0x0044e2a0 _$E420
// FUNCTION: STBC 0x0044e2b0 _$E425
// FUNCTION: STBC 0x0044e2c0 _$E424

// The focus manager (the SWIG variable g_kFocusManager).
// FUNCTION: STBC 0x0044d1a0 _$E430
// FUNCTION: STBC 0x0044d1b0 _$E427
// FUNCTION: STBC 0x0044d1c0 _$E429
// FUNCTION: STBC 0x0044d1d0 _$E428
// DATA: STBC 0x009803a0
FocusManager	g_kFocusManager;

/*
================
FocusManager::FocusManager
================
*/
// FUNCTION: STBC 0x0044d1e0
FocusManager::FocusManager( void ) : tabOrder( 2 ) {
}

/*
================
FocusManager::~FocusManager
================
*/
// FUNCTION: STBC 0x0044d210
FocusManager::~FocusManager( void ) {
}

/*
================
FocusManager::AddObjectToTabOrder

Puts the object's ID at the end of the tab order, unless the object or
one of its parents is in it already. IDs whose objects are gone are
dropped on the way.
================
*/
// FUNCTION: STBC 0x0044d260
void FocusManager::AddObjectToTabOrder( TGUIObject *object ) {
	NiTListIterator item = tabOrder.GetHeadPos();
	while ( item != NULL ) {
		long id = tabOrder.Get( item );
		NiTListIterator next = tabOrder.GetNextPos( item );
		TGUIObject *other = static_cast<TGUIObject *>( TGObject::GetTGObjectPtr( id ) );
		if ( other != NULL ) {
			if ( object == other ) {
				return;
			}
			for ( TGPane *parent = object->GetParent(); parent != NULL && parent != UI_NO_PARENT; parent = parent->GetParent() ) {
				if ( parent == other ) {
					return;
				}
			}
		} else {
			tabOrder.RemovePos( item );
		}
		item = next;
	}
	tabOrder.AddTail( object->GetObjID() );
}

/*
================
FocusManager::RemoveObjectFromTabOrder

Takes the object's ID out of the tab order, if it is listed.
================
*/
// FUNCTION: STBC 0x0044d3b0
void FocusManager::RemoveObjectFromTabOrder( TGUIObject *object ) {
	if ( object != NULL ) {
		tabOrder.Remove( object->GetObjID() );
	}
}

/*
================
FocusManager::RemoveAllObjects

Empties the tab order.
================
*/
// FUNCTION: STBC 0x0044d4c0
void FocusManager::RemoveAllObjects( void ) {
	tabOrder.RemoveAll();
}

/*
================
FocusManager::RemoveAllObjectsUnder

Takes every listed object that sits below the pane out of the tab order,
and the pane itself. An ID with no object is dropped.
================
*/
// FUNCTION: STBC 0x0044d510
void FocusManager::RemoveAllObjectsUnder( TGUIObject *root ) {
	NiTListIterator item = tabOrder.GetHeadPos();
	if ( root != NULL ) {
		while ( item != NULL ) {
			long id = tabOrder.Get( item );
			NiTListIterator next = tabOrder.GetNextPos( item );
			TGUIObject *object = static_cast<TGUIObject *>( TGObject::GetTGObjectPtr( id ) );
			if ( object != NULL ) {
				for ( TGPane *parent = object->GetParent(); parent != NULL && parent != UI_NO_PARENT; parent = parent->GetParent() ) {
					if ( parent == root ) {
						RemoveObjectFromTabOrder( object );
						break;
					}
				}
			} else {
				tabOrder.RemovePos( item );
			}
			item = next;
		}
		RemoveObjectFromTabOrder( root );
	}
}

/*
================
FocusManager::FocusObject

Gives the object the focus in its parent, the parent the focus in its
own, and so on up to the root.
================
*/
// FUNCTION: STBC 0x0044d620
void FocusManager::FocusObject( TGUIObject *object, bool layout ) {
	TGPane *parent;
	while ( ( parent = object->GetParent() ) != NULL && parent != UI_NO_PARENT ) {
		parent->TGPane::SetFocus( object, layout );
		layout = true;
		object = parent;
	}
}

/*
================
FocusManager::Tab

Moves the true focus to the next visible object of the tab order after
the one that holds it, wrapping around; from the start when the focus is
in none of them. IDs whose objects are gone are dropped on the way.
================
*/
// FUNCTION: STBC 0x0044d650
void FocusManager::Tab( void ) {
	TGUIObject *focus = g_kRootWindow.GetTrueFocus();
	NiTListIterator item = tabOrder.GetHeadPos();
	NiTListIterator pos = NULL;
	TGUIObject *holder = NULL;
	while ( item != NULL ) {
		long id = tabOrder.Get( item );
		NiTListIterator next = tabOrder.GetNextPos( item );
		TGUIObject *object = static_cast<TGUIObject *>( TGObject::GetTGObjectPtr( id ) );
		if ( object != NULL ) {
			TGPane *parent = focus->GetParent();
			if ( focus == object ) {
				holder = focus;
				pos = next;
				break;
			}
			for ( ; parent != NULL && parent != UI_NO_PARENT; parent = parent->GetParent() ) {
				if ( parent == object ) {
					holder = parent;
					break;
				}
			}
			if ( holder != NULL ) {
				pos = next;
				break;
			}
		} else {
			tabOrder.RemovePos( item );
		}
		item = next;
	}
	if ( holder == NULL || pos == NULL ) {
		pos = tabOrder.GetHeadPos();
		if ( pos == NULL ) {
			return;
		}
	}
	NiTListIterator start = pos;
	do {
		TGUIObject *object = static_cast<TGUIObject *>( TGObject::GetTGObjectPtr( tabOrder.Get( pos ) ) );
		if ( object != NULL && object->IsCompletelyVisible() ) {
			FocusObject( object, true );
			return;
		}
		pos = tabOrder.GetNextPos( pos );
		if ( pos == NULL ) {
			pos = tabOrder.GetHeadPos();
		}
	} while ( pos != start );
}

/*
================
FocusManager::WriteToStream

Saves the tab order: the number of IDs, then each ID.
================
*/
// FUNCTION: STBC 0x0044d7a0
bool FocusManager::WriteToStream( TGStream *stream ) {
	stream->WriteInt( tabOrder.GetSize() );
	NiTListIterator item = tabOrder.GetHeadPos();
	while ( item != NULL ) {
		stream->WriteID( tabOrder.GetNext( item ) );
	}
	return true;
}

/*
================
FocusManager::ReadFromStream

Loads the tab order WriteToStream saved, in place of the current one.
================
*/
// FUNCTION: STBC 0x0044d7e0
bool FocusManager::ReadFromStream( TGStream *stream ) {
	int count = stream->ReadInt();
	tabOrder.RemoveAll();
	for ( int i = 0; i < count; i++ ) {
		tabOrder.AddTail( stream->ReadID() );
	}
	return true;
}
