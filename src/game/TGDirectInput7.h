/*
===============================================================================

	TGDirectInput7.h

	The DirectInput 7 declarations the input code uses.

	The game asks for DirectInput 7: TGSystemWrapperClass::GetDirectInput
	(0x71A9B0) calls DirectInputCreateEx (DirectX 7's entry point, the
	DINPUT.dll import at 0x78C8A0) with the version 0x700 and
	IID_IDirectInput7A ({9A4CB684-236D-11D3-8E9D-00C04F6844AE}, 0x89D150).
	The input manager's startup (0x6E4D80) creates its devices through
	IDirectInput7's CreateDeviceEx (slot 9) with IID_IDirectInputDevice7A
	(0x89D0E0), and the gamepads poll through IDirectInputDevice2's Poll
	(slot 25).

	The compiler's own include directory holds the DirectX 5 dinput.h, which
	supplies everything DirectX 7 kept unchanged (the device interfaces up
	to IDirectInputDevice2A, the data formats, the properties). The DirectX
	7 SDK itself is not available here, so this header declares its
	additions as the SDK does, as far as the game's code proves them.

===============================================================================
*/

#ifndef __TGDIRECTINPUT7_H__
#define __TGDIRECTINPUT7_H__

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION		0x0700
#endif

#include <windows.h>
#include <dinput.h>

// dinput.h (DirectX 7): the interfaces added after DirectX 5
DEFINE_GUID( IID_IDirectInput7A,			0x9a4cb684,0x236d,0x11d3,0x8e,0x9d,0x00,0xc0,0x4f,0x68,0x44,0xae );
DEFINE_GUID( IID_IDirectInputDevice7A,		0x57d7c6bc,0x2356,0x11d3,0x8e,0x9d,0x00,0xc0,0x4f,0x68,0x44,0xae );

#undef INTERFACE
#define INTERFACE IDirectInput7A

DECLARE_INTERFACE_( IDirectInput7A, IDirectInput2A )
{
	/*** IUnknown methods ***/
	STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID * ppvObj) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;

	/*** IDirectInputA methods ***/
	STDMETHOD(CreateDevice)(THIS_ REFGUID,LPDIRECTINPUTDEVICEA *,LPUNKNOWN) PURE;
	STDMETHOD(EnumDevices)(THIS_ DWORD,LPDIENUMDEVICESCALLBACKA,LPVOID,DWORD) PURE;
	STDMETHOD(GetDeviceStatus)(THIS_ REFGUID) PURE;
	STDMETHOD(RunControlPanel)(THIS_ HWND,DWORD) PURE;
	STDMETHOD(Initialize)(THIS_ HINSTANCE,DWORD) PURE;

	/*** IDirectInput2A methods ***/
	STDMETHOD(FindDevice)(THIS_ REFGUID,LPCSTR,LPGUID) PURE;

	/*** IDirectInput7A methods ***/
	STDMETHOD(CreateDeviceEx)(THIS_ REFGUID,REFIID,LPVOID *,LPUNKNOWN) PURE;
};

typedef struct IDirectInput7A *LPDIRECTINPUT7A;

extern "C" HRESULT WINAPI DirectInputCreateEx( HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID *ppvOut, LPUNKNOWN punkOuter );

#endif /* !__TGDIRECTINPUT7_H__ */
