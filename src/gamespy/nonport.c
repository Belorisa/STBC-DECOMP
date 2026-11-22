/******
nonport.c

The GameSpy SDK's portability layer, retail's 2001 version. Fitted to
retail; names and bodies follow IGN's 2011 SDK where it keeps the same
functions (common/gsPlatformUtil.c, common/gsPlatformSocket.c). Retail's
older version reads and writes the unique ID through two registry helpers
of its own (GetReg, SetReg: invented names) and writes the ID's generation
inline, where the 2011 SDK has GenerateID.
******/

#include <stdlib.h>
#include <string.h>
#include "nonport.h"
#include <time.h>

#define REG_KEY	  "Software\\GameSpy\\GameSpy 3D\\Registration"

static int SetReg(HKEY key, char *subkey, char *valname, char *value);
static int GetReg(HKEY key, char *subkey, char *valname, char *dest);
static long longrand(void);
static long nextlongrand(long seed);

// FUNCTION: STBC 0x6ACD90
unsigned long current_time(void)  //returns current time in milliseconds
{
	return (GetTickCount());
}

// FUNCTION: STBC 0x6ACDA0
void msleep(unsigned long msec)
{
	Sleep(msec);
}

// FUNCTION: STBC 0x6ACDB0
void SocketStartUp(void)
{
	WSADATA data;

	WSAStartup(MAKEWORD(1,1), &data);
}

// FUNCTION: STBC 0x6ACDD0
void SocketShutDown(void)
{
	WSACleanup();
}

// FUNCTION: STBC 0x6ACDE0
int SetSockBlocking(SOCKET sock, int isblocking)
{
	int rcode;
	unsigned long argp;

	if(isblocking)
		argp = 0;
	else
		argp = 1;
	rcode = ioctlsocket(sock, FIONBIO, &argp);
	return (rcode == 0);
}

// FUNCTION: STBC 0x6ACE10
int DisableNagle(SOCKET sock)
{
	int rcode;
	int noDelay = 1;

	rcode = setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char *)&noDelay, sizeof(int));
	return (rcode != SOCKET_ERROR);
}

// FUNCTION: STBC 0x6ACE40
char *GOAGetUniqueID(void)
{
	static char keyval[20];
	int i;

	if (GetReg(HKEY_CURRENT_USER, REG_KEY, "Crypt", keyval) != 0 || strlen(keyval) != 19)//need to generate a new key
	{
		const char crypttab[63] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
		LARGE_INTEGER l1;
		UINT seed;
		if (QueryPerformanceCounter(&l1))
			seed = (l1.LowPart ^ l1.HighPart);
		else
			seed = 0;
		Util_RandSeed(seed ^ GetTickCount() ^ (unsigned long)time(NULL) ^ clock());
		for (i = 0; i < 19; i++)
			if (i == 4 || i == 9 || i == 14)
				keyval[i] = '-';
		else
			keyval[i] = crypttab[Util_RandInt(0, 62)];
		keyval[19] = 0;
		SetReg(HKEY_CURRENT_USER, REG_KEY, "Crypt", keyval);
	}

	// Strip out the -'s.
	/////////////////////
	memmove(keyval + 4, keyval + 5, 4);
	memmove(keyval + 8, keyval + 10, 4);
	memmove(keyval + 12, keyval + 15, 4);
	keyval[16] = '\0';

	return keyval;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Cross platform random number generator
#define RANa 16807                 // multiplier
#define LONGRAND_MAX 2147483647L   // 2**31 - 1

// DATA: STBC 0x95A920
static long randomnum = 1;

// to seed it
// FUNCTION: STBC 0x6ACF70
void Util_RandSeed(unsigned long seed)
{
	// nonzero seed
	randomnum = seed ? (long)(seed & LONGRAND_MAX) : 1;
}

// FUNCTION: STBC 0x6ACF90
int Util_RandInt(int low, int high)
{
	int range = high-low;
	int num;

	if (range == 0)
		return (low); // Prevent divide by zero

	num = (int)(longrand() % range);

	return(num + low);
}

// return next random long
// FUNCTION: STBC 0x6ACFC0
static long longrand(void)
{
	randomnum = nextlongrand(randomnum);
	return randomnum;
}

// FUNCTION: STBC 0x6ACFE0
static long nextlongrand(long seed)
{
	unsigned

	long lo, hi;
	lo = RANa *(unsigned long)(seed & 0xFFFF);
	hi = RANa *((unsigned long)seed >> 16);
	lo += (hi & 0x7FFF) << 16;

	if (lo > LONGRAND_MAX)
	{
		lo &= LONGRAND_MAX;
		++lo;
	}
	lo += hi >> 15;

	if (lo > LONGRAND_MAX)
	{
		lo &= LONGRAND_MAX;
		++lo;
	}

	return(long)lo;
}

// Writes the string value valname of the key subkey, creating the key
// (invented name).
// FUNCTION: STBC 0x6AD060
static int SetReg(HKEY key, char *subkey, char *valname, char *value)
{
	HKEY thekey = NULL;
	DWORD disp;
	int ret;

	ret = RegCreateKeyExA(key, subkey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &thekey, &disp);
	if (ret != ERROR_SUCCESS)
		return ret;
	ret = RegSetValueExA(thekey, valname, 0, REG_SZ, (const LPBYTE)value, strlen(value)+1);
	RegCloseKey(thekey);
	return ret;
}

// Reads the string value valname of the key subkey into dest; zero when it
// was read (invented name).
// FUNCTION: STBC 0x6AD0D0
static int GetReg(HKEY key, char *subkey, char *valname, char *dest)
{
	HKEY thekey = NULL;
	DWORD len;
	char buf[MAX_PATH];
	int ret;

	ret = RegOpenKeyExA(key, subkey, 0, KEY_READ, &thekey);
	if (ret != ERROR_SUCCESS)
		return ret;
	len = MAX_PATH;
	ret = RegQueryValueExA(thekey, valname, 0, NULL, (LPBYTE)buf, &len);
	if (ret == ERROR_SUCCESS)
		strcpy(dest, buf);
	RegCloseKey(thekey);
	return ret;
}
