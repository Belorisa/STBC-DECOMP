/*
===============================================================================

	TGModule.h

	A part of the program that takes part in its start, its frames and its
	end: every module joins one list, ordered by a priority given at
	construction (larger first), and the static calls run one of the
	module slots over the whole list, forward or backward. A module is a
	static object: it joins the list in its constructor and leaves it in
	its destructor. UtopiaModule and InterfaceModule (priority
	0x20000000), the TG interface's module (0x80000000) and the actions
	module (-2) are modules; eighteen constructors call the base's.

	0x14 bytes: InterfaceModule's own members start there.

	Evidence: the constructor (0x71E2B0) stores the vtable 0x896B6C (8
	slots; 0 and 5 are _purecall), clears the links and keeps the priority,
	then calls AddModule (0x71E2F0, which inserts it before the first
	module of a lower priority, compared unsigned, in the list 0x99C8A4 to
	0x99C8A8); the destructor (0x71E2E0, not virtual) calls RemoveModule
	(0x71E360). ForEach (0x71E3B0) and ForEachReverse (0x71E3D0) call a
	pointer to a member function on every module; 0x71E3F0 to 0x71E4C0
	pass them the vcall thunks of slots 0 to 7. Slots 1 to 4, 6 and 7 are
	empty inline functions in the base (0x4863E0, 0x4863F0, 0x486400,
	0x43AD60, 0x486410, 0x486420); slot 7 takes one argument.

	No string names the class or its slots: every name here is invented,
	and the slots keep placeholder names. The actions module fills slot 0
	with its setup, slot 2 with its per-frame update and slot 5 with
	deleting the actions.

===============================================================================
*/

#ifndef __TGMODULE_H__
#define __TGMODULE_H__

class TGModule;

// A module slot, called on every module by ForEach and ForEachReverse.
typedef void ( TGModule::*moduleFunction_t )( void );
typedef void ( TGModule::*moduleFunctionWithArgument_t )( int argument );

// The compiler's vcall thunks for the slot pointers ForEach and its
// siblings take (one per slot, by its vtable offset).
// FUNCTION: STBC 0x0071e510 ??_9@$BBM@AE @game/TGModule
// FUNCTION: STBC 0x0071e520 ??_9@$BA@AE @game/TGModule
// FUNCTION: STBC 0x0071e530 ??_9@$B3AE @game/TGModule
// FUNCTION: STBC 0x0071e540 ??_9@$B7AE @game/TGModule
// FUNCTION: STBC 0x0071e550 ??_9@$BM@AE @game/TGModule
// FUNCTION: STBC 0x0071e560 ??_9@$BBA@AE @game/TGModule
// FUNCTION: STBC 0x0071e570 ??_9@$BBE@AE @game/TGModule
// FUNCTION: STBC 0x0071e580 ??_9@$BBI@AE @game/TGModule
class TGModule {
public:
					TGModule( unsigned int priority_ );
					~TGModule( void );

	virtual void	VirtualSlot00( void ) = 0;
	// FUNCTION: STBC 0x4863E0 ?VirtualSlot01@TGModule@@UAEXXZ
	virtual void	VirtualSlot01( void ) {}
	// FUNCTION: STBC 0x4863F0 ?VirtualSlot02@TGModule@@UAEXXZ
	virtual void	VirtualSlot02( void ) {}
	// FUNCTION: STBC 0x486400 ?VirtualSlot03@TGModule@@UAEXXZ
	virtual void	VirtualSlot03( void ) {}
	// FUNCTION: STBC 0x43AD60 ?VirtualSlot04@TGModule@@UAEXXZ
	virtual void	VirtualSlot04( void ) {}
	virtual void	VirtualSlot05( void ) = 0;
	// FUNCTION: STBC 0x486410 ?VirtualSlot06@TGModule@@UAEXXZ
	virtual void	VirtualSlot06( void ) {}
	// FUNCTION: STBC 0x486420 ?VirtualSlot07@TGModule@@UAEXH@Z
	virtual void	VirtualSlot07( int ) {}

	// Inserts the module before the first one of a lower priority; a
	// module already linked is left alone.
	static void		AddModule( TGModule *module );
	static void		RemoveModule( TGModule *module );

	// Calls the function on every module, from the highest priority down,
	// or the other way.
	static void		ForEach( moduleFunction_t function );
	static void		ForEachReverse( moduleFunction_t function );
	static void		ForEachWithArgument( moduleFunctionWithArgument_t function, int argument );
	static void		ForEachReverseWithArgument( moduleFunctionWithArgument_t function, int argument );

	// Each slot over every module, in the order the program uses.
	static void		CallVirtualSlot00( void );			// forward
	static void		CallVirtualSlot05( void );			// backward
	static void		CallVirtualSlot01( void );			// forward
	static void		CallVirtualSlot02( void );			// forward
	static void		CallVirtualSlots03And04( void );	// backward, slot 3 then slot 4
	static void		CallVirtualSlot06( void );			// backward
	static void		CallVirtualSlot07( int argument );	// backward

protected:
	int				unknown04;			// 0x04 not set by the base
	TGModule *		next;				// 0x08 the next module, of the same or a lower priority
	TGModule *		prev;				// 0x0C
	unsigned int	priority;			// 0x10

	static TGModule *	first;			// 0x99C8A4 the highest priority
	static TGModule *	last;			// 0x99C8A8
};

#endif /* !__TGMODULE_H__ */
