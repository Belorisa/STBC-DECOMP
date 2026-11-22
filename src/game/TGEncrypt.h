/*
===============================================================================

	TGEncrypt.h

	The encryption a network applies to its packets (the type of
	TGNetwork_SetEncryptor's argument). The network owns the one it is
	given: SetEncryptor (0x6B7090) deletes the old one through its vtable
	slot 0.

	0x58 bytes. Evidence: TGNetwork's constructor allocates 0x58 bytes,
	stores the vtable 0x8958C0 (three slots: the scalar deleting destructor
	0x6B8220, then 0x6C2490 and 0x6C2520, which each restart the state with
	0x6C2280 and transform a buffer of the given length in place, returning
	true) and calls 0x6C2280, which zeroes the state and copies the key
	"AlbyRules!" to 0x48.

===============================================================================
*/

#ifndef __TGENCRYPT_H__
#define __TGENCRYPT_H__

// FUNCTION: STBC 0x006b8220 ??_GTGEncrypt@@UAEPAXI@Z @game/TGNetwork
class TGEncrypt {
public:
					TGEncrypt( void ) { Init(); }
	virtual			~TGEncrypt( void ) {}

	virtual bool	Encrypt( char *buffer, int length );	// invented name
	virtual bool	Decrypt( char *buffer, int length );	// invented name

	void			Init( void );		// invented name

protected:
	// The cipher is Alexander Pukall's PC1 with an 80-bit key: its
	// multipliers 0x15A and 0x4E35, its five key words and its key mixing;
	// the members keep the reference code's names (its globals), widened
	// to 32 bits.
	void			Assemble( void );	// the keystream word for the next byte, in inter
	void			Code( void );		// one round of x1a0[i]

	unsigned int	ax;					// 0x04
	unsigned int	bx;					// 0x08
	unsigned int	cx;					// 0x0C
	unsigned int	dx;					// 0x10
	unsigned int	si;					// 0x14
	unsigned int	tmp;				// 0x18
	unsigned int	x1a2;				// 0x1C
	unsigned int	x1a0[5];			// 0x20
	unsigned int	res;				// 0x34 the last round's output
	unsigned int	i;					// 0x38 the round
	unsigned int	inter;				// 0x3C
	unsigned int	cfc;				// 0x40 inter's high byte
	unsigned int	cfd;				// 0x44 inter's low byte
	unsigned char	key[12];			// 0x48 "AlbyRules!", its first ten bytes cleared first; PC1's cle
	int				c;					// 0x54 the byte being transformed
};

#endif /* !__TGENCRYPT_H__ */
