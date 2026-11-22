/*
===============================================================================

	NiVisKey.h

	A visibility key: from its time on, the animated object is shown or
	hidden. NiVisData keeps its keys as an array of pointers.

	Evidence: NiVisData::LoadBinary (0x7B5E80) allocates 8 bytes per key
	and reads it through 0x7B7050 (a float, then one byte); the key search
	(0x7B6F70) compares the float at 0 against the time and returns the
	byte at 4. NiVisKey has no vtable. The class and member names follow
	the Gamebryo lineage.

===============================================================================
*/

#ifndef __NIVISKEY_H__
#define __NIVISKEY_H__

class NiStream;

class NiVisKey {
public:
							NiVisKey() {}
							NiVisKey( float time_, bool visible_ ) { time = time_; visible = visible_; }

	bool					operator==( const NiVisKey &key ) const { return time == key.time && visible == key.visible; }
	bool					operator!=( const NiVisKey &key ) const { return !( *this == key ); }

	float					GetTime() const { return time; }

	NiVisKey *				Copy() const;		// name invented

	void					LoadBinary( NiStream &stream );
	void					SaveBinary( NiStream &stream );

	// the visibility at time: the last key at or before it
	static bool				GenInterp( float time, NiVisKey **keys, unsigned int numKeys, unsigned int &lastIndex );

	float					time;
	bool					visible;
};

#endif /* !__NIVISKEY_H__ */
