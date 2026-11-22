/*
===============================================================================

	KeyboardBinding.cpp

===============================================================================
*/

#include <iostream>
#include "KeyboardBinding.h"
#include "TGBoolEvent.h"
#include "TGEventManager.h"
#include "TGFloatEvent.h"
#include "TGInputManager.h"
#include "TGKeyboardEvent.h"
#include "TGStream.h"
#include "TGString.h"
#include "TGSystemWrapperClass.h"
#include "PyEmbed.h"
#include "UtopiaModule.h"
#include <stdio.h>
#include "TGIntEvent.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiMultiTextureProperty.h"
#include "NiPointLight.h"
#include "NiShadeProperty.h"
#include "NiSkinController.h"
#include "NiSortAdjustNode.h"
#include "NiSoundSystem.h"
#include "NiSource.h"
#include "NiSpecularProperty.h"
#include "NiSpotLight.h"
#include "NiStencilProperty.h"
#include "NiStringExtraData.h"
#include "NiTextureEffect.h"
#include "NiTextureProperty.h"
#include "NiTransparentProperty.h"
#include "NiTriShapeDynamicData.h"
#include "NiTriShapeSkinController.h"
#include "NiTriStrip.h"
#include "NiTriStripData.h"
#include "NiTriStrips.h"
#include "NiTriStripsData.h"
#include "NiTriangles.h"
#include "NiTrianglesData.h"
#include "NiVertWeightsExtraData.h"
#include "NiVertexColorProperty.h"
#include "NiWireframeProperty.h"
#include "NiZBufferProperty.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0055b5c0 _$E32
// FUNCTION: STBC 0x0055b5d0 _$E29
// FUNCTION: STBC 0x0055b5e0 _$E31
// FUNCTION: STBC 0x0055b5f0 _$E30
// FUNCTION: STBC 0x0055b600 _$E38
// FUNCTION: STBC 0x0055b610 _$E35
// FUNCTION: STBC 0x0055b620 _$E37
// FUNCTION: STBC 0x0055b630 _$E36

// The guard of std::ctype<unsigned short>::id and its exit-time thunk (<iostream>).
// FUNCTION: STBC 0x0055c2d0 _$E471
// FUNCTION: STBC 0x0055c2f0 _$E470

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00559820 _$E463
// FUNCTION: STBC 0x00559830 _$E460
// FUNCTION: STBC 0x00559840 _$E462
// FUNCTION: STBC 0x00559850 _$E461
// FUNCTION: STBC 0x0055b640 _$E89
// FUNCTION: STBC 0x0055b650 _$E88
// FUNCTION: STBC 0x0055b660 _$E245
// FUNCTION: STBC 0x0055b670 _$E244
// FUNCTION: STBC 0x0055b680 _$E100
// FUNCTION: STBC 0x0055b690 _$E99
// FUNCTION: STBC 0x0055b6a0 _$E104
// FUNCTION: STBC 0x0055b6b0 _$E103
// FUNCTION: STBC 0x0055b6c0 _$E108
// FUNCTION: STBC 0x0055b6d0 _$E107
// FUNCTION: STBC 0x0055b6e0 _$E112
// FUNCTION: STBC 0x0055b6f0 _$E111
// FUNCTION: STBC 0x0055b700 _$E232
// FUNCTION: STBC 0x0055b710 _$E231
// FUNCTION: STBC 0x0055b720 _$E174
// FUNCTION: STBC 0x0055b730 _$E173
// FUNCTION: STBC 0x0055b740 _$E187
// FUNCTION: STBC 0x0055b750 _$E186
// FUNCTION: STBC 0x0055b7e0 _$E237
// FUNCTION: STBC 0x0055b7f0 _$E236
// FUNCTION: STBC 0x0055b800 _$E250
// FUNCTION: STBC 0x0055b810 _$E249
// FUNCTION: STBC 0x0055b820 _$E241
// FUNCTION: STBC 0x0055b830 _$E240
// FUNCTION: STBC 0x0055b840 _$E254
// FUNCTION: STBC 0x0055b850 _$E253
// FUNCTION: STBC 0x0055b860 _$E261
// FUNCTION: STBC 0x0055b870 _$E260
// FUNCTION: STBC 0x0055b880 _$E291
// FUNCTION: STBC 0x0055b890 _$E290
// FUNCTION: STBC 0x0055b8a0 _$E295
// FUNCTION: STBC 0x0055b8b0 _$E294
// FUNCTION: STBC 0x0055b8c0 _$E136
// FUNCTION: STBC 0x0055b8d0 _$E135
// FUNCTION: STBC 0x0055ba80 _$E277
// FUNCTION: STBC 0x0055ba90 _$E276
// FUNCTION: STBC 0x0055bb80 _$E284
// FUNCTION: STBC 0x0055bb90 _$E283
// FUNCTION: STBC 0x0055bc20 _$E307
// FUNCTION: STBC 0x0055bc30 _$E306
// FUNCTION: STBC 0x0055bc40 _$E311
// FUNCTION: STBC 0x0055bc50 _$E310
// FUNCTION: STBC 0x0055bc60 _$E351
// FUNCTION: STBC 0x0055bc70 _$E350
// FUNCTION: STBC 0x0055bc80 _$E357
// FUNCTION: STBC 0x0055bc90 _$E356
// FUNCTION: STBC 0x0055bca0 _$E299
// FUNCTION: STBC 0x0055bcb0 _$E298
// FUNCTION: STBC 0x0055bcc0 _$E303
// FUNCTION: STBC 0x0055bcd0 _$E302
// FUNCTION: STBC 0x0055bce0 _$E315
// FUNCTION: STBC 0x0055bcf0 _$E314
// FUNCTION: STBC 0x0055bd00 _$E319
// FUNCTION: STBC 0x0055bd10 _$E318
// FUNCTION: STBC 0x0055bd20 _$E323
// FUNCTION: STBC 0x0055bd30 _$E322
// FUNCTION: STBC 0x0055bdf0 _$E327
// FUNCTION: STBC 0x0055be00 _$E326
// FUNCTION: STBC 0x0055be10 _$E331
// FUNCTION: STBC 0x0055be20 _$E330
// FUNCTION: STBC 0x0055be30 _$E335
// FUNCTION: STBC 0x0055be40 _$E334
// FUNCTION: STBC 0x0055be50 _$E339
// FUNCTION: STBC 0x0055be60 _$E338
// FUNCTION: STBC 0x0055be70 _$E343
// FUNCTION: STBC 0x0055be80 _$E342
// FUNCTION: STBC 0x0055be90 _$E141
// FUNCTION: STBC 0x0055bea0 _$E140
// FUNCTION: STBC 0x0055beb0 _$E347
// FUNCTION: STBC 0x0055bec0 _$E346
// FUNCTION: STBC 0x0055bed0 _$E146
// FUNCTION: STBC 0x0055bee0 _$E145
// FUNCTION: STBC 0x0055bef0 _$E361
// FUNCTION: STBC 0x0055bf00 _$E360
// FUNCTION: STBC 0x0055bf10 _$E365
// FUNCTION: STBC 0x0055bf20 _$E364
// FUNCTION: STBC 0x0055bf30 _$E369
// FUNCTION: STBC 0x0055bf40 _$E368
// FUNCTION: STBC 0x0055bf50 _$E373
// FUNCTION: STBC 0x0055bf60 _$E372
// FUNCTION: STBC 0x0055bf70 _$E151
// FUNCTION: STBC 0x0055bf80 _$E150
// FUNCTION: STBC 0x0055bf90 _$E377
// FUNCTION: STBC 0x0055bfa0 _$E376
// FUNCTION: STBC 0x0055bfb0 _$E381
// FUNCTION: STBC 0x0055bfc0 _$E380
// FUNCTION: STBC 0x0055bfd0 _$E385
// FUNCTION: STBC 0x0055bfe0 _$E384
// FUNCTION: STBC 0x0055bff0 _$E157
// FUNCTION: STBC 0x0055c000 _$E156
// FUNCTION: STBC 0x0055c010 _$E162
// FUNCTION: STBC 0x0055c020 _$E161
// FUNCTION: STBC 0x0055c1b0 _$E192
// FUNCTION: STBC 0x0055c1c0 _$E191
// FUNCTION: STBC 0x0055c350 _$E389
// FUNCTION: STBC 0x0055c360 _$E388
// FUNCTION: STBC 0x0055c370 _$E197
// FUNCTION: STBC 0x0055c380 _$E196
// FUNCTION: STBC 0x0055c390 _$E393
// FUNCTION: STBC 0x0055c3a0 _$E392
// FUNCTION: STBC 0x0055c3b0 _$E397
// FUNCTION: STBC 0x0055c3c0 _$E396
// FUNCTION: STBC 0x0055c3d0 _$E202
// FUNCTION: STBC 0x0055c3e0 _$E201
// FUNCTION: STBC 0x0055c3f0 _$E401
// FUNCTION: STBC 0x0055c400 _$E400
// FUNCTION: STBC 0x0055c410 _$E167
// FUNCTION: STBC 0x0055c420 _$E166
// FUNCTION: STBC 0x0055c430 _$E405
// FUNCTION: STBC 0x0055c440 _$E404
// FUNCTION: STBC 0x0055c450 _$E409
// FUNCTION: STBC 0x0055c460 _$E408
// FUNCTION: STBC 0x0055c470 _$E207
// FUNCTION: STBC 0x0055c480 _$E206
// FUNCTION: STBC 0x0055c490 _$E212
// FUNCTION: STBC 0x0055c4a0 _$E211
// FUNCTION: STBC 0x0055c4b0 _$E413
// FUNCTION: STBC 0x0055c4c0 _$E412
// FUNCTION: STBC 0x0055c4d0 _$E417
// FUNCTION: STBC 0x0055c4e0 _$E416
// FUNCTION: STBC 0x0055c4f0 _$E421
// FUNCTION: STBC 0x0055c500 _$E420
// FUNCTION: STBC 0x0055c510 _$E425
// FUNCTION: STBC 0x0055c520 _$E424
// FUNCTION: STBC 0x0055c530 _$E429
// FUNCTION: STBC 0x0055c540 _$E428
// FUNCTION: STBC 0x0055c550 _$E433
// FUNCTION: STBC 0x0055c560 _$E432
// FUNCTION: STBC 0x0055c570 _$E437
// FUNCTION: STBC 0x0055c580 _$E436
// FUNCTION: STBC 0x0055c590 _$E441
// FUNCTION: STBC 0x0055c5a0 _$E440
// FUNCTION: STBC 0x0055c5b0 _$E217
// FUNCTION: STBC 0x0055c5c0 _$E216
// FUNCTION: STBC 0x0055c5d0 _$E445
// FUNCTION: STBC 0x0055c5e0 _$E444
// FUNCTION: STBC 0x0055c5f0 _$E222
// FUNCTION: STBC 0x0055c600 _$E221
// FUNCTION: STBC 0x0055c610 _$E227
// FUNCTION: STBC 0x0055c620 _$E226
// FUNCTION: STBC 0x0055c630 _$E449
// FUNCTION: STBC 0x0055c640 _$E448
// FUNCTION: STBC 0x0055c650 _$E454
// FUNCTION: STBC 0x0055c660 _$E453
// FUNCTION: STBC 0x0055c670 _$E458
// FUNCTION: STBC 0x0055c680 _$E457

// DATA: STBC 0x0098b538
KeyboardBinding g_kKeyboardBinding;

/*
KeyboardBinding::KeyboardBinding

No bindings; at most two keys bound to each event at first.
*/
// FUNCTION: STBC 0x00559860 ??0KeyboardBinding@@QAE@XZ
KeyboardBinding::KeyboardBinding( void ) : unknown04( 2 ) {
	maxBoundKeys = 2;
}

/*
KeyboardBinding::~KeyboardBinding

Deletes every binding before the containers go.
*/
// FUNCTION: STBC 0x005598f0 ??1KeyboardBinding@@QAE@XZ
KeyboardBinding::~KeyboardBinding( void ) {
	Unknown55AF50();
}

/*
================
KeyboardBinding::BindKey

Binds the key (for a bool event, its character on the scan code's default
key list) in the key state to the event, ahead of the key's other bindings.
Every other key bound to the same event either loses that binding
(BIND_UNBIND_OTHER_KEYS) or is ranked by the time it was bound: past the
newest maxBoundKeys keys, counting the new one, the older keys lose it. A
locked key is not rebound.
================
*/
// FUNCTION: STBC 0x005599d0
// DRAFT: inline budget: the body agrees with retail up to the end, where retail still expands NiTMap::GetAt and the ~NiTList of `delete oldBinds` that this build calls out of line (the extra GetAt out-parameter slot shifts the frame by 4); and the BIND_REPLACE branch lays out its delete after the AddTail where retail puts it first
bool KeyboardBinding::BindKey( int key, int keyState, int eventType, int eventKind, float value, long flags,
							   bool writeMappingFile ) {
	if ( eventKind == GET_BOOL_EVENT ) {
		key = g_kInputManager.GetUnicodeKey( g_kInputManager.GetScanCode( key ), false );
	}
	if ( unknown04.FindPos( key ) ) {
		return false;
	}

	NiTList<int> emptyKeys( 2 );			// the keys left with no bindings
	NiTList<BoundKey *> boundKeys( 2 );		// the keys bound to the event, newest first
	boundKeys.AddTail( new BoundKey( key, NiApplication::application->GetCurrentTime() ) );
	int numKeys = maxBoundKeys;

	KeyBind *bind = new KeyBind;
	bind->eventKind = eventKind;
	bind->eventType = eventType;
	bind->value = value;
	bind->keyState = keyState;
	bind->time = NiApplication::application->GetCurrentTime();
	NiTList<KeyBind *> *newBinds = new NiTList<KeyBind *>( 2 );
	newBinds->AddTail( bind );

	NiTMapIterator pos = keyBinds.GetFirstPos();
	while ( pos ) {
		int boundKey;
		NiTList<KeyBind *> *binds;
		keyBinds.GetNext( pos, boundKey, binds );
		NiTListIterator bindPos = binds->GetHeadPos();
		while ( bindPos ) {
			KeyBind *other = binds->GetNext( bindPos );
			if ( other->eventKind == eventKind && other->eventType == eventType && other->value == value && other->keyState == keyState ) {
				if ( flags & BIND_UNBIND_OTHER_KEYS ) {
					binds->Remove( other );
					delete other;
				} else if ( boundKey != key ) {
					bool ranked = false;
					NiTListIterator rankPos = boundKeys.GetHeadPos();
					while ( rankPos ) {
						BoundKey *newer = boundKeys.GetNext( rankPos );
						if ( newer->time < other->time ) {
							boundKeys.InsertBefore( boundKeys.FindPos( newer ), new BoundKey( boundKey, other->time ) );
							ranked = true;
							break;
						}
					}
					if ( !ranked ) {
						boundKeys.AddTail( new BoundKey( boundKey, other->time ) );
					}
				}
			}
		}
		if ( boundKey == key ) {
			while ( binds->GetSize() > 0 ) {
				KeyBind *old = binds->RemoveHead();
				if ( ( flags & BIND_REPLACE ) || ( old->eventKind == eventKind && old->eventType == eventType && old->value == value && old->keyState == keyState ) ) {
					delete old;
					numKeys--;
				} else {
					newBinds->AddTail( old );
				}
			}
		}
		if ( binds->GetSize() == 0 ) {
			emptyKeys.AddTail( boundKey );
			delete binds;
		}
	}

	// Past the newest numKeys keys, the older ones lose the binding.
	int rank = 0;
	NiTListIterator rankPos = boundKeys.GetHeadPos();
	NiTList<KeyBind *> *binds;
	while ( rankPos ) {
		BoundKey *bound = boundKeys.GetNext( rankPos );
		if ( ++rank > numKeys ) {
			keyBinds.GetAt( bound->key, binds );
			NiTListIterator bindPos = binds->GetHeadPos();
			while ( bindPos ) {
				KeyBind *other = binds->GetNext( bindPos );
				if ( other->eventKind == eventKind && other->eventType == eventType && other->value == value && other->keyState == keyState ) {
					binds->Remove( other );
					delete other;
				}
			}
			if ( binds->GetSize() == 0 ) {
				delete binds;
				keyBinds.RemoveAt( bound->key );
				delete bound;
			}
		}
	}
	boundKeys.RemoveAll();

	NiTListIterator emptyPos = emptyKeys.GetHeadPos();
	while ( emptyPos ) {
		keyBinds.RemoveAt( emptyKeys.GetNext( emptyPos ) );
	}
	emptyKeys.RemoveAll();

	NiTList<KeyBind *> *oldBinds;
	if ( keyBinds.GetAt( key, oldBinds ) && oldBinds ) {
		keyBinds.RemoveAt( key );
		NiTListIterator oldPos = oldBinds->GetHeadPos();
		while ( oldPos ) {
			delete oldBinds->GetNext( oldPos );
		}
		oldBinds->RemoveAll();
		delete oldBinds;
	}
	keyBinds.SetAt( key, newBinds );

	if ( flags & BIND_LOCK_KEY ) {
		unknown04.AddTail( key );
	}
	if ( writeMappingFile ) {
		GenerateMappingFile();
	}
	return true;
}

// How far apart the float members of two bindings may be to count as the
// same (invented name). A literal, not a const object: retail reads the
// compiler's merged constant.
#define KEY_BIND_EPSILON		0.0001f

/*
================
KeyboardBinding::GetNumKeys

The number of bindings, over every key, of the event kind and type with the value (within a hair).
================
*/
// FUNCTION: STBC 0x0055a3d0
int KeyboardBinding::GetNumKeys( int eventType, int eventKind, float value ) {
	int count = 0;
	NiTMapIterator pos = keyBinds.GetFirstPos();
	while ( pos ) {
		int key;
		NiTList<KeyBind *> *binds;
		keyBinds.GetNext( pos, key, binds );
		NiTListIterator bindPos = binds->GetHeadPos();
		while ( bindPos ) {
			KeyBind *bind = binds->GetNext( bindPos );
			if ( bind->eventKind == eventKind && bind->eventType == eventType && value - KEY_BIND_EPSILON <= bind->value && bind->value <= value + KEY_BIND_EPSILON ) {
				count++;
			}
		}
	}
	return count;
}

/*
================
KeyboardBinding::FindKey

The key of the nth binding, over every key, of the event kind and type with the value;
-1 when there are fewer.
================
*/
// FUNCTION: STBC 0x0055a4c0
int KeyboardBinding::FindKey( int eventType, int eventKind, float value, int n ) {
	int count = 0;
	NiTMapIterator pos = keyBinds.GetFirstPos();
	while ( pos ) {
		int key;
		NiTList<KeyBind *> *binds;
		keyBinds.GetNext( pos, key, binds );
		NiTListIterator bindPos = binds->GetHeadPos();
		while ( bindPos ) {
			KeyBind *bind = binds->GetNext( bindPos );
			if ( bind->eventKind == eventKind && bind->eventType == eventType && value - KEY_BIND_EPSILON <= bind->value && bind->value <= value + KEY_BIND_EPSILON ) {
				if ( count++ == n ) {
					return key;
				}
			}
		}
	}
	return -1;
}

/*
================
KeyboardBinding::FindKeyString

Writes the keys bound to the event kind and type with the value (within a
hair) into the string, newest binding first: "( A, B )" with the three
given characters.
================
*/
// FUNCTION: STBC 0x0055a5c0
void KeyboardBinding::FindKeyString( TGString *string, int eventType, int eventKind, float value, char open,
									 char separator, char close ) {
	string->SetString( "" );
	TGString openString( "" );
	openString.Unknown6F54A0( 0, open );
	openString.Unknown6F54A0( 1, ' ' );
	TGString closeString( "" );
	closeString.Unknown6F54A0( 0, ' ' );
	closeString.Unknown6F54A0( 1, close );
	TGString separatorString( "" );
	separatorString.Unknown6F54A0( 0, separator );
	separatorString.Unknown6F54A0( 1, ' ' );

	int numKeys = GetNumKeys( eventType, eventKind, value );
	int written = 0;
	if ( numKeys > 0 ) {
		string->Unknown6F4970( &openString );

		NiTList<BoundKey *> boundKeys( 2 );		// the keys bound to the event, newest first
		NiTMapIterator pos = keyBinds.GetFirstPos();
		while ( pos ) {
			int key;
			NiTList<KeyBind *> *binds;
			keyBinds.GetNext( pos, key, binds );
			NiTListIterator bindPos = binds->GetHeadPos();
			while ( bindPos ) {
				KeyBind *bind = binds->GetNext( bindPos );
				if ( bind->eventKind == eventKind && bind->eventType == eventType && value - KEY_BIND_EPSILON <= bind->value && bind->value <= value + KEY_BIND_EPSILON ) {
					bool ranked = false;
					NiTListIterator rankPos = boundKeys.GetHeadPos();
					while ( rankPos ) {
						BoundKey *newer = boundKeys.GetNext( rankPos );
						if ( newer->time < bind->time ) {
							boundKeys.InsertBefore( boundKeys.FindPos( newer ), new BoundKey( key, bind->time ) );
							ranked = true;
							break;
						}
					}
					if ( !ranked ) {
						boundKeys.AddTail( new BoundKey( key, bind->time ) );
					}
				}
			}
		}

		NiTListIterator keyPos = boundKeys.GetHeadPos();
		while ( keyPos ) {
			BoundKey *bound = boundKeys.GetNext( keyPos );
			string->Unknown6F4970( &g_kInputManager.GetDisplayStringFromUnicode( bound->key ) );
			if ( ++written < numKeys ) {
				string->Unknown6F4970( &separatorString );
			}
			delete bound;
		}
		boundKeys.RemoveAll();
		string->Unknown6F4970( &closeString );
	}
}

/*
================
KeyboardBinding::ClearBinding

Deletes every binding, over every key, of the event kind and type with the
value; a key left with no bindings is dropped from the map.
================
*/
// FUNCTION: STBC 0x0055aae0
void KeyboardBinding::ClearBinding( int eventType, int eventKind, float value ) {
	NiTMapIterator pos = keyBinds.GetFirstPos();
	while ( pos ) {
		int key;
		NiTList<KeyBind *> *binds;
		keyBinds.GetNext( pos, key, binds );
		NiTListIterator bindPos = binds->GetHeadPos();
		while ( bindPos ) {
			KeyBind *bind = binds->GetNext( bindPos );
			if ( bind->eventKind == eventKind && bind->eventType == eventType && value - KEY_BIND_EPSILON <= bind->value && bind->value <= value + KEY_BIND_EPSILON ) {
				binds->Remove( bind );
				delete bind;
			}
		}
		if ( binds->GetSize() == 0 ) {
			keyBinds.RemoveAt( key );
			delete binds;
		}
	}
}

/*
================
KeyboardBinding::LaunchEvent

Sends the event of each binding of the key in the key state to the
destination, from the source; the key's other character on the same scan
code sends its bool events too. True when every event sent was handled.
================
*/
// FUNCTION: STBC 0x0055ad00
bool KeyboardBinding::LaunchEvent( int key, int keyState, TGEventHandlerObject *destination, TGObject *source,
								   bool alternateKey ) {
	TGEvent *event = NULL;
	bool handled = false;
	NiTList<KeyBind *> *binds;
	if ( keyBinds.GetAt( key, binds ) ) {
		NiTListIterator pos = binds->GetHeadPos();
		while ( pos ) {
			KeyBind *bind = binds->GetNext( pos );
			if ( keyState != bind->keyState ) {
				continue;
			}
			// BUG: event is not cleared after the delete below, so a binding that makes no
			// event (another kind on the alternate key) sends the deleted one again.
			switch ( bind->eventKind ) {
			case GET_EVENT:
				if ( !alternateKey ) {
					event = new TGEvent;
				}
				break;
			case GET_INT_EVENT:
				if ( !alternateKey ) {
					TGIntEvent *intEvent = new TGIntEvent;
					event = intEvent;
					intEvent->SetInt( (int)bind->value );
				}
				break;
			case GET_BOOL_EVENT: {
				TGBoolEvent *boolEvent = new TGBoolEvent;
				event = boolEvent;
				boolEvent->SetBool( bind->value != 0.0f );
				break;
			}
			case GET_FLOAT_EVENT:
				if ( !alternateKey ) {
					TGFloatEvent *floatEvent = new TGFloatEvent;
					floatEvent->SetFloat( bind->value );
					event = floatEvent;
				}
				break;
			}
			if ( event != NULL ) {
				event->SetEventType( bind->eventType );
				event->SetDestination( destination );
				event->SetSource( source );
				g_kEventManager.ProcessEvent( event );
				if ( event->GetDestination() == NULL ) {
					handled = true;
				}
				delete event;
			}
		}
	}

	int alternate = g_kInputManager.GetUnicodeKey( g_kInputManager.GetScanCode( key ), false );
	if ( alternate != key ) {
		handled &= LaunchEvent( alternate, keyState, destination, source, true );
	}
	return handled;
}

/*
================
KeyboardBinding::Unknown55AF50

Deletes every binding and its list, and empties both containers (identity
open: RebuildMappingFromFile calls it before it runs the script that binds
the keys again, so it clears the bindings).
================
*/
// FUNCTION: STBC 0x0055af50
void KeyboardBinding::Unknown55AF50( void ) {
	NiTMapIterator pos = keyBinds.GetFirstPos();
	while ( pos ) {
		int key;
		NiTList<KeyBind *> *binds;
		keyBinds.GetNext( pos, key, binds );
		while ( !binds->IsEmpty() ) {
			delete binds->RemoveHead();
		}
		delete binds;
	}
	unknown04.RemoveAll();
	keyBinds.RemoveAll();
}

/*
================
KeyboardBinding::RebuildMappingFromFile

Binds the keys again by running the Initialize function of the script
scripts/<fileName>.py (or .pyc), KeyboardBinding's own when fileName is NULL.
Does nothing when the script is not there.
================
*/
// FUNCTION: STBC 0x0055b0e0
void KeyboardBinding::RebuildMappingFromFile( char *fileName ) {
	char path[256];
	if ( fileName != NULL ) {
		sprintf( path, "scripts/%s.py", fileName );
		if ( !g_kSystemWrapper.FileExists( path ) ) {
			sprintf( path, "scripts/%s.pyc", fileName );
			if ( !g_kSystemWrapper.FileExists( path ) ) {
				return;
			}
		}
	} else if ( !g_kSystemWrapper.FileExists( "scripts/KeyboardBinding.py" ) ) {
		return;
	}

	Unknown55AF50();
	int result;
	if ( fileName == NULL ) {
		result = PyEmbed::CallFunction( "KeyboardBinding", "Initialize", NULL, NULL, NULL, NULL );
	} else {
		result = PyEmbed::CallFunction( fileName, "Initialize", NULL, NULL, NULL, NULL );
	}
	if ( result == -1 ) {
		PyErr_Print();
	}
}

/*
================
KeyboardBinding::GenerateMappingFile

Writes scripts//KeyboardBinding.py: an Initialize() that binds each key to
its events again with BindKey, led by the version it was written by.
================
*/
// FUNCTION: STBC 0x0055b1a0
void KeyboardBinding::GenerateMappingFile( void ) {
	TGString version = TGString( "g_pcVersion = \"" ) + TGString( UtopiaModule::GetGameVersion() ) + TGString( "\"\n" );
	char *versionLine = version.Unknown6F56F0();

	TGStream file;
	file.VirtualSlot00( "scripts//KeyboardBinding.py", "w" );
	file.WriteCLine( "###############################################################################\n" );
	file.WriteCLine( "#\tThis file is automatically generated by KeyboardBinding\n" );
	file.WriteCLine( "#\tPlease check KeyboardBinding.cpp or KeyboardBinding.h if there is a problem\n" );
	file.WriteCLine( "#\t\n" );
	file.WriteCLine( "#\tConfidential and Proprietary, Copyright 2001 by Totally Games\n" );
	file.WriteCLine( "#\t\n" );
	file.WriteCLine( "#\tCreated:\t5/8/01- Colin Carley\n" );
	file.WriteCLine( "###############################################################################\n" );
	file.WriteCLine( "\n" );
	file.WriteCLine( "\n" );
	file.WriteCLine( "import App\n" );
	file.WriteCLine( "\n" );
	file.WriteCLine( versionLine );
	file.WriteCLine( "###############################################################################\n" );
	file.WriteCLine( "#\tInitialize()\n" );
	file.WriteCLine( "#\t\n" );
	file.WriteCLine( "#\tSets up keyboard bindings for use by the game\n" );
	file.WriteCLine( "#\t\n" );
	file.WriteCLine( "#\tArgs:\tnone\n" );
	file.WriteCLine( "#\t\n" );
	file.WriteCLine( "#\tReturn:\tnone\n" );
	file.WriteCLine( "###############################################################################\n" );
	file.WriteCLine( "def Initialize():\n" );

	NiTMapIterator pos = keyBinds.GetFirstPos();
	while ( pos ) {
		int key;
		NiTList<KeyBind *> *binds;
		keyBinds.GetNext( pos, key, binds );
		NiTListIterator bindPos = binds->GetHeadPos();
		while ( bindPos ) {
			KeyBind *bind = binds->GetNext( bindPos );
			char eventKindName[128];
			char keyStateName[128];
			char line[512];

			switch ( bind->eventKind ) {
			case GET_EVENT:
				sprintf( eventKindName, "App.KeyboardBinding.GET_EVENT" );
				break;
			case GET_BOOL_EVENT:
				sprintf( eventKindName, "App.KeyboardBinding.GET_BOOL_EVENT" );
				break;
			case GET_INT_EVENT:
				sprintf( eventKindName, "App.KeyboardBinding.GET_INT_EVENT" );
				break;
			case GET_FLOAT_EVENT:
				sprintf( eventKindName, "App.KeyboardBinding.GET_FLOAT_EVENT" );
				break;
			}
			switch ( bind->keyState ) {
			case KS_NORMAL:
				sprintf( keyStateName, "App.TGKeyboardEvent.KS_NORMAL" );
				break;
			case KS_KEYDOWN:
				sprintf( keyStateName, "App.TGKeyboardEvent.KS_KEYDOWN" );
				break;
			case KS_KEYUP:
				sprintf( keyStateName, "App.TGKeyboardEvent.KS_KEYUP" );
				break;
			}
			sprintf( line, "\tApp.g_kKeyboardBinding.BindKey(%d, %s, %d, %s, %f)\n", key, keyStateName, bind->eventType, eventKindName, bind->value );
			file.WriteCLine( line );
		}
	}
	file.WriteCLine( "\n" );
	file.Close();
	delete[] versionLine;
}
