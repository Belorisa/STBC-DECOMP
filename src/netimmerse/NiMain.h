/*
===============================================================================

	NiMain.h

	The NetImmerse core library's umbrella header: the scene graph, property,
	geometry, light and particle classes, in alphabetical order.

	Evidence: units that include it carry the 73 class registrations of these
	headers (NiStreamMacros.h) in the order alphabetical includes give, each
	header's own includes first (SetClass's run from 0x40CA80, ScriptObject's
	at 0x40A600). Gamebryo 1.1.2 keeps the header under this name with the
	same alphabetical list.

===============================================================================
*/

#ifndef __NIMAIN_H__
#define __NIMAIN_H__

#include "NiAccumulator.h"
#include "NiAlphaAccumulator.h"
#include "NiAlphaProperty.h"
#include "NiAmbientLight.h"
#include "NiAutoNormalParticles.h"
#include "NiAVObject.h"
#include "NiBillboardNode.h"
#include "NiBone.h"
#include "NiBSPNode.h"
#include "NiCamera.h"
#include "NiClusterAccumulator.h"
#include "NiCollisionSwitch.h"
#include "NiCorrectionProperty.h"
#include "NiDirectionalLight.h"
#include "NiDitherProperty.h"
#include "NiDynamicEffect.h"
#include "NiEnvMappedTriShape.h"
#include "NiExtraData.h"
#include "NiFltAnimationNode.h"
#include "NiFogProperty.h"
#include "NiGeometry.h"
#include "NiLight.h"
#include "NiLines.h"
#include "NiLODNode.h"
#include "NiMaterialProperty.h"
#include "NiMultiTextureProperty.h"
#include "NiNode.h"
#include "NiObject.h"
#include "NiObjectNET.h"
#include "NiParticles.h"
#include "NiPointLight.h"
#include "NiProperty.h"
#include "NiScreenPolygon.h"
#include "NiShadeProperty.h"
#include "NiSkinController.h"
#include "NiSortAdjustNode.h"
#include "NiSpecularProperty.h"
#include "NiSpotLight.h"
#include "NiStencilProperty.h"
#include "NiStringExtraData.h"
#include "NiSwitchNode.h"
#include "NiTextureEffect.h"
#include "NiTextureModeProperty.h"
#include "NiTextureProperty.h"
#include "NiTransparentProperty.h"
#include "NiTriangles.h"
#include "NiTriShape.h"
#include "NiTriShapeDynamicData.h"
#include "NiTriShapeSkinController.h"
#include "NiTriStrip.h"
#include "NiTriStrips.h"
#include "NiVertexColorProperty.h"
#include "NiVertWeightsExtraData.h"
#include "NiWireframeProperty.h"
#include "NiZBufferProperty.h"

#endif /* !__NIMAIN_H__ */
