/*
===============================================================================

	FuzzyLogic.cpp

===============================================================================
*/

#include "FuzzyLogic.h"
#include "AppConstants.h"
#include "Python.h"

#include <string.h>
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0047d280 _$E177
// FUNCTION: STBC 0x0047d290 _$E176
// FUNCTION: STBC 0x0047d320 _$E222
// FUNCTION: STBC 0x0047d330 _$E221
// FUNCTION: STBC 0x0047d340 _$E226
// FUNCTION: STBC 0x0047d350 _$E225
// FUNCTION: STBC 0x0047d440 _$E230
// FUNCTION: STBC 0x0047d450 _$E229
// FUNCTION: STBC 0x0047d460 _$E234
// FUNCTION: STBC 0x0047d470 _$E233
// FUNCTION: STBC 0x0047d480 _$E246
// FUNCTION: STBC 0x0047d490 _$E245
// FUNCTION: STBC 0x0047d4a0 _$E250
// FUNCTION: STBC 0x0047d4b0 _$E249
// FUNCTION: STBC 0x0047d4f0 _$E254
// FUNCTION: STBC 0x0047d500 _$E253
// FUNCTION: STBC 0x0047d510 _$E258
// FUNCTION: STBC 0x0047d520 _$E257
// FUNCTION: STBC 0x0047d530 _$E262
// FUNCTION: STBC 0x0047d540 _$E261
// FUNCTION: STBC 0x0047d550 _$E266
// FUNCTION: STBC 0x0047d560 _$E265
// FUNCTION: STBC 0x0047d570 _$E270
// FUNCTION: STBC 0x0047d580 _$E269
// FUNCTION: STBC 0x0047d590 _$E274
// FUNCTION: STBC 0x0047d5a0 _$E273
// FUNCTION: STBC 0x0047d5d0 _$E278
// FUNCTION: STBC 0x0047d5e0 _$E277
// FUNCTION: STBC 0x0047d610 _$E292
// FUNCTION: STBC 0x0047d620 _$E291
// FUNCTION: STBC 0x0047d630 _$E296
// FUNCTION: STBC 0x0047d640 _$E295
// FUNCTION: STBC 0x0047d650 _$E300
// FUNCTION: STBC 0x0047d660 _$E299
// FUNCTION: STBC 0x0047d670 _$E304
// FUNCTION: STBC 0x0047d680 _$E303
// FUNCTION: STBC 0x0047d6b0 _$E308
// FUNCTION: STBC 0x0047d6c0 _$E307
// FUNCTION: STBC 0x0047d6d0 _$E312
// FUNCTION: STBC 0x0047d6e0 _$E311
// FUNCTION: STBC 0x0047d6f0 _$E316
// FUNCTION: STBC 0x0047d700 _$E315
// FUNCTION: STBC 0x0047d770 _$E320
// FUNCTION: STBC 0x0047d780 _$E319
// FUNCTION: STBC 0x0047d7b0 _$E324
// FUNCTION: STBC 0x0047d7c0 _$E323
// FUNCTION: STBC 0x0047d7d0 _$E328
// FUNCTION: STBC 0x0047d7e0 _$E327
// FUNCTION: STBC 0x0047d810 _$E332
// FUNCTION: STBC 0x0047d820 _$E331
// FUNCTION: STBC 0x0047d850 _$E336
// FUNCTION: STBC 0x0047d860 _$E335
// FUNCTION: STBC 0x0047d870 _$E340
// FUNCTION: STBC 0x0047d880 _$E339
// FUNCTION: STBC 0x0047d8d0 _$E344
// FUNCTION: STBC 0x0047d8e0 _$E343
// FUNCTION: STBC 0x0047d8f0 _$E348
// FUNCTION: STBC 0x0047d900 _$E347
// FUNCTION: STBC 0x0047d910 _$E352
// FUNCTION: STBC 0x0047d920 _$E351
// FUNCTION: STBC 0x0047d930 _$E356
// FUNCTION: STBC 0x0047d940 _$E355
// FUNCTION: STBC 0x0047d950 _$E360
// FUNCTION: STBC 0x0047d960 _$E359
// FUNCTION: STBC 0x0047d970 _$E364
// FUNCTION: STBC 0x0047d980 _$E363
// FUNCTION: STBC 0x0047d990 _$E368
// FUNCTION: STBC 0x0047d9a0 _$E367
// FUNCTION: STBC 0x0047d9b0 _$E372
// FUNCTION: STBC 0x0047d9c0 _$E371
// FUNCTION: STBC 0x0047d9f0 _$E376
// FUNCTION: STBC 0x0047da00 _$E375

/*
================
FuzzyLogic::FuzzyLogic
================
*/
// FUNCTION: STBC 0x0047cd10
FuzzyLogic::FuzzyLogic( void ) {
	maxRules = 0;
	numRules = 0;
	rules = NULL;
}

/*
================
FuzzyLogic::~FuzzyLogic
================
*/
// FUNCTION: STBC 0x0047cd20
FuzzyLogic::~FuzzyLogic( void ) {
	if ( rules != NULL ) {
		delete [] rules;
	}
}

/*
================
FuzzyLogic::SetMaxRules

Makes room for the rules.
================
*/
// FUNCTION: STBC 0x0047cd30
void FuzzyLogic::SetMaxRules( int maxRules_ ) {
	maxRules = maxRules_;
	rules = new FuzzyRule[maxRules_];
}

/*
================
FuzzyLogic::AddRule

Appends a rule, and returns -1 when there is no room for it.
================
*/
// FUNCTION: STBC 0x0047cd50
int FuzzyLogic::AddRule( int setID, int resultID, float confidence ) {
	int index = -1;
	if ( numRules < maxRules ) {
		rules[numRules].setID = setID;
		rules[numRules].resultID = resultID;
		rules[numRules].confidence = confidence;
		rules[numRules].percentageInSet = 0;
		index = numRules++;
	}
	return index;
}

/*
================
FuzzyLogic::GetRule

Copies the rule with the number out: its set, its result and its
confidence; false for no such rule.
================
*/
// FUNCTION: STBC 0x0047cdb0
bool FuzzyLogic::GetRule( FuzzySet &setID, FuzzySet &resultID, float *confidence, int index ) {
	bool found = false;
	if ( index >= 0 && index < numRules ) {
		setID = rules[index].setID;
		resultID = rules[index].resultID;
		*confidence = rules[index].confidence;
		found = true;
	}
	return found;
}

/*
================
FuzzyLogic::RemoveRule

Removes the rule with the number, closing the gap; false for no such rule.
================
*/
// FUNCTION: STBC 0x0047cdf0
bool FuzzyLogic::RemoveRule( int index ) {
	bool removed = false;
	if ( index >= 0 && index < numRules ) {
		memmove( &rules[index], &rules[numRules - 1], sizeof( FuzzyRule ) );
		numRules--;
		removed = true;
	}
	return removed;
}

/*
================
FuzzyLogic::SetRuleConfidence

Changes the confidence of the rule with the number; false for no such rule.
================
*/
// FUNCTION: STBC 0x0047ce30
bool FuzzyLogic::SetRuleConfidence( int index, float confidence ) {
	bool changed = false;
	if ( index >= 0 && index < numRules ) {
		rules[index].confidence = confidence;
		changed = true;
	}
	return changed;
}

/*
================
FuzzyLogic::BreakIntoSets

How far the value lies in each of the sets the ascending boundaries
make: a tuple of one membership per boundary, two of them nonzero
between boundaries, a single 1 at or beyond the ends.
================
*/
// FUNCTION: STBC 0x0047ce60
PyObject *FuzzyLogic::BreakIntoSets( double value, PyObject *boundaries ) {
	int count = PyTuple_GET_SIZE( boundaries );
	PyObject *memberships = PyTuple_New( count );
	double lower = PyFloat_AsDouble( PyTuple_GET_ITEM( boundaries, 0 ) );
	if ( value <= lower ) {
		PyTuple_SET_ITEM( memberships, 0, PyFloat_FromDouble( 1.0 ) );
		for ( int i = 1; i < count; i++ ) {
			PyTuple_SET_ITEM( memberships, i, PyFloat_FromDouble( 0.0 ) );
		}
		return memberships;
	}
	if ( PyFloat_AsDouble( PyTuple_GET_ITEM( boundaries, count - 1 ) ) <= value ) {
		PyTuple_SET_ITEM( memberships, count - 1, PyFloat_FromDouble( 1.0 ) );
		for ( int i = 0; i < count - 1; i++ ) {
			PyTuple_SET_ITEM( memberships, i, PyFloat_FromDouble( 0.0 ) );
		}
		return memberships;
	}
	for ( int i = 1; i < count; i++ ) {
		double upper = PyFloat_AsDouble( PyTuple_GET_ITEM( boundaries, i ) );
		if ( upper > lower && value <= upper ) {
			// BUG: lower is never advanced: the fraction is measured from the
			// first boundary, not from boundary i - 1
			double fraction = ( value - lower ) / ( upper - lower );
			PyTuple_SET_ITEM( memberships, i, PyFloat_FromDouble( fraction ) );
			PyTuple_SET_ITEM( memberships, i - 1, PyFloat_FromDouble( 1.0 - fraction ) );
			for ( int j = 0; j < count; j++ ) {
				if ( j < i - 1 || j > i ) {
					PyTuple_SET_ITEM( memberships, j, PyFloat_FromDouble( 0.0 ) );
				}
			}
			return memberships;
		}
	}
	return memberships;
}

/*
================
FuzzyLogic_BreakIntoSets

The App function; SWIG's BreakIntoSets wrapper calls it too.
================
*/
// FUNCTION: STBC 0x0047d000
PyObject *FuzzyLogic_BreakIntoSets( PyObject *self, PyObject *args ) {
	double value = 0.0;
	PyObject *boundaries = NULL;
	// BUG: PyArg_ParseTuple returns 0 on failure, never -1
	if ( PyArg_ParseTuple( args, "dO", &value, &boundaries ) != -1 ) {
		return FuzzyLogic::BreakIntoSets( value, boundaries );
	}
	Py_INCREF( Py_None );
	return Py_None;
}

/*
================
FuzzyLogic::SetPercentageInSet

Sets the percentage of every rule of the set; false when there is none.
================
*/
// FUNCTION: STBC 0x0047d070
bool FuzzyLogic::SetPercentageInSet( int setID, float percentage ) {
	bool found = false;
	for ( int i = 0; i < numRules; i++ ) {
		if ( rules[i].setID == setID ) {
			rules[i].percentageInSet = percentage;
			found = true;
		}
	}
	return found;
}

/*
================
FuzzyLogic::GetResultBySet

The sum of confidence times percentage over the rules that give the set
as their result.
================
*/
// FUNCTION: STBC 0x0047d0b0
float FuzzyLogic::GetResultBySet( int resultID ) {
	float result = 0.0f;
	for ( int i = 0; i < numRules; i++ ) {
		if ( rules[i].resultID == resultID ) {
			result += rules[i].confidence * rules[i].percentageInSet;
		}
	}
	return result;
}

// The rule number of no rule.
// DATA: STBC 0x0088bd00
extern const int RULENUM_INVALID = -1;
