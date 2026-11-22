/*
===============================================================================

	TGEncrypt.cpp

===============================================================================
*/

#include <iostream>
#include <string.h>

#include "TGEncrypt.h"
#include "NiMain.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x006c1a70 _$E408
// FUNCTION: STBC 0x006c1a90 _$E407

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006c1910 _$E32
// FUNCTION: STBC 0x006c1920 _$E29
// FUNCTION: STBC 0x006c1930 _$E31
// FUNCTION: STBC 0x006c1940 _$E30
// FUNCTION: STBC 0x006c1950 _$E38
// FUNCTION: STBC 0x006c1960 _$E35
// FUNCTION: STBC 0x006c1970 _$E37
// FUNCTION: STBC 0x006c1980 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006c1990 _$E45
// FUNCTION: STBC 0x006c19a0 _$E44
// FUNCTION: STBC 0x006c19b0 _$E49
// FUNCTION: STBC 0x006c19c0 _$E48
// FUNCTION: STBC 0x006c19d0 _$E60
// FUNCTION: STBC 0x006c19e0 _$E59
// FUNCTION: STBC 0x006c19f0 _$E64
// FUNCTION: STBC 0x006c1a00 _$E63
// FUNCTION: STBC 0x006c1a10 _$E68
// FUNCTION: STBC 0x006c1a20 _$E67
// FUNCTION: STBC 0x006c1a30 _$E72
// FUNCTION: STBC 0x006c1a40 _$E71
// FUNCTION: STBC 0x006c1a50 _$E196
// FUNCTION: STBC 0x006c1a60 _$E195
// FUNCTION: STBC 0x006c1aa0 _$E138
// FUNCTION: STBC 0x006c1ab0 _$E137
// FUNCTION: STBC 0x006c1ac0 _$E151
// FUNCTION: STBC 0x006c1ad0 _$E150
// FUNCTION: STBC 0x006c1ae0 _$E201
// FUNCTION: STBC 0x006c1af0 _$E200
// FUNCTION: STBC 0x006c1b00 _$E206
// FUNCTION: STBC 0x006c1b10 _$E205
// FUNCTION: STBC 0x006c1b20 _$E210
// FUNCTION: STBC 0x006c1b30 _$E209
// FUNCTION: STBC 0x006c1b40 _$E214
// FUNCTION: STBC 0x006c1b50 _$E213
// FUNCTION: STBC 0x006c1b60 _$E221
// FUNCTION: STBC 0x006c1b70 _$E220
// FUNCTION: STBC 0x006c1b80 _$E246
// FUNCTION: STBC 0x006c1b90 _$E245
// FUNCTION: STBC 0x006c1ba0 _$E250
// FUNCTION: STBC 0x006c1bb0 _$E249
// FUNCTION: STBC 0x006c1bc0 _$E96
// FUNCTION: STBC 0x006c1bd0 _$E95
// FUNCTION: STBC 0x006c1be0 _$E232
// FUNCTION: STBC 0x006c1bf0 _$E231
// FUNCTION: STBC 0x006c1c00 _$E239
// FUNCTION: STBC 0x006c1c10 _$E238
// FUNCTION: STBC 0x006c1c20 _$E262
// FUNCTION: STBC 0x006c1c30 _$E261
// FUNCTION: STBC 0x006c1c40 _$E266
// FUNCTION: STBC 0x006c1c50 _$E265
// FUNCTION: STBC 0x006c1c60 _$E306
// FUNCTION: STBC 0x006c1c70 _$E305
// FUNCTION: STBC 0x006c1c80 _$E312
// FUNCTION: STBC 0x006c1c90 _$E311
// FUNCTION: STBC 0x006c1ca0 _$E254
// FUNCTION: STBC 0x006c1cb0 _$E253
// FUNCTION: STBC 0x006c1cc0 _$E258
// FUNCTION: STBC 0x006c1cd0 _$E257
// FUNCTION: STBC 0x006c1ce0 _$E270
// FUNCTION: STBC 0x006c1cf0 _$E269
// FUNCTION: STBC 0x006c1d00 _$E274
// FUNCTION: STBC 0x006c1d10 _$E273
// FUNCTION: STBC 0x006c1d20 _$E278
// FUNCTION: STBC 0x006c1d30 _$E277
// FUNCTION: STBC 0x006c1d40 _$E282
// FUNCTION: STBC 0x006c1d50 _$E281
// FUNCTION: STBC 0x006c1d60 _$E286
// FUNCTION: STBC 0x006c1d70 _$E285
// FUNCTION: STBC 0x006c1d80 _$E290
// FUNCTION: STBC 0x006c1d90 _$E289
// FUNCTION: STBC 0x006c1da0 _$E294
// FUNCTION: STBC 0x006c1db0 _$E293
// FUNCTION: STBC 0x006c1dc0 _$E298
// FUNCTION: STBC 0x006c1dd0 _$E297
// FUNCTION: STBC 0x006c1de0 _$E101
// FUNCTION: STBC 0x006c1df0 _$E100
// FUNCTION: STBC 0x006c1e00 _$E302
// FUNCTION: STBC 0x006c1e10 _$E301
// FUNCTION: STBC 0x006c1e20 _$E106
// FUNCTION: STBC 0x006c1e30 _$E105
// FUNCTION: STBC 0x006c1e40 _$E316
// FUNCTION: STBC 0x006c1e50 _$E315
// FUNCTION: STBC 0x006c1e60 _$E320
// FUNCTION: STBC 0x006c1e70 _$E319
// FUNCTION: STBC 0x006c1e80 _$E324
// FUNCTION: STBC 0x006c1e90 _$E323
// FUNCTION: STBC 0x006c1ea0 _$E328
// FUNCTION: STBC 0x006c1eb0 _$E327
// FUNCTION: STBC 0x006c1ec0 _$E115
// FUNCTION: STBC 0x006c1ed0 _$E114
// FUNCTION: STBC 0x006c1ee0 _$E332
// FUNCTION: STBC 0x006c1ef0 _$E331
// FUNCTION: STBC 0x006c1f00 _$E336
// FUNCTION: STBC 0x006c1f10 _$E335
// FUNCTION: STBC 0x006c1f20 _$E340
// FUNCTION: STBC 0x006c1f30 _$E339
// FUNCTION: STBC 0x006c1f40 _$E121
// FUNCTION: STBC 0x006c1f50 _$E120
// FUNCTION: STBC 0x006c1f60 _$E126
// FUNCTION: STBC 0x006c1f70 _$E125
// FUNCTION: STBC 0x006c1f80 _$E156
// FUNCTION: STBC 0x006c1f90 _$E155
// FUNCTION: STBC 0x006c1fa0 _$E344
// FUNCTION: STBC 0x006c1fb0 _$E343
// FUNCTION: STBC 0x006c1fc0 _$E161
// FUNCTION: STBC 0x006c1fd0 _$E160
// FUNCTION: STBC 0x006c1fe0 _$E348
// FUNCTION: STBC 0x006c1ff0 _$E347
// FUNCTION: STBC 0x006c2000 _$E352
// FUNCTION: STBC 0x006c2010 _$E351
// FUNCTION: STBC 0x006c2020 _$E166
// FUNCTION: STBC 0x006c2030 _$E165
// FUNCTION: STBC 0x006c2040 _$E356
// FUNCTION: STBC 0x006c2050 _$E355
// FUNCTION: STBC 0x006c2060 _$E131
// FUNCTION: STBC 0x006c2070 _$E130
// FUNCTION: STBC 0x006c2080 _$E360
// FUNCTION: STBC 0x006c2090 _$E359
// FUNCTION: STBC 0x006c20a0 _$E364
// FUNCTION: STBC 0x006c20b0 _$E363
// FUNCTION: STBC 0x006c20c0 _$E171
// FUNCTION: STBC 0x006c20d0 _$E170
// FUNCTION: STBC 0x006c20e0 _$E176
// FUNCTION: STBC 0x006c20f0 _$E175
// FUNCTION: STBC 0x006c2100 _$E368
// FUNCTION: STBC 0x006c2110 _$E367
// FUNCTION: STBC 0x006c2120 _$E372
// FUNCTION: STBC 0x006c2130 _$E371
// FUNCTION: STBC 0x006c2140 _$E376
// FUNCTION: STBC 0x006c2150 _$E375
// FUNCTION: STBC 0x006c2160 _$E380
// FUNCTION: STBC 0x006c2170 _$E379
// FUNCTION: STBC 0x006c2180 _$E384
// FUNCTION: STBC 0x006c2190 _$E383
// FUNCTION: STBC 0x006c21a0 _$E388
// FUNCTION: STBC 0x006c21b0 _$E387
// FUNCTION: STBC 0x006c21c0 _$E392
// FUNCTION: STBC 0x006c21d0 _$E391
// FUNCTION: STBC 0x006c21e0 _$E396
// FUNCTION: STBC 0x006c21f0 _$E395
// FUNCTION: STBC 0x006c2200 _$E181
// FUNCTION: STBC 0x006c2210 _$E180
// FUNCTION: STBC 0x006c2220 _$E400
// FUNCTION: STBC 0x006c2230 _$E399
// FUNCTION: STBC 0x006c2240 _$E186
// FUNCTION: STBC 0x006c2250 _$E185
// FUNCTION: STBC 0x006c2260 _$E191
// FUNCTION: STBC 0x006c2270 _$E190

// The key every network starts with.
#define TGENCRYPT_KEY		"AlbyRules!"

// PC1's key length in bytes (80 bits).
#define TGENCRYPT_KEY_LENGTH	10

/*
================
TGEncrypt::Init

Clears the state and installs the key.
================
*/
// FUNCTION: STBC 0x006c2280
void TGEncrypt::Init( void ) {
	memset( key, 0, TGENCRYPT_KEY_LENGTH );
	ax = 0;
	bx = 0;
	cx = 0;
	dx = 0;
	si = 0;
	tmp = 0;
	x1a2 = 0;
	x1a0[0] = 0;
	x1a0[1] = 0;
	x1a0[2] = 0;
	x1a0[3] = 0;
	x1a0[4] = 0;
	res = 0;
	i = 0;
	inter = 0;
	cfc = 0;
	cfd = 0;
	c = 0;
	strcpy( (char *)key, TGENCRYPT_KEY );
}

/*
================
TGEncrypt::Assemble

Runs the five rounds, each key word chained from the last and two key bytes,
and leaves their combined output in inter.
================
*/
// FUNCTION: STBC 0x006c22f0
void TGEncrypt::Assemble( void ) {
	x1a0[0] = ( key[0] * 256 ) + key[1];
	Code();
	inter = res;

	x1a0[1] = x1a0[0] ^ ( ( key[2] * 256 ) + key[3] );
	Code();
	inter = inter ^ res;

	x1a0[2] = x1a0[1] ^ ( ( key[4] * 256 ) + key[5] );
	Code();
	inter = inter ^ res;

	x1a0[3] = x1a0[2] ^ ( ( key[6] * 256 ) + key[7] );
	Code();
	inter = inter ^ res;

	x1a0[4] = x1a0[3] ^ ( ( key[8] * 256 ) + key[9] );
	Code();
	inter = inter ^ res;

	i = 0;
}

/*
================
TGEncrypt::Code

One round: the linear congruential step of PC1 on x1a0[i].
================
*/
// FUNCTION: STBC 0x006c23c0
void TGEncrypt::Code( void ) {
	dx = x1a2 + i;
	ax = x1a0[i];
	cx = 0x015a;
	bx = 0x4e35;

	tmp = ax;
	ax = si;
	si = tmp;

	tmp = ax;
	ax = dx;
	dx = tmp;

	if ( ax != 0 ) {
		ax = ax * bx;
	}

	tmp = ax;
	ax = cx;
	cx = tmp;

	if ( ax != 0 ) {
		ax = ax * si;
		cx = ax + cx;
	}

	tmp = ax;
	ax = si;
	si = tmp;
	ax = ax * bx;
	dx = cx + dx;

	ax = ax + 1;

	x1a2 = dx;
	x1a0[i] = ax;

	res = ax ^ dx;
	i = i + 1;
}

/*
================
TGEncrypt::Encrypt

Encrypts the buffer in place from a fresh state: each plaintext byte is
mixed into the key, then xored with the keystream.
================
*/
// FUNCTION: STBC 0x006c2490
bool TGEncrypt::Encrypt( char *buffer, int length ) {
	Init();
	for ( int n = 0; n < length; n++ ) {
		c = buffer[n];
		Assemble();
		cfc = inter >> 8;
		cfd = inter & 255;

		for ( int compte = 0; compte < TGENCRYPT_KEY_LENGTH; compte++ ) {
			key[compte] = key[compte] ^ c;
		}

		c = c ^ ( cfc ^ cfd );
		buffer[n] = c;
	}
	return true;
}

/*
================
TGEncrypt::Decrypt

The inverse of Encrypt: each byte is xored with the keystream, then the
plaintext is mixed into the key.
================
*/
// FUNCTION: STBC 0x006c2520
bool TGEncrypt::Decrypt( char *buffer, int length ) {
	Init();
	for ( int n = 0; n < length; n++ ) {
		c = buffer[n];
		Assemble();
		cfc = inter >> 8;
		cfd = inter & 255;

		c = c ^ ( cfc ^ cfd );

		for ( int compte = 0; compte < TGENCRYPT_KEY_LENGTH; compte++ ) {
			key[compte] = key[compte] ^ c;
		}

		buffer[n] = c;
	}
	return true;
}
