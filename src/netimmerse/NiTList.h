/*
===============================================================================

	NiTList.h

	NetImmerse's doubly linked list of pointers. Each list draws its items
	from blocks it allocates itself and recycles removed items through a
	free list; the destructor frees the blocks without visiting the items.

	Names: NiTList and NiTListIterator are the binary's own (the SWIG type
	strings "NiTList<ShipSubsystem *>" and "NiTListIterator"). NiTListItem
	and the member names follow the Gamebryo lineage (NiTPointerList and its
	block allocator), whose list behaves the same way; NiTListBlock is
	invented.

	Evidence: a list is 0x18 bytes without a vtable: the count, the head,
	the tail, the free list, the blocks and the items per block (a static
	list at 0x9A11E8 is built with 2). An item is the element, the next and
	the previous item. NewItem (0x486BE0) takes an item from the free list,
	first allocating a block when it is empty: an 8-byte header holding the
	next block and an array of items chained through their next links.
	DeleteItem (0x486CA0) clears the element and pushes the item on the free
	list. The destructor (0x7C4C40) deletes the blocks and clears the list.
	ShipClass::AddSubsystem (0x5B3E50) and RepairSubsystem (0x565520,
	0x565B50) inline AddTail, GetHeadPos, GetNext, FindPos and RemovePos;
	0x565B50 also inlines AddHead. NewItem stays out of line (it holds an EH
	state); its copies are per element type: 0x486BE0 (ShipSubsystem *),
	0x4292B0 (the TGObject lists of the save and load code) and 0x6F1B60
	(the load's fix-up records).

===============================================================================
*/

#ifndef __NITLIST_H__
#define __NITLIST_H__

#include <assert.h>

// a position in a list: the item to return next, or NULL at the end
typedef void *			NiTListIterator;

// FUNCTION: STBC 0x78EE00 ??0?$NiTListItem@V?$NiPointer@VNiSource@@@@@@QAE@XZ @netimmerse/NiSoundSystem
// FUNCTION: STBC 0x78EE10 ??0?$NiTListItem@V?$NiPointer@VNiProvider_Info@@@@@@QAE@XZ @netimmerse/NiSoundSystem
// FUNCTION: STBC 0x78EC20 ??1?$NiTListItem@V?$NiPointer@VNiSource@@@@@@QAE@XZ @netimmerse/NiSoundSystem
// FUNCTION: STBC 0x78EC40 ??1?$NiTListItem@V?$NiPointer@VNiProvider_Info@@@@@@QAE@XZ @netimmerse/NiSoundSystem
// FUNCTION: STBC 0x78EB60 ??_E?$NiTListItem@V?$NiPointer@VNiSource@@@@@@QAEPAXI@Z @netimmerse/NiSoundSystem
// FUNCTION: STBC 0x78EBC0 ??_E?$NiTListItem@V?$NiPointer@VNiProvider_Info@@@@@@QAEPAXI@Z @netimmerse/NiSoundSystem
// FUNCTION: STBC 0x665BB0 ??0?$NiTListItem@V?$NiPointer@VNiDynamicEffect@@@@@@QAE@XZ @game/BridgeObjectClass
// FUNCTION: STBC 0x417DC0 ??0?$NiTListItem@V?$NiPointer@VNiAVObject@@@@@@QAE@XZ @game/SetClass
// FUNCTION: STBC 0x665C40 ??0?$NiTListItem@V?$NiPointer@VNiTimeController@@@@@@QAE@XZ @game/BridgeObjectClass
// FUNCTION: STBC 0x665BC0 ??1?$NiTListItem@V?$NiPointer@VNiDynamicEffect@@@@@@QAE@XZ @game/BridgeObjectClass
// FUNCTION: STBC 0x417DD0 ??1?$NiTListItem@V?$NiPointer@VNiAVObject@@@@@@QAE@XZ @game/SetClass
// FUNCTION: STBC 0x665C50 ??1?$NiTListItem@V?$NiPointer@VNiTimeController@@@@@@QAE@XZ @game/BridgeObjectClass
// FUNCTION: STBC 0x665B50 ??_E?$NiTListItem@V?$NiPointer@VNiDynamicEffect@@@@@@QAEPAXI@Z @game/BridgeObjectClass
// FUNCTION: STBC 0x417D60 ??_E?$NiTListItem@V?$NiPointer@VNiAVObject@@@@@@QAEPAXI@Z @game/SetClass
// FUNCTION: STBC 0x665BE0 ??_E?$NiTListItem@V?$NiPointer@VNiTimeController@@@@@@QAEPAXI@Z @game/BridgeObjectClass
// FUNCTION: STBC 0x4E9710 ??0?$NiTListItem@V?$NiPointer@VNiNode@@@@@@QAE@XZ @game/PulsingLight
// FUNCTION: STBC 0x4E91D0 ??1?$NiTListItem@V?$NiPointer@VNiNode@@@@@@QAE@XZ @game/PulsingLight
// FUNCTION: STBC 0x4E9170 ??_E?$NiTListItem@V?$NiPointer@VNiNode@@@@@@QAEPAXI@Z @game/PulsingLight
template <class T> class NiTListItem {
public:
	T					element;
	NiTListItem *		next;
	NiTListItem *		prev;
};

/*
================
NiTListBlock

A block of items; the items are free until NewItem hands them out.
================
*/
// FUNCTION: STBC 0x417CE0 ??_G?$NiTListBlock@PAVObjectClass@@@@QAEPAXI@Z @game/SetClass
// FUNCTION: STBC 0x42E690 ??_G?$NiTListBlock@PAVCameraMode@@@@QAEPAXI@Z @game/CameraObjectClass
// FUNCTION: STBC 0x478630 ??_G?$NiTListBlock@J@@QAEPAXI@Z @game/BuilderAI
// FUNCTION: STBC 0x47AC80 ??_G?$NiTListBlock@PAU_object@@@@QAEPAXI@Z @game/ConditionalAI
// FUNCTION: STBC 0x4BB9C0 ??_G?$NiTListBlock@PAVVisibleDamagePiece@@@@QAEPAXI@Z @game/VisibleDamageBuilder
// FUNCTION: STBC 0x502250 ??_G?$NiTListBlock@PAVShipClass@@@@QAEPAXI@Z @game/MapWindow
// FUNCTION: STBC 0x665B10 ??_G?$NiTListBlock@V?$NiPointer@VNiDynamicEffect@@@@@@QAEPAXI@Z @game/BridgeObjectClass
// FUNCTION: STBC 0x55BDC0 ??_G?$NiTListBlock@PAUBoundKey@@@@QAEPAXI@Z @game/KeyboardBinding
// FUNCTION: STBC 0x486CF0 ??_G?$NiTListBlock@H@@QAEPAXI@Z @game/OptimizedFireScript
// FUNCTION: STBC 0x486790 ??_G?$NiTListBlock@PAVWeaponSystem@@@@QAEPAXI@Z @game/OptimizedFireScript
// FUNCTION: STBC 0x486D20 ??_G?$NiTListBlock@PAVShipSubsystem@@@@QAEPAXI@Z @game/OptimizedFireScript
// FUNCTION: STBC 0x7311C0 ??_G?$NiTListBlock@PAVTGUIObject@@@@QAEPAXI@Z @game/TGUIObject
template <class T> class NiTListBlock {
public:
						NiTListBlock( unsigned int size );
						~NiTListBlock() { delete [] items; }

	NiTListBlock *		next;		// the next block of the same list
	NiTListItem<T> *	items;		// heap block of size items
};

/*
================
NiTListBlock::NiTListBlock

Allocates the items and chains them through their next links.
================
*/
// FUNCTION: STBC 0x7C9530 ??0?$NiTListBlock@PAVNi3DDeviceDesc@@@@QAE@I@Z
// FUNCTION: STBC 0x417D10 ??0?$NiTListBlock@PAU_object@@@@QAE@I@Z @game/SetClass
// FUNCTION: STBC 0x434A00 ??0?$NiTListBlock@J@@QAE@I@Z @game/LightObjectClass
// FUNCTION: STBC 0x41bb20 ??0?$NiTListBlock@PAVShipClass@@@@QAE@I@Z @game/MapWindow
// FUNCTION: STBC 0x46F010 ??0?$NiTListBlock@PAVNiTriShape@@@@QAE@I@Z @game/ModelMerge
// FUNCTION: STBC 0x735BA0 ??0?$NiTListBlock@PAVTGFontGroup@@@@QAE@I@Z @game/TGFontManager
// FUNCTION: STBC 0x478660 ??0?$NiTListBlock@PAD@@QAE@I@Z @game/BuilderAI
// FUNCTION: STBC 0x5B3BD0 ??0?$NiTListBlock@PAVTractorBeamProjector@@@@QAE@I@Z @game/ShipClass
// FUNCTION: STBC 0x566AD0 ??0?$NiTListBlock@PAVShipSubsystem@@@@QAE@I@Z @game/RepairSubsystem
// FUNCTION: STBC 0x586BB0 ??0?$NiTListBlock@PAVObjectClass@@@@QAE@I@Z @game/WeaponSystem
// FUNCTION: STBC 0x586C00 ??0?$NiTListBlock@PAUWeaponSystemTarget@@@@QAE@I@Z @game/WeaponSystem
// FUNCTION: STBC 0x59C6B0 ??0?$NiTListBlock@PAUDistanceSphere@@@@QAE@I@Z @game/MetaNebula
// FUNCTION: STBC 0x586C50 ??0?$NiTListBlock@PAVWeapon@@@@QAE@I@Z @game/WeaponSystem
// FUNCTION: STBC 0x564E90 ??0?$NiTListBlock@PAVPoweredSubsystem@@@@QAE@I@Z @game/PowerSubsystem
// FUNCTION: STBC 0x6B8C60 ??0?$NiTListBlock@PAVTGMessage@@@@QAE@I@Z @game/TGMessage
// FUNCTION: STBC 0x712100 ??0?$NiTListBlock@PAX@@QAE@I@Z @game/TGSoundManager
// FUNCTION: STBC 0x729030 ??0?$NiTListBlock@PAVTGUIObject@@@@QAE@I@Z @game/TGRootPane
// FUNCTION: STBC 0x6E7C50 ??0?$NiTListBlock@H@@QAE@I@Z @game/TGInputManager
// FUNCTION: STBC 0x6E7CA0 ??0?$NiTListBlock@PAVTGUnicodeKeyList@@@@QAE@I@Z @game/TGInputManager
// FUNCTION: STBC 0x66F7C0 ??0?$NiTListBlock@H@@QAE@I@Z @game/CharacterClass
// FUNCTION: STBC 0x71A030 ??0?$NiTListBlock@M@@QAE@I@Z @game/TGProfilingInfo
// FUNCTION: STBC 0x71A080 ??0?$NiTListBlock@K@@QAE@I@Z @game/TGProfilingInfo
// FUNCTION: STBC 0x55C300 ??0?$NiTListBlock@PAUBoundKey@@@@QAE@I@Z @game/KeyboardBinding
// FUNCTION: STBC 0x486D50 ??0?$NiTListBlock@PAVWeaponSystem@@@@QAE@I@Z @game/OptimizedFireScript
// FUNCTION: STBC 0x486DA0 ??0?$NiTListBlock@PAUFireTorpRating@@@@QAE@I@Z @game/OptimizedFireScript
// FUNCTION: STBC 0x6B03F0 ??0?$NiTListBlock@PAVTGMovieAction@@@@QAE@I@Z @game/TGMovieManager
// FUNCTION: STBC 0x458710 ??0?$NiTListBlock@UTGOverlayRun@@@@QAE@I@Z @game/TGOverlayController
// (0x458710: retail tests the new item block for NULL, as for an element type with a constructor; TGOverlayRun has none here)
// FUNCTION: STBC 0x70D580 ??0?$NiTListBlock@PAUTGFadePoint@@@@QAE@I@Z @game/TGSound
// (0x70D580: our TGSound::TGSound( TGSound * ) still inlines it, so game/TGSound emits no copy yet)
template <class T> inline NiTListBlock<T>::NiTListBlock( unsigned int size ) {
	next = 0;
	items = new NiTListItem<T>[size];
	assert( items );
	for ( unsigned int i = 0; i < size - 1; i++ ) {
		items[i].next = &items[i + 1];
	}
	items[size - 1].next = 0;
}

template <class T> class NiTList {
public:
						NiTList( unsigned int blockSize_ );
						~NiTList();

	unsigned int		GetSize() const { return count; }
	bool				IsEmpty() const { return count == 0; }

	void				AddHead( T element );
	void				AddTail( T element );
	T					RemoveHead();
	T					RemoveTail();

	// walking the list
	NiTListIterator		GetHeadPos() const { return head; }
	NiTListIterator		GetTailPos() const { return tail; }
	NiTListIterator		GetNextPos( NiTListIterator pos ) const { return pos ? ( (NiTListItem<T> *)pos )->next : 0; }
	NiTListIterator		GetPrevPos( NiTListIterator pos ) const { return pos ? ( (NiTListItem<T> *)pos )->prev : 0; }
	// FUNCTION: STBC 0x665470 ?GetHead@?$NiTList@V?$NiPointer@VNiTimeController@@@@@@QBE?AV?$NiPointer@VNiTimeController@@@@XZ @game/BridgeObjectClass
	T					GetHead() const { return head->element; }	// the list must not be empty
	T					GetTail() const { return tail->element; }	// the list must not be empty
	// the element at pos; an empty element at the end. Unlike Gamebryo
	// 1.1.2's Get (an assert only), BC's tests pos like GetNextPos: retail
	// reads the element and the next position under one test and zeroes both
	// on NULL, element first, even inside a while ( pos ) walk
	// (FocusManager::Tab 0x44D650, ShipSubsystems' slot 3 0x5B7160,
	// VisibleDamagePiece 0x4B3D70). T() rather than 0: the list also holds
	// structs (TGOverlayImage::Build's runs).
	T					Get( NiTListIterator pos ) const { return pos ? ( (NiTListItem<T> *)pos )->element : T(); }
	T					GetNext( NiTListIterator &pos ) const;
	T					GetPrev( NiTListIterator &pos ) const;
	NiTListIterator		FindPos( T element, NiTListIterator start = 0 ) const;

	NiTListIterator		InsertBefore( NiTListIterator pos, T element );
	NiTListIterator		InsertAfter( NiTListIterator pos, T element );

	T					RemovePos( NiTListIterator &pos );
	T					Remove( T element );

	// frees every item at once (Gamebryo lineage name)
	void				RemoveAll();

protected:
	void				NewItem( NiTListItem<T> *&item );
	void				DeleteItem( NiTListItem<T> *item );

	unsigned int		count;		// items in the list
	NiTListItem<T> *	head;
	NiTListItem<T> *	tail;
	NiTListItem<T> *	freeList;	// unused items, chained through their next links
	NiTListBlock<T> *	blocks;		// every block allocated, newest first
	unsigned int		blockSize;	// items per block
};

/*
================
NiTList::NiTList
================
*/
// FUNCTION: STBC 0x462920 ??0?$NiTList@PAVNiD3DDisplayMode@@@@QAE@I@Z
// FUNCTION: STBC 0x7C7330 ??0?$NiTList@PAVNiDDImageSurface@@@@QAE@I@Z @netimmerse/renderer/NiDDImage
// FUNCTION: STBC 0x477C90 ??0?$NiTList@PAD@@QAE@I@Z @game/BuilderAI
// FUNCTION: STBC 0x665010 ??0?$NiTList@PAVTGNamedModelPropertyList@@@@QAE@I@Z @game/BridgeObjectClass
// FUNCTION: STBC 0x66F4B0 ??0?$NiTList@PAUQueuedSound@@@@QAE@I@Z @game/CharacterClass
// FUNCTION: STBC 0x4C41F0 ??0?$NiTList@PAVDamageVoxelGrid@@@@QAE@I@Z @game/DamagePolygonizer
template <class T> inline NiTList<T>::NiTList( unsigned int blockSize_ ) {
	count = 0;
	head = 0;
	tail = 0;
	freeList = 0;
	blocks = 0;
	// a block holds at least two items (the out-of-line constructor
	// 0x7C7330 clamps its argument)
	blockSize = ( blockSize_ > 2 ) ? blockSize_ : 2;
}

/*
================
NiTList::~NiTList
================
*/
// FUNCTION: STBC 0x59C380 ??1?$NiTList@PAUDistanceSphere@@@@QAE@XZ @game/MetaNebula
// FUNCTION: STBC 0x6FB8B0 ??1?$NiTList@PAUPyGetStateFunction@@@@QAE@XZ @game/PyEmbed
// FUNCTION: STBC 0x6FB8F0 ??1?$NiTList@PAUPySetStateFunction@@@@QAE@XZ @game/PyEmbed
// FUNCTION: STBC 0x409BE0 ??1?$NiTList@PAUPrecreatedShip@@@@QAE@XZ @game/Mission
// FUNCTION: STBC 0x428780 ??1?$NiTList@PAVCameraLagTracker@@@@QAE@XZ @game/CameraMode
// FUNCTION: STBC 0x428640 ??1?$NiTList@PAVTGObject@@@@QAE@XZ @game/CameraMode
// FUNCTION: STBC 0x7D1DB0 ??1?$NiTList@PAVNiLight@@@@QAE@XZ
// FUNCTION: STBC 0x46EDE0 ??1?$NiTList@PAVNiTriShape@@@@QAE@XZ @game/ModelMerge
// FUNCTION: STBC 0x46EE20 ??1?$NiTList@PAV?$NiTList@PAVNiTriShape@@@@@@QAE@XZ @game/ModelMerge
// FUNCTION: STBC 0x46D6A0 ??1?$NiTList@PAUDistanceRange@@@@QAE@XZ @game/DistanceModifier
// FUNCTION: STBC 0x545370 ??1?$NiTList@PAURadarBracket@@@@QAE@XZ @game/RadarScope
// FUNCTION: STBC 0x541E80 ??1?$NiTList@PAVDamageIcon@@@@QAE@XZ @game/DamageDisplay
// FUNCTION: STBC 0x4EFC40 ??1?$NiTList@PAX@@QAE@XZ @game/SpecularPass
// FUNCTION: STBC 0x66F380 ??1?$NiTList@PAUCharacterAnimation@@@@QAE@XZ @game/CharacterClass
// FUNCTION: STBC 0x7C7360 ??1?$NiTList@PAVNiDDImageSurface@@@@QAE@XZ
// FUNCTION: STBC 0x42DC40 ??1?$NiTList@PAVCameraMode@@@@QAE@XZ @game/CameraObjectClass
// FUNCTION: STBC 0x7C4AD0 ??1?$NiTList@PAVNi3DDeviceDesc@@@@QAE@XZ @netimmerse/renderer/NiD3DRender
// FUNCTION: STBC 0x462BA0 ??_G?$NiTList@PAVNiD3DDisplayMode@@@@QAEPAXI@Z @netimmerse/renderer/NiD3DRender
// FUNCTION: STBC 0x70A760 ??1?$NiTList@PAUTGProviderReserve@@@@QAE@XZ @game/TGSoundModule
// FUNCTION: STBC 0x70A7A0 ??1?$NiTList@PAVTGSound@@@@QAE@XZ @game/TGSoundModule
// FUNCTION: STBC 0x477CC0 ??1?$NiTList@PAD@@QAE@XZ @game/BuilderAI
// FUNCTION: STBC 0x478480 ??1?$NiTList@J@@QAE@XZ @game/BuilderAI
// FUNCTION: STBC 0x434410 ??1?$NiTList@J@@QAE@XZ @game/LightObjectClass
// FUNCTION: STBC 0x471BB0 ??1?$NiTList@PAVArtificialIntelligence@@@@QAE@XZ @game/ArtificialIntelligence
// FUNCTION: STBC 0x473600 ??1?$NiTList@PAVArtificialIntelligence@@@@QAE@XZ @game/AIManager
// FUNCTION: STBC 0x48AF30 ??1?$NiTList@PAJ@@QAE@XZ @game/OptimizedSelectTarget
// FUNCTION: STBC 0x487C80 ??1?$NiTList@PAUPowerShare@@@@QAE@XZ @game/OptimizedManagePower
// FUNCTION: STBC 0x417530 ??1?$NiTList@PAVObjectClass@@@@QAE@XZ @game/SetClass
// FUNCTION: STBC 0x4177F0 ??1?$NiTList@PAVShipClass@@@@QAE@XZ @game/SetClass
// FUNCTION: STBC 0x4178B0 ??1?$NiTList@PAU_object@@@@QAE@XZ @game/SetClass
// FUNCTION: STBC 0x4813C0 ??1?$NiTList@PAUAvoidedObstacle@@@@QAE@XZ @game/OptimizedAvoidObstacles
// FUNCTION: STBC 0x481480 ??1?$NiTList@PAUAvoidanceCandidate@@@@QAE@XZ @game/OptimizedAvoidObstacles
// FUNCTION: STBC 0x54F660 ??1?$NiTList@PAVPoweredSubsystem@@@@QAE@XZ @game/PowerSubsystem
// FUNCTION: STBC 0x6B8B90 ??1?$NiTList@PAVTGMessage@@@@QAE@XZ @game/TGMessage
// FUNCTION: STBC 0x70AB10 ??1?$NiTList@PAUTGMusicFade@@@@QAE@XZ @game/TGSoundModule
// FUNCTION: STBC 0x70CF70 ??1?$NiTList@PAUTGFadePoint@@@@QAE@XZ @game/TGSound
// FUNCTION: STBC 0x728E50 ??1?$NiTList@PAVTGUIObject@@@@QAE@XZ @game/TGRootPane
// FUNCTION: STBC 0x4C4220 ??1?$NiTList@PAVDamageVoxelGrid@@@@QAE@XZ @game/DamagePolygonizer
// FUNCTION: STBC 0x4C4190 ??1?$NiTList@UDamageCell@@@@QAE@XZ @game/DamagePolygonizer
// FUNCTION: STBC 0x4BB000 ??1?$NiTList@PAVMetaVolume@@@@QAE@XZ @game/VisibleDamageBuilder
// FUNCTION: STBC 0x4BB040 ??1?$NiTList@PAVVisibleDamagePiece@@@@QAE@XZ @game/VisibleDamageBuilder
// FUNCTION: STBC 0x4C4300 ??1?$NiTList@PAUVoxelPolygon@@@@QAE@XZ @game/DamagePolygonizer
// FUNCTION: STBC 0x4C4340 ??1?$NiTList@_J@@QAE@XZ @game/DamagePolygonizer
// FUNCTION: STBC 0x5B39E0 ??1?$NiTList@PAVTractorBeamProjector@@@@QAE@XZ @game/ShipClass
// FUNCTION: STBC 0x4E37D0 ??1?$NiTList@PAULensFlareImage@@@@QAE@XZ @game/LensFlare
// FUNCTION: STBC 0x665040 ??1?$NiTList@V?$NiPointer@VNiDynamicEffect@@@@@@QAE@XZ @game/BridgeObjectClass
// FUNCTION: STBC 0x417570 ??1?$NiTList@V?$NiPointer@VNiAVObject@@@@@@QAE@XZ @game/SetClass
// FUNCTION: STBC 0x6652A0 ??1?$NiTList@V?$NiPointer@VNiTimeController@@@@@@QAE@XZ @game/BridgeObjectClass
// FUNCTION: STBC 0x446C60 ??1?$NiTList@PAVMainWindow@@@@QAE@XZ @game/UtopiaModule
// FUNCTION: STBC 0x446C20 ??1?$NiTList@PAUQueuedSound@@@@QAE@XZ @game/UtopiaModule
// FUNCTION: STBC 0x4752C0 ??1?$NiTList@PAUAILogScope@@@@QAE@XZ @game/AILog
// FUNCTION: STBC 0x475320 ??1?$NiTList@PAUAILogTiming@@@@QAE@XZ @game/AILog
// FUNCTION: STBC 0x4584E0 ??1?$NiTList@UTGOverlayRun@@@@QAE@XZ @game/TGOverlayController
// FUNCTION: STBC 0x6E7620 ??1?$NiTList@PAVTGUnicodeKeyList@@@@QAE@XZ @game/TGInputManager
// FUNCTION: STBC 0x5020A0 ??1?$NiTList@PAUMapStalk@@@@QAE@XZ @game/MapWindow
// FUNCTION: STBC 0x7359F0 ??1?$NiTList@PAVTGFontRegistration@@@@QAE@XZ @game/TGFontManager
// FUNCTION: STBC 0x735A30 ??1?$NiTList@PAVTGFontGroup@@@@QAE@XZ @game/TGFontManager
// FUNCTION: STBC 0x735A70 ??1?$NiTList@PAVTGFontFamily@@@@QAE@XZ @game/TGFontManager
// FUNCTION: STBC 0x72AD90 ??1?$NiTList@PAVTGIconGroup@@@@QAE@XZ @game/TGIconManager
// FUNCTION: STBC 0x72ADD0 ??1?$NiTList@PAVTGIconPoly@@@@QAE@XZ @game/TGIconManager
// FUNCTION: STBC 0x731150 ??1?$NiTList@PAVTGUIObject@@@@QAE@XZ @game/TGUIObject
// FUNCTION: STBC 0x4E8CC0 ??1?$NiTList@V?$NiPointer@VNiNode@@@@@@QAE@XZ @game/PulsingLight
// FUNCTION: STBC 0x54CCA0 ??1?$NiTList@PAUWeaponChargeChecks@@@@QAE@XZ @game/WeaponsDisplay
// FUNCTION: STBC 0x55BB00 ??1?$NiTList@PAUBoundKey@@@@QAE@XZ @game/KeyboardBinding
// FUNCTION: STBC 0x569B30 ??1?$NiTList@PAUSensorIdentification@@@@QAE@XZ @game/SensorSubsystem
// FUNCTION: STBC 0x586840 ??1?$NiTList@PAUWeaponSystemTarget@@@@QAE@XZ @game/WeaponSystem
// FUNCTION: STBC 0x5868C0 ??1?$NiTList@PAVFiringChain@@@@QAE@XZ @game/WeaponSystem
// FUNCTION: STBC 0x586980 ??1?$NiTList@PAVWeapon@@@@QAE@XZ @game/WeaponSystem
// FUNCTION: STBC 0x4863A0 ??1?$NiTList@H@@QAE@XZ @game/OptimizedFireScript
// FUNCTION: STBC 0x486430 ??1?$NiTList@PAUFireTargetSubsystem@@@@QAE@XZ @game/OptimizedFireScript
// FUNCTION: STBC 0x486470 ??1?$NiTList@PAVWeaponSystem@@@@QAE@XZ @game/OptimizedFireScript
// FUNCTION: STBC 0x4866D0 ??1?$NiTList@PAVShipSubsystem@@@@QAE@XZ @game/OptimizedFireScript
// FUNCTION: STBC 0x486710 ??1?$NiTList@PAUFireTorpRating@@@@QAE@XZ @game/OptimizedFireScript
// FUNCTION: STBC 0x486750 ??1?$NiTList@PAVNiPoint3@@@@QAE@XZ @game/OptimizedFireScript
// FUNCTION: STBC 0x6B0380 ??1?$NiTList@PAVTGMovieAction@@@@QAE@XZ @game/TGMovieManager
// FUNCTION: STBC 0x6C4520 ??1?$NiTList@PAVTGModelContainer@@@@QAE@XZ @game/TGModelManager
template <class T> inline NiTList<T>::~NiTList() {
	RemoveAll();
}

/*
================
NiTList::RemoveAll

Frees the blocks, and with them every item, without visiting the items.

Evidence: WeaponSystem::DeleteFiringChains (0x585200) ends with this
body inline, outside a destructor.
================
*/
// FUNCTION: STBC 0x7C4C40 ?RemoveAll@?$NiTList@PAVNi3DDeviceDesc@@@@QAEXXZ
// FUNCTION: STBC 0x46D7E0 ?RemoveAll@?$NiTList@PAUDistanceRange@@@@QAEXXZ @game/DistanceModifier
// FUNCTION: STBC 0x4F0240 ?RemoveAll@?$NiTList@PAX@@QAEXXZ @game/SpecularPass
// FUNCTION: STBC 0x5B3B60 ?RemoveAll@?$NiTList@PAVPoweredSubsystem@@@@QAEXXZ @game/ShipClass
// FUNCTION: STBC 0x47AB30 ?RemoveAll@?$NiTList@PAU_object@@@@QAEXXZ @game/ConditionalAI
// FUNCTION: STBC 0x66F6F0 ?RemoveAll@?$NiTList@PAUCharacterAnimation@@@@QAEXXZ @game/CharacterClass
// FUNCTION: STBC 0x66F670 ?RemoveAll@?$NiTList@J@@QAEXXZ @game/CharacterClass
// FUNCTION: STBC 0x66F6B0 ?RemoveAll@?$NiTList@H@@QAEXXZ @game/CharacterClass
// FUNCTION: STBC 0x486840 ?RemoveAll@?$NiTList@H@@QAEXXZ @game/OptimizedFireScript
// FUNCTION: STBC 0x5021E0 ?RemoveAll@?$NiTList@PAVShipClass@@@@QAEXXZ @game/MapWindow
// FUNCTION: STBC 0x5020E0 ?RemoveAll@?$NiTList@PAUMapStalk@@@@QAEXXZ @game/MapWindow
// FUNCTION: STBC 0x665940 ?RemoveAll@?$NiTList@V?$NiPointer@VNiTimeController@@@@@@QAEXXZ @game/BridgeObjectClass
// FUNCTION: STBC 0x596430 ?RemoveAll@?$NiTList@PAVNiPoint3@@@@QAEXXZ @game/DamageableObject
// FUNCTION: STBC 0x48B3B0 ?RemoveAll@?$NiTList@PAVObjectClass@@@@QAEXXZ @game/OptimizedSelectTarget
// FUNCTION: STBC 0x55BB40 ?RemoveAll@?$NiTList@PAUBoundKey@@@@QAEXXZ @game/KeyboardBinding
// FUNCTION: STBC 0x55BBC0 ?RemoveAll@?$NiTList@PAUKeyBind@@@@QAEXXZ @game/KeyboardBinding
// FUNCTION: STBC 0x486BA0 ?RemoveAll@?$NiTList@PAVShipSubsystem@@@@QAEXXZ @game/OptimizedFireScript
// FUNCTION: STBC 0x554FD0 ?RemoveAll@?$NiTList@PAVNiD3DDisplayMode@@@@QAEXXZ @game/GraphicsMenu
// FUNCTION: STBC 0x4C4A90 ?RemoveAll@?$NiTList@PAUVoxelPolygon@@@@QAEXXZ @game/DamagePolygonizer
// FUNCTION: STBC 0x4C4380 ?RemoveAll@?$NiTList@_J@@QAEXXZ @game/DamagePolygonizer
// FUNCTION: STBC 0x4C4260 ?RemoveAll@?$NiTList@PAVDamageVoxelGrid@@@@QAEXXZ @game/DamagePolygonizer
// FUNCTION: STBC 0x4C4780 ?RemoveAll@?$NiTList@UDamageCell@@@@QAEXXZ @game/DamagePolygonizer
// FUNCTION: STBC 0x4C4620 ?RemoveAll@?$NiTList@PAVMetaVolume@@@@QAEXXZ @game/DamagePolygonizer
// FUNCTION: STBC 0x42E410 ?RemoveAll@?$NiTList@PAVCameraMode@@@@QAEXXZ @game/CameraObjectClass
template <class T> inline void NiTList<T>::RemoveAll() {
	NiTListBlock<T> *block = blocks;
	while ( block ) {
		NiTListBlock<T> *save = block;
		block = block->next;
		delete save;
	}
	count = 0;
	head = 0;
	tail = 0;
	freeList = 0;
	blocks = 0;
}

/*
================
NiTList::NewItem

Takes an item from the free list, allocating a block first when the free
list is empty.
================
*/
// FUNCTION: STBC 0x59C5C0 ?NewItem@?$NiTList@PAUDistanceSphere@@@@IAEXAAPAV?$NiTListItem@PAUDistanceSphere@@@@@Z @game/MetaNebula
// FUNCTION: STBC 0x4293B0 ?NewItem@?$NiTList@PAUCameraLagSample@@@@IAEXAAPAV?$NiTListItem@PAUCameraLagSample@@@@@Z @game/CameraMode
// FUNCTION: STBC 0x672F40 ?NewItem@?$NiTList@PAUQueuedSound@@@@IAEXAAPAV?$NiTListItem@PAUQueuedSound@@@@@Z @game/CharacterSpeakingQueue
// FUNCTION: STBC 0x6D07A0 ?NewItem@?$NiTList@PAUTGLocalizationEntry@@@@IAEXAAPAV?$NiTListItem@PAUTGLocalizationEntry@@@@@Z @game/TGLocalizationManager
// FUNCTION: STBC 0x707F10 ?NewItem@?$NiTList@PAUTGPhonemeEntry@@@@IAEXAAPAV?$NiTListItem@PAUTGPhonemeEntry@@@@@Z @game/TGPhoneme
// FUNCTION: STBC 0x70D430 ?NewItem@?$NiTList@PAUTGFadePoint@@@@IAEXAAPAV?$NiTListItem@PAUTGFadePoint@@@@@Z @game/TGSound
// FUNCTION: STBC 0x4292B0 ?NewItem@?$NiTList@PAVTGObject@@@@IAEXAAPAV?$NiTListItem@PAVTGObject@@@@@Z @game/CameraMode
// FUNCTION: STBC 0x6EBCA0 ?NewItem@?$NiTList@PAVTGNamedModelPropertyList@@@@IAEXAAPAV?$NiTListItem@PAVTGNamedModelPropertyList@@@@@Z @game/TGModelPropertySet
// FUNCTION: STBC 0x6C4640 ?NewItem@?$NiTList@PAVTGModelContainer@@@@IAEXAAPAV?$NiTListItem@PAVTGModelContainer@@@@@Z @game/TGModelManager
// FUNCTION: STBC 0x735AB0 ?NewItem@?$NiTList@PAVTGFontGroup@@@@IAEXAAPAV?$NiTListItem@PAVTGFontGroup@@@@@Z @game/TGFontManager
// FUNCTION: STBC 0x6EA5B0 ?NewItem@?$NiTList@PAUTGModelPropertyFilterRecord@@@@IAEXAAPAV?$NiTListItem@PAUTGModelPropertyFilterRecord@@@@@Z @game/TGModelPropertyManager
// FUNCTION: STBC 0x6EA670 ?NewItem@?$NiTList@P6A_NPAVTGModelProperty@@@Z@@IAEXAAPAV?$NiTListItem@P6A_NPAVTGModelProperty@@@Z@@@Z @game/TGModelPropertyManager
// FUNCTION: STBC 0x6EA750 ?NewItem@?$NiTList@PAVTGModelProperty@@@@IAEXAAPAV?$NiTListItem@PAVTGModelProperty@@@@@Z @game/TGModelPropertyManager
// FUNCTION: STBC 0x4BB410 ?NewItem@?$NiTList@PAVMetaVolume@@@@IAEXAAPAV?$NiTListItem@PAVMetaVolume@@@@@Z @game/VisibleDamageBuilder
// FUNCTION: STBC 0x4BB870 ?NewItem@?$NiTList@PAVVisibleDamagePiece@@@@IAEXAAPAV?$NiTListItem@PAVVisibleDamagePiece@@@@@Z @game/VisibleDamageBuilder
// FUNCTION: STBC 0x417BF0 ?NewItem@?$NiTList@PAVShipClass@@@@IAEXAAPAV?$NiTListItem@PAVShipClass@@@@@Z @game/SetClass
// FUNCTION: STBC 0x54CCE0 ?NewItem@?$NiTList@PAUWeaponChargeChecks@@@@IAEXAAPAV?$NiTListItem@PAUWeaponChargeChecks@@@@@Z @game/WeaponsDisplay
// FUNCTION: STBC 0x6DFFB0 ?NewItem@?$NiTList@PAUPyStreamCallback@@@@IAEXAAPAV?$NiTListItem@PAUPyStreamCallback@@@@@Z @game/TGPythonInstanceWrapper
// FUNCTION: STBC 0x46EE60 ?NewItem@?$NiTList@PAVNiTriShape@@@@IAEXAAPAV?$NiTListItem@PAVNiTriShape@@@@@Z @game/ModelMerge
// FUNCTION: STBC 0x46EF20 ?NewItem@?$NiTList@PAV?$NiTList@PAVNiTriShape@@@@@@IAEXAAPAV?$NiTListItem@PAV?$NiTList@PAVNiTriShape@@@@@@@Z @game/ModelMerge
// FUNCTION: STBC 0x46D820 ?NewItem@?$NiTList@PAUDistanceRange@@@@IAEXAAPAV?$NiTListItem@PAUDistanceRange@@@@@Z @game/DistanceModifier
// FUNCTION: STBC 0x5454B0 ?NewItem@?$NiTList@PAURadarBracket@@@@IAEXAAPAV?$NiTListItem@PAURadarBracket@@@@@Z @game/RadarScope
// FUNCTION: STBC 0x4F0280 ?NewItem@?$NiTList@PAX@@IAEXAAPAV?$NiTListItem@PAX@@@Z @game/SpecularPass
// FUNCTION: STBC 0x4AC660 ?NewItem@?$NiTList@PAUVoxelPolygon@@@@IAEXAAPAV?$NiTListItem@PAUVoxelPolygon@@@@@Z @game/NiBinaryVoxelData
// FUNCTION: STBC 0x417B30 ?NewItem@?$NiTList@PAVObjectClass@@@@IAEXAAPAV?$NiTListItem@PAVObjectClass@@@@@Z @game/SetClass
// FUNCTION: STBC 0x7C9440 ?NewItem@?$NiTList@PAVNiD3DDirectDraw@@@@IAEXAAPAV?$NiTListItem@PAVNiD3DDirectDraw@@@@@Z
// FUNCTION: STBC 0x7C79C0 ?NewItem@?$NiTList@PAVNiDDImage@@@@IAEXAAPAV?$NiTListItem@PAVNiDDImage@@@@@Z
// FUNCTION: STBC 0x7C7690 ?NewItem@?$NiTList@PAVNiDDImageSurface@@@@IAEXAAPAV?$NiTListItem@PAVNiDDImageSurface@@@@@Z
// FUNCTION: STBC 0x7D79B0 ?NewItem@?$NiTList@PAU_DPSetupTexture@NiTextureManager@@@@IAEXAAPAV?$NiTListItem@PAU_DPSetupTexture@NiTextureManager@@@@@Z
// FUNCTION: STBC 0x462C40 ?NewItem@?$NiTList@PAVNiD3DDisplayMode@@@@IAEXAAPAV?$NiTListItem@PAVNiD3DDisplayMode@@@@@Z @game/GraphicsModeInfo
// FUNCTION: STBC 0x7E1E80 ?NewItem@?$NiTList@PAVNiRender@@@@IAEXAAPAV?$NiTListItem@PAVNiRender@@@@@Z @netimmerse/renderer/NiImage
// FUNCTION: STBC 0x54F720 ?NewItem@?$NiTList@PAVPoweredSubsystem@@@@IAEXAAPAV?$NiTListItem@PAVPoweredSubsystem@@@@@Z @game/PowerSubsystem
// FUNCTION: STBC 0x5420C0 ?NewItem@?$NiTList@PAVDamageIcon@@@@IAEXAAPAV?$NiTListItem@PAVDamageIcon@@@@@Z @game/DamageDisplay
// FUNCTION: STBC 0x728EA0 ?NewItem@?$NiTList@PAVTGUIObject@@@@IAEXAAPAV?$NiTListItem@PAVTGUIObject@@@@@Z @game/TGRootPane
// FUNCTION: STBC 0x502120 ?NewItem@?$NiTList@PAUMapStalk@@@@IAEXAAPAV?$NiTListItem@PAUMapStalk@@@@@Z @game/MapWindow
// FUNCTION: STBC 0x72AFF0 ?NewItem@?$NiTList@PAVTGIconGroup@@@@IAEXAAPAV?$NiTListItem@PAVTGIconGroup@@@@@Z @game/TGIconManager
// FUNCTION: STBC 0x70EB40 ?NewItem@?$NiTList@PAVTGSound@@@@IAEXAAPAV?$NiTListItem@PAVTGSound@@@@@Z @game/TGSoundRegion
// FUNCTION: STBC 0x711FB0 ?NewItem@?$NiTList@PAUTGProviderReserve@@@@IAEXAAPAV?$NiTListItem@PAUTGProviderReserve@@@@@Z @game/TGSoundManager
// FUNCTION: STBC 0x4784C0 ?NewItem@?$NiTList@PAD@@IAEXAAPAV?$NiTListItem@PAD@@@Z @game/BuilderAI
// FUNCTION: STBC 0x47AB90 ?NewItem@?$NiTList@PAVArtificialIntelligence@@@@IAEXAAPAV?$NiTListItem@PAVArtificialIntelligence@@@@@Z @game/ConditionalAI
// FUNCTION: STBC 0x523960 ?NewItem@?$NiTList@PAVSTFillGauge@@@@IAEXAAPAV?$NiTListItem@PAVSTFillGauge@@@@@Z @game/STFillGauge
// FUNCTION: STBC 0x42E450 ?NewItem@?$NiTList@PAVCameraMode@@@@IAEXAAPAV?$NiTListItem@PAVCameraMode@@@@@Z @game/CameraObjectClass
// FUNCTION: STBC 0x6FBA70 ?NewItem@?$NiTList@PAUPyMethodDef@@@@IAEXAAPAV?$NiTListItem@PAUPyMethodDef@@@@@Z @game/PyEmbed
// FUNCTION: STBC 0x447250 ?NewItem@?$NiTList@PAVMainWindow@@@@IAEXAAPAV?$NiTListItem@PAVMainWindow@@@@@Z @game/UtopiaModule
// FUNCTION: STBC 0x446F40 ?NewItem@?$NiTList@PAUPyGetStateFunction@@@@IAEXAAPAV?$NiTListItem@PAUPyGetStateFunction@@@@@Z @game/UtopiaModule
// FUNCTION: STBC 0x447000 ?NewItem@?$NiTList@PAUPySetStateFunction@@@@IAEXAAPAV?$NiTListItem@PAUPySetStateFunction@@@@@Z @game/UtopiaModule
// FUNCTION: STBC 0x7153D0 ?NewItem@?$NiTList@PAUTGMusicFade@@@@IAEXAAPAV?$NiTListItem@PAUTGMusicFade@@@@@Z @game/TGMusic
// FUNCTION: STBC 0x434810 ?NewItem@?$NiTList@J@@IAEXAAPAV?$NiTListItem@J@@@Z @game/SensorSubsystem
// FUNCTION: STBC 0x569DB0 ?NewItem@?$NiTList@PAUSensorIdentification@@@@IAEXAAPAV?$NiTListItem@PAUSensorIdentification@@@@@Z @game/SensorSubsystem
// FUNCTION: STBC 0x486880 ?NewItem@?$NiTList@H@@IAEXAAPAV?$NiTListItem@H@@@Z @game/TGInputManager
// FUNCTION: STBC 0x719E80 ?NewItem@?$NiTList@M@@IAEXAAPAV?$NiTListItem@M@@@Z @game/TGProfilingInfo
// FUNCTION: STBC 0x719F40 ?NewItem@?$NiTList@K@@IAEXAAPAV?$NiTListItem@K@@@Z @game/TGProfilingInfo
// FUNCTION: STBC 0x6F1B60 ?NewItem@?$NiTList@PAUTGFixUpCallback@@@@IAEXAAPAV?$NiTListItem@PAUTGFixUpCallback@@@@@Z @game/TGObject
// FUNCTION: STBC 0x486BE0 ?NewItem@?$NiTList@PAVShipSubsystem@@@@IAEXAAPAV?$NiTListItem@PAVShipSubsystem@@@@@Z @game/RepairSubsystem
// FUNCTION: STBC 0x487F00 ?NewItem@?$NiTList@PAUPowerShare@@@@IAEXAAPAV?$NiTListItem@PAUPowerShare@@@@@Z @game/OptimizedManagePower
// FUNCTION: STBC 0x48B2F0 ?NewItem@?$NiTList@PAJ@@IAEXAAPAV?$NiTListItem@PAJ@@@Z @game/OptimizedSelectTarget
// FUNCTION: STBC 0x58AA40 ?NewItem@?$NiTList@PAVCollisionRecord@@@@IAEXAAPAV?$NiTListItem@PAVCollisionRecord@@@@@Z @game/CollisionRecorder
// FUNCTION: STBC 0x486A60 ?NewItem@?$NiTList@PAVNiPoint3@@@@IAEXAAPAV?$NiTListItem@PAVNiPoint3@@@@@Z @game/CollisionRecorder
// FUNCTION: STBC 0x5B3AA0 ?NewItem@?$NiTList@PAVTractorBeamProjector@@@@IAEXAAPAV?$NiTListItem@PAVTractorBeamProjector@@@@@Z @game/ShipClass
// FUNCTION: STBC 0x439720 ?NewItem@?$NiTList@PAVNiSortedObject@@@@IAEXAAPAV?$NiTListItem@PAVNiSortedObject@@@@@Z @netimmerse/NiAlphaAccumulator
// FUNCTION: STBC 0x475980 ?NewItem@?$NiTList@PAUAILogTiming@@@@IAEXAAPAV?$NiTListItem@PAUAILogTiming@@@@@Z @game/AILog
// FUNCTION: STBC 0x481640 ?NewItem@?$NiTList@PAUAvoidedObstacle@@@@IAEXAAPAV?$NiTListItem@PAUAvoidedObstacle@@@@@Z @game/OptimizedAvoidObstacles
// FUNCTION: STBC 0x481700 ?NewItem@?$NiTList@PAUAvoidanceCandidate@@@@IAEXAAPAV?$NiTListItem@PAUAvoidanceCandidate@@@@@Z @game/OptimizedAvoidObstacles
// FUNCTION: STBC 0x6E7B60 ?NewItem@?$NiTList@PAVTGUnicodeKeyList@@@@IAEXAAPAV?$NiTListItem@PAVTGUnicodeKeyList@@@@@Z @game/TGInputManager
// FUNCTION: STBC 0x55C030 ?NewItem@?$NiTList@PAUBoundKey@@@@IAEXAAPAV?$NiTListItem@PAUBoundKey@@@@@Z @game/KeyboardBinding
// FUNCTION: STBC 0x4C47C0 ?NewItem@?$NiTList@UDamageCell@@@@IAEXAAPAV?$NiTListItem@UDamageCell@@@@@Z @game/DamagePolygonizer
// FUNCTION: STBC 0x4C4AD0 ?NewItem@?$NiTList@_J@@IAEXAAPAV?$NiTListItem@_J@@@Z @game/DamagePolygonizer
// FUNCTION: STBC 0x55C210 ?NewItem@?$NiTList@PAUKeyBind@@@@IAEXAAPAV?$NiTListItem@PAUKeyBind@@@@@Z @game/KeyboardBinding
// FUNCTION: STBC 0x4869A0 ?NewItem@?$NiTList@PAVWeaponSystem@@@@IAEXAAPAV?$NiTListItem@PAVWeaponSystem@@@@@Z @game/OptimizedFireScript
// FUNCTION: STBC 0x709580 ?NewItem@?$NiTList@PAVTGPhonemeSequence@@@@IAEXAAPAV?$NiTListItem@PAVTGPhonemeSequence@@@@@Z @game/TGPhonemeSequence
template <class T> inline void NiTList<T>::NewItem( NiTListItem<T> *&item ) {
	if ( !freeList ) {
		NiTListBlock<T> *block = new NiTListBlock<T>( blockSize );
		assert( block );
		block->next = blocks;
		blocks = block;
		freeList = block->items;
	}
	item = freeList;
	freeList = freeList->next;
}

/*
================
NiTList::DeleteItem

Returns an item to the free list.
================
*/
// FUNCTION: STBC 0x6EA8A0 ?DeleteItem@?$NiTList@PAVTGModelProperty@@@@IAEXPAV?$NiTListItem@PAVTGModelProperty@@@@@Z
// FUNCTION: STBC 0x429470 ?DeleteItem@?$NiTList@PAUCameraLagSample@@@@IAEXPAV?$NiTListItem@PAUCameraLagSample@@@@@Z @game/CameraMode
// FUNCTION: STBC 0x475A40 ?DeleteItem@?$NiTList@PAUAILogTiming@@@@IAEXPAV?$NiTListItem@PAUAILogTiming@@@@@Z @game/AILog
// FUNCTION: STBC 0x6F1B40 ?DeleteItem@?$NiTList@PAVTGObject@@@@IAEXPAV?$NiTListItem@PAVTGObject@@@@@Z @game/TGObject
// FUNCTION: STBC 0x7154B0 ?DeleteItem@?$NiTList@PAUTGMusicFade@@@@IAEXPAV?$NiTListItem@PAUTGMusicFade@@@@@Z @game/TGMusic
// FUNCTION: STBC 0x7C9610 ?DeleteItem@?$NiTList@PAVNiD3DDirectDraw@@@@IAEXPAV?$NiTListItem@PAVNiD3DDirectDraw@@@@@Z
// FUNCTION: STBC 0x7C7750 ?DeleteItem@?$NiTList@PAVNiDDImageSurface@@@@IAEXPAV?$NiTListItem@PAVNiDDImageSurface@@@@@Z
// FUNCTION: STBC 0x7C7B20 ?DeleteItem@?$NiTList@PAVNiDDImage@@@@IAEXPAV?$NiTListItem@PAVNiDDImage@@@@@Z
// FUNCTION: STBC 0x7C4C80 ?DeleteItem@?$NiTList@PAVNi3DDeviceDesc@@@@IAEXPAV?$NiTListItem@PAVNi3DDeviceDesc@@@@@Z @netimmerse/renderer/NiD3DRender
// FUNCTION: STBC 0x7D1ED0 ?DeleteItem@?$NiTList@PAVNiLight@@@@IAEXPAV?$NiTListItem@PAVNiLight@@@@@Z
// FUNCTION: STBC 0x47AB70 ?DeleteItem@?$NiTList@PAU_object@@@@IAEXPAV?$NiTListItem@PAU_object@@@@@Z @game/ConditionalAI
// FUNCTION: STBC 0x487FC0 ?DeleteItem@?$NiTList@PAUPowerShare@@@@IAEXPAV?$NiTListItem@PAUPowerShare@@@@@Z @game/OptimizedManagePower
// FUNCTION: STBC 0x72B0B0 ?DeleteItem@?$NiTList@PAVTGIconGroup@@@@IAEXPAV?$NiTListItem@PAVTGIconGroup@@@@@Z @game/TGIconManager
// FUNCTION: STBC 0x44DCA0 ?DeleteItem@?$NiTList@J@@IAEXPAV?$NiTListItem@J@@@Z @game/FocusManager
// FUNCTION: STBC 0x486CA0 ?DeleteItem@?$NiTList@PAVShipSubsystem@@@@IAEXPAV?$NiTListItem@PAVShipSubsystem@@@@@Z @game/OptimizedFireScript
template <class T> inline void NiTList<T>::DeleteItem( NiTListItem<T> *item ) {
	item->element = 0;
	item->next = freeList;
	freeList = item;
}

/*
================
NiTList::AddHead

Evidence: the TGObject load (0x6F0C40) inlines it.
================
*/
template <class T> inline void NiTList<T>::AddHead( T element ) {
	NiTListItem<T> *item;
	NewItem( item );
	item->element = element;
	item->prev = 0;
	item->next = head;
	if ( head ) {
		head->prev = item;
	} else {
		tail = item;
	}
	head = item;
	count++;
}

/*
================
NiTList::AddTail
================
*/
// FUNCTION: STBC 0x462950 ?AddTail@?$NiTList@PAVNiD3DDisplayMode@@@@QAEXPAVNiD3DDisplayMode@@@Z
// FUNCTION: STBC 0x475360 ?AddTail@?$NiTList@PAUAILogTiming@@@@QAEXPAUAILogTiming@@@Z @game/AILog
// FUNCTION: STBC 0x7C73A0 ?AddTail@?$NiTList@PAVNiDDImage@@@@QAEXPAVNiDDImage@@@Z
// FUNCTION: STBC 0x5B3A20 ?AddTail@?$NiTList@PAVPoweredSubsystem@@@@QAEXPAVPoweredSubsystem@@@Z @game/ShipClass
// FUNCTION: STBC 0x5699F0 ?AddTail@?$NiTList@J@@QAEXJ@Z @game/SensorSubsystem
// FUNCTION: STBC 0x6650A0 ?AddTail@?$NiTList@V?$NiPointer@VNiDynamicEffect@@@@@@QAEXV?$NiPointer@VNiDynamicEffect@@@@@Z @game/BridgeObjectClass
// FUNCTION: STBC 0x4175D0 ?AddTail@?$NiTList@V?$NiPointer@VNiAVObject@@@@@@QAEXV?$NiPointer@VNiAVObject@@@@@Z @game/SetClass
// FUNCTION: STBC 0x665300 ?AddTail@?$NiTList@V?$NiPointer@VNiTimeController@@@@@@QAEXV?$NiPointer@VNiTimeController@@@@@Z @game/BridgeObjectClass
// FUNCTION: STBC 0x446890 ?AddTail@?$NiTList@PAUPySetStateFunction@@@@QAEXPAUPySetStateFunction@@@Z @game/UtopiaModule
// FUNCTION: STBC 0x446830 ?AddTail@?$NiTList@PAUPyGetStateFunction@@@@QAEXPAUPyGetStateFunction@@@Z @game/UtopiaModule
// FUNCTION: STBC 0x55B760 ?AddTail@?$NiTList@H@@QAEXH@Z @game/KeyboardBinding
// FUNCTION: STBC 0x4E8D20 ?AddTail@?$NiTList@V?$NiPointer@VNiNode@@@@@@QAEXV?$NiPointer@VNiNode@@@@@Z @game/PulsingLight
template <class T> inline void NiTList<T>::AddTail( T element ) {
	NiTListItem<T> *item;
	NewItem( item );
	item->element = element;
	item->next = 0;
	item->prev = tail;
	if ( tail ) {
		tail->next = item;
	} else {
		head = item;
	}
	tail = item;
	count++;
}

/*
================
NiTList::RemoveHead

Unlinks the first item and returns its element. The list must not be
empty.

Evidence: NiDDImage's slot 13 (0x7C51A0) and NiDDBufferImage's slot 13
(0x7C70F0) inline it.

The if and else are unbraced as in Gamebryo's NiTPointerListBase: braces
would add four units to the inline cost, and ConditionalAI's destructor
(0x478B60) then has too little budget left to expand NiTMap::RemoveAll
after its four RemoveHead sites.
================
*/
// FUNCTION: STBC 0x417830 ?RemoveHead@?$NiTList@PAVShipClass@@@@QAEPAVShipClass@@XZ @game/SetClass
// FUNCTION: STBC 0x428740 ?RemoveHead@?$NiTList@PAUCameraLagSample@@@@QAEPAUCameraLagSample@@XZ @game/CameraMode
// FUNCTION: STBC 0x6C4560 ?RemoveHead@?$NiTList@PAVTGModelContainer@@@@QAEPAVTGModelContainer@@XZ @game/TGModelManager
// FUNCTION: STBC 0x46D6E0 ?RemoveHead@?$NiTList@PAUDistanceRange@@@@QAEPAUDistanceRange@@XZ @game/DistanceModifier
// FUNCTION: STBC 0x569D70 ?RemoveHead@?$NiTList@J@@QAEJXZ @game/SensorSubsystem
// FUNCTION: STBC 0x7C7AA0 ?RemoveHead@?$NiTList@PAVNiDDImage@@@@QAEPAVNiDDImage@@XZ
// FUNCTION: STBC 0x7C9590 ?RemoveHead@?$NiTList@PAVNiD3DDirectDraw@@@@QAEPAVNiD3DDirectDraw@@XZ
// FUNCTION: STBC 0x4629B0 ?RemoveHead@?$NiTList@PAVNiD3DDisplayMode@@@@QAEPAVNiD3DDisplayMode@@XZ
// FUNCTION: STBC 0x477D00 ?RemoveHead@?$NiTList@PAD@@QAEPADXZ @game/BuilderAI
// FUNCTION: STBC 0x78EE20 ?RemoveHead@?$NiTList@V?$NiPointer@VNiSource@@@@@@QAE?AV?$NiPointer@VNiSource@@@@XZ @netimmerse/NiSoundSystem
// FUNCTION: STBC 0x6A75F0 ?RemoveHead@?$NiTList@PAVTGMessage@@@@QAEPAVTGMessage@@XZ @game/NetFile
// FUNCTION: STBC 0x4E9720 ?RemoveHead@?$NiTList@V?$NiPointer@VNiNode@@@@@@QAE?AV?$NiPointer@VNiNode@@@@XZ @game/PulsingLight
// FUNCTION: STBC 0x486B20 ?RemoveHead@?$NiTList@PAVShipSubsystem@@@@QAEPAVShipSubsystem@@XZ @game/OptimizedFireScript
template <class T> inline T NiTList<T>::RemoveHead() {
	// NiTList.inl's "m_pHead" assert is left out: its 2 units a site leave
	// ConditionalAI::~ConditionalAI 1 unit short of NiTMap::RemoveAll, and
	// no spelling of that destructor is worth the unit
	NiTListItem<T> *item = head;
	head = head->next;
	if ( head )
		head->prev = 0;
	else
		tail = 0;
	T element = item->element;
	DeleteItem( item );
	count--;
	return element;
}

/*
================
NiTList::RemoveTail

Unlinks the last item and returns its element. The list must not be
empty.
================
*/
// FUNCTION: STBC 0x417870 ?RemoveTail@?$NiTList@PAVShipClass@@@@QAEPAVShipClass@@XZ @game/SetClass
// FUNCTION: STBC 0x429370 ?RemoveTail@?$NiTList@PAUCameraLagSample@@@@QAEPAUCameraLagSample@@XZ @game/CameraMode
// FUNCTION: STBC 0x72F250 ?RemoveTail@?$NiTList@PAVTGUIObject@@@@QAEPAVTGUIObject@@XZ @game/TGPane
// FUNCTION: STBC 0x6C4600 ?RemoveTail@?$NiTList@PAVTGModelContainer@@@@QAEPAVTGModelContainer@@XZ @game/TGModelManager
// FUNCTION: STBC 0x6EA860 ?RemoveTail@?$NiTList@PAVTGModelProperty@@@@QAEPAVTGModelProperty@@XZ
// FUNCTION: STBC 0x7C7AE0 ?RemoveTail@?$NiTList@PAVNiDDImage@@@@QAEPAVNiDDImage@@XZ
// FUNCTION: STBC 0x715390 ?RemoveTail@?$NiTList@PAUTGMusicFade@@@@QAEPAUTGMusicFade@@XZ @game/TGMusic
// FUNCTION: STBC 0x7C95D0 ?RemoveTail@?$NiTList@PAVNiD3DDirectDraw@@@@QAEPAVNiD3DDirectDraw@@XZ
// FUNCTION: STBC 0x47AAF0 ?RemoveTail@?$NiTList@PAU_object@@@@QAEPAU_object@@XZ @game/ConditionalAI
// FUNCTION: STBC 0x78EEF0 ?RemoveTail@?$NiTList@V?$NiPointer@VNiSource@@@@@@QAE?AV?$NiPointer@VNiSource@@@@XZ @netimmerse/NiSoundSystem
// FUNCTION: STBC 0x487EC0 ?RemoveTail@?$NiTList@PAUPowerShare@@@@QAEPAUPowerShare@@XZ @game/OptimizedManagePower
// FUNCTION: STBC 0x6A71B0 ?RemoveTail@?$NiTList@PAVTGMessage@@@@QAEPAVTGMessage@@XZ @game/NetFile
// FUNCTION: STBC 0x44DC40 ?RemoveTail@?$NiTList@J@@QAEJXZ @game/FocusManager
// FUNCTION: STBC 0x4E97F0 ?RemoveTail@?$NiTList@V?$NiPointer@VNiNode@@@@@@QAE?AV?$NiPointer@VNiNode@@@@XZ @game/PulsingLight
// FUNCTION: STBC 0x486B60 ?RemoveTail@?$NiTList@PAVShipSubsystem@@@@QAEPAVShipSubsystem@@XZ @game/OptimizedFireScript
template <class T> inline T NiTList<T>::RemoveTail() {
	assert( tail );
	NiTListItem<T> *item = tail;
	tail = tail->prev;
	if ( tail )
		tail->next = 0;
	else
		head = 0;
	T element = item->element;
	DeleteItem( item );
	count--;
	return element;
}

/*
================
NiTList::GetNext

Returns the element at pos and advances pos; NULL when pos is at the end.
T() rather than 0, as in Get: TGOverlayImage::Build (0x457680) walks its
list of runs, a struct, with it (the expanded copy keeps the run on a NULL
pos); the pointer instances compile alike either way.
================
*/
// FUNCTION: STBC 0x4629F0 ?GetNext@?$NiTList@PAVNiD3DDisplayMode@@@@QBEPAVNiD3DDisplayMode@@AAPAX@Z
// FUNCTION: STBC 0x428880 ?GetNext@?$NiTList@PAVCameraLagTracker@@@@QBEPAVCameraLagTracker@@AAPAX@Z @game/CameraMode
// FUNCTION: STBC 0x7C7400 ?GetNext@?$NiTList@PAVNiDDImage@@@@QBEPAVNiDDImage@@AAPAX@Z
// FUNCTION: STBC 0x5B3A80 ?GetNext@?$NiTList@PAVPoweredSubsystem@@@@QBEPAVPoweredSubsystem@@AAPAX@Z @game/ShipClass
// FUNCTION: STBC 0x5667A0 ?GetNext@?$NiTList@PAVShipSubsystem@@@@QBEPAVShipSubsystem@@AAPAX@Z @game/RepairSubsystem
// FUNCTION: STBC 0x475300 ?GetNext@?$NiTList@PAUAILogScope@@@@QBEPAUAILogScope@@AAPAX@Z @game/AILog
// FUNCTION: STBC 0x78EAD0 ?GetNext@?$NiTList@V?$NiPointer@VNiProvider_Info@@@@@@QBE?AV?$NiPointer@VNiProvider_Info@@@@AAPAX@Z @netimmerse/NiSoundSystem
// FUNCTION: STBC 0x78EA20 ?GetNext@?$NiTList@V?$NiPointer@VNiSource@@@@@@QBE?AV?$NiPointer@VNiSource@@@@AAPAX@Z @netimmerse/NiSoundSystem
// FUNCTION: STBC 0x665210 ?GetNext@?$NiTList@V?$NiPointer@VNiDynamicEffect@@@@@@QBE?AV?$NiPointer@VNiDynamicEffect@@@@AAPAX@Z @game/BridgeObjectClass
// FUNCTION: STBC 0x417740 ?GetNext@?$NiTList@V?$NiPointer@VNiAVObject@@@@@@QBE?AV?$NiPointer@VNiAVObject@@@@AAPAX@Z @game/SetClass
// FUNCTION: STBC 0x665490 ?GetNext@?$NiTList@V?$NiPointer@VNiTimeController@@@@@@QBE?AV?$NiPointer@VNiTimeController@@@@AAPAX@Z @game/BridgeObjectClass
// FUNCTION: STBC 0x6D6F80 ?GetNext@?$NiTList@PAVTGEvent@@@@QBEPAVTGEvent@@AAPAX@Z @game/TGEvent
// FUNCTION: STBC 0x6E7660 ?GetNext@?$NiTList@PAVTGUnicodeKeyList@@@@QBEPAVTGUnicodeKeyList@@AAPAX@Z @game/TGInputManager
// FUNCTION: STBC 0x596130 ?GetNext@?$NiTList@J@@QBEJAAPAX@Z @game/DamageableObject
// FUNCTION: STBC 0x55BC00 ?GetNext@?$NiTList@PAUKeyBind@@@@QBEPAUKeyBind@@AAPAX@Z @game/KeyboardBinding
// FUNCTION: STBC 0x55B7C0 ?GetNext@?$NiTList@H@@QBEHAAPAX@Z @game/KeyboardBinding
// FUNCTION: STBC 0x55BBA0 ?GetNext@?$NiTList@PAUBoundKey@@@@QBEPAUBoundKey@@AAPAX@Z @game/KeyboardBinding
// FUNCTION: STBC 0x4E8FE0 ?GetNext@?$NiTList@V?$NiPointer@VNiNode@@@@@@QBE?AV?$NiPointer@VNiNode@@@@AAPAX@Z @game/PulsingLight
// FUNCTION: STBC 0x7094A0 ?GetNext@?$NiTList@PAVTGPhonemeSequence@@@@QBEPAVTGPhonemeSequence@@AAPAX@Z @game/TGPhonemeSequence
template <class T> inline T NiTList<T>::GetNext( NiTListIterator &pos ) const {
	if ( !pos ) {
		return T();
	}
	T element = ( (NiTListItem<T> *)pos )->element;
	pos = ( (NiTListItem<T> *)pos )->next;
	return element;
}

/*
================
NiTList::GetPrev

Returns the element at pos and moves pos back; NULL when pos is at the
start. The tail-side twin of GetNext (Gamebryo lineage name).
================
*/
template <class T> inline T NiTList<T>::GetPrev( NiTListIterator &pos ) const {
	if ( !pos ) {
		return 0;
	}
	T element = ( (NiTListItem<T> *)pos )->element;
	pos = ( (NiTListItem<T> *)pos )->prev;
	return element;
}

/*
================
NiTList::InsertBefore

Links a new item holding element in front of the item at pos, which must
be a valid position, and returns the new item's position (Gamebryo lineage
name).

Evidence: TGPane's insertion of a child at an index (0x72E400) walks to
the position and links the new item before it in this order.
================
*/
template <class T> inline NiTListIterator NiTList<T>::InsertBefore( NiTListIterator pos, T element ) {
	NiTListItem<T> *next = (NiTListItem<T> *)pos;
	NiTListItem<T> *item;
	NewItem( item );
	item->element = element;
	item->next = next;
	item->prev = next->prev;
	if ( next->prev ) {
		next->prev->next = item;
	} else {
		head = item;
	}
	next->prev = item;
	count++;
	return item;
}

/*
================
NiTList::InsertAfter
================
*/
template <class T> inline NiTListIterator NiTList<T>::InsertAfter( NiTListIterator pos, T element ) {
	NiTListItem<T> *prev = (NiTListItem<T> *)pos;
	NiTListItem<T> *item;
	NewItem( item );
	item->element = element;
	item->prev = prev;
	item->next = prev->next;
	if ( prev->next ) {
		prev->next->prev = item;
	} else {
		tail = item;
	}
	prev->next = item;
	count++;
	return item;
}

/*
================
NiTList::FindPos

The position of the first item from start (the head by default) holding
element; NULL when there is none. The element is the left operand: retail
compares it first (0x7C7D90, 0x7D1B00). The out-of-line copy of
NiTList<long> (0x5AA2B0) tests start first (Gamebryo lineage).
================
*/
// FUNCTION: STBC 0x42E620 ?FindPos@?$NiTList@PAVCameraObjectClass@@@@QBEPAXPAVCameraObjectClass@@PAX@Z @game/CameraObjectClass
// FUNCTION: STBC 0x4E3BD0 ?FindPos@?$NiTList@PAVLensFlare@@@@QBEPAXPAVLensFlare@@PAX@Z @game/LensFlare
// FUNCTION: STBC 0x545470 ?FindPos@?$NiTList@PAURadarBracket@@@@QBEPAXPAURadarBracket@@PAX@Z @game/RadarScope
// FUNCTION: STBC 0x5AA2B0 ?FindPos@?$NiTList@J@@QBEPAXJPAX@Z @game/ProximityManager
// FUNCTION: STBC 0x55C1D0 ?FindPos@?$NiTList@PAUKeyBind@@@@QBEPAXPAUKeyBind@@PAX@Z @game/KeyboardBinding
// FUNCTION: STBC 0x4E95C0 ?FindPos@?$NiTList@V?$NiPointer@VNiNode@@@@@@QBEPAXV?$NiPointer@VNiNode@@@@PAX@Z @game/PulsingLight
template <class T> inline NiTListIterator NiTList<T>::FindPos( T element, NiTListIterator start ) const {
	if ( start == 0 ) {
		start = GetHeadPos();
	}
	while ( start ) {
		NiTListIterator pos = start;
		if ( element == GetNext( start ) ) {
			return pos;
		}
	}
	return 0;
}

/*
================
NiTList::RemovePos

Unlinks the item at pos, returns it to the free list and returns its
element; pos moves on to the next item.

Evidence: the out-of-line copy for TGEvent's lists (0x6D7100) advances
pos and returns the element, branching to RemoveHead and RemoveTail at the
ends. The TGObject fix-up pass (0x6F11B0) inlines RemoveHead's DeleteItem
on the head branch but calls DeleteItem out of line on the tail and middle
branches, so the head branch is a separate call site.
================
*/
// FUNCTION: STBC 0x4BB6C0 ?RemovePos@?$NiTList@PAVVisibleDamagePiece@@@@QAEPAVVisibleDamagePiece@@AAPAX@Z @game/VisibleDamageBuilder
// FUNCTION: STBC 0x4287C0 ?RemovePos@?$NiTList@PAVCameraLagTracker@@@@QAEPAVCameraLagTracker@@AAPAX@Z @game/CameraMode
// FUNCTION: STBC 0x7C7770 ?RemovePos@?$NiTList@PAVNiDDImage@@@@QAEPAVNiDDImage@@AAPAX@Z
// FUNCTION: STBC 0x7E33A0 ?RemovePos@?$NiTList@PAVNiRender@@@@QAEPAVNiRender@@AAPAX@Z @netimmerse/renderer/NiRender
// FUNCTION: STBC 0x46D720 ?RemovePos@?$NiTList@PAUDistanceRange@@@@QAEPAUDistanceRange@@AAPAX@Z @game/DistanceModifier
// FUNCTION: STBC 0x5453B0 ?RemovePos@?$NiTList@PAURadarBracket@@@@QAEPAURadarBracket@@AAPAX@Z @game/RadarScope
// FUNCTION: STBC 0x5A7130 ?RemovePos@?$NiTList@PAVProximityCheck@@@@QAEPAVProximityCheck@@AAPAX@Z @game/ProximityCheck
// FUNCTION: STBC 0x78EC80 ?RemovePos@?$NiTList@V?$NiPointer@VNiSource@@@@@@QAE?AV?$NiPointer@VNiSource@@@@AAPAX@Z @netimmerse/NiSoundSystem
// FUNCTION: STBC 0x42E560 ?RemovePos@?$NiTList@PAVCameraObjectClass@@@@QAEPAVCameraObjectClass@@AAPAX@Z @game/CameraObjectClass
// FUNCTION: STBC 0x42E350 ?RemovePos@?$NiTList@PAVCameraMode@@@@QAEPAVCameraMode@@AAPAX@Z @game/CameraObjectClass
// FUNCTION: STBC 0x5666E0 ?RemovePos@?$NiTList@PAVShipSubsystem@@@@QAEPAVShipSubsystem@@AAPAX@Z @game/ShipClass
// FUNCTION: STBC 0x6D7100 ?RemovePos@?$NiTList@PAVTGEvent@@@@QAEPAVTGEvent@@AAPAX@Z @game/TGEvent
// FUNCTION: STBC 0x7154D0 ?RemovePos@?$NiTList@J@@QAEJAAPAX@Z @game/TGMusic
// FUNCTION: STBC 0x4E3B10 ?RemovePos@?$NiTList@PAVLensFlare@@@@QAEPAVLensFlare@@AAPAX@Z @game/LensFlare
// FUNCTION: STBC 0x6E7AA0 ?RemovePos@?$NiTList@PAVTGUnicodeKeyList@@@@QAEPAVTGUnicodeKeyList@@AAPAX@Z @game/TGInputManager
// FUNCTION: STBC 0x5AA410 ?RemovePos@?$NiTList@H@@QAEHAAPAX@Z @game/ProximityManager
// FUNCTION: STBC 0x55C0F0 ?RemovePos@?$NiTList@PAUKeyBind@@@@QAEPAUKeyBind@@AAPAX@Z @game/KeyboardBinding
// FUNCTION: STBC 0x4E9470 ?RemovePos@?$NiTList@V?$NiPointer@VNiNode@@@@@@QAE?AV?$NiPointer@VNiNode@@@@AAPAX@Z @game/PulsingLight
// FUNCTION: STBC 0x7094C0 ?RemovePos@?$NiTList@PAVTGPhonemeSequence@@@@QAEPAVTGPhonemeSequence@@AAPAX@Z @game/TGPhonemeSequence
template <class T> inline T NiTList<T>::RemovePos( NiTListIterator &pos ) {
	NiTListItem<T> *item = (NiTListItem<T> *)pos;
	// NiTList.inl's "pNode" assert is left out: its 2 units take
	// RemovePos 2 units past its share in
	// DamageableObject::EnableCollisionsWith, and no spelling of that
	// function is worth the 6 units of budget it would need
	if ( item == head ) {
		pos = item->next;
		return RemoveHead();
	}
	if ( item == tail ) {
		pos = 0;
		return RemoveTail();
	}
	NiTListItem<T> *prev = item->prev;
	NiTListItem<T> *next = item->next;
	pos = next;
	if ( prev ) {
		prev->next = next;
	}
	if ( next ) {
		next->prev = prev;
	}
	T element = item->element;
	DeleteItem( item );
	count--;
	return element;
}

/*
================
NiTList::Remove

Removes the first item holding element, if any, and returns the element.

Evidence: NiD3DDirectDraw's destructor (0x7C7D90) and
NiD3DLightManager::RemoveLight search and remove through it.
================
*/
// FUNCTION: STBC 0x78E8D0 ?Remove@?$NiTList@V?$NiPointer@VNiSource@@@@@@QAE?AV?$NiPointer@VNiSource@@@@V2@@Z @netimmerse/NiSoundSystem
// PARKED: inline budget: retail calls NiPointer's copy constructor (0x78EC60) out of line for FindPos's argument and releases each GetNext temporary inline through DecRefCount (0x40CFE0); this build does the reverse
// FUNCTION: STBC 0x4E8EB0 ?Remove@?$NiTList@V?$NiPointer@VNiNode@@@@@@QAE?AV?$NiPointer@VNiNode@@@@V2@@Z @game/PulsingLight
template <class T> inline T NiTList<T>::Remove( T element ) {
	NiTListIterator pos = FindPos( element );
	return pos ? RemovePos( pos ) : element;
}

#endif /* !__NITLIST_H__ */
