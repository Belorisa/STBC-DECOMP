/*
===============================================================================

	AIScriptAssist.h

	Tests the AI scripts call that are too slow to write in Python.

	Evidence: the SWIG wrapper AIScriptAssist_TorpIsIncoming (0x6063D0)
	calls 0x473960 with an object, a torpedo, a float, a long and a bool
	("OOfli"); the App function AIScriptAssist_GetIncomingTorpIDsInSet
	(0x473830, written by hand in this unit) runs the same test over a
	set's torpedoes. The class has no members: SWIG's classmap lists only
	its static method.

===============================================================================
*/

#ifndef __AISCRIPTASSIST_H__
#define __AISCRIPTASSIST_H__

#include "Python.h"

class ObjectClass;
class Torpedo;

class AIScriptAssist {
public:
	// Whether the torpedo homes on the object, is moving toward it and
	// reaches it within the time; with checkParent, only a torpedo the
	// parent fired counts. The parameter names are invented.
	static bool		TorpIsIncoming( ObjectClass *object, Torpedo *torpedo, float time, long parentID, bool checkParent );
};

// The App function: a list of the IDs of the set's torpedoes that pass
// TorpIsIncoming for the object.
extern "C" PyObject *	_wrap_AIScriptAssist_GetIncomingTorpIDsInSet( PyObject *self, PyObject *args );

#endif /* !__AISCRIPTASSIST_H__ */
