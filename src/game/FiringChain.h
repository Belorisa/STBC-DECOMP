/*
===============================================================================

	FiringChain.h

	A firing chain: the weapon groups that fire together, as a bit per group
	(group 1 is bit 0), and the chain's name.

	8 bytes: WeaponSystem::ReadFromStream (0x585B80) allocates that much and
	calls the constructor (0x586190), which clears both members. The SWIG
	wrappers FiringChain_* call SetName, GetFirstGroup and GetNextGroup;
	SetName copies the name into the pools, and the destructor (0x5861A0)
	frees it through SetName( NULL ). The stream functions read and write
	the groups as an int and the name with TGStream's slots 10 and 11.

===============================================================================
*/

#ifndef __FIRINGCHAIN_H__
#define __FIRINGCHAIN_H__

class TGStream;

// A chain holds at most this many groups, one bit each.
#define FIRINGCHAIN_MAX_GROUPS		32

class FiringChain {
public:
					FiringChain( void );
					~FiringChain( void );

	void	SetName( char * );
	int	GetFirstGroup( void );
	int	GetNextGroup( int );
	char *	GetName( void ) { return name; }
	int	GetWeaponGroups( void ) { return groups; }
	bool	HasGroup( int group ) { return ( groups & ( 1 << ( group - 1 ) ) ) ? true : false; }
	void	SetWeaponGroups( int groups_ ) { groups = groups_; }

	// Writes the chain to a saved game, and reads it back (invented names).
	bool	WriteToStream( TGStream *stream );
	bool	ReadFromStream( TGStream *stream );

protected:
	int				groups;					// 0x00 a bit per group in the chain (invented name)
	char *			name;					// 0x04 owned copy
};

#endif /* !__FIRINGCHAIN_H__ */
