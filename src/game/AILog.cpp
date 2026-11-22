/*
===============================================================================

	AILog.cpp

	The AI tree log's scopes. See AILog.h.

===============================================================================
*/

#include <iostream>
#include <stdarg.h>
#include <string.h>

#include "AILog.h"
#include "PyEmbed.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// A static object's construction and exit-time destruction.
// FUNCTION: STBC 0x004744f0 _$E444
// FUNCTION: STBC 0x00474500 _$E440
// FUNCTION: STBC 0x00474510 _$E443
// FUNCTION: STBC 0x00474520 _$E441

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00475a80 _$E452
// FUNCTION: STBC 0x00475aa0 _$E451

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x004750c0 _$E32
// FUNCTION: STBC 0x004750d0 _$E29
// FUNCTION: STBC 0x004750e0 _$E31
// FUNCTION: STBC 0x004750f0 _$E30
// FUNCTION: STBC 0x00475100 _$E38
// FUNCTION: STBC 0x00475110 _$E35
// FUNCTION: STBC 0x00475120 _$E37
// FUNCTION: STBC 0x00475130 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00475140 _$E70
// FUNCTION: STBC 0x00475150 _$E69
// FUNCTION: STBC 0x00475160 _$E74
// FUNCTION: STBC 0x00475170 _$E73
// FUNCTION: STBC 0x00475180 _$E85
// FUNCTION: STBC 0x00475190 _$E84
// FUNCTION: STBC 0x004751a0 _$E89
// FUNCTION: STBC 0x004751b0 _$E88
// FUNCTION: STBC 0x004751c0 _$E93
// FUNCTION: STBC 0x004751d0 _$E92
// FUNCTION: STBC 0x004751e0 _$E97
// FUNCTION: STBC 0x004751f0 _$E96
// FUNCTION: STBC 0x00475200 _$E221
// FUNCTION: STBC 0x00475210 _$E220
// FUNCTION: STBC 0x00475220 _$E163
// FUNCTION: STBC 0x00475230 _$E162
// FUNCTION: STBC 0x00475240 _$E176
// FUNCTION: STBC 0x00475250 _$E175
// FUNCTION: STBC 0x00475260 _$E226
// FUNCTION: STBC 0x00475270 _$E225
// FUNCTION: STBC 0x00475280 _$E231
// FUNCTION: STBC 0x00475290 _$E230
// FUNCTION: STBC 0x004752a0 _$E235
// FUNCTION: STBC 0x004752b0 _$E234
// FUNCTION: STBC 0x004753c0 _$E239
// FUNCTION: STBC 0x004753d0 _$E238
// FUNCTION: STBC 0x00475620 _$E246
// FUNCTION: STBC 0x00475630 _$E245
// FUNCTION: STBC 0x00475640 _$E271
// FUNCTION: STBC 0x00475650 _$E270
// FUNCTION: STBC 0x00475660 _$E275
// FUNCTION: STBC 0x00475670 _$E274
// FUNCTION: STBC 0x00475680 _$E121
// FUNCTION: STBC 0x00475690 _$E120
// FUNCTION: STBC 0x004756a0 _$E257
// FUNCTION: STBC 0x004756b0 _$E256
// FUNCTION: STBC 0x00475760 _$E264
// FUNCTION: STBC 0x00475770 _$E263
// FUNCTION: STBC 0x00475780 _$E287
// FUNCTION: STBC 0x00475790 _$E286
// FUNCTION: STBC 0x004757a0 _$E291
// FUNCTION: STBC 0x004757b0 _$E290
// FUNCTION: STBC 0x004757c0 _$E331
// FUNCTION: STBC 0x004757d0 _$E330
// FUNCTION: STBC 0x004757e0 _$E337
// FUNCTION: STBC 0x004757f0 _$E336
// FUNCTION: STBC 0x00475800 _$E279
// FUNCTION: STBC 0x00475810 _$E278
// FUNCTION: STBC 0x00475820 _$E283
// FUNCTION: STBC 0x00475830 _$E282
// FUNCTION: STBC 0x00475840 _$E295
// FUNCTION: STBC 0x00475850 _$E294
// FUNCTION: STBC 0x00475860 _$E299
// FUNCTION: STBC 0x00475870 _$E298
// FUNCTION: STBC 0x00475880 _$E303
// FUNCTION: STBC 0x00475890 _$E302
// FUNCTION: STBC 0x004758a0 _$E307
// FUNCTION: STBC 0x004758b0 _$E306
// FUNCTION: STBC 0x004758c0 _$E311
// FUNCTION: STBC 0x004758d0 _$E310
// FUNCTION: STBC 0x004758e0 _$E315
// FUNCTION: STBC 0x004758f0 _$E314
// FUNCTION: STBC 0x00475900 _$E319
// FUNCTION: STBC 0x00475910 _$E318
// FUNCTION: STBC 0x00475920 _$E323
// FUNCTION: STBC 0x00475930 _$E322
// FUNCTION: STBC 0x00475940 _$E126
// FUNCTION: STBC 0x00475950 _$E125
// FUNCTION: STBC 0x00475960 _$E327
// FUNCTION: STBC 0x00475970 _$E326
// FUNCTION: STBC 0x00475a60 _$E131
// FUNCTION: STBC 0x00475a70 _$E130
// FUNCTION: STBC 0x00475ab0 _$E341
// FUNCTION: STBC 0x00475ac0 _$E340
// FUNCTION: STBC 0x00475ad0 _$E345
// FUNCTION: STBC 0x00475ae0 _$E344
// FUNCTION: STBC 0x00475af0 _$E349
// FUNCTION: STBC 0x00475b00 _$E348
// FUNCTION: STBC 0x00475b10 _$E353
// FUNCTION: STBC 0x00475b20 _$E352
// FUNCTION: STBC 0x00475b30 _$E140
// FUNCTION: STBC 0x00475b40 _$E139
// FUNCTION: STBC 0x00475b50 _$E357
// FUNCTION: STBC 0x00475b60 _$E356
// FUNCTION: STBC 0x00475b70 _$E361
// FUNCTION: STBC 0x00475b80 _$E360
// FUNCTION: STBC 0x00475b90 _$E365
// FUNCTION: STBC 0x00475ba0 _$E364
// FUNCTION: STBC 0x00475bb0 _$E146
// FUNCTION: STBC 0x00475bc0 _$E145
// FUNCTION: STBC 0x00475bd0 _$E151
// FUNCTION: STBC 0x00475be0 _$E150
// FUNCTION: STBC 0x00475bf0 _$E181
// FUNCTION: STBC 0x00475c00 _$E180
// FUNCTION: STBC 0x00475c10 _$E369
// FUNCTION: STBC 0x00475c20 _$E368
// FUNCTION: STBC 0x00475c30 _$E186
// FUNCTION: STBC 0x00475c40 _$E185
// FUNCTION: STBC 0x00475c50 _$E373
// FUNCTION: STBC 0x00475c60 _$E372
// FUNCTION: STBC 0x00475c70 _$E377
// FUNCTION: STBC 0x00475c80 _$E376
// FUNCTION: STBC 0x00475c90 _$E191
// FUNCTION: STBC 0x00475ca0 _$E190
// FUNCTION: STBC 0x00475cb0 _$E381
// FUNCTION: STBC 0x00475cc0 _$E380
// FUNCTION: STBC 0x00475cd0 _$E156
// FUNCTION: STBC 0x00475ce0 _$E155
// FUNCTION: STBC 0x00475cf0 _$E385
// FUNCTION: STBC 0x00475d00 _$E384
// FUNCTION: STBC 0x00475d10 _$E389
// FUNCTION: STBC 0x00475d20 _$E388
// FUNCTION: STBC 0x00475d30 _$E196
// FUNCTION: STBC 0x00475d40 _$E195
// FUNCTION: STBC 0x00475d50 _$E201
// FUNCTION: STBC 0x00475d60 _$E200
// FUNCTION: STBC 0x00475d70 _$E393
// FUNCTION: STBC 0x00475d80 _$E392
// FUNCTION: STBC 0x00475d90 _$E397
// FUNCTION: STBC 0x00475da0 _$E396
// FUNCTION: STBC 0x00475db0 _$E401
// FUNCTION: STBC 0x00475dc0 _$E400
// FUNCTION: STBC 0x00475dd0 _$E405
// FUNCTION: STBC 0x00475de0 _$E404
// FUNCTION: STBC 0x00475df0 _$E409
// FUNCTION: STBC 0x00475e00 _$E408
// FUNCTION: STBC 0x00475e10 _$E413
// FUNCTION: STBC 0x00475e20 _$E412
// FUNCTION: STBC 0x00475e30 _$E417
// FUNCTION: STBC 0x00475e40 _$E416
// FUNCTION: STBC 0x00475e50 _$E421
// FUNCTION: STBC 0x00475e60 _$E420
// FUNCTION: STBC 0x00475e70 _$E206
// FUNCTION: STBC 0x00475e80 _$E205
// FUNCTION: STBC 0x00475e90 _$E425
// FUNCTION: STBC 0x00475ea0 _$E424
// FUNCTION: STBC 0x00475eb0 _$E211
// FUNCTION: STBC 0x00475ec0 _$E210
// FUNCTION: STBC 0x00475ed0 _$E216
// FUNCTION: STBC 0x00475ee0 _$E215
// FUNCTION: STBC 0x00475ef0 _$E429
// FUNCTION: STBC 0x00475f00 _$E428
// FUNCTION: STBC 0x00475f10 _$E434
// FUNCTION: STBC 0x00475f20 _$E433
// FUNCTION: STBC 0x00475f30 _$E438
// FUNCTION: STBC 0x00475f40 _$E437

// DATA: STBC 0x009819b8 ?rootsByText@AILog@@1V?$NiTStringMap@PAUAILogScope@@@@A
NiTStringMap<AILogScope *> AILog::rootsByText;

// DATA: STBC 0x00981adc ?file@AILog@@1PAU_iobuf@@A
FILE *AILog::file;

// DATA: STBC 0x00981ae0 ?reportToPython@AILog@@1_NA
bool AILog::reportToPython;

// DATA: STBC 0x00981ae4 ?current@AILog@@1PAUAILogScope@@A
AILogScope *AILog::current;

// DATA: STBC 0x00981ae8 ?keepTimings@AILog@@1_NA
bool AILog::keepTimings;

// The Python module the log reports to, and its function (invented names).
// DATA: STBC 0x008db178
static char *loggingModule = "AILogging";
// DATA: STBC 0x008db17c
static char *loggingFunction = "Log";

/*
================
AILog::AILog

Opens a record under the current one, with the formatted text, while the
log is open.
================
*/
// FUNCTION: STBC 0x00474540 ??0AILog@@QAA@PADZZ
// PARKED: register wall: one addressing-mode operand order in the expanded NiTListBlock constructor (retail [ebx+eax-8], this build [eax+ebx-8] for items[size - 1].next); everything else is exact
AILog::AILog( char *format, ... ) {
	if ( file == NULL ) {
		return;
	}
	AILogScope *scope = new AILogScope;
	AILogScope *parent = current;
	scope->parent = parent;
	if ( parent != NULL ) {
		parent->children.AddTail( scope );
	}
	current = scope;

	int length = strlen( format ) + 128;
	scope->text = new char[length];
	va_list args;
	va_start( args, format );
	_vsnprintf( scope->text, length, format, args );
	scope->text[length - 1] = 0;
}

/*
================
AILog::~AILog

Closes the open record: it keeps the time it took, and the scope it was
opened under becomes the open one. A root record is handed to CloseRoot.
================
*/
// FUNCTION: STBC 0x00474700 ??1AILog@@QAE@XZ
AILog::~AILog( void ) {
	if ( file == NULL || current == NULL ) {
		return;
	}
	if ( keepTimings ) {
		AILogTiming *timing = new AILogTiming;
		timing->updateNumber = g_kSystemWrapper.GetUpdateNumber();
		timing->elapsed = g_kSystemWrapper.GetTimeSinceFrameStart() - current->start;
		current->timings.AddTail( timing );
	}
	AILogScope *parent = current->parent;
	if ( parent != NULL ) {
		current = parent;
	} else {
		AILogScope *root = current;
		current = NULL;
		CloseRoot( root );
	}
}

/*
================
AILog::Open
================
*/
// FUNCTION: STBC 0x004747e0 ?Open@AILog@@SA_NPAD_N@Z
bool AILog::Open( char *fileName, bool unknown ) {
	if ( file != NULL ) {
		NiTMapIterator pos = rootsByText.GetFirstPos();
		while ( pos != NULL ) {
			const char *key;
			AILogScope *scope;
			rootsByText.GetNext( pos, key, scope );
			scope->Write( 0 );
			delete scope;
		}
		g_kSystemWrapper.FileClose( file );
	}
	file = NULL;
	if ( fileName != NULL ) {
		if ( strcmp( "AITree.txt", fileName ) != 0 && g_kSystemWrapper.FileExists( fileName ) ) {
			return false;
		}
		file = g_kSystemWrapper.FileOpen( fileName, "w" );
		delete current;
		current = NULL;
		PyErr_Clear();
		PyObject *module = PyEmbed::GetModule( loggingModule, false );
		PyErr_Clear();
		reportToPython = module != NULL;
	}
	keepTimings = unknown;
	return file != NULL;
}

/*
================
AILogScope::~AILogScope

Deletes the text, the records opened under this one and the timings.
================
*/
// FUNCTION: STBC 0x00474970 ??1AILogScope@@QAE@XZ
AILogScope::~AILogScope( void ) {
	delete [] text;
	while ( !children.IsEmpty() ) {
		AILogScope *child = children.RemoveTail();
		delete child;
	}
	while ( !timings.IsEmpty() ) {
		AILogTiming *timing = timings.RemoveTail();
		delete timing;
	}
}

/*
================
AILog::CloseRoot

A finished root record replaces the one kept under its text, unless the two
trees have the same shape: then the timings of the new one are added to the
kept one and the new one is dropped. A replaced record is written out, and
the new one is written and reported to the AILogging module.
================
*/
// FUNCTION: STBC 0x00474ab0 ?CloseRoot@AILog@@QAEXPAUAILogScope@@@Z
void AILog::CloseRoot( AILogScope *root ) {
	AILogScope *kept;

	if ( rootsByText.GetAt( root->text, kept ) ) {
		if ( root->SameTree( kept ) ) {
			if ( keepTimings ) {
				kept->Merge( root );
			}
			delete root;
			return;
		}
		kept->Write( 0 );
		delete kept;
	}
	rootsByText.SetAt( root->text, root );

	PyObject *list = NULL;
	if ( reportToPython ) {
		list = PyList_New( 0 );
	}
	root->Report( 0, list );
	if ( list != NULL ) {
		PyEmbed::CallFunction( loggingModule, loggingFunction, "", NULL, "(O)", list );
		Py_DECREF( list );
	}
}

/*
================
AILogScope::SameTree

Whether the other record and this one have the same text and the same
records under them, in the same order.
================
*/
// FUNCTION: STBC 0x00474d60 ?SameTree@AILogScope@@QAE_NPAU1@@Z
inline bool AILogScope::SameTree( AILogScope *other ) {
	if ( strcmp( text, other->text ) != 0 ) {
		return false;
	}
	NiTListIterator pos = children.GetHeadPos();
	NiTListIterator otherPos = other->children.GetHeadPos();
	while ( pos != NULL && otherPos != NULL ) {
		AILogScope *child = children.GetNext( pos );
		AILogScope *otherChild = other->children.GetNext( otherPos );
		if ( !child->SameTree( otherChild ) ) {
			return false;
		}
	}
	if ( pos != NULL || otherPos != NULL ) {
		return false;
	}
	return true;
}

/*
================
AILogScope::Merge

Moves the timings of the newer record of the same shape onto this one, and
does the same for the records under them.
================
*/
// FUNCTION: STBC 0x00474e00 ?Merge@AILogScope@@QAEXPAU1@@Z
inline void AILogScope::Merge( AILogScope *newer ) {
	while ( !newer->timings.IsEmpty() ) {
		timings.AddTail( newer->timings.RemoveHead() );
	}
	NiTListIterator pos = children.GetHeadPos();
	NiTListIterator newerPos = newer->children.GetHeadPos();
	while ( pos != NULL && newerPos != NULL ) {
		AILogScope *child = children.GetNext( pos );
		AILogScope *newerChild = newer->children.GetNext( newerPos );
		child->Merge( newerChild );
	}
}

/*
================
AILogScope::Write

Writes the text, the timings and the records under this one, indented by
their depth. A root is followed by an empty line.
================
*/
// FUNCTION: STBC 0x00474ed0 ?Write@AILogScope@@QAEXH@Z
void AILogScope::Write( int depth ) {
	FILE *file = AILog::file;
	int i;

	if ( file == NULL || !AILog::keepTimings ) {
		return;
	}
	for ( i = 0; i < depth; i++ ) {
		fprintf( file, "  " );
	}
	fprintf( file, "%s\n", text );
	for ( i = 0; i < depth; i++ ) {
		fprintf( file, "  " );
	}
	NiTListIterator pos = timings.GetHeadPos();
	while ( pos != NULL ) {
		AILogTiming *timing = timings.GetNext( pos );
		fprintf( file, "%ld:%lf ", timing->updateNumber, timing->elapsed * 1000.0 );
	}
	fprintf( file, "\n" );
	pos = children.GetHeadPos();
	while ( pos != NULL ) {
		AILogScope *child = children.GetNext( pos );
		child->Write( depth + 1 );
	}
	if ( depth == 0 ) {
		fprintf( file, "\n\n" );
	}
}

/*
================
AILogScope::Report

Writes the text of this record and of those under it, one per line and
indented by their depth, and appends each, as (depth, text), to the list
when there is one.
================
*/
// FUNCTION: STBC 0x00474fd0 ?Report@AILogScope@@QAEXHPAU_object@@@Z
void AILogScope::Report( int depth, PyObject *list ) {
	FILE *file = AILog::file;
	PyObject *entry = NULL;

	if ( list != NULL ) {
		entry = PyTuple_New( 2 );
	}
	if ( file != NULL ) {
		for ( int i = 0; i < depth; i++ ) {
			fprintf( file, "  " );
		}
		if ( entry != NULL ) {
			PyTuple_SetItem( entry, 0, PyInt_FromLong( depth ) );
		}
		fprintf( file, "%s\n", text );
		if ( entry != NULL ) {
			PyTuple_SetItem( entry, 1, PyString_FromString( text ) );
		}
		if ( list != NULL ) {
			PyList_Append( list, entry );
		}
		NiTListIterator pos = children.GetHeadPos();
		while ( pos != NULL ) {
			AILogScope *child = children.GetNext( pos );
			child->Report( depth + 1, list );
		}
		if ( depth == 0 ) {
			fprintf( file, "\n\n" );
		}
	}
}
