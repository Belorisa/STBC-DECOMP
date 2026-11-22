/*
===============================================================================

	NiStream.h

	Loads and saves NetImmerse object graphs (NIF files). Objects read and
	write their members through the stream's input and output byte streams.

	Evidence: the constructor (0x816C60) and destructor (0x816DA0) give the
	layout; the version check (0x817420) names the format ("is not a NIF
	file", "3.0"). NiStream has no vtable. Members without name evidence keep
	their offsets as names.

===============================================================================
*/

#ifndef __NISTREAM_H__
#define __NISTREAM_H__

#include "NiBinaryStream.h"
#include "NiTArray.h"
#include "NiTStringMap.h"
#include "NiPointer.h"

class NiObject;
class NiImage;

/*
===============================================================================

	Load records: what an object's LoadBinary reads for LinkObject to
	resolve once every object exists, chiefly the link IDs of the objects it
	points to. Each class's creation function allocates the record of its
	own class and hands it to the stream before calling LoadBinary; the
	link ID map keeps it with the object. The class names are invented.

	Evidence: every record's destructor leaves the vtable 0x88B840 (one
	slot, the scalar deleting destructor 0x4554D0) behind; NiObject::LoadBinary
	(0x7D8820) stores the object's own link ID at 0x04 of whatever record is
	current, the extra-data records included. NiObject's record (vtable
	0x88C578, constructor 0x7A4D00) clears that link ID at construction; the
	extra-data records (NiExtraData.h) do not derive from it: their
	constructors leave it unset and their destructors clear it.

===============================================================================
*/

// FUNCTION: STBC 0x004554d0 ??_GNiLoadRecord@@UAEPAXI@Z @game/NiBinaryVoxelData
class NiLoadRecord {
public:
	virtual				~NiLoadRecord() {}

	unsigned int		linkID;		// the object's own link ID
};

// FUNCTION: STBC 0x004abe60 ??_GNiObjectLoadRecord@@UAEPAXI@Z @game/NiBinaryVoxelData
// FUNCTION: STBC 0x004abea0 ??1NiObjectLoadRecord@@UAE@XZ @game/NiBinaryVoxelData
class NiObjectLoadRecord : public NiLoadRecord {
public:
	// FUNCTION: STBC 0x7A4D00 ??0NiObjectLoadRecord@@QAE@XZ @netimmerse/NiKeyframeManager
						NiObjectLoadRecord() { linkID = 0; }
};

/*
================
NiLinkIDItem

An entry of the stream's link ID map: a loaded object under the link ID it
was saved with, and its load record. The class name is invented.
================
*/
class NiLinkIDItem {
public:
	unsigned int		linkID;
	NiObject *			object;
	NiLoadRecord *		record;
	bool				unknown0C;	// cleared by InsertObject
	NiLinkIDItem *		next;		// the next item in the same bucket
};

class NiStream {
public:
	// creates an object of a registered class and loads its members
	typedef void		( *CreateFunction )( NiStream &stream, NiObject *&object );

	static NiTStringMap<CreateFunction> *	loaders;	// class name to creation function, created on first use

						NiStream();
						~NiStream();

	// reads a NIF file's objects; false, with an error message, when the
	// file cannot be opened or read. A NULL file reads the file named last.
	// (Gamebryo lineage names)
	bool				Load( const char *file );
	// reads the objects from a block of memory
	bool				Load( char *buffer, int size );
	// writes the top-level objects and everything they reach to a file, or
	// to a new block of memory the caller then owns
	bool				Save( const char *file );
	bool				Save( char *&buffer, int &size );

	// Loading a step at a time (names invented): BeginLoad opens the file
	// and reads the header, then each LoadStep does one step and says which
	// it did: a chunk of the file read into memory (bytes, 0 for a default
	// of 1024), one object created and loaded, or one object linked;
	// LOAD_DONE once nothing is left.
	enum LoadState {
		LOAD_DONE,
		LOAD_READING,
		LOAD_CREATING,
		LOAD_LINKING
	};

	bool				BeginLoad( const char *file );
	LoadState			LoadStep( unsigned int bytes );
	// one object of the memory copy; false once the end marker is read
	bool				LoadNextObject();
	// links one loaded object; false once all are linked
	bool				LinkNextObject();

	// the top-level objects of the file read last (Gamebryo lineage names)
	unsigned int		GetObjectCount() const { return topObjects.GetSize(); }
	NiObject *			GetObjectAt( unsigned int index ) const { return topObjects.GetAt( index ); }

	// strings: a length, then the characters without the terminator
	void				LoadCString( char *&string );
	// TODO identity: LoadCString without the empty case, a new block even
	// for a zero length; nothing in retail calls it
	void				Unknown818A60( char *&string );
	void				SaveCString( const char *string );

	const char *		GetFileVersion() const { return fileVersion; }
	NiBinaryStream *	GetIstr() const { return istr; }
	NiLoadRecord *		GetLoadRecord() const { return loadRecord; }
	void				SetLoadRecord( NiLoadRecord *record_ ) { loadRecord = record_; }
	NiBinaryStream *	GetOstr() const { return ostr; }

	// an object's link ID is its address when it was saved
	void				SaveLinkID( NiObject *object ) { ostr->Write( &object, sizeof( object ) ); }

	// adds a top-level object, once (Gamebryo lineage name)
	void				InsertObject( NiObject *object );
	// drops one (Gamebryo lineage name); TODO identity: whether one is a
	// top-level object (0x8170F0)
	void				RemoveObject( NiObject *object );
	bool				Unknown8170F0( NiObject *object );

	// the link ID map: false when the link ID is already registered
	bool				InsertObject( unsigned int linkID, NiObject *object );
	NiObject *			GetObjectFromLinkID( unsigned int linkID );
	void				SetObjectFromLinkID( unsigned int linkID, NiObject *object );	// invented name
	NiObject *			GetLinkedObjectAt( unsigned int index );	// invented name
	// appends an object to the stream order (name invented; AnimOnly.exe
	// 0x409AD0, called by NiObject::LoadBinary and RegisterStreamables)
	void				AddLinkedObject( NiObject *object ) { linkIDs.Add( object ); }
	void				RemoveAllLinkIDs();	// invented name

	// TODO identity: finds the file at path, trying the directory at
	// fileName and then each directory of the search path; replaces path
	// with the one found and returns true, or false (0x818680)
	bool				Unknown818680( char *&path );

	// the ';'-separated directories Unknown818680 searches; the game reads
	// the list to skip setting it again (names invented)
	static char *		searchPath;
	static void			SetSearchPath( const char *searchPath_ );

	// the external images kept by file name in the map at unknown70, so that
	// NiImage::LoadBinary shares one image per file (names invented)
	void				SetImageMap( NiTStringMap<NiPointer<NiImage> > *map );
	void				SetImage( const char *name, NiImage *image );
	NiImage *			GetImage( const char *name );

	// The link ID map hashes by multiplication (Knuth): the fraction of
	// linkID times the golden ratio, scaled to the bucket count.
	unsigned int		LinkIDToHashIndex( unsigned int linkID );

	static unsigned int	linkIDHashSize;		// buckets of the link ID map
	static double		linkIDHashScale;	// linkIDHashSize as a double
	static double		linkIDHashMultiplier;	// ( sqrt( 5 ) - 1 ) / 2

	// TODO identity: compares each loaded object with the object at its link
	// ID, the address it was saved from (NiObject::IsEqual); LoadStream calls
	// it when unknown3C is set
	bool				Unknown818CB0();

	// the last load error: the version check (0x817420) sets a code (2: not
	// a NIF file, 3: older version, 4: later version) and its message. The
	// names are invented.
	static unsigned int	errorCode;
	static char			errorMessage[256];

protected:
	// the header: the format line ("NetImmerse File Format, Version 3.1")
	// and three copyright lines; false, with an error message, when the
	// format or version is wrong (Gamebryo lineage names)
	bool				LoadHeader();
	void				SaveHeader();
	// the objects in stream order, each after its class name, a top-level
	// object after a "Top Level Object" marker, and an "End Of File" marker
	bool				LoadStream();
	bool				SaveStream();
	// releases the top-level objects and forgets the stream order (Gamebryo
	// lineage name)
	void				RemoveAllObjects();

	char				fileVersion[5];	// the format version read from the file ("3.05"); empty until then
	bool				creatingObjects;	// LoadStep creates objects next (name invented)
	bool				linkingObjects;	// LoadStep links objects next (name invented)
	unsigned int		linkIndex;		// the next object LinkNextObject links (Gamebryo lineage name)
	unsigned int		loadBufferSize;	// the memory copy of the file past its header (names invented)
	unsigned int		loadBufferRead;	// bytes of it read so far
	unsigned int		loadBufferLeft;	// bytes of it still to read
	LoadState			loadState;		// what the last LoadStep did
	char *				loadBuffer;		// freed by the destructor
	char *				fileName;		// the file loaded or saved last; freed by the destructor (Gamebryo lineage name)
	// the objects being loaded or saved, in stream order, as their link IDs;
	// the same instantiation as topObjects: the constructor keeps the one
	// vtable address in a register for both arrays
	NiTArray<NiObject *>	linkIDs;
	bool				unknown3C;
	NiBinaryStream *	istr;			// input stream while loading
	NiBinaryStream *	ostr;			// output stream while saving
	NiTArray<NiObject *>	topObjects;	// the top-level objects (referenced; Gamebryo lineage name)
	NiLoadRecord *		loadRecord;	// the record of the object being loaded
	unsigned int		linkIDCount;	// items in the link ID map
	NiLinkIDItem **		linkIDTable;	// heap block of linkIDHashSize chain heads
	bool				unknown6C;
	NiTStringMap<NiPointer<NiImage> > *	unknown70;	// deleted by the destructor when unknown6C is set
};

/*
================
NiImplementRegister

Defines a streamable class's Register function, which adds the class's
creation function to the loader registry once, creating the registry on
first use. Every unit that includes the class's header calls it from a
static initializer.

The registry code is part of the macro, not an inline function: VC6 never
inlines a function whose new-expression needs an unwind state, yet every
Register in retail creates the registry in its own body.
================
*/
#define NiImplementRegister( classname ) \
	int classname::Register() { \
		static bool registered = false; \
		if ( registered ) { \
			return 0; \
		} \
		registered = true; \
		if ( !NiStream::loaders ) { \
			NiStream::loaders = new NiTStringMap<NiStream::CreateFunction>; \
		} \
		NiStream::loaders->SetAt( #classname, classname::CreateObject ); \
		return 1; \
	}


/*
================
NiStreamEndianSwap

Puts one loaded value in the platform's byte order. NIF files are
little-endian, so on this platform it does nothing. The function name is
invented.
================
*/
template <class T> inline void NiStreamEndianSwap( T &value ) {
}

/*
================
NiBinaryStreamLoad

Reads count values of plain data from a binary stream (Gamebryo 1.1.2 and
1.2.2: NiBinaryLoadSave.h, NiBinaryStreamLoad). The empty test of the count
is the lineage header's own (it stood beside an assert); it compiles to
nothing, but it makes VC6 form the value's address before it loads the
input stream, as every single-value LoadBinary read in retail does
(NiFloatController::LoadBinary, NiVisController::LoadBinary).
================
*/
// FUNCTION: STBC 0x803610 ?NiBinaryStreamLoad@@YAXAAVNiBinaryStream@@PAPAVNiImage@@I@Z @netimmerse/NiMultiTextureProperty
// FUNCTION: STBC 0x803630 ?NiBinaryStreamLoad@@YAXAAVNiBinaryStream@@PAFI@Z @netimmerse/NiMultiTextureProperty
// FUNCTION: STBC 0x803650 ?NiBinaryStreamLoad@@YAXAAVNiBinaryStream@@PAEI@Z @netimmerse/NiMultiTextureProperty
// FUNCTION: STBC 0x7B37A0 ?NiBinaryStreamLoad@@YAXAAVNiBinaryStream@@PAII@Z @netimmerse/NiParticleSystemController
// FUNCTION: STBC 0x7B37C0 ?NiBinaryStreamLoad@@YAXAAVNiBinaryStream@@PAMI@Z @netimmerse/NiParticleSystemController
// FUNCTION: STBC 0x7B37E0 ?NiBinaryStreamLoad@@YAXAAVNiBinaryStream@@PAHI@Z @netimmerse/NiParticleSystemController
template <class T> inline void NiBinaryStreamLoad( NiBinaryStream &binstream, T *values, unsigned int count ) {
	if ( count == 0 ) {
	}
	binstream.Read( values, sizeof( T ) * count );
}

/*
================
NiStreamLoadBinary

Reads an array of count values of plain data from the stream's input,
through NiBinaryStreamLoad, as the SDK's debug NetImmerse 3.1 compiles it
(AnimOnly.exe 0x40E730, called by NiRawImageData::LoadBinary at 0x40DDE1).
================
*/
template <class T> inline void NiStreamLoadBinary( NiStream &stream, T *values, unsigned int count ) {
	NiBinaryStreamLoad( *stream.GetIstr(), values, count );
}

/*
================
NiStreamLoadBinary

Reads one value of plain data from the stream's input (Gamebryo 1.1.2 and
1.2.2: NiStream.inl, NiStreamLoadBinary).
================
*/
template <class T> inline void NiStreamLoadBinary( NiStream &stream, T &value ) {
	NiBinaryStreamLoad( *stream.GetIstr(), &value, 1 );
}

/*
================
NiStreamLoadEnum

Reads an enumeration stored as an int (Gamebryo 1.1.2 and 1.2.2:
NiBinaryLoadSave.h, NiBinaryStreamLoadEnum; NiStream.inl,
NiStreamLoadEnum).
================
*/
template <class T> inline void NiBinaryStreamLoadEnum( NiBinaryStream &binstream, T *value ) {
	int dummy;
	binstream.Read( &dummy, sizeof( int ) );
	*value = (T)dummy;
}

template <class T> inline void NiStreamLoadEnum( NiStream &stream, T &value ) {
	NiBinaryStreamLoadEnum( *stream.GetIstr(), &value );
}

/*
================
NiStreamSaveBinary

Writes one value of plain data to the stream's output (Gamebryo 1.1.2 and
1.2.2: NiBinaryLoadSave.h, NiBinaryStreamSave; NiStream.inl,
NiStreamSaveBinary).
================
*/
template <class T> inline void NiBinaryStreamSave( NiBinaryStream &binstream, const T *values, unsigned int count ) {
	binstream.Write( values, sizeof( T ) * count );
}

template <class T> inline void NiStreamSaveBinary( NiStream &stream, const T &value ) {
	NiBinaryStreamSave( *stream.GetOstr(), &value, 1 );
}

/*
================
NiStreamSaveEnum

Writes an enumeration as an int (Gamebryo 1.1.2: NiBinaryLoadSave.h,
NiBinaryStreamSaveEnum; NiStream.inl, NiStreamSaveEnum), as the SDK's debug
NetImmerse 3.1 compiles it (AnimOnly.exe 0x40E770 and 0x40E8A0, called by
NiRawImageData::SaveBinary).
================
*/
template <class T> inline void NiBinaryStreamSaveEnum( NiBinaryStream &binstream, const T &value ) {
	binstream.Write( &value, sizeof( int ) );
}

template <class T> inline void NiStreamSaveEnum( NiStream &stream, const T &value ) {
	NiBinaryStreamSaveEnum( *stream.GetOstr(), value );
}

/*
================
NiStreamSaveBinary

Writes an array of count values of plain data to the stream's output
(Gamebryo 1.1.2: NiStream.inl), as the SDK's debug NetImmerse 3.1 compiles
it (AnimOnly.exe 0x433580, called by NiTriShapeData::SaveBinary).
================
*/
template <class T> inline void NiStreamSaveBinary( NiStream &stream, const T *values, unsigned int count ) {
	NiBinaryStreamSave( *stream.GetOstr(), values, count );
}

#endif /* !__NISTREAM_H__ */
