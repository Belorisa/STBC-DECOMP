/*
===============================================================================

	SensorSubsystem.h

	The ship's sensors: what they have scanned and identified, and the
	probes the ship carries.

	0xCC bytes: the SWIG wrapper SensorSubsystem_Create allocates that much.

	Evidence: the constructor (0x566D10) calls PoweredSubsystem's, builds the
	list at 0xAC (an NiTList with two items per block, inline), stores the
	vtable 0x892EAC (34 slots, none new) and clears the members; the
	destructor (0x566E50) empties the list and deletes the timer 0xC4.
	GetObjType returns 0x8023, the type Cast checks. The SWIG wrappers
	SensorSubsystem_* read the property (0x18) and the number of probes
	(0xC8) inline, and write the number of probes inline.

===============================================================================
*/

#ifndef __SENSORSUBSYSTEM_H__
#define __SENSORSUBSYSTEM_H__

#include "PoweredSubsystem.h"
#include "SensorProperty.h"
#include "NiTList.h"

class BaseObjectClass;
class SensorSubsystem;
class TGStream;
class ShipClass;
class TGObject;
class TGSequence;

// The sensors scan their surroundings (the SWIG constant ET_SENSORS_PERIODIC_SCAN).
#define ET_SENSORS_PERIODIC_SCAN	0x80008B

// Above this power percentage the sensors see everything outside nebulae
// (invented name).
#define OVERPOWERED_SENSORS		1.2f

// Near range is this fraction of the sensor range (invented name).
#define NEAR_RANGE_FRACTION		0.5f

// The seconds the sensors take to identify an object (invented name).
#define IDENTIFICATION_TIME		4.0f

// GetObjType's value for a SensorSubsystem.
#define SENSORSUBSYSTEM_TYPE		0x8023

// What the player's sensors report, as the SWIG constants of the App
// module give them.
#define ET_SENSORS_RANGE_CHANGED		0x800087
#define ET_SENSORS_SHIP_IDENTIFIED		0x800088
#define ET_SENSORS_SHIP_NEAR_PROXIMITY	0x800089
#define ET_SENSORS_SHIP_FAR_PROXIMITY	0x80008A

/*
===============================================================================

	SensorData

	What the player's sensors know: the objects identified, the
	identifications under way, and the objects in far and in near range.
	The proximity events go to the player when an object enters or leaves
	a range (the event's bool is set on entering).

	0x68 bytes, the one instance is SensorSubsystem's static sensorData
	(0x98C000; the save code's log calls it the SensorSubsystem statics).

	Evidence: the constructor (0x5683A0) builds four lists of 2-item
	blocks and sets 0x64; the destructor (0x568400) empties them. The
	log strings "Saving/Loading sensor data desiring timer %d on object
	%d" name the identifications. The class and member names are
	invented.

===============================================================================
*/

// An identification under way: the timer that will send
// ET_SENSORS_SHIP_IDENTIFIED, and the object (invented name).
struct SensorIdentification {
	long				timerID;	// 0x00
	BaseObjectClass *	object;		// 0x04 an ID between Read and FixUp
};

// invented name
class SensorData {
public:
						SensorData( void );
						~SensorData( void );

	void				UpdateObjects( void );
	void				UpdateObject( BaseObjectClass *object );
	void				ClearProximity( void );
	bool				IsNear( BaseObjectClass *object );
	bool				IsInRange( BaseObjectClass *object );
	void				StartIdentification( BaseObjectClass *object );
	void				EndIdentification( TGObject *object );
	void				ClearIdentified( void );
	void				CancelIdentification( TGObject *object );
	void				ClearIdentifications( void );
	void				AddIdentified( TGObject *object );
	bool				IsIdentified( BaseObjectClass *object );
	bool				IsIdentifying( BaseObjectClass *object );
	bool				Write( TGStream *stream );
	bool				Read( TGStream *stream );
	bool				CanFixUp( void );
	void				FixUp( void );

	NiTList<long>		identifiedObjects;	// 0x00 IDs
	NiTList<SensorIdentification *>	identifications;	// 0x18 owned
	NiTList<long>		farObjects;			// 0x30 IDs
	NiTList<long>		nearObjects;		// 0x48 IDs
	SensorSubsystem *	playerSensors;		// 0x60 an ID between Read and FixUp
	bool				unpowered;			// 0x64 the player's sensors had no power at the last scan; true at first
};

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class SensorSubsystem : public PoweredSubsystem {
public:
					SensorSubsystem( long id = 0 );
	virtual			~SensorSubsystem( void );

	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	// TGEventHandlerObject
	virtual TGEventHandlerTable *	GetClassHandlers( void );
	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	// ShipSubsystem
	virtual void	SetProperty( SubsystemProperty *property_ );
	virtual void	Update( float deltaTime );

	// PoweredSubsystem
	virtual void	TurnOff( void );

	static SensorSubsystem *	Create( void ) { return new SensorSubsystem; }
	static SensorSubsystem *	Cast( TGObject * );
	SensorProperty *	GetProperty( void ) { return (SensorProperty *)property; }
	float			GetBaseSensorRange( void ) { return GetProperty()->GetBaseSensorRange(); }
	int				GetNumProbes( void ) { return numProbes; }
	void			SetNumProbes( int numProbes_ ) { numProbes = numProbes_; }
	float	GetSensorRange( void );
	TGSequence *	ScanAllObjects( void );
	void	IdentifyObject( BaseObjectClass * );
	void	ForceObjectIdentified( BaseObjectClass * );
	float	GetIdentificationTime( void );
	void	AddProbe( long );
	bool	IsObjectVisible( BaseObjectClass * );
	static void	TargetableChanged( ShipClass *ship );	// ShipClass::SetTargetable calls it after the change (invented name)
	// measure: always measure the distance, also for the player's ship (invented name).
	bool	IsObjectNear( BaseObjectClass *object, bool measure );
	bool	IsObjectFar( BaseObjectClass *object, bool measure );
	bool	IsObjectKnown( BaseObjectClass * );

	// Register the class's handler methods by name, and add them to its table.
	// The save code's hooks for sensorData (invented names).
	static bool		WriteStatics( TGStream *stream );
	static bool		ReadStatics( TGStream *stream );
	static bool		CanFixUpStatics( void );
	static void		FixUpStatics( void );
	// The player's sensor knowledge, which the interface reads (invented name).
	static SensorData &	GetSensorData( void ) { return sensorData; }

	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	static void		HandleSetPlayer( TGEventHandlerObject *object, TGEvent *event );
	void			HandleEnterSet( TGEvent *event );
	void			HandleExitSet( TGEvent *event );
	void			HandleShipIdentified( TGEvent *event );
	void			HandleShipDecloaked( TGEvent *event );
	void			HandlePeriodicScanEvent( TGEvent *event );

protected:
	float			lastSensorRange;		// 0xA8 the range at the last Update (invented name)
	NiTList<long>	probes;					// 0xAC the IDs of the probes AddProbe added (invented name)
	long			scanTimerID;				// 0xC4 the ID of the periodic scan's timer; the destructor deletes it (invented name)
	int				numProbes;				// 0xC8

	static TGEventHandlerTable	classHandlers;		// invented name
	static SensorData	sensorData;		// invented name
};

/*
================
SensorSubsystem::IsTypeOf
================
*/
// FUNCTION: STBC 0x00566d80
inline bool SensorSubsystem::IsTypeOf( int type ) {
	if ( type == SENSORSUBSYSTEM_TYPE ) {
		return true;
	}
	return PoweredSubsystem::IsTypeOf( type );
}

#endif /* !__SENSORSUBSYSTEM_H__ */
