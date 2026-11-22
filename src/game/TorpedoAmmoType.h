/*
===============================================================================

	TorpedoAmmoType.h

	One torpedo type of a TorpedoSystemProperty: how many the system carries
	and the script that describes the torpedo. 0x10 bytes: the property
	keeps four of them at 0x64 (TorpedoSystemProperty::GetMaxTorpedoes
	0x6944D0 reads the count at index * 0x10 + 0x64).

	Evidence: SetTorpedoScript (0x694240) frees and replaces the script at
	0x04 and resets 0x08 to 1 and 0x0C to 0; TorpedoTube's torpedo creation
	(0x57CD90) passes the script at 0x04 to Torpedo's Create.

===============================================================================
*/

#ifndef __TORPEDOAMMOTYPE_H__
#define __TORPEDOAMMOTYPE_H__

// The number of torpedo types a torpedo system carries (invented name).
#define MAX_AMMO_TYPES			4

class TorpedoAmmoType {
public:
					TorpedoAmmoType( void );
					~TorpedoAmmoType( void );

	float	GetPowerCost( void );
	char *	GetAmmoName( void );
	void	SetTorpedoScript( char * );

	int				GetMaxTorpedoes( void ) { return maxTorpedoes; }
	char *			GetTorpedoScript( void ) { return torpedoScript; }
	void			SetMaxTorpedoes( int maxTorpedoes_ ) { maxTorpedoes = maxTorpedoes_; }
	float			GetLaunchSpeed( void ) { return launchSpeed; }
	char *	GetLaunchSound( void ) { return launchSound; }
	// The SWIG members m_pcModule, m_fLaunchSpeed and m_pcLaunchSound store through these, taking the pointer as it is.
	void			SetModuleName( char *module ) { torpedoScript = module; }
	void			SetLaunchSpeed( float speed ) { launchSpeed = speed; }
	void			SetLaunchSound( char *sound ) { launchSound = sound; }

	// The property streams the members directly.
	friend class TorpedoSystemProperty;

protected:
	int				maxTorpedoes;			// 0x00
	char *			torpedoScript;			// 0x04 owned
	float			launchSpeed;			// 0x08 the script's GetLaunchSpeed; 1 without a script
	char *			launchSound;			// 0x0C the script's GetLaunchSound; NULL without one
};

#endif /* !__TORPEDOAMMOTYPE_H__ */
