/*
===============================================================================

	OptimizedAvoidObstacles.h

	The optimized version of the script preprocessor AvoidObstacles
	(AI.Preprocessors): it looks ahead along the ship's motion for objects
	in the way and, when it finds one, steers the ship around it, skipping
	the contained AI while it does; the impulse engines' power setting is
	raised meanwhile and restored after.

	0x68 bytes: CreateFromScript (0x47F380) and Create (0x480D60) allocate
	that much.

	Evidence: the static initialiser 0x47F1B0 files the script class name
	"AvoidObstacles" with 0x47F380 in PreprocessingAI's list of optimized
	versions, and 0x47F150 files the stream create function 0x480D60 for
	the AI type 7. 0x47F380 calls the constructor 0x47F270 with the
	scripted AI, which it then deletes; the constructor and the stream
	constructor (0x480BE0) store the vtable 0x88BD9C (PreprocessingAI's 23
	slots: the destructor 0x47F310, WriteToStream 0x480B70, GetAIType
	0x47F2E0, VirtualSlot20 0x47F2F0 and the preprocessing call 0x47F3F0
	are overridden). The class has no name string: it is named after
	OptimizedFireScript and OptimizedSelectTarget, the optimized versions
	the SWIG interface names. Member names are invented.

===============================================================================
*/

#ifndef __OPTIMIZEDAVOIDOBSTACLES_H__
#define __OPTIMIZEDAVOIDOBSTACLES_H__

#include "PreprocessingAI.h"
#include "NiTList.h"
#include "NiPoint3.h"

class NiPoint3;
class ObjectClass;
class TGObject;
class ShipClass;

// The AI type of the class (slot 19).
#define AI_TYPE_AVOID_OBSTACLES		7

// An object the AI steers around, and a time of its own; the stream
// keeps both. The names are invented.
struct AvoidedObstacle {
	long				objectID;	// 0x00
	float				time;		// 0x04
};

/*
================
AvoidanceCandidate

An object near the ship's way, as the steering weighs it (0x20 bytes: 0x480250
allocates and fills them; the name and the member names are invented).
================
*/
struct AvoidanceCandidate {
	NiPoint3			direction;	// 0x00 the unit vector from the ship to the object
	NiPoint3			velocity;	// 0x0C the object's velocity; zero for one that doesn't move
	float				cosine;		// 0x18 the cosine of the half angle the object blocks, from the ship's radius and the object's and the distance; 0 when they overlap
	float				weight;		// 0x1C minus a constant over the distance (written, not read by the code mapped so far)
};

class OptimizedAvoidObstacles : public PreprocessingAI {
public:
	// Takes over the scripted AI's contained AI.
						OptimizedAvoidObstacles( PreprocessingAI *ai );
						OptimizedAvoidObstacles( TGStream *stream, bool *ok );
	virtual				~OptimizedAvoidObstacles( void );

	// ArtificialIntelligence
	virtual bool		WriteToStream( TGStream *stream );
	virtual int			GetAIType( void );
	virtual bool		VirtualSlot20( int type );

	// PreprocessingAI
	virtual int			CallPreprocessingMethod( double time );

	// The optimized version of the scripted AI, which is deleted (invented
	// name).
	static ArtificialIntelligence *	CreateFromScript( PreprocessingAI *ai );
	// The stream constructor, for CreateFromStream.
	static ArtificialIntelligence *	Create( TGStream *stream );

protected:
	// Looks for objects in the way of the ship's motion; true when there
	// is one, with the direction to steer and the impulse to use.
	bool				Unknown47F5F0( ShipClass *ship, NiPoint3 *direction, float *impulse );

	// Drops the noted obstacles that aren't among the found objects when
	// their object is gone or was noted over a second ago, and adds the ones
	// that stay to the found objects.
	void				Unknown47FB00( NiTList<ObjectClass *> *found );

	// Whether the ship, with the radius, would come near the object within
	// the look-ahead time: true at once for one already close, else both are
	// stepped and tested with WillCollide.
	bool				Unknown47FCA0( ShipClass *ship, float radius, ObjectClass *object );

	// Picks the direction to steer and the impulse to use among the
	// candidates the objects make: the best of the ones away from each object
	// and of the 26 directions of a 3x3x3 cube.
	void				Unknown480250( ShipClass *ship, NiTList<ObjectClass *> *objects, NiPoint3 *direction, float *impulse );

	// The rating of a direction against the candidates (invented split of
	// 0x480250's work).
	float				Unknown4806F0( ShipClass *ship, NiPoint3 *direction, NiTList<AvoidanceCandidate *> *candidates );

	// Whether the direction is out of every candidate's cone.
	bool				Unknown480B20( const NiPoint3 &direction, NiTList<AvoidanceCandidate *> *candidates );

	// Whether two spheres, the first of the radius, each moving by the given
	// step from its position, touch within the fraction maxTime of the step
	// (invented name).
	// Notes the object as one in the way, with the time (invented name).
	void				NoteObstacle( TGObject *object );

	static bool			WillCollide( NiPoint3 *position, NiPoint3 *step, NiPoint3 *otherPosition, NiPoint3 *otherStep, float radius, float maxTime );

	NiTList<AvoidedObstacle *>	obstacles;	// 0x4C owned records
	float				savedPowerPercentage;	// 0x64 the impulse engines' setting before the avoiding; -1 for none
};

#endif /* !__OPTIMIZEDAVOIDOBSTACLES_H__ */
