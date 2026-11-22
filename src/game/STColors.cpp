/*
===============================================================================

	STColors.cpp

	The interface colours (STColors.h), and the colour schemes of the
	menus. The scripts set the colours; only the button colours and the
	condition colours have values of their own.

===============================================================================
*/

#include "STColors.h"
#include "STButton.h"
#include "STMenu.h"
#include <iostream>
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x0051f1d0 _$E710
// FUNCTION: STBC 0x0051f1f0 _$E709

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0051ec30 _$E37
// FUNCTION: STBC 0x0051ec40 _$E36
// FUNCTION: STBC 0x0051ec50 _$E41
// FUNCTION: STBC 0x0051ec60 _$E40
// FUNCTION: STBC 0x0051ec70 _$E52
// FUNCTION: STBC 0x0051ec80 _$E51
// FUNCTION: STBC 0x0051ec90 _$E56
// FUNCTION: STBC 0x0051eca0 _$E55
// FUNCTION: STBC 0x0051ecb0 _$E60
// FUNCTION: STBC 0x0051ecc0 _$E59
// FUNCTION: STBC 0x0051ecd0 _$E64
// FUNCTION: STBC 0x0051ece0 _$E63
// FUNCTION: STBC 0x0051ecf0 _$E184
// FUNCTION: STBC 0x0051ed00 _$E183
// FUNCTION: STBC 0x0051ed10 _$E126
// FUNCTION: STBC 0x0051ed20 _$E125
// FUNCTION: STBC 0x0051ed30 _$E139
// FUNCTION: STBC 0x0051ed40 _$E138
// FUNCTION: STBC 0x0051ed50 _$E189
// FUNCTION: STBC 0x0051ed60 _$E188
// FUNCTION: STBC 0x0051ed70 _$E194
// FUNCTION: STBC 0x0051ed80 _$E193
// FUNCTION: STBC 0x0051ed90 _$E198
// FUNCTION: STBC 0x0051eda0 _$E197
// FUNCTION: STBC 0x0051edb0 _$E202
// FUNCTION: STBC 0x0051edc0 _$E201
// FUNCTION: STBC 0x0051edd0 _$E209
// FUNCTION: STBC 0x0051ede0 _$E208
// FUNCTION: STBC 0x0051edf0 _$E268
// FUNCTION: STBC 0x0051ee00 _$E267
// FUNCTION: STBC 0x0051ee10 _$E272
// FUNCTION: STBC 0x0051ee20 _$E271
// FUNCTION: STBC 0x0051ee30 _$E88
// FUNCTION: STBC 0x0051ee40 _$E87
// FUNCTION: STBC 0x0051ee50 _$E254
// FUNCTION: STBC 0x0051ee60 _$E253
// FUNCTION: STBC 0x0051ee70 _$E261
// FUNCTION: STBC 0x0051ee80 _$E260
// FUNCTION: STBC 0x0051ee90 _$E284
// FUNCTION: STBC 0x0051eea0 _$E283
// FUNCTION: STBC 0x0051eeb0 _$E288
// FUNCTION: STBC 0x0051eec0 _$E287
// FUNCTION: STBC 0x0051eed0 _$E328
// FUNCTION: STBC 0x0051eee0 _$E327
// FUNCTION: STBC 0x0051eef0 _$E334
// FUNCTION: STBC 0x0051ef00 _$E333
// FUNCTION: STBC 0x0051ef10 _$E276
// FUNCTION: STBC 0x0051ef20 _$E275
// FUNCTION: STBC 0x0051ef30 _$E280
// FUNCTION: STBC 0x0051ef40 _$E279
// FUNCTION: STBC 0x0051ef50 _$E292
// FUNCTION: STBC 0x0051ef60 _$E291
// FUNCTION: STBC 0x0051ef70 _$E296
// FUNCTION: STBC 0x0051ef80 _$E295
// FUNCTION: STBC 0x0051ef90 _$E300
// FUNCTION: STBC 0x0051efa0 _$E299
// FUNCTION: STBC 0x0051efb0 _$E304
// FUNCTION: STBC 0x0051efc0 _$E303
// FUNCTION: STBC 0x0051efd0 _$E308
// FUNCTION: STBC 0x0051efe0 _$E307
// FUNCTION: STBC 0x0051eff0 _$E312
// FUNCTION: STBC 0x0051f000 _$E311
// FUNCTION: STBC 0x0051f010 _$E316
// FUNCTION: STBC 0x0051f020 _$E315
// FUNCTION: STBC 0x0051f030 _$E320
// FUNCTION: STBC 0x0051f040 _$E319
// FUNCTION: STBC 0x0051f050 _$E93
// FUNCTION: STBC 0x0051f060 _$E92
// FUNCTION: STBC 0x0051f070 _$E324
// FUNCTION: STBC 0x0051f080 _$E323
// FUNCTION: STBC 0x0051f090 _$E98
// FUNCTION: STBC 0x0051f0a0 _$E97
// FUNCTION: STBC 0x0051f0b0 _$E338
// FUNCTION: STBC 0x0051f0c0 _$E337
// FUNCTION: STBC 0x0051f0d0 _$E342
// FUNCTION: STBC 0x0051f0e0 _$E341
// FUNCTION: STBC 0x0051f0f0 _$E346
// FUNCTION: STBC 0x0051f100 _$E345
// FUNCTION: STBC 0x0051f110 _$E350
// FUNCTION: STBC 0x0051f120 _$E349
// FUNCTION: STBC 0x0051f130 _$E103
// FUNCTION: STBC 0x0051f140 _$E102
// FUNCTION: STBC 0x0051f150 _$E354
// FUNCTION: STBC 0x0051f160 _$E353
// FUNCTION: STBC 0x0051f170 _$E358
// FUNCTION: STBC 0x0051f180 _$E357
// FUNCTION: STBC 0x0051f190 _$E362
// FUNCTION: STBC 0x0051f1a0 _$E361
// FUNCTION: STBC 0x0051f1b0 _$E109
// FUNCTION: STBC 0x0051f1c0 _$E108
// FUNCTION: STBC 0x0051f200 _$E114
// FUNCTION: STBC 0x0051f210 _$E113
// FUNCTION: STBC 0x0051f220 _$E144
// FUNCTION: STBC 0x0051f230 _$E143
// FUNCTION: STBC 0x0051f240 _$E366
// FUNCTION: STBC 0x0051f250 _$E365
// FUNCTION: STBC 0x0051f260 _$E149
// FUNCTION: STBC 0x0051f270 _$E148
// FUNCTION: STBC 0x0051f280 _$E370
// FUNCTION: STBC 0x0051f290 _$E369
// FUNCTION: STBC 0x0051f2a0 _$E374
// FUNCTION: STBC 0x0051f2b0 _$E373
// FUNCTION: STBC 0x0051f2c0 _$E154
// FUNCTION: STBC 0x0051f2d0 _$E153
// FUNCTION: STBC 0x0051f2e0 _$E378
// FUNCTION: STBC 0x0051f2f0 _$E377
// FUNCTION: STBC 0x0051f300 _$E119
// FUNCTION: STBC 0x0051f310 _$E118
// FUNCTION: STBC 0x0051f320 _$E382
// FUNCTION: STBC 0x0051f330 _$E381
// FUNCTION: STBC 0x0051f340 _$E386
// FUNCTION: STBC 0x0051f350 _$E385
// FUNCTION: STBC 0x0051f360 _$E159
// FUNCTION: STBC 0x0051f370 _$E158
// FUNCTION: STBC 0x0051f380 _$E164
// FUNCTION: STBC 0x0051f390 _$E163
// FUNCTION: STBC 0x0051f3a0 _$E390
// FUNCTION: STBC 0x0051f3b0 _$E389
// FUNCTION: STBC 0x0051f3c0 _$E394
// FUNCTION: STBC 0x0051f3d0 _$E393
// FUNCTION: STBC 0x0051f3e0 _$E398
// FUNCTION: STBC 0x0051f3f0 _$E397
// FUNCTION: STBC 0x0051f400 _$E402
// FUNCTION: STBC 0x0051f410 _$E401
// FUNCTION: STBC 0x0051f420 _$E406
// FUNCTION: STBC 0x0051f430 _$E405
// FUNCTION: STBC 0x0051f440 _$E410
// FUNCTION: STBC 0x0051f450 _$E409
// FUNCTION: STBC 0x0051f460 _$E414
// FUNCTION: STBC 0x0051f470 _$E413
// FUNCTION: STBC 0x0051f480 _$E418
// FUNCTION: STBC 0x0051f490 _$E417
// FUNCTION: STBC 0x0051f4a0 _$E169
// FUNCTION: STBC 0x0051f4b0 _$E168
// FUNCTION: STBC 0x0051f4c0 _$E422
// FUNCTION: STBC 0x0051f4d0 _$E421
// FUNCTION: STBC 0x0051f4e0 _$E174
// FUNCTION: STBC 0x0051f4f0 _$E173
// FUNCTION: STBC 0x0051f500 _$E179
// FUNCTION: STBC 0x0051f510 _$E178
// FUNCTION: STBC 0x0051f520 _$E426
// FUNCTION: STBC 0x0051f530 _$E425
// FUNCTION: STBC 0x0051f540 _$E431
// FUNCTION: STBC 0x0051f550 _$E430
// FUNCTION: STBC 0x0051f560 _$E435
// FUNCTION: STBC 0x0051f570 _$E434

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0051ebb0 _$E240
// FUNCTION: STBC 0x0051ebc0 _$E237
// FUNCTION: STBC 0x0051ebd0 _$E239
// FUNCTION: STBC 0x0051ebe0 _$E238
// FUNCTION: STBC 0x0051ebf0 _$E246
// FUNCTION: STBC 0x0051ec00 _$E243
// FUNCTION: STBC 0x0051ec10 _$E245
// FUNCTION: STBC 0x0051ec20 _$E244

// The colours' dynamic initialisers (each NiColorA global is constructed at
// start-up, zero unless given a value).
// FUNCTION: STBC 0x0051d040 _$E438
// FUNCTION: STBC 0x0051d050 _$E437
// FUNCTION: STBC 0x0051d2e0 _$E441
// FUNCTION: STBC 0x0051d2f0 _$E440
// FUNCTION: STBC 0x0051d320 _$E444
// FUNCTION: STBC 0x0051d330 _$E443
// FUNCTION: STBC 0x0051d360 _$E447
// FUNCTION: STBC 0x0051d370 _$E446
// FUNCTION: STBC 0x0051d3a0 _$E450
// FUNCTION: STBC 0x0051d3b0 _$E449
// FUNCTION: STBC 0x0051d3e0 _$E453
// FUNCTION: STBC 0x0051d3f0 _$E452
// FUNCTION: STBC 0x0051d420 _$E456
// FUNCTION: STBC 0x0051d430 _$E455
// FUNCTION: STBC 0x0051d460 _$E459
// FUNCTION: STBC 0x0051d470 _$E458
// FUNCTION: STBC 0x0051d4a0 _$E462
// FUNCTION: STBC 0x0051d4b0 _$E461
// FUNCTION: STBC 0x0051d4e0 _$E465
// FUNCTION: STBC 0x0051d4f0 _$E464
// FUNCTION: STBC 0x0051d520 _$E468
// FUNCTION: STBC 0x0051d530 _$E467
// FUNCTION: STBC 0x0051d560 _$E471
// FUNCTION: STBC 0x0051d570 _$E470
// FUNCTION: STBC 0x0051d5a0 _$E474
// FUNCTION: STBC 0x0051d5b0 _$E473
// FUNCTION: STBC 0x0051d5e0 _$E477
// FUNCTION: STBC 0x0051d5f0 _$E476
// FUNCTION: STBC 0x0051d620 _$E480
// FUNCTION: STBC 0x0051d630 _$E479
// FUNCTION: STBC 0x0051d660 _$E483
// FUNCTION: STBC 0x0051d670 _$E482
// FUNCTION: STBC 0x0051d6a0 _$E486
// FUNCTION: STBC 0x0051d6b0 _$E485
// FUNCTION: STBC 0x0051d6e0 _$E489
// FUNCTION: STBC 0x0051d6f0 _$E488
// FUNCTION: STBC 0x0051d720 _$E492
// FUNCTION: STBC 0x0051d730 _$E491
// FUNCTION: STBC 0x0051d760 _$E495
// FUNCTION: STBC 0x0051d770 _$E494
// FUNCTION: STBC 0x0051d7a0 _$E498
// FUNCTION: STBC 0x0051d7b0 _$E497
// FUNCTION: STBC 0x0051d7e0 _$E501
// FUNCTION: STBC 0x0051d7f0 _$E500
// FUNCTION: STBC 0x0051d820 _$E504
// FUNCTION: STBC 0x0051d830 _$E503
// FUNCTION: STBC 0x0051d860 _$E507
// FUNCTION: STBC 0x0051d870 _$E506
// FUNCTION: STBC 0x0051d8a0 _$E510
// FUNCTION: STBC 0x0051d8b0 _$E509
// FUNCTION: STBC 0x0051d8e0 _$E513
// FUNCTION: STBC 0x0051d8f0 _$E512
// FUNCTION: STBC 0x0051d920 _$E516
// FUNCTION: STBC 0x0051d930 _$E515
// FUNCTION: STBC 0x0051d960 _$E519
// FUNCTION: STBC 0x0051d970 _$E518
// FUNCTION: STBC 0x0051d9a0 _$E522
// FUNCTION: STBC 0x0051d9b0 _$E521
// FUNCTION: STBC 0x0051d9e0 _$E525
// FUNCTION: STBC 0x0051d9f0 _$E524
// FUNCTION: STBC 0x0051da20 _$E528
// FUNCTION: STBC 0x0051da30 _$E527
// FUNCTION: STBC 0x0051da60 _$E531
// FUNCTION: STBC 0x0051da70 _$E530
// FUNCTION: STBC 0x0051daa0 _$E534
// FUNCTION: STBC 0x0051dab0 _$E533
// FUNCTION: STBC 0x0051dae0 _$E537
// FUNCTION: STBC 0x0051daf0 _$E536
// FUNCTION: STBC 0x0051db20 _$E540
// FUNCTION: STBC 0x0051db30 _$E539
// FUNCTION: STBC 0x0051db60 _$E543
// FUNCTION: STBC 0x0051db70 _$E542
// FUNCTION: STBC 0x0051dba0 _$E546
// FUNCTION: STBC 0x0051dbb0 _$E545
// FUNCTION: STBC 0x0051dbe0 _$E549
// FUNCTION: STBC 0x0051dbf0 _$E548
// FUNCTION: STBC 0x0051dc20 _$E552
// FUNCTION: STBC 0x0051dc30 _$E551
// FUNCTION: STBC 0x0051dc60 _$E555
// FUNCTION: STBC 0x0051dc70 _$E554
// FUNCTION: STBC 0x0051dca0 _$E558
// FUNCTION: STBC 0x0051dcb0 _$E557
// FUNCTION: STBC 0x0051dce0 _$E561
// FUNCTION: STBC 0x0051dcf0 _$E560
// FUNCTION: STBC 0x0051dd20 _$E564
// FUNCTION: STBC 0x0051dd30 _$E563
// FUNCTION: STBC 0x0051dd60 _$E567
// FUNCTION: STBC 0x0051dd70 _$E566
// FUNCTION: STBC 0x0051dda0 _$E570
// FUNCTION: STBC 0x0051ddb0 _$E569
// FUNCTION: STBC 0x0051dde0 _$E573
// FUNCTION: STBC 0x0051ddf0 _$E572
// FUNCTION: STBC 0x0051de20 _$E576
// FUNCTION: STBC 0x0051de30 _$E575
// FUNCTION: STBC 0x0051de60 _$E579
// FUNCTION: STBC 0x0051de70 _$E578
// FUNCTION: STBC 0x0051e170 _$E582
// FUNCTION: STBC 0x0051e180 _$E581
// FUNCTION: STBC 0x0051e1b0 _$E585
// FUNCTION: STBC 0x0051e1c0 _$E584
// FUNCTION: STBC 0x0051e1f0 _$E588
// FUNCTION: STBC 0x0051e200 _$E587
// FUNCTION: STBC 0x0051e230 _$E591
// FUNCTION: STBC 0x0051e240 _$E590
// FUNCTION: STBC 0x0051e270 _$E594
// FUNCTION: STBC 0x0051e280 _$E593
// FUNCTION: STBC 0x0051e2b0 _$E597
// FUNCTION: STBC 0x0051e2c0 _$E596
// FUNCTION: STBC 0x0051e2f0 _$E600
// FUNCTION: STBC 0x0051e300 _$E599
// FUNCTION: STBC 0x0051e330 _$E603
// FUNCTION: STBC 0x0051e340 _$E602
// FUNCTION: STBC 0x0051e370 _$E606
// FUNCTION: STBC 0x0051e380 _$E605
// FUNCTION: STBC 0x0051e3b0 _$E609
// FUNCTION: STBC 0x0051e3c0 _$E608
// FUNCTION: STBC 0x0051e3f0 _$E612
// FUNCTION: STBC 0x0051e400 _$E611
// FUNCTION: STBC 0x0051e430 _$E615
// FUNCTION: STBC 0x0051e440 _$E614
// FUNCTION: STBC 0x0051e470 _$E618
// FUNCTION: STBC 0x0051e480 _$E617
// FUNCTION: STBC 0x0051e4b0 _$E621
// FUNCTION: STBC 0x0051e4c0 _$E620
// FUNCTION: STBC 0x0051e4f0 _$E624
// FUNCTION: STBC 0x0051e500 _$E623
// FUNCTION: STBC 0x0051e530 _$E627
// FUNCTION: STBC 0x0051e540 _$E626
// FUNCTION: STBC 0x0051e570 _$E630
// FUNCTION: STBC 0x0051e580 _$E629
// FUNCTION: STBC 0x0051e5b0 _$E633
// FUNCTION: STBC 0x0051e5c0 _$E632
// FUNCTION: STBC 0x0051e5f0 _$E636
// FUNCTION: STBC 0x0051e600 _$E635
// FUNCTION: STBC 0x0051e630 _$E639
// FUNCTION: STBC 0x0051e640 _$E638
// FUNCTION: STBC 0x0051e670 _$E642
// FUNCTION: STBC 0x0051e680 _$E641
// FUNCTION: STBC 0x0051e6b0 _$E645
// FUNCTION: STBC 0x0051e6c0 _$E644
// FUNCTION: STBC 0x0051e6f0 _$E648
// FUNCTION: STBC 0x0051e700 _$E647
// FUNCTION: STBC 0x0051e730 _$E651
// FUNCTION: STBC 0x0051e740 _$E650
// FUNCTION: STBC 0x0051e770 _$E654
// FUNCTION: STBC 0x0051e780 _$E653
// FUNCTION: STBC 0x0051e7b0 _$E657
// FUNCTION: STBC 0x0051e7c0 _$E656
// FUNCTION: STBC 0x0051e7f0 _$E660
// FUNCTION: STBC 0x0051e800 _$E659
// FUNCTION: STBC 0x0051e830 _$E663
// FUNCTION: STBC 0x0051e840 _$E662
// FUNCTION: STBC 0x0051e870 _$E666
// FUNCTION: STBC 0x0051e880 _$E665
// FUNCTION: STBC 0x0051e8b0 _$E669
// FUNCTION: STBC 0x0051e8c0 _$E668
// FUNCTION: STBC 0x0051e8f0 _$E672
// FUNCTION: STBC 0x0051e900 _$E671
// FUNCTION: STBC 0x0051e930 _$E675
// FUNCTION: STBC 0x0051e940 _$E674
// FUNCTION: STBC 0x0051e970 _$E678
// FUNCTION: STBC 0x0051e980 _$E677
// FUNCTION: STBC 0x0051e9b0 _$E681
// FUNCTION: STBC 0x0051e9c0 _$E680
// FUNCTION: STBC 0x0051e9f0 _$E684
// FUNCTION: STBC 0x0051ea00 _$E683
// FUNCTION: STBC 0x0051ea30 _$E687
// FUNCTION: STBC 0x0051ea40 _$E686
// FUNCTION: STBC 0x0051ea70 _$E690
// FUNCTION: STBC 0x0051ea80 _$E689
// FUNCTION: STBC 0x0051eab0 _$E693
// FUNCTION: STBC 0x0051eac0 _$E692
// FUNCTION: STBC 0x0051eaf0 _$E696
// FUNCTION: STBC 0x0051eb00 _$E695
// FUNCTION: STBC 0x0051eb30 _$E699
// FUNCTION: STBC 0x0051eb40 _$E698
// FUNCTION: STBC 0x0051eb70 _$E702
// FUNCTION: STBC 0x0051eb80 _$E701

// DATA: STBC 0x009889f0
NiColorA		g_kSTButtonColors[STBUTTON_NUM_UI_HEIGHTS][STBUTTON_NUM_COLORS] = {
	{
		NiColorA( 216.0f / 255.0f, 94.0f / 255.0f, 86.0f / 255.0f, 1.0f ),
		NiColorA( 127.0f / 255.0f, 60.0f / 255.0f, 43.0f / 255.0f, 1.0f ),
		NiColorA( 254.0f / 255.0f, 120.0f / 255.0f, 86.0f / 255.0f, 1.0f ),
		NiColorA( 0.25f, 0.25f, 0.25f, 1.0f )
	},
	{
		NiColorA( 147.0f / 255.0f, 103.0f / 255.0f, 1.0f, 1.0f ),
		NiColorA( 86.5f / 255.0f, 66.0f / 255.0f, 127.5f / 255.0f, 1.0f ),
		NiColorA( 173.0f / 255.0f, 132.0f / 255.0f, 1.0f, 1.0f ),
		NiColorA( 0.25f, 0.25f, 0.25f, 1.0f )
	},
	{
		NiColorA( 207.0f / 255.0f, 96.0f / 255.0f, 159.0f / 255.0f, 1.0f ),
		NiColorA( 103.5f / 255.0f, 48.0f / 255.0f, 79.5f / 255.0f, 1.0f ),
		NiColorA( 246.0f / 255.0f, 147.0f / 255.0f, 204.0f / 255.0f, 1.0f ),
		NiColorA( 0.25f, 0.25f, 0.25f, 1.0f )
	},
	{
		NiColorA( 144.0f / 255.0f, 103.0f / 255.0f, 144.0f / 255.0f, 1.0f ),
		NiColorA( 72.0f / 255.0f, 51.5f / 255.0f, 72.0f / 255.0f, 1.0f ),
		NiColorA( 175.0f / 255.0f, 144.0f / 255.0f, 175.0f / 255.0f, 1.0f ),
		NiColorA( 0.25f, 0.25f, 0.25f, 1.0f )
	}
};

// DATA: STBC 0x00988718
NiColorA		g_kSTButtonMarkerDefault;

// DATA: STBC 0x00988768
NiColorA		g_kSTButtonMarkerHighlighted;

// DATA: STBC 0x00988938
NiColorA		g_kSTButtonMarkerSelected;

// DATA: STBC 0x00988320
NiColorA		g_kSTButtonMarkerGray;

// DATA: STBC 0x00988380
NiColorA		g_kSTButtonCheckmarkOn;

// DATA: STBC 0x00988558
NiColorA		g_kSTButtonCheckmarkOff;

// DATA: STBC 0x00988598
NiColorA		g_kSTMenuArrowColor;

// DATA: STBC 0x00988af0
NiColorA		g_kSTMenu1NormalBase;

// DATA: STBC 0x009883a0
NiColorA		g_kSTMenu1HighlightedBase;

// DATA: STBC 0x00988998
NiColorA		g_kSTMenu1Disabled;

// DATA: STBC 0x009886a0
NiColorA		g_kSTMenu1OpenedHighlightedBase;

// DATA: STBC 0x009886d8
NiColorA		g_kSTMenu1Selected;

// DATA: STBC 0x00988610
NiColorA		g_kSTMenu2NormalBase;

// DATA: STBC 0x009885b0
NiColorA		g_kSTMenu2HighlightedBase;

// DATA: STBC 0x00988350
NiColorA		g_kSTMenu2Disabled;

// DATA: STBC 0x009888a8
NiColorA		g_kSTMenu2OpenedHighlightedBase;

// DATA: STBC 0x009887e8
NiColorA		g_kSTMenu2Selected;

// DATA: STBC 0x009888e0
NiColorA		g_kSTMenu3NormalBase;

// DATA: STBC 0x00988bb0
NiColorA		g_kSTMenu3HighlightedBase;

// DATA: STBC 0x00988920
NiColorA		g_kSTMenu3Disabled;

// DATA: STBC 0x00988738
NiColorA		g_kSTMenu3OpenedHighlightedBase;

// DATA: STBC 0x00988b88
NiColorA		g_kSTMenu3Selected;

// DATA: STBC 0x00988568
NiColorA		g_kSTMenu4NormalBase;

// DATA: STBC 0x00988b30
NiColorA		g_kSTMenu4HighlightedBase;

// DATA: STBC 0x00988338
NiColorA		g_kSTMenu4Disabled;

// DATA: STBC 0x00988578
NiColorA		g_kSTMenu4OpenedHighlightedBase;

// DATA: STBC 0x009885f0
NiColorA		g_kSTMenu4Selected;

// DATA: STBC 0x009888f0
NiColorA		g_kSTMenuTextColor;

// DATA: STBC 0x00988638
NiColorA		g_kSTMenuTextSelectedColor;

// DATA: STBC 0x009888b8
NiColorA		g_kSTMenuTextHighlightColor;

// DATA: STBC 0x00988678
NiColorA		g_kTextEntryColor;

// DATA: STBC 0x00988ba0
NiColorA		g_kTextHighlightColor;

// DATA: STBC 0x00988b78
NiColorA		g_kTextEntryBackgroundColor;

// DATA: STBC 0x00988b60
NiColorA		g_kTextEntryBackgroundHighlightColor;

// DATA: STBC 0x00988b50
NiColorA		g_kTIPhotonReadyColor;

// DATA: STBC 0x00988390
NiColorA		g_kTIPhotonNotReadyColor;

// DATA: STBC 0x00988950
NiColorA		g_kSTRadarBorderHighlighted;

// DATA: STBC 0x00988788
NiColorA		g_kTitleColor;

// DATA: STBC 0x00988588
NiColorA		g_kInterfaceBorderColor;

// DATA: STBC 0x009883c0
NiColorA		g_kLeftSeparatorColor;

// DATA: STBC 0x00988be0
NiColorA		g_kRadarBorder;

// DATA: STBC 0x00988868
NiColorA		g_kSTRadarIncomingTorpColor;

// DATA: STBC 0x00988980
NiColorA		g_kRadarFriendlyColor;

// DATA: STBC 0x00988b40
NiColorA		g_kRadarEnemyColor;

// DATA: STBC 0x009885d8
NiColorA		g_kRadarNeutralColor;

// DATA: STBC 0x00988b20
NiColorA		g_kRadarUnknownColor;

// DATA: STBC 0x00988418
NiColorA		g_pConditionColors[] = {
	NiColorA( 0.0f, 0.0f, 0.0f, 0.0f ),
	NiColorA( 0.61f, 0.0f, 0.0f, 1.0f ),
	NiColorA( 0.61f, 0.0f, 0.0f, 1.0f ),
	NiColorA( 1.0f, 0.25f, 0.0f, 1.0f ),
	NiColorA( 1.0f, 0.25f, 0.0f, 1.0f ),
	NiColorA( 1.0f, 0.41f, 0.0f, 1.0f ),
	NiColorA( 1.0f, 0.41f, 0.0f, 1.0f ),
	NiColorA( 1.0f, 0.68f, 0.0f, 1.0f ),
	NiColorA( 1.0f, 0.86f, 0.0f, 1.0f ),
	NiColorA( 1.0f, 1.0f, 0.0f, 1.0f ),
	NiColorA( 1.0f, 1.0f, 0.0f, 1.0f ),
	NiColorA( 1.0f, 1.0f, 0.0f, 1.0f ),
	NiColorA( 0.91f, 1.0f, 0.0f, 1.0f ),
	NiColorA( 0.91f, 1.0f, 0.0f, 1.0f ),
	NiColorA( 0.91f, 1.0f, 0.0f, 1.0f ),
	NiColorA( 0.72f, 1.0f, 0.0f, 1.0f ),
	NiColorA( 0.72f, 1.0f, 0.0f, 1.0f ),
	NiColorA( 0.72f, 1.0f, 0.0f, 1.0f ),
	NiColorA( 0.0f, 1.0f, 0.0f, 1.0f ),
	NiColorA( 0.0f, 1.0f, 0.0f, 1.0f )
};

// DATA: STBC 0x009886c8
NiColorA		g_kSubsystemFillColor;

// DATA: STBC 0x009888d0
NiColorA		g_kSubsystemEmptyColor;

// DATA: STBC 0x00988c48
NiColorA		g_kSubsystemDisabledColor;

// DATA: STBC 0x009889d0
NiColorA		g_kTacWeaponsCtrlHeaderTextColor;

// DATA: STBC 0x009887a8
NiColorA		g_kDamageDisplayDestroyedColor;

// DATA: STBC 0x00988bf8
NiColorA		g_kDamageDisplayDamagedColor;

// DATA: STBC 0x00988858
NiColorA		g_kDamageDisplayDisabledColor;

// DATA: STBC 0x00988690
NiColorA		g_kMainMenuButtonColor;

// DATA: STBC 0x00988c30
NiColorA		g_kMainMenuButtonHighlightedColor;

// DATA: STBC 0x009883f0
NiColorA		g_kMainMenuButtonSelectedColor;

// DATA: STBC 0x009889a8
NiColorA		g_kMainMenuButton1Color;

// DATA: STBC 0x00988890
NiColorA		g_kMainMenuButton1HighlightedColor;

// DATA: STBC 0x00988968
NiColorA		g_kMainMenuButton1SelectedColor;

// DATA: STBC 0x00988800
NiColorA		g_kMainMenuButton2Color;

// DATA: STBC 0x009886b8
NiColorA		g_kMainMenuButton2HighlightedColor;

// DATA: STBC 0x00988600
NiColorA		g_kMainMenuButton2SelectedColor;

// DATA: STBC 0x00988878
NiColorA		g_kMainMenuButton3Color;

// DATA: STBC 0x00988750
NiColorA		g_kMainMenuButton3HighlightedColor;

// DATA: STBC 0x00988b08
NiColorA		g_kMainMenuButton3SelectedColor;

// DATA: STBC 0x00988408
NiColorA		g_kMainMenuBorderMainColor;

// DATA: STBC 0x00988648
NiColorA		g_kMainMenuBorderOffColor;

// DATA: STBC 0x00988bc8
NiColorA		g_kMainMenuBorderBlock1Color;

// DATA: STBC 0x00988910
NiColorA		g_kMainMenuBorderTopColor;

// DATA: STBC 0x00988668
NiColorA		g_kEngineeringShieldsColor;

// DATA: STBC 0x009887c0
NiColorA		g_kEngineeringEnginesColor;

// DATA: STBC 0x00988818
NiColorA		g_kEngineeringWeaponsColor;

// DATA: STBC 0x009889e0
NiColorA		g_kEngineeringSensorsColor;

// DATA: STBC 0x00988728
NiColorA		g_kEngineeringCloakColor;

// DATA: STBC 0x00988830
NiColorA		g_kEngineeringTractorColor;

// DATA: STBC 0x009889c0
NiColorA		g_kEngineeringWarpCoreColor;

// DATA: STBC 0x00988900
NiColorA		g_kEngineeringMainPowerColor;

// DATA: STBC 0x00988778
NiColorA		g_kEngineeringBackupPowerColor;

// DATA: STBC 0x009887d0
NiColorA		g_kEngineeringCtrlBkgndLineColor;

// DATA: STBC 0x00988c10
NiColorA		g_kQuickBattleBrightRed;

// DATA: STBC 0x00988708
NiColorA		g_kMultiplayerBorderBlue;

// DATA: STBC 0x00988840
NiColorA		g_kMultiplayerBorderPurple;

// DATA: STBC 0x00988368
NiColorA		g_kMultiplayerStylizedPurple;

// DATA: STBC 0x009886e8
NiColorA		g_kMultiplayerButtonOrange;

// DATA: STBC 0x00988658
NiColorA		g_kMultiplayerButtonPurple;

// A colour no code reads; it is not a SWIG variable.
// DATA: STBC 0x009883d8
NiColorA		g_kUnknown9883D8;

// DATA: STBC 0x00988628
NiColorA		g_kMultiplayerDividerPurple;

/*
===============================================================================

	The menu colour schemes (invented names): for each depth, the title bar's and the
	name's colours of each STMENU_STATE_*. The third and fourth depths
	keep the name's normal colour when highlighted.

===============================================================================
*/

// DATA: STBC 0x008e28ec
static STMenu::STMenuColorScheme menu1ColorScheme = {
	{
		{ &g_kSTMenu1NormalBase, &g_kSTMenuTextColor },
		{ &g_kSTMenu1HighlightedBase, &g_kSTMenuTextHighlightColor },
		{ &g_kSTMenu1NormalBase, &g_kSTMenuTextColor },
		{ &g_kSTMenu1Disabled, &g_kSTMenuTextColor },
		{ &g_kSTMenu1OpenedHighlightedBase, &g_kSTMenuTextHighlightColor },
		{ &g_kSTMenu1Disabled, &g_kSTMenuTextColor },
		{ &g_kSTMenu1NormalBase, &g_kSTMenuTextColor },
		{ &g_kSTMenu1NormalBase, &g_kSTMenuTextHighlightColor },
		{ &g_kSTMenu1Disabled, &g_kSTMenuTextColor },
		{ &g_kSTMenu1Selected, &g_kSTMenuTextColor }
	}
};

// DATA: STBC 0x008e293c
static STMenu::STMenuColorScheme menu2ColorScheme = {
	{
		{ &g_kSTMenu2NormalBase, &g_kSTMenuTextColor },
		{ &g_kSTMenu2HighlightedBase, &g_kSTMenuTextHighlightColor },
		{ &g_kSTMenu2NormalBase, &g_kSTMenuTextColor },
		{ &g_kSTMenu2Disabled, &g_kSTMenuTextColor },
		{ &g_kSTMenu2OpenedHighlightedBase, &g_kSTMenuTextHighlightColor },
		{ &g_kSTMenu2Disabled, &g_kSTMenuTextColor },
		{ &g_kSTMenu2NormalBase, &g_kSTMenuTextColor },
		{ &g_kSTMenu2NormalBase, &g_kSTMenuTextHighlightColor },
		{ &g_kSTMenu2Disabled, &g_kSTMenuTextColor },
		{ &g_kSTMenu2Selected, &g_kSTMenuTextColor }
	}
};

// DATA: STBC 0x008e298c
static STMenu::STMenuColorScheme menu3ColorScheme = {
	{
		{ &g_kSTMenu3NormalBase, &g_kSTMenuTextColor },
		{ &g_kSTMenu3HighlightedBase, &g_kSTMenuTextColor },
		{ &g_kSTMenu3NormalBase, &g_kSTMenuTextColor },
		{ &g_kSTMenu3Disabled, &g_kSTMenuTextColor },
		{ &g_kSTMenu3OpenedHighlightedBase, &g_kSTMenuTextColor },
		{ &g_kSTMenu3Disabled, &g_kSTMenuTextColor },
		{ &g_kSTMenu3NormalBase, &g_kSTMenuTextColor },
		{ &g_kSTMenu3NormalBase, &g_kSTMenuTextColor },
		{ &g_kSTMenu3Disabled, &g_kSTMenuTextColor },
		{ &g_kSTMenu3Selected, &g_kSTMenuTextColor }
	}
};

// DATA: STBC 0x008e29dc
static STMenu::STMenuColorScheme menu4ColorScheme = {
	{
		{ &g_kSTMenu4NormalBase, &g_kSTMenuTextColor },
		{ &g_kSTMenu4HighlightedBase, &g_kSTMenuTextColor },
		{ &g_kSTMenu4NormalBase, &g_kSTMenuTextColor },
		{ &g_kSTMenu4Disabled, &g_kSTMenuTextColor },
		{ &g_kSTMenu4OpenedHighlightedBase, &g_kSTMenuTextColor },
		{ &g_kSTMenu4Disabled, &g_kSTMenuTextColor },
		{ &g_kSTMenu4NormalBase, &g_kSTMenuTextColor },
		{ &g_kSTMenu4NormalBase, &g_kSTMenuTextColor },
		{ &g_kSTMenu4Disabled, &g_kSTMenuTextColor },
		{ &g_kSTMenu4Selected, &g_kSTMenuTextColor }
	}
};

// DATA: STBC 0x008e2a2c
STMenu::STMenuColorScheme *g_kSTMenuColors[STMENU_NUM_UI_HEIGHTS] = {
	&menu1ColorScheme,
	&menu2ColorScheme,
	&menu3ColorScheme,
	&menu4ColorScheme
};
