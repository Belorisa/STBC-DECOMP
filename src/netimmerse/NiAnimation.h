/*
===============================================================================

	NiAnimation.h

	The NetImmerse animation library's umbrella header: the keys, key data
	and controllers, in alphabetical order.

	Evidence: FlareController's unit (statics 0x4F1040-0x4F3D90, .bss
	0x986728-0x986910) registers these 46 classes beside NiMain.h's run and
	the sound headers', and no other include list gives them together.
	Gamebryo 1.1.2 keeps the header under this name with the same
	alphabetical list (sdk/NIANIMATION.H); the headers that version adds
	and retail lacks are left out.

===============================================================================
*/

#ifndef __NIANIMATION_H__
#define __NIANIMATION_H__

#include "NiAlphaController.h"
#include "NiAnimBlender.h"
#include "NiBezFloatKey.h"
#include "NiBezPosKey.h"
#include "NiBezRotKey.h"
#include "NiColorData.h"
#include "NiColorKey.h"
#include "NiEulerRotKey.h"
#include "NiFlipController.h"
#include "NiFloatController.h"
#include "NiFloatData.h"
#include "NiFloatKey.h"
#include "NiGravity.h"
#include "NiKeyframeController.h"
#include "NiKeyframeData.h"
#include "NiKeyframeManager.h"
#include "NiKeyType4FloatKey.h"
#include "NiKeyType6FloatKey.h"
#include "NiLightColorController.h"
#include "NiLinColorKey.h"
#include "NiLinFloatKey.h"
#include "NiLinPosKey.h"
#include "NiLinRotKey.h"
#include "NiLookAtController.h"
#include "NiMaterialColorController.h"
#include "NiMorphController.h"
#include "NiMorphData.h"
#include "NiMorphKey.h"
#include "NiMorpherController.h"
#include "NiParticleBomb.h"
#include "NiParticleSystemController.h"
#include "NiPathController.h"
#include "NiPlanarCollider.h"
#include "NiPosData.h"
#include "NiPosKey.h"
#include "NiRollController.h"
#include "NiRotKey.h"
#include "NiSequenceStreamHelper.h"
#include "NiSphericalCollider.h"
#include "NiTCBFloatKey.h"
#include "NiTCBPosKey.h"
#include "NiTCBRotKey.h"
#include "NiTextKeyExtraData.h"
#include "NiVisController.h"
#include "NiVisData.h"

#endif /* !__NIANIMATION_H__ */
