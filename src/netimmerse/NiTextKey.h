/*
===============================================================================

	NiTextKey.h

	A string at a time in an animation: the text keys of an animation's
	NiTextKeyExtraData, which mark its events.

	Evidence: an 8-byte key, a float and a heap string; the constructor
	(0x7B6CC0) stores the time, clears the string and copies the text
	through 0x7B6D00, which frees the old string first; the destructor
	(0x7B6CF0) frees the string; the stream holds the time, then the string
	through NiStream's C-string helpers (0x7B6DA0, 0x7B6DD0).
	NiTextKeyExtraData builds its arrays through the default constructor
	(0x7974D0) and this destructor. The names follow the Gamebryo lineage
	(NiTextKey: m_fTime, m_pcText, GetTime, GetText, SetText).

===============================================================================
*/

#ifndef __NITEXTKEY_H__
#define __NITEXTKEY_H__

#include <string.h>

class NiStream;

// FUNCTION: STBC 0x7975A0 ??_ENiTextKey@@QAEPAXI@Z @netimmerse/NiTextKeyExtraData
class NiTextKey {
public:
	// FUNCTION: STBC 0x7974D0 ??0NiTextKey@@QAE@XZ @netimmerse/NiTextKeyExtraData
						NiTextKey() { text = 0; }
						NiTextKey( float time_, const char *text_ );
						~NiTextKey();

	float				GetTime() const { return time; }
	void				SetTime( float time_ ) { time = time_; }
	const char *		GetText() const { return text; }
	// copies the text; NULL clears it
	void				SetText( const char *text_ );

	// equal times and texts (Gamebryo 1.2.2: NiTextKey.inl)
	bool				operator!=( const NiTextKey &key ) { return time != key.time || strcmp( text, key.text ); }

	// a heap copy of the key (name invented)
	NiTextKey *			Copy() const;

	// streaming
	void				LoadBinary( NiStream &stream );
	void				SaveBinary( NiStream &stream );

	float				time;
	char *				text;		// heap copy, owned
};

// the prefix of the text keys a morph may leave or join a sequence at
extern const char MORPH_PREFIX[];

#endif /* !__NITEXTKEY_H__ */
