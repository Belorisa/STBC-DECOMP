/*
===============================================================================

	SequenceAI.cpp

===============================================================================
*/

#include <iostream>
#include <string.h>

#include "AILog.h"
#include "PyEmbed.h"
#include "SequenceAI.h"
#include "TGStream.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00492380 _$E452
// FUNCTION: STBC 0x004923a0 _$E451

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00491da0 _$E32
// FUNCTION: STBC 0x00491db0 _$E29
// FUNCTION: STBC 0x00491dc0 _$E31
// FUNCTION: STBC 0x00491dd0 _$E30
// FUNCTION: STBC 0x00491de0 _$E38
// FUNCTION: STBC 0x00491df0 _$E35
// FUNCTION: STBC 0x00491e00 _$E37
// FUNCTION: STBC 0x00491e10 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00491e20 _$E73
// FUNCTION: STBC 0x00491e30 _$E72
// FUNCTION: STBC 0x00491e40 _$E77
// FUNCTION: STBC 0x00491e50 _$E76
// FUNCTION: STBC 0x00491e60 _$E88
// FUNCTION: STBC 0x00491e70 _$E87
// FUNCTION: STBC 0x00491e80 _$E92
// FUNCTION: STBC 0x00491e90 _$E91
// FUNCTION: STBC 0x00491ea0 _$E96
// FUNCTION: STBC 0x00491eb0 _$E95
// FUNCTION: STBC 0x00491ec0 _$E100
// FUNCTION: STBC 0x00491ed0 _$E99
// FUNCTION: STBC 0x00491ee0 _$E224
// FUNCTION: STBC 0x00491ef0 _$E223
// FUNCTION: STBC 0x00491f00 _$E166
// FUNCTION: STBC 0x00491f10 _$E165
// FUNCTION: STBC 0x00491f20 _$E179
// FUNCTION: STBC 0x00491f30 _$E178
// FUNCTION: STBC 0x00491f40 _$E229
// FUNCTION: STBC 0x00491f50 _$E228
// FUNCTION: STBC 0x00491f60 _$E234
// FUNCTION: STBC 0x00491f70 _$E233
// FUNCTION: STBC 0x00491f80 _$E238
// FUNCTION: STBC 0x00491f90 _$E237
// FUNCTION: STBC 0x00491fa0 _$E242
// FUNCTION: STBC 0x00491fb0 _$E241
// FUNCTION: STBC 0x00491fc0 _$E249
// FUNCTION: STBC 0x00491fd0 _$E248
// FUNCTION: STBC 0x00491fe0 _$E274
// FUNCTION: STBC 0x00491ff0 _$E273
// FUNCTION: STBC 0x00492000 _$E278
// FUNCTION: STBC 0x00492010 _$E277
// FUNCTION: STBC 0x00492020 _$E124
// FUNCTION: STBC 0x00492030 _$E123
// FUNCTION: STBC 0x00492040 _$E260
// FUNCTION: STBC 0x00492050 _$E259
// FUNCTION: STBC 0x00492060 _$E267
// FUNCTION: STBC 0x00492070 _$E266
// FUNCTION: STBC 0x00492080 _$E290
// FUNCTION: STBC 0x00492090 _$E289
// FUNCTION: STBC 0x004920a0 _$E294
// FUNCTION: STBC 0x004920b0 _$E293
// FUNCTION: STBC 0x004920c0 _$E334
// FUNCTION: STBC 0x004920d0 _$E333
// FUNCTION: STBC 0x004920e0 _$E340
// FUNCTION: STBC 0x004920f0 _$E339
// FUNCTION: STBC 0x00492100 _$E282
// FUNCTION: STBC 0x00492110 _$E281
// FUNCTION: STBC 0x00492120 _$E286
// FUNCTION: STBC 0x00492130 _$E285
// FUNCTION: STBC 0x00492140 _$E298
// FUNCTION: STBC 0x00492150 _$E297
// FUNCTION: STBC 0x00492160 _$E302
// FUNCTION: STBC 0x00492170 _$E301
// FUNCTION: STBC 0x00492180 _$E306
// FUNCTION: STBC 0x00492190 _$E305
// FUNCTION: STBC 0x004921a0 _$E310
// FUNCTION: STBC 0x004921b0 _$E309
// FUNCTION: STBC 0x004921c0 _$E314
// FUNCTION: STBC 0x004921d0 _$E313
// FUNCTION: STBC 0x004921e0 _$E318
// FUNCTION: STBC 0x004921f0 _$E317
// FUNCTION: STBC 0x00492200 _$E322
// FUNCTION: STBC 0x00492210 _$E321
// FUNCTION: STBC 0x00492220 _$E326
// FUNCTION: STBC 0x00492230 _$E325
// FUNCTION: STBC 0x00492240 _$E129
// FUNCTION: STBC 0x00492250 _$E128
// FUNCTION: STBC 0x00492260 _$E330
// FUNCTION: STBC 0x00492270 _$E329
// FUNCTION: STBC 0x00492280 _$E134
// FUNCTION: STBC 0x00492290 _$E133
// FUNCTION: STBC 0x004922a0 _$E344
// FUNCTION: STBC 0x004922b0 _$E343
// FUNCTION: STBC 0x004922c0 _$E348
// FUNCTION: STBC 0x004922d0 _$E347
// FUNCTION: STBC 0x004922e0 _$E352
// FUNCTION: STBC 0x004922f0 _$E351
// FUNCTION: STBC 0x00492300 _$E356
// FUNCTION: STBC 0x00492310 _$E355
// FUNCTION: STBC 0x00492320 _$E143
// FUNCTION: STBC 0x00492330 _$E142
// FUNCTION: STBC 0x00492340 _$E360
// FUNCTION: STBC 0x00492350 _$E359
// FUNCTION: STBC 0x00492360 _$E364
// FUNCTION: STBC 0x00492370 _$E363
// FUNCTION: STBC 0x004923b0 _$E368
// FUNCTION: STBC 0x004923c0 _$E367
// FUNCTION: STBC 0x004923d0 _$E149
// FUNCTION: STBC 0x004923e0 _$E148
// FUNCTION: STBC 0x004923f0 _$E154
// FUNCTION: STBC 0x00492400 _$E153
// FUNCTION: STBC 0x00492410 _$E184
// FUNCTION: STBC 0x00492420 _$E183
// FUNCTION: STBC 0x00492430 _$E372
// FUNCTION: STBC 0x00492440 _$E371
// FUNCTION: STBC 0x00492450 _$E189
// FUNCTION: STBC 0x00492460 _$E188
// FUNCTION: STBC 0x00492470 _$E376
// FUNCTION: STBC 0x00492480 _$E375
// FUNCTION: STBC 0x00492490 _$E380
// FUNCTION: STBC 0x004924a0 _$E379
// FUNCTION: STBC 0x004924b0 _$E194
// FUNCTION: STBC 0x004924c0 _$E193
// FUNCTION: STBC 0x004924d0 _$E384
// FUNCTION: STBC 0x004924e0 _$E383
// FUNCTION: STBC 0x004924f0 _$E159
// FUNCTION: STBC 0x00492500 _$E158
// FUNCTION: STBC 0x00492510 _$E388
// FUNCTION: STBC 0x00492520 _$E387
// FUNCTION: STBC 0x00492530 _$E392
// FUNCTION: STBC 0x00492540 _$E391
// FUNCTION: STBC 0x00492550 _$E199
// FUNCTION: STBC 0x00492560 _$E198
// FUNCTION: STBC 0x00492570 _$E204
// FUNCTION: STBC 0x00492580 _$E203
// FUNCTION: STBC 0x00492590 _$E396
// FUNCTION: STBC 0x004925a0 _$E395
// FUNCTION: STBC 0x004925b0 _$E400
// FUNCTION: STBC 0x004925c0 _$E399
// FUNCTION: STBC 0x004925d0 _$E404
// FUNCTION: STBC 0x004925e0 _$E403
// FUNCTION: STBC 0x004925f0 _$E408
// FUNCTION: STBC 0x00492600 _$E407
// FUNCTION: STBC 0x00492610 _$E412
// FUNCTION: STBC 0x00492620 _$E411
// FUNCTION: STBC 0x00492630 _$E416
// FUNCTION: STBC 0x00492640 _$E415
// FUNCTION: STBC 0x00492650 _$E420
// FUNCTION: STBC 0x00492660 _$E419
// FUNCTION: STBC 0x00492670 _$E424
// FUNCTION: STBC 0x00492680 _$E423
// FUNCTION: STBC 0x00492690 _$E209
// FUNCTION: STBC 0x004926a0 _$E208
// FUNCTION: STBC 0x004926b0 _$E428
// FUNCTION: STBC 0x004926c0 _$E427
// FUNCTION: STBC 0x004926d0 _$E214
// FUNCTION: STBC 0x004926e0 _$E213
// FUNCTION: STBC 0x004926f0 _$E219
// FUNCTION: STBC 0x00492700 _$E218
// FUNCTION: STBC 0x00492710 _$E432
// FUNCTION: STBC 0x00492720 _$E431
// FUNCTION: STBC 0x00492730 _$E437
// FUNCTION: STBC 0x00492740 _$E436
// FUNCTION: STBC 0x00492750 _$E441
// FUNCTION: STBC 0x00492760 _$E440

// FUNCTION: STBC 0x00492770 _$E444
// FUNCTION: STBC 0x00492780 _$E443
static AICreateFunctionRegistration sequenceAIRegistration( AI_TYPE_SEQUENCE, SequenceAI::Create );

/*
================
SequenceAI::SequenceAI
================
*/
// FUNCTION: STBC 0x004927d0 ??0SequenceAI@@QAE@PAVPhysicsObjectClass@@PAD@Z
SequenceAI::SequenceAI( PhysicsObjectClass *object, char *name_ ) : ArtificialIntelligence( object, name_ ),
	count( 0 ), current( -1 ), ais( NULL ), loopsLeft( 1 ), loopCount( 1 ),
	resetIfInterrupted( false ), doubleCheckAllDone( false ), skipDormant( false ) {
}

/*
================
SequenceAI::VirtualSlot15
================
*/
// FUNCTION: STBC 0x00492820
PyObject *SequenceAI::VirtualSlot15( void ) {
	return PyEmbed::Unknown6F9920( this, "SequenceAI" );
}

/*
================
SequenceAI::GetAIType
================
*/
// FUNCTION: STBC 0x00492830
int SequenceAI::GetAIType( void ) {
	return AI_TYPE_SEQUENCE;
}

/*
================
SequenceAI::VirtualSlot20
================
*/
// FUNCTION: STBC 0x00492840
bool SequenceAI::VirtualSlot20( int type ) {
	return type == AI_TYPE_SEQUENCE ? true : false;
}

/*
================
SequenceAI::~SequenceAI

Deletes the AIs.
================
*/
// FUNCTION: STBC 0x00492870
// FUNCTION: STBC 0x00492850 ??_GSequenceAI@@UAEPAXI@Z
SequenceAI::~SequenceAI( void ) {
	for ( int i = 0; i < count; i++ ) {
		delete ais[i];
	}
	delete [] ais;
}

/*
================
SequenceAI::AddAI

The AI's parent becomes this sequence.
================
*/
// FUNCTION: STBC 0x004928f0
void SequenceAI::AddAI( ArtificialIntelligence *ai ) {
	ArtificialIntelligence **newAIs = new ArtificialIntelligence *[count + 1];
	memcpy( newAIs, ais, count * sizeof( ArtificialIntelligence * ) );
	delete [] ais;
	ais = newAIs;
	ais[count] = ai->VirtualSlot13();
	ais[count]->SetParentID( id );
	count++;
}

/*
================
SequenceAI::RemoveAI
================
*/
// FUNCTION: STBC 0x00492960
void SequenceAI::RemoveAI( ArtificialIntelligence *ai ) {
	for ( int i = 0; i < count; i++ ) {
		if ( ais[i] == ai ) {
			RemoveAIByIndex( i );
			return;
		}
	}
}

/*
================
SequenceAI::RemoveAIByIndex
================
*/
// FUNCTION: STBC 0x00492990
void SequenceAI::RemoveAIByIndex( int index ) {
	if ( index >= 0 && index < count ) {
		ArtificialIntelligence *ai = ais[index];
		memmove( &ais[index], &ais[index + 1], ( count - ( index + 1 ) ) * sizeof( ArtificialIntelligence * ) );
		count--;
		ais[count] = NULL;
		delete ai;
	}
}

/*
================
SequenceAI::GetAI
================
*/
// FUNCTION: STBC 0x004929f0
ArtificialIntelligence *SequenceAI::GetAI( int index ) {
	if ( index >= 0 && index < count ) {
		return ais[index];
	}
	return NULL;
}

/*
================
SequenceAI::GetAllAIsInTree
================
*/
// FUNCTION: STBC 0x00492a10
void SequenceAI::GetAllAIsInTree( NiTList<ArtificialIntelligence *> &ais_ ) {
	ais_.AddTail( this );
	for ( int i = 0; i < count; i++ ) {
		ais[i]->GetAllAIsInTree( ais_ );
	}
}

/*
================
SequenceAI::GetFocusAIs
================
*/
// FUNCTION: STBC 0x00492a90
void SequenceAI::GetFocusAIs( NiTList<ArtificialIntelligence *> &ais_ ) {
	if ( focus ) {
		ais_.AddTail( this );
		for ( int i = 0; i < count; i++ ) {
			if ( ais[i]->HasFocus() ) {
				ais[i]->GetFocusAIs( ais_ );
			}
		}
	}
}

/*
================
SequenceAI::SetActive
================
*/
// FUNCTION: STBC 0x00492b10
void SequenceAI::SetActive( void ) {
	AILog log( "SequenceAI(%s)::SetActive", name );
	active = true;
	if ( current >= 0 && !ais[current]->IsActive() ) {
		ais[current]->SetActive();
	}
}

/*
================
SequenceAI::SetInactive
================
*/
// FUNCTION: STBC 0x00492b90
void SequenceAI::SetInactive( void ) {
	AILog log( "SequenceAI(%s)::SetInactive", name );
	active = false;
	if ( current >= 0 && ais[current]->IsActive() ) {
		ais[current]->SetInactive();
	}
	if ( resetIfInterrupted ) {
		Reset();
	}
}

/*
================
SequenceAI::GotFocus
================
*/
// FUNCTION: STBC 0x00492c20
void SequenceAI::GotFocus( void ) {
	AILog log( "SequenceAI(%s)::GotFocus", name );
	focus = true;
}

/*
================
SequenceAI::LostFocus
================
*/
// FUNCTION: STBC 0x00492c50
void SequenceAI::LostFocus( void ) {
	AILog log( "SequenceAI(%s)::LostFocus", name );
	if ( current >= 0 && ais[current]->HasFocus() ) {
		ais[current]->LostFocus();
	}
	focus = false;
}

/*
================
SequenceAI::IsInterruptable
================
*/
// FUNCTION: STBC 0x00492cd0
bool SequenceAI::IsInterruptable( void ) {
	bool result = true;
	result &= interruptable;
	if ( result && active && current >= 0 && current < count ) {
		result &= ais[current]->IsInterruptable();
	}
	return result;
}

/*
================
SequenceAI::Update

Updates the current AI and moves on while the AIs are done (or dormant
and skipped), each AI at most once per update. Any other status of the
current AI makes the sequence active.
================
*/
// FUNCTION: STBC 0x00492d00
int SequenceAI::Update( double time ) {
	AILog log( "SequenceAI(%s)::Update, contains %d", name, count );
	int updated = 0;
	int dormantCount = 0;
	while ( count != 0 && loopsLeft != 0 && updated < count ) {
		ArtificialIntelligence *ai;
		if ( current < 0 ) {
			ai = SetCurrentAI( 0 );
		} else {
			ai = ais[current];
		}
		if ( !ai->HasFocus() ) {
			ai->GotFocus();
		}

		int status = ai->Update( time );
		updated++;
		if ( status != US_DONE && ( status != US_DORMANT || !skipDormant ) ) {
			return US_ACTIVE;
		}

		ai->LostFocus();
		if ( status == US_DORMANT ) {
			dormantCount++;
		}
		int next = current + 1;
		if ( next >= count ) {
			if ( loopsLeft == LOOP_INFINITE || --loopsLeft > 0 ) {
				SetCurrentAI( 0 );
			}
		} else {
			SetCurrentAI( next );
		}
	}
	if ( dormantCount != 0 ) {
		return US_DORMANT;
	}

	if ( doubleCheckAllDone ) {
		doubleCheckAllDone = false;
		int status = Update( time );
		doubleCheckAllDone = true;
		return status;
	}
	return US_DONE;
}

/*
================
SequenceAI::Reset
================
*/
// FUNCTION: STBC 0x00492e90
void SequenceAI::Reset( void ) {
	loopsLeft = loopCount;
	SetCurrentAI( -1 );
}

/*
================
SequenceAI::IsDormant

Dormant when every AI is; an empty sequence is not.
================
*/
// FUNCTION: STBC 0x00492ea0
bool SequenceAI::IsDormant( void ) {
	AILog log( "SequenceAI(%s)::IsDormant (no easy check)", name );
	for ( int i = 0; i < count; i++ ) {
		if ( !ais[i]->IsDormant() ) {
			return false;
		}
	}
	if ( count == 0 ) {
		return false;
	}
	return true;
}

/*
================
SequenceAI::SetCurrentAI

Only a negative index is checked.
================
*/
// FUNCTION: STBC 0x00492f60
// PARKED: calls and logic match; retail keeps the NULL result in eax across the deactivation block (rematerialized after it) and saves edi only inside that block
ArtificialIntelligence *SequenceAI::SetCurrentAI( int index ) {
	if ( current >= 0 && current < count ) {
		ArtificialIntelligence *previous = ais[current];
		if ( previous->HasFocus() ) {
			previous->LostFocus();
		}
		if ( previous->IsActive() ) {
			previous->SetInactive();
		}
	}
	current = index;
	ArtificialIntelligence *ai = NULL;
	if ( index >= 0 ) {
		ai = ais[index];
		ai->Reset();
		ai->SetActive();
	}
	return ai;
}

/*
================
SequenceAI::WriteToStream
================
*/
// FUNCTION: STBC 0x00492fc0
bool SequenceAI::WriteToStream( TGStream *stream ) {
	bool result = true;
	result &= ArtificialIntelligence::WriteToStream( stream );
	if ( result ) {
		stream->WriteInt( count );
		stream->WriteInt( current );
		for ( int i = 0; i < count; i++ ) {
			result &= ais[i]->WriteToStream( stream );
		}
		stream->WriteInt( loopsLeft );
		stream->WriteInt( loopCount );
		stream->WriteChar( resetIfInterrupted );
		stream->WriteChar( doubleCheckAllDone );
		stream->WriteChar( skipDormant );
	}
	return result;
}

/*
================
SequenceAI::SequenceAI
================
*/
// FUNCTION: STBC 0x00493060 ??0SequenceAI@@QAE@PAVTGStream@@PA_N@Z
SequenceAI::SequenceAI( TGStream *stream, bool *ok ) : ArtificialIntelligence( stream, ok ),
	count( 0 ), current( -1 ), ais( NULL ), loopsLeft( 1 ), loopCount( 1 ),
	resetIfInterrupted( false ), doubleCheckAllDone( false ), skipDormant( false ) {
	if ( *ok ) {
		count = stream->ReadInt();
		current = stream->ReadInt();
		ais = new ArtificialIntelligence *[count];
		for ( int i = 0; i < count; i++ ) {
			ais[i] = CreateFromStream( stream );
			if ( ais[i] == NULL ) {
				*ok = false;
			}
		}
		loopsLeft = stream->ReadInt();
		loopCount = stream->ReadInt();

		char flag = stream->ReadChar();
		if ( flag == true || flag == false ) {
			resetIfInterrupted = flag != 0;
		} else {
			*ok = false;
		}
		flag = stream->ReadChar();
		if ( flag == true || flag == false ) {
			doubleCheckAllDone = flag != 0;
		} else {
			*ok = false;
		}
		flag = stream->ReadChar();
		if ( flag == true || flag == false ) {
			skipDormant = flag != 0;
		} else {
			*ok = false;
		}
	}
}

/*
================
SequenceAI::Create
================
*/
// FUNCTION: STBC 0x004931b0
ArtificialIntelligence *SequenceAI::Create( TGStream *stream ) {
	bool ok = true;
	SequenceAI *ai = new SequenceAI( stream, &ok );
	if ( !ok ) {
		delete ai;
		ai = NULL;
	}
	return ai;
}

/*
================
SequenceAI::FixUpPointers
================
*/
// FUNCTION: STBC 0x00493230
bool SequenceAI::FixUpPointers( void ) {
	for ( int i = 0; i < count; i++ ) {
		if ( ais[i] != NULL && !ais[i]->FixUpPointers() ) {
			return false;
		}
	}
	return true;
}

/*
================
SequenceAI::PostFixUp
================
*/
// FUNCTION: STBC 0x00493270
void SequenceAI::PostFixUp( void ) {
	for ( int i = 0; i < count; i++ ) {
		if ( ais[i] != NULL ) {
			ais[i]->PostFixUp();
		}
	}
}
