/*
===============================================================================

	TGGamepad.h

	A gamepad read through DirectInput: one of the joysticks attached when
	the first gamepad was made. The input manager's startup makes one for
	each of its gamepad slots and polls them each frame. 0x98 bytes. The
	name is invented.

	Evidence: TGInputManager's startup (0x6E4D80) allocates 0x98 bytes and
	calls the constructor 0x721D50 with the application and the slot's
	index; its shutdown (0x6E5090) calls the destructor 0x722020 and frees
	the object. The constructor enumerates the attached joysticks once for
	all gamepads (EnumDevices with DIDEVTYPE_JOYSTICK), sets the range of
	each axis the device has and acquires it; Poll (0x7220D0) reads a
	DIJOYSTATE into the values.

===============================================================================
*/

#ifndef __TGGAMEPAD_H__
#define __TGGAMEPAD_H__

class NiApplication;
struct IDirectInputDevice2A;
struct DIDEVICEINSTANCEA;

// The gamepads the enumeration keeps.
#define TGGAMEPAD_MAX_DEVICES		4

// The buttons a gamepad reports (DIJOYSTATE's).
#define TGGAMEPAD_MAX_BUTTONS		32

// The axes, in the order of the value arrays; each has a bit in the axes
// mask when the device has it (invented names).
enum gamepadAxis_t {
	GAMEPAD_AXIS_X,
	GAMEPAD_AXIS_Y,
	GAMEPAD_AXIS_Z,
	GAMEPAD_AXIS_RZ,
	GAMEPAD_AXIS_RX,
	GAMEPAD_AXIS_RY,
	GAMEPAD_AXIS_SLIDER0,
	GAMEPAD_AXIS_SLIDER1,
	GAMEPAD_AXIS_POV,
	GAMEPAD_NUM_AXES
};

#define GAMEPAD_AXIS_BIT( axis )	( 1 << ( axis ) )

// What the axes are scaled to, and the point-of-view hat's largest value
// (hundredths of a degree).
#define GAMEPAD_AXIS_MINIMUM		-32767
#define GAMEPAD_AXIS_MAXIMUM		32768
#define GAMEPAD_POV_MAXIMUM			35999

// What the last poll read (invented names).
struct TGGamepadState {
	unsigned int	buttons;							// 0x00 a bit per button held
	unsigned char	buttonStates[TGGAMEPAD_MAX_BUTTONS];	// 0x04 0xFF per button held
	unsigned int	axes;								// 0x24 a GAMEPAD_AXIS_BIT per axis the device has
	int				values[GAMEPAD_NUM_AXES];			// 0x28
	int				minimums[GAMEPAD_NUM_AXES];			// 0x4C
	int				maximums[GAMEPAD_NUM_AXES];			// 0x70
};

class TGGamepad {
public:
					TGGamepad( NiApplication *application, int index );
					~TGGamepad( void );

	// Reads the device's state; false when there is no device or it can't
	// be read (it is acquired again when it was lost).
	bool			Poll( void );
	// What the last poll read (invented name; TGInputManager::Poll reads it).
	const TGGamepadState *	GetState( void ) const { return &state; }

protected:
	// Finds the attached joysticks (the first gamepad does it for all).
	static void		EnumerateDevices( NiApplication *application );
	static int __stdcall	EnumerateDevice( const DIDEVICEINSTANCEA *instance, void *application );
	// Lets go of the joysticks (the last gamepad does it).
	static void		ReleaseDevices( void );

	IDirectInputDevice2A *	device;		// 0x00 one of devices, or NULL
	TGGamepadState	state;				// 0x04

	// The attached joysticks, acquired (invented names). One aggregate:
	// EnumerateDevice reloads the count after storing into the array.
	struct JoystickList {
		IDirectInputDevice2A *	devices[TGGAMEPAD_MAX_DEVICES];
		int				numDevices;
	};

	static JoystickList	joysticks;
	static int		refCount;			// the gamepads made and not yet deleted
};

#endif /* !__TGGAMEPAD_H__ */
