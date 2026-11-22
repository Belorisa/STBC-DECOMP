/*
===============================================================================

	NiRTTI.h

	NetImmerse's own run-time type information. Every NetImmerse class owns
	one static NiRTTI that records its name and its base class's NiRTTI; the
	engine is built without compiler RTTI (/GR).

	Evidence: each class's static initializer passes the class name string and
	the base class's NiRTTI to the constructor at 0x818F20, for example
	NiNode's (0x7E3630) passes "NiNode" and NiAVObject's NiRTTI.

===============================================================================
*/

#ifndef __NIRTTI_H__
#define __NIRTTI_H__

class NiRTTI {
public:
					NiRTTI( const char *name, const NiRTTI *baseRTTI );
					~NiRTTI();

	const char *	GetName() const { return name; }
	const NiRTTI *	GetBaseRTTI() const { return baseRTTI; }

protected:
	char *			name;		// a copy of the class name, owned
	const NiRTTI *	baseRTTI;	// NULL for NiObject
};

#endif /* !__NIRTTI_H__ */
