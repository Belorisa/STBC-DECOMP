/*
===============================================================================

	FuzzyLogic.h

	The members of FuzzyLogic that the SWIG wrappers FuzzyLogic_* call. Only what the
	wrappers prove is declared: no data members and no virtual functions yet.

===============================================================================
*/

#ifndef __FUZZYLOGIC_H__
#define __FUZZYLOGIC_H__

#include "Python.h"

// A fuzzy set's number. SWIG's type table makes FuzzySet & interchangeable
// with int * (FuzzyLogic_GetRule), so it is a typedef of int.
typedef int FuzzySet;

// One rule of a FuzzyLogic: 0x10 bytes. The field names are invented, after
// the SWIG wrappers' arguments (AddRule, SetRuleConfidence,
// SetPercentageInSet).
struct FuzzyRule {
	int				setID;					// 0x00 AddRule's first argument; SetPercentageInSet looks rules up by it
	int				resultID;				// 0x04 AddRule's second argument
	float			confidence;				// 0x08 AddRule's third argument
	float			percentageInSet;		// 0x0C 0 at first
};

class FuzzyLogic {
public:
					FuzzyLogic( void );
	int	GetMaxRules( void ) { return maxRules; }
					~FuzzyLogic( void );

	void	SetMaxRules( int );
	int	AddRule( int, int, float );
	bool	GetRule( FuzzySet &setID, FuzzySet &resultID, float *confidence, int index );
	bool	RemoveRule( int );
	bool	SetRuleConfidence( int, float );
	bool	SetPercentageInSet( int, float );
	float	GetResultBySet( int );

	// The value's membership in each set the boundaries (a tuple of
	// floats) make; a new tuple (invented name, after the App function).
	static PyObject *	BreakIntoSets( double value, PyObject *boundaries );

protected:
	int				maxRules;				// 0x00 the room in rules (invented name)
	int				numRules;				// 0x04 (invented name)
	FuzzyRule *		rules;					// 0x08 an array of maxRules (invented name)
};

// The App function FuzzyLogic_BreakIntoSets: a value and a tuple of
// boundaries.
PyObject *	FuzzyLogic_BreakIntoSets( PyObject *self, PyObject *args );

#endif /* !__FUZZYLOGIC_H__ */
