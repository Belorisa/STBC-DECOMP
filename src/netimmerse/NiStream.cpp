// NiStream.cpp -- loading and saving NetImmerse object graphs

#include "NiRTLib.h"
#include <math.h>

#include "NiStream.h"
#include "NiObject.h"
#include "renderer/NiImage.h"
#include "NiFile.h"
#include "NiFilename.h"
#include "NiMemStream.h"
#include "NiSystem.h"
#include <stdio.h>
#include <string.h>

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00818de0 _$E37
// FUNCTION: STBC 0x00818f00 _$E44
// FUNCTION: STBC 0x00818f10 _$E57

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00818ee0 _$E75
// FUNCTION: STBC 0x00818d00 _$E17
// FUNCTION: STBC 0x00818d20 _$E15
// FUNCTION: STBC 0x00818d30 _$E23
// FUNCTION: STBC 0x00818d50 _$E21

// DATA: STBC 0x9A2B98 ?loaders@NiStream@@2PAV?$NiTStringMap@P6AXAAVNiStream@@AAPAVNiObject@@@Z@@A
NiTStringMap<NiStream::CreateFunction> *NiStream::loaders;
// DATA: STBC 0x9A2B9C ?searchPath@NiStream@@2PADA
char *NiStream::searchPath;

// the header's lines, each saved with a line end
static char *headerLines[] = {
	"NetImmerse File Format, Version 3.1",
	"Numerical Design Limited, Chapel Hill, NC 27514",
	"Copyright (c) 1996-2000",
	"All Rights Reserved"
};

// DATA: STBC 0x9794A8
unsigned int NiStream::linkIDHashSize = 32768;

// the markers between the objects
static char topLevelObjectMarker[] = "Top Level Object";
static char endOfFileMarker[] = "End Of File";

// the bytes a LoadStep reads by default (name invented). It and headerSize
// are the unit's own, not NiStream statics: a static data member added to
// NiStream changes VC6's code for NiTriStrips::GetTriangleIndices.
static unsigned int defaultChunkSize = 1024;
// The two hash constants are one declaration: retail initialises both in one
// function (0x816BB0), which VC6 emits per declaration.
// FUNCTION: STBC 0x00816bb0 _$E63
// DATA: STBC 0x9A2A80
double NiStream::linkIDHashScale = NiStream::linkIDHashSize,
// DATA: STBC 0x9A2978
	NiStream::linkIDHashMultiplier = ( sqrt( 5.0 ) - 1.0 ) * 0.5;

// DATA: STBC 0x9A2A8C
unsigned int NiStream::errorCode;
// DATA: STBC 0x9A2A94
char NiStream::errorMessage[256];

// the header's bytes, line ends included (name invented); the unit's static
// initializer (0x816BF0) sums the header lines into it
static unsigned int headerSize;

// The unit's start-up and shut-down: an object at file scope (its exit
// destructor has no guard) whose constructor measures the header and clears
// the error, and whose destructor frees the loader map and the search path.
// The class and the object are invented names (invented).
class NiStreamStatics {
public:
	NiStreamStatics() {
		headerSize = 0;
		for ( unsigned int i = 0; i < sizeof( headerLines ) / sizeof( headerLines[0] ); i++ ) {
			headerSize += strlen( headerLines[i] ) + 1;
		}
		NiStream::errorCode = 0;
		NiStream::errorMessage[0] = 0;
	}
	~NiStreamStatics() {
		delete NiStream::loaders;
		delete [] NiStream::searchPath;
	}
};

// FUNCTION: STBC 0x00816bf0 _$E68
// FUNCTION: STBC 0x00816c40 _$E66
static NiStreamStatics streamStatics;

// the oldest and the current format version
static const char oldestVersion[] = "3.0";
static const char currentVersion[] = "3.1";

// the format line up to the version: "NetImmerse File Format, Version "
#define VERSION_OFFSET		32

// the size of the files' read and write buffers
#define FILE_BUFFER_SIZE	10240

// the class name of the object loaded last
static char lastClassName[256];

/*
================
NiStream::NiStream

An empty stream: no streams attached, an empty link ID map, and its own
map at unknown70. Clears the last load error.
================
*/
// FUNCTION: STBC 0x816C60
NiStream::NiStream() : linkIDs( 1024, 1024 ) {
	istr = 0;
	ostr = 0;
	fileVersion[0] = 0;
	creatingObjects = false;
	linkingObjects = false;
	loadBufferSize = 0;
	loadBufferRead = 0;
	loadBufferLeft = 0;
	loadState = LOAD_DONE;
	loadBuffer = 0;
	linkIDTable = new NiLinkIDItem *[linkIDHashSize];
	memset( linkIDTable, 0, linkIDHashSize * sizeof( NiLinkIDItem * ) );
	linkIDCount = 0;
	unknown3C = false;
	fileName = 0;
	unknown6C = true;
	unknown70 = new NiTStringMap<NiPointer<NiImage> >;
	loadRecord = 0;
	errorCode = 0;
	errorMessage[0] = 0;
}

/*
================
NiStream::~NiStream

Deletes the attached streams, releases the top-level objects and empties
the link ID map.
================
*/
// FUNCTION: STBC 0x816DA0
NiStream::~NiStream() {
	delete istr;
	delete ostr;
	RemoveAllObjects();
	RemoveAllLinkIDs();
	delete [] linkIDTable;
	delete [] loadBuffer;
	delete [] fileName;
	if ( unknown6C ) {
		delete unknown70;
	}
}

/*
================
NiStream::InsertObject

Adds a top-level object once, keeping a reference to it; NULL is
skipped.
================
*/
// FUNCTION: STBC 0x816F10 ?InsertObject@NiStream@@QAEXPAVNiObject@@@Z
void NiStream::InsertObject( NiObject *object ) {
	if ( !object ) {
		return;
	}

	unsigned int i;
	for ( i = 0; i < topObjects.GetSize(); i++ ) {
		if ( object == topObjects.GetAt( i ) ) {
			break;
		}
	}
	if ( i == topObjects.GetSize() ) {
		object->IncRefCount();
		topObjects.Add( object );
	}
}

/*
================
NiStream::RemoveObject

Drops a top-level object and its reference (Gamebryo 1.2.2:
NiStream::RemoveObject).
================
*/
// FUNCTION: STBC 0x817000
void NiStream::RemoveObject( NiObject *object ) {
	for ( unsigned int i = 0; i < topObjects.GetSize(); i++ ) {
		if ( object == topObjects.GetAt( i ) ) {
			topObjects.RemoveAt( i );
			object->DecRefCount();
			return;
		}
	}
}

/*
================
NiStream::RemoveAllObjects
================
*/
// FUNCTION: STBC 0x817070
void NiStream::RemoveAllObjects() {
	for ( unsigned int i = 0; i < topObjects.GetSize(); i++ ) {
		NiObject *object = topObjects.GetAt( i );
		if ( object ) {
			object->DecRefCount();
		}
	}
	topObjects.RemoveAll();
	linkIDs.RemoveAll();
	linkIndex = 0;
}

/*
================
NiStream::Unknown8170F0

Whether the object is one of the top-level objects.
================
*/
// FUNCTION: STBC 0x8170F0
bool NiStream::Unknown8170F0( NiObject *object ) {
	for ( unsigned int i = 0; i < topObjects.GetSize(); i++ ) {
		if ( object == topObjects.GetAt( i ) ) {
			return true;
		}
	}
	return false;
}

/*
================
NiStream::LinkIDToHashIndex
================
*/
// FUNCTION: STBC 0x817120
unsigned int NiStream::LinkIDToHashIndex( unsigned int linkID ) {
	double fraction = fmod( linkID * linkIDHashMultiplier, 1.0 );
	return (unsigned int)floor( fraction * linkIDHashScale );
}

/*
================
NiStream::InsertObject

Enters an object under its link ID, with the current load record; false
when the link ID is already in the map.
================
*/
// FUNCTION: STBC 0x817170 ?InsertObject@NiStream@@QAE_NIPAVNiObject@@@Z
bool NiStream::InsertObject( unsigned int linkID, NiObject *object ) {
	unsigned int index = LinkIDToHashIndex( linkID );
	NiLinkIDItem *item;
	for ( item = linkIDTable[index]; item; item = item->next ) {
		if ( linkID == item->linkID ) {
			return false;
		}
	}

	item = new NiLinkIDItem;
	item->linkID = linkID;
	item->object = object;
	item->record = loadRecord;
	item->unknown0C = false;
	item->next = linkIDTable[index];
	linkIDTable[index] = item;
	linkIDCount++;
	return true;
}

/*
================
NiStream::SetObjectFromLinkID

Replaces the object entered under linkID, which must be in the map.
================
*/
// FUNCTION: STBC 0x817220
void NiStream::SetObjectFromLinkID( unsigned int linkID, NiObject *object ) {
	NiLinkIDItem *item = linkIDTable[LinkIDToHashIndex( linkID )];
	while ( linkID != item->linkID ) {
		item = item->next;
	}
	item->object = object;
}

/*
================
NiStream::RemoveAllLinkIDs

Empties the link ID map, deleting the items' load records.
================
*/
// FUNCTION: STBC 0x8172A0
void NiStream::RemoveAllLinkIDs() {
	for ( unsigned int i = 0; i < linkIDHashSize; i++ ) {
		while ( linkIDTable[i] ) {
			NiLinkIDItem *next = linkIDTable[i]->next;
			delete linkIDTable[i]->record;
			delete linkIDTable[i];
			linkIDTable[i] = next;
		}
	}
	linkIDCount = 0;
}

/*
================
NiStream::GetLinkedObjectAt

The object at index in the stream order, found through its link ID; makes
its load record current. The object must be in the map (the debug copy's
assert( pItem ), AnimOnly.exe 0x419DCB, NiStream.cpp).
================
*/
// FUNCTION: STBC 0x817320
NiObject *NiStream::GetLinkedObjectAt( unsigned int index ) {
	unsigned int linkID = (unsigned int)linkIDs.GetAt( index );
	unsigned int hashIndex = LinkIDToHashIndex( linkID );
	NiLinkIDItem *item = linkIDTable[hashIndex];
	while ( item && item->linkID != linkID ) {
		item = item->next;
	}
	assert( item );
	loadRecord = item->record;
	return item->object;
}

/*
================
NiStream::GetObjectFromLinkID

The loaded object saved under linkID; NULL when there is none.
================
*/
// FUNCTION: STBC 0x8173A0
NiObject *NiStream::GetObjectFromLinkID( unsigned int linkID ) {
	for ( NiLinkIDItem *item = linkIDTable[LinkIDToHashIndex( linkID )]; item; item = item->next ) {
		if ( linkID == item->linkID ) {
			return item->object;
		}
	}
	return 0;
}

/*
================
NiStream::LoadHeader

The format line must match up to the version, and the version must lie
between the oldest and the current one; a version of three characters
gets a "0" (3.1 reads as 3.10). The copyright lines are only measured.

Quirk: the "expecting" part of a version error starts where the format
lines first differ, not at the version.
================
*/
// FUNCTION: STBC 0x817420
bool NiStream::LoadHeader() {
	const unsigned int LINE_SIZE = 128;	// (name invented; fits the /Od frame order)
	char lineBuffer[LINE_SIZE];	// (name invented; fits the /Od frame order)
	unsigned int i;
	istr->GetLine( lineBuffer, LINE_SIZE );
	if ( strcmp( lineBuffer, headerLines[0] ) != 0 ) {
		unsigned int length = strlen( headerLines[0] );
		for ( i = 0; i < length; i++ ) {
			if ( lineBuffer[i] != headerLines[0][i] ) {
				break;
			}
		}
		if ( i < VERSION_OFFSET ) {
			errorCode = 2;
			strcpy( errorMessage, "is not a NIF file" );
			return false;
		}

		strcpy( fileVersion, &lineBuffer[VERSION_OFFSET] );
		if ( strlen( fileVersion ) == 3 ) {
			strcat( fileVersion, "0" );
		}
		if ( strcmp( oldestVersion, fileVersion ) > 0 ) {
			errorCode = 3;
			sprintf( errorMessage, "%s %s %s", fileVersion, "is older version, expecting", &headerLines[0][i] );
			return false;
		}
		if ( strcmp( fileVersion, currentVersion ) > 0 ) {
			errorCode = 4;
			sprintf( errorMessage, "%s %s %s", fileVersion, "is later version, expecting", &headerLines[0][i] );
			return false;
		}
	} else {
		strcpy( fileVersion, currentVersion );
	}

	for ( i = 1; i < 4; i++ ) {
		istr->GetLine( lineBuffer, LINE_SIZE );
		if ( strlen( lineBuffer ) != strlen( headerLines[i] ) ) {
			errorCode = 2;
			strcpy( errorMessage, "is not a NIF file" );
			return false;
		}
	}
	return true;
}

/*
================
NiStream::LoadStream

Creates each object through its class's creation function, which loads
it; then links every object and lets each finish. A class without a
creation function stops the load with hints on the debug output.
Statements and asserts as the debug copy has them (AnimOnly.exe 0x41A10E,
NiStream.cpp: assert( bFound ), assert( bSuccessful )).
================
*/
// FUNCTION: STBC 0x8176B0
bool NiStream::LoadStream() {
	if ( !LoadHeader() ) {
		return false;
	}

	RemoveAllObjects();

	NiObject *object;
	while ( true ) {
		char *name;
		Unknown818A60( name );
		if ( strcmp( name, endOfFileMarker ) == 0 ) {
			delete [] name;
			break;
		}

		bool topLevel = strcmp( name, topLevelObjectMarker ) == 0;
		if ( topLevel ) {
			delete [] name;
			Unknown818A60( name );
		}
		strcpy( lastClassName, name );

		CreateFunction create;
		bool found = loaders->GetAt( name, create );
		if ( !found ) {
			char message[256];
			sprintf( message, "\nNiStream: Unable to find loader for class %s.\n", name );
			NiOutputDebugString( message );
			NiOutputDebugString( "* Make sure you are linking in the library " );
			NiOutputDebugString( "that contains this class.\n" );
			NiOutputDebugString( "* Make sure one of your source files " );
			NiOutputDebugString( "includes the top level header file for " );
			NiOutputDebugString( "that library\n\n" );
			errorCode = 5;
			sprintf( errorMessage, "%s %s", "cannot find create function", name );
			delete [] name;
			return false;
		}
		assert( found );
		delete [] name;

		object = 0;
		create( *this, object );
		if ( topLevel ) {
			InsertObject( object );
		}
	}

	unsigned int i;
	for ( i = 0; i < linkIDs.GetSize(); i++ ) {
		object = GetLinkedObjectAt( i );
		object->LinkObject( *this );
	}
	for ( i = 0; i < linkIDs.GetSize(); i++ ) {
		object = GetLinkedObjectAt( i );
		object->VirtualSlot11();
	}
	if ( unknown3C ) {
		bool successful = Unknown818CB0();
		assert( successful );
	}
	RemoveAllLinkIDs();
	loadRecord = 0;
	return true;
}

/*
================
NiStream::Load
================
*/
// FUNCTION: STBC 0x817A40 ?Load@NiStream@@QAE_NPBD@Z
bool NiStream::Load( const char *file ) {
	if ( file ) {
		delete [] fileName;
		fileName = new char[strlen( file ) + 1];
		strcpy( fileName, file );
		NiFile::StandardizeFilePath( &fileName );
	}

	istr = new NiFile( fileName, NiFile::READ_ONLY, FILE_BUFFER_SIZE );
	if ( !istr ) {
		errorCode = 1;
		sprintf( errorMessage, "%s %s", file, "cannot load file" );
		return false;
	}
	if ( !*istr ) {
		delete istr;
		istr = 0;
		errorCode = 1;
		sprintf( errorMessage, "%s %s", file, "cannot load file" );
		return false;
	}
	bool result = LoadStream();
	delete istr;
	istr = 0;
	return result;
}

/*
================
NiStream::Load
================
*/
// FUNCTION: STBC 0x817B60 ?Load@NiStream@@QAE_NPADH@Z
bool NiStream::Load( char *buffer, int size ) {
	istr = new NiMemStream( buffer, size );
	if ( !istr ) {
		return false;
	}
	bool result = LoadStream();
	delete istr;
	istr = 0;
	return result;
}

/*
================
NiStream::SaveHeader
================
*/
// FUNCTION: STBC 0x817C00
void NiStream::SaveHeader() {
	assert( ostr );
	for ( unsigned int i = 0; i < 4; i++ ) {
		ostr->PutS( headerLines[i] );
		ostr->PutS( "\n" );
	}
}

/*
================
NiStream::SaveStream

The top-level objects register what they reach in the stream order; each
object is then saved after its class name.
================
*/
// FUNCTION: STBC 0x817C30
bool NiStream::SaveStream() {
	SaveHeader();
	linkIDs.RemoveAll();

	unsigned int i;
	NiObject *object;
	for ( i = 0; i < topObjects.GetSize(); i++ ) {
		object = topObjects.GetAt( i );
		if ( object ) {
			object->RegisterStreamables( *this );
		}
	}

	for ( i = 0; i < linkIDs.GetSize(); i++ ) {
		object = linkIDs.GetAt( i );
		if ( Unknown8170F0( object ) ) {
			SaveCString( topLevelObjectMarker );
		}
		object->SaveBinary( *this );
	}
	SaveCString( endOfFileMarker );

	RemoveAllLinkIDs();
	return true;
}

/*
================
NiStream::Save
================
*/
// FUNCTION: STBC 0x817DD0 ?Save@NiStream@@QAE_NPBD@Z
bool NiStream::Save( const char *file ) {
	if ( file ) {
		delete [] fileName;
		fileName = new char[strlen( file ) + 1];
		strcpy( fileName, file );
		NiFile::StandardizeFilePath( &fileName );
	}

	ostr = new NiFile( fileName, NiFile::WRITE_ONLY, FILE_BUFFER_SIZE );
	if ( !ostr ) {
		return false;
	}
	if ( !*ostr ) {
		delete ostr;
		ostr = 0;
		return false;
	}
	bool result = SaveStream();
	delete ostr;
	ostr = 0;
	return result;
}

/*
================
NiStream::Save

The caller frees the block.
================
*/
// FUNCTION: STBC 0x817ED0 ?Save@NiStream@@QAE_NAAPADAAH@Z
bool NiStream::Save( char *&buffer, int &size ) {
	NiMemStream *memStream = new NiMemStream;
	ostr = memStream;
	if ( !ostr ) {
		return false;
	}
	bool bSuccessful = SaveStream();	// the name of LoadStream's asserted result (NiStream.cpp); fits the /Od frame order
	size = memStream->GetSize();
	buffer = (char *)memStream->Str();
	delete ostr;
	ostr = 0;
	return bSuccessful;
}

/*
================
NiStream::BeginLoad

Opens the file, reads its header and sets up the memory copy of the rest
for LoadStep to fill.
================
*/
// FUNCTION: STBC 0x817F80
bool NiStream::BeginLoad( const char *file ) {
	if ( !file ) {
		return false;
	}
	delete [] fileName;
	fileName = new char[strlen( file ) + 1];
	strcpy( fileName, file );
	NiFile::StandardizeFilePath( &fileName );

	unsigned int fileSize = NiFile::GetFileSize( fileName );
	if ( !fileSize ) {
		return false;
	}

	istr = new NiFile( fileName, NiFile::READ_ONLY, FILE_BUFFER_SIZE );
	if ( !istr ) {
		return false;
	}
	if ( !*istr ) {
		delete istr;
		istr = 0;
		return false;
	}
	if ( !LoadHeader() ) {
		delete istr;
		istr = 0;
		return false;
	}

	RemoveAllObjects();
	loadRecord = 0;

	loadBufferSize = fileSize - headerSize;
	if ( fileVersion[3] == '0' ) {
		loadBufferSize++;
	}
	loadBuffer = new char[loadBufferSize];
	loadBufferLeft = loadBufferSize;
	loadBufferRead = 0;
	creatingObjects = false;
	linkingObjects = false;
	return true;
}

/*
================
NiStream::LoadNextObject

As LoadStream's loop, for one object.

Quirk: a class without a creation function is not caught.

The class names are read through Unknown818A60, as the SDK's debug
NetImmerse 3.1 reads them here and in LoadStream (AnimOnly.exe 0x41AD44,
0x41A10E).
================
*/
// FUNCTION: STBC 0x818150
bool NiStream::LoadNextObject() {
	if ( creatingObjects ) {
		char *pName;	// (name invented; fits the /Od frame order)
		Unknown818A60( pName );
		if ( strcmp( pName, endOfFileMarker ) == 0 ) {
			delete [] pName;
			creatingObjects = false;
			linkingObjects = true;
			linkIndex = 0;
			return creatingObjects;
		}

		bool bTopLevel = strcmp( pName, topLevelObjectMarker ) == 0;	// (name invented; fits the /Od frame order)
		if ( bTopLevel ) {
			delete [] pName;
			Unknown818A60( pName );
		}
		strcpy( lastClassName, pName );

		CreateFunction create;
		bool bFound = loaders->GetAt( pName, create );
		assert( bFound );
		delete [] pName;

		NiObject *object = 0;
		create( *this, object );
		if ( bTopLevel ) {
			InsertObject( object );
		}
	}
	return creatingObjects;
}

/*
================
NiStream::LinkNextObject

After the last object, lets every object finish, empties the link ID map
and closes the memory copy.
================
*/
// FUNCTION: STBC 0x8183D0
bool NiStream::LinkNextObject() {
	if ( linkingObjects ) {
		NiObject *object = GetLinkedObjectAt( linkIndex );
		object->LinkObject( *this );
		linkIndex++;
		if ( linkIndex >= linkIDs.GetSize() ) {
			for ( unsigned int i = 0; i < linkIDs.GetSize(); i++ ) {
				object = GetLinkedObjectAt( i );
				object->VirtualSlot11();
			}
			linkingObjects = false;
			RemoveAllLinkIDs();
			delete istr;
			istr = 0;
		}
	}
	return linkingObjects;
}

/*
================
NiStream::LoadStep

Reading the file ends by putting the memory copy behind the input stream.
================
*/
// FUNCTION: STBC 0x818560
NiStream::LoadState NiStream::LoadStep( unsigned int bytes ) {
	if ( loadBufferLeft > 0 ) {
		if ( !bytes ) {
			bytes = defaultChunkSize;
		}
		if ( bytes > loadBufferLeft ) {
			bytes = loadBufferLeft;
		}
		unsigned int bytesRead = istr->Read( loadBuffer + loadBufferRead, bytes );
		assert( bytesRead == bytes );
		loadBufferRead += bytes;
		loadBufferLeft -= bytes;
		if ( !loadBufferLeft ) {
			// the file is closed as everywhere else, then the copy opened
			delete istr;
			istr = 0;
			istr = new NiMemStream( loadBuffer, loadBufferSize );
			assert( istr );
			creatingObjects = true;
		}
		loadState = LOAD_READING;
		return LOAD_READING;
	} else if ( creatingObjects ) {
		LoadNextObject();
		loadState = LOAD_CREATING;
		return LOAD_CREATING;
	} else if ( linkingObjects ) {
		LinkNextObject();
		if ( !linkingObjects ) {
			delete [] loadBuffer;
			loadBuffer = 0;
			loadBufferSize = 0;
		}
		loadState = LOAD_LINKING;
		return LOAD_LINKING;
	} else {
		loadState = LOAD_DONE;
		return LOAD_DONE;
	}
}

/*
================
NiStream::Unknown818680

Looks for the file at path: as given, then in the current directory (the
path without its drive and directory), then in the directory of the file
the stream works on (fileName), then in each directory of the search
path. On success path is replaced by a new copy of the path found.
================
*/
// FUNCTION: STBC 0x818680
bool NiStream::Unknown818680( char *&path ) {
	bool found = false;
	char *streamDir = new char[NiFilename::GetMaxPathLength()];
	char *fullPath = new char[NiFilename::GetMaxPathLength()];

	streamDir[0] = 0;
	if ( fileName ) {
		NiFilename streamFilename( fileName );
		streamFilename.SetFilename( "" );
		streamFilename.SetExt( "" );
		streamFilename.GetFullPath( streamDir );
	}
	NiFile::StandardizeFilePath( &streamDir );

	NiFilename filename( path );
	filename.GetFullPath( fullPath );
	NiFile::StandardizeFilePath( &fullPath );
	if ( NiFile::Access( fullPath, 0 ) == 0 ) {
		found = true;
	} else {
		filename.SetDrive( "" );
		filename.SetDir( "" );
		filename.GetFullPath( fullPath );
		if ( NiFile::Access( fullPath, 0 ) == 0 ) {
			found = true;
		} else {
			filename.SetDir( streamDir );
			filename.GetFullPath( fullPath );
			if ( NiFile::Access( fullPath, 0 ) == 0 ) {
				found = true;
			} else if ( searchPath ) {
				// the entries are cut out in place, and the separators put back
				char *dir = searchPath;
				bool last = false;
				do {
					char *separator = strchr( dir, ';' );
					if ( separator ) {
						*separator = 0;
					} else {
						last = true;
					}
					filename.SetDir( dir );
					filename.GetFullPath( fullPath );
					if ( NiFile::Access( fullPath, 0 ) == 0 ) {
						found = true;
						last = true;
					}
					if ( separator ) {
						*separator = ';';
						dir = separator + 1;
					}
				} while ( !last );
			}
		}
	}

	if ( found ) {
		delete [] path;
		path = new char[strlen( fullPath ) + 1];
		strcpy( path, fullPath );
	}
	delete [] fullPath;
	delete [] streamDir;
	return found;
}

/*
================
NiStream::SetSearchPath

An empty or NULL list clears the search path.
================
*/
// FUNCTION: STBC 0x818990
void NiStream::SetSearchPath( const char *searchPath_ ) {
	delete [] searchPath;
	searchPath = 0;
	if ( searchPath_ && strlen( searchPath_ ) > 0 ) {
		searchPath = new char[strlen( searchPath_ ) + 1];
		strcpy( searchPath, searchPath_ );
	}
}

/*
================
NiStream::LoadCString

Reads a string saved by SaveCString into a new block; NULL for an empty
or missing string.
================
*/
// FUNCTION: STBC 0x818A00
void NiStream::LoadCString( char *&string ) {
	assert( istr && string == 0 );
	int length;
	NiStreamLoadBinary( *this, length );
	if ( length > 0 ) {
		string = new char[length + 1];
		assert( string );
		istr->Read( string, length );
		string[length] = 0;
	} else {
		string = 0;
	}
}

/*
================
NiStream::Unknown818A60

Reads a string saved by SaveCString into a new block, which an empty string
gets too.
================
*/
// FUNCTION: STBC 0x818A60
void NiStream::Unknown818A60( char *&string ) {
	assert( istr );
	int length;
	NiStreamLoadBinary( *this, length );
	assert( length > 0 );
	string = new char[length + 1];
	assert( string );
	istr->Read( string, length );
	string[length] = 0;
	assert( ( string[0] == 'N' && string[1] == 'i' ) || ( strcmp( string, topLevelObjectMarker ) == 0 )
			|| ( strcmp( string, endOfFileMarker ) == 0 ) );
}

/*
================
NiStream::SaveCString

The length, then the characters without the terminator; NULL saves as the
empty string.
================
*/
// FUNCTION: STBC 0x818AB0
void NiStream::SaveCString( const char *string ) {
	assert( ostr );
	int length = string ? strlen( string ) : 0;
	NiStreamSaveBinary( *this, length );
	if ( length > 0 ) {
		ostr->Write( string, length );
	}
}

/*
================
NiStream::SetImageMap

Gives the stream the map of shared images to use (the stream no longer owns
its map), or with NULL goes back to a map of its own, created anew.
================
*/
// FUNCTION: STBC 0x818B00
void NiStream::SetImageMap( NiTStringMap<NiPointer<NiImage> > *map ) {
	if ( unknown70 != map ) {
		if ( map != NULL ) {
			if ( unknown6C ) {
				delete unknown70;
			}
			unknown6C = false;
		} else {
			if ( unknown6C ) {
				return;
			}
			unknown6C = true;
			map = new NiTStringMap<NiPointer<NiImage> >;
		}
		unknown70 = map;
	}
}

/*
================
NiStream::SetImage

The map keeps a reference to the image.
================
*/
// FUNCTION: STBC 0x818BE0
void NiStream::SetImage( const char *name, NiImage *image ) {
	unknown70->SetAt( name, image );
}

/*
================
NiStream::GetImage

NULL when no image is kept under the name.
================
*/
// FUNCTION: STBC 0x818C10
NiImage *NiStream::GetImage( const char *name ) {
	NiPointer<NiImage> image;
	if ( unknown70->GetAt( name, image ) ) {
		return image;
	}
	return 0;
}

/*
================
NiStream::Unknown818CB0

False at the first object that differs from its original.
================
*/
// FUNCTION: STBC 0x818CB0
bool NiStream::Unknown818CB0() {
	for ( unsigned int i = 0; i < linkIDHashSize; i++ ) {
		for ( NiLinkIDItem *item = linkIDTable[i]; item; item = item->next ) {
			if ( !( (NiObject *)item->linkID )->IsEqual( item->object ) ) {
				return false;
			}
		}
	}
	return true;
}
