/*
===============================================================================

	TGIconManager.cpp

===============================================================================
*/

#include "TGIconManager.h"
#include "TGIconGroup.h"
#include "TGIconPoly.h"
#include "NiCamera.h"
#include "TGRect.h"
#include "PyEmbed.h"
#include "TGRootPane.h"
#include "TGStream.h"
#include "NiMain.h"
#include <iostream>

#include <string.h>

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00729080 _$E427
// FUNCTION: STBC 0x00729090 _$E424
// FUNCTION: STBC 0x007290a0 _$E426
// FUNCTION: STBC 0x007290b0 _$E425
// FUNCTION: STBC 0x007290c0 _$E433
// FUNCTION: STBC 0x007290d0 _$E430
// FUNCTION: STBC 0x007290e0 _$E432
// FUNCTION: STBC 0x007290f0 _$E431

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00729100 _$E21
// FUNCTION: STBC 0x00729110 _$E20
// FUNCTION: STBC 0x00729120 _$E28
// FUNCTION: STBC 0x00729130 _$E27
// FUNCTION: STBC 0x00729140 _$E39
// FUNCTION: STBC 0x00729150 _$E38
// FUNCTION: STBC 0x00729160 _$E43
// FUNCTION: STBC 0x00729170 _$E42
// FUNCTION: STBC 0x00729180 _$E47
// FUNCTION: STBC 0x00729190 _$E46
// FUNCTION: STBC 0x007291a0 _$E51
// FUNCTION: STBC 0x007291b0 _$E50
// FUNCTION: STBC 0x007291c0 _$E175
// FUNCTION: STBC 0x007291d0 _$E174
// FUNCTION: STBC 0x007291e0 _$E117
// FUNCTION: STBC 0x007291f0 _$E116
// FUNCTION: STBC 0x00729200 _$E130
// FUNCTION: STBC 0x00729210 _$E129
// FUNCTION: STBC 0x00729220 _$E180
// FUNCTION: STBC 0x00729230 _$E179
// FUNCTION: STBC 0x00729240 _$E185
// FUNCTION: STBC 0x00729250 _$E184
// FUNCTION: STBC 0x00729260 _$E189
// FUNCTION: STBC 0x00729270 _$E188
// FUNCTION: STBC 0x00729280 _$E193
// FUNCTION: STBC 0x00729290 _$E192
// FUNCTION: STBC 0x007292a0 _$E200
// FUNCTION: STBC 0x007292b0 _$E199
// FUNCTION: STBC 0x007292c0 _$E273
// FUNCTION: STBC 0x007292d0 _$E272
// FUNCTION: STBC 0x007292e0 _$E277
// FUNCTION: STBC 0x007292f0 _$E276
// FUNCTION: STBC 0x00729300 _$E75
// FUNCTION: STBC 0x00729310 _$E74
// FUNCTION: STBC 0x00729320 _$E206
// FUNCTION: STBC 0x00729330 _$E205
// FUNCTION: STBC 0x00729340 _$E213
// FUNCTION: STBC 0x00729350 _$E212
// FUNCTION: STBC 0x00729360 _$E217
// FUNCTION: STBC 0x00729370 _$E216
// FUNCTION: STBC 0x00729380 _$E221
// FUNCTION: STBC 0x00729390 _$E220
// FUNCTION: STBC 0x007293a0 _$E225
// FUNCTION: STBC 0x007293b0 _$E224
// FUNCTION: STBC 0x007293c0 _$E231
// FUNCTION: STBC 0x007293d0 _$E230
// FUNCTION: STBC 0x007293e0 _$E281
// FUNCTION: STBC 0x007293f0 _$E280
// FUNCTION: STBC 0x00729400 _$E285
// FUNCTION: STBC 0x00729410 _$E284
// FUNCTION: STBC 0x00729420 _$E289
// FUNCTION: STBC 0x00729430 _$E288
// FUNCTION: STBC 0x00729440 _$E293
// FUNCTION: STBC 0x00729450 _$E292
// FUNCTION: STBC 0x00729460 _$E297
// FUNCTION: STBC 0x00729470 _$E296
// FUNCTION: STBC 0x00729480 _$E301
// FUNCTION: STBC 0x00729490 _$E300
// FUNCTION: STBC 0x007294a0 _$E305
// FUNCTION: STBC 0x007294b0 _$E304
// FUNCTION: STBC 0x007294c0 _$E309
// FUNCTION: STBC 0x007294d0 _$E308
// FUNCTION: STBC 0x007294e0 _$E313
// FUNCTION: STBC 0x007294f0 _$E312
// FUNCTION: STBC 0x00729500 _$E317
// FUNCTION: STBC 0x00729510 _$E316
// FUNCTION: STBC 0x00729520 _$E80
// FUNCTION: STBC 0x00729530 _$E79
// FUNCTION: STBC 0x00729540 _$E321
// FUNCTION: STBC 0x00729550 _$E320
// FUNCTION: STBC 0x00729560 _$E85
// FUNCTION: STBC 0x00729570 _$E84
// FUNCTION: STBC 0x00729580 _$E325
// FUNCTION: STBC 0x00729590 _$E324
// FUNCTION: STBC 0x007295a0 _$E329
// FUNCTION: STBC 0x007295b0 _$E328
// FUNCTION: STBC 0x007295c0 _$E333
// FUNCTION: STBC 0x007295d0 _$E332
// FUNCTION: STBC 0x007295e0 _$E337
// FUNCTION: STBC 0x007295f0 _$E336
// FUNCTION: STBC 0x00729600 _$E94
// FUNCTION: STBC 0x00729610 _$E93
// FUNCTION: STBC 0x00729620 _$E341
// FUNCTION: STBC 0x00729630 _$E340
// FUNCTION: STBC 0x00729640 _$E345
// FUNCTION: STBC 0x00729650 _$E344
// FUNCTION: STBC 0x00729660 _$E349
// FUNCTION: STBC 0x00729670 _$E348
// FUNCTION: STBC 0x00729680 _$E100
// FUNCTION: STBC 0x00729690 _$E99
// FUNCTION: STBC 0x007296a0 _$E105
// FUNCTION: STBC 0x007296b0 _$E104
// FUNCTION: STBC 0x007296c0 _$E135
// FUNCTION: STBC 0x007296d0 _$E134
// FUNCTION: STBC 0x007296e0 _$E353
// FUNCTION: STBC 0x007296f0 _$E352
// FUNCTION: STBC 0x00729700 _$E140
// FUNCTION: STBC 0x00729710 _$E139
// FUNCTION: STBC 0x00729720 _$E357
// FUNCTION: STBC 0x00729730 _$E356
// FUNCTION: STBC 0x00729740 _$E361
// FUNCTION: STBC 0x00729750 _$E360
// FUNCTION: STBC 0x00729760 _$E145
// FUNCTION: STBC 0x00729770 _$E144
// FUNCTION: STBC 0x00729780 _$E365
// FUNCTION: STBC 0x00729790 _$E364
// FUNCTION: STBC 0x007297a0 _$E110
// FUNCTION: STBC 0x007297b0 _$E109
// FUNCTION: STBC 0x007297c0 _$E369
// FUNCTION: STBC 0x007297d0 _$E368
// FUNCTION: STBC 0x007297e0 _$E373
// FUNCTION: STBC 0x007297f0 _$E372
// FUNCTION: STBC 0x00729800 _$E150
// FUNCTION: STBC 0x00729810 _$E149
// FUNCTION: STBC 0x00729820 _$E155
// FUNCTION: STBC 0x00729830 _$E154
// FUNCTION: STBC 0x00729840 _$E377
// FUNCTION: STBC 0x00729850 _$E376
// FUNCTION: STBC 0x00729860 _$E381
// FUNCTION: STBC 0x00729870 _$E380
// FUNCTION: STBC 0x00729880 _$E385
// FUNCTION: STBC 0x00729890 _$E384
// FUNCTION: STBC 0x007298a0 _$E389
// FUNCTION: STBC 0x007298b0 _$E388
// FUNCTION: STBC 0x007298c0 _$E393
// FUNCTION: STBC 0x007298d0 _$E392
// FUNCTION: STBC 0x007298e0 _$E397
// FUNCTION: STBC 0x007298f0 _$E396
// FUNCTION: STBC 0x00729900 _$E401
// FUNCTION: STBC 0x00729910 _$E400
// FUNCTION: STBC 0x00729920 _$E405
// FUNCTION: STBC 0x00729930 _$E404
// FUNCTION: STBC 0x00729940 _$E160
// FUNCTION: STBC 0x00729950 _$E159
// FUNCTION: STBC 0x00729960 _$E409
// FUNCTION: STBC 0x00729970 _$E408
// FUNCTION: STBC 0x00729980 _$E165
// FUNCTION: STBC 0x00729990 _$E164
// FUNCTION: STBC 0x007299a0 _$E170
// FUNCTION: STBC 0x007299b0 _$E169

// The icon manager's constructor and its exit-time destructor.
// FUNCTION: STBC 0x007299c0 _$E438
// FUNCTION: STBC 0x007299d0 _$E435
// FUNCTION: STBC 0x007299e0 _$E437
// FUNCTION: STBC 0x007299f0 _$E436
// DATA: STBC 0x0099d1f0
TGIconManager g_kIconManager;

/*
================
TGIconManager::~TGIconManager
================
*/
// FUNCTION: STBC 0x00729a00
TGIconManager::~TGIconManager( void ) {
}

/*
================
TGIconManager::TGIconManager
================
*/
// FUNCTION: STBC 0x00729b30
TGIconManager::TGIconManager( void ) : registrations( 1, 1 ), iconGroups( 2 ), polyBlocks( 2 ), unknown6C( 37 ) {
	freeHead = NULL;
	freeTail = NULL;
	camera = NULL;
	scale = 0.0f;
	screenWidth = 0.0f;
	screenHeight = 0.0f;
	pixelCenterAdj = -0.5f;
	screenCenterAdjX = 0;
	screenCenterAdjY = 0;
}

/*
================
TGIconManager::Shutdown
================
*/
// FUNCTION: STBC 0x00729c40
void TGIconManager::Shutdown( void ) {
	camera = NULL;
	for ( unsigned int i = 0; i < registrations.GetSize(); i++ ) {
		delete registrations.GetAt( i );
	}
	registrations.RemoveAll();
	while ( iconGroups.GetSize() != 0 ) {
		delete iconGroups.RemoveHead();
	}
	while ( polyBlocks.GetSize() != 0 ) {
		delete [] polyBlocks.RemoveHead();
	}
}

/*
================
TGIconManager::SetDisplayInfo

Draws with the camera's renderer at its screen size; a NULL camera keeps
the current one.
================
*/
// FUNCTION: STBC 0x00729dc0
void TGIconManager::SetDisplayInfo( NiCamera *camera_, float scale_ ) {
	if ( camera_ != NULL ) {
		camera = camera_;
		NiRender *renderer = camera->GetRenderer();
		screenWidth = (float)renderer->VirtualSlot19();
		screenHeight = (float)renderer->VirtualSlot20();
		SetPixelCenterAdj( GetPixelCenterAdj() );
	}
	scale = scale_;
}

/*
================
TGIconManager::RegisterIconGroup

Files the script function that creates the group with the name.
================
*/
// FUNCTION: STBC 0x00729e70 ?RegisterIconGroup@TGIconManager@@QAEXPAD00@Z
void TGIconManager::RegisterIconGroup( char *name, char *module, char *function ) {
	registrations.Add( new TGIconGroupRegistration( name, module, function ) );
}

/*
================
TGIconManager::CreateIconGroup

A new, empty group of the name.
================
*/
// FUNCTION: STBC 0x00729fa0
TGIconGroup *TGIconManager::CreateIconGroup( char *name ) {
	return new TGIconGroup( name );
}

/*
================
TGIconManager::AddIconGroup

Loads the group, unless one of its name is loaded already.
================
*/
// FUNCTION: STBC 0x0072a000
void TGIconManager::AddIconGroup( TGIconGroup *iconGroup ) {
	if ( FindIconGroup( iconGroup->GetName() ) == NULL ) {
		iconGroups.AddTail( iconGroup );
	}
}

/*
================
TGIconManager::Purge

Unloads the groups nothing uses.
================
*/
// FUNCTION: STBC 0x0072a080
void TGIconManager::Purge( void ) {
	NiTListIterator pos = iconGroups.GetHeadPos();
	while ( pos != NULL ) {
		TGIconGroup *iconGroup = iconGroups.Get( pos );
		if ( iconGroup->GetRefCount() == 0 ) {
			iconGroups.RemovePos( pos );
			delete iconGroup;
		} else {
			iconGroups.GetNext( pos );	// steps past the kept group
		}
	}
}

/*
================
TGIconManager::GetIconGroup

The loaded group of the name; a registered group is created by its script
function first when it is not loaded yet. NULL when it is none.
================
*/
// FUNCTION: STBC 0x0072a150
TGIconGroup *TGIconManager::GetIconGroup( char *name ) {
	for ( unsigned int i = 0; i < registrations.GetSize(); i++ ) {
		TGIconGroupRegistration *registration = registrations.GetAt( i );
		if ( strcmp( name, registration->name ) == 0 ) {
			TGIconGroup *iconGroup = FindIconGroup( name );
			if ( iconGroup == NULL ) {
				PyEmbed::CallFunction( registration->module, registration->function, NULL, NULL, NULL, NULL );
				iconGroup = FindIconGroup( name );
				if ( iconGroup == NULL ) {
					return NULL;
				}
			}
			return iconGroup;
		}
	}
	return NULL;
}

/*
================
TGIconManager::DeleteIconGroup

Deletes the named group and its registration, after the root window has
dropped its batches; false when there is no such group.
================
*/
// FUNCTION: STBC 0x0072a210
// PARKED: inline budget: NiTArray::Compact's debug-proven text (AnimOnly.exe 0x4D9B50) costs 279, leaving NiTList::RemovePos's RemoveTail a share of 94 against its cost 100 (6 units short; retail expands it). Accepted loss, user's ruling 2026-11-20
bool TGIconManager::DeleteIconGroup( char *name ) {
	TGIconGroup *iconGroup = GetIconGroup( name );
	if ( iconGroup == NULL ) {
		return false;
	}
	g_kRootWindow.VirtualSlot60();
	for ( unsigned int i = 0; i < registrations.GetSize(); i++ ) {
		TGIconGroupRegistration *registration = registrations.GetAt( i );
		if ( strcmp( name, registration->name ) == 0 ) {
			registrations.RemoveAt( i );
			registrations.Compact();
			delete registration;
			break;
		}
	}
	iconGroups.Remove( iconGroup );
	delete iconGroup;
	return true;
}

/*
================
TGIconManager::FindIconGroup
================
*/
// FUNCTION: STBC 0x0072a400
TGIconGroup *TGIconManager::FindIconGroup( char *name ) {
	NiTListIterator pos = iconGroups.GetHeadPos();
	while ( pos != NULL ) {
		TGIconGroup *iconGroup = iconGroups.GetNext( pos );
		if ( strcmp( name, iconGroup->GetName() ) == 0 ) {
			return iconGroup;
		}
	}
	return NULL;
}

/*
================
TGIconManager::CreateIconPoly

A poly for an icon of the named group in rect, in the batch of the list
that draws from the icon's texture (a new one when none does); NULL when
the group or the icon is missing or there is no screen to draw on.
================
*/
// FUNCTION: STBC 0x0072a470
TGIconPoly *TGIconManager::CreateIconPoly( TGUIBatch **batches, char *groupName, int iconNum, TGRect *rect, NiColorA *color, int clamp ) {
	NiColorA white[4] = {
		NiColorA( 1.0f, 1.0f, 1.0f, 1.0f ),
		NiColorA( 1.0f, 1.0f, 1.0f, 1.0f ),
		NiColorA( 1.0f, 1.0f, 1.0f, 1.0f ),
		NiColorA( 1.0f, 1.0f, 1.0f, 1.0f )
	};
	NiColorA *polyColor = color;
	TGIconGroup *iconGroup = GetIconGroup( groupName );
	TGIconLocation *location;
	if ( iconGroup == NULL || camera == NULL || screenWidth <= 0.0f || screenHeight <= 0.0f ) {
		return NULL;
	}
	if ( !iconGroup->icons.GetAt( iconNum, location ) || location == NULL ) {
		return NULL;
	}
	if ( polyColor == NULL ) {
		polyColor = white;
	}
	NiPoint2 textureCoords[4];
	for ( int i = 0; i < 4; i++ ) {
		textureCoords[i].x = location->corners[i][0];
		textureCoords[i].y = location->corners[i][1];
	}
	TGUIBatch *batch = Unknown72A940( *batches, location->texture );
	if ( batch == NULL ) {
		batch = Unknown72A880( batches, location->texture, clamp );
		if ( batch == NULL ) {
			return NULL;
		}
	}
	return batch->AddIconPoly( rect, textureCoords, polyColor );
}

/*
================
TGIconManager::ReloadIconGroups

Empties every registered group and has its script function fill it again,
for the current display mode, then lays the root window out again.
================
*/
// FUNCTION: STBC 0x0072a600
void TGIconManager::ReloadIconGroups( void ) {
	g_kRootWindow.VirtualSlot60();
	unsigned int count = registrations.GetSize();
	for ( unsigned int i = 0; i < count; i++ ) {
		TGIconGroupRegistration *registration = registrations.GetAt( i );
		TGIconGroup *iconGroup = GetIconGroup( registration->name );
		iconGroup->Reset();
		PyObject *wrapper = iconGroup->GetPyWrapper();
		PyEmbed::CallFunction( registration->module, registration->function, NULL, NULL, "(O)", wrapper );
		Py_DECREF( wrapper );
	}
	g_kRootWindow.Layout();
}

/*
================
TGIconManager::Draw

Draws the root window's batches with the camera's renderer.
================
*/
// FUNCTION: STBC 0x0072a690
void TGIconManager::Draw( void ) {
	if ( camera != NULL ) {
		NiRender *renderer = camera->GetRenderer();
		if ( renderer != NULL ) {
			renderer->VirtualSlot38( camera );
			renderer->VirtualSlot43( true );
			g_kRootWindow.VirtualSlot63( renderer );
			renderer->VirtualSlot44();
			renderer->VirtualSlot39();
		}
	}
}

/*
================
TGIconManager::AllocIconPoly
================
*/
// FUNCTION: STBC 0x0072a6e0
TGIconPoly *TGIconManager::AllocIconPoly( void ) {
	if ( freeHead == NULL ) {
		TGIconPoly *block = new TGIconPoly[ICONPOLY_BLOCK_SIZE];
		for ( int i = 0; i < ICONPOLY_BLOCK_SIZE; i++ ) {
			FreeIconPoly( &block[i] );
		}
		polyBlocks.AddTail( block );
	}
	TGIconPoly *poly = freeHead;
	freeHead = poly->nextFree;
	if ( poly == freeTail ) {
		freeTail = NULL;
	}
	return poly;
}

/*
================
TGIconManager::FreeIconPoly
================
*/
// FUNCTION: STBC 0x0072a850
void TGIconManager::FreeIconPoly( TGIconPoly *poly ) {
	poly->index = ICONPOLY_NO_INDEX;
	poly->nextFree = NULL;
	if ( freeTail == NULL ) {
		freeHead = poly;
		freeTail = poly;
	} else {
		freeTail->nextFree = poly;
		freeTail = poly;
	}
}

/*
================
TGIconManager::Unknown72A880

Appends a new batch for the texture to the list.
================
*/
// FUNCTION: STBC 0x0072a880
TGUIBatch *TGIconManager::Unknown72A880( TGUIBatch **batches, NiTextureProperty *texture, int clamp ) {
	if ( batches == NULL ) {
		return NULL;
	}
	TGUIBatch *batch = new TGUIBatch( texture, clamp );
	if ( *batches == NULL ) {
		*batches = batch;
	} else {
		TGUIBatch *last = *batches;
		while ( last->next != NULL ) {
			last = last->next;
		}
		last->next = batch;
	}
	return batch;
}

/*
================
TGIconManager::SetPixelCenterAdj

The offset that puts the icons' texels on the screen's pixel centres, in
pixels; it is kept in screen units as well.
================
*/
// FUNCTION: STBC 0x0072a920
void TGIconManager::SetPixelCenterAdj( float adj ) {
	pixelCenterAdj = adj;
	screenCenterAdjX = adj / GetScreenWidth();
	screenCenterAdjY = adj / GetScreenHeight();
}

/*
================
TGIconManager::Unknown72A940

The batch of the list that draws from the texture's image.
================
*/
// FUNCTION: STBC 0x0072a940
TGUIBatch *TGIconManager::Unknown72A940( TGUIBatch *batch, NiTextureProperty *texture ) {
	while ( batch != NULL && batch->texture->GetImage() != texture->GetImage() ) {
		batch = batch->next;
	}
	return batch;
}

// The standard library's locale facet id (<iostream>): a test-and-set initialiser and its exit-time destructor.
// FUNCTION: STBC 0x0072b140 _$E447
// FUNCTION: STBC 0x0072b160 _$E446

/*
================
TGIconManager::WriteToStream
================
*/
// FUNCTION: STBC 0x0072a960
bool TGIconManager::WriteToStream( TGStream *stream ) {
	stream->WriteLong( registrations.GetSize() );
	for ( unsigned int i = 0; i < registrations.GetSize(); i++ ) {
		TGIconGroupRegistration *registration = registrations.GetAt( i );
		if ( registration != NULL ) {
			registration->WriteToStream( stream );
		}
	}
	stream->WriteLong( iconGroups.GetSize() );
	NiTListIterator pos = iconGroups.GetHeadPos();
	while ( pos ) {
		TGIconGroup *iconGroup = iconGroups.GetNext( pos );
		if ( iconGroup != NULL ) {
			iconGroup->WriteToStream( stream );
		}
	}
	stream->WriteFloat( scale );
	stream->WriteFloat( screenWidth );
	stream->WriteFloat( screenHeight );
	return true;
}

/*
================
TGIconManager::ReadFromStream
================
*/
// FUNCTION: STBC 0x0072a9f0
bool TGIconManager::ReadFromStream( TGStream *stream ) {
	long count = stream->ReadLong();
	for ( ; count > 0; count-- ) {
		TGIconGroupRegistration *registration = new TGIconGroupRegistration;
		registration->ReadFromStream( stream );
		registrations.Add( registration );
	}
	count = stream->ReadLong();
	for ( ; count > 0; count-- ) {
		TGIconGroup *iconGroup = new TGIconGroup;
		iconGroup->ReadFromStream( stream );
		iconGroups.AddTail( iconGroup );
	}
	scale = stream->ReadFloat();
	screenWidth = stream->ReadFloat();
	screenHeight = stream->ReadFloat();
	return true;
}

/*
================
TGIconManager::ForgetAll

Forgets the registrations, the icon groups and the poly blocks without deleting
them.
================
*/
// FUNCTION: STBC 0x0072acd0
void TGIconManager::ForgetAll( void ) {
	registrations.RemoveAll();
	iconGroups.RemoveAll();
	polyBlocks.RemoveAll();
	freeHead = NULL;
	freeTail = NULL;
}
