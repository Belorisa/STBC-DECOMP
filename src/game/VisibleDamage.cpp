/*
===============================================================================

	VisibleDamage.cpp

	The damage drawn on a DamageableObject's model.

===============================================================================
*/

#include "VisibleDamage.h"
#include "DamagePolygonizer.h"
#include "OBBModel.h"
#include "VisibleDamageBuilder.h"
#include "NiLines.h"
#include "NiLinesData.h"
#include "NiMain.h"

/*
================
VisibleDamage::Init
================
*/
// FUNCTION: STBC 0x004b12c0
void VisibleDamage::Init( void ) {
	MetaSurfacePolygonizer::Init();
	MetaVolumePolygonizer::Init();
}

/*
================
VisibleDamage::Shutdown
================
*/
// FUNCTION: STBC 0x004b12d0
void VisibleDamage::Shutdown( void ) {
	MetaSurfacePolygonizer::Shutdown();
	MetaVolumePolygonizer::Shutdown();
}

/*
================
VisibleDamage::VisibleDamage
================
*/
// FUNCTION: STBC 0x004b12e0
VisibleDamage::VisibleDamage() {
	polygonizer = new MetaVolumePolygonizer;
	builder = new MetaSurfacePolygonizer;
}

/*
================
VisibleDamage::~VisibleDamage
================
*/
// FUNCTION: STBC 0x004b13a0
VisibleDamage::~VisibleDamage() {
	if ( obbModel != NULL ) {
		obbModel->callback = NULL;
	}
	delete polygonizer;
	delete builder;
}

/*
================
VisibleDamage::GetGrids
================
*/
// FUNCTION: STBC 0x004b14f0
NiTList<DamageVoxelGrid *> *VisibleDamage::GetGrids( void ) {
	return &polygonizer->grids;
}

/*
================
VisibleDamage::ClearGrids
================
*/
// FUNCTION: STBC 0x004b1500
void VisibleDamage::ClearGrids( void ) {
	polygonizer->grids.RemoveAll();
}

/*
================
VisibleDamage::CloneGrid

The piece that breaks off along the grid: a new VisibleDamage drawn with
copies of this one's properties on a new model, whose polygonizer keeps only
the grid's cells and whose builder takes the pieces that meet the grid's box.
The volumes whose box meets the grid's box go to list and to the new
polygonizer.
================
*/
// FUNCTION: STBC 0x004b1550
// DRAFT: call set as retail's (the voxel data pointer's constructor 0x4B21B0 and GetBox's NiPoint3 constructor out of line); register picks around the builder's grid store and the SetUnknown18 argument, and the box loads' schedule differ
VisibleDamage *VisibleDamage::CloneGrid( DamageVoxelGrid *grid, MetaVolumePool *list ) {
	VisibleDamage *clone = new VisibleDamage;
	if ( unknown10 != NULL ) {
		clone->unknown10 = (NiProperty *)unknown10->Clone();
	}
	if ( unknown14 != NULL ) {
		clone->unknown14 = (NiProperty *)unknown14->Clone();
	}
	if ( unknown18 != NULL ) {
		clone->unknown18 = (NiProperty *)unknown18->Clone();
	}
	clone->SetModel( new NiNode );
	clone->SetVolumeList( list );
	clone->polygonizer->SetUnknown00( polygonizer->unknown00 );

	NiBinaryVoxelExtraData *data = new NiBinaryVoxelExtraData;
	data->SetVoxelData( polygonizer->voxelData->GetVoxelData() );
	clone->polygonizer->SetVoxelData( data, false );

	builder->Unknown4BA030( grid, clone->builder );
	clone->builder->SetUnknown0C( builder->GetUnknown0C() );
	clone->builder->SetUnknown08( builder->GetUnknown08() );
	clone->builder->SetUnknown10( builder->GetUnknown10() );
	clone->builder->SetUnknown19( builder->GetUnknown19() );
	clone->builder->grid = clone->polygonizer->grid;
	clone->builder->SetUnknown18( builder->GetUnknown18() );
	clone->SetOBBModel( new OBBModel( clone->model, 0 ) );
	clone->polygonizer->SetUnknown08( false );
	clone->polygonizer->SetUnknown09( polygonizer->unknown09 );
	clone->polygonizer->Unknown4C2F80( grid );
	clone->builder->Unknown4BA250();

	NiPoint3 boxMin;
	NiPoint3 boxMax;
	grid->GetBox( boxMin, boxMax );

	for ( unsigned int i = 0; i < volumeList->objects.GetSize(); i++ ) {
		MetaVolume *volume = volumeList->objects.GetAt( i );
		if ( volume->GetBoxMax().x > boxMin.x && volume->GetBoxMax().y > boxMin.y && volume->GetBoxMax().z > boxMin.z
		  && volume->GetBoxMin().x < boxMax.x && volume->GetBoxMin().y < boxMax.y && volume->GetBoxMin().z < boxMax.z ) {
			list->AddVolume( volume );
			clone->polygonizer->AddVolume( volume );
		}
	}
	clone->Polygonize();
	clone->Unknown4B2050();
	return clone;
}

/*
================
VisibleDamage::GetCellCount
================
*/
// FUNCTION: STBC 0x004b1930
unsigned int VisibleDamage::GetCellCount( void ) {
	return polygonizer->cellCount;
}

/*
================
VisibleDamage::Unknown4B1940
================
*/
// FUNCTION: STBC 0x004b1940
bool VisibleDamage::Unknown4B1940( const NiPoint3 *point, bool flag ) {
	float scale = model->GetWorldScale();
	NiPoint3 local = ( *point - model->GetWorldTranslate() ) * model->GetWorldRotate() / scale;
	return polygonizer->Unknown4C2AF0( &local, flag );
}

/*
================
VisibleDamage::IsCellSet
================
*/
// FUNCTION: STBC 0x004b19d0
bool VisibleDamage::IsCellSet( unsigned short x, unsigned short y, unsigned short z ) {
	DamageVoxelGrid *grid = polygonizer->grid;
	if ( grid != NULL && grid->IsInBox( x, y, z ) ) {
		return grid->IsSet( x, y, z );
	}
	return false;
}

/*
================
VisibleDamage::GetCell
================
*/
// FUNCTION: STBC 0x004b1a60
void VisibleDamage::GetCell( const NiPoint3 *point, unsigned short *x, unsigned short *y, unsigned short *z ) {
	float scale = model->GetWorldScale();
	NiPoint3 local = ( *point - model->GetWorldTranslate() ) * model->GetWorldRotate() / scale;
	polygonizer->GetCell( &local, x, y, z );
}

/*
================
VisibleDamage::IsCellFilled
================
*/
// FUNCTION: STBC 0x004b1b00
bool VisibleDamage::IsCellFilled( unsigned short x, unsigned short y, unsigned short z ) {
	NiBinaryVoxelExtraData *voxelData = polygonizer->voxelData;
	if ( voxelData->GetVoxelData()->IsSolid( x, y, z ) ) {
		return true;
	}
	const VoxelBSPNode *cell;
	return voxelData->GetVoxelData()->GetSurfaceCell( x, y, z, cell ) ? true : false;
}

/*
================
VisibleDamage::Unknown4B1B50
================
*/
// FUNCTION: STBC 0x004b1b50
bool VisibleDamage::Unknown4B1B50( void ) {
	return polygonizer->voxelData != NULL;
}

/*
================
VisibleDamage::Unknown4B1B90
================
*/
// FUNCTION: STBC 0x004b1b90
void VisibleDamage::Unknown4B1B90( DamageVoxelGrid *grid ) {
	polygonizer->Unknown4C2F80( grid );
	builder->Unknown4BA250();
}

/*
================
VisibleDamage::VirtualSlot00
================
*/
// FUNCTION: STBC 0x004b1bc0
bool VisibleDamage::VirtualSlot00( NiTriShape *shape, int triangle, const unsigned short *corner, unsigned short *corner1,
								   unsigned short *corner2, VisibleDamagePiece **piece ) {
	return builder->Unknown4B48C0( shape, triangle, corner, corner1, corner2, piece );
}

/*
================
VisibleDamage::VirtualSlot03
================
*/
// FUNCTION: STBC 0x004b1bf0
DamageBitArray *VisibleDamage::VirtualSlot03( NiTriShape *shape ) {
	return builder->GetUnknown58( shape );
}

/*
================
VisibleDamage::VirtualSlot04
================
*/
// FUNCTION: STBC 0x004b1c00
DamageBitArray *VisibleDamage::VirtualSlot04( NiTriShape *shape ) {
	return builder->GetUnknown70( shape );
}

/*
================
VisibleDamage::VirtualSlot05
================
*/
// FUNCTION: STBC 0x004b1c10
unsigned short *VisibleDamage::VirtualSlot05( NiTriShape *shape ) {
	return builder->GetTriList( shape );
}

/*
================
VisibleDamage::VirtualSlot01
================
*/
// FUNCTION: STBC 0x004b1c20
int VisibleDamage::VirtualSlot01( int unknown1, int, int, const NiPoint3 *origin, const NiPoint3 *direction, const NiPoint3 *point,
								 const float *t, int unknown8, int unknown9, VisibleDamagePiece *piece ) {
	if ( !builder->Unknown4B49B0( piece, point ) ) {
		return 1;
	}
	return polygonizer->Unknown4C3680( origin, direction, point, unknown8, t, unknown1, unknown9 ) ? 2 : 0;
}

/*
================
VisibleDamage::VirtualSlot02
================
*/
// FUNCTION: STBC 0x004b1c80
int VisibleDamage::VirtualSlot02( int unknown1, int, int, const NiPoint3 *origin, const NiPoint3 *direction, const NiPoint3 *point,
								 const float *t, int unknown8, int unknown9, VisibleDamagePiece *piece ) {
	if ( !builder->Unknown4B49B0( piece, point ) ) {
		return 1;
	}
	return polygonizer->Unknown4C38C0( origin, direction, point, unknown8, t, unknown1, unknown9 ) ? 2 : 0;
}

/*
================
VisibleDamage::SetBuilderUnknown18
================
*/
// FUNCTION: STBC 0x004b1ce0
void VisibleDamage::SetBuilderUnknown18( bool enable ) {
	builder->SetUnknown18( enable );
}

/*
================
VisibleDamage::SetPolygonizerUnknown09
================
*/
// FUNCTION: STBC 0x004b1cf0
void VisibleDamage::SetPolygonizerUnknown09( bool enable ) {
	polygonizer->SetUnknown09( enable );
}

/*
================
VisibleDamage::SetPolygonizerUnknown08
================
*/
// FUNCTION: STBC 0x004b1d00
void VisibleDamage::SetPolygonizerUnknown08( bool enable ) {
	polygonizer->SetUnknown08( enable );
}

/*
================
VisibleDamage::SetModel
================
*/
// FUNCTION: STBC 0x004b1d10
void VisibleDamage::SetModel( NiNode *model_ ) {
	model = model_;
	polygonizer->CreateDebugNode( model_, unknown10 );
	builder->SetModel( model_, unknown10, unknown14, unknown18 );
}

/*
================
VisibleDamage::SetVolumeList
================
*/
// FUNCTION: STBC 0x004b1d70
void VisibleDamage::SetVolumeList( MetaVolumePool *list ) {
	volumeList = list;
	polygonizer->volumeList = list;
	builder->volumeList = list;
}

/*
================
VisibleDamage::SetVoxelData
================
*/
// FUNCTION: STBC 0x004b1e10
void VisibleDamage::SetVoxelData( NiBinaryVoxelExtraData *data ) {
	polygonizer->SetVoxelData( data, true );
	builder->grid = polygonizer->grid;
}

/*
================
VisibleDamage::SetUnknown14
================
*/
// FUNCTION: STBC 0x004b1e40
void VisibleDamage::SetUnknown14( NiProperty *property ) {
	unknown14 = property;
}

/*
================
VisibleDamage::SetUnknown10
================
*/
// FUNCTION: STBC 0x004b1e80
void VisibleDamage::SetUnknown10( NiProperty *property ) {
	unknown10 = property;
}

/*
================
VisibleDamage::SetUnknown18
================
*/
// FUNCTION: STBC 0x004b1ec0
void VisibleDamage::SetUnknown18( NiProperty *property ) {
	unknown18 = property;
}

/*
================
VisibleDamage::SetUnknown00
================
*/
// FUNCTION: STBC 0x004b1f00
void VisibleDamage::SetUnknown00( float value ) {
	polygonizer->SetUnknown00( value );
}

/*
================
VisibleDamage::SetOBBModel
================
*/
// FUNCTION: STBC 0x004b1f20
void VisibleDamage::SetOBBModel( OBBModel *model_ ) {
	if ( obbModel != NULL ) {
		obbModel->callback = NULL;
	}
	obbModel = model_;
	if ( obbModel != NULL ) {
		obbModel->callback = this;
	}
	builder->obbModel = model_;
}

/*
================
VisibleDamage::SetBuilderUnknown0C
================
*/
// FUNCTION: STBC 0x004b1fa0
void VisibleDamage::SetBuilderUnknown0C( float value ) {
	builder->SetUnknown0C( value );
}

/*
================
VisibleDamage::SetBuilderUnknown08
================
*/
// FUNCTION: STBC 0x004b1fb0
void VisibleDamage::SetBuilderUnknown08( float value ) {
	builder->SetUnknown08( value );
}

/*
================
VisibleDamage::SetBuilderUnknown10
================
*/
// FUNCTION: STBC 0x004b1fc0
void VisibleDamage::SetBuilderUnknown10( float value ) {
	builder->SetUnknown10( value );
}

/*
================
VisibleDamage::SetBuilderUnknown19
================
*/
// FUNCTION: STBC 0x004b1fe0
void VisibleDamage::SetBuilderUnknown19( bool value ) {
	builder->SetUnknown19( value );
}

/*
================
VisibleDamage::AddVolume
================
*/
// FUNCTION: STBC 0x004b1ff0
void VisibleDamage::AddVolume( MetaVolume *volume ) {
	if ( builder->unknown18 ) {
		builder->AddVolume( volume );
	}
	if ( polygonizer->unknown09 || polygonizer->unknown08 ) {
		polygonizer->AddVolume( volume );
	}
}

/*
================
VisibleDamage::Polygonize
================
*/
// FUNCTION: STBC 0x004b2030
void VisibleDamage::Polygonize( void ) {
	if ( polygonizer->unknown09 || polygonizer->unknown08 ) {
		polygonizer->Refresh();
	}
}

/*
================
VisibleDamage::Unknown4B2050
================
*/
// FUNCTION: STBC 0x004b2050
void VisibleDamage::Unknown4B2050( void ) {
	if ( builder->unknown18 ) {
		builder->Unknown4B4E50( false );
	}
}

