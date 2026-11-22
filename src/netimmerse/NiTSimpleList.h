/*
===============================================================================

	NiTSimpleList.h

	NetImmerse's singly linked list whose first node is embedded in its
	owner: an NiAVObject's properties, an NiNode's dynamic effects and an
	NiDynamicEffect's affected nodes. The embedded node holds the newest
	item; AddHead moves its old item into a new heap node behind it.

	Names: NiTSimpleList and its members AddHead, Remove, GetItem,
	ItemsInList and IsInList are DamageTool.exe's (a release build of the
	same NetImmerse with a COFF symbol table: NiTSimpleList<NiPointer<
	NiProperty> >::AddHead 0x402AD0, Remove 0x438770, GetItem 0x433590,
	ItemsInList 0x438750, the constructor 0x402BC0 and the destructor
	0x438AF0; NiTSimpleList<NiDynamicEffect *>::IsInList 0x433BD0, AddHead
	0x433C10, Remove 0x433C60; NiTSimpleList<NiNode *>::AddHead 0x430230).
	DamageTool's Remove of the property list is byte-identical to retail's
	0x498250. The list typedefs (NiPropertyList, NiDynamicEffectList,
	NiNodeList) are the Gamebryo lineage's. The other member names are
	invented.

	Evidence: a list node is 8 bytes, the item and the next node; the
	constructor takes the item by value and clears the link; the count
	(0x7DDD90, 0x7E2820, 0x7E6360) recurses through the links.

===============================================================================
*/

#ifndef __NITSIMPLELIST_H__
#define __NITSIMPLELIST_H__

template <class T> class NiTSimpleList {
public:
							// FUNCTION: STBC 0x416EE0 ??0?$NiTSimpleList@V?$NiPointer@VNiProperty@@@@@@QAE@V?$NiPointer@VNiProperty@@@@@Z @game/CharacterClass
							NiTSimpleList( T item_ = 0 ) { item = item_; next = 0; }

	bool					IsEmpty() const { return !next && item == T( 0 ); }
	// FUNCTION: STBC 0x4E11F0 ?GetItem@?$NiTSimpleList@V?$NiPointer@VNiProperty@@@@@@QBE?AV?$NiPointer@VNiProperty@@@@XZ @netimmerse/NiAVObject
	T						GetItem() const { return item; }
	// the SDK's debug NetImmerse 3.1 walks the lists through GetItem and
	// GetNext (AnimOnly.exe 0x418690 and 0x41FA70; 0x412C20, 0x4186A0 and
	// 0x41FA80); the name GetNext is invented
	NiTSimpleList *			GetNext() const { return next; }
	unsigned int			ItemsInList() const;
	bool					IsInList( T item_ ) const;

	void					AddHead( T item_ );
	void					Remove( T item_ );
	void					RemoveHead();
	void					RemoveAll();
	// the embedded node takes the item; NULL is ignored (name invented)
	// FUNCTION: STBC 0x4E1160 ?SetItem@?$NiTSimpleList@V?$NiPointer@VNiProperty@@@@@@QAEXV?$NiPointer@VNiProperty@@@@@Z @game/Glow
	void					SetItem( T item_ ) { if ( !item_ ) { return; } item = item_; }

	T						item;
	NiTSimpleList *			next;
};

/*
================
NiTSimpleList::ItemsInList

The number of items in the list.
================
*/
// FUNCTION: STBC 0x7DDD90 ?ItemsInList@?$NiTSimpleList@V?$NiPointer@VNiProperty@@@@@@QBEIXZ
// FUNCTION: STBC 0x7E2820 ?ItemsInList@?$NiTSimpleList@PAVNiNode@@@@QBEIXZ
// FUNCTION: STBC 0x7E6360 ?ItemsInList@?$NiTSimpleList@PAVNiDynamicEffect@@@@QBEIXZ
template <class T> inline unsigned int NiTSimpleList<T>::ItemsInList() const {
	unsigned int count = item ? 1 : 0;
	if ( next ) {
		count += next->ItemsInList();
	}
	return count;
}

/*
================
NiTSimpleList::IsInList

True when the list holds item_.
================
*/
// FUNCTION: STBC 0x7E6340 ?IsInList@?$NiTSimpleList@PAVNiDynamicEffect@@@@QBE_NPAVNiDynamicEffect@@@Z @netimmerse/NiNode
template <class T> inline bool NiTSimpleList<T>::IsInList( T item_ ) const {
	const NiTSimpleList *list = this;
	while ( list ) {
		if ( list->item == item_ ) {
			break;
		}
		list = list->next;
	}
	return list != 0;
}

/*
================
NiTSimpleList::AddHead

The embedded node takes the new item; its old item moves into a new heap
node behind it. NULL is ignored.
================
*/
// FUNCTION: STBC 0x416DE0 ?AddHead@?$NiTSimpleList@V?$NiPointer@VNiProperty@@@@@@QAEXV?$NiPointer@VNiProperty@@@@@Z @game/CharacterClass
// FUNCTION: STBC 0x7E2840 ?AddHead@?$NiTSimpleList@PAVNiNode@@@@QAEXPAVNiNode@@@Z @netimmerse/NiDynamicEffect
// FUNCTION: STBC 0x7E6380 ?AddHead@?$NiTSimpleList@PAVNiDynamicEffect@@@@QAEXPAVNiDynamicEffect@@@Z @netimmerse/NiNode
// FUNCTION: STBC 0x7D78E0 ?AddHead@?$NiTSimpleList@PAVNiTextureManager@@@@QAEXPAVNiTextureManager@@@Z @netimmerse/renderer/NiTextureManager
// FUNCTION: STBC 0x8223F0 ?AddHead@?$NiTSimpleList@PAD@@QAEXPAD@Z @netimmerse/NiSortedObject
template <class T> inline void NiTSimpleList<T>::AddHead( T item_ ) {
	if ( item_ ) {
		if ( item ) {
			NiTSimpleList *list = new NiTSimpleList( item );
			list->next = next;
			next = list;
		}
		item = item_;
	}
}

/*
================
NiTSimpleList::Remove

Unlinks the item; when it is in the embedded node, the second node moves
into it.
================
*/
// FUNCTION: STBC 0x498250 ?Remove@?$NiTSimpleList@V?$NiPointer@VNiProperty@@@@@@QAEXV?$NiPointer@VNiProperty@@@@@Z @game/Editor
// FUNCTION: STBC 0x7E63D0 ?Remove@?$NiTSimpleList@PAVNiDynamicEffect@@@@QAEXPAVNiDynamicEffect@@@Z @netimmerse/NiNode
template <class T> inline void NiTSimpleList<T>::Remove( T item_ ) {
	if ( !item_ || IsEmpty() ) {
		return;
	}

	NiTSimpleList *list = this;
	NiTSimpleList *prev = this;
	while ( list ) {
		if ( list->item == item_ ) {
			break;
		}
		prev = list;
		list = list->next;
	}
	if ( list ) {
		if ( list == this ) {
			if ( next ) {
				NiTSimpleList *second = next;
				next = second->next;
				item = second->item;
				delete second;
			} else {
				item = 0;
			}
		} else {
			prev->next = list->next;
			delete list;
		}
	}
}

/*
================
NiTSimpleList::RemoveHead

Drops the first item: the second node moves into the embedded one, or the
embedded node is cleared when it is the only one (Gamebryo lineage name,
NiTPointerList::RemoveHead).
================
*/
template <class T> inline void NiTSimpleList<T>::RemoveHead() {
	if ( next ) {
		NiTSimpleList *second = next;
		next = second->next;
		item = second->item;
		delete second;
	} else {
		item = 0;
	}
}

/*
================
NiTSimpleList::RemoveAll

Frees the heap nodes, then clears the embedded node's item.
================
*/
template <class T> inline void NiTSimpleList<T>::RemoveAll() {
	while ( next ) {
		NiTSimpleList *nextNode = next->next;
		delete next;
		next = nextNode;
	}
	item = 0;
}

#endif /* !__NITSIMPLELIST_H__ */
