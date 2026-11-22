/*
===============================================================================

	NiBool.h

	The engine's one-byte boolean.

	Evidence: Gamebryo 1.1.2's NIBOOL.H (typedef unsigned char NiBool), and
	NiStencilProperty::GetViewerStrings (0x807D70), whose m_bEnable string
	retail formats through the unsigned char NiGetViewerString overload
	(0x821A40), as NiStencilProperty::GetStencilOn returning an NiBool does.

===============================================================================
*/

#ifndef __NIBOOL_H__
#define __NIBOOL_H__

typedef unsigned char NiBool;

#endif /* !__NIBOOL_H__ */
