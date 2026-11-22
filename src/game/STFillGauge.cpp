/*
===============================================================================

	STFillGauge.cpp

===============================================================================
*/

#include "STFillGauge.h"
#include "ShipSubsystem.h"
#include "PowerSubsystem.h"
#include "GraphicsModeInfo.h"
#include "TGIcon.h"
#include "TGIconGroup.h"
#include "TGIconManager.h"
#include "TGIconPoly.h"
#include "TGUIBatch.h"
#include "TGEventHandlerTable.h"
#include "TGStream.h"
#include "PyEmbed.h"
#include "NiTriShape.h"
#include <math.h>
#include <string.h>
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"
#include <iostream>
#include "NiMain.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x005233c0 _$E33
// FUNCTION: STBC 0x005233d0 _$E32
// FUNCTION: STBC 0x005233e0 _$E37
// FUNCTION: STBC 0x005233f0 _$E36
// FUNCTION: STBC 0x00523400 _$E48
// FUNCTION: STBC 0x00523410 _$E47
// FUNCTION: STBC 0x00523420 _$E52
// FUNCTION: STBC 0x00523430 _$E51
// FUNCTION: STBC 0x00523440 _$E56
// FUNCTION: STBC 0x00523450 _$E55
// FUNCTION: STBC 0x00523460 _$E60
// FUNCTION: STBC 0x00523470 _$E59
// FUNCTION: STBC 0x00523480 _$E184
// FUNCTION: STBC 0x00523490 _$E183
// FUNCTION: STBC 0x005234a0 _$E126
// FUNCTION: STBC 0x005234b0 _$E125
// FUNCTION: STBC 0x005234c0 _$E139
// FUNCTION: STBC 0x005234d0 _$E138
// FUNCTION: STBC 0x005234e0 _$E189
// FUNCTION: STBC 0x005234f0 _$E188
// FUNCTION: STBC 0x00523500 _$E194
// FUNCTION: STBC 0x00523510 _$E193
// FUNCTION: STBC 0x00523520 _$E198
// FUNCTION: STBC 0x00523530 _$E197
// FUNCTION: STBC 0x00523540 _$E202
// FUNCTION: STBC 0x00523550 _$E201
// FUNCTION: STBC 0x00523560 _$E209
// FUNCTION: STBC 0x00523570 _$E208
// FUNCTION: STBC 0x00523580 _$E329
// FUNCTION: STBC 0x00523590 _$E328
// FUNCTION: STBC 0x005235a0 _$E333
// FUNCTION: STBC 0x005235b0 _$E332
// FUNCTION: STBC 0x005235c0 _$E84
// FUNCTION: STBC 0x005235d0 _$E83
// FUNCTION: STBC 0x005235e0 _$E250
// FUNCTION: STBC 0x005235f0 _$E249
// FUNCTION: STBC 0x00523600 _$E257
// FUNCTION: STBC 0x00523610 _$E256
// FUNCTION: STBC 0x00523620 _$E261
// FUNCTION: STBC 0x00523630 _$E260
// FUNCTION: STBC 0x00523640 _$E265
// FUNCTION: STBC 0x00523650 _$E264
// FUNCTION: STBC 0x00523660 _$E269
// FUNCTION: STBC 0x00523670 _$E268
// FUNCTION: STBC 0x00523680 _$E275
// FUNCTION: STBC 0x00523690 _$E274
// FUNCTION: STBC 0x005236a0 _$E337
// FUNCTION: STBC 0x005236b0 _$E336
// FUNCTION: STBC 0x005236c0 _$E341
// FUNCTION: STBC 0x005236d0 _$E340
// FUNCTION: STBC 0x005236e0 _$E345
// FUNCTION: STBC 0x005236f0 _$E344
// FUNCTION: STBC 0x00523700 _$E349
// FUNCTION: STBC 0x00523710 _$E348
// FUNCTION: STBC 0x00523720 _$E353
// FUNCTION: STBC 0x00523730 _$E352
// FUNCTION: STBC 0x00523740 _$E357
// FUNCTION: STBC 0x00523750 _$E356
// FUNCTION: STBC 0x00523760 _$E361
// FUNCTION: STBC 0x00523770 _$E360
// FUNCTION: STBC 0x00523780 _$E365
// FUNCTION: STBC 0x00523790 _$E364
// FUNCTION: STBC 0x005237a0 _$E369
// FUNCTION: STBC 0x005237b0 _$E368
// FUNCTION: STBC 0x005237c0 _$E373
// FUNCTION: STBC 0x005237d0 _$E372
// FUNCTION: STBC 0x005237e0 _$E89
// FUNCTION: STBC 0x005237f0 _$E88
// FUNCTION: STBC 0x00523800 _$E377
// FUNCTION: STBC 0x00523810 _$E376
// FUNCTION: STBC 0x00523820 _$E94
// FUNCTION: STBC 0x00523830 _$E93
// FUNCTION: STBC 0x00523840 _$E381
// FUNCTION: STBC 0x00523850 _$E380
// FUNCTION: STBC 0x00523860 _$E385
// FUNCTION: STBC 0x00523870 _$E384
// FUNCTION: STBC 0x00523880 _$E389
// FUNCTION: STBC 0x00523890 _$E388
// FUNCTION: STBC 0x005238a0 _$E393
// FUNCTION: STBC 0x005238b0 _$E392
// FUNCTION: STBC 0x005238c0 _$E103
// FUNCTION: STBC 0x005238d0 _$E102
// FUNCTION: STBC 0x005238e0 _$E397
// FUNCTION: STBC 0x005238f0 _$E396
// FUNCTION: STBC 0x00523900 _$E401
// FUNCTION: STBC 0x00523910 _$E400
// FUNCTION: STBC 0x00523920 _$E405
// FUNCTION: STBC 0x00523930 _$E404
// FUNCTION: STBC 0x00523940 _$E109
// FUNCTION: STBC 0x00523950 _$E108
// FUNCTION: STBC 0x00523a20 _$E114
// FUNCTION: STBC 0x00523a30 _$E113
// FUNCTION: STBC 0x00523a70 _$E144
// FUNCTION: STBC 0x00523a80 _$E143
// FUNCTION: STBC 0x00523a90 _$E409
// FUNCTION: STBC 0x00523aa0 _$E408
// FUNCTION: STBC 0x00523ab0 _$E149
// FUNCTION: STBC 0x00523ac0 _$E148
// FUNCTION: STBC 0x00523ad0 _$E413
// FUNCTION: STBC 0x00523ae0 _$E412
// FUNCTION: STBC 0x00523af0 _$E417
// FUNCTION: STBC 0x00523b00 _$E416
// FUNCTION: STBC 0x00523b10 _$E154
// FUNCTION: STBC 0x00523b20 _$E153
// FUNCTION: STBC 0x00523b30 _$E421
// FUNCTION: STBC 0x00523b40 _$E420
// FUNCTION: STBC 0x00523b50 _$E119
// FUNCTION: STBC 0x00523b60 _$E118
// FUNCTION: STBC 0x00523b70 _$E425
// FUNCTION: STBC 0x00523b80 _$E424
// FUNCTION: STBC 0x00523b90 _$E429
// FUNCTION: STBC 0x00523ba0 _$E428
// FUNCTION: STBC 0x00523bb0 _$E159
// FUNCTION: STBC 0x00523bc0 _$E158
// FUNCTION: STBC 0x00523bd0 _$E164
// FUNCTION: STBC 0x00523be0 _$E163
// FUNCTION: STBC 0x00523bf0 _$E433
// FUNCTION: STBC 0x00523c00 _$E432
// FUNCTION: STBC 0x00523c10 _$E437
// FUNCTION: STBC 0x00523c20 _$E436
// FUNCTION: STBC 0x00523c30 _$E441
// FUNCTION: STBC 0x00523c40 _$E440
// FUNCTION: STBC 0x00523c50 _$E445
// FUNCTION: STBC 0x00523c60 _$E444
// FUNCTION: STBC 0x00523c70 _$E449
// FUNCTION: STBC 0x00523c80 _$E448
// FUNCTION: STBC 0x00523c90 _$E453
// FUNCTION: STBC 0x00523ca0 _$E452
// FUNCTION: STBC 0x00523cb0 _$E457
// FUNCTION: STBC 0x00523cc0 _$E456
// FUNCTION: STBC 0x00523cd0 _$E461
// FUNCTION: STBC 0x00523ce0 _$E460
// FUNCTION: STBC 0x00523cf0 _$E169
// FUNCTION: STBC 0x00523d00 _$E168
// FUNCTION: STBC 0x00523d10 _$E465
// FUNCTION: STBC 0x00523d20 _$E464
// FUNCTION: STBC 0x00523d30 _$E174
// FUNCTION: STBC 0x00523d40 _$E173
// FUNCTION: STBC 0x00523d50 _$E179
// FUNCTION: STBC 0x00523d60 _$E178
// FUNCTION: STBC 0x00523d70 _$E299
// FUNCTION: STBC 0x00523d80 _$E298
// FUNCTION: STBC 0x00523d90 _$E304
// FUNCTION: STBC 0x00523da0 _$E303
// FUNCTION: STBC 0x00523db0 _$E308
// FUNCTION: STBC 0x00523dc0 _$E307

// Further static initialisers of the unit.
// FUNCTION: STBC 0x00521c30 _$E486
// FUNCTION: STBC 0x00521c50 _$E485
// Further static initialisers of the unit.

// The functions a saved gauge names by their place; the first place is
// none.
// DATA: STBC 0x008e2e78
GaugeValueFunction STFillGauge::functions[STFILLGAUGE_NUM_FUNCTIONS] = {
	NULL,
	(GaugeValueFunction)&ShipSubsystem::GetCondition,
	(GaugeValueFunction)&ShipSubsystem::GetMaxCondition,
	(GaugeValueFunction)&ShipSubsystem::GetCombinedConditionPercentage,
	(GaugeValueFunction)&ShipSubsystem::GetConditionPercentage,
	(GaugeValueFunction)&ShipSubsystem::IsDisabled,
	(GaugeValueFunction)&PowerSubsystem::GetMainBatteryPower,
	(GaugeValueFunction)&PowerSubsystem::GetMainBatteryLimit,
	(GaugeValueFunction)&PowerSubsystem::GetBackupBatteryPower,
	(GaugeValueFunction)&PowerSubsystem::GetBackupBatteryLimit,
	(GaugeValueFunction)&PowerSubsystem::GetMainConduitCapacity,
	(GaugeValueFunction)&PowerSubsystem::GetMaxMainConduitCapacity
};

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00523340 _$E313
// FUNCTION: STBC 0x00523350 _$E310
// FUNCTION: STBC 0x00523360 _$E312
// FUNCTION: STBC 0x00523370 _$E311
// FUNCTION: STBC 0x00523380 _$E319
// FUNCTION: STBC 0x00523390 _$E316
// FUNCTION: STBC 0x005233a0 _$E318
// FUNCTION: STBC 0x005233b0 _$E317

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x00522020 _$E471
// FUNCTION: STBC 0x00522030 _$E467
// FUNCTION: STBC 0x00522040 _$E470
// FUNCTION: STBC 0x00522050 _$E468
// DATA: STBC 0x00989030
TGEventHandlerTable STFillGauge::classHandlers;

/*
================
STFillGauge::Cast

The object as a fill gauge; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00522070
STFillGauge *STFillGauge::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( STFILLGAUGE_TYPE ) ) {
		return (STFillGauge *)object;
	}
	return NULL;
}

// The gauge list's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x005220a0 _$E477
// FUNCTION: STBC 0x005220b0 _$E473
// FUNCTION: STBC 0x005220e0 _$E476
// FUNCTION: STBC 0x005220f0 _$E474

// DATA: STBC 0x00988fa0
NiTList<STFillGauge *> STFillGauge::gauges( 2 );

/*
================
STFillGauge::STFillGauge

Builds the fill and the empty icon, both the icon of the group (the LCARS
group of the current resolution without one).
================
*/
// FUNCTION: STBC 0x00522160 ??0STFillGauge@@QAE@HPAD_N@Z
STFillGauge::STFillGauge( int iconNum, char *iconGroupName, bool tileIcons ) {
	Init();
	this->tileIcons = tileIcons;
	char *lcarsGroupName = GraphicsModeInfo::GetCurrentMode()->GetLcarsString();
	if ( iconGroupName == NULL ) {
		iconGroupName = lcarsGroupName;
	}
	AddChild( new TGIcon( iconGroupName, iconNum, &fillColor ), 0.0f, 0.0f, false );
	AddChild( new TGIcon( iconGroupName, iconNum, &emptyColor ), 0.0f, 0.0f, false );
}

/*
================
STFillGauge::STFillGauge

The gauge a stream loads; its icons come from the stream.
================
*/
// FUNCTION: STBC 0x00522380 ??0STFillGauge@@QAE@J@Z
STFillGauge::STFillGauge( long id ) : TGPane( id ) {
	Init();
}

/*
================
STFillGauge::ClearSource

Forgets the object and the functions the value comes from, and takes the
default colours.
================
*/
inline void STFillGauge::ClearSource( void ) {
	pythonModule = NULL;
	pythonValueFunction = NULL;
	pythonMaxFunction = NULL;
	pythonAlternateFunction = NULL;
	valueFunction = NULL;
	maxFunction = NULL;
	alternateFunction = NULL;
	objectID = 0;
	fillColor = NiColorA::WHITE;
	emptyColor = NiColorA::BLACK;
}

/*
================
STFillGauge::Init

The members both constructors set up; the gauge joins the list of gauges.
================
*/
// FUNCTION: STBC 0x00522410
void STFillGauge::Init( void ) {
	ClearSource();
	fillDirection = LEFT_TO_RIGHT;
	tileIcons = false;
	gauges.AddTail( this );
	SetBatchChildPolys( false );
	SetNoFocus();
}

/*
================
STFillGauge::~STFillGauge
================
*/
// FUNCTION: STBC 0x00522510
STFillGauge::~STFillGauge( void ) {
	if ( pythonModule != NULL ) {
		delete[] pythonModule;
		pythonModule = NULL;
	}
	if ( pythonValueFunction != NULL ) {
		delete[] pythonValueFunction;
		pythonValueFunction = NULL;
	}
	if ( pythonMaxFunction != NULL ) {
		delete[] pythonMaxFunction;
		pythonMaxFunction = NULL;
	}
	// BUG: the alternate function's name is never freed.
	gauges.Remove( this );
}

/*
================
STFillGauge::SetPythonMethods

Takes copies of the names; a NULL name is left unset.
================
*/
// FUNCTION: STBC 0x00522630
void STFillGauge::SetPythonMethods( char *module, char *valueFunction, char *maxFunction, char *alternateFunction ) {
	if ( pythonModule != NULL ) {
		delete[] pythonModule;
		pythonModule = NULL;
	}
	if ( pythonValueFunction != NULL ) {
		delete[] pythonValueFunction;
		pythonValueFunction = NULL;
	}
	if ( pythonMaxFunction != NULL ) {
		delete[] pythonMaxFunction;
		pythonMaxFunction = NULL;
	}
	if ( pythonAlternateFunction != NULL ) {
		delete[] pythonAlternateFunction;
		pythonAlternateFunction = NULL;
	}
	if ( module != NULL ) {
		pythonModule = new char[strlen( module ) + 1];
		strcpy( pythonModule, module );
	}
	if ( valueFunction != NULL ) {
		pythonValueFunction = new char[strlen( valueFunction ) + 1];
		strcpy( pythonValueFunction, valueFunction );
	}
	if ( maxFunction != NULL ) {
		pythonMaxFunction = new char[strlen( maxFunction ) + 1];
		strcpy( pythonMaxFunction, maxFunction );
	}
	if ( alternateFunction != NULL ) {
		pythonAlternateFunction = new char[strlen( alternateFunction ) + 1];
		strcpy( pythonAlternateFunction, alternateFunction );
	}
}

/*
================
STFillGauge::SetFillColor
================
*/
// FUNCTION: STBC 0x00522790
void STFillGauge::SetFillColor( NiColorA color ) {
	fillColor = color;
	TGIcon *fill = (TGIcon *)GetNthChild( 0 );
	fill->SetColor( &fillColor );
}

/*
================
STFillGauge::SetEmptyColor
================
*/
// FUNCTION: STBC 0x005227d0
void STFillGauge::SetEmptyColor( NiColorA color ) {
	emptyColor = color;
	TGIcon *empty = (TGIcon *)GetNthChild( 1 );
	empty->SetColor( &emptyColor );
}

/*
================
STFillGauge::SetAlternateColor
================
*/
// FUNCTION: STBC 0x00522810
void STFillGauge::SetAlternateColor( NiColorA color ) {
	alternateColor = color;
}

/*
================
STFillGauge::Layout

When the icons tile, sets their texture coordinates so that the art repeats
along the gauge rather than stretching: the empty icon shows whole copies of
the art, and the fill the part of them its share covers.
================
*/
// FUNCTION: STBC 0x00522840
void STFillGauge::Layout( void ) {
	TGPane::Layout();
	if ( tileIcons == true ) {
		TGIcon *fill = TGIcon::Cast( GetNthChild( 0 ) );
		TGIcon *empty = TGIcon::Cast( GetNthChild( 1 ) );
		if ( fill != NULL && empty != NULL ) {
			TGIconGroup *iconGroup = g_kIconManager.GetIconGroup( fill->GetIconGroupName() );
			if ( iconGroup != NULL ) {
				TGIconPoly *fillPoly = fill->GetIconPoly();
				TGIconPoly *emptyPoly = empty->GetIconPoly();
				NiPoint2 *fillCoords = NULL;
				NiPoint2 *emptyCoords = NULL;
				if ( fillPoly != NULL ) {
					fillCoords = fillPoly->GetTextureCoords();
				}
				if ( emptyPoly != NULL ) {
					emptyCoords = emptyPoly->GetTextureCoords();
				}
				float emptyWidth = empty->GetWidth();
				float repeatU = (float)floor( emptyWidth / iconGroup->GetIconScreenWidth( empty->GetIconNum() ) );
				float emptyHeight = empty->GetHeight();
				float repeatV = (float)floor( emptyHeight / iconGroup->GetIconScreenHeight( empty->GetIconNum() ) );

				switch ( fillDirection ) {
					case LEFT_TO_RIGHT: {
						float share = fill->GetWidth() / empty->GetWidth();
						if ( emptyCoords != NULL ) {
							emptyCoords[3].x = emptyCoords[2].x = repeatU;
						}
						if ( fillCoords != NULL ) {
							float u = share * repeatU;
							fillCoords[2].x = RoundToPixels( u, 0 );
							fillCoords[3].x = RoundToPixels( u, 0 );
						}
						break;
					}
					case RIGHT_TO_LEFT: {
						float share = fill->GetWidth() / empty->GetWidth();
						if ( emptyCoords != NULL ) {
							emptyCoords[1].x = emptyCoords[0].x = -repeatU;
							emptyCoords[3].x = emptyCoords[2].x = 0.0f;
						}
						if ( fillCoords != NULL ) {
							float u = -( share * repeatU );
							fillCoords[0].x = RoundToPixels( u, 0 );
							fillCoords[1].x = RoundToPixels( u, 0 );
							fillCoords[2].x = RoundToPixels( 0.0f, 0 );
							fillCoords[3].x = RoundToPixels( 0.0f, 0 );
						}
						break;
					}
					case BOTTOM_TO_TOP: {
						float share = fill->GetHeight() / empty->GetHeight();
						if ( emptyCoords != NULL ) {
							emptyCoords[0].y = 0.0f;
							emptyCoords[1].y = repeatV;
							emptyCoords[2].y = repeatV;
							emptyCoords[3].y = 0.0f;
						}
						if ( fillCoords != NULL ) {
							float v = 1.0f - share * repeatV;
							fillCoords[0].y = RoundToPixels( v, 1 );
							fillCoords[1].y = RoundToPixels( 1.0f, 1 );
							fillCoords[2].y = RoundToPixels( 1.0f, 1 );
							fillCoords[3].y = RoundToPixels( v, 1 );
						}
						break;
					}
					case TOP_TO_BOTTOM: {
						float share = fill->GetHeight() / empty->GetHeight();
						if ( emptyCoords != NULL ) {
							emptyCoords[2].y = emptyCoords[1].y = repeatV;
						}
						if ( fillCoords != NULL ) {
							float v = share * repeatV;
							fillCoords[1].y = RoundToPixels( v, 1 );
							fillCoords[2].y = RoundToPixels( v, 1 );
						}
						break;
					}
				}

				if ( fillPoly != NULL ) {
					fillPoly->batch->shape->GetShapeData()->IncRevisionID();
				}
				if ( emptyPoly != NULL ) {
					emptyPoly->batch->shape->GetShapeData()->IncRevisionID();
				}
			}
		}
	}
}

/*
================
STFillGauge::RoundToPixels
================
*/
// FUNCTION: STBC 0x00522b30
float STFillGauge::RoundToPixels( float coord, int axis ) {
	float screenWidth = g_kIconManager.GetScreenWidth();
	float screenHeight = g_kIconManager.GetScreenHeight();
	switch ( axis ) {
		case 0: {
			float pixels = (float)(int)( screenWidth * coord + 0.5f );
			return pixels / screenWidth;
		}
		case 1: {
			float pixels = (float)(int)( screenHeight * coord + 0.5f );
			return pixels / screenHeight;
		}
	}
	return 0.0f;
}

/*
================
STFillGauge::UpdateGauge

Reads the value, its maximum and the alternate flag from the Python
functions when there are any, else from the object's functions, then sizes
the fill and the empty icon and colours the fill.
================
*/
// FUNCTION: STBC 0x00522ba0
void STFillGauge::UpdateGauge( void ) {
	if ( IsCompletelyVisible() ) {
		float value = 0.0f;
		float maxValue = 1.0f;
		int alternate = 0;
		TGObject *object = TGObject::GetTGObjectPtr( objectID );
		if ( pythonModule != NULL && pythonValueFunction != NULL ) {
			PyObject *pyObject = Py_None;
			if ( object != NULL ) {
				pyObject = object->VirtualSlot08();
			} else {
				Py_INCREF( Py_None );
			}
			PyEmbed::CallFunction( pythonModule, pythonValueFunction, "f", &value, "(O)", pyObject );
			PyEmbed::CallFunction( pythonModule, pythonMaxFunction, "f", &maxValue, "(O)", pyObject );
			if ( pythonAlternateFunction != NULL ) {
				PyEmbed::CallFunction( pythonModule, pythonAlternateFunction, "i", &alternate, "(O)", pyObject );
			}
			Py_DECREF( pyObject );
		} else if ( object != NULL ) {
			value = ( object->*valueFunction )();
			if ( maxFunction != NULL ) {
				maxValue = ( object->*maxFunction )();
			} else {
				maxValue = 1.0f;
			}
			if ( alternateFunction != NULL ) {
				alternate = ( object->*alternateFunction )() == true;
			}
		}

		TGIcon *fill = (TGIcon *)GetNthChild( 0 );
		TGIcon *empty = (TGIcon *)GetNthChild( 1 );
		float width = GetWidth();
		float height = GetHeight();
		empty->Resize( width, height, false );
		float share = value / ( maxValue != 0.0f ? maxValue : 1.0f );
		float fillWidth = share * width;
		if ( fillDirection == LEFT_TO_RIGHT || fillDirection == RIGHT_TO_LEFT ) {
			fill->Resize( fillWidth, height, false );
			empty->Resize( width - fill->GetWidth(), height, false );
		} else {
			float fillHeight = share * height;
			fill->Resize( width, fillHeight, false );
			empty->Resize( width, height - fill->GetHeight(), false );
		}

		alignment_t fillAlign = ALIGN_UL;
		alignment_t emptyAlign = ALIGN_UR;
		switch ( fillDirection ) {
			case RIGHT_TO_LEFT:
				fillAlign = ALIGN_UR;
				emptyAlign = ALIGN_UL;
				break;
			case BOTTOM_TO_TOP:
				emptyAlign = ALIGN_UR;
				// BUG: falls through, so a top-to-bottom gauge also keeps its fill at the bottom (Layout tiles it from the top)
			case TOP_TO_BOTTOM:
				fillAlign = ALIGN_BR;
				break;
		}
		fill->AlignTo( NULL, fillAlign, ALIGN_UL, false );
		empty->AlignTo( NULL, emptyAlign, ALIGN_UL, false );

		if ( alternate == 1 ) {
			fill->SetColor( &alternateColor );
		} else {
			fill->SetColor( &fillColor );
		}
		Layout();
	}
}

/*
================
STFillGauge::WriteToStream

The object functions are saved by their place in the table; the Python
names only when there are no object functions.
================
*/
// FUNCTION: STBC 0x00522e20
bool STFillGauge::WriteToStream( TGStream *stream ) {
	if ( !TGPane::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteID( objectID );
	stream->WriteFloat( fillColor.r );
	stream->WriteFloat( fillColor.g );
	stream->WriteFloat( fillColor.b );
	stream->WriteFloat( fillColor.a );
	stream->WriteFloat( emptyColor.r );
	stream->WriteFloat( emptyColor.g );
	stream->WriteFloat( emptyColor.b );
	stream->WriteFloat( emptyColor.a );
	stream->WriteFloat( alternateColor.r );
	stream->WriteFloat( alternateColor.g );
	stream->WriteFloat( alternateColor.b );
	stream->WriteFloat( alternateColor.a );
	if ( valueFunction == NULL && maxFunction == NULL && alternateFunction == NULL ) {
		stream->WriteBool( true );
		if ( pythonModule != NULL ) {
			stream->WriteBool( true );
			stream->VirtualSlot11( pythonModule );
		} else {
			stream->WriteBool( false );
		}
		if ( pythonValueFunction != NULL ) {
			stream->WriteBool( true );
			stream->VirtualSlot11( pythonValueFunction );
		} else {
			stream->WriteBool( false );
		}
		if ( pythonMaxFunction != NULL ) {
			stream->WriteBool( true );
			stream->VirtualSlot11( pythonMaxFunction );
		} else {
			stream->WriteBool( false );
		}
		if ( pythonAlternateFunction != NULL ) {
			stream->WriteBool( true );
			stream->VirtualSlot11( pythonAlternateFunction );
		} else {
			stream->WriteBool( false );
		}
	} else {
		stream->WriteBool( false );
		if ( valueFunction != NULL ) {
			stream->WriteBool( true );
			stream->WriteID( GetFunctionIndex( valueFunction ) );
		} else {
			stream->WriteBool( false );
		}
		if ( maxFunction != NULL ) {
			stream->WriteBool( true );
			stream->WriteID( GetFunctionIndex( maxFunction ) );
		} else {
			stream->WriteBool( false );
		}
		if ( alternateFunction != NULL ) {
			stream->WriteBool( true );
			stream->WriteID( GetFunctionIndex( alternateFunction ) );
		} else {
			stream->WriteBool( false );
		}
	}
	stream->WriteInt( fillDirection );
	stream->WriteBool( tileIcons );
	return true;
}

/*
================
STFillGauge::ReadFromStream

The object functions come back as their places; PostFixUp turns them into
functions.
================
*/
// FUNCTION: STBC 0x00523070
bool STFillGauge::ReadFromStream( TGStream *stream ) {
	ClearSource();
	if ( !TGPane::ReadFromStream( stream ) ) {
		return false;
	}
	savedValueFunction = 0;
	savedMaxFunction = 0;
	savedAlternateFunction = 0;
	objectID = stream->ReadID();
	fillColor.r = stream->ReadFloat();
	fillColor.g = stream->ReadFloat();
	fillColor.b = stream->ReadFloat();
	fillColor.a = stream->ReadFloat();
	emptyColor.r = stream->ReadFloat();
	emptyColor.g = stream->ReadFloat();
	emptyColor.b = stream->ReadFloat();
	emptyColor.a = stream->ReadFloat();
	alternateColor.r = stream->ReadFloat();
	alternateColor.g = stream->ReadFloat();
	alternateColor.b = stream->ReadFloat();
	alternateColor.a = stream->ReadFloat();
	if ( !stream->ReadBool() ) {
		if ( stream->ReadBool() == true ) {
			savedValueFunction = stream->ReadID();
		} else {
			valueFunction = NULL;
		}
		if ( stream->ReadBool() == true ) {
			savedMaxFunction = stream->ReadID();
		} else {
			maxFunction = NULL;
		}
		if ( stream->ReadBool() == true ) {
			savedAlternateFunction = stream->ReadID();
		} else {
			alternateFunction = NULL;
		}
	} else {
		if ( stream->ReadBool() == true ) {
			pythonModule = stream->VirtualSlot10( -1 );
		} else {
			pythonModule = NULL;
		}
		if ( stream->ReadBool() == true ) {
			pythonValueFunction = stream->VirtualSlot10( -1 );
		} else {
			pythonValueFunction = NULL;
		}
		if ( stream->ReadBool() == true ) {
			pythonMaxFunction = stream->VirtualSlot10( -1 );
		} else {
			pythonMaxFunction = NULL;
		}
		if ( stream->ReadBool() == true ) {
			pythonAlternateFunction = stream->VirtualSlot10( -1 );
		} else {
			pythonAlternateFunction = NULL;
		}
	}
	fillDirection = stream->ReadInt();
	tileIcons = stream->ReadBool();
	return true;
}

/*
================
STFillGauge::PostFixUp
================
*/
// FUNCTION: STBC 0x005232a0
void STFillGauge::PostFixUp( void ) {
	TGUIObject::PostFixUp();
	valueFunction = GetValueFunction( savedValueFunction );
	maxFunction = GetValueFunction( savedMaxFunction );
	alternateFunction = GetFlagFunction( savedAlternateFunction );
}

/*
================
STFillGauge::GetFunctionIndex

The function's place in the table; 0 when it is not there.
================
*/
// FUNCTION: STBC 0x005232e0 ?GetFunctionIndex@STFillGauge@@IAEHP8TGObject@@AEMXZ@Z
int STFillGauge::GetFunctionIndex( GaugeValueFunction function ) {
	for ( int i = 0; i < STFILLGAUGE_NUM_FUNCTIONS; i++ ) {
		if ( functions[i] == function ) {
			return i;
		}
	}
	return 0;
}

/*
================
STFillGauge::GetFunctionIndex
================
*/
// FUNCTION: STBC 0x00523300 ?GetFunctionIndex@STFillGauge@@IAEHP8TGObject@@AE_NXZ@Z
int STFillGauge::GetFunctionIndex( GaugeFlagFunction function ) {
	for ( int i = 0; i < STFILLGAUGE_NUM_FUNCTIONS; i++ ) {
		if ( functions[i] == (GaugeValueFunction)function ) {
			return i;
		}
	}
	return 0;
}

/*
================
STFillGauge::GetValueFunction
================
*/
// FUNCTION: STBC 0x00523320
GaugeValueFunction STFillGauge::GetValueFunction( int index ) {
	return functions[index];
}

/*
================
STFillGauge::GetFlagFunction
================
*/
// FUNCTION: STBC 0x00523330
GaugeFlagFunction STFillGauge::GetFlagFunction( int index ) {
	return (GaugeFlagFunction)functions[index];
}
