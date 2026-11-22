/******
nonport.h

The GameSpy SDK's portability layer as retail's 2001 version has it: the
Win32 time, sleep and socket calls, and the unique ID kept in the registry.

Fitted to retail (0x6ACD90-0x6AD170); the names follow IGN's 2011 SDK
(common/gsPlatformUtil.c, common/gsPlatformSocket.c), where the same
functions survive.
******/

#ifndef _NONPORT_H_
#define _NONPORT_H_

#include <windows.h>
#include <winsock.h>

#ifdef __cplusplus
extern "C" {
#endif

unsigned long current_time(void);
void msleep(unsigned long msec);

void SocketStartUp(void);
void SocketShutDown(void);
int SetSockBlocking(SOCKET sock, int isblocking);
int DisableNagle(SOCKET sock);

char *GOAGetUniqueID(void);

void Util_RandSeed(unsigned long seed);
int Util_RandInt(int low, int high);

#ifdef __cplusplus
}
#endif

#endif
