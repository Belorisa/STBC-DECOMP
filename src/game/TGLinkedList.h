/*
===============================================================================

	TGLinkedList.h

	A singly linked list of pointers that owns its elements, with a cursor
	for walking it and a cached position. The name is invented; the
	network's own instance is the SWIG type "TGNetworkListType" (the list
	of every TGNetwork, TGNetwork_GetTGNetworkList), and TGNetwork keeps its
	message queues in five more.

	Evidence: TGNetwork's constructors expand the constructor five times
	(head, tail, count, cached node, cached index -1, cursor), the destructor
	the same walk that deletes each element and frees each node; the
	out-of-line copies are the destructor of the message list 0x6B3400 (the
	unwinds of TGNetwork's constructors) and RemoveAt for messages 0x6B78D0
	and for networks 0x6B7BD0. No code in the network unit sets the cached
	node; removals only clear it.

===============================================================================
*/

#ifndef __TGLINKEDLIST_H__
#define __TGLINKEDLIST_H__

#include <stddef.h>

template <class T>
class TGLinkedList {
public:
	struct Node {
		T *			data;		// 0x00
		Node *		next;		// 0x04
	};

					TGLinkedList( void ) {
						head = NULL;
						tail = NULL;
						count = 0;
						cachedNode = NULL;
						cachedIndex = -1;
						cursor = NULL;
					}
					// FUNCTION: STBC 0x006b3400 ??1?$TGLinkedList@VTGMessage@@@@QAE@XZ @game/TGNetwork
					~TGLinkedList( void ) { DeleteAll(); }

	// Deletes every element and empties the list (invented name).
	void			DeleteAll( void ) {
						Node *node = head;
						while ( node != NULL ) {
							Node *next = node->next;
							delete node->data;
							delete node;
							node = next;
						}
						head = NULL;
						tail = NULL;
						count = 0;
						cachedNode = NULL;
						cachedIndex = -1;
						cursor = NULL;
					}

	int				GetCount( void ) { return count; }

	// The index of the element; -1 when it is not in the list.
	int				Find( T *item ) {
						int index = 0;
						for ( Node *node = head; node != NULL; node = node->next ) {
							if ( node->data == item ) {
								return index;
							}
							index++;
						}
						return -1;
					}

	// Takes the element out of the list, if it is there; it is not deleted.
	void			Remove( T *item ) {
						int index = Find( item );
						if ( index >= 0 ) {
							RemoveAt( index );
						}
					}

	// Appends the element.
	void			AddTail( T *item ) {
						Node *node = new Node;
						if ( node != NULL ) {
							node->data = item;
							node->next = NULL;
							if ( tail == NULL ) {
								tail = node;
								head = node;
							} else {
								tail->next = node;
								tail = node;
							}
							count++;
						}
					}

	// Puts the element in front.
	void			AddHead( T *item ) {
						Node *node = new Node;
						if ( node != NULL ) {
							node->data = item;
							node->next = NULL;
							if ( tail == NULL ) {
								tail = node;
								head = node;
							} else {
								node->next = head;
								head = node;
							}
							count++;
						}
					}

	// Puts the element after the one before the index (invented name).
	// BUG: with index 0 the element goes after the head, not in front of it.
	void			InsertAt( T *item, int index ) {
						Node *node = new Node;
						if ( node != NULL ) {
							index--;
							node->data = item;
							node->next = NULL;
							Node *previous = head;
							for ( int i = 0; i < index; i++ ) {
								previous = previous->next;
							}
							node->next = previous->next;
							previous->next = node;
							count++;
						}
					}

	// Starts a walk: the first element, or NULL.
	T *				GetFirst( void ) {
						cursorIndex = 0;
						if ( head == NULL ) {
							return NULL;
						}
						cursor = head->next;
						return head->data;
					}

	// The next element of the walk, or NULL at the end.
	T *				GetNext( void ) {
						if ( cursor == NULL ) {
							return NULL;
						}
						T *item = cursor->data;
						cursor = cursor->next;
						cursorIndex++;
						return item;
					}

	// The index of the element the walk is on (invented name).
	int				GetCursorIndex( void ) { return cursorIndex; }

	T *				RemoveAt( int index );

	// Takes the element at the index out of the list and deletes it
	// (invented name).
	void			DeleteAt( int index ) { delete RemoveAt( index ); }

	// Empties the list; the elements are not deleted (invented name).
	void			RemoveAll( void ) {
						Node *node = head;
						while ( node != NULL ) {
							Node *next = node->next;
							delete node;
							node = next;
						}
						head = NULL;
						tail = NULL;
						count = 0;
						cachedNode = NULL;
						cursor = NULL;
						cachedIndex = -1;
					}

protected:
	Node *			head;			// 0x00
	Node *			tail;			// 0x04
	Node *			cachedNode;		// 0x08 cleared when its node is removed
	Node *			cursor;			// 0x0C the next node of the walk
	int				cursorIndex;	// 0x10 the index of the element GetNext returned last
	int				cachedIndex;	// 0x14 the index of cachedNode; -1 without one
	int				count;			// 0x18
};

/*
================
TGLinkedList::RemoveAt

Takes the element at the index out of the list and returns it; NULL when
the index is out of range.
================
*/
// FUNCTION: STBC 0x006b78d0 ?RemoveAt@?$TGLinkedList@VTGMessage@@@@QAEPAVTGMessage@@H@Z @game/TGNetwork
// FUNCTION: STBC 0x006b7bd0 ?RemoveAt@?$TGLinkedList@VTGNetwork@@@@QAEPAVTGNetwork@@H@Z @game/TGNetwork
template <class T> inline T *TGLinkedList<T>::RemoveAt( int index ) {
	if ( index >= count || index < 0 ) {
		return NULL;
	}
	Node *previous = NULL;
	Node *node = head;
	for ( int i = 0; i < index; i++ ) {
		previous = node;
		node = node->next;
	}
	if ( previous == NULL ) {
		if ( head == tail ) {
			head = node->next;
			tail = head;
		} else {
			head = node->next;
		}
	} else {
		previous->next = node->next;
		if ( node->next == NULL ) {
			tail = previous;
		}
	}
	if ( node == cachedNode ) {
		cachedNode = NULL;
		cachedIndex = -1;
	}
	if ( node == cursor ) {
		cursor = node->next;
	}
	if ( index <= cursorIndex ) {
		cursorIndex--;
	}
	T *item = node->data;
	delete node;
	count--;
	return item;
}

#endif /* !__TGLINKEDLIST_H__ */
