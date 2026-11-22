/*
===============================================================================

	KeyArray.h

	A growable array of animation keys held by value and kept in ascending
	order of time. The effect controllers keep their colour, alpha, size and
	intensity keys in these: Insert puts a key after the keys at or before
	its time, growing the array by growBy elements when it is full, and
	RemoveAt closes the gap.

	0x14 bytes. Evidence: EffectControllerData's constructor (expanded in
	EffectController's, 0x4C9CA0) builds three of them, clearing the
	elements, count and size and setting the growth to 1, under the
	vtables 0x88C95C (NiLinColorKey elements) and 0x88C960 (NiLinFloatKey
	elements), each with the one slot of the deleting destructor (0x4CA980,
	0x4CAA30). The growth (0x4CB260) allocates the elements with new [] and
	copies the old ones over with memcpy. The class and member names are
	invented.

===============================================================================
*/

#ifndef __KEYARRAY_H__
#define __KEYARRAY_H__

#include <string.h>

// FUNCTION: STBC 0x004ca980 ??_G?$KeyArray@VNiLinColorKey@@@@UAEPAXI@Z @game/EffectController
// FUNCTION: STBC 0x004caa30 ??_G?$KeyArray@VNiLinFloatKey@@@@UAEPAXI@Z @game/EffectController
// FUNCTION: STBC 0x004d3870 ??_G?$KeyArray@VNiLinPosKey@@@@UAEPAXI@Z @game/FlareController
template <class T>
class KeyArray {
public:
					// retail keeps an out-of-line NiLinFloatKey copy that takes the growth (0x4F35D0, FlareController's unit)
					KeyArray( unsigned int growBy_ = 1 ) : keys( NULL ), count( 0 ), size( 0 ), growBy( growBy_ ) {}
					// FUNCTION: STBC 0x004ca900 ??1?$KeyArray@VNiLinColorKey@@@@UAE@XZ @game/EffectController
					// FUNCTION: STBC 0x004ca930 ??1?$KeyArray@VNiLinFloatKey@@@@UAE@XZ @game/EffectController
	virtual			~KeyArray() { delete [] keys; }

	unsigned int	GetCount() const { return count; }
	void			RemoveAll() { count = 0; }

	// reallocates the elements for size_ keys, dropping the keys past it
	// retail keeps the NiLinColorKey copy out of line at 0x4CB260 (AddColorKey calls it); this build expands it
	// FUNCTION: STBC 0x004cb260 ?SetSize@?$KeyArray@VNiLinColorKey@@@@QAEXI@Z @game/EffectController
	void			SetSize( unsigned int size_ ) {
						if ( size_ == size ) {
							return;
						}
						if ( size_ < (unsigned int)count ) {
							count = size_;
						}
						T *oldKeys = keys;
						size = size_;
						keys = new T[size];
						memcpy( keys, oldKeys, count * sizeof( T ) );
						delete [] oldKeys;
					}

	// adds the key after the keys at or before its time
	void			Insert( const T &key ) {
						if ( count + 1 > size ) {
							SetSize( count + growBy );
						}
						unsigned int i;
						for ( i = 0; i < (unsigned int)count; i++ ) {
							if ( Compare( keys[i], key ) > 0 ) {
								memmove( &keys[i + 1], &keys[i], ( count - i ) * sizeof( T ) );
								break;
							}
						}
						keys[i] = key;
						count++;
					}

	// the order of two keys by time: negative, zero or positive
	static int		Compare( const T &key0, const T &key1 ) {
						if ( key1.GetTime() > key0.GetTime() ) {
							return -1;
						}
						if ( key1.GetTime() < key0.GetTime() ) {
							return 1;
						}
						return 0;
					}

	void			RemoveAt( int index ) {
						if ( (unsigned int)( index + 1 ) < (unsigned int)count ) {
							// BUG: moves one key more than follows the removed one, reading past the last
							memmove( &keys[index], &keys[index + 1], ( count - index ) * sizeof( T ) );
						}
						count--;
					}

	T *				keys;			// 0x04 count keys, in ascending order of time
	int				count;			// 0x08
	unsigned int	size;			// 0x0C keys allocated
	unsigned int	growBy;			// 0x10 keys added when the array is full
};

#endif /* !__KEYARRAY_H__ */
