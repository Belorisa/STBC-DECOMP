/*
===============================================================================

	RandomAI.cpp

===============================================================================
*/

#include <iostream>
#include <string.h>

#include "AILog.h"
#include "PyEmbed.h"
#include "RandomAI.h"
#include "TGStream.h"
#include "TGSystemWrapperClass.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00490f60 _$E452
// FUNCTION: STBC 0x00490f80 _$E451

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00490940 _$E32
// FUNCTION: STBC 0x00490950 _$E29
// FUNCTION: STBC 0x00490960 _$E31
// FUNCTION: STBC 0x00490970 _$E30
// FUNCTION: STBC 0x00490980 _$E38
// FUNCTION: STBC 0x00490990 _$E35
// FUNCTION: STBC 0x004909a0 _$E37
// FUNCTION: STBC 0x004909b0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x004909c0 _$E73
// FUNCTION: STBC 0x004909d0 _$E72
// FUNCTION: STBC 0x004909e0 _$E77
// FUNCTION: STBC 0x004909f0 _$E76
// FUNCTION: STBC 0x00490a00 _$E88
// FUNCTION: STBC 0x00490a10 _$E87
// FUNCTION: STBC 0x00490a20 _$E92
// FUNCTION: STBC 0x00490a30 _$E91
// FUNCTION: STBC 0x00490a40 _$E96
// FUNCTION: STBC 0x00490a50 _$E95
// FUNCTION: STBC 0x00490a60 _$E100
// FUNCTION: STBC 0x00490a70 _$E99
// FUNCTION: STBC 0x00490a80 _$E224
// FUNCTION: STBC 0x00490a90 _$E223
// FUNCTION: STBC 0x00490aa0 _$E166
// FUNCTION: STBC 0x00490ab0 _$E165
// FUNCTION: STBC 0x00490ac0 _$E179
// FUNCTION: STBC 0x00490ad0 _$E178
// FUNCTION: STBC 0x00490ae0 _$E229
// FUNCTION: STBC 0x00490af0 _$E228
// FUNCTION: STBC 0x00490b00 _$E234
// FUNCTION: STBC 0x00490b10 _$E233
// FUNCTION: STBC 0x00490b20 _$E238
// FUNCTION: STBC 0x00490b30 _$E237
// FUNCTION: STBC 0x00490b40 _$E242
// FUNCTION: STBC 0x00490b50 _$E241
// FUNCTION: STBC 0x00490b60 _$E249
// FUNCTION: STBC 0x00490b70 _$E248
// FUNCTION: STBC 0x00490b80 _$E274
// FUNCTION: STBC 0x00490b90 _$E273
// FUNCTION: STBC 0x00490ba0 _$E278
// FUNCTION: STBC 0x00490bb0 _$E277
// FUNCTION: STBC 0x00490bc0 _$E124
// FUNCTION: STBC 0x00490bd0 _$E123
// FUNCTION: STBC 0x00490be0 _$E260
// FUNCTION: STBC 0x00490bf0 _$E259
// FUNCTION: STBC 0x00490c00 _$E267
// FUNCTION: STBC 0x00490c10 _$E266
// FUNCTION: STBC 0x00490c20 _$E290
// FUNCTION: STBC 0x00490c30 _$E289
// FUNCTION: STBC 0x00490c40 _$E294
// FUNCTION: STBC 0x00490c50 _$E293
// FUNCTION: STBC 0x00490c60 _$E334
// FUNCTION: STBC 0x00490c70 _$E333
// FUNCTION: STBC 0x00490c80 _$E340
// FUNCTION: STBC 0x00490c90 _$E339
// FUNCTION: STBC 0x00490ca0 _$E282
// FUNCTION: STBC 0x00490cb0 _$E281
// FUNCTION: STBC 0x00490cc0 _$E286
// FUNCTION: STBC 0x00490cd0 _$E285
// FUNCTION: STBC 0x00490ce0 _$E298
// FUNCTION: STBC 0x00490cf0 _$E297
// FUNCTION: STBC 0x00490d00 _$E302
// FUNCTION: STBC 0x00490d10 _$E301
// FUNCTION: STBC 0x00490d20 _$E306
// FUNCTION: STBC 0x00490d30 _$E305
// FUNCTION: STBC 0x00490d40 _$E310
// FUNCTION: STBC 0x00490d50 _$E309
// FUNCTION: STBC 0x00490d60 _$E314
// FUNCTION: STBC 0x00490d70 _$E313
// FUNCTION: STBC 0x00490d80 _$E318
// FUNCTION: STBC 0x00490d90 _$E317
// FUNCTION: STBC 0x00490da0 _$E322
// FUNCTION: STBC 0x00490db0 _$E321
// FUNCTION: STBC 0x00490dc0 _$E326
// FUNCTION: STBC 0x00490dd0 _$E325
// FUNCTION: STBC 0x00490de0 _$E129
// FUNCTION: STBC 0x00490df0 _$E128
// FUNCTION: STBC 0x00490e00 _$E330
// FUNCTION: STBC 0x00490e10 _$E329
// FUNCTION: STBC 0x00490e20 _$E134
// FUNCTION: STBC 0x00490e30 _$E133
// FUNCTION: STBC 0x00490e40 _$E344
// FUNCTION: STBC 0x00490e50 _$E343
// FUNCTION: STBC 0x00490e60 _$E348
// FUNCTION: STBC 0x00490e70 _$E347
// FUNCTION: STBC 0x00490e80 _$E352
// FUNCTION: STBC 0x00490e90 _$E351
// FUNCTION: STBC 0x00490ea0 _$E356
// FUNCTION: STBC 0x00490eb0 _$E355
// FUNCTION: STBC 0x00490ec0 _$E143
// FUNCTION: STBC 0x00490ed0 _$E142
// FUNCTION: STBC 0x00490ee0 _$E360
// FUNCTION: STBC 0x00490ef0 _$E359
// FUNCTION: STBC 0x00490f00 _$E364
// FUNCTION: STBC 0x00490f10 _$E363
// FUNCTION: STBC 0x00490f20 _$E368
// FUNCTION: STBC 0x00490f30 _$E367
// FUNCTION: STBC 0x00490f40 _$E149
// FUNCTION: STBC 0x00490f50 _$E148
// FUNCTION: STBC 0x00490f90 _$E154
// FUNCTION: STBC 0x00490fa0 _$E153
// FUNCTION: STBC 0x00490fb0 _$E184
// FUNCTION: STBC 0x00490fc0 _$E183
// FUNCTION: STBC 0x00490fd0 _$E372
// FUNCTION: STBC 0x00490fe0 _$E371
// FUNCTION: STBC 0x00490ff0 _$E189
// FUNCTION: STBC 0x00491000 _$E188
// FUNCTION: STBC 0x00491010 _$E376
// FUNCTION: STBC 0x00491020 _$E375
// FUNCTION: STBC 0x00491030 _$E380
// FUNCTION: STBC 0x00491040 _$E379
// FUNCTION: STBC 0x00491050 _$E194
// FUNCTION: STBC 0x00491060 _$E193
// FUNCTION: STBC 0x00491070 _$E384
// FUNCTION: STBC 0x00491080 _$E383
// FUNCTION: STBC 0x00491090 _$E159
// FUNCTION: STBC 0x004910a0 _$E158
// FUNCTION: STBC 0x004910b0 _$E388
// FUNCTION: STBC 0x004910c0 _$E387
// FUNCTION: STBC 0x004910d0 _$E392
// FUNCTION: STBC 0x004910e0 _$E391
// FUNCTION: STBC 0x004910f0 _$E199
// FUNCTION: STBC 0x00491100 _$E198
// FUNCTION: STBC 0x00491110 _$E204
// FUNCTION: STBC 0x00491120 _$E203
// FUNCTION: STBC 0x00491130 _$E396
// FUNCTION: STBC 0x00491140 _$E395
// FUNCTION: STBC 0x00491150 _$E400
// FUNCTION: STBC 0x00491160 _$E399
// FUNCTION: STBC 0x00491170 _$E404
// FUNCTION: STBC 0x00491180 _$E403
// FUNCTION: STBC 0x00491190 _$E408
// FUNCTION: STBC 0x004911a0 _$E407
// FUNCTION: STBC 0x004911b0 _$E412
// FUNCTION: STBC 0x004911c0 _$E411
// FUNCTION: STBC 0x004911d0 _$E416
// FUNCTION: STBC 0x004911e0 _$E415
// FUNCTION: STBC 0x004911f0 _$E420
// FUNCTION: STBC 0x00491200 _$E419
// FUNCTION: STBC 0x00491210 _$E424
// FUNCTION: STBC 0x00491220 _$E423
// FUNCTION: STBC 0x00491230 _$E209
// FUNCTION: STBC 0x00491240 _$E208
// FUNCTION: STBC 0x00491250 _$E428
// FUNCTION: STBC 0x00491260 _$E427
// FUNCTION: STBC 0x00491270 _$E214
// FUNCTION: STBC 0x00491280 _$E213
// FUNCTION: STBC 0x00491290 _$E219
// FUNCTION: STBC 0x004912a0 _$E218
// FUNCTION: STBC 0x004912b0 _$E432
// FUNCTION: STBC 0x004912c0 _$E431
// FUNCTION: STBC 0x004912d0 _$E437
// FUNCTION: STBC 0x004912e0 _$E436
// FUNCTION: STBC 0x004912f0 _$E441
// FUNCTION: STBC 0x00491300 _$E440

// FUNCTION: STBC 0x00491310 _$E444
// FUNCTION: STBC 0x00491320 _$E443
static AICreateFunctionRegistration randomAIRegistration( AI_TYPE_RANDOM, RandomAI::Create );

/*
================
RandomAI::RandomAI
================
*/
// FUNCTION: STBC 0x00491370 ??0RandomAI@@QAE@PAVPhysicsObjectClass@@PAD@Z
RandomAI::RandomAI( PhysicsObjectClass *object, char *name_ ) : ArtificialIntelligence( object, name_ ),
	currentRan( false ), available( NULL ), count( 0 ), current( -1 ), dormantCount( 0 ), ais( NULL ) {
}

/*
================
RandomAI::VirtualSlot15
================
*/
// FUNCTION: STBC 0x004913b0
PyObject *RandomAI::VirtualSlot15( void ) {
	return PyEmbed::Unknown6F9920( this, "RandomAI" );
}

/*
================
RandomAI::GetAIType
================
*/
// FUNCTION: STBC 0x004913c0
int RandomAI::GetAIType( void ) {
	return AI_TYPE_RANDOM;
}

/*
================
RandomAI::VirtualSlot20
================
*/
// FUNCTION: STBC 0x004913d0
bool RandomAI::VirtualSlot20( int type ) {
	return type == AI_TYPE_RANDOM ? true : false;
}

/*
================
RandomAI::~RandomAI

Deletes the AIs.
================
*/
// FUNCTION: STBC 0x00491400
// FUNCTION: STBC 0x004913e0 ??_GRandomAI@@UAEPAXI@Z
RandomAI::~RandomAI( void ) {
	for ( int i = 0; i < count; i++ ) {
		delete ais[i];
	}
	delete [] ais;
	delete [] available;
}

/*
================
RandomAI::AddAI

The AI's parent becomes this RandomAI.
================
*/
// FUNCTION: STBC 0x00491490
void RandomAI::AddAI( ArtificialIntelligence *ai ) {
	ArtificialIntelligence **newAIs = new ArtificialIntelligence *[count + 1];
	memcpy( newAIs, ais, count * sizeof( ArtificialIntelligence * ) );
	delete [] ais;
	ais = newAIs;
	ais[count] = ai->VirtualSlot13();
	ais[count]->SetParentID( id );
	count++;

	delete [] available;
	available = new bool[count];
	ResetAvailable();
}

/*
================
RandomAI::GetAllAIsInTree
================
*/
// FUNCTION: STBC 0x00491520
void RandomAI::GetAllAIsInTree( NiTList<ArtificialIntelligence *> &ais_ ) {
	ais_.AddTail( this );
	for ( int i = 0; i < count; i++ ) {
		ais[i]->GetAllAIsInTree( ais_ );
	}
}

/*
================
RandomAI::GetFocusAIs
================
*/
// FUNCTION: STBC 0x004915a0
void RandomAI::GetFocusAIs( NiTList<ArtificialIntelligence *> &ais_ ) {
	if ( focus ) {
		ais_.AddTail( this );
		// The AIs with the focus add their whole trees, not their focus
		// AIs.
		for ( int i = 0; i < count; i++ ) {
			if ( ais[i]->HasFocus() ) {
				ais[i]->GetAllAIsInTree( ais_ );
			}
		}
	}
}

/*
================
RandomAI::SetActive

Starts over: every AI is available again.
================
*/
// FUNCTION: STBC 0x00491620
void RandomAI::SetActive( void ) {
	AILog log( "RandomAI(%s)::SetActive", name );
	active = true;
	Reset();
}

/*
================
RandomAI::SetInactive

The active flag stays set.
================
*/
// FUNCTION: STBC 0x00491690
void RandomAI::SetInactive( void ) {
	AILog log( "RandomAI(%s)::SetInactive", name );
	if ( current >= 0 && current < count && ais[current]->IsActive() ) {
		ais[current]->SetInactive();
	}
}

/*
================
RandomAI::GotFocus
================
*/
// FUNCTION: STBC 0x00491710
void RandomAI::GotFocus( void ) {
	AILog log( "RandomAI(%s)::GotFocus", name );
	focus = true;
}

/*
================
RandomAI::LostFocus
================
*/
// FUNCTION: STBC 0x00491740
void RandomAI::LostFocus( void ) {
	AILog log( "RandomAI(%s)::LostFocus", name );
	if ( current >= 0 && current < count && ais[current]->HasFocus() ) {
		ais[current]->LostFocus();
	}
	focus = false;
}

/*
================
RandomAI::IsInterruptable
================
*/
// FUNCTION: STBC 0x004917c0
bool RandomAI::IsInterruptable( void ) {
	bool result = true;
	result &= interruptable;
	if ( result && active && current >= 0 && current < count ) {
		result &= ais[current]->IsInterruptable();
	}
	return result;
}

/*
================
RandomAI::Update

Runs the current AI, picking one first when there is none. An AI that
ends before it was ever active is passed over for another pick at once.
================
*/
// FUNCTION: STBC 0x004917f0
// PARKED: retail's active case sets currentRan and joins the shared `return status` (eax from the status register); a return in the case folds the constant 0. The loop as do/while ( PickNextAI() ) gives retail's shared none-left exit
int RandomAI::Update( double time ) {
	AILog log( "RandomAI(%s)::Update, contains %d", name, count );
	if ( ( current < 0 || current >= count ) && !PickNextAI() ) {
		if ( dormantCount != 0 ) {
			return US_DORMANT;
		}
		return US_DONE;
	}

	do {
		ArtificialIntelligence *ai = ais[current];
		if ( !ai->IsActive() ) {
			ai->SetActive();
		}
		if ( !ai->HasFocus() ) {
			ai->GotFocus();
		}

		int status = ai->Update( time );
		switch ( status ) {
		case US_ACTIVE:
			currentRan = true;
			return status;
		case US_DORMANT:
			dormantCount++;
			// fall through
		case US_DONE:
			ai->LostFocus();
			ai->SetInactive();
			if ( currentRan ) {
				return status;
			}
			available[current] = false;
			break;
		default:
			return status;
		}
	} while ( PickNextAI() );

	if ( dormantCount != 0 ) {
		return US_DORMANT;
	}
	return US_DONE;
}

/*
================
RandomAI::Reset
================
*/
// FUNCTION: STBC 0x00491940
void RandomAI::Reset( void ) {
	ResetAvailable();
	SetCurrentAI( -1 );
}

/*
================
RandomAI::IsDormant
================
*/
// FUNCTION: STBC 0x00491960
bool RandomAI::IsDormant( void ) {
	if ( current >= 0 && current < count ) {
		return ais[current]->IsDormant();
	}
	return false;
}

/*
================
RandomAI::PickNextAI
================
*/
// FUNCTION: STBC 0x00491980
bool RandomAI::PickNextAI( void ) {
	SetCurrentAI( -1 );

	int availableCount = 0;
	int i;
	for ( i = 0; i < count; i++ ) {
		availableCount += available[i];
	}

	bool *tried = new bool[count];
	memset( tried, 0, count );
	int triedCount = 0;
	while ( triedCount < availableCount ) {
		int index;
		do {
			// TG: the random number is unsigned in retail (GetRandomNumber
			// itself divides unsigned); the shared declaration says int.
			index = (unsigned int)g_kSystemWrapper.GetRandomNumber( count ) % count;
		} while ( !available[index] || tried[index] );
		tried[index] = true;
		if ( !ais[index]->IsDormant() ) {
			SetCurrentAI( index );
			break;
		}
		available[index] = false;
		dormantCount++;
		triedCount++;
	}
	delete [] tried;

	return current != -1;
}

/*
================
RandomAI::ResetAvailable
================
*/
// FUNCTION: STBC 0x00491a50
void RandomAI::ResetAvailable( void ) {
	for ( int i = 0; i < count; i++ ) {
		available[i] = true;
	}
	dormantCount = 0;
}

/*
================
RandomAI::SetCurrentAI
================
*/
// FUNCTION: STBC 0x00491a80
void RandomAI::SetCurrentAI( int index ) {
	if ( current >= 0 && current < count && ais[current]->IsActive() ) {
		ais[current]->SetInactive();
	}
	current = index;
	if ( active && index >= 0 && index < count ) {
		ais[current]->SetActive();
		ais[current]->Reset();
	}
	currentRan = false;
}

/*
================
RandomAI::WriteToStream

The AIs' own results are not checked.
================
*/
// FUNCTION: STBC 0x00491ae0
bool RandomAI::WriteToStream( TGStream *stream ) {
	bool result = true;
	result &= ArtificialIntelligence::WriteToStream( stream );
	if ( result ) {
		stream->WriteChar( currentRan );
		stream->WriteInt( count );
		stream->WriteInt( current );
		int i;
		for ( i = 0; i < count; i++ ) {
			stream->WriteChar( available[i] );
		}
		stream->WriteInt( dormantCount );
		for ( i = 0; i < count; i++ ) {
			ais[i]->WriteToStream( stream );
		}
	}
	return result;
}

/*
================
RandomAI::RandomAI
================
*/
// FUNCTION: STBC 0x00491b70 ??0RandomAI@@QAE@PAVTGStream@@PA_N@Z
RandomAI::RandomAI( TGStream *stream, bool *ok ) : ArtificialIntelligence( stream, ok ),
	currentRan( false ), available( NULL ), count( 0 ), current( -1 ), dormantCount( 0 ), ais( NULL ) {
	if ( *ok ) {
		char flag = stream->ReadChar();
		if ( flag == true || flag == false ) {
			currentRan = flag != 0;
		} else {
			*ok = false;
		}
		if ( *ok ) {
			count = stream->ReadInt();
			current = stream->ReadInt();
			int i;
			if ( count != 0 ) {
				available = new bool[count];
				for ( i = 0; i < count; i++ ) {
					flag = stream->ReadChar();
					if ( flag == true || flag == false ) {
						available[i] = flag != 0;
					} else {
						*ok = false;
					}
				}
			}
			dormantCount = stream->ReadInt();
			ais = new ArtificialIntelligence *[count];
			for ( i = 0; i < count; i++ ) {
				ais[i] = CreateFromStream( stream );
				if ( ais[i] == NULL ) {
					*ok = false;
				}
			}
		}
	}
}

/*
================
RandomAI::Create
================
*/
// FUNCTION: STBC 0x00491cb0
ArtificialIntelligence *RandomAI::Create( TGStream *stream ) {
	bool ok = true;
	RandomAI *ai = new RandomAI( stream, &ok );
	if ( !ok ) {
		delete ai;
		ai = NULL;
	}
	return ai;
}

/*
================
RandomAI::FixUpPointers
================
*/
// FUNCTION: STBC 0x00491d30
bool RandomAI::FixUpPointers( void ) {
	for ( int i = 0; i < count; i++ ) {
		if ( ais[i] != NULL && !ais[i]->FixUpPointers() ) {
			return false;
		}
	}
	return true;
}

/*
================
RandomAI::PostFixUp
================
*/
// FUNCTION: STBC 0x00491d70
void RandomAI::PostFixUp( void ) {
	for ( int i = 0; i < count; i++ ) {
		if ( ais[i] != NULL ) {
			ais[i]->PostFixUp();
		}
	}
}
