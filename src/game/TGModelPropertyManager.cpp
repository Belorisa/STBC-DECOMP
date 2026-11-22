/*
===============================================================================

	TGModelPropertyManager.cpp

===============================================================================
*/

#include <iostream>
#include <string.h>

#include "TGModelPropertyManager.h"
#include "TGModelProperty.h"
#include "TGStream.h"
#include "NiMain.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x006ea810 _$E416
// FUNCTION: STBC 0x006ea830 _$E415

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006ea430 _$E32
// FUNCTION: STBC 0x006ea440 _$E29
// FUNCTION: STBC 0x006ea450 _$E31
// FUNCTION: STBC 0x006ea460 _$E30
// FUNCTION: STBC 0x006ea470 _$E38
// FUNCTION: STBC 0x006ea480 _$E35
// FUNCTION: STBC 0x006ea490 _$E37
// FUNCTION: STBC 0x006ea4a0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006ea4b0 _$E53
// FUNCTION: STBC 0x006ea4c0 _$E52
// FUNCTION: STBC 0x006ea4d0 _$E57
// FUNCTION: STBC 0x006ea4e0 _$E56
// FUNCTION: STBC 0x006ea4f0 _$E68
// FUNCTION: STBC 0x006ea500 _$E67
// FUNCTION: STBC 0x006ea510 _$E72
// FUNCTION: STBC 0x006ea520 _$E71
// FUNCTION: STBC 0x006ea530 _$E76
// FUNCTION: STBC 0x006ea540 _$E75
// FUNCTION: STBC 0x006ea550 _$E80
// FUNCTION: STBC 0x006ea560 _$E79
// FUNCTION: STBC 0x006ea570 _$E204
// FUNCTION: STBC 0x006ea580 _$E203
// FUNCTION: STBC 0x006ea590 _$E146
// FUNCTION: STBC 0x006ea5a0 _$E145
// FUNCTION: STBC 0x006ea730 _$E159
// FUNCTION: STBC 0x006ea740 _$E158
// FUNCTION: STBC 0x006ea840 _$E209
// FUNCTION: STBC 0x006ea850 _$E208
// FUNCTION: STBC 0x006ea8c0 _$E214
// FUNCTION: STBC 0x006ea8d0 _$E213
// FUNCTION: STBC 0x006ea8e0 _$E218
// FUNCTION: STBC 0x006ea8f0 _$E217
// FUNCTION: STBC 0x006ea900 _$E222
// FUNCTION: STBC 0x006ea910 _$E221
// FUNCTION: STBC 0x006ea920 _$E229
// FUNCTION: STBC 0x006ea930 _$E228
// FUNCTION: STBC 0x006ea940 _$E254
// FUNCTION: STBC 0x006ea950 _$E253
// FUNCTION: STBC 0x006ea960 _$E258
// FUNCTION: STBC 0x006ea970 _$E257
// FUNCTION: STBC 0x006ea980 _$E104
// FUNCTION: STBC 0x006ea990 _$E103
// FUNCTION: STBC 0x006ea9a0 _$E240
// FUNCTION: STBC 0x006ea9b0 _$E239
// FUNCTION: STBC 0x006ea9c0 _$E247
// FUNCTION: STBC 0x006ea9d0 _$E246
// FUNCTION: STBC 0x006ea9e0 _$E270
// FUNCTION: STBC 0x006ea9f0 _$E269
// FUNCTION: STBC 0x006eaa00 _$E274
// FUNCTION: STBC 0x006eaa10 _$E273
// FUNCTION: STBC 0x006eaa20 _$E314
// FUNCTION: STBC 0x006eaa30 _$E313
// FUNCTION: STBC 0x006eaa40 _$E320
// FUNCTION: STBC 0x006eaa50 _$E319
// FUNCTION: STBC 0x006eaa60 _$E262
// FUNCTION: STBC 0x006eaa70 _$E261
// FUNCTION: STBC 0x006eaa80 _$E266
// FUNCTION: STBC 0x006eaa90 _$E265
// FUNCTION: STBC 0x006eaaa0 _$E278
// FUNCTION: STBC 0x006eaab0 _$E277
// FUNCTION: STBC 0x006eaac0 _$E282
// FUNCTION: STBC 0x006eaad0 _$E281
// FUNCTION: STBC 0x006eaae0 _$E286
// FUNCTION: STBC 0x006eaaf0 _$E285
// FUNCTION: STBC 0x006eab00 _$E290
// FUNCTION: STBC 0x006eab10 _$E289
// FUNCTION: STBC 0x006eab20 _$E294
// FUNCTION: STBC 0x006eab30 _$E293
// FUNCTION: STBC 0x006eab40 _$E298
// FUNCTION: STBC 0x006eab50 _$E297
// FUNCTION: STBC 0x006eab60 _$E302
// FUNCTION: STBC 0x006eab70 _$E301
// FUNCTION: STBC 0x006eab80 _$E306
// FUNCTION: STBC 0x006eab90 _$E305
// FUNCTION: STBC 0x006eaba0 _$E109
// FUNCTION: STBC 0x006eabb0 _$E108
// FUNCTION: STBC 0x006eabc0 _$E310
// FUNCTION: STBC 0x006eabd0 _$E309
// FUNCTION: STBC 0x006eabe0 _$E114
// FUNCTION: STBC 0x006eabf0 _$E113
// FUNCTION: STBC 0x006eac00 _$E324
// FUNCTION: STBC 0x006eac10 _$E323
// FUNCTION: STBC 0x006eac20 _$E328
// FUNCTION: STBC 0x006eac30 _$E327
// FUNCTION: STBC 0x006eac40 _$E332
// FUNCTION: STBC 0x006eac50 _$E331
// FUNCTION: STBC 0x006eac60 _$E336
// FUNCTION: STBC 0x006eac70 _$E335
// FUNCTION: STBC 0x006eac80 _$E123
// FUNCTION: STBC 0x006eac90 _$E122
// FUNCTION: STBC 0x006eaca0 _$E340
// FUNCTION: STBC 0x006eacb0 _$E339
// FUNCTION: STBC 0x006eacc0 _$E344
// FUNCTION: STBC 0x006eacd0 _$E343
// FUNCTION: STBC 0x006eace0 _$E348
// FUNCTION: STBC 0x006eacf0 _$E347
// FUNCTION: STBC 0x006ead00 _$E129
// FUNCTION: STBC 0x006ead10 _$E128
// FUNCTION: STBC 0x006ead20 _$E134
// FUNCTION: STBC 0x006ead30 _$E133
// FUNCTION: STBC 0x006ead40 _$E164
// FUNCTION: STBC 0x006ead50 _$E163
// FUNCTION: STBC 0x006ead60 _$E352
// FUNCTION: STBC 0x006ead70 _$E351
// FUNCTION: STBC 0x006ead80 _$E169
// FUNCTION: STBC 0x006ead90 _$E168
// FUNCTION: STBC 0x006eada0 _$E356
// FUNCTION: STBC 0x006eadb0 _$E355
// FUNCTION: STBC 0x006eadc0 _$E360
// FUNCTION: STBC 0x006eadd0 _$E359
// FUNCTION: STBC 0x006eade0 _$E174
// FUNCTION: STBC 0x006eadf0 _$E173
// FUNCTION: STBC 0x006eae00 _$E364
// FUNCTION: STBC 0x006eae10 _$E363
// FUNCTION: STBC 0x006eae20 _$E139
// FUNCTION: STBC 0x006eae30 _$E138
// FUNCTION: STBC 0x006eae40 _$E368
// FUNCTION: STBC 0x006eae50 _$E367
// FUNCTION: STBC 0x006eae60 _$E372
// FUNCTION: STBC 0x006eae70 _$E371
// FUNCTION: STBC 0x006eae80 _$E179
// FUNCTION: STBC 0x006eae90 _$E178
// FUNCTION: STBC 0x006eaea0 _$E184
// FUNCTION: STBC 0x006eaeb0 _$E183
// FUNCTION: STBC 0x006eaec0 _$E376
// FUNCTION: STBC 0x006eaed0 _$E375
// FUNCTION: STBC 0x006eaee0 _$E380
// FUNCTION: STBC 0x006eaef0 _$E379
// FUNCTION: STBC 0x006eaf00 _$E384
// FUNCTION: STBC 0x006eaf10 _$E383
// FUNCTION: STBC 0x006eaf20 _$E388
// FUNCTION: STBC 0x006eaf30 _$E387
// FUNCTION: STBC 0x006eaf40 _$E392
// FUNCTION: STBC 0x006eaf50 _$E391
// FUNCTION: STBC 0x006eaf60 _$E396
// FUNCTION: STBC 0x006eaf70 _$E395
// FUNCTION: STBC 0x006eaf80 _$E400
// FUNCTION: STBC 0x006eaf90 _$E399
// FUNCTION: STBC 0x006eafa0 _$E404
// FUNCTION: STBC 0x006eafb0 _$E403
// FUNCTION: STBC 0x006eafc0 _$E189
// FUNCTION: STBC 0x006eafd0 _$E188
// FUNCTION: STBC 0x006eafe0 _$E408
// FUNCTION: STBC 0x006eaff0 _$E407
// FUNCTION: STBC 0x006eb000 _$E194
// FUNCTION: STBC 0x006eb010 _$E193
// FUNCTION: STBC 0x006eb020 _$E199
// FUNCTION: STBC 0x006eb030 _$E198

/*
================
TGModelPropertyManager::TGModelPropertyManager
================
*/
// FUNCTION: STBC 0x006e9650
TGModelPropertyManager::TGModelPropertyManager( void ) : registeredFilters( 2 ), currentFilters( 2 ), globalTemplates( 2 ), localTemplates( 2 ) {
}

/*
================
TGModelPropertyManager::RegisterGlobalTemplate

Adds the template to the global ones, letting go of the one it replaces
(of the same name and type).
================
*/
// FUNCTION: STBC 0x006e96b0 ?RegisterGlobalTemplate@TGModelPropertyManager@@QAE_NPAVTGModelProperty@@@Z
bool TGModelPropertyManager::RegisterGlobalTemplate( TGModelProperty *property ) {
	char *name = property->GetName().Unknown6F56F0();
	TGModelProperty *existing = FindByNameAndType( name, property->GetObjType(), false );
	if ( existing != NULL ) {
		GetGlobalTemplates()->Remove( existing );
		existing->DecRefCount();
	}
	globalTemplates.AddTail( property );
	delete [] name;
	return true;
}

/*
================
TGModelPropertyManager::RegisterLocalTemplate

Adds the template to the local ones, letting go of the one it replaces
(of the same name and type).
================
*/
// FUNCTION: STBC 0x006e9880 ?RegisterLocalTemplate@TGModelPropertyManager@@QAE_NPAVTGModelProperty@@@Z
bool TGModelPropertyManager::RegisterLocalTemplate( TGModelProperty *property ) {
	char *name = property->GetName().Unknown6F56F0();
	TGModelProperty *existing = FindByNameAndType( name, property->GetObjType(), true );
	if ( existing != NULL ) {
		GetLocalTemplates()->Remove( existing );
		existing->DecRefCount();
	}
	localTemplates.AddTail( property );
	delete [] name;
	return true;
}

/*
================
TGModelPropertyManager::FindByNameAndType

The template of the name and type, among the local ones or the global ones.
================
*/
// FUNCTION: STBC 0x006e9a50 ?FindByNameAndType@TGModelPropertyManager@@QAEPAVTGModelProperty@@PADHH@Z
TGModelProperty *TGModelPropertyManager::FindByNameAndType( char *name, int type, int local ) {
	NiTList<TGModelProperty *> *templates;
	if ( local == 0 ) {
		templates = &globalTemplates;
	} else {
		templates = &localTemplates;
	}
	NiTListIterator pos = templates->GetHeadPos();
	while ( pos != NULL ) {
		TGModelProperty *property = templates->GetNext( pos );
		if ( property->IsTypeOf( type ) == true && property->GetName().CompareC( name, true ) == 0 ) {
			return property;
		}
	}
	return NULL;
}

/*
================
TGModelPropertyManager::FindByName

The template of the name, among the local ones or the global ones.
================
*/
// FUNCTION: STBC 0x006e9b00 ?FindByName@TGModelPropertyManager@@QAEPAVTGModelProperty@@PADH@Z
TGModelProperty *TGModelPropertyManager::FindByName( char *name, int local ) {
	NiTList<TGModelProperty *> *templates;
	if ( local == 0 ) {
		templates = &globalTemplates;
	} else {
		templates = &localTemplates;
	}
	NiTListIterator pos = templates->GetHeadPos();
	while ( pos != NULL ) {
		TGModelProperty *property = templates->GetNext( pos );
		if ( property->GetName().CompareC( name, true ) == 0 ) {
			return property;
		}
	}
	return NULL;
}

/*
================
TGModelPropertyManager::ClearLocalTemplates

Lets go of the templates of the set being built.
================
*/
// FUNCTION: STBC 0x006e9ba0 ?ClearLocalTemplates@TGModelPropertyManager@@QAEXXZ
void TGModelPropertyManager::ClearLocalTemplates( void ) {
	NiTListIterator pos = localTemplates.GetHeadPos();
	while ( pos != NULL ) {
		TGModelProperty *property = localTemplates.GetNext( pos );
		property->DecRefCount();
	}
	localTemplates.RemoveAll();
}

/*
================
TGModelPropertyManager::ClearGlobalTemplates

Lets go of the global templates.
================
*/
// FUNCTION: STBC 0x006e9c00 ?ClearGlobalTemplates@TGModelPropertyManager@@QAEXXZ
void TGModelPropertyManager::ClearGlobalTemplates( void ) {
	NiTListIterator pos = globalTemplates.GetHeadPos();
	while ( pos != NULL ) {
		TGModelProperty *property = globalTemplates.GetNext( pos );
		property->DecRefCount();
	}
	globalTemplates.RemoveAll();
}

/*
================
TGModelPropertyManager::GetGlobalTemplates
================
*/
// FUNCTION: STBC 0x006e9c60
NiTList<TGModelProperty *> *TGModelPropertyManager::GetGlobalTemplates( void ) {
	return &globalTemplates;
}

/*
================
TGModelPropertyManager::GetLocalTemplates
================
*/
// FUNCTION: STBC 0x006e9c70
NiTList<TGModelProperty *> *TGModelPropertyManager::GetLocalTemplates( void ) {
	return &localTemplates;
}

/*
================
TGModelPropertyManager::RemoveTemplate

Takes the template out of the local or the global ones, deleting it when
asked to.
================
*/
// FUNCTION: STBC 0x006e9c80 ?RemoveTemplate@TGModelPropertyManager@@QAE_NHPAVTGModelProperty@@_N@Z
bool TGModelPropertyManager::RemoveTemplate( int local, TGModelProperty *property, bool deleteIt ) {
	NiTList<TGModelProperty *> *templates;
	if ( local == 0 ) {
		templates = &globalTemplates;
	} else {
		templates = &localTemplates;
	}
	NiTListIterator pos = templates->GetHeadPos();
	while ( pos != NULL ) {
		if ( templates->GetNext( pos ) == property ) {
			templates->Remove( property );
			if ( deleteIt == true && property != NULL ) {
				delete property;
			}
			return true;
		}
	}
	return false;
}

/*
================
TGModelPropertyManager::ClearCurrentFilters
================
*/
// FUNCTION: STBC 0x006e9d90 ?ClearCurrentFilters@TGModelPropertyManager@@QAEXXZ
void TGModelPropertyManager::ClearCurrentFilters( void ) {
	currentFilters.RemoveAll();
}

/*
================
TGModelPropertyManager::AddFilter

Adds the filter to the ones in use; NULL adds nothing.
================
*/
// FUNCTION: STBC 0x006e9de0
void TGModelPropertyManager::AddFilter( modelPropertyFilter_t filter ) {
	if ( filter != NULL ) {
		currentFilters.AddTail( filter );
	}
}

/*
================
TGModelPropertyManager::RemoveFilter

Takes the filter out of the current filters; false for no filter.
================
*/
// FUNCTION: STBC 0x006e9e50
bool TGModelPropertyManager::RemoveFilter( modelPropertyFilter_t filter ) {
	if ( filter == NULL ) {
		return false;
	}
	// BUG: NiTList::Remove returns the element itself when it isn't in the list, so a filter that was never added
	// reports true as if it had been removed.
	return currentFilters.Remove( filter ) != NULL ? true : false;
}

/*
================
TGModelPropertyManager::ApplyFilters

True when every current filter takes the property.
================
*/
// FUNCTION: STBC 0x006e9f40 ?ApplyFilters@TGModelPropertyManager@@QAE_NPAVTGModelProperty@@@Z
bool TGModelPropertyManager::ApplyFilters( TGModelProperty *property ) {
	NiTListIterator pos = currentFilters.GetHeadPos();
	while ( pos != NULL ) {
		modelPropertyFilter_t filter = currentFilters.GetNext( pos );
		if ( !filter( property ) ) {
			return false;
		}
	}
	return true;
}

/*
================
TGModelPropertyManager::Unknown6E9F80

A new list of the local or the global templates every current filter
takes; the caller owns it. Nothing in retail calls it.
================
*/
// FUNCTION: STBC 0x006e9f80
NiTList<TGModelProperty *> *TGModelPropertyManager::Unknown6E9F80( int local ) {
	NiTList<TGModelProperty *> *templates;
	if ( local == 0 ) {
		templates = &globalTemplates;
	} else {
		templates = &localTemplates;
	}
	NiTList<TGModelProperty *> *filtered = new NiTList<TGModelProperty *>( 2 );
	NiTListIterator pos = templates->GetHeadPos();
	while ( pos != NULL ) {
		TGModelProperty *property = templates->GetNext( pos );
		if ( ApplyFilters( property ) == true ) {
			filtered->AddTail( property );
		}
	}
	return filtered;
}

/*
================
TGModelPropertyManager::RegisterFilter

Files the filter under a copy of the name.
================
*/
// FUNCTION: STBC 0x006ea0e0
void TGModelPropertyManager::RegisterFilter( modelPropertyFilter_t filter, char *name ) {
	TGModelPropertyFilterRecord *record = new TGModelPropertyFilterRecord;
	record->filter = filter;
	record->name = new char[strlen( name ) + 1];
	strcpy( record->name, name );
	registeredFilters.AddTail( record );
}

/*
================
TGModelPropertyManager::ClearRegisteredFilters

Deletes the records of the registered filters.
================
*/
// FUNCTION: STBC 0x006ea1a0 ?ClearRegisteredFilters@TGModelPropertyManager@@QAEXXZ
void TGModelPropertyManager::ClearRegisteredFilters( void ) {
	NiTListIterator pos = registeredFilters.GetHeadPos();
	while ( pos != NULL ) {
		TGModelPropertyFilterRecord *record = registeredFilters.GetNext( pos );
		delete record->name;
		delete record;
	}
	registeredFilters.RemoveAll();
}

/*
================
TGModelPropertyManager::IsGlobalTemplate
================
*/
// FUNCTION: STBC 0x006ea220 ?IsGlobalTemplate@TGModelPropertyManager@@QAE_NPAVTGModelProperty@@@Z
bool TGModelPropertyManager::IsGlobalTemplate( TGModelProperty *property ) {
	if ( globalTemplates.FindPos( property ) != NULL ) {
		return true;
	}
	return false;
}

/*
================
TGModelPropertyManager::IsLocalTemplate
================
*/
// FUNCTION: STBC 0x006ea260 ?IsLocalTemplate@TGModelPropertyManager@@QAE_NPAVTGModelProperty@@@Z
bool TGModelPropertyManager::IsLocalTemplate( TGModelProperty *property ) {
	if ( localTemplates.FindPos( property ) != NULL ) {
		return true;
	}
	return false;
}

/*
================
TGModelPropertyManager::WriteToStream

Writes the local templates, then the global ones, each preceded by a true
flag and the list ended by a false one.
================
*/
// FUNCTION: STBC 0x006ea2a0 ?WriteToStream@TGModelPropertyManager@@UAE_NPAVTGStream@@@Z
bool TGModelPropertyManager::WriteToStream( TGStream *stream ) {
	NiTListIterator pos = localTemplates.GetHeadPos();
	while ( pos != NULL ) {
		TGModelProperty *property = localTemplates.GetNext( pos );
		if ( property != NULL ) {
			stream->WriteBool( true );
			if ( !property->WriteToStream( stream ) ) {
				return false;
			}
		}
	}
	stream->WriteBool( false );

	pos = globalTemplates.GetHeadPos();
	while ( pos != NULL ) {
		TGModelProperty *property = globalTemplates.GetNext( pos );
		if ( property != NULL ) {
			stream->WriteBool( true );
			if ( !property->WriteToStream( stream ) ) {
				return false;
			}
		}
	}
	stream->WriteBool( false );
	return true;
}

/*
================
TGModelPropertyManager::ReadFromStream

Reads what WriteToStream wrote: the local templates, then the global ones.
================
*/
// FUNCTION: STBC 0x006ea320
bool TGModelPropertyManager::ReadFromStream( TGStream *stream ) {
	TGObject::Log( "\nLoading TGModelPropertyManager\n\n" );
	while ( stream->ReadBool() == true ) {
		localTemplates.AddTail( (TGModelProperty *)TGObject::LoadObject( stream, 0 ) );
	}
	while ( stream->ReadBool() == true ) {
		globalTemplates.AddTail( (TGModelProperty *)TGObject::LoadObject( stream, 0 ) );
	}
	TGObject::Log( "\nFinished loading TGModelPropertyManager\n\n" );
	return true;
}
