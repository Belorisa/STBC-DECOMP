/*
===============================================================================

	SortedArray.h

	A growable array of pointers kept in ascending order of a key. The
	elements compare themselves: T::CompareKey( key ) is zero when the
	element has the key, positive when the key sorts after the element and
	negative when it sorts before; T::Compare( other ) compares the element
	with another element's key the same way. Find is a binary search.

	The array owns its elements: the destructor deletes the ones still in
	it. An owner that does not own them empties it first (RemoveAll).

	Evidence: the same binary search over (items 0x4, count 0x8) is
	expanded in the set's object table (0x40AF40-0x40B560) at both of its
	levels, over the type lists, which compare through virtual calls, and
	over the objects of a list, which compare through the non-virtual
	BaseObjectClass::CompareKey (0x431030); and in SetClass's own ID-sorted
	object array at 0x30 (GetNextObject 0x40FE80). The insertions grow the
	array to 2 and then by doubling, and shift the tail up with memmove. The
	names are invented.

	SortedArray<TGNetPlayer, int> has every member out of line, unreferenced
	ones included, among the first object's static initialisers
	(0x401630-0x401DB2): the copies an explicit instantiation emits, kept from
	ChangeRenderedSetAction's object as the link's first. The instantiation
	is written at the end of ChangeRenderedSetAction.cpp; which file of the
	original held it is not known. Retail's run also holds two more overloads of each
	of Get, Delete, Remove and Find (0x401880, 0x4018A0, 0x401950, 0x4019B0,
	0x401B30, 0x401B80, 0x401D10, 0x401D30) whose comparison folded to 0, one
	taking 8 bytes of arguments and one 4: an overload set this header does
	not have (open). Neither reads its arguments or the element: the search's
	first probe always hits (Find returns ( count - 1 ) >> 1), so the element
	comparison each calls is a constant 0 for TGNetPlayer, whose only
	comparisons are Compare and CompareKey; the demo build's copies are the
	same. Retail also keeps the `vector constructor iterator` (??_H, 0x401DC0)
	from this object, which a local array or new[] of a class with a
	constructor and no destructor emits (NiOBBox.h and TextureAnimManager.h's
	inline constructors do) but nothing in ChangeRenderedSetAction.cpp now
	has. In retail's order the members run constructor,
	destructor, Insert, DeleteAt, RemoveAt, Get, Delete, Remove, DeleteAll,
	RemoveAll, Find.

===============================================================================
*/

#ifndef __SORTEDARRAY_H__
#define __SORTEDARRAY_H__

#include <string.h>

template <class T, class KEY>
class SortedArray {
public:
					// FUNCTION: STBC 0x00401630 ??0?$SortedArray@VTGNetPlayer@@H@@QAE@XZ @game/ChangeRenderedSetAction
					SortedArray( void ) : items( NULL ), count( 0 ), capacity( 0 ) {}
					// FUNCTION: STBC 0x00401640 ??1?$SortedArray@VTGNetPlayer@@H@@QAE@XZ @game/ChangeRenderedSetAction
					// FUNCTION: STBC 0x006c7220 ??1?$SortedArray@VTGAnimTrack@@PBD@@QAE@XZ @game/TGAnimBlender
					// FUNCTION: STBC 0x006cc630 ??1?$SortedArray@VTGNodeAnimation@@PBD@@QAE@XZ @game/TGAnimationEntry
					// FUNCTION: STBC 0x006b79b0 ??1?$SortedArray@VTGNetGroup@@PAD@@QAE@XZ @game/TGNetwork
					// FUNCTION: STBC 0x0044b980 ??1?$SortedArray@VFloatVariableScope@@PAD@@QAE@XZ @game/VarManagerClass
					// FUNCTION: STBC 0x004170d0 ??1?$SortedArray@VObjectClass@@J@@QAE@XZ @game/SetClass
					// FUNCTION: STBC 0x004171d0 ??1?$SortedArray@VCameraObjectClass@@J@@QAE@XZ @game/SetClass
					// FUNCTION: STBC 0x00417220 ??1?$SortedArray@VLightObjectClass@@J@@QAE@XZ @game/SetClass
// FUNCTION: STBC 0x00418db0 ??1?$SortedArray@VSetClass@@PAD@@QAE@XZ @game/SetManager
// FUNCTION: STBC 0x006b10c0 ??1?$SortedArray@VMovieFrameAction@@H@@QAE@XZ @game/TGMovieAction
// FUNCTION: STBC 0x006c8cd0 ??1?$SortedArray@VTGAnimationEntry@@PAD@@QAE@XZ @game/TGAnimationManagerClass
// FUNCTION: STBC 0x006c4470 ??1?$SortedArray@VTGModelContainer@@PAD@@QAE@XZ @game/TGModelManager
					~SortedArray( void ) { DeleteAll(); }

	int				GetCount( void ) { return count; }
	T **			GetItems( void ) { return items; }
	T *				GetAt( int index ) { return items[index]; }

	// The index of the element with the key; -1 when there is none.
	// FUNCTION: STBC 0x00401cc0 ?Find@?$SortedArray@VTGNetPlayer@@H@@QAEHH@Z @game/TGNetwork
	// FUNCTION: STBC 0x006cc730 ?Find@?$SortedArray@VTGNodeAnimation@@PBD@@QAEHPBD@Z @game/TGAnimationEntry
	// FUNCTION: STBC 0x004055a0 ?Find@?$SortedArray@VSetClass@@PAD@@QAEHPAD@Z @game/ExplodeParticleController
	// FUNCTION: STBC 0x006c45a0 ?Find@?$SortedArray@VTGModelContainer@@PAD@@QAEHPAD@Z @game/TGModelManager
	// FUNCTION: STBC 0x006a2f10 ?Find@?$SortedArray@UTGGroupPlayer@@H@@QAEHH@Z @game/MultiplayerGame
	// FUNCTION: STBC 0x006a2fc0 ?Find@?$SortedArray@VTGNetGroup@@PAD@@QAEHPAD@Z @game/MultiplayerGame
	// FUNCTION: STBC 0x00417170 ?Find@?$SortedArray@VObjectClass@@J@@QAEHJ@Z @game/SetClass
	// FUNCTION: STBC 0x00417a30 ?Find@?$SortedArray@VCameraObjectClass@@J@@QAEHJ@Z @game/SetClass
	int				Find( KEY key ) {
						if ( items != NULL ) {
							int high = count - 1;
							int low = 0;
							while ( high >= low ) {
								int middle = ( low + high ) >> 1;
								int result = items[middle]->CompareKey( key );
								if ( result == 0 ) {
									return middle;
								}
								if ( result > 0 ) {
									low = middle + 1;
								} else {
									high = middle - 1;
								}
							}
						}
						return -1;
					}

	// The index of the element that compares equal to the item; -1 when
	// there is none.
	// FUNCTION: STBC 0x00401d70 ?Find@?$SortedArray@VTGNetPlayer@@H@@QAEHPAVTGNetPlayer@@@Z @game/ChangeRenderedSetAction
	int				Find( T *item ) {
						if ( items != NULL ) {
							int high = count - 1;
							int low = 0;
							while ( high >= low ) {
								int middle = ( low + high ) >> 1;
								int result = items[middle]->Compare( item );
								if ( result == 0 ) {
									return middle;
								}
								if ( result > 0 ) {
									low = middle + 1;
								} else {
									high = middle - 1;
								}
							}
						}
						return -1;
					}

	// Takes the element out of the array; it is not deleted.
	// FUNCTION: STBC 0x00401bd0 ?Remove@?$SortedArray@VTGNetPlayer@@H@@QAEXPAVTGNetPlayer@@@Z @game/ChangeRenderedSetAction
	void			Remove( T *item ) {
						int index = Find( item );
						if ( index >= 0 ) {
							RemoveAt( index );
						}
					}

	// Takes the element with the key out of the array; it is not deleted.
	// FUNCTION: STBC 0x00401ab0 ?Remove@?$SortedArray@VTGNetPlayer@@H@@QAEXH@Z @game/ChangeRenderedSetAction
	void			Remove( KEY key ) {
						if ( items != NULL ) {
							int index = Find( key );
							if ( index >= 0 ) {
								RemoveAt( index );
							}
						}
					}

	// The element with the key; NULL when there is none.
	// The early return for an empty array is the original's shape: written
	// nested inside `if ( items != NULL )` the inline callers that test the
	// items once and branch out (PlacementEditor::SwitchSetsDialogHandler,
	// PlacementObject::GetObjectByID, ChangeRenderedSetAction::Play, the
	// SetManager_GetSet wrapper) compile with retail's blocks in another
	// order. The unbraced guard costs 72 tuples, inside the 70-73 for which
	// MultiplayerGame::DeletePlayerHandler's inline budget spends as retail's
	// does (its later DeleteAt stays out of line); the braced guard is 74 and
	// the `== NULL` test two more.
	// FUNCTION: STBC 0x00401830 ?Get@?$SortedArray@VTGNetPlayer@@H@@QAEPAVTGNetPlayer@@H@Z @game/TGNetwork
	// FUNCTION: STBC 0x00596210 ?Get@?$SortedArray@VSetClass@@PAD@@QAEPAVSetClass@@PAD@Z @game/DamageableObject
	T *				Get( KEY key ) {
						if ( !items ) return NULL;
						int index = Find( key );
						if ( index >= 0 ) {
							return items[index];
						}
						return NULL;
					}

	// Inserts the element before the first one that sorts after it; always
	// 1 (its out-of-line copies return it in EAX).
	// FUNCTION: STBC 0x00401690 ?Insert@?$SortedArray@VTGNetPlayer@@H@@QAEHPAVTGNetPlayer@@@Z @game/ChangeRenderedSetAction
	// FUNCTION: STBC 0x006a2d90 ?Insert@?$SortedArray@UTGGroupPlayer@@H@@QAEHPAUTGGroupPlayer@@@Z @game/MultiplayerGame
	int				Insert( T *item ) {
						if ( count == capacity ) {
							if ( capacity == 0 ) {
								capacity = 2;
							} else {
								capacity *= 2;
							}
							T **newItems = new T *[capacity];
							if ( items != NULL ) {
								memcpy( newItems, items, count * sizeof( T * ) );
								delete [] items;
							}
							items = newItems;
						}
						int index;
						for ( index = 0; index < count; index++ ) {
							if ( items[index]->Compare( item ) < 0 ) {
								memmove( &items[index + 1], &items[index], ( count - index ) * sizeof( T * ) );
								break;
							}
						}
						items[index] = item;
						count++;
						return 1;
					}

	// Takes the element at the index out of the array; it is not deleted.
	// The index must not be negative.
	// FUNCTION: STBC 0x004017e0 ?RemoveAt@?$SortedArray@VTGNetPlayer@@H@@QAEXH@Z @game/ChangeRenderedSetAction
	// FUNCTION: STBC 0x00417120 ?RemoveAt@?$SortedArray@VObjectClass@@J@@QAEXH@Z @game/SetClass
	// FUNCTION: STBC 0x004a3b20 ?RemoveAt@?$SortedArray@VPlacementObject@@J@@QAEXH@Z @game/PlacementObject
	void			RemoveAt( int index ) {
						if ( index < count && items != NULL ) {
							memmove( &items[index], &items[index + 1], ( count - ( index + 1 ) ) * sizeof( T * ) );
							count--;
						}
					}

	// Deletes the element at the index and takes it out of the array. The
	// index must not be negative.
	// FUNCTION: STBC 0x00401780 ?DeleteAt@?$SortedArray@VTGNetPlayer@@H@@QAEXH@Z @game/ChangeRenderedSetAction
	// FUNCTION: STBC 0x006c7410 ?DeleteAt@?$SortedArray@VTGAnimTrack@@PBD@@QAEXH@Z @game/TGAnimBlender
	// FUNCTION: STBC 0x006a2e60 ?DeleteAt@?$SortedArray@UTGGroupPlayer@@H@@QAEXH@Z @game/TGNetwork
	// FUNCTION: STBC 0x006a2f60 ?DeleteAt@?$SortedArray@VTGNetGroup@@PAD@@QAEXH@Z @game/MultiplayerGame
	void			DeleteAt( int index ) {
						if ( index < count && items != NULL ) {
							delete items[index];
							memmove( &items[index], &items[index + 1], ( count - ( index + 1 ) ) * sizeof( T * ) );
							count--;
						}
					}

	// Deletes the element and takes it out of the array.
	// FUNCTION: STBC 0x00401a10 ?Delete@?$SortedArray@VTGNetPlayer@@H@@QAEXPAVTGNetPlayer@@@Z @game/ChangeRenderedSetAction
	void			Delete( T *item ) {
						int index = Find( item );
						if ( index >= 0 ) {
							DeleteAt( index );
						}
					}

	// Deletes the element with the key and takes it out of the array.
	// The array test before the search is the original's: retail tests the
	// items ahead of the out-of-line Find in MultiplayerGame's forwards
	// (0x69FDA0). Written `if ( items )`: its cost (67, two units under the
	// `!= NULL` form) is what spends MultiplayerGame::DeletePlayerHandler's
	// inline budget as retail does (member Find expanded, DeleteAt called).
	// FUNCTION: STBC 0x004018c0 ?Delete@?$SortedArray@VTGNetPlayer@@H@@QAEXH@Z @game/ChangeRenderedSetAction
	void			Delete( KEY key ) {
						if ( items ) {
							int index = Find( key );
							if ( index >= 0 ) {
								DeleteAt( index );
							}
						}
					}

	// Forgets the elements without deleting them; the array stays allocated.
	// FUNCTION: STBC 0x00401cb0 ?RemoveAll@?$SortedArray@VTGNetPlayer@@H@@QAEXXZ @game/ChangeRenderedSetAction
	void			RemoveAll( void ) { count = 0; }

	// Deletes the elements and frees the array.
	// FUNCTION: STBC 0x00401c60 ?DeleteAll@?$SortedArray@VTGNetPlayer@@H@@QAEXXZ @game/ChangeRenderedSetAction
	// FUNCTION: STBC 0x00417990 ?DeleteAll@?$SortedArray@VObjectClass@@J@@QAEXXZ @game/SetClass
	// FUNCTION: STBC 0x004179e0 ?DeleteAll@?$SortedArray@VCameraObjectClass@@J@@QAEXXZ @game/SetClass
	// FUNCTION: STBC 0x00417a90 ?DeleteAll@?$SortedArray@VLightObjectClass@@J@@QAEXXZ @game/SetClass
	// FUNCTION: STBC 0x006b7a20 ?DeleteAll@?$SortedArray@VTGNetGroup@@PAD@@QAEXXZ @game/TGNetwork
	void			DeleteAll( void ) {
						if ( items != NULL ) {
							while ( count > 0 ) {
								delete items[count - 1];
								count--;
							}
							count = 0;
							delete [] items;
							items = NULL;
							capacity = 0;
						}
					}

private:
	T **			items;
	int				count;
	int				capacity;
};

#endif /* !__SORTEDARRAY_H__ */
