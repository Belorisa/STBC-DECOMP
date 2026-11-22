/*
===============================================================================

	NiProcessorSpecificCode.h

	The vertex transforms that NetImmerse picks per processor: function
	pointers that the processor check points at the plain or the optimized
	versions.

	Evidence: the pointers at 0x9797BC and 0x9797C0 start at the plain
	versions (0x823470, 0x823380) and the processor check (0x8232F0, inlined
	into the unit's static initializer 0x822F30) repoints them for some of the seven processor kinds ("Pentium",
	"Pentium with MMX", "Pentium Pro", "Pentium II" ...). 0x823380 computes
	scale * ( rotate * point ) + translate for each point, 0x823470
	rotate * vector; both are cdecl and take the count as an unsigned
	short. NiGeometry::UpdateWorldVertices (0x7EE000) and
	UpdateWorldNormals (0x7EE0F0) call through them, evaluating every
	argument before the first push: the calls go through an inline
	function. The class name follows
	the Gamebryo lineage (NiProcessorSpecificCode::TransformPoints and
	TransformVectors); Gamebryo 1.1.2's NiProcessor.h has the rest:
	IdentifyProcessor, GetID, GetIdentityString,
	OptimizeMainEngineForProcessor, the ProcessorID values and the three
	data members behind GetID and GetIdentityString (ms_eProcessorID,
	ms_bIDSet, ms_ppIDStrings). The plain versions are compiled without
	optimization (NiProcessor.cpp, under #pragma optimize off, as in the
	Gamebryo lineage): both keep an EBP frame, their loop counter and
	pointer copies live in frame slots, and every operand is reloaded from
	memory.

===============================================================================
*/

#ifndef __NIPROCESSORSPECIFICCODE_H__
#define __NIPROCESSORSPECIFICCODE_H__

class NiMatrix3;
class NiPoint3;

class NiProcessorSpecificCode {
public:
	// the processor kinds the check tells apart (Gamebryo 1.1.2's names)
	enum ProcessorID {
		NI_UNSUPPORTED,
		NI_PENTIUM,
		NI_PENTIUM_MMX,
		NI_PENTIUM_PRO,
		NI_PENTIUM_II,
		NI_PENTIUM_III,
		NI_3DNOW,
		NI_NUM_PROCESSOR_IDS
	};

	// points the transforms at the versions for the processor
	static void				OptimizeMainEngineForProcessor();
	// asks the processor through CPUID what it is
	static void				IdentifyProcessor();
	// the processor kind, identified on first use (inline in NiProcessor.cpp)
	static ProcessorID		GetID();
	// "Pentium", "Pentium with MMX" ...
	static const char *		GetIdentityString();

	typedef void			( *PointTransformFunc )( unsigned short count, const float *model, float *world, NiMatrix3 *matrix, float scale, const float *translate );
	typedef void			( *VectorTransformFunc )( unsigned short count, const float *model, float *world, NiMatrix3 *matrix );

	static void				TransformPoints( unsigned short count, const float *model, float *world, NiMatrix3 *matrix, float scale, const float *translate ) { transformPoints( count, model, world, matrix, scale, translate ); }
	static void				TransformVectors( unsigned short count, const float *model, float *world, NiMatrix3 *matrix ) { transformVectors( count, model, world, matrix ); }

	// the plain versions, where the pointers start (Gamebryo 1.1.2: NiProcessorSpecificCode::Transform*Basic)
	static void				TransformPointsBasic( unsigned short count, const float *model, float *world, NiMatrix3 *matrix, float scale, const float *translate );
	static void				TransformVectorsBasic( unsigned short count, const float *model, float *world, NiMatrix3 *matrix );

	static PointTransformFunc	transformPoints;
	static VectorTransformFunc	transformVectors;

protected:
	static ProcessorID		processorID;	// set by IdentifyProcessor
	static bool				idSet;			// whether IdentifyProcessor has run
	static const char *		idStrings[NI_NUM_PROCESSOR_IDS];	// GetIdentityString's names
};

// the Pentium version of TransformVectors, chosen by the processor check
void NiTransformVectorsPentium( unsigned short count, const float *model, float *world, NiMatrix3 *matrix );
// the Pentium version of TransformPoints
void NiTransformPointsPentium( unsigned short count, const float *model, float *world, NiMatrix3 *matrix, float scale,
							   const float *translate );

#endif /* !__NIPROCESSORSPECIFICCODE_H__ */
