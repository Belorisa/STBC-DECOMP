/*
===============================================================================

	TGQueue.h

	A first-in first-out queue in a circular array that doubles when it is
	full. The name is invented.

	Evidence: TGRootPane keeps its pushed cursors in one (0x4C): the
	constructor allocates one entry and sets the head to -1 and the tail to
	0; PushCursor stores at the tail, doubling the array when the tail has
	caught up with the head; PopCursor takes from the head and empties the
	queue (head -1, tail 0) when the head reaches the tail. The array copy
	that doubles it is the out-of-line 0x728F60, and the destructor that
	frees the array the out-of-line 0x728E90 (the unwind of a local queue).

===============================================================================
*/

#ifndef __TGQUEUE_H__
#define __TGQUEUE_H__

// The head of an empty queue.
#define QUEUE_EMPTY				( (unsigned int)-1 )

template <class T> class TGQueue {			// invented name
public:
					TGQueue( void ) { data = new T[1]; size = 1; head = QUEUE_EMPTY; tail = 0; }
	// FUNCTION: STBC 0x00728e90 ??1?$TGQueue@PAVTGIcon@@@@QAE@XZ
					~TGQueue( void ) { delete [] data; }

	bool			IsEmpty( void ) { return head == QUEUE_EMPTY; }
	void			Push( T item );
	T				Pop( void );
	void			Resize( unsigned int newSize );

	T *				data;					// 0x00 size entries, from head to tail around the end
	unsigned int	size;					// 0x04
	unsigned int	head;					// 0x08 the oldest entry; QUEUE_EMPTY when empty
	unsigned int	tail;					// 0x0C where the next entry goes
};

/*
================
TGQueue::Push
================
*/
template <class T> inline void TGQueue<T>::Push( T item ) {
	if ( head == QUEUE_EMPTY ) {
		head = 0;
		data[0] = item;
		tail = 1;
	} else {
		if ( head == tail ) {
			Resize( size * 2 );
		}
		data[tail] = item;
		tail++;
	}
	if ( tail == size ) {
		tail = 0;
	}
}

/*
================
TGQueue::Pop
================
*/
template <class T> inline T TGQueue<T>::Pop( void ) {
	T item = data[head];
	head++;
	if ( head == size ) {
		head = 0;
	}
	if ( head == tail ) {
		head = QUEUE_EMPTY;
		tail = 0;
	}
	return item;
}

/*
================
TGQueue::Resize

Copies the entries, oldest first, to the start of a new array.
================
*/
// FUNCTION: STBC 0x00728f60 ?Resize@?$TGQueue@PAVTGIcon@@@@QAEXI@Z
template <class T> inline void TGQueue<T>::Resize( unsigned int newSize ) {
	T *newData = new T[newSize];
	unsigned int count = 0;
	unsigned int i;

	if ( head >= tail ) {
		for ( i = head; i < size; i++ ) {
			newData[count++] = data[i];
		}
		for ( i = 0; i < tail; i++ ) {
			newData[count++] = data[i];
		}
	} else {
		for ( i = head; i < tail; i++ ) {
			newData[count++] = data[i];
		}
	}
	head = 0;
	tail = count;
	size = newSize;
	delete [] data;
	data = newData;
}

#endif /* !__TGQUEUE_H__ */
