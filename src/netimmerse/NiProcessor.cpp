// NiProcessor.cpp -- the processor-specific vertex transforms

#include "NiRTLib.h"
#include "NiProcessorSpecificCode.h"
#include "NiMatrix3.h"
#include "NiPoint3.h"
#include "NiSystem.h"
#include <windows.h>

// The <iostream> quads (ios_base::Init, _Winit). Retail lays them out after
// the plain transforms and the locale pair (0x823590); their .bss bytes
// (0x9A2DA1) sit beside this unit's processor flags (0x9A2DA4).
// FUNCTION: STBC 0x00823590 _$E17
// FUNCTION: STBC 0x008235b0 _$E15
// FUNCTION: STBC 0x008235c0 _$E23
// FUNCTION: STBC 0x008235e0 _$E21

// The locale-id guard, compiled without optimization like the code
// before the end of the unit: the guard and its registration apart.
// FUNCTION: STBC 0x00823540 _$E45
// FUNCTION: STBC 0x00823570 _$E44

// DATA: STBC 0x9797C0 ?transformPoints@NiProcessorSpecificCode@@2P6AXGPBMPAMPAVNiMatrix3@@M0@ZA
NiProcessorSpecificCode::PointTransformFunc NiProcessorSpecificCode::transformPoints = NiProcessorSpecificCode::TransformPointsBasic;
// DATA: STBC 0x9797BC ?transformVectors@NiProcessorSpecificCode@@2P6AXGPBMPAMPAVNiMatrix3@@@ZA
NiProcessorSpecificCode::VectorTransformFunc NiProcessorSpecificCode::transformVectors = NiProcessorSpecificCode::TransformVectorsBasic;

// The processor kind and its names (Gamebryo 1.1.2: ms_eProcessorID,
// ms_bIDSet and ms_ppIDStrings).
// DATA: STBC 0x9A2DA4 ?processorID@NiProcessorSpecificCode@@1W4ProcessorID@1@A
NiProcessorSpecificCode::ProcessorID NiProcessorSpecificCode::processorID;
// DATA: STBC 0x9A2DA8 ?idSet@NiProcessorSpecificCode@@1_NA
bool NiProcessorSpecificCode::idSet;
// DATA: STBC 0x9797C4 ?idStrings@NiProcessorSpecificCode@@1PAPBDA
const char *NiProcessorSpecificCode::idStrings[NiProcessorSpecificCode::NI_NUM_PROCESSOR_IDS] = {
	"Unsupported processor",
	"Pentium",
	"Pentium with MMX",
	"Pentium Pro",
	"Pentium II",
	"Pentium III",
	"3DNow-compatible"
};

/*
================
NiProcessorSpecificCode::GetID
================
*/
inline NiProcessorSpecificCode::ProcessorID NiProcessorSpecificCode::GetID() {
	if ( !idSet ) {
		IdentifyProcessor();
	}
	return processorID;
}

/*
================
NiProcessorInitializer

Points the transforms at the versions for the processor when the program
starts: a static object whose constructor runs the check. The name is
invented.
================
*/
class NiProcessorInitializer {
public:
	NiProcessorInitializer() { NiProcessorSpecificCode::OptimizeMainEngineForProcessor(); }
};

// FUNCTION: STBC 0x822F30 _$E38
static NiProcessorInitializer processorInitializer;

/*
================
NiTransformVectorsPentium

rotate * vector for each vector, hand scheduled for the Pentium's two
pipes. The matrix is read as nine floats in row-major order.
================
*/
// FUNCTION: STBC 0x822FA0 ?NiTransformVectorsPentium@@YAXGPBMPAMPAVNiMatrix3@@@Z
// ORIGINAL-ASM: the loop is the original's inline assembly, interleaved by hand for the Pentium
void NiTransformVectorsPentium( unsigned short count, const float *model, float *world, NiMatrix3 *matrix ) {
	unsigned int vertexCount = (unsigned int)count;

	__asm {
		mov		edx, vertexCount
		cmp		edx, 0
		jnz		loopSetup
		jmp		loopEnd

	loopSetup:
		mov		eax, model
		mov		ebx, world
		mov		ecx, matrix

	loopTop:
		fld		dword ptr [eax]
		fmul	dword ptr [ecx]
		fld		dword ptr [eax+4]
		fmul	dword ptr [ecx+4]
		fld		dword ptr [eax+8]
		fmul	dword ptr [ecx+8]
		fxch
		faddp	st(2), st(0)
		fld		dword ptr [eax]
		fmul	dword ptr [ecx+12]
		fxch	st(2)
		faddp	st(1), st(0)
		fxch
		fld		dword ptr [eax+4]
		fmul	dword ptr [ecx+16]
		fld		dword ptr [eax+8]
		fmul	dword ptr [ecx+20]
		fxch	st(3)
		fstp	dword ptr [ebx]
		faddp	st(1), st(0)
		fld		dword ptr [eax]
		fmul	dword ptr [ecx+24]
		fxch	st(2)
		faddp	st(1), st(0)
		fld		dword ptr [eax+4]
		fmul	dword ptr [ecx+28]
		fld		dword ptr [eax+8]
		fmul	dword ptr [ecx+32]
		fxch
		faddp	st(3), st(0)
		fxch
		fstp	dword ptr [ebx+4]
		fadd
		add		eax, 12
		add		ebx, 12
		fstp	dword ptr [ebx-4]
		dec		edx
		jnz		loopTop
	loopEnd:
	}
}

/*
================
NiTransformPointsPentium

scale * ( rotate * point ) + translate for each point, hand scheduled for
the Pentium's two pipes: the rotation is scaled once into a local copy,
read as nine floats in row-major order. Gamebryo 1.1.2 has the name, with
an NiTransform for the last three arguments.
================
*/
// FUNCTION: STBC 0x823030 ?NiTransformPointsPentium@@YAXGPBMPAMPAVNiMatrix3@@M0@Z
// ORIGINAL-ASM: the loop is the original's inline assembly, interleaved by hand for the Pentium
void NiTransformPointsPentium( unsigned short count, const float *model, float *world, NiMatrix3 *matrix, float scale,
							   const float *translate ) {
	unsigned int vertexCount = (unsigned int)count;
	float translateX = translate[0];
	float translateY = translate[1];
	float translateZ = translate[2];

	float scaled[9];
	float *scaledMatrix = scaled;
	scaled[0] = scale * matrix->entry[0][0];
	scaled[1] = scale * matrix->entry[0][1];
	scaled[2] = scale * matrix->entry[0][2];
	scaled[3] = scale * matrix->entry[1][0];
	scaled[4] = scale * matrix->entry[1][1];
	scaled[5] = scale * matrix->entry[1][2];
	scaled[6] = scale * matrix->entry[2][0];
	scaled[7] = scale * matrix->entry[2][1];
	scaled[8] = scale * matrix->entry[2][2];

	__asm {
		mov		edx, vertexCount
		cmp		edx, 0
		jnz		loopSetup
		jmp		loopEnd

	loopSetup:
		mov		eax, model
		mov		ebx, world
		mov		ecx, scaledMatrix

	loopTop:
		fld		dword ptr [eax]
		fmul	dword ptr [ecx]
		fld		dword ptr [eax+4]
		fmul	dword ptr [ecx+4]
		fld		dword ptr [eax+8]
		fmul	dword ptr [ecx+8]
		fxch
		faddp	st(2), st(0)
		fld		dword ptr [eax]
		fmul	dword ptr [ecx+12]
		fxch	st(2)
		faddp	st(1), st(0)
		fxch
		fld		dword ptr [eax+4]
		fmul	dword ptr [ecx+16]
		fld		dword ptr [eax+8]
		fmul	dword ptr [ecx+20]
		fxch
		faddp	st(2), st(0)
		fxch	st(2)
		fadd	translateX
		fld		dword ptr [ecx+24]
		fxch	st(2)
		faddp	st(3), st(0)
		fxch
		fmul	dword ptr [eax]
		fld		dword ptr [ecx+28]
		fld		dword ptr [ecx+32]
		fxch
		fmul	dword ptr [eax+4]
		fxch	st(4)
		fadd	translateY
		fxch
		fmul	dword ptr [eax+8]
		fxch	st(4)
		faddp	st(2), st(0)
		fxch	st(2)
		fstp	dword ptr [ebx]
		faddp	st(2), st(0)
		fstp	dword ptr [ebx+4]
		fadd	translateZ
		add		eax, 12
		add		ebx, 12
		fstp	dword ptr [ebx-4]
		dec		edx
		jnz		loopTop
	loopEnd:
	}
}

// The plain transforms are compiled without optimization, as in the
// Gamebryo lineage's NiProcessor.cpp: retail's two keep an EBP frame, the
// loop counter and the pointer copies in frame slots, and reload every
// operand from memory.
#pragma optimize( "", off )

/*
================
NiProcessorSpecificCode::IdentifyProcessor

Asks the processor through CPUID for its extended functions (3DNow), then
for its MMX and CMOV feature bits; a processor without CPUID raises an
exception and counts as unsupported.
================
*/
// FUNCTION: STBC 0x823140
// ORIGINAL-ASM: the CPUID queries are the original's inline assembly (Gamebryo 1.1.2's NiProcessor.cpp compiles to the same blocks)
void NiProcessorSpecificCode::IdentifyProcessor() {
	static unsigned int cpuidResults;

	idSet = true;
	NiOutputDebugString( "NetImmerse WARNING: Processor detection code: benign first-chance exception possible\n" );

	__try {
		__asm {
			mov		eax, 0x80000000
			cpuid
			mov		cpuidResults, eax
		}
	} __except ( EXCEPTION_EXECUTE_HANDLER ) {
		processorID = NI_UNSUPPORTED;
		return;
	}

	if ( cpuidResults & 0x80000000 ) {
		__try {
			__asm {
				mov		edx, 0x80000001		// BUG: the function number belongs in eax
				cpuid
				mov		cpuidResults, eax
			}
		} __except ( EXCEPTION_EXECUTE_HANDLER ) {
			cpuidResults = 0;
		}
		if ( cpuidResults & 0x80000000 ) {
			processorID = NI_3DNOW;
			return;
		}
	}

	__try {
		__asm {
			mov		eax, 1
			cpuid
			mov		cpuidResults, edx
		}
	} __except ( EXCEPTION_EXECUTE_HANDLER ) {
		processorID = NI_UNSUPPORTED;
		return;
	}

	if ( !( cpuidResults & 0x800000 ) ) {
		if ( !( cpuidResults & 0x8000 ) ) {
			processorID = NI_PENTIUM;
			return;
		} else {
			processorID = NI_PENTIUM_PRO;
		}
	} else {
		if ( !( cpuidResults & 0x8000 ) ) {
			processorID = NI_PENTIUM_MMX;
		} else {
			processorID = NI_PENTIUM_II;
		}
	}
}

/*
================
NiProcessorSpecificCode::GetIdentityString
================
*/
// FUNCTION: STBC 0x8232C0
const char *NiProcessorSpecificCode::GetIdentityString() {
	return idStrings[GetID()];
}

/*
================
NiProcessorSpecificCode::OptimizeMainEngineForProcessor

The Pentium versions for every processor with CPUID (the Pentium III's
case is a block of its own), the plain ones otherwise.
================
*/
// FUNCTION: STBC 0x8232F0
void NiProcessorSpecificCode::OptimizeMainEngineForProcessor() {
	switch ( GetID() ) {
	case NI_UNSUPPORTED:
		transformPoints = TransformPointsBasic;
		transformVectors = TransformVectorsBasic;
		break;
	case NI_PENTIUM:
	case NI_PENTIUM_MMX:
	case NI_PENTIUM_PRO:
	case NI_PENTIUM_II:
	case NI_3DNOW:
		transformPoints = NiTransformPointsPentium;
		transformVectors = NiTransformVectorsPentium;
		break;
	case NI_PENTIUM_III:
		transformPoints = NiTransformPointsPentium;
		transformVectors = NiTransformVectorsPentium;
		break;
	}
}

/*
================
NiProcessorSpecificCode::TransformPointsBasic

scale * ( rotate * point ) + translate for each point. The name and the
float pointers are DamageTool.exe's and Gamebryo 1.1.2's.
================
*/
// FUNCTION: STBC 0x823380
void NiProcessorSpecificCode::TransformPointsBasic( unsigned short count, const float *model, float *world, NiMatrix3 *matrix, float scale, const float *translate ) {
	// Gamebryo 1.1.2's text: under /Od *( model + 1 ) loads the coordinate before the matrix pointer,
	// where model[1] would load both pointers first. The cast is the lineage's: NiMatrix3 stores its
	// entries row by row as float[3][3].
	float *pFMatrix = (float *)matrix;
	for ( unsigned int i = 0; i < count; i++, model += 3, world += 3 ) {
		*world = ( pFMatrix[0] * ( *model ) + pFMatrix[1] * ( *( model + 1 ) ) + pFMatrix[2] * ( *( model + 2 ) ) ) * scale + *translate;
		*( world + 1 ) = ( pFMatrix[3] * ( *model ) + pFMatrix[4] * ( *( model + 1 ) ) + pFMatrix[5] * ( *( model + 2 ) ) ) * scale + *( translate + 1 );
		*( world + 2 ) = ( pFMatrix[6] * ( *model ) + pFMatrix[7] * ( *( model + 1 ) ) + pFMatrix[8] * ( *( model + 2 ) ) ) * scale + *( translate + 2 );
	}
}

/*
================
NiProcessorSpecificCode::TransformVectorsBasic

rotate * vector for each vector. The name and the float pointers are
DamageTool.exe's and Gamebryo 1.1.2's.
================
*/
// FUNCTION: STBC 0x823470
void NiProcessorSpecificCode::TransformVectorsBasic( unsigned short count, const float *model, float *world, NiMatrix3 *matrix ) {
	// Gamebryo 1.1.2's text: under /Od *( model + 1 ) loads the coordinate before the matrix pointer,
	// where model[1] would load both pointers first. The cast is the lineage's: NiMatrix3 stores its
	// entries row by row as float[3][3].
	float *pFMatrix = (float *)matrix;
	for ( unsigned int i = 0; i < count; i++, model += 3, world += 3 ) {
		*world = pFMatrix[0] * ( *model ) + pFMatrix[1] * ( *( model + 1 ) ) + pFMatrix[2] * ( *( model + 2 ) );
		*( world + 1 ) = pFMatrix[3] * ( *model ) + pFMatrix[4] * ( *( model + 1 ) ) + pFMatrix[5] * ( *( model + 2 ) );
		*( world + 2 ) = pFMatrix[6] * ( *model ) + pFMatrix[7] * ( *( model + 1 ) ) + pFMatrix[8] * ( *( model + 2 ) );
	}
}
