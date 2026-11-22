/*
===============================================================================

	BuilderAI.cpp

===============================================================================
*/

#include <iostream>
#include <string.h>

#include "BuilderAI.h"
#include "NiMath.h"
#include "PhysicsObjectClass.h"
#include "PyEmbed.h"
#include "TGStream.h"
#include "TGSystemWrapperClass.h"
#include "UtopiaModule.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// An AI block of a BuilderAI, filed under its name: the script function
// that builds the block's AI, and what the function is passed. The name is
// invented. Only this unit uses it, so it is defined here, after the
// headers: its destructor's out-of-line copy (0x4765C0) follows
// DeleteBlocks, its first out-of-line caller, and precedes AddAIBlock, which
// then sees the list destructors as non-throwing. Under /YX a definition
// in the precompiled headers is compiled at the unit's end instead.
// FUNCTION: STBC 0x004765c0 ??1BuilderAIBlock@@QAE@XZ
struct BuilderAIBlock {
					BuilderAIBlock( void ) : dependencies( 2 ), objectNames( 2 ) { function = NULL; ai = NULL; }
					~BuilderAIBlock( void ) {
						delete [] function;
						while ( !dependencies.IsEmpty() ) {
							delete [] dependencies.RemoveHead();
						}
						while ( !objectNames.IsEmpty() ) {
							delete [] objectNames.RemoveHead();
						}
					}

	char *			function;				// 0x00 owned: the name of the script function (invented name)
	NiTList<char *>	dependencies;			// 0x04 owned names of the blocks whose AIs the function is passed (AddDependency)
	NiTList<char *>	objectNames;			// 0x1C owned names of the dependency objects the function is passed (invented name)
	ArtificialIntelligence *	ai;			// 0x34 the AI built for the block; NULL until built
};

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00478600 _$E477
// FUNCTION: STBC 0x00478620 _$E476

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00477ad0 _$E32
// FUNCTION: STBC 0x00477ae0 _$E29
// FUNCTION: STBC 0x00477af0 _$E31
// FUNCTION: STBC 0x00477b00 _$E30
// FUNCTION: STBC 0x00477b10 _$E38
// FUNCTION: STBC 0x00477b20 _$E35
// FUNCTION: STBC 0x00477b30 _$E37
// FUNCTION: STBC 0x00477b40 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00477b50 _$E90
// FUNCTION: STBC 0x00477b60 _$E89
// FUNCTION: STBC 0x00477b70 _$E250
// FUNCTION: STBC 0x00477b80 _$E249
// FUNCTION: STBC 0x00477b90 _$E101
// FUNCTION: STBC 0x00477ba0 _$E100
// FUNCTION: STBC 0x00477bb0 _$E105
// FUNCTION: STBC 0x00477bc0 _$E104
// FUNCTION: STBC 0x00477bd0 _$E109
// FUNCTION: STBC 0x00477be0 _$E108
// FUNCTION: STBC 0x00477bf0 _$E113
// FUNCTION: STBC 0x00477c00 _$E112
// FUNCTION: STBC 0x00477c10 _$E230
// FUNCTION: STBC 0x00477c20 _$E229
// FUNCTION: STBC 0x00477c30 _$E172
// FUNCTION: STBC 0x00477c40 _$E171
// FUNCTION: STBC 0x00477c50 _$E185
// FUNCTION: STBC 0x00477c60 _$E184
// FUNCTION: STBC 0x00477c70 _$E235
// FUNCTION: STBC 0x00477c80 _$E234
// FUNCTION: STBC 0x00477d40 _$E255
// FUNCTION: STBC 0x00477d50 _$E254
// FUNCTION: STBC 0x00477d60 _$E239
// FUNCTION: STBC 0x00477d70 _$E238
// FUNCTION: STBC 0x00477d80 _$E259
// FUNCTION: STBC 0x00477d90 _$E258
// FUNCTION: STBC 0x00477da0 _$E266
// FUNCTION: STBC 0x00477db0 _$E265
// FUNCTION: STBC 0x00477dc0 _$E299
// FUNCTION: STBC 0x00477dd0 _$E298
// FUNCTION: STBC 0x00477de0 _$E303
// FUNCTION: STBC 0x00477df0 _$E302
// FUNCTION: STBC 0x00477e00 _$E130
// FUNCTION: STBC 0x00477e10 _$E129
// FUNCTION: STBC 0x00477e20 _$E285
// FUNCTION: STBC 0x00477e30 _$E284
// FUNCTION: STBC 0x00477ed0 _$E292
// FUNCTION: STBC 0x00477ee0 _$E291
// FUNCTION: STBC 0x00477ef0 _$E315
// FUNCTION: STBC 0x00477f00 _$E314
// FUNCTION: STBC 0x00477f20 _$E319
// FUNCTION: STBC 0x00477f30 _$E318
// FUNCTION: STBC 0x00478060 _$E359
// FUNCTION: STBC 0x00478070 _$E358
// FUNCTION: STBC 0x00478140 _$E365
// FUNCTION: STBC 0x00478150 _$E364
// FUNCTION: STBC 0x00478160 _$E307
// FUNCTION: STBC 0x00478170 _$E306
// FUNCTION: STBC 0x00478180 _$E311
// FUNCTION: STBC 0x00478190 _$E310
// FUNCTION: STBC 0x004781a0 _$E323
// FUNCTION: STBC 0x004781b0 _$E322
// FUNCTION: STBC 0x004781c0 _$E327
// FUNCTION: STBC 0x004781d0 _$E326
// FUNCTION: STBC 0x004781e0 _$E331
// FUNCTION: STBC 0x004781f0 _$E330
// FUNCTION: STBC 0x00478200 _$E335
// FUNCTION: STBC 0x00478210 _$E334
// FUNCTION: STBC 0x00478220 _$E339
// FUNCTION: STBC 0x00478230 _$E338
// FUNCTION: STBC 0x00478240 _$E343
// FUNCTION: STBC 0x00478250 _$E342
// FUNCTION: STBC 0x00478300 _$E347
// FUNCTION: STBC 0x00478310 _$E346
// FUNCTION: STBC 0x00478320 _$E351
// FUNCTION: STBC 0x00478330 _$E350
// FUNCTION: STBC 0x00478340 _$E135
// FUNCTION: STBC 0x00478350 _$E134
// FUNCTION: STBC 0x00478360 _$E355
// FUNCTION: STBC 0x00478370 _$E354
// FUNCTION: STBC 0x00478380 _$E140
// FUNCTION: STBC 0x00478390 _$E139
// FUNCTION: STBC 0x004783a0 _$E369
// FUNCTION: STBC 0x004783b0 _$E368
// FUNCTION: STBC 0x004783c0 _$E373
// FUNCTION: STBC 0x004783d0 _$E372
// FUNCTION: STBC 0x004783e0 _$E377
// FUNCTION: STBC 0x004783f0 _$E376
// FUNCTION: STBC 0x00478400 _$E381
// FUNCTION: STBC 0x00478410 _$E380
// FUNCTION: STBC 0x00478420 _$E149
// FUNCTION: STBC 0x00478430 _$E148
// FUNCTION: STBC 0x00478440 _$E385
// FUNCTION: STBC 0x00478450 _$E384
// FUNCTION: STBC 0x00478460 _$E389
// FUNCTION: STBC 0x00478470 _$E388
// FUNCTION: STBC 0x00478580 _$E393
// FUNCTION: STBC 0x00478590 _$E392
// FUNCTION: STBC 0x004785a0 _$E155
// FUNCTION: STBC 0x004785b0 _$E154
// FUNCTION: STBC 0x004785c0 _$E160
// FUNCTION: STBC 0x004785d0 _$E159
// FUNCTION: STBC 0x004785e0 _$E190
// FUNCTION: STBC 0x004785f0 _$E189
// FUNCTION: STBC 0x004786b0 _$E397
// FUNCTION: STBC 0x004786c0 _$E396
// FUNCTION: STBC 0x004786d0 _$E195
// FUNCTION: STBC 0x004786e0 _$E194
// FUNCTION: STBC 0x004786f0 _$E401
// FUNCTION: STBC 0x00478700 _$E400
// FUNCTION: STBC 0x00478710 _$E405
// FUNCTION: STBC 0x00478720 _$E404
// FUNCTION: STBC 0x00478730 _$E200
// FUNCTION: STBC 0x00478740 _$E199
// FUNCTION: STBC 0x00478750 _$E409
// FUNCTION: STBC 0x00478760 _$E408
// FUNCTION: STBC 0x00478770 _$E165
// FUNCTION: STBC 0x00478780 _$E164
// FUNCTION: STBC 0x00478790 _$E413
// FUNCTION: STBC 0x004787a0 _$E412
// FUNCTION: STBC 0x004787b0 _$E417
// FUNCTION: STBC 0x004787c0 _$E416
// FUNCTION: STBC 0x004787d0 _$E205
// FUNCTION: STBC 0x004787e0 _$E204
// FUNCTION: STBC 0x004787f0 _$E210
// FUNCTION: STBC 0x00478800 _$E209
// FUNCTION: STBC 0x00478810 _$E421
// FUNCTION: STBC 0x00478820 _$E420
// FUNCTION: STBC 0x00478830 _$E425
// FUNCTION: STBC 0x00478840 _$E424
// FUNCTION: STBC 0x00478850 _$E429
// FUNCTION: STBC 0x00478860 _$E428
// FUNCTION: STBC 0x00478870 _$E433
// FUNCTION: STBC 0x00478880 _$E432
// FUNCTION: STBC 0x00478890 _$E437
// FUNCTION: STBC 0x004788a0 _$E436
// FUNCTION: STBC 0x004788b0 _$E441
// FUNCTION: STBC 0x004788c0 _$E440
// FUNCTION: STBC 0x004788d0 _$E445
// FUNCTION: STBC 0x004788e0 _$E444
// FUNCTION: STBC 0x004788f0 _$E449
// FUNCTION: STBC 0x00478900 _$E448
// FUNCTION: STBC 0x00478910 _$E215
// FUNCTION: STBC 0x00478920 _$E214
// FUNCTION: STBC 0x00478930 _$E453
// FUNCTION: STBC 0x00478940 _$E452
// FUNCTION: STBC 0x00478950 _$E220
// FUNCTION: STBC 0x00478960 _$E219
// FUNCTION: STBC 0x00478970 _$E225
// FUNCTION: STBC 0x00478980 _$E224
// FUNCTION: STBC 0x00478990 _$E457
// FUNCTION: STBC 0x004789a0 _$E456
// FUNCTION: STBC 0x004789b0 _$E462
// FUNCTION: STBC 0x004789c0 _$E461
// FUNCTION: STBC 0x004789d0 _$E466
// FUNCTION: STBC 0x004789e0 _$E465

// FUNCTION: STBC 0x00475f50 _$E469
// FUNCTION: STBC 0x00475f60 _$E468
static AICreateFunctionRegistration builderAIRegistration( AI_TYPE_BUILDER, BuilderAI::Create );

/*
================
BuilderAI::BuilderAI
================
*/
// FUNCTION: STBC 0x00475fb0 ??0BuilderAI@@QAE@PAVPhysicsObjectClass@@PAD1@Z
BuilderAI::BuilderAI( PhysicsObjectClass *object, char *name_, char *moduleName_ ) : PreprocessingAI( object, name_ ), pendingBlocks( 2 ) {
	moduleName = new char[strlen( moduleName_ ) + 1];
	strcpy( moduleName, moduleName_ );
}

/*
================
BuilderAI::VirtualSlot15
================
*/
// FUNCTION: STBC 0x00476100
PyObject *BuilderAI::VirtualSlot15( void ) {
	return PyEmbed::Unknown6F9920( this, "BuilderAI" );
}

/*
================
BuilderAI::GetAIType
================
*/
// FUNCTION: STBC 0x00476110
int BuilderAI::GetAIType( void ) {
	return AI_TYPE_BUILDER;
}

/*
================
BuilderAI::VirtualSlot20
================
*/
// FUNCTION: STBC 0x00476120
bool BuilderAI::VirtualSlot20( int type ) {
	if ( type == AI_TYPE_BUILDER ) {
		return true;
	}
	return PreprocessingAI::VirtualSlot20( type );
}

/*
================
BuilderAI::~BuilderAI
================
*/
// FUNCTION: STBC 0x00476160
BuilderAI::~BuilderAI( void ) {
	delete [] moduleName;
	DeleteBlocks();
}

/*
================
BuilderAI::DeleteBlocks

The AIs are found again by ID after the blocks are gone, and each is
deleted from the root of its tree.
================
*/
// FUNCTION: STBC 0x00476340
void BuilderAI::DeleteBlocks( void ) {
	if ( blocks.GetCount() != 0 ) {
		long *aiIDs = new long[blocks.GetCount()];
		int numAIs = 0;
		NiTMapIterator pos = blocks.GetFirstPos();
		while ( pos != NULL ) {
			const char *blockName;
			BuilderAIBlock *block;
			blocks.GetNext( pos, blockName, block );
			if ( block->ai != NULL ) {
				aiIDs[numAIs++] = block->ai->GetID();
			}
			delete block;
		}

		while ( --numAIs >= 0 ) {
			ArtificialIntelligence *ai = ArtificialIntelligence::GetAIByID( aiIDs[numAIs] );
			if ( ai != NULL ) {
				while ( true ) {
					ArtificialIntelligence *parent = ArtificialIntelligence::GetAIByID( ai->GetParentID() );
					if ( parent == NULL ) {
						break;
					}
					ai = parent;
				}
				delete ai;
			}
		}
		delete [] aiIDs;
		blocks.RemoveAll();
	}

	while ( !pendingBlocks.IsEmpty() ) {
		delete [] pendingBlocks.RemoveHead();
	}

	NiTMapIterator pos = dependencyObjects.GetFirstPos();
	while ( pos != NULL ) {
		const char *functionName;
		PyObject *function;
		dependencyObjects.GetNext( pos, functionName, function );
		Py_DECREF( function );
	}
	dependencyObjects.RemoveAll();
}

/*
================
BuilderAI::AddAIBlock

Files a block under its name, replacing a block of the name, and queues
it to be built.
================
*/
// FUNCTION: STBC 0x004766c0
void BuilderAI::AddAIBlock( char *blockName, char *function ) {
	BuilderAIBlock *block;
	if ( blocks.GetAt( blockName, block ) ) {
		delete block;
	}

	block = new BuilderAIBlock;
	block->function = new char[strlen( function ) + 1];
	strcpy( block->function, function );
	blocks.SetAt( blockName, block );

	char *name = new char[strlen( blockName ) + 1];
	strcpy( name, blockName );
	pendingBlocks.AddTail( name );
}

/*
================
BuilderAI::AddDependency

The block's function is passed the AI of the dependency's block.
================
*/
// FUNCTION: STBC 0x00476940
void BuilderAI::AddDependency( char *blockName, char *dependency ) {
	BuilderAIBlock *block;
	if ( blocks.GetAt( blockName, block ) ) {
		char *name = new char[strlen( dependency ) + 1];
		strcpy( name, dependency );
		block->dependencies.AddTail( name );
	}
}

/*
================
BuilderAI::AddDependencyObject
================
*/
// FUNCTION: STBC 0x00476a20
void BuilderAI::AddDependencyObject( char *blockName, char *objectName, PyObject *object ) {
	BuilderAIBlock *block;
	if ( blocks.GetAt( blockName, block ) ) {
		char *name = new char[strlen( objectName ) + 1];
		strcpy( name, objectName );
		block->objectNames.AddTail( name );
		Py_INCREF( object );
		dependencyObjects.SetAt( name, object );
	}
}

/*
================
BuilderAI::CallPreprocessingMethod

Builds the queued blocks while the frame has time left, at least one a
call. When the last is built, its AI becomes the contained AI and the
blocks are deleted, but not the AIs built for them.
================
*/
// FUNCTION: STBC 0x00476b80
int BuilderAI::CallPreprocessingMethod( double time ) {
	if ( !forceUpdate && g_kUtopiaModule.GetGameTime() < nextUpdateTime ) {
		return preprocessStatus;
	}
	if ( containedAI != NULL ) {
		nextUpdateTime = NI_INFINITY;
		return PS_NORMAL;
	}

	do {
		char *blockName = pendingBlocks.RemoveHead();
		ArtificialIntelligence *ai = BuildBlock( blockName );
		if ( ai == NULL ) {
			return PS_DONE;
		}
		if ( pendingBlocks.IsEmpty() ) {
			SetContainedAI( ai );
			NiTMapIterator pos = blocks.GetFirstPos();
			while ( pos != NULL ) {
				const char *name;
				BuilderAIBlock *block;
				blocks.GetNext( pos, name, block );
				block->ai = NULL;
			}
			DeleteBlocks();
			time = -1.0;
		}
		delete [] blockName;
	} while ( g_kSystemWrapper.GetTimeSinceFrameStart() < time );
	return PS_SKIP_ACTIVE;
}

/*
================
BuilderAI::BuildBlock
================
*/
// FUNCTION: STBC 0x00476ce0
ArtificialIntelligence *BuilderAI::BuildBlock( char *blockName ) {
	BuilderAIBlock *block;
	if ( !blocks.GetAt( blockName, block ) ) {
		return NULL;
	}

	PyObject *arguments = PyTuple_New( block->dependencies.GetSize() + block->objectNames.GetSize() + 1 );
	PyTuple_SetItem( arguments, 0, GetObject()->VirtualSlot08() );
	int index = 1;

	NiTListIterator pos = block->dependencies.GetHeadPos();
	while ( pos != NULL ) {
		BuilderAIBlock *dependency;
		if ( !blocks.GetAt( block->dependencies.GetNext( pos ), dependency ) || dependency->ai == NULL ) {
			return NULL;
		}
		PyTuple_SetItem( arguments, index++, dependency->ai->VirtualSlot15() );
	}

	pos = block->objectNames.GetHeadPos();
	while ( pos != NULL ) {
		PyObject *object;
		if ( !dependencyObjects.GetAt( block->objectNames.GetNext( pos ), object ) ) {
			return NULL;
		}
		Py_INCREF( object );
		PyTuple_SetItem( arguments, index++, object );
	}

	PyObject *function = NULL;
	if ( PyEmbed::GetValue( moduleName, block->function, "O", &function ) == -1 ) {
		return NULL;
	}
	PyObject *result;
	if ( PyEmbed::CallObject( function, "O", &result, arguments ) == -1 ) {
		PyErr_Print();
		return NULL;
	}
	Py_DECREF( function );
	PyEmbed::Unknown6F98E0( result, (void **)&block->ai, "ArtificialIntelligence" );
	Py_DECREF( result );
	if ( block->ai != NULL ) {
		block->ai = block->ai->VirtualSlot13();
	}
	return block->ai;
}

/*
================
BuilderAI::WriteToStream
================
*/
// FUNCTION: STBC 0x00476f30
bool BuilderAI::WriteToStream( TGStream *stream ) {
	if ( !PreprocessingAI::WriteToStream( stream ) ) {
		return false;
	}

	stream->VirtualSlot11( moduleName );
	stream->WriteInt( pendingBlocks.GetSize() );
	NiTListIterator pos = pendingBlocks.GetHeadPos();
	while ( pos != NULL ) {
		stream->VirtualSlot11( pendingBlocks.GetNext( pos ) );
	}

	stream->WriteInt( blocks.GetCount() );
	NiTMapIterator blockPos = blocks.GetFirstPos();
	while ( blockPos != NULL ) {
		const char *blockName;
		BuilderAIBlock *block;
		blocks.GetNext( blockPos, blockName, block );
		stream->VirtualSlot11( (char *)blockName );
		stream->VirtualSlot11( block->function );
		if ( block->ai != NULL ) {
			stream->WriteBool( true );
			if ( !block->ai->WriteToStream( stream ) ) {
				return false;
			}
		} else {
			stream->WriteBool( false );
		}
		stream->WriteInt( block->dependencies.GetSize() );
		pos = block->dependencies.GetHeadPos();
		while ( pos != NULL ) {
			stream->VirtualSlot11( block->dependencies.GetNext( pos ) );
		}
		stream->WriteInt( block->objectNames.GetSize() );
		pos = block->objectNames.GetHeadPos();
		while ( pos != NULL ) {
			stream->VirtualSlot11( block->objectNames.GetNext( pos ) );
		}
	}

	stream->WriteInt( dependencyObjects.GetCount() );
	NiTMapIterator objectPos = dependencyObjects.GetFirstPos();
	while ( objectPos != NULL ) {
		const char *name;
		PyObject *object;
		dependencyObjects.GetNext( objectPos, name, object );
		stream->VirtualSlot11( (char *)name );
		if ( !PyEmbed::SerializePython( object, stream, 0 ) ) {
			return false;
		}
	}
	return true;
}

/*
================
BuilderAI::BuilderAI

Reads what WriteToStream wrote, after the type; ok is cleared on a
failure.
================
*/
// FUNCTION: STBC 0x00477160 ??0BuilderAI@@QAE@PAVTGStream@@PA_N@Z
BuilderAI::BuilderAI( TGStream *stream, bool *ok ) : PreprocessingAI( stream, ok ), pendingBlocks( 2 ) {
	if ( !*ok ) {
		return;
	}

	moduleName = stream->VirtualSlot10( -1 );
	int numPending = stream->ReadInt();
	while ( --numPending >= 0 ) {
		pendingBlocks.AddTail( stream->VirtualSlot10( -1 ) );
	}

	int numBlocks = stream->ReadInt();
	while ( --numBlocks >= 0 ) {
		BuilderAIBlock *block = new BuilderAIBlock;
		char *blockName = stream->VirtualSlot10( -1 );
		block->function = stream->VirtualSlot10( -1 );
		if ( stream->ReadBool() ) {
			block->ai = CreateFromStream( stream );
			if ( block->ai == NULL ) {
				*ok = false;
				return;
			}
		} else {
			block->ai = NULL;
		}
		int numDependencies = stream->ReadInt();
		while ( --numDependencies >= 0 ) {
			block->dependencies.AddTail( stream->VirtualSlot10( -1 ) );
		}
		int numObjectNames = stream->ReadInt();
		while ( --numObjectNames >= 0 ) {
			block->objectNames.AddTail( stream->VirtualSlot10( -1 ) );
		}
		blocks.SetAt( blockName, block );
		delete [] blockName;
	}

	int numObjects = stream->ReadInt();
	while ( --numObjects >= 0 && *ok ) {
		char *name = stream->VirtualSlot10( -1 );
		PyObject *object = PyEmbed::UnserializePython( stream, 0 );
		if ( object == NULL ) {
			*ok = false;
		}
		dependencyObjects.SetAt( name, object );
		delete [] name;
	}
}

/*
================
BuilderAI::FixUpPointers

Fixes up the tree of each block's AI once, from its root.
================
*/
// FUNCTION: STBC 0x00477590
bool BuilderAI::FixUpPointers( void ) {
	if ( !PreprocessingAI::FixUpPointers() ) {
		return false;
	}

	NiTList<long> fixedTrees( 2 );
	if ( blocks.GetCount() != 0 ) {
		NiTMapIterator pos = blocks.GetFirstPos();
		while ( pos != NULL ) {
			const char *blockName;
			BuilderAIBlock *block;
			blocks.GetNext( pos, blockName, block );
			ArtificialIntelligence *root = block->ai;
			if ( root == NULL ) {
				continue;
			}
			ArtificialIntelligence *parent;
			while ( ( parent = ArtificialIntelligence::GetAIByID( root->GetParentID() ) ) != NULL ) {
				root = parent;
			}
			if ( fixedTrees.FindPos( root->GetID() ) == NULL ) {
				if ( !root->FixUpPointers() ) {
					return false;
				}
				fixedTrees.AddTail( root->GetID() );
			}
		}
	}
	return true;
}

/*
================
BuilderAI::PostFixUp
================
*/
// FUNCTION: STBC 0x004777d0
void BuilderAI::PostFixUp( void ) {
	PreprocessingAI::PostFixUp();

	NiTList<long> fixedTrees( 2 );
	if ( blocks.GetCount() != 0 ) {
		NiTMapIterator pos = blocks.GetFirstPos();
		while ( pos != NULL ) {
			const char *blockName;
			BuilderAIBlock *block;
			blocks.GetNext( pos, blockName, block );
			ArtificialIntelligence *root = block->ai;
			if ( root == NULL ) {
				continue;
			}
			ArtificialIntelligence *parent;
			while ( ( parent = ArtificialIntelligence::GetAIByID( root->GetParentID() ) ) != NULL ) {
				root = parent;
			}
			if ( fixedTrees.FindPos( root->GetID() ) == NULL ) {
				root->PostFixUp();
				fixedTrees.AddTail( root->GetID() );
			}
		}
	}
}

/*
================
BuilderAI::Create
================
*/
// FUNCTION: STBC 0x004779b0
ArtificialIntelligence *BuilderAI::Create( TGStream *stream ) {
	bool ok = true;
	BuilderAI *ai = new BuilderAI( stream, &ok );
	if ( !ok ) {
		delete ai;
		ai = NULL;
	}
	return ai;
}

/*
================
BuilderAI_AddDependencyObject
================
*/
// FUNCTION: STBC 0x00477a30
PyObject *BuilderAI_AddDependencyObject( PyObject *self, PyObject *args ) {
	PyObject *aiInstance = NULL;
	char *blockName = NULL;
	char *objectName = NULL;
	PyObject *object = NULL;
	if ( !PyArg_ParseTuple( args, "OssO:BuilderAI_AddDependencyObject", &aiInstance, &blockName, &objectName, &object ) ) {
		return NULL;
	}
	BuilderAI *ai = NULL;
	PyEmbed::Unknown6F98E0( aiInstance, (void **)&ai, "BuilderAI" );
	if ( ai == NULL ) {
		return NULL;
	}
	ai->AddDependencyObject( blockName, objectName, object );
	Py_INCREF( Py_None );
	return Py_None;
}
