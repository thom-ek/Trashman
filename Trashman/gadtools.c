/*
 *  Source machine generated by GadToolsBox V2.0b
 *  which is (c) Copyright 1991-1993 Jaba Development
 *
 *  GUI Designed by : Thom/Union
 */

#include <exec/types.h>
#include <intuition/intuition.h>
#include <intuition/classes.h>
#include <intuition/classusr.h>
#include <intuition/imageclass.h>
#include <intuition/gadgetclass.h>
#include <libraries/gadtools.h>
#include <graphics/displayinfo.h>
#include <graphics/gfxbase.h>
#include <clib/exec_protos.h>
#include <clib/intuition_protos.h>
#include <clib/gadtools_protos.h>
#include <clib/graphics_protos.h>
#include <clib/utility_protos.h>
#include <string.h>
#include "gadtools.h"

struct Screen         *Scr = NULL;
UBYTE                 *PubScreenName = NULL;
APTR                   VisualInfo = NULL;
struct Window         *DummyWnd = NULL;
struct Window         *PrefsWnd = NULL;
struct Gadget         *PrefsGList = NULL;
struct Menu           *DummyMenus = NULL;
struct Gadget         *PrefsGadgets[22];
extern Class          *initGet( void );
struct IClass         *getClass = NULL;
struct _Object        *getImage = NULL;
UWORD                  DummyLeft = 0;
UWORD                  DummyTop = 11;
UWORD                  DummyWidth = 392;
UWORD                  DummyHeight = 73;
UWORD                  PrefsLeft = 0;
UWORD                  PrefsTop = 11;
UWORD                  PrefsWidth = 526;
UWORD                  PrefsHeight = 193;
UBYTE                 *DummyWdt = NULL;
UBYTE                 *PrefsWdt = (UBYTE *)"Preferences";
struct TextAttr       *Font, Attr;
UWORD                  FontX, FontY;
UWORD                  OffX, OffY;

UBYTE *PREFS_OVERWRITE1Labels[] = {
	(UBYTE *)"Never",
	(UBYTE *)"Always",
	(UBYTE *)"Ask",
	(UBYTE *)"Same Location",
	NULL };

struct IntuiText PrefsIText[] = {
	2, 0, JAM2,262, 116, NULL, (UBYTE *)"Miscelaenous", NULL,
	2, 0, JAM2,76, 8, NULL, (UBYTE *)"Volumes", NULL,
	2, 0, JAM1,257, 8, NULL, (UBYTE *)"Force Delete", NULL,
	2, 0, JAM1,425, 8, NULL, (UBYTE *)"Overwrite", NULL,
	2, 0, JAM1,429, 84, NULL, (UBYTE *)"Min. free on disk", NULL };

#define Prefs_TNUM 5

struct NewMenu DummyNewMenu[] = {
	NM_TITLE, (STRPTR)"Project", NULL, 0, NULL, NULL,
	NM_ITEM, (STRPTR)"Open...", (STRPTR)"O", 0, 0L, NULL,
	NM_ITEM, (STRPTR)"Save As...", (STRPTR)"A", 0, 0L, NULL,
	NM_ITEM, (STRPTR)NM_BARLABEL, NULL, 0, 0L, NULL,
	NM_ITEM, (STRPTR)"Hide", (STRPTR)"H", 0, 0L, NULL,
	NM_ITEM, (STRPTR)"Quit", (STRPTR)"Q", 0, 0L, NULL,
	NM_TITLE, (STRPTR)"Window", NULL, 0, NULL, NULL,
	NM_ITEM, (STRPTR)"Close", (STRPTR)"K", 0, 0L, NULL,
	NM_ITEM, (STRPTR)"Update", NULL, 0, 0L, NULL,
	NM_ITEM, (STRPTR)"Select Contents", NULL, 0, 0L, NULL,
	NM_ITEM, (STRPTR)"Snapshot", NULL, 0, NULL, NULL,
	NM_SUB, (STRPTR)"Desktop Icon", NULL, 0, 0L, NULL,
	NM_SUB, (STRPTR)"Main Window", NULL, 0, 0L, NULL,
	NM_SUB, (STRPTR)"Trashcan Window", NULL, 0, 0L, NULL,
	NM_ITEM, (STRPTR)"View by", NULL, 0, NULL, NULL,
	NM_SUB, (STRPTR)"Name", NULL, CHECKIT|CHECKED, 14L, NULL,
	NM_SUB, (STRPTR)"Size", NULL, CHECKIT, 13L, NULL,
	NM_SUB, (STRPTR)"Date", NULL, CHECKIT, 11L, NULL,
	NM_SUB, (STRPTR)"Location", NULL, CHECKIT, 7L, NULL,
	NM_TITLE, (STRPTR)"Files", NULL, 0, NULL, NULL,
	NM_ITEM, (STRPTR)"Remove Selected", (STRPTR)"S", 0, 0L, NULL,
	NM_ITEM, (STRPTR)"Empty All Trashcans", (STRPTR)"E", 0, 0L, NULL,
	NM_ITEM, (STRPTR)NM_BARLABEL, NULL, 0, 0L, NULL,
	NM_ITEM, (STRPTR)"Undelete to Orginal", (STRPTR)"U", 0, 0L, NULL,
	NM_ITEM, (STRPTR)"Undelete to Other", (STRPTR)"T", 0, 0L, NULL,
	NM_TITLE, (STRPTR)"Settings", NULL, 0, NULL, NULL,
	NM_ITEM, (STRPTR)"Edit...", (STRPTR)"P", 0, 0L, NULL,
	NM_ITEM, (STRPTR)NM_BARLABEL, NULL, 0, 0L, NULL,
	NM_ITEM, (STRPTR)"About...", NULL, 0, 0L, NULL,
	NM_END, NULL, NULL, 0, 0L, NULL };

UWORD PrefsGTypes[] = {
	BUTTON_KIND,
	BUTTON_KIND,
	BUTTON_KIND,
	CHECKBOX_KIND,
	CHECKBOX_KIND,
	LISTVIEW_KIND,
	BUTTON_KIND,
	BUTTON_KIND,
	CHECKBOX_KIND,
	CHECKBOX_KIND,
	LISTVIEW_KIND,
	BUTTON_KIND,
	BUTTON_KIND,
	MX_KIND,
	INTEGER_KIND,
	CHECKBOX_KIND,
	CHECKBOX_KIND,
	CHECKBOX_KIND,
	CHECKBOX_KIND,
	CHECKBOX_KIND,
	STRING_KIND,
	GENERIC_KIND
};

struct NewGadget PrefsNGad[] = {
	13, 171, 89, 14, (UBYTE *)"_Save", NULL, GD_PREFS_SAVE, PLACETEXT_IN, NULL, NULL,
	216, 171, 89, 14, (UBYTE *)"_Use", NULL, GD_PREFS_USE, PLACETEXT_IN, NULL, NULL,
	419, 171, 89, 14, (UBYTE *)"_Cancel", NULL, GD_PREFS_CANCEL, PLACETEXT_IN, NULL, NULL,
	13, 126, 26, 11, (UBYTE *)"_Patch Delete()", NULL, GD_PREFS_PATCHDELETE, PLACETEXT_RIGHT, NULL, NULL,
	13, 141, 26, 11, (UBYTE *)"Patch _Open()", NULL, GD_PREFS_PATCHOPEN, PLACETEXT_RIGHT, NULL, NULL,
	13, 14, 141, 56, NULL, NULL, GD_PREFS_TRASHCANS, 0, NULL, NULL,
	13, 70, 61, 14, (UBYTE *)"_Add", NULL, GD_PREFS_ADD, PLACETEXT_IN, NULL, NULL,
	81, 70, 73, 14, (UBYTE *)"_Remove", NULL, GD_PREFS_REMOVE, PLACETEXT_IN, NULL, NULL,
	192, 126, 26, 11, (UBYTE *)"App _Window", NULL, GD_PREFS_APPWINDOW, PLACETEXT_RIGHT, NULL, NULL,
	192, 141, 26, 11, (UBYTE *)"App _Icon", NULL, GD_PREFS_APPICON, PLACETEXT_RIGHT, NULL, NULL,
	189, 14, 141, 56, NULL, NULL, GD_PREFS_FORCEDELETE, 0, NULL, NULL,
	189, 70, 61, 14, (UBYTE *)"A_dd", NULL, GD_PREFS_FD_ADD, PLACETEXT_IN, NULL, NULL,
	257, 70, 73, 14, (UBYTE *)"R_emove", NULL, GD_PREFS_FD_REMOVE, PLACETEXT_IN, NULL, NULL,
	367, 14, 17, 9, NULL, NULL, GD_PREFS_OVERWRITE, PLACETEXT_RIGHT, NULL, NULL,
	361, 92, 47, 14, (UBYTE *)"KB on disk", NULL, GD_PREFS_FREEDISK, PLACETEXT_RIGHT, NULL, NULL,
	344, 125, 26, 11, (UBYTE *)"As_k before Del.", NULL, GD_PREFS_ASKBEFOREDELETE, PLACETEXT_RIGHT, NULL, NULL,
	192, 156, 26, 11, (UBYTE *)"App _Menu", NULL, GD_PREFS_APPMENU, PLACETEXT_RIGHT, NULL, NULL,
	13, 156, 26, 11, (UBYTE *)"A_nimations", NULL, GD_PREFS_ANIMATIONS, PLACETEXT_RIGHT, NULL, NULL,
	344, 141, 26, 11, (UBYTE *)"_Hide closes all", NULL, GD_PREFS_HIDECLOSESALL, PLACETEXT_RIGHT, NULL, NULL,
	344, 156, 26, 11, (UBYTE *)"_Delete if size 0", NULL, GD_PREFS_DELETEZEROK, PLACETEXT_RIGHT, NULL, NULL,
	62, 92, 248, 14, (UBYTE *)"Sound", NULL, GD_PREFS_SOUND, PLACETEXT_LEFT|NG_HIGHLABEL, NULL, NULL,
	310, 92, 20, 14, NULL, NULL, GD_PREFS_GETSOUND, 0, NULL, NULL
};

ULONG PrefsGTags[] = {
	(GT_Underscore), '_', (TAG_DONE),
	(GT_Underscore), '_', (TAG_DONE),
	(GT_Underscore), '_', (TAG_DONE),
	(GT_Underscore), '_', (TAG_DONE),
	(GT_Underscore), '_', (TAG_DONE),
	(GTLV_ShowSelected), NULL, (TAG_DONE),
	(GT_Underscore), '_', (TAG_DONE),
	(GT_Underscore), '_', (TAG_DONE),
	(GT_Underscore), '_', (TAG_DONE),
	(GT_Underscore), '_', (TAG_DONE),
	(GTLV_ShowSelected), NULL, (TAG_DONE),
	(GT_Underscore), '_', (TAG_DONE),
	(GT_Underscore), '_', (TAG_DONE),
	(GTMX_Labels), (ULONG)&PREFS_OVERWRITE1Labels[ 0 ], (GTMX_Spacing), 3, (GT_Underscore), '_', (TAG_DONE),
	(GTIN_Number), 0, (GTIN_MaxChars), 6, (TAG_DONE),
	(GT_Underscore), '_', (TAG_DONE),
	(GT_Underscore), '_', (TAG_DONE),
	(GT_Underscore), '_', (GA_Disabled), TRUE, (TAG_DONE),
	(GT_Underscore), '_', (TAG_DONE),
	(GT_Underscore), '_', (TAG_DONE),
	(GTST_MaxChars), 256, (TAG_DONE),
	(TAG_DONE)
};

UWORD ComputeX( UWORD value )
{
	return(( UWORD )((( FontX * value ) + 4 ) / 8 ));
}

UWORD ComputeY( UWORD value )
{
	return(( UWORD )((( FontY * value ) + 4 ) / 8 ));
}

void ComputeFont( UWORD width, UWORD height )
{
	Font = &Attr;
	Font->ta_Name = (STRPTR)Scr->RastPort.Font->tf_Message.mn_Node.ln_Name;
	Font->ta_YSize = FontY = Scr->RastPort.Font->tf_YSize;
	FontX = Scr->RastPort.Font->tf_XSize;

	OffX = Scr->WBorLeft;
	OffY = Scr->RastPort.TxHeight + Scr->WBorTop + 1;

	if ( width && height ) {
		if (( ComputeX( width ) + OffX + Scr->WBorRight ) > Scr->Width )
			goto UseTopaz;
		if (( ComputeY( height ) + OffY + Scr->WBorBottom ) > Scr->Height )
			goto UseTopaz;
	}
	return;

UseTopaz:
	Font->ta_Name = (STRPTR)"topaz.font";
	FontX = FontY = Font->ta_YSize = 8;
}

int SetupScreen( void )
{
	if ( ! ( Scr = LockPubScreen( PubScreenName )))
		return( 1L );

	ComputeFont( 0, 0 );

	if ( ! ( VisualInfo = GetVisualInfo( Scr, TAG_DONE )))
		return( 2L );

	if ( ! ( getClass = initGet()))
		return( 3L );

	if ( ! ( getImage = NewObject( getClass, NULL, GT_VisualInfo, VisualInfo, TAG_DONE )))
		return( 4L );

	return( 0L );
}

void CloseDownScreen( void )
{
	if ( getImage   ) {
		DisposeObject( getImage );
		getImage = NULL;
	}

	if ( getClass   ) {
		FreeClass( getClass );
		getClass = NULL;
	}

	if ( VisualInfo ) {
		FreeVisualInfo( VisualInfo );
		VisualInfo = NULL;
	}

	if ( Scr        ) {
		UnlockPubScreen( NULL, Scr );
		Scr = NULL;
	}
}

int OpenDummyWindow( void )
{
	struct NewGadget	ng;
	struct Gadget	*g;
	UWORD		lc, tc;
	UWORD		wleft = DummyLeft, wtop = DummyTop, ww, wh;

	ComputeFont( DummyWidth, DummyHeight );

	ww = ComputeX( DummyWidth );
	wh = ComputeY( DummyHeight );

	if (( wleft + ww + OffX + Scr->WBorRight ) > Scr->Width ) wleft = Scr->Width - ww;
	if (( wtop + wh + OffY + Scr->WBorBottom ) > Scr->Height ) wtop = Scr->Height - wh;

	if ( ! ( DummyMenus = CreateMenus( DummyNewMenu, GTMN_FrontPen, 0L, TAG_DONE )))
		return( 3L );

	LayoutMenus( DummyMenus, VisualInfo, TAG_DONE );

	if ( ! ( DummyWnd = OpenWindowTags( NULL,
				WA_Left,	wleft,
				WA_Top,		wtop,
				WA_Width,	ww + OffX + Scr->WBorRight,
				WA_Height,	wh + OffY + Scr->WBorBottom,
				WA_IDCMP,	IDCMP_MENUPICK|IDCMP_CLOSEWINDOW|IDCMP_REFRESHWINDOW,
				WA_Flags,	WFLG_SIZEGADGET|WFLG_DRAGBAR|WFLG_DEPTHGADGET|WFLG_CLOSEGADGET|WFLG_SMART_REFRESH,
				WA_Title,	DummyWdt,
				WA_PubScreen,	Scr,
				WA_MinWidth,	67,
				WA_MinHeight,	21,
				WA_MaxWidth,	656,
				WA_MaxHeight,	495,
				TAG_DONE )))
	return( 4L );

	SetMenuStrip( DummyWnd, DummyMenus );
	GT_RefreshWindow( DummyWnd, NULL );

	return( 0L );
}

void CloseDummyWindow( void )
{
	if ( DummyMenus      ) {
		ClearMenuStrip( DummyWnd );
		FreeMenus( DummyMenus );
		DummyMenus = NULL;	}

	if ( DummyWnd        ) {
		CloseWindow( DummyWnd );
		DummyWnd = NULL;
	}
}

void PrefsRender( void )
{
	struct IntuiText	it;
	UWORD			cnt;

	ComputeFont( PrefsWidth, PrefsHeight );


	for ( cnt = 0; cnt < Prefs_TNUM; cnt++ ) {
		CopyMem(( char * )&PrefsIText[ cnt ], ( char * )&it, (long)sizeof( struct IntuiText ));
		it.ITextFont = Font;
		it.LeftEdge  = OffX + ComputeX( it.LeftEdge ) - ( IntuiTextLength( &it ) >> 1 );
		it.TopEdge   = OffY + ComputeY( it.TopEdge ) - ( Font->ta_YSize >> 1 );
		PrintIText( PrefsWnd->RPort, &it, 0, 0 );
	}
}

int OpenPrefsWindow( void )
{
	struct NewGadget	ng;
	struct Gadget	*g;
	UWORD		lc, tc;
	UWORD		wleft = PrefsLeft, wtop = PrefsTop, ww, wh;

	ComputeFont( PrefsWidth, PrefsHeight );

	ww = ComputeX( PrefsWidth );
	wh = ComputeY( PrefsHeight );

	if (( wleft + ww + OffX + Scr->WBorRight ) > Scr->Width ) wleft = Scr->Width - ww;
	if (( wtop + wh + OffY + Scr->WBorBottom ) > Scr->Height ) wtop = Scr->Height - wh;

	if ( ! ( g = CreateContext( &PrefsGList )))
		return( 1L );

	for( lc = 0, tc = 0; lc < Prefs_CNT; lc++ ) {

		CopyMem((char * )&PrefsNGad[ lc ], (char * )&ng, (long)sizeof( struct NewGadget ));

		ng.ng_VisualInfo = VisualInfo;
		ng.ng_TextAttr   = Font;
		ng.ng_LeftEdge   = OffX + ComputeX( ng.ng_LeftEdge );
		ng.ng_TopEdge    = OffY + ComputeY( ng.ng_TopEdge );

		if ( PrefsGTypes[ lc ] != GENERIC_KIND ) {
			ng.ng_Width      = ComputeX( ng.ng_Width );
			ng.ng_Height     = ComputeY( ng.ng_Height);
		}

		PrefsGadgets[ lc ] = g = CreateGadgetA((ULONG)PrefsGTypes[ lc ], g, &ng, ( struct TagItem * )&PrefsGTags[ tc ] );

		if ( PrefsGTypes[ lc ] == GENERIC_KIND ) {
			g->Flags             |= GFLG_GADGIMAGE | GFLG_GADGHIMAGE;
			g->Activation        |= GACT_RELVERIFY;
			g->GadgetRender       = (APTR)getImage;
			g->SelectRender       = (APTR)getImage;
		}

		while( PrefsGTags[ tc ] ) tc += 2;
		tc++;

		if ( NOT g )
			return( 2L );
	}

	if ( ! ( PrefsWnd = OpenWindowTags( NULL,
				WA_Left,	wleft,
				WA_Top,		wtop,
				WA_Width,	ww + OffX + Scr->WBorRight,
				WA_Height,	wh + OffY + Scr->WBorBottom,
				WA_IDCMP,	BUTTONIDCMP|CHECKBOXIDCMP|LISTVIEWIDCMP|MXIDCMP|INTEGERIDCMP|STRINGIDCMP|IDCMP_GADGETUP|IDCMP_CLOSEWINDOW|IDCMP_REFRESHWINDOW,
				WA_Flags,	WFLG_DRAGBAR|WFLG_DEPTHGADGET|WFLG_SMART_REFRESH|WFLG_ACTIVATE|WFLG_RMBTRAP,
				WA_Gadgets,	PrefsGList,
				WA_Title,	PrefsWdt,
				WA_ScreenTitle,	"Trashman v1.3 by Tomasz Muszynski",
				WA_PubScreen,	Scr,
				TAG_DONE )))
	return( 4L );

	GT_RefreshWindow( PrefsWnd, NULL );

	PrefsRender();

	return( 0L );
}

void ClosePrefsWindow( void )
{
	if ( PrefsWnd        ) {
		CloseWindow( PrefsWnd );
		PrefsWnd = NULL;
	}

	if ( PrefsGList      ) {
		FreeGadgets( PrefsGList );
		PrefsGList = NULL;
	}
}

