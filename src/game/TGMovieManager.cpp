/*
===============================================================================

	TGMovieManager.cpp

	The Bink movie player.

===============================================================================
*/

#include <iostream>

#include "TGModule.h"
#include "TGMovieManager.h"
#include "TGMovieAction.h"
#include "TGIconManager.h"
#include "TGActionManager.h"
#include "TGObject.h"
#include "TGStream.h"
#include "UtopiaApp.h"
#include "NiAppWindow.h"
#include "NiCamera.h"
#include "renderer/NiD3DRender.h"
#include "renderer/NiDDImage.h"
#include "TGBinkImports.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x006b03c0 _$E460
// FUNCTION: STBC 0x006b03e0 _$E459

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006ae2f0 _$E32
// FUNCTION: STBC 0x006ae300 _$E29
// FUNCTION: STBC 0x006ae310 _$E31
// FUNCTION: STBC 0x006ae320 _$E30
// FUNCTION: STBC 0x006ae330 _$E38
// FUNCTION: STBC 0x006ae340 _$E35
// FUNCTION: STBC 0x006ae350 _$E37
// FUNCTION: STBC 0x006ae360 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006ae370 _$E54
// FUNCTION: STBC 0x006ae380 _$E53
// FUNCTION: STBC 0x006ae390 _$E61
// FUNCTION: STBC 0x006ae3a0 _$E60
// FUNCTION: STBC 0x006ae3b0 _$E72
// FUNCTION: STBC 0x006ae3c0 _$E71
// FUNCTION: STBC 0x006ae3d0 _$E76
// FUNCTION: STBC 0x006ae3e0 _$E75
// FUNCTION: STBC 0x006ae3f0 _$E80
// FUNCTION: STBC 0x006ae400 _$E79
// FUNCTION: STBC 0x006ae410 _$E84
// FUNCTION: STBC 0x006ae420 _$E83
// FUNCTION: STBC 0x006ae430 _$E208
// FUNCTION: STBC 0x006ae440 _$E207
// FUNCTION: STBC 0x006ae450 _$E150
// FUNCTION: STBC 0x006ae460 _$E149
// FUNCTION: STBC 0x006ae470 _$E163
// FUNCTION: STBC 0x006ae480 _$E162
// FUNCTION: STBC 0x006ae490 _$E213
// FUNCTION: STBC 0x006ae4a0 _$E212
// FUNCTION: STBC 0x006ae4b0 _$E218
// FUNCTION: STBC 0x006ae4c0 _$E217
// FUNCTION: STBC 0x006ae4d0 _$E222
// FUNCTION: STBC 0x006ae4e0 _$E221
// FUNCTION: STBC 0x006ae4f0 _$E226
// FUNCTION: STBC 0x006ae500 _$E225
// FUNCTION: STBC 0x006ae510 _$E233
// FUNCTION: STBC 0x006ae520 _$E232
// FUNCTION: STBC 0x006ae530 _$E284
// FUNCTION: STBC 0x006ae540 _$E283
// FUNCTION: STBC 0x006ae550 _$E288
// FUNCTION: STBC 0x006ae560 _$E287
// FUNCTION: STBC 0x006ae570 _$E108
// FUNCTION: STBC 0x006ae580 _$E107
// FUNCTION: STBC 0x006ae590 _$E270
// FUNCTION: STBC 0x006ae5a0 _$E269
// FUNCTION: STBC 0x006ae5b0 _$E277
// FUNCTION: STBC 0x006ae5c0 _$E276
// FUNCTION: STBC 0x006ae5d0 _$E296
// FUNCTION: STBC 0x006ae5e0 _$E295
// FUNCTION: STBC 0x006ae5f0 _$E300
// FUNCTION: STBC 0x006ae600 _$E299
// FUNCTION: STBC 0x006ae610 _$E340
// FUNCTION: STBC 0x006ae620 _$E339
// FUNCTION: STBC 0x006ae630 _$E346
// FUNCTION: STBC 0x006ae640 _$E345
// FUNCTION: STBC 0x006ae650 _$E260
// FUNCTION: STBC 0x006ae660 _$E259
// FUNCTION: STBC 0x006ae670 _$E292
// FUNCTION: STBC 0x006ae680 _$E291
// FUNCTION: STBC 0x006ae690 _$E304
// FUNCTION: STBC 0x006ae6a0 _$E303
// FUNCTION: STBC 0x006ae6b0 _$E308
// FUNCTION: STBC 0x006ae6c0 _$E307
// FUNCTION: STBC 0x006ae6d0 _$E312
// FUNCTION: STBC 0x006ae6e0 _$E311
// FUNCTION: STBC 0x006ae6f0 _$E316
// FUNCTION: STBC 0x006ae700 _$E315
// FUNCTION: STBC 0x006ae710 _$E320
// FUNCTION: STBC 0x006ae720 _$E319
// FUNCTION: STBC 0x006ae730 _$E324
// FUNCTION: STBC 0x006ae740 _$E323
// FUNCTION: STBC 0x006ae750 _$E328
// FUNCTION: STBC 0x006ae760 _$E327
// FUNCTION: STBC 0x006ae770 _$E332
// FUNCTION: STBC 0x006ae780 _$E331
// FUNCTION: STBC 0x006ae790 _$E113
// FUNCTION: STBC 0x006ae7a0 _$E112
// FUNCTION: STBC 0x006ae7b0 _$E336
// FUNCTION: STBC 0x006ae7c0 _$E335
// FUNCTION: STBC 0x006ae7d0 _$E118
// FUNCTION: STBC 0x006ae7e0 _$E117
// FUNCTION: STBC 0x006ae7f0 _$E350
// FUNCTION: STBC 0x006ae800 _$E349
// FUNCTION: STBC 0x006ae810 _$E354
// FUNCTION: STBC 0x006ae820 _$E353
// FUNCTION: STBC 0x006ae830 _$E358
// FUNCTION: STBC 0x006ae840 _$E357
// FUNCTION: STBC 0x006ae850 _$E362
// FUNCTION: STBC 0x006ae860 _$E361
// FUNCTION: STBC 0x006ae870 _$E127
// FUNCTION: STBC 0x006ae880 _$E126
// FUNCTION: STBC 0x006ae890 _$E366
// FUNCTION: STBC 0x006ae8a0 _$E365
// FUNCTION: STBC 0x006ae8b0 _$E370
// FUNCTION: STBC 0x006ae8c0 _$E369
// FUNCTION: STBC 0x006ae8d0 _$E374
// FUNCTION: STBC 0x006ae8e0 _$E373
// FUNCTION: STBC 0x006ae8f0 _$E133
// FUNCTION: STBC 0x006ae900 _$E132
// FUNCTION: STBC 0x006ae910 _$E138
// FUNCTION: STBC 0x006ae920 _$E137
// FUNCTION: STBC 0x006ae930 _$E168
// FUNCTION: STBC 0x006ae940 _$E167
// FUNCTION: STBC 0x006ae950 _$E378
// FUNCTION: STBC 0x006ae960 _$E377
// FUNCTION: STBC 0x006ae970 _$E173
// FUNCTION: STBC 0x006ae980 _$E172
// FUNCTION: STBC 0x006ae990 _$E382
// FUNCTION: STBC 0x006ae9a0 _$E381
// FUNCTION: STBC 0x006ae9b0 _$E386
// FUNCTION: STBC 0x006ae9c0 _$E385
// FUNCTION: STBC 0x006ae9d0 _$E178
// FUNCTION: STBC 0x006ae9e0 _$E177
// FUNCTION: STBC 0x006ae9f0 _$E390
// FUNCTION: STBC 0x006aea00 _$E389
// FUNCTION: STBC 0x006aea10 _$E143
// FUNCTION: STBC 0x006aea20 _$E142
// FUNCTION: STBC 0x006aea30 _$E394
// FUNCTION: STBC 0x006aea40 _$E393
// FUNCTION: STBC 0x006aea50 _$E398
// FUNCTION: STBC 0x006aea60 _$E397
// FUNCTION: STBC 0x006aea70 _$E183
// FUNCTION: STBC 0x006aea80 _$E182
// FUNCTION: STBC 0x006aea90 _$E188
// FUNCTION: STBC 0x006aeaa0 _$E187
// FUNCTION: STBC 0x006aeab0 _$E402
// FUNCTION: STBC 0x006aeac0 _$E401
// FUNCTION: STBC 0x006aead0 _$E406
// FUNCTION: STBC 0x006aeae0 _$E405
// FUNCTION: STBC 0x006aeaf0 _$E410
// FUNCTION: STBC 0x006aeb00 _$E409
// FUNCTION: STBC 0x006aeb10 _$E414
// FUNCTION: STBC 0x006aeb20 _$E413
// FUNCTION: STBC 0x006aeb30 _$E418
// FUNCTION: STBC 0x006aeb40 _$E417
// FUNCTION: STBC 0x006aeb50 _$E422
// FUNCTION: STBC 0x006aeb60 _$E421
// FUNCTION: STBC 0x006aeb70 _$E426
// FUNCTION: STBC 0x006aeb80 _$E425
// FUNCTION: STBC 0x006aeb90 _$E430
// FUNCTION: STBC 0x006aeba0 _$E429
// FUNCTION: STBC 0x006aebb0 _$E193
// FUNCTION: STBC 0x006aebc0 _$E192
// FUNCTION: STBC 0x006aebd0 _$E434
// FUNCTION: STBC 0x006aebe0 _$E433
// FUNCTION: STBC 0x006aebf0 _$E198
// FUNCTION: STBC 0x006aec00 _$E197
// FUNCTION: STBC 0x006aec10 _$E203
// FUNCTION: STBC 0x006aec20 _$E202
// FUNCTION: STBC 0x006aec30 _$E438
// FUNCTION: STBC 0x006aec40 _$E437
// FUNCTION: STBC 0x006aec50 _$E443
// FUNCTION: STBC 0x006aec60 _$E442
// FUNCTION: STBC 0x006aec70 _$E447
// FUNCTION: STBC 0x006aec80 _$E446

// Its construction and exit-time destruction.
// FUNCTION: STBC 0x006aec90 _$E452
// FUNCTION: STBC 0x006aeca0 _$E449
// FUNCTION: STBC 0x006aecb0 _$E451
// FUNCTION: STBC 0x006aecc0 _$E450
// DATA: STBC 0x00995a00
TGMovieManager g_kMovieManager;

/*
================
TGMovieManager::TGMovieManager
================
*/
// FUNCTION: STBC 0x006aecd0
TGMovieManager::TGMovieManager( void ) {
	movieMode = false;
	savedDevice = new Ni3DDeviceDesc;
	numMovies = 0;
	playMovies = true;
	volume = 1.0f;
	forcedMovie = NULL;
}

/*
================
TGMovieManager::~TGMovieManager
================
*/
// FUNCTION: STBC 0x006aed90
TGMovieManager::~TGMovieManager( void ) {
	SwitchOutOfMovieMode();
	delete savedDevice;
}

/*
================
TGMovieManager::IsPlaying
================
*/
// FUNCTION: STBC 0x006aedb0
bool TGMovieManager::IsPlaying( TGMovieAction *action ) {
	for ( int i = 0; i < numMovies; i++ ) {
		TGMovieAction *movieAction = movies[i].action;
		if ( g_kTGActionManager.FindAction( movieAction ) != -1 ) {
			if ( action == movieAction ) {
				return true;
			}
		} else {
			RemoveMovie( i, false );
			i--;
		}
	}
	return false;
}

/*
================
TGMovieManager::RemoveMovie
================
*/
// FUNCTION: STBC 0x006aee10
void TGMovieManager::RemoveMovie( int index, bool skipAction ) {
	if ( numMovies > 0 ) {
		if ( movies[index].bink != NULL ) {
			BinkClose( movies[index].bink );
			movies[index].bink = NULL;
		}
		if ( skipAction && g_kTGActionManager.FindAction( movies[index].action ) != -1 ) {
			TGMovieAction *action = movies[index].action;
			movies[index].action = NULL;
			action->Skip();
		}

		int i;
		for ( i = index; i < numMovies - 1; i++ ) {
			movies[i].action = movies[i + 1].action;
			movies[i].bink = movies[i + 1].bink;
			movies[i].waiting = movies[i + 1].waiting;
		}
		movies[i].action = NULL;
		movies[i].bink = NULL;
		numMovies--;
	}
}

/*
================
TGMovieManager::AddMovie
================
*/
// FUNCTION: STBC 0x006aeeb0
bool TGMovieManager::AddMovie( char *file, TGMovieAction *action, bool skipFrames ) {
	// BUG: lets a sixth movie in, over the count
	if ( numMovies > MAX_MOVIES_AT_ONCE ) {
		return false;
	}

	movies[numMovies].action = action;
	BinkSetSoundSystem( BinkOpenMiles, (unsigned long)NiSoundSystem::GetSoundSystem()->GetDriver() );
	if ( skipFrames ) {
		movies[numMovies].bink = BinkOpen( file, 0 );
	} else {
		movies[numMovies].bink = BinkOpen( file, BINKNOSKIP );
	}
	BINK *bink = movies[numMovies].bink;
	if ( bink == NULL ) {
		movies[numMovies].action = NULL;
		movies[numMovies].bink = NULL;
		return false;
	}

	BinkSetVolume( bink, 0, (int)( volume * 32768.0f ) );
	numMovies++;
	return true;
}

/*
================
TGMovieManager::OpenBuffer
================
*/
// FUNCTION: STBC 0x006aef60
// PARKED: register allocation only: retail loads the application's window through EDX and reloads this into ECX for the buffer store; named locals for the window and the app window change nothing
bool TGMovieManager::OpenBuffer( void ) {
	NiD3DRender *renderer = (NiD3DRender *)(NiRender *)UtopiaApp::GetRenderer();
	if ( renderer == NULL ) {
		return false;
	}

	IDirectDraw7 *directDraw = renderer->Unknown7C1320();
	NiPointer<NiImage> image = (NiImage *)renderer->VirtualSlot22( 1 );
	NiPointer<NiDDBufferImage> bufferImage = NiDynamicCast( NiDDBufferImage, image );

	IDirectDrawSurface7 *surface;
	bufferImage->Unknown7C70C0( &surface );
	IDirectDrawSurface *primary;
	surface->QueryInterface( IID_IDirectDrawSurface, (void **)&primary );
	BinkBufferSetDirectDraw( directDraw, primary );

	unsigned int width = renderer->VirtualSlot19();
	unsigned int height = renderer->VirtualSlot20();
	buffer = BinkBufferOpen( NiApplication::application->GetAppWindow()->GetWindow(), width, height, 0 );
	if ( buffer == NULL ) {
		return false;
	}
	BinkBufferSetOffset( buffer, 0, 0 );
	return true;
}

/*
================
TGMovieManager::PlayNextFrame

Decodes the due frame of every movie into the back buffer, at the action's
offset or centred, and plays the frame actions; the movies that reached
their last frame are removed and their actions completed afterwards.
================
*/
// FUNCTION: STBC 0x006af140
void TGMovieManager::PlayNextFrame( void ) {
	if ( numMovies <= 0 || !playMovies ) {
		return;
	}

	NiD3DRender *renderer = (NiD3DRender *)(NiRender *)UtopiaApp::GetRenderer();
	if ( renderer == NULL ) {
		return;
	}

	NiTList<TGMovieAction *> finished( 2 );
	renderer->Unknown7C1320();
	NiPointer<NiImage> image = (NiImage *)renderer->VirtualSlot22( 1 );
	NiPointer<NiDDBufferImage> bufferImage = NiDynamicCast( NiDDBufferImage, image );

	IDirectDrawSurface7 *surface;
	bufferImage->Unknown7C70C0( &surface );
	DDSURFACEDESC2 desc;
	desc.dwSize = sizeof( desc );
	int surfaceType = BinkDDSurfaceType( surface );

	int i;
	for ( i = 0; i < numMovies; i++ ) {
		BINK *bink = movies[i].bink;
		if ( bink != NULL ) {
			movies[i].waiting = BinkWait( bink ) != 0;
			if ( !movies[i].waiting ) {
				BinkDoFrame( bink );
			}
		}
	}

	for ( ;; ) {
		HRESULT result = surface->Lock( NULL, &desc, 0, NULL );
		if ( result == DDERR_SURFACELOST ) {
			surface->Restore();
		} else if ( result != DDERR_WASSTILLDRAWING && result != DDERR_SURFACEBUSY && result != DDERR_SURFACEISOBSCURED ) {
			break;
		}
	}

	void *bits = desc.lpSurface;
	for ( i = 0; i < numMovies; i++ ) {
		BINK *bink = movies[i].bink;
		if ( bink != NULL ) {
			float offsetX = movies[i].action->GetOffsetX();
			float offsetY = movies[i].action->GetOffsetY();
			unsigned int height = renderer->VirtualSlot20();
			unsigned int width = renderer->VirtualSlot19();
			unsigned int x;
			if ( offsetX < 0.0f ) {
				x = ( width - bink->Width ) >> 1;
			} else {
				x = (unsigned int)( width * offsetX );
			}
			unsigned int y;
			if ( offsetY < 0.0f ) {
				y = ( height - bink->Height ) >> 1;
			} else {
				y = (unsigned int)( height * offsetY );
			}
			BinkCopyToBuffer( bink, bits, desc.lPitch, desc.dwHeight, x, y, surfaceType | BINKCOPYALL );
		}
	}

	while ( surface->Unlock( NULL ) != DD_OK ) {
	}

	for ( i = 0; i < numMovies; i++ ) {
		BINK *bink = movies[i].bink;
		if ( bink != NULL ) {
			TGMovieAction *action = movies[i].action;
			if ( !movies[i].waiting ) {
				if ( bink->FrameNum == bink->Frames ) {
					RemoveMovie( i, false );
					i--;
					if ( action != NULL ) {
						finished.AddTail( action );
					}
				} else {
					if ( action != NULL ) {
						action->PlayFrameActions( bink->FrameNum );
					}
					BinkNextFrame( bink );
				}
			}
		}
	}

	NiTListIterator pos = finished.GetHeadPos();
	while ( pos != NULL ) {
		TGMovieAction *action = finished.GetNext( pos );
		if ( action != NULL ) {
			action->Completed();
		}
	}
}

/*
================
TGMovieManager::StopAllMovies
================
*/
// FUNCTION: STBC 0x006af5a0
void TGMovieManager::StopAllMovies( void ) {
	for ( int i = numMovies - 1; i >= 0; i-- ) {
		RemoveMovie( i, true );
	}
	if ( buffer != NULL ) {
		BinkBufferClose( buffer );
	}
	buffer = NULL;
}

/*
================
TGMovieManager::StopMovie
================
*/
// FUNCTION: STBC 0x006af5d0
void TGMovieManager::StopMovie( TGMovieAction *action ) {
	for ( int i = 0; i < numMovies; i++ ) {
		TGMovieAction *movieAction = movies[i].action;
		if ( g_kTGActionManager.FindAction( movieAction ) != -1 ) {
			if ( action == movieAction ) {
				RemoveMovie( i, false );
				break;
			}
		} else if ( movieAction != NULL ) {
			RemoveMovie( i, false );
			i--;
		}
	}

	if ( numMovies == 0 ) {
		if ( buffer != NULL ) {
			BinkBufferClose( buffer );
		}
		buffer = NULL;
	}
}

/*
================
TGMovieManager::SwitchToMovieMode

Saves the display device, size and depth and recreates the renderer at
640 by 480 for the movies.
================
*/
// FUNCTION: STBC 0x006af650
// PARKED: schedule: calls, frame and registers as retail; retail loads the mode for NiD3DRender::Create after the window handle and stores the height's 64-bit conversion temporary before popping CallVirtualSlot07's argument
void TGMovieManager::SwitchToMovieMode( void ) {
	if ( movieMode )
		return;

	{
		NiPointer<NiRender> render = UtopiaApp::GetRenderer();
		NiD3DRender *renderer = NiDynamicCast( NiD3DRender, render );
		if ( renderer == NULL )
			return;

		*savedDevice = *renderer->Unknown7C2EE0();
		if ( renderer->VirtualSlot13() & 0x100 )
			savedBitDepth = 32;
		else
			savedBitDepth = 16;
		if ( NiApplication::application->GetFullscreen() )
			savedWindowed = false;
		else
			savedWindowed = true;
		unknown5D = false;
		savedWidth = render->VirtualSlot19();
		savedHeight = render->VirtualSlot20();
	}

	int mode = GetRendererMode();
	unsigned int width = GetRendererWidth();
	unsigned int height = GetRendererHeight();
	unsigned int flags = GetRendererFlags();
	void *window = GetRendererWindow();
	if ( window == NULL )
		return;
	Ni3DDeviceDesc *device = GetDisplayDevice();
	if ( device == NULL )
		return;

	TGModule::CallVirtualSlot06();
	NiD3DRender *renderer = NiD3DRender::Create( mode, width, height, flags, (HWND)window,
												 NiApplication::application->GetAppWindow()->GetWindow(), device );
	if ( renderer == NULL )
		SwitchOutOfMovieMode();
	TGModule::CallVirtualSlot07( (int)renderer );

	g_kIconManager.SetScreenWidth( (float)width );
	g_kIconManager.SetScreenHeight( (float)height );
	NiApplication::application->GetCamera()->Clear( 1 );
	NiApplication::application->GetCamera()->Click();
	NiApplication::application->GetCamera()->Unknown7E81B0();
	movieMode = true;
}

/*
================
TGMovieManager::SwitchOutOfMovieMode

Stops the movies and recreates the renderer with the saved display
device, size and depth; a failure stops the program.
================
*/
// FUNCTION: STBC 0x006afa40
// PARKED: schedule: retail loads the application pointer before the screen-size fild and loads the Create arguments in another register order; the body and calls are complete
void TGMovieManager::SwitchOutOfMovieMode( void ) {
	if ( !movieMode ) {
		return;
	}

	StopAllMovies();
	int mode = GetRendererMode();
	unsigned int width = GetRendererWidth();
	unsigned int height = GetRendererHeight();
	unsigned int flags = GetRendererFlags();
	void *window = GetRendererWindow();
	if ( window == NULL ) {
		return;
	}
	Ni3DDeviceDesc *device = GetDisplayDevice();
	if ( device == NULL ) {
		return;
	}

	TGModule::CallVirtualSlot06();
	NiD3DRender *renderer = NiD3DRender::Create( mode, width, height, flags, (HWND)window,
												 NiApplication::application->GetAppWindow()->GetWindow(), device );
	if ( renderer == NULL ) {
		TGModule::CallVirtualSlots03And04();
		TGModule::CallVirtualSlot05();
	}
	TGModule::CallVirtualSlot07( (int)renderer );

	g_kIconManager.SetScreenWidth( (float)width );
	g_kIconManager.SetScreenHeight( (float)height );
	NiApplication::application->GetCamera()->Clear( 1 );
	NiApplication::application->GetCamera()->Click();
	NiApplication::application->GetCamera()->Unknown7E81B0();
	movieMode = false;
}

/*
================
TGMovieManager::SetMovieVolume

Sets the volume of every movie playing, 0 (silent) to 1.
================
*/
// FUNCTION: STBC 0x006AFC20
void TGMovieManager::SetMovieVolume( float volume_ ) {
	if ( volume_ < 0.0f ) {
		volume_ = 0.0f;
	} else if ( volume_ > 1.0f ) {
		volume_ = 1.0f;
	}
	volume = volume_;

	for ( int i = 0; i < numMovies; i++ ) {
		if ( movies[i].bink != NULL ) {
			BinkSetVolume( movies[i].bink, 0, (int)( volume * 32768.0f ) );
		}
	}
}

/*
================
TGMovieManager::GetRendererMode
================
*/
// FUNCTION: STBC 0x006afca0
int TGMovieManager::GetRendererMode( void ) {
	return savedWindowed == false;
}

/*
================
TGMovieManager::GetRendererWidth
================
*/
// FUNCTION: STBC 0x006afcb0
unsigned int TGMovieManager::GetRendererWidth( void ) {
	if ( movieMode ) {
		return savedWidth;
	}
	return 640;
}

/*
================
TGMovieManager::GetRendererHeight
================
*/
// FUNCTION: STBC 0x006afcd0
unsigned int TGMovieManager::GetRendererHeight( void ) {
	if ( movieMode ) {
		return savedHeight;
	}
	return 480;
}

/*
================
TGMovieManager::GetRendererFlags
================
*/
// FUNCTION: STBC 0x006afcf0
unsigned int TGMovieManager::GetRendererFlags( void ) {
	unsigned int flags = ( savedBitDepth == 16 ) ? 8 : 16;
	if ( unknown5D ) {
		return flags | 2;
	}
	return flags | 1;
}

/*
================
TGMovieManager::GetRendererWindow
================
*/
// FUNCTION: STBC 0x006afd10
void *TGMovieManager::GetRendererWindow( void ) {
	if ( savedWindowed ) {
		return NiApplication::application->GetAppWindow()->GetRenderWindow();
	}
	return NiApplication::application->GetAppWindow()->GetWindow();
}

/*
================
TGMovieManager::GetDisplayDevice
================
*/
// FUNCTION: STBC 0x006afd30
Ni3DDeviceDesc *TGMovieManager::GetDisplayDevice( void ) {
	return savedDevice;
}

/*
================
TGMovieManager::WriteToStream

Each movie's action, file and frame, then the movie mode.
================
*/
// FUNCTION: STBC 0x006afd40
bool TGMovieManager::WriteToStream( TGStream *stream ) {
	stream->WriteInt( numMovies );
	for ( int i = 0; i < numMovies; i++ ) {
		stream->WriteID( TGObject::GetSavedID( movies[i].action ) );
		if ( movies[i].action != NULL ) {
			stream->VirtualSlot11( movies[i].action->GetMovie() );
			BINKREALTIME realtime;
			BinkGetRealtime( movies[i].bink, &realtime, 0 );
			stream->WriteLong( realtime.FrameNum );
		}
	}
	stream->WriteBool( movieMode );
	return true;
}

/*
================
TGMovieManager::ReadFromStream

The actions are read as IDs; PostFixUp turns them into pointers.
================
*/
// FUNCTION: STBC 0x006afdd0
bool TGMovieManager::ReadFromStream( TGStream *stream ) {
	StopAllMovies();
	int count = stream->ReadInt();
	numMovies = 0;
	for ( int i = 0; i < count; i++ ) {
		TGMovieAction *action = (TGMovieAction *)stream->ReadID();
		if ( action != NULL ) {
			bool added = AddMovie( stream->VirtualSlot10( -1 ), action, true );
			long frame = stream->ReadLong();
			if ( added ) {
				BinkGoto( movies[numMovies - 1].bink, frame, BINKGOTOQUICK );
			}
		}
	}

	if ( stream->ReadBool() ) {
		if ( forcedMovie == NULL ) {
			SwitchToMovieMode();
			return true;
		}
		movieMode = true;
	}
	return true;
}

/*
================
TGMovieManager::FixUpPointers

False while an action is not loaded yet.
================
*/
// FUNCTION: STBC 0x006afe80
bool TGMovieManager::FixUpPointers( void ) {
	for ( int i = 0; i < numMovies; i++ ) {
		if ( movies[i].action != NULL && TGObject::GetTGObjectPtr( (long)movies[i].action ) == NULL ) {
			return false;
		}
	}
	return true;
}

/*
================
TGMovieManager::PostFixUp
================
*/
// FUNCTION: STBC 0x006afec0
void TGMovieManager::PostFixUp( void ) {
	for ( int i = 0; i < numMovies; i++ ) {
		movies[i].action = (TGMovieAction *)TGObject::GetSavedObject( (long)movies[i].action );
	}
}

/*
================
TGMovieManager::LoadForcedMovie

Opens the movie and clears the screen through a camera of its own.
================
*/
// FUNCTION: STBC 0x006afef0
void TGMovieManager::LoadForcedMovie( char *file ) {
	BinkSetSoundSystem( BinkOpenMiles, (unsigned long)NiSoundSystem::GetSoundSystem()->GetDriver() );
	forcedMovie = BinkOpen( file, BINKNOSKIP );
	BinkSetVolume( forcedMovie, 0, (int)( volume * 32768.0f ) );

	NiPointer<NiCamera> camera = new NiCamera;
	NiRender *renderer = UtopiaApp::GetRenderer();
	camera->SetRenderer( renderer );
	camera->Clear( 1 );
	camera->Unknown7E81B0();
	camera->Clear( 1 );
	camera->Click();
}

/*
================
TGMovieManager::PlayForcedMovieFrame

Decodes the forced movie's next frame into the back buffer, centred, and
loops the movie at its end; the screen is presented when the frame was
due.
================
*/
// FUNCTION: STBC 0x006b0030
// PARKED: inline budget and tail duplication: retail releases the renderer temporary through the out-of-line DecRefCount (0x40CFE0), keeps one more frame slot, and gives each give-up return of the two retry loops its own copy of the NiPointer releases where this build shares one
void TGMovieManager::PlayForcedMovieFrame( void ) {
	if ( forcedMovie == NULL ) {
		return;
	}

	NiD3DRender *renderer = (NiD3DRender *)(NiRender *)UtopiaApp::GetRenderer();
	if ( renderer == NULL ) {
		return;
	}

	// BUG: fetched as OpenBuffer does, and never used
	IDirectDraw7 *directDraw = renderer->Unknown7C1320();
	NiPointer<NiImage> image = (NiImage *)renderer->VirtualSlot22( 1 );
	NiPointer<NiDDBufferImage> bufferImage = NiDynamicCast( NiDDBufferImage, image );

	IDirectDrawSurface7 *surface;
	bufferImage->Unknown7C70C0( &surface );
	DDSURFACEDESC2 desc;
	desc.dwSize = sizeof( desc );
	int surfaceType = BinkDDSurfaceType( surface );

	BINK *bink = forcedMovie;
	int waiting = BinkWait( bink );
	if ( !waiting ) {
		BinkDoFrame( bink );
	}

	int lockTries = 0;
	for ( ;; ) {
		HRESULT result = surface->Lock( NULL, &desc, 0, NULL );
		if ( result == DDERR_SURFACELOST ) {
			surface->Restore();
		} else if ( result != DDERR_WASSTILLDRAWING && result != DDERR_SURFACEBUSY && result != DDERR_SURFACEISOBSCURED ) {
			break;
		}
		if ( lockTries++ > 5 ) {
			return;
		}
	}

	unsigned int height = renderer->VirtualSlot20();
	unsigned int width = renderer->VirtualSlot19();
	BinkCopyToBuffer( bink, desc.lpSurface, desc.lPitch, desc.dwHeight, ( width - bink->Width ) >> 1,
					  ( height - bink->Height ) >> 1, surfaceType | BINKCOPYALL );
	if ( bink->FrameNum == bink->Frames - 1 ) {
		BinkGoto( bink, 0, 0 );
	} else if ( !waiting ) {
		BinkNextFrame( bink );
	}

	int unlockTries = 0;
	while ( surface->Unlock( NULL ) != DD_OK ) {
		if ( unlockTries++ > 5 ) {
			return;
		}
	}

	if ( !waiting ) {
		NiApplication::application->GetCamera()->Unknown7E81B0();
	}
}

/*
================
TGMovieManager::CloseForcedMovie

Closes the forced movie, if there is one.
================
*/
// FUNCTION: STBC 0x006B0360
void TGMovieManager::CloseForcedMovie( void ) {
	if ( forcedMovie != NULL ) {
		BinkClose( forcedMovie );
		forcedMovie = NULL;
	}
}
