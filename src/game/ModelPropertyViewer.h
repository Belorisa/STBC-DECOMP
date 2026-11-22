/*
===============================================================================

	ModelPropertyViewer.h

	The object TGModelProperty slot 15 shows a property's display model in.
	TODO identity: no code in stbc.exe calls slot 15, so the class (likely
	the model property editor's view, which is not part of the game) is
	known only from what the slot 15 overrides read: the node at 0x16C,
	which the display models are attached under. The name is invented.

===============================================================================
*/

#ifndef __MODELPROPERTYVIEWER_H__
#define __MODELPROPERTYVIEWER_H__

class NiNode;

class ModelPropertyViewer {
public:
	NiNode *		GetRootNode( void ) { return rootNode; }		// invented name

protected:
	char			unknown00[0x16C];	// 0x00 not read by the game
	NiNode *		rootNode;			// 0x16C the display models are attached under (invented name)
};

#endif /* !__MODELPROPERTYVIEWER_H__ */
