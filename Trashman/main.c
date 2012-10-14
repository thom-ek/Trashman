/*
** Includes
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<proto/wb.h>
#include<proto/dos.h>
#include<proto/exec.h>
#include<proto/icon.h>
#include<proto/locale.h>
#include<proto/utility.h>
#include<proto/diskfont.h>
#include<proto/gadtools.h>
#include<proto/graphics.h>
#include<proto/iffparse.h>
#include<proto/progress.h>
#include<proto/reqtools.h>
#include<proto/datatypes.h>
#include<proto/intuition.h>
#include<proto/multiuser.h>
#include<proto/commodities.h>
#include<proto/screennotify.h>
#include<dos/notify.h>
#include<dos/datetime.h>
#include<exec/memory.h>
#include<exec/execbase.h>
#include<intuition/icclass.h>
#include<intuition/classusr.h>
#include<intuition/imageclass.h>
#include<intuition/gadgetclass.h>
#include<datatypes/soundclass.h>
#include<workbench/startup.h>
#include<workbench/workbench.h>
#include<libraries/reqtools.h>
#include<libraries/screennotify.h>
#include<prefs/prefhdr.h>
#include"gadtools.h"
#include"Trashman_rev.h"
#include"Trashman_cat.h"

/*
** Version
*/

TEXT ver[]=VERSTAG;
extern long __oslibversion=38;

/*
** Prototypes
*/

void main(int,char **);
int  OpenMainWindow(void);
int  OpenPreferences(void);
int  OpenInfoWindow(STRPTR,STRPTR,ULONG);
int  OpenDropWindow(void);
void CloseMainWindow(void);
void ClosePreferences(void);
void CloseInfoWindow(void);
void CloseDropWindow(void);
void SetupBits(void);
void HandleCxMsg(void);
void HandleAppMsg(void);
void HandleSNMsg(void);
void HandleNotifyMsg(void);
void HandleMainWnd(void);
void HandlePrefsWnd(void);
void HandleDropWnd(void);
void MakeFileList(struct AppMessage *);
void FreeFileList(void);
struct DeletedFile *DoFile(BPTR,struct FileInfoBlock *,STRPTR);
struct DeletedFile *DoDirectory(BPTR,struct FileInfoBlock *,STRPTR);
void InfoPrint(STRPTR,ULONG);
void MoveDelta(void);
void DrawWindow(void);
void DrawLine(struct DeletedFile *,ULONG,ULONG);
void UpdateProps(void);
int  SysISize(void);
Object *NewImageObject(ULONG);
int  SelectFile(ULONG,int);
void LoadPrefs(void);
void LoadPrefsAs(void);
void SavePrefs(STRPTR);
void SavePrefsAs(void);
void InitTrashcan(struct TNode *);
void FreeTrashcan(struct TNode *);
void InitTrashcansList(void);
void FreeTrashcansList(void);
void MakeTrashcansList(void);
void FreeFiltersList(void);
void RemoveSelected(void);
void RemoveAll(void);
void UndeleteSelected(void);
void UndeleteSelectedOther(void);
BOOL MyDeleteRecursive(STRPTR);
void DirDirectory(STRPTR);
void DeleteDirectory(STRPTR,ULONG *,BOOL);
void About(void);
BOOL CheckFilter(STRPTR);
void SortDirs(void);
void SelectAll(void);
void SetTitles(void);
void LockWindows(void);
void UnLockWindows(void);
ULONG RequestUser(STRPTR,STRPTR,STRPTR,...);
void ShowError(STRPTR);
BOOL MoveFile(STRPTR,STRPTR);
void FixLocale(void);
void HideAll(void);
void ShowAll(void);
void FreeSpace(void);
Object *OpenSound(void);
void CloseSound(Object *);
void PlaySound(Object *);
void MakeUserDir(void);
ULONG FindMaxPen(void);

BOOL __saveds __asm MyDeleteFile(register __d1 STRPTR);
BOOL __saveds __asm CallDeleteFile(register __a0 APTR, register __d1 STRPTR);
BPTR __saveds __asm MyOpen(register __d1 STRPTR, register __d2 LONG);
BPTR __saveds __asm CallOpen(register __a0 APTR, register __d1 STRPTR, register __d2 LONG);
struct DiskObject *TTGetOurIcon(struct WBStartup *);
void TTFreeOurIcon(struct DiskObject *);
char *TTString(struct DiskObject *,char *, char *);
LONG TTInt(struct DiskObject *,char *, LONG);
int TTBool(struct DiskObject *,char *, BOOL);

/*
** Defines and macros
*/

#define CopyDeviceName(src,dest)	{ int a=0; while(src[a]!=':') { dest[a]=src[a]; a++; if(a>=strlen(src)+1) break; } dest[a]=':'; dest[a+1]='\0'; }
#define Node(x) ((struct Node *)x)
#define List(x) ((struct List *)x)
#define NextNode(x) (((struct Node *)x)->ln_Succ)
#define PrevNode(x) (((struct Node *)x)->ln_Pred)
#define ListEmpty(x) ((((struct List *)x)->lh_TailPred) == ((struct Node *)x))
#define WhileNode(x) while(((struct Node *)x)->ln_Succ!=0)
#define SetFlag(x,f) x|=(f)
#define GetFlag(x,f) if((x)&(f))
#define ClearFlag(x,f) x&=((f)^-1)
#define DoFlag(x,f) if(code) SetFlag(x,f); else ClearFlag(x,f);
#define unless(x) if(!(x))
#define LEAVE(x)	\
				{	\
					if(x)	\
					{	\
						if(ReqToolsBase) RequestUser(GetTrashmanString(x),GetTrashmanString(MSG_OK_GAD),NULL);	\
						else printf("%s\n",GetTrashmanString(x));	\
					}	\
					goto end;	\
				}
#define GAD(x) ((struct Gadget *)x)
#define GD_LEFT	0
#define GD_RIGHT	1
#define GD_UP	2
#define GD_DOWN 3
#define GD_HORIZ	4
#define GD_VERT	5
#define APP_MAINWND 0L
#define APP_DROPWND 1L
#define APP_MAINICON 0L
#define APP_EMPTYTRASH 0L
#define TYPE_FILE 0
#define TYPE_DIR 1
#define EVT_POPKEY 0
#define ENVPREFS "ENV:Trashman.prefs"
#define ENVARCPREFS "ENVARC:Trashman.prefs"
#define DEF_UNDERSCORE '_'

struct Trashcan
{
	struct MinList Files;
	struct MinList Trashcans;
	struct MinList Filters;
	ULONG Entries;
	ULONG DeletedSize;
	ULONG maxlen,TotalMaxLen;
	ULONG Columns,Lines;
	ULONG FontXSize,FontYSize,FontBaseline;
	ULONG FromX,FromY;
};

struct DeletedFile
{
	struct MinNode Node;
	TEXT FileName[108];
	TEXT OriginalFileName[108];
	TEXT SourceFileName[108];
	ULONG Size;
	struct DateStamp Date;
	TEXT DateStr[2*LEN_DATSTRING];
	LONG Protection;
	TEXT ProtectionStr[9];
	BOOL Selected;
	UBYTE Type;
};

struct Prefs
{
	ULONG Flags;
	UWORD IconLeft,IconTop,DropLeft,DropTop,MainLeft,MainTop,MainWidth,MainHeight;
	LONG FreeDisk;
	UBYTE ViewBy;
	UBYTE Overwrite;
	TEXT  Sound[108];
};

struct TNode
{
	struct Node *ln_Succ;
	struct Node *ln_Pred;
	UBYTE ln_Type;
	BYTE ln_Pri;
	char *ln_Name;
	BPTR ln_Lock;
	BOOL ln_InitNR;
	struct NotifyRequest ln_NR;
};

// Flags
#define PREFS_PATCHDELETE (1<<1)
#define PREFS_ANIMATIONS (1<<2)
#define PREFS_APPWINDOW (1<<3)
#define PREFS_APPICON (1<<4)
#define PREFS_APPMENU (1<<5)
#define PREFS_ASKBEFOREDELETE (1<<6)
#define PREFS_PATCHOPEN (1<<7)
#define PREFS_HIDECLOSESALL (1<<8)
#define PREFS_DELETEZEROK (1<<9)

// Overwrite
#define PREFS_OVERWRITE_NEVER	(0)
#define PREFS_OVERWRITE_ALWAYS (1)
#define PREFS_OVERWRITE_ASK (2)
#define PREFS_OVERWRITE_SAMELOCATION (3)

// View by
#define PREFS_VIEWBY_NAME (0)
#define PREFS_VIEWBY_SIZE (1)
#define PREFS_VIEWBY_DATE (2)
#define PREFS_VIEWBY_LOCATION (3)

#define TEMPLATE "CX_PRIORITY/N/K,CX_POPKEY/K,FROM,FONTNAME=FN/K,FONTSIZE=FS/N/K,MULTIUSER=MU/S"
#define ARG_PRIORITY 0
#define ARG_POPKEY 1
#define ARG_FROM 2
#define ARG_FONTNAME 3
#define ARG_FONTSIZE 4
#define ARG_MULTIUSER 5
#define ARG_COUNT 6

ULONG DEF_PRIORITY=0;
TEXT  DEF_POPKEY[]="control g";
TEXT  DEF_FROM[]=ENVPREFS;
TEXT  DEF_FONTNAME[]="topaz.font";
ULONG DEF_FONTSIZE=8;
ULONG DEF_MULTIUSER=FALSE;

/*
** Commodities Variables
*/

struct NewBroker newbroker=
{
	NB_VERSION,
	"Trashman",
	"Trashman v1.3 by Tomasz Muszynski",
	"Trashcan Manager",
	NBU_UNIQUE|NBU_NOTIFY,
	COF_SHOW_HIDE
};

CxObj *Broker=NULL,*PopKeyObj;
TEXT PopKeyStr[50];

/*
** Variables
*/

Object *LeftImage=NULL,*RightImage=NULL,*UpImage=NULL,*DownImage=NULL,*SizeImage=NULL;
Object *LeftButton=NULL,*RightButton=NULL,*UpButton=NULL,*DownButton=NULL,*HorizProp=NULL,*VertProp=NULL;
Object *ProgressGad=NULL;
Object *ErrSoundObj=NULL;
Class *ProgressClass=NULL;
extern struct ExecBase *SysBase;
extern struct WBStartup *WBenchMsg;
struct Library *ReqToolsBase=NULL,*ProgressBase=NULL,*ScreenNotifyBase=NULL;
struct muBase *muBase=NULL;
struct Locale *Locale=NULL;
struct IntuiMessage *message=NULL;
struct AppIcon *AppIcon=NULL;
struct AppWindow *AppDropWnd=NULL,*AppMainWnd=NULL;
struct AppMenuItem *AppMenuItem=NULL;
struct DiskObject *icon=NULL,*AppDO=NULL;
struct DrawInfo *DRI=NULL;
struct FileInfoBlock *FIB=NULL;
struct MsgPort *AppMsgPort=NULL,*NotifyMsgPort=NULL,*BrokerMsgPort=NULL,*SNMsgPort=NULL;
struct MinList NRDrives;
struct Prefs Prefs,TmpPrefs;
struct rtFileRequester *FileReq=NULL,*OtherReq=NULL,*SaveReq=NULL;
struct Trashcan Trashcan={NULL,0,0,0,0,0};
struct Window *MainWnd=NULL,*InfoWnd=NULL,*DropWnd=NULL;
struct TextFont *MainWndFont=NULL;
struct TextAttr MainWndFontAttr={NULL,NULL,FS_NORMAL,FPB_DISKFONT};
struct muUserInfo *muUI=NULL;
BOOL running=TRUE,RunPrefs=FALSE,FirstClick=TRUE;
BOOL Mouse=FALSE,WhatMouseDo,UpdateContents=FALSE;
ULONG WindowBits=NULL,SNSigBit=NULL,class,code,qualifier,PrevMouse,SelectedList=0,FDSelectedList=0;
ULONG StartSeconds,StartMicros,EndSeconds,EndMicros,ProgressCnt=0,FontSize,MultiUser=FALSE;
APTR OldDeleteFile=NULL,OldOpen=NULL,SNHandle=NULL;
TEXT PrefsFrom[108],WindowTitle[256],FontName[MAXFONTNAME],CTitle[256];

/*
** Main program
*/

void main(int argc, char *argv[])
{
	ULONG signals;
	long *(opts[ARG_COUNT]),arg;
	LONG BrokerError;
	struct RDArgs *rdargs;

	if(SysBase->LibNode.lib_Version<39)
	{
		LEAVE(MSG_BAD_SYSTEM_VERSION);
	}

	NewList(List(&Trashcan.Files));
	if(argc!=0)
	{
		opts[ARG_PRIORITY]=&DEF_PRIORITY;
		opts[ARG_POPKEY]=(long *)DEF_POPKEY;
		opts[ARG_FROM]=(long *)DEF_FROM;
		opts[ARG_FONTNAME]=(long *)DEF_FONTNAME;
		opts[ARG_FONTSIZE]=&DEF_FONTSIZE;
		opts[ARG_MULTIUSER]=(long *)DEF_MULTIUSER;
		if(rdargs=ReadArgs(TEMPLATE,(long *) opts, NULL))
		{
			arg=FindArg(TEMPLATE,"CX_PRIORITY");
			if(arg>-1) newbroker.nb_Pri=*opts[arg];

			arg=FindArg(TEMPLATE,"CX_POPKEY");
			if(arg>-1) strcpy(PopKeyStr,(char *)opts[arg]);

			arg=FindArg(TEMPLATE,"FROM");
			if(arg>-1) strcpy(PrefsFrom,(char *)opts[arg]);

			arg=FindArg(TEMPLATE,"FONTNAME");
			if(arg>-1) strcpy(FontName,(char *)opts[arg]);

			arg=FindArg(TEMPLATE,"FONTSIZE");
			if(arg>-1) FontSize=*opts[arg];

			arg=FindArg(TEMPLATE,"MULTIUSER");
			if(arg>-1) MultiUser=(ULONG)opts[arg];

			FreeArgs(rdargs);
		}
		else
		{
			PrintFault(IoErr(),argv[0]);
			LEAVE(NULL);
		}
	}
	else
	{
		unless(icon=TTGetOurIcon(WBenchMsg)) icon=GetDiskObject(argv[0]);
		newbroker.nb_Pri=TTInt(icon,"CX_PRIORITY",DEF_PRIORITY);
		strcpy(PopKeyStr,TTString(icon,"CX_POPKEY",(char *)DEF_POPKEY));
		strcpy(PrefsFrom,TTString(icon,"FROM",(char *)DEF_FROM));
		strcpy(FontName,TTString(icon,"FONTNAME",(char *)DEF_FONTNAME));
		FontSize=TTInt(icon,"FONTSIZE",DEF_FONTSIZE);
		MultiUser=(ULONG) TTBool(icon,"MULTIUSER",DEF_MULTIUSER);
		TTFreeOurIcon(icon);
	}

	if(MultiUser) MultiUser=TRUE;

	LoadPrefs();

	unless(ReqToolsBase=OpenLibrary(REQTOOLSNAME,REQTOOLSVERSION)) LEAVE(MSG_ERROR_CANT_OPEN_REQTOOLS);
	unless(Locale=OpenLocale(NULL)) LEAVE(MSG_ERROR_CANT_OPEN_LOCALE);
	OpenTrashmanCatalog(Locale,NULL);
	if(MultiUser)
	{
		unless(muBase=(struct muBase *)OpenLibrary(MULTIUSERNAME,MULTIUSERVERSION)) LEAVE(MSG_ERROR_CANT_OPEN_MULTIUSER);
		unless(muUI=muAllocUserInfo()) LEAVE(MSG_ERROR_CANT_ALLOC_USERINFO);
		MakeUserDir();
	}
	FixLocale();
	newbroker.nb_Descr=GetTrashmanString(MSG_COMMODITY_DESCRIPTION);
	sprintf(CTitle,GetTrashmanString(MSG_COMMODITY_TITLE),VERSION,REVISION);
	newbroker.nb_Title=CTitle;
	unless(BrokerMsgPort=CreateMsgPort()) LEAVE(MSG_ERROR_CANT_CREATE_PORT);
	newbroker.nb_Port=BrokerMsgPort;
	unless(Broker=CxBroker(&newbroker,&BrokerError))
	{
		if(BrokerError!=CBERR_DUP) { LEAVE(MSG_ERROR_CANT_CREATE_COMMODITY); }
		else LEAVE(NULL);
	}
	unless(PopKeyObj=HotKey(PopKeyStr,BrokerMsgPort,EVT_POPKEY)) LEAVE(MSG_ERROR_CANT_CREATE_HOTKEY);
	AttachCxObj(Broker,PopKeyObj);
	if(CxObjError(PopKeyObj)) LEAVE(MSG_ERROR_CANT_CREATE_COMMODITY);
	ActivateCxObj(Broker,1L);

	MainWndFontAttr.ta_Name=FontName;
	MainWndFontAttr.ta_YSize=FontSize;
	unless(MainWndFont=OpenDiskFont(&MainWndFontAttr)) LEAVE(MSG_ERROR_CANT_OPEN_FONT);

	unless(ProgressBase=OpenLibrary(PROGRESSNAME,PROGRESSVERSION)) LEAVE(MSG_ERROR_CANT_OPEN_PROGRESS);
	ScreenNotifyBase=OpenLibrary(SCREENNOTIFY_NAME,SCREENNOTIFY_VERSION);
	if(ScreenNotifyBase) if(SNMsgPort=CreateMsgPort()) if(SNHandle=AddWorkbenchClient(SNMsgPort,0)) SNSigBit=(1L<<SNMsgPort->mp_SigBit);
	ProgressClass=GetProgressClass();
	if(SetupScreen()) LEAVE(MSG_ERROR_CANT_SETUP_SCREEN);
	unless(DRI=GetScreenDrawInfo(Scr)) LEAVE(MSG_ERROR_CANT_GET_DRI);
	unless(LeftImage=NewImageObject(LEFTIMAGE)) LEAVE(MSG_ERROR_CANT_CREATE_OBJECT);
	unless(RightImage=NewImageObject(RIGHTIMAGE)) LEAVE(MSG_ERROR_CANT_CREATE_OBJECT);
	unless(UpImage=NewImageObject(UPIMAGE)) LEAVE(MSG_ERROR_CANT_CREATE_OBJECT);
	unless(DownImage=NewImageObject(DOWNIMAGE)) LEAVE(MSG_ERROR_CANT_CREATE_OBJECT);
	unless(SizeImage=NewImageObject(SIZEIMAGE)) LEAVE(MSG_ERROR_CANT_CREATE_OBJECT);

	unless(FileReq=rtAllocRequest(RT_FILEREQ,TAG_DONE)) LEAVE(MSG_ERROR_CANT_ALLOC_REQ);
	unless(SaveReq=rtAllocRequest(RT_FILEREQ,TAG_DONE)) LEAVE(MSG_ERROR_CANT_ALLOC_REQ);
	unless(OtherReq=rtAllocRequest(RT_FILEREQ,TAG_DONE)) LEAVE(MSG_ERROR_CANT_ALLOC_REQ);
	rtChangeReqAttr(SaveReq,RTFI_Dir,"SYS:Prefs/Presets",TAG_DONE);
	unless(AppMsgPort=CreateMsgPort()) LEAVE(MSG_ERROR_CANT_CREATE_PORT);
	unless(NotifyMsgPort=CreateMsgPort()) LEAVE(MSG_ERROR_CANT_CREATE_PORT);
	unless(AppDO=GetDefDiskObject(WBGARBAGE)) LEAVE(MSG_ERROR_CANT_OPEN_ICON);
	AppDO->do_CurrentX=Prefs.IconLeft;
	AppDO->do_CurrentY=Prefs.IconTop;
	GetFlag(Prefs.Flags,PREFS_APPICON) unless(AppIcon=AddAppIcon(APP_MAINICON,NULL,GetTrashmanString(MSG_ICON_TITLE),AppMsgPort,NULL,AppDO,TAG_DONE)) LEAVE(MSG_ERROR_CANT_CREATE_APPICON);
	unless(FIB=AllocDosObjectTags(DOS_FIB,TAG_DONE)) LEAVE(MSG_ERROR_CANT_ALLOC_DOSOBJ);
	GetFlag(Prefs.Flags,PREFS_APPWINDOW) if(OpenDropWindow()) LEAVE(MSG_ERROR_CANT_CREATE_APPWINDOW);
	GetFlag(Prefs.Flags,PREFS_APPMENU) unless(AppMenuItem=AddAppMenuItem(APP_EMPTYTRASH,NULL,GetTrashmanString(MSG_TOOLS_MENU),AppMsgPort,TAG_DONE)) LEAVE(MSG_ERROR_CANT_CREATE_APPMENU);
	GetFlag(Prefs.Flags,PREFS_PATCHDELETE) OldDeleteFile=SetFunction((struct Library *)DOSBase,-72,(ULONG (*)())MyDeleteFile);
	GetFlag(Prefs.Flags,PREFS_PATCHOPEN) OldOpen=SetFunction((struct Library *)DOSBase,-30,(ULONG (*)())MyOpen);
	ErrSoundObj=OpenSound();

	InitTrashcansList();

	FreeSpace();
	SetupBits();
	while(running)
	{
		signals=Wait((1L<<BrokerMsgPort->mp_SigBit) | (1L<<AppMsgPort->mp_SigBit) | SNSigBit | (1L<<NotifyMsgPort->mp_SigBit) | WindowBits);
		GetFlag(signals,1L<<BrokerMsgPort->mp_SigBit) HandleCxMsg();
		GetFlag(signals,1L<<AppMsgPort->mp_SigBit) HandleAppMsg();
		GetFlag(signals,SNSigBit) HandleSNMsg();
		GetFlag(signals,1L<<NotifyMsgPort->mp_SigBit) HandleNotifyMsg();
		if(MainWnd) GetFlag(signals,1L<<MainWnd->UserPort->mp_SigBit) HandleMainWnd();
		if(PrefsWnd) GetFlag(signals,1L<<PrefsWnd->UserPort->mp_SigBit) HandlePrefsWnd();
		if(DropWnd) GetFlag(signals,1L<<DropWnd->UserPort->mp_SigBit) HandleDropWnd();
	}

end:
	if(OldOpen) SetFunction((struct Library *)DOSBase,-30,OldOpen);
	if(OldDeleteFile) SetFunction((struct Library *)DOSBase,-72,OldDeleteFile);
	FreeTrashcansList();
	FreeFiltersList();
	FreeFileList();
	if(ErrSoundObj) CloseSound(ErrSoundObj);
	if(DropWnd) CloseDropWindow();
	if(PrefsWnd) ClosePrefsWindow();
	if(MainWnd) CloseMainWindow();
	if(FIB) FreeDosObject(DOS_FIB,FIB);
	if(AppDO) FreeDiskObject(AppDO);
	if(AppMenuItem) RemoveAppMenuItem(AppMenuItem);
	if(AppIcon) RemoveAppIcon(AppIcon);
	if(OtherReq) rtFreeRequest(OtherReq);
	if(SaveReq) rtFreeRequest(SaveReq);
	if(FileReq) rtFreeRequest(FileReq);
	if(NotifyMsgPort) DeleteMsgPort(NotifyMsgPort);
	if(AppMsgPort) DeleteMsgPort(AppMsgPort);
	if(SizeImage) DisposeObject(SizeImage);
	if(DownImage) DisposeObject(DownImage);
	if(UpImage) DisposeObject(UpImage);
	if(RightImage) DisposeObject(RightImage);
	if(LeftImage) DisposeObject(LeftImage);
	if(DRI) FreeScreenDrawInfo(Scr, DRI);
	if(Scr) CloseDownScreen();
	if(ScreenNotifyBase) CloseLibrary(ScreenNotifyBase);
	if(SNHandle) while(!RemWorkbenchClient(SNHandle)) Delay(10);
	if(SNMsgPort) DeleteMsgPort(SNMsgPort);
	if(ProgressBase) CloseLibrary(ProgressBase);
	if(ReqToolsBase) CloseLibrary(ReqToolsBase);
	if(muBase) CloseLibrary((struct Library *)muBase);
	if(muUI) muFreeUserInfo(muUI);
	if(Broker) DeleteCxObjAll(Broker);
	if(BrokerMsgPort) DeleteMsgPort(BrokerMsgPort);
	CloseTrashmanCatalog();
	if(Locale) CloseLocale(Locale);
	if(MainWndFont) CloseFont(MainWndFont);
}

void SetupBits()
{
	WindowBits=NULL;
	if(MainWnd) SetFlag(WindowBits,(1L<<MainWnd->UserPort->mp_SigBit));
	if(PrefsWnd) SetFlag(WindowBits,(1L<<PrefsWnd->UserPort->mp_SigBit));
	if(DropWnd) SetFlag(WindowBits,(1L<<DropWnd->UserPort->mp_SigBit));
}

void HandleCxMsg()
{
	ULONG msgid, msgtype;
	CxMsg *cxmsg;

	while(cxmsg=(CxMsg *)GetMsg(BrokerMsgPort))
	{
		msgid  = CxMsgID(cxmsg);
		msgtype= CxMsgType(cxmsg);
		ReplyMsg((struct Message *)cxmsg);
		switch(msgtype)
		{
			case CXM_IEVENT:
				switch(msgid)
				{
					case EVT_POPKEY:
						if(MainWnd)
						{
							ActivateWindow(MainWnd);
							WindowToFront(MainWnd);
						}
						else
						{
							GetFlag(Prefs.Flags,PREFS_HIDECLOSESALL) ShowAll();
							OpenMainWindow();
						}
						break;
				}
				break;
			case CXM_COMMAND:
				switch(msgid)
				{
					case CXCMD_DISABLE:
						ActivateCxObj(Broker,0L);
						break;
					case CXCMD_ENABLE:
						ActivateCxObj(Broker,1L);
						break;
					case CXCMD_APPEAR:
						if(MainWnd)
						{
							ActivateWindow(MainWnd);
							WindowToFront(MainWnd);
						}
						else
						{
							GetFlag(Prefs.Flags,PREFS_HIDECLOSESALL) ShowAll();
							OpenMainWindow();
						}
						break;
					case CXCMD_DISAPPEAR:
						while(message=GT_GetIMsg(MainWnd->UserPort)) GT_ReplyIMsg(message);
						GetFlag(Prefs.Flags,PREFS_HIDECLOSESALL) HideAll();
						else if(MainWnd) CloseMainWindow();
						break;
					case CXCMD_KILL:
						running=FALSE;
						break;
					case CXCMD_UNIQUE:
						if(MainWnd)
						{
							ActivateWindow(MainWnd);
							WindowToFront(MainWnd);
						}
						else
						{
							GetFlag(Prefs.Flags,PREFS_HIDECLOSESALL) ShowAll();
							OpenMainWindow();
						}
						break;
				}
				break;
		}
	}
}

void HandleSNMsg()
{
	struct ScreenNotifyMessage *SNMsg;
	BOOL showall=FALSE;

	while(SNMsg=(struct ScreenNotifyMessage *)GetMsg(SNMsgPort))
	{
		if(SNMsg->snm_Type == SCREENNOTIFY_TYPE_WORKBENCH)
		{
			switch((ULONG)SNMsg->snm_Value)
			{
				case FALSE: /* Workbench close notification */
					HideAll();
					break;
				case TRUE:  /* Workbench open notification */
					showall=TRUE;
					break;
			}
		}
		ReplyMsg((struct Message *) SNMsg);
	}
	if(showall)
	{
		Delay(50*10);
		ShowAll();
	}
}

void HandleAppMsg()
{
	struct AppMessage *AppMsg;

	while(AppMsg=(struct AppMessage *)GetMsg(AppMsgPort))
	{
		switch(AppMsg->am_Type)
		{
			case AMTYPE_APPMENUITEM:
				switch(AppMsg->am_ID)
				{
					case APP_EMPTYTRASH:
						GetFlag(Prefs.Flags,PREFS_ASKBEFOREDELETE)
						{
							LockWindows();
							if(RequestUser(GetTrashmanString(MSG_EMPTYALL),GetTrashmanString(MSG_YESNO_GAD),NULL))
							{
								UnLockWindows();
								RemoveAll();
							}
							else UnLockWindows();
						}
						else
							RemoveAll();
						break;
				}
				break;
			case AMTYPE_APPICON:
				switch(AppMsg->am_ID)
				{
					case APP_MAINICON:
						if(AppMsg->am_NumArgs==NULL)
						{
							if(MainWnd)
							{
								ActivateWindow(MainWnd);
								WindowToFront(MainWnd);
							}
							else OpenMainWindow();
						}
						else
						{
							unless(OpenInfoWindow(GetTrashmanString(MSG_MOVING_FILES),GetTrashmanString(MSG_DELETED_PROGRESS),AppMsg->am_NumArgs))
							{
								MakeFileList(AppMsg);
								CloseInfoWindow();
							}
						}
						break;
				}
				break;
			case AMTYPE_APPWINDOW:
				switch(AppMsg->am_ID)
				{
					case APP_MAINWND:
					case APP_DROPWND:
						unless(OpenInfoWindow(GetTrashmanString(MSG_MOVING_FILES),GetTrashmanString(MSG_DELETED_PROGRESS),AppMsg->am_NumArgs))
						{
							MakeFileList(AppMsg);
							CloseInfoWindow();
						}
						if(MainWnd)
						{
							DrawWindow();
							UpdateProps();
						}
						break;
				}
				break;
		}
		ReplyMsg((struct Message *)AppMsg);
	}
}

void HandleNotifyMsg()
{
	struct NotifyMessage *NotifyMsg;

	while(NotifyMsg=(struct NotifyMessage *)GetMsg(NotifyMsgPort))
	{
		switch(NotifyMsg->nm_Class)
		{
			case NOTIFY_CLASS:
				if(MainWnd)
				{
					UpdateContents=TRUE;
					FreeSpace();
/*
					FreeFileList();
					MakeTrashcansList();
					DrawWindow();
					UpdateProps();
*/
				}
				break;
		}
		ReplyMsg((struct Message *)NotifyMsg);
	}
}

void HandleMainWnd()
{
	ULONG a,b,from;
	struct Gadget *gadget;
	struct MenuItem *item;
	APTR iadr;

	while(message=GT_GetIMsg(MainWnd->UserPort))
	{
		class  = message->Class;
		code   = message->Code;
		qualifier = message->Qualifier;
		iadr = (APTR) message->IAddress;
		gadget = (struct Gadget *) message->IAddress;
		switch(class)
		{
			case IDCMP_CLOSEWINDOW:
				while(message=GT_GetIMsg(MainWnd->UserPort)) GT_ReplyIMsg(message);
				if(MainWnd) CloseMainWindow();
				goto skip;
				break;
			case IDCMP_CHANGEWINDOW: break;
			case IDCMP_ACTIVEWINDOW:
				if(UpdateContents)
				{
					FreeFileList();
					MakeTrashcansList();
					DrawWindow();
					UpdateProps();
					UpdateContents=FALSE;
				}
				break;
			case IDCMP_NEWSIZE:
				UpdateProps();
				DrawWindow();
				UpdateProps();
				break;
			case IDCMP_MOUSEBUTTONS:
				switch(code)
				{
					case SELECTDOWN:
						if(message->MouseX>MainWnd->BorderLeft && message->MouseX<MainWnd->Width-MainWnd->BorderRight && message->MouseY>MainWnd->BorderTop && message->MouseY<Trashcan.Lines*Trashcan.FontYSize+MainWnd->BorderTop+1)
						{
							Mouse=TRUE;
							PrevMouse=-1;
							WhatMouseDo=SelectFile(message->MouseY-1,-1);
							SetTitles();
						}
						break;
					case SELECTUP:
						Mouse=FALSE;
						break;
				}
				break;
			case IDCMP_MOUSEMOVE:
				if(Mouse)
				{
					if(((message->MouseY-MainWnd->BorderTop-1)/Trashcan.FontYSize)<Trashcan.Lines && message->MouseY>MainWnd->BorderTop && message->MouseY<MainWnd->Height-MainWnd->BorderBottom-1)
					{
						if(PrevMouse!=((message->MouseY-MainWnd->BorderTop-1)/Trashcan.FontYSize))
						{
							if(PrevMouse!=-1)
							{
								if(PrevMouse<((message->MouseY-MainWnd->BorderTop-1)/Trashcan.FontYSize))
								{
									for(a=PrevMouse;a<((message->MouseY-MainWnd->BorderTop-1)/Trashcan.FontYSize)+1;a++)
										WhatMouseDo=SelectFile((a*Trashcan.FontYSize)+MainWnd->BorderTop+1,WhatMouseDo);
									SetTitles();
								}
								else
								{
									for(a=((message->MouseY-MainWnd->BorderTop-1)/Trashcan.FontYSize);a<PrevMouse;a++)
										WhatMouseDo=SelectFile((a*Trashcan.FontYSize)+MainWnd->BorderTop+1,WhatMouseDo);
									SetTitles();
								}
								PrevMouse=((message->MouseY-MainWnd->BorderTop-1)/Trashcan.FontYSize);
							}
							else
							{
								WhatMouseDo=SelectFile(message->MouseY-1,WhatMouseDo);
								PrevMouse=((message->MouseY-MainWnd->BorderTop-1)/Trashcan.FontYSize);
								SetTitles();
							}
						}
					}
				}
				break;
			case IDCMP_GADGETUP:
			case IDCMP_GADGETDOWN:
				break;
			case IDCMP_IDCMPUPDATE:
				switch(GetTagData(GA_ID,0,iadr))
				{
					case GD_VERT: MoveDelta(); break;
					case GD_HORIZ: DrawWindow(); break;
					case GD_LEFT:
						GetAttr(PGA_Top,HorizProp,&from);
						if(from>0)
						{
							from--;
							SetGadgetAttrs(GAD(HorizProp),MainWnd,NULL,PGA_Top,from,TAG_DONE);
							DrawWindow();
						}
						break;
					case GD_RIGHT:
						GetAttr(PGA_Top,HorizProp,&from);
						GetAttr(PGA_Total,HorizProp,&a);
						GetAttr(PGA_Visible,HorizProp,&b);
						if(from<a-b)
						{
							from++;
							SetGadgetAttrs(GAD(HorizProp),MainWnd,NULL,PGA_Top,from,TAG_DONE);
							DrawWindow();
						}
						break;
					case GD_UP:
						GetAttr(PGA_Top,VertProp,&from);
						if(from>0)
						{
							from--;
							SetGadgetAttrs(GAD(VertProp),MainWnd,NULL,PGA_Top,from,TAG_DONE);
							DrawWindow();
						}
						break;
					case GD_DOWN:
						GetAttr(PGA_Top,VertProp,&from);
						GetAttr(PGA_Total,VertProp,&a);
						GetAttr(PGA_Visible,VertProp,&b);
						if(from<a-b)
						{
							from++;
							SetGadgetAttrs(GAD(VertProp),MainWnd,NULL,PGA_Top,from,TAG_DONE);
							DrawWindow();
						}
						break;
				}
				break;
			case IDCMP_MENUPICK:
				while(code!=MENUNULL) 
				{
					item=(struct MenuItem *)ItemAddress(DummyMenus,code);
					switch(MENUNUM(code))
					{
						case 0:		// Project
							switch(ITEMNUM(code))
							{
								case 0:		// Open...
									LoadPrefsAs();
									break;
								case 1:		// Save As...
									SavePrefsAs();
									break;
								case 3:		// Hide
									while(message=GT_GetIMsg(MainWnd->UserPort)) GT_ReplyIMsg(message);
									GetFlag(Prefs.Flags,PREFS_HIDECLOSESALL) HideAll();
									else if(MainWnd) CloseMainWindow();
									goto skip;
									break;
								case 4:		// Quit
									running=FALSE;
									break;
							}
							break;
						case 1:		// Window
							switch(ITEMNUM(code))
							{
								case 0:	// Close
									while(message=GT_GetIMsg(MainWnd->UserPort)) GT_ReplyIMsg(message);
									if(MainWnd) CloseMainWindow();
									goto skip;
									break;
								case 1:	// Update
									FreeFileList();
									MakeTrashcansList();
									DrawWindow();
									UpdateProps();
									break;
								case 2:	// Select Contents
									SelectAll();
									DrawWindow();
									break;
								case 3:	// Snapshot
									switch(SUBNUM(code))
									{
										case 0:	// Desktop Icon
											if(DropWnd)
											{
												Prefs.IconLeft=DropWnd->LeftEdge;
												Prefs.IconTop=DropWnd->TopEdge;
												SavePrefs(ENVPREFS);
												SavePrefs(ENVARCPREFS);
											}
											break;
										case 1:	// Main Window
											Prefs.MainLeft=MainWnd->LeftEdge;
											Prefs.MainTop=MainWnd->TopEdge;
											Prefs.MainWidth=MainWnd->Width;
											Prefs.MainHeight=MainWnd->Height;
											SavePrefs(ENVPREFS);
											SavePrefs(ENVARCPREFS);
											break;
										case 2:	// Trashcan Window
											if(DropWnd)
											{
												Prefs.DropLeft=DropWnd->LeftEdge;
												Prefs.DropTop=DropWnd->TopEdge;
												SavePrefs(ENVPREFS);
												SavePrefs(ENVARCPREFS);
											}
											break;
									}
									break;
								case 4:	// View by
									switch(SUBNUM(code))
									{
										case 0:	Prefs.ViewBy=PREFS_VIEWBY_NAME; break; // Name
										case 1:	Prefs.ViewBy=PREFS_VIEWBY_SIZE; break; // Size
										case 2:	Prefs.ViewBy=PREFS_VIEWBY_DATE; break; // Date
										case 3:	Prefs.ViewBy=PREFS_VIEWBY_LOCATION; break; // Location
									}
									SortDirs();
									DrawWindow();
									break;
							}
							break;
						case 2:		// Files
							switch(ITEMNUM(code))
							{
								case 0:		// Remove Selected
									GetFlag(Prefs.Flags,PREFS_ASKBEFOREDELETE)
									{
										LockWindows();
										if(RequestUser(GetTrashmanString(MSG_REMOVESELECTED),GetTrashmanString(MSG_YESNO_GAD),NULL))
										{
											UnLockWindows();
											RemoveSelected();
										}
										else UnLockWindows();
									}
									else
										RemoveSelected();
									break;
								case 1:		// Empty All Trashcans
									GetFlag(Prefs.Flags,PREFS_ASKBEFOREDELETE)
									{
										LockWindows();
										if(RequestUser(GetTrashmanString(MSG_EMPTYALL),GetTrashmanString(MSG_YESNO_GAD),NULL))
										{
											UnLockWindows();
											RemoveAll();
										}
										else UnLockWindows();
									}
									else
										RemoveAll();
									break;
								case 3:		// Undelete to Original
									UndeleteSelected();
									break;
								case 4:		// Undelete to Other
									UndeleteSelectedOther();
									break;
							}
							break;
						case 3:		// Settings
							switch(ITEMNUM(code))
							{
								case 0:		// Edit...
									OpenPreferences();
									break;
								case 2:		// About
									About();
									break;
							}
							break;
					}
					code = item->NextSelect;
				}
				break;
		}
		GT_ReplyIMsg(message);
	}
skip:;
}

void HandlePrefsWnd()
{
	struct Gadget *gadget;
	struct Node *node;
	struct TNode *tnode;
	APTR iadr;
	TEXT name[108];
	STRPTR remname;
	int a;

	while(message=GT_GetIMsg(PrefsWnd->UserPort))
	{
		class  = message->Class;
		code   = message->Code;
		qualifier = message->Qualifier;
		iadr = (APTR) message->IAddress;
		gadget = (struct Gadget *) message->IAddress;
		switch(class)
		{
			case IDCMP_CLOSEWINDOW:
				while(message=GT_GetIMsg(PrefsWnd->UserPort)) GT_ReplyIMsg(message);
				ClosePreferences();
				goto skip;
				break;
			case IDCMP_GADGETUP:
			case IDCMP_GADGETDOWN:
				switch(gadget->GadgetID)
				{
					case GD_PREFS_SAVE:
						Prefs.FreeDisk=GetNumber(PrefsGadgets[GD_PREFS_FREEDISK]);
						strcpy(Prefs.Sound,GetString(PrefsGadgets[GD_PREFS_SOUND]));
						SavePrefs(ENVPREFS);
						SavePrefs(ENVARCPREFS);
					case GD_PREFS_USE:
						Prefs.FreeDisk=GetNumber(PrefsGadgets[GD_PREFS_FREEDISK]);
						strcpy(Prefs.Sound,GetString(PrefsGadgets[GD_PREFS_SOUND]));
						while(message=GT_GetIMsg(PrefsWnd->UserPort)) GT_ReplyIMsg(message);
						ClosePreferences();
						goto skip;
						break;
					case GD_PREFS_CANCEL:
						CopyMem(&TmpPrefs.Flags,&Prefs.Flags,sizeof(struct Prefs));
						while(message=GT_GetIMsg(PrefsWnd->UserPort)) GT_ReplyIMsg(message);
						ClosePreferences();
						GetFlag(Prefs.Flags,PREFS_APPICON) 
						{
							unless(AppIcon)
							{
								unless(AppIcon=AddAppIcon(APP_MAINICON,NULL,GetTrashmanString(MSG_ICON_TITLE),AppMsgPort,NULL,AppDO,TAG_DONE))
									ClearFlag(Prefs.Flags,PREFS_APPICON);
							}
						}
						else
							if(AppIcon) RemoveAppIcon(AppIcon);
						GetFlag(Prefs.Flags,PREFS_APPWINDOW) 
						{
							unless(DropWnd)
							{
								if(OpenDropWindow())
									ClearFlag(Prefs.Flags,PREFS_APPWINDOW);
							}
						}
						else
							if(DropWnd) CloseDropWindow();
						GetFlag(Prefs.Flags,PREFS_APPMENU)
						{
							unless(AppMenuItem)
							{
								unless(AppMenuItem=AddAppMenuItem(APP_EMPTYTRASH,NULL,GetTrashmanString(MSG_TOOLS_MENU),AppMsgPort,NULL,TAG_DONE))
									ClearFlag(Prefs.Flags,PREFS_APPMENU);
							}
						}
						else
							if(AppMenuItem) RemoveAppMenuItem(AppMenuItem);
						goto skip;
						break;
					case GD_PREFS_SOUND:
						break;
					case GD_PREFS_GETSOUND:
						LockWindows();
						if(rtFileRequest(FileReq,name,GetTrashmanString(MSG_SELECT_SOUND),
								RT_Window,PrefsWnd,
								RT_ReqPos,REQPOS_CENTERSCR,
								RT_Underscore,DEF_UNDERSCORE,
								TAG_DONE))
						{
							strcpy(Prefs.Sound,FileReq->Dir);
							AddPart(Prefs.Sound,name,108);
							GT_SetGadgetAttrs(PrefsGadgets[GD_PREFS_SOUND],PrefsWnd,NULL,GTST_String,Prefs.Sound,TAG_DONE);
						}
						UnLockWindows();
						break;
					case GD_PREFS_PATCHDELETE:
						DoFlag(Prefs.Flags,PREFS_PATCHDELETE);
						break;
					case GD_PREFS_PATCHOPEN:
						DoFlag(Prefs.Flags,PREFS_PATCHOPEN);
						break;
					case GD_PREFS_ANIMATIONS:
						DoFlag(Prefs.Flags,PREFS_ANIMATIONS);
						break;
					case GD_PREFS_ASKBEFOREDELETE:
						DoFlag(Prefs.Flags,PREFS_ASKBEFOREDELETE);
						break;
					case GD_PREFS_FREEDISK:
						break;
					case GD_PREFS_OVERWRITE:
						Prefs.Overwrite=code;
						break;
					case GD_PREFS_HIDECLOSESALL:
						DoFlag(Prefs.Flags,PREFS_HIDECLOSESALL);
						break;
					case GD_PREFS_DELETEZEROK:
						DoFlag(Prefs.Flags,PREFS_DELETEZEROK);
						break;
					case GD_PREFS_APPWINDOW:
						if(code)
						{
							SetFlag(Prefs.Flags,PREFS_APPWINDOW);
							unless(DropWnd)
							{
								if(OpenDropWindow())
								{
									ClearFlag(Prefs.Flags,PREFS_APPWINDOW);
									GT_SetGadgetAttrs(PrefsGadgets[GD_PREFS_APPWINDOW],PrefsWnd,NULL,GTCB_Checked,FALSE,TAG_DONE);
								}
							}
						}
						else
						{
							ClearFlag(Prefs.Flags,PREFS_APPWINDOW);
							if(DropWnd) CloseDropWindow();
						}
						break;
					case GD_PREFS_APPICON:
						if(code)
						{
							SetFlag(Prefs.Flags,PREFS_APPICON);
							unless(AppIcon)
							{
								unless(AppIcon=AddAppIcon(APP_MAINICON,NULL,GetTrashmanString(MSG_ICON_TITLE),AppMsgPort,NULL,AppDO,TAG_DONE))
								{
									ClearFlag(Prefs.Flags,PREFS_APPICON);
									GT_SetGadgetAttrs(PrefsGadgets[GD_PREFS_APPICON],PrefsWnd,NULL,GTCB_Checked,FALSE,TAG_DONE);
								}
							}
						}
						else
						{
							ClearFlag(Prefs.Flags,PREFS_APPICON);
							if(AppIcon) RemoveAppIcon(AppIcon);
							AppIcon=NULL;
						}
						break;
					case GD_PREFS_APPMENU:
						if(code)
						{
							SetFlag(Prefs.Flags,PREFS_APPMENU);
							unless(AppMenuItem)
							{
								unless(AppMenuItem=AddAppMenuItem(APP_EMPTYTRASH,NULL,GetTrashmanString(MSG_TOOLS_MENU),AppMsgPort,NULL,TAG_DONE))
								{
									ClearFlag(Prefs.Flags,PREFS_APPMENU);
									GT_SetGadgetAttrs(PrefsGadgets[GD_PREFS_APPMENU],PrefsWnd,NULL,GTCB_Checked,FALSE,TAG_DONE);
								}
							}
						}
						else
						{
							ClearFlag(Prefs.Flags,PREFS_APPMENU);
							if(AppMenuItem) RemoveAppMenuItem(AppMenuItem);
							AppMenuItem=NULL;
						}
						break;
					case GD_PREFS_TRASHCANS:
						SelectedList=code+1;
						break;
					case GD_PREFS_ADD:
						LockWindows();
						if(rtFileRequest(FileReq,name,GetTrashmanString(MSG_ADD_VOLUMES),
								RT_Window,PrefsWnd,
								RT_ReqPos,REQPOS_CENTERSCR,
								RT_Underscore,DEF_UNDERSCORE,
								RTFI_OkText,GetTrashmanString(MSG_ADD_GAD),
								RTFI_VolumeRequest,VREQF_NOASSIGNS,
								TAG_DONE))
						{
							GT_SetGadgetAttrs(PrefsGadgets[GD_PREFS_TRASHCANS],PrefsWnd,NULL,GTLV_Labels,NULL,TAG_DONE);
							if(remname=AllocVec(strlen(FileReq->Dir)+1,MEMF_ANY))
							{
								strcpy(remname,FileReq->Dir);
								if(tnode=AllocVec(sizeof(struct TNode),MEMF_ANY))
								{
									tnode->ln_Name=remname;
									tnode->ln_Lock=tnode->ln_InitNR=FALSE;
									InitTrashcan(tnode);
									AddTail((struct List *)&Trashcan.Trashcans,(struct Node *)tnode);
								}
								else
									FreeVec(remname);
							}
							GT_SetGadgetAttrs(PrefsGadgets[GD_PREFS_TRASHCANS],PrefsWnd,NULL,GTLV_Labels,&Trashcan.Trashcans,TAG_DONE);
						}
						UnLockWindows();
						break;
					case GD_PREFS_REMOVE:
						if(SelectedList)
						{
							GT_SetGadgetAttrs(PrefsGadgets[GD_PREFS_TRASHCANS],PrefsWnd,NULL,GTLV_Labels,NULL,TAG_DONE);
							tnode=(struct TNode *)Trashcan.Trashcans.mlh_Head;
							unless(IsListEmpty((struct List *)&Trashcan.Trashcans))
							{
								a=0;
								while(a<SelectedList-1)
								{
									if(tnode->ln_Succ) tnode=(struct TNode *)NextNode(tnode);
									a++;
								}
								if(tnode)
								{
									FreeTrashcan(tnode);
									Remove((struct Node *)tnode);
									FreeVec(tnode->ln_Name);
									FreeVec(tnode);
								}
							}
							SelectedList=0;
							GT_SetGadgetAttrs(PrefsGadgets[GD_PREFS_TRASHCANS],PrefsWnd,NULL,GTLV_Labels,&Trashcan.Trashcans,TAG_DONE);
						}
						break;
					case GD_PREFS_FORCEDELETE:
						FDSelectedList=code+1;
						break;
					case GD_PREFS_FD_ADD:
						LockWindows();
						name[0]='\0';
						if(rtGetString(name,108,GetTrashmanString(MSG_FORCE_DELETE_TITLE),NULL,
								RT_Window,PrefsWnd,
								RT_ReqPos,REQPOS_CENTERSCR,
								TAG_DONE))
						{
							GT_SetGadgetAttrs(PrefsGadgets[GD_PREFS_FORCEDELETE],PrefsWnd,NULL,GTLV_Labels,NULL,TAG_DONE);
							if(remname=AllocVec(strlen(name)+1,MEMF_ANY))
							{
								strcpy(remname,name);
								if(node=AllocVec(sizeof(struct Node),MEMF_ANY))
								{
									node->ln_Name=remname;
									AddTail((struct List *)&Trashcan.Filters,node);
								}
								else
									FreeVec(remname);
							}
							GT_SetGadgetAttrs(PrefsGadgets[GD_PREFS_FORCEDELETE],PrefsWnd,NULL,GTLV_Labels,&Trashcan.Filters,TAG_DONE);
						}
						UnLockWindows();
						break;
					case GD_PREFS_FD_REMOVE:
						if(FDSelectedList)
						{
							GT_SetGadgetAttrs(PrefsGadgets[GD_PREFS_FORCEDELETE],PrefsWnd,NULL,GTLV_Labels,NULL,TAG_DONE);
							node=(struct Node *)Trashcan.Filters.mlh_Head;
							unless(IsListEmpty((struct List *)&Trashcan.Filters))
							{
								a=0;
								while(a<FDSelectedList-1)
								{
									if(node->ln_Succ) node=node->ln_Succ;
									a++;
								}
								if(node)
								{
									Remove(node);
									FreeVec(node->ln_Name);
									FreeVec(node);
								}
							}
							FDSelectedList=0;
							GT_SetGadgetAttrs(PrefsGadgets[GD_PREFS_FORCEDELETE],PrefsWnd,NULL,GTLV_Labels,&Trashcan.Filters,TAG_DONE);
						}
						break;
				}
				break;
		}
		GT_ReplyIMsg(message);
	}
skip:;
}

void HandleDropWnd()
{
	while(message=GT_GetIMsg(DropWnd->UserPort))
	{
		class  = message->Class;
		code   = message->Code;
		switch(class)
		{
			case IDCMP_CHANGEWINDOW: break;
			case IDCMP_MOUSEBUTTONS:
				switch(code)
				{
					case SELECTDOWN:
						CurrentTime(&EndSeconds,&EndMicros);
						if(DoubleClick(StartSeconds,StartMicros,EndSeconds,EndMicros))
						{
							if(MainWnd)
							{
								ActivateWindow(MainWnd);
								WindowToFront(MainWnd);
							}
							else OpenMainWindow();
						}
						CurrentTime(&StartSeconds,&StartMicros);
						break;
				}
				break;
		}
		GT_ReplyIMsg(message);
	}
}

void MakeFileList(struct AppMessage *msg)
{
	struct DeletedFile *df;
	struct WBArg *WBArg;
	TEXT name[108];
	int a;
	BPTR lock=NULL;
	BOOL TryAgain=TRUE;

	df=(struct DeletedFile *) Trashcan.Files.mlh_Head;
	for(WBArg=msg->am_ArgList,a=0;a<msg->am_NumArgs;a++,WBArg++)
	{
		NameFromLock(WBArg->wa_Lock,name,108);
		if(WBArg->wa_Name[0]!='\0') AddPart(name,WBArg->wa_Name,108);

tryagain:
		if(lock=Lock(name,ACCESS_READ))
		{
			UnLock(lock);
			unless(MyDeleteFile(name)) ShowError(GetTrashmanString(MSG_DELETE_ERROR));
			if(TryAgain)
			{
				strcat(name,".info");
				TryAgain=FALSE;
				goto tryagain;
			}
		}
		else
		{
			if(TryAgain)
			{
				strcat(name,".info");
				TryAgain=FALSE;
				goto tryagain;
			}
		}
		ProgressCnt++;
	}

	df=(struct DeletedFile *) Trashcan.Files.mlh_Head;
	Trashcan.Entries=0;
	WhileNode(df)
	{
		Trashcan.Entries++;
		df=(struct DeletedFile *) NextNode(df);
	}
	ProgressCnt=0;
}

struct DeletedFile *DoFile(BPTR lock,struct FileInfoBlock *fib,STRPTR name)
{
	struct DeletedFile *df=NULL;
	TEXT time[LEN_DATSTRING],date[LEN_DATSTRING];
	struct DateTime dt={NULL,FORMAT_DOS,DTF_SUBST,NULL,NULL,NULL};

	dt.dat_StrDate=date;
	dt.dat_StrTime=time;

	InfoPrint(name,ProgressCnt);
	if(df=AllocVec(sizeof(struct DeletedFile),MEMF_ANY))
	{
		strcpy(df->OriginalFileName,name);
		strcpy(df->FileName,FilePart(name));
		df->Size=fib->fib_Size;
		df->Type=TYPE_FILE;
		df->Protection=fib->fib_Protection;
		df->Selected=FALSE;
		CopyMem(&fib->fib_Date,&df->Date,sizeof(struct DateStamp));
		CopyMem(&fib->fib_Date,&dt.dat_Stamp,sizeof(struct DateStamp));
		DateToStr(&dt);
		sprintf(df->DateStr,"%s %s",date,time);
		strcpy(df->ProtectionStr,"----rwed");
		GetFlag(df->Protection,(1<<7)) df->ProtectionStr[0]='h';
		GetFlag(df->Protection,FIBF_SCRIPT) df->ProtectionStr[1]='s';
		GetFlag(df->Protection,FIBF_PURE) df->ProtectionStr[2]='p';
		GetFlag(df->Protection,FIBF_ARCHIVE) df->ProtectionStr[3]='a';
		GetFlag(df->Protection,FIBF_READ) df->ProtectionStr[4]='-';
		GetFlag(df->Protection,FIBF_WRITE) df->ProtectionStr[5]='-';
		GetFlag(df->Protection,FIBF_EXECUTE) df->ProtectionStr[6]='-';
		GetFlag(df->Protection,FIBF_DELETE) df->ProtectionStr[7]='-';
		AddTail(List(&Trashcan.Files),Node(df));
	}
	return df;
}

struct DeletedFile *DoDirectory(BPTR lock,struct FileInfoBlock *fib,STRPTR name)
{
	struct DeletedFile *df=NULL;
	TEXT time[LEN_DATSTRING],date[LEN_DATSTRING];
	struct DateTime dt={NULL,FORMAT_DOS,NULL,NULL,NULL,NULL};

	dt.dat_StrDate=date;
	dt.dat_StrTime=time;

	InfoPrint(name,ProgressCnt);
	if(df=AllocVec(sizeof(struct DeletedFile),MEMF_ANY))
	{
		strcpy(df->OriginalFileName,name);
		strcpy(df->FileName,FilePart(name));
		df->Size=NULL;
		df->Type=TYPE_DIR;
		df->Protection=fib->fib_Protection;
		df->Selected=FALSE;
		CopyMem(&fib->fib_Date,&df->Date,sizeof(struct DateStamp));
		CopyMem(&fib->fib_Date,&dt.dat_Stamp,sizeof(struct DateStamp));
		DateToStr(&dt);
		sprintf(df->DateStr,"%s %s",date,time);
		strcpy(df->ProtectionStr,"----rwed");
		GetFlag(df->Protection,(1<<7)) df->ProtectionStr[0]='h';
		GetFlag(df->Protection,FIBF_SCRIPT) df->ProtectionStr[1]='s';
		GetFlag(df->Protection,FIBF_PURE) df->ProtectionStr[2]='p';
		GetFlag(df->Protection,FIBF_ARCHIVE) df->ProtectionStr[3]='a';
		GetFlag(df->Protection,FIBF_READ) df->ProtectionStr[4]='-';
		GetFlag(df->Protection,FIBF_WRITE) df->ProtectionStr[5]='-';
		GetFlag(df->Protection,FIBF_EXECUTE) df->ProtectionStr[6]='-';
		GetFlag(df->Protection,FIBF_DELETE) df->ProtectionStr[7]='-';
		AddTail(List(&Trashcan.Files),Node(df));
	}
	return df;
}

/*
void DoDirectory(BPTR lock,struct FileInfoBlock *fib,STRPTR oldname)
{
	BPTR newlock;
	TEXT name[108];
	struct FileInfoBlock *newfib;

//	InfoPrint(name);
	while(ExNext(lock,fib))
	{
		strcpy(name,oldname);
		AddPart(name,fib->fib_FileName,108);
		if(fib->fib_DirEntryType<0)		// IF File
		{
			DoFile(lock,fib,name);
		}
		if(fib->fib_DirEntryType>=0)
		{
			if(newlock=Lock(name,ACCESS_READ))
			{
				if(newfib=AllocDosObjectTags(DOS_FIB,TAG_DONE))
				{
					if(Examine(newlock,newfib))
					{
						DoDirectory(newlock,newfib,name);
					}
					FreeDosObject(DOS_FIB,newfib);
				}
				UnLock(newlock);
			}
		}
	}
}
*/

void FreeFileList()
{
	struct DeletedFile *df;

	while(df=(struct DeletedFile *) RemTail(List(&Trashcan.Files))) FreeVec(df);
}

void MoveDelta()
{
	int a,Lines;
	struct DeletedFile *df,*tdf;
	ULONG fromx,fromy;
	WORD dy;

	Trashcan.maxlen=Trashcan.TotalMaxLen=0;
	if(MainWnd)
	{
		SetTitles();
		Trashcan.Lines=Lines=(MainWnd->GZZHeight-1)/Trashcan.FontYSize;
		Trashcan.Columns=(MainWnd->GZZWidth-3)/Trashcan.FontXSize;

		df=(struct DeletedFile *) Trashcan.Files.mlh_Head;
		GetAttr(PGA_Top,VertProp,&fromy);
		GetAttr(PGA_Top,HorizProp,&fromx);

		for(a=0;a<fromy;a++) if(NextNode(df)) df=(struct DeletedFile *) NextNode(df);

		dy=max(Trashcan.FromY,fromy)-min(Trashcan.FromY,fromy);
		if(dy<Trashcan.Lines)
		{
			if(max(Trashcan.FromY,fromy)==Trashcan.FromY) dy*=-1;
			ScrollRasterBF(MainWnd->RPort,0,dy*Trashcan.FontYSize,MainWnd->BorderLeft,MainWnd->BorderTop+1,MainWnd->Width-MainWnd->BorderRight-1,MainWnd->Height-MainWnd->BorderBottom-1);
		}

		tdf=(struct DeletedFile *) Trashcan.Files.mlh_Head;
		for(a=0;a<Trashcan.Entries;a++)
		{
			if(Trashcan.maxlen<strlen(tdf->FileName)) Trashcan.maxlen=strlen(tdf->FileName);
			tdf=(struct DeletedFile *) NextNode(tdf);
		}

		if(dy<0)
		{
			for(a=0;a<Trashcan.Entries && a<abs(dy) && a<Lines;a++)
			{
				DrawLine(df,a,fromx);
				df=(struct DeletedFile *)NextNode(df);
			}
		}
		else
		{
			for(a=0;a<Lines-(Lines<dy?Lines:dy);a++) if(NextNode(df)) df=(struct DeletedFile *) NextNode(df);
			for(a=Lines-(Lines<dy?Lines:dy);a<Trashcan.Entries && a<Lines;a++)
			{
				DrawLine(df,a,fromx);
				df=(struct DeletedFile *)NextNode(df);
			}
		}

		SetAPen(MainWnd->RPort,DRI->dri_Pens[BACKGROUNDPEN]);
		if(Lines>Trashcan.Entries) Lines=Trashcan.Entries;
		RectFill(MainWnd->RPort,MainWnd->BorderLeft,MainWnd->BorderTop+(Lines*Trashcan.FontYSize)+1,MainWnd->Width-MainWnd->BorderRight-1,MainWnd->Height-MainWnd->BorderBottom-1);
		RectFill(MainWnd->RPort,MainWnd->BorderLeft+(Trashcan.Columns*Trashcan.FontXSize)+2,MainWnd->BorderTop,MainWnd->Width-MainWnd->BorderRight-1,MainWnd->Height-MainWnd->BorderBottom-1);
		Trashcan.FromY=fromy;
		Trashcan.FromX=fromx;
	}
}

void DrawWindow()
{
	int a,Lines;
	struct DeletedFile *df,*tdf;
	ULONG fromx,fromy;

	Trashcan.maxlen=Trashcan.TotalMaxLen=0;
	if(MainWnd)
	{
		SetTitles();
		Trashcan.Lines=Lines=(MainWnd->GZZHeight-1)/Trashcan.FontYSize;
		Trashcan.Columns=(MainWnd->GZZWidth-3)/Trashcan.FontXSize;

		df=(struct DeletedFile *) Trashcan.Files.mlh_Head;
		GetAttr(PGA_Top,VertProp,&fromy);
		GetAttr(PGA_Top,HorizProp,&fromx);
		for(a=0;a<fromy;a++) if(NextNode(df)) df=(struct DeletedFile *) NextNode(df);

		tdf=(struct DeletedFile *) Trashcan.Files.mlh_Head;
		for(a=0;a<Trashcan.Entries;a++)
		{
			if(Trashcan.maxlen<strlen(tdf->FileName)) Trashcan.maxlen=strlen(tdf->FileName);
			tdf=(struct DeletedFile *) NextNode(tdf);
		}

		for(a=0;a<Trashcan.Entries && a<Lines;a++)
		{
			DrawLine(df,a,fromx);
			df=(struct DeletedFile *)NextNode(df);
		}
		SetAPen(MainWnd->RPort,DRI->dri_Pens[BACKGROUNDPEN]);
		if(Lines>Trashcan.Entries) Lines=Trashcan.Entries;
		RectFill(MainWnd->RPort,MainWnd->BorderLeft,MainWnd->BorderTop+(Lines*Trashcan.FontYSize)+1,MainWnd->Width-MainWnd->BorderRight-1,MainWnd->Height-MainWnd->BorderBottom-1);
		RectFill(MainWnd->RPort,MainWnd->BorderLeft+(Trashcan.Columns*Trashcan.FontXSize)+2,MainWnd->BorderTop,MainWnd->Width-MainWnd->BorderRight-1,MainWnd->Height-MainWnd->BorderBottom-1);
		Trashcan.FromY=fromy;
		Trashcan.FromX=fromx;
	}
}

void DrawLine(struct DeletedFile *df,ULONG a,ULONG fromx)
{
	int b;
	TEXT text[257],tmp[200];
	ULONG t;

	if(df->Selected)
	{
		SetAPen(MainWnd->RPort,DRI->dri_Pens[BACKGROUNDPEN]);
		if(df->Type==TYPE_FILE) SetBPen(MainWnd->RPort,DRI->dri_Pens[SHADOWPEN]); else SetBPen(MainWnd->RPort,DRI->dri_Pens[FILLPEN]);
	}
	else
	{
		if(df->Type==TYPE_FILE) SetAPen(MainWnd->RPort,DRI->dri_Pens[SHADOWPEN]); else SetAPen(MainWnd->RPort,DRI->dri_Pens[FILLPEN]);
		SetBPen(MainWnd->RPort,DRI->dri_Pens[BACKGROUNDPEN]);
	}

	sprintf(tmp,"%s",df->FileName);
	for(b=strlen(tmp);b<Trashcan.maxlen;b++) tmp[b]=' ';
	tmp[Trashcan.maxlen]='\0';

	if(df->Type==TYPE_FILE) sprintf(text,"%s %10d %18s %8s %.108s",tmp,df->Size,df->DateStr,df->ProtectionStr,df->OriginalFileName);
	else sprintf(text,"%s %10s %18s %8s %s",tmp,GetTrashmanString(MSG_DIR),df->DateStr,df->ProtectionStr,df->OriginalFileName);
	t=strlen(text);
	if(Trashcan.TotalMaxLen<t) Trashcan.TotalMaxLen=t;
	for(b=strlen(text);b<255;b++) text[b]=' ';
	text[256]='\0';
	Move(MainWnd->RPort,MainWnd->BorderLeft+2,MainWnd->BorderTop+a*Trashcan.FontYSize+Trashcan.FontBaseline+1);
	Text(MainWnd->RPort,text+fromx,Trashcan.Columns);
}

void UpdateProps()
{
	int Lines,Columns;
	struct PropInfo *pi;

	if(MainWnd)
	{
		Lines=(MainWnd->GZZHeight-1)/Trashcan.FontYSize;
		Columns=(MainWnd->GZZWidth-3)/Trashcan.FontXSize;

		if(Lines>Trashcan.Entries) Lines=Trashcan.Entries;
		SetAttrs(VertProp,PGA_Visible,Lines,PGA_Total,Trashcan.Entries,TAG_DONE);
		SetAttrs(HorizProp,PGA_Visible,Columns,PGA_Total,Trashcan.TotalMaxLen,TAG_DONE);

		pi= (struct PropInfo *) GAD(VertProp)->SpecialInfo;
		NewModifyProp(GAD(VertProp),MainWnd,NULL,pi->Flags, pi->HorizPot, pi->VertPot, pi->HorizBody, pi->VertBody, 1);
		pi= (struct PropInfo *) GAD(HorizProp)->SpecialInfo;
		NewModifyProp(GAD(HorizProp),MainWnd,NULL,pi->Flags, pi->HorizPot, pi->VertPot, pi->HorizBody, pi->VertBody, 1);
	}
}

int SelectFile(ULONG y,int wmd)
{
	BOOL ret;
	int b;
	ULONG FileNum,fromx,fromy;
	struct DeletedFile *df;
	TEXT text[512],tmp[200];

	GetAttr(PGA_Top,VertProp,&fromy);
	GetAttr(PGA_Top,HorizProp,&fromx);
	FileNum=((y-MainWnd->BorderTop)/Trashcan.FontYSize)+fromy;

	if(FileNum<Trashcan.Entries)
	{
		df=(struct DeletedFile *) Trashcan.Files.mlh_Head;
		for(b=0;b<FileNum;b++) df=(struct DeletedFile *) NextNode(df);
		if(wmd==-1)
		{
			if(df->Selected) df->Selected=FALSE; else df->Selected=TRUE;
			ret=df->Selected;
		}
		else
		{
			df->Selected=wmd;
			ret=wmd;
		}
		if(df->Selected)
		{
			SetAPen(MainWnd->RPort,DRI->dri_Pens[BACKGROUNDPEN]);
			if(df->Type==TYPE_FILE) SetBPen(MainWnd->RPort,DRI->dri_Pens[SHADOWPEN]); else SetBPen(MainWnd->RPort,DRI->dri_Pens[FILLPEN]);
		}
		else
		{
			if(df->Type==TYPE_FILE) SetAPen(MainWnd->RPort,DRI->dri_Pens[SHADOWPEN]); else SetAPen(MainWnd->RPort,DRI->dri_Pens[FILLPEN]);
			SetBPen(MainWnd->RPort,DRI->dri_Pens[BACKGROUNDPEN]);
		}
		sprintf(tmp,"%s",df->FileName);
		for(b=strlen(tmp);b<Trashcan.maxlen;b++) tmp[b]=' ';
		tmp[Trashcan.maxlen]='\0';

		if(df->Type==TYPE_FILE) sprintf(text,"%s %10d %18s %8s %.108s",tmp,df->Size,df->DateStr,df->ProtectionStr,df->OriginalFileName);
		else sprintf(text,"%s %10s %18s %8s %.108s",tmp,GetTrashmanString(MSG_DIR),df->DateStr,df->ProtectionStr,df->OriginalFileName);
		for(b=strlen(text);b<512;b++) text[b]=' ';
		text[256]='\0';
		Move(MainWnd->RPort,MainWnd->BorderLeft+2,MainWnd->BorderTop+((FileNum-fromy)*Trashcan.FontYSize)+Trashcan.FontBaseline+1);
		Text(MainWnd->RPort,text+fromx,Trashcan.Columns);
		return ret;
	}
	if(wmd!=-1) return wmd;
	else return -1;
}

int SysISize()
{
	return (Scr->Flags & SCREENHIRES ? SYSISIZE_MEDRES : SYSISIZE_LOWRES);
}

Object *NewImageObject(ULONG which)
{
	return NewObject(NULL, SYSICLASS,
		SYSIA_DrawInfo, DRI,
		SYSIA_Which, which,
		SYSIA_Size, SysISize(),
		TAG_DONE);
}

#define IM_WIDTH(o) ((struct Image *) o)->Width
#define IM_HEIGHT(o) ((struct Image *) o)->Height

int OpenMainWindow()
{
	unless(LeftButton=NewObject(NULL, BUTTONGCLASS,
		ICA_TARGET, ICTARGET_IDCMP,
		GA_Image, LeftImage,
		GA_RelRight, 1-(IM_WIDTH(LeftImage)+IM_WIDTH(SizeImage)+IM_WIDTH(RightImage)),
		GA_RelBottom, 1-(IM_HEIGHT(LeftImage)),
		GA_BottomBorder,TRUE,
		GA_ID, GD_LEFT,
		TAG_DONE)) return 1;
	unless(RightButton=NewObject(NULL, BUTTONGCLASS,
		ICA_TARGET, ICTARGET_IDCMP,
		GA_Image, RightImage,
		GA_RelRight, 1-(IM_WIDTH(RightImage)+IM_WIDTH(SizeImage)),
		GA_RelBottom, 1-(IM_HEIGHT(RightImage)),
		GA_BottomBorder,TRUE,
		GA_Previous,LeftButton,
		GA_ID, GD_RIGHT,
		TAG_DONE)) return 1;
	unless(DownButton=NewObject(NULL, BUTTONGCLASS,
		ICA_TARGET, ICTARGET_IDCMP,
		GA_Image, DownImage,
		GA_RelRight, 1-(IM_WIDTH(DownImage)),
		GA_RelBottom, 1-(IM_HEIGHT(SizeImage)+IM_HEIGHT(DownImage)),
		GA_RightBorder,TRUE,
		GA_Previous,RightButton,
		GA_ID, GD_DOWN,
		TAG_DONE)) return 1;
	unless(UpButton=NewObject(NULL, BUTTONGCLASS,
		ICA_TARGET, ICTARGET_IDCMP,
		GA_Image, UpImage,
		GA_RelRight, 1-(IM_WIDTH(UpImage)),
		GA_RelBottom, 1-(IM_HEIGHT(SizeImage)+IM_HEIGHT(DownImage)+IM_HEIGHT(UpImage)),
		GA_RightBorder,TRUE,
		GA_Previous,DownButton,
		GA_ID, GD_UP,
		TAG_DONE)) return 1;
	unless(HorizProp=NewObject(NULL, PROPGCLASS,
		ICA_TARGET, ICTARGET_IDCMP,
		PGA_Freedom, FREEHORIZ,
		PGA_NewLook, TRUE,
		PGA_Borderless, ((DRI->dri_Flags & DRIF_NEWLOOK) && DRI->dri_Depth != 1),
		GA_Left, 3,
		GA_RelBottom, -(IM_HEIGHT(SizeImage)-3),
		GA_RelWidth, -(4+IM_WIDTH(LeftImage)+IM_WIDTH(RightImage)+IM_WIDTH(SizeImage)+1),
		GA_Height, IM_HEIGHT(SizeImage)-4,
		GA_BottomBorder, TRUE,
		GA_Previous,UpButton,
		GA_ID, GD_HORIZ,
		PGA_Total, Trashcan.TotalMaxLen,
		PGA_Visible, Trashcan.TotalMaxLen,
		TAG_DONE)) return 1;
	unless(VertProp=NewObject(NULL, PROPGCLASS,
		ICA_TARGET, ICTARGET_IDCMP,
		PGA_Freedom, FREEVERT,
		PGA_NewLook, TRUE,
		PGA_Borderless, ((DRI->dri_Flags & DRIF_NEWLOOK) && DRI->dri_Depth != 1),
		GA_RelRight, -(IM_WIDTH(SizeImage)-5),
		GA_Top, Scr->WBorTop+Scr->Font->ta_YSize+1+1,
		GA_Width, IM_WIDTH(SizeImage)-2-2-4,
		GA_RelHeight, -(IM_HEIGHT(SizeImage)+IM_HEIGHT(UpImage)+IM_HEIGHT(DownImage)+Scr->WBorTop+Scr->Font->ta_YSize+1+1+1),
		GA_RightBorder, TRUE,
		GA_Previous,HorizProp,
		GA_ID, GD_VERT,
		PGA_Total, Trashcan.Entries,
		PGA_Visible, Trashcan.Entries,
		TAG_DONE)) return 1;

	if ( ! ( DummyMenus = CreateMenus( DummyNewMenu, TAG_DONE )))
		return( 3L );

	LayoutMenus( DummyMenus, VisualInfo, GTMN_NewLookMenus,TRUE, TAG_DONE );

	unless(MainWnd=OpenWindowTags(NULL,
						WA_Left,Prefs.MainLeft,
						WA_Top,Prefs.MainTop,
						WA_Width,Prefs.MainWidth,
						WA_Height,Prefs.MainHeight,
						WA_Gadgets,LeftButton,
						WA_IDCMP,IDCMP_CLOSEWINDOW|IDCMP_MOUSEBUTTONS|IDCMP_MOUSEMOVE|IDCMP_GADGETUP|IDCMP_GADGETDOWN|IDCMP_NEWSIZE|IDCMP_IDCMPUPDATE|IDCMP_MENUPICK|IDCMP_CHANGEWINDOW|IDCMP_INTUITICKS|IDCMP_ACTIVEWINDOW,
						WA_Flags,WFLG_CLOSEGADGET|WFLG_DRAGBAR|WFLG_DEPTHGADGET|WFLG_SIZEGADGET|WFLG_NEWLOOKMENUS|WFLG_REPORTMOUSE|WFLG_ACTIVATE|WFLG_SIZEBRIGHT|WFLG_SIZEBBOTTOM,
						WA_PubScreen,	Scr,
						WA_Title,GetTrashmanString(MSG_WINDOW_TITLE),
						WA_ScreenTitle,GetTrashmanString(MSG_SCREEN_TITLE),
						WA_MaxWidth, -1,
						WA_MaxHeight, -1,
						WA_MinWidth, 100,
						WA_MinHeight, 2*(Scr->WBorTop+Scr->Font->ta_YSize+1)-1,
						WA_AutoAdjust, TRUE,
						WA_RptQueue, 2,
						TAG_DONE)) return 1;

	unless(AppMainWnd=AddAppWindow(APP_MAINWND,0,MainWnd,AppMsgPort,TAG_DONE)) return 2;
	SetMenuStrip( MainWnd, DummyMenus );
	GT_RefreshWindow( MainWnd, NULL );
	SetMaxPen(MainWnd->RPort,FindMaxPen());
	SetFont(MainWnd->RPort,MainWndFont);
	Trashcan.FontYSize=MainWndFont->tf_YSize;
	Trashcan.FontXSize=MainWndFont->tf_XSize;
	Trashcan.FontBaseline=MainWndFont->tf_Baseline;

	MakeTrashcansList();

	SetupBits();
	DrawWindow();
	UpdateProps();
	FreeSpace();
	return 0;
}

void CloseMainWindow(void)
{
	if(AppMainWnd) RemoveAppWindow(AppMainWnd);
	AppMainWnd=NULL;
	if(DummyMenus)
	{
		ClearMenuStrip(MainWnd);
		FreeMenus(DummyMenus);
		DummyMenus=NULL;
	}
	if(MainWnd) CloseWindow(MainWnd);
	MainWnd=NULL;
	if(VertProp) DisposeObject(VertProp);
	VertProp=NULL;
	if(HorizProp) DisposeObject(HorizProp);
	HorizProp=NULL;
	if(UpButton) DisposeObject(UpButton);
	UpButton=NULL;
	if(DownButton) DisposeObject(DownButton);
	DownButton=NULL;
	if(RightButton) DisposeObject(RightButton);
	RightButton=NULL;
	if(LeftButton) DisposeObject(LeftButton);
	LeftButton=NULL;

	FreeFileList();

	SetupBits();

	FreeSpace();
}

void InitTrashcan(struct TNode *node)
{
	STRPTR newname;
	ULONG uid,s=0;

	if(!node->ln_Lock) node->ln_Lock=Lock(node->ln_Name,ACCESS_READ);
	if(!node->ln_InitNR)
	{
		if(MultiUser)
		{
			if(uid=muGetTaskOwner(NULL))
			{
				muUI->uid=(UWORD)(uid>>16);
				muGetUserInfo(muUI,muKeyType_uid);
				s=strlen(muUI->UserID)+1;
			}
		}
		if(newname=AllocVec(strlen(node->ln_Name)+1+strlen("Trashcan")+s,MEMF_ANY))
		{
			if(MultiUser) sprintf(newname,"%sTrashcan/%s",node->ln_Name,muUI->UserID);
			else sprintf(newname,"%sTrashcan",node->ln_Name);
			node->ln_NR.nr_Name=newname;
			node->ln_NR.nr_UserData=(long)newname;
			node->ln_NR.nr_Flags=NRF_SEND_MESSAGE;
			node->ln_NR.nr_stuff.nr_Msg.nr_Port=NotifyMsgPort;
			node->ln_NR.nr_Handler=NULL;
			node->ln_NR.nr_FullName=NULL;
			node->ln_NR.nr_MsgCount=NULL;
			StartNotify(&node->ln_NR);
			node->ln_InitNR=TRUE;
		}
	}
}

void FreeTrashcan(struct TNode *node)
{
	if(node->ln_InitNR)
	{
		EndNotify(&node->ln_NR);
		if(node->ln_NR.nr_Name) FreeVec(node->ln_NR.nr_Name);
	}
	if(node->ln_Lock) UnLock(node->ln_Lock);
	node->ln_Lock=node->ln_InitNR=FALSE;
}

void InitTrashcansList()
{
	struct TNode *node;

	node=(struct TNode *)Trashcan.Trashcans.mlh_Head;
	WhileNode(node)
	{
		InitTrashcan(node);
		node=(struct TNode *)NextNode(node);
	}
}

void FreeTrashcansList()
{
	struct TNode *node;

	while(node=(struct TNode *)RemTail((struct List *) &Trashcan.Trashcans))
	{
		FreeTrashcan(node);
		if(node->ln_Name) FreeVec(node->ln_Name);
		FreeVec(node);
	}
}

void FreeFiltersList()
{
	struct Node *node;

	while(node=RemTail((struct List *) &Trashcan.Filters))
	{
		if(node->ln_Name) FreeVec(node->ln_Name);
		FreeVec(node);
	}
}

int OpenPreferences()
{
	if(!OpenPrefsWindow())
	{
		SetupBits();
		RunPrefs=TRUE;
		CopyMem(&Prefs.Flags,&TmpPrefs.Flags,sizeof(struct Prefs));
		GetFlag(Prefs.Flags,PREFS_PATCHDELETE) GT_SetGadgetAttrs(PrefsGadgets[GD_PREFS_PATCHDELETE],PrefsWnd,NULL,GTCB_Checked,TRUE,TAG_DONE);
		GetFlag(Prefs.Flags,PREFS_ANIMATIONS) GT_SetGadgetAttrs(PrefsGadgets[GD_PREFS_ANIMATIONS],PrefsWnd,NULL,GTCB_Checked,TRUE,TAG_DONE);
		GetFlag(Prefs.Flags,PREFS_APPWINDOW) GT_SetGadgetAttrs(PrefsGadgets[GD_PREFS_APPWINDOW],PrefsWnd,NULL,GTCB_Checked,TRUE,TAG_DONE);
		GetFlag(Prefs.Flags,PREFS_APPICON) GT_SetGadgetAttrs(PrefsGadgets[GD_PREFS_APPICON],PrefsWnd,NULL,GTCB_Checked,TRUE,TAG_DONE);
		GetFlag(Prefs.Flags,PREFS_APPMENU) GT_SetGadgetAttrs(PrefsGadgets[GD_PREFS_APPMENU],PrefsWnd,NULL,GTCB_Checked,TRUE,TAG_DONE);
		GetFlag(Prefs.Flags,PREFS_ASKBEFOREDELETE) GT_SetGadgetAttrs(PrefsGadgets[GD_PREFS_ASKBEFOREDELETE],PrefsWnd,NULL,GTCB_Checked,TRUE,TAG_DONE);
		GetFlag(Prefs.Flags,PREFS_PATCHOPEN) GT_SetGadgetAttrs(PrefsGadgets[GD_PREFS_PATCHOPEN],PrefsWnd,NULL,GTCB_Checked,TRUE,TAG_DONE);
		GetFlag(Prefs.Flags,PREFS_HIDECLOSESALL) GT_SetGadgetAttrs(PrefsGadgets[GD_PREFS_HIDECLOSESALL],PrefsWnd,NULL,GTCB_Checked,TRUE,TAG_DONE);
		GetFlag(Prefs.Flags,PREFS_DELETEZEROK) GT_SetGadgetAttrs(PrefsGadgets[GD_PREFS_DELETEZEROK],PrefsWnd,NULL,GTCB_Checked,TRUE,TAG_DONE);
		GT_SetGadgetAttrs(PrefsGadgets[GD_PREFS_OVERWRITE],PrefsWnd,NULL,GTMX_Active,Prefs.Overwrite,TAG_DONE);
		GT_SetGadgetAttrs(PrefsGadgets[GD_PREFS_FREEDISK],PrefsWnd,NULL,GTIN_Number,Prefs.FreeDisk,TAG_DONE);
		GT_SetGadgetAttrs(PrefsGadgets[GD_PREFS_TRASHCANS],PrefsWnd,NULL,GTLV_Labels,&Trashcan.Trashcans,TAG_DONE);
		GT_SetGadgetAttrs(PrefsGadgets[GD_PREFS_FORCEDELETE],PrefsWnd,NULL,GTLV_Labels,&Trashcan.Filters,TAG_DONE);
		GT_SetGadgetAttrs(PrefsGadgets[GD_PREFS_SOUND],PrefsWnd,NULL,GTST_String,Prefs.Sound,TAG_DONE);
		SelectedList=0;
		return 1;
	}
	return 0;
}

void ClosePreferences()
{
	if(PrefsWnd) ClosePrefsWindow();
	SetupBits();
}

int OpenDropWindow()
{
	unless(DropWnd=OpenWindowTags(NULL,
						WA_Left,Prefs.DropLeft,
						WA_Top,Prefs.DropTop,
						WA_InnerWidth,AppDO->do_Gadget.Width,
						WA_InnerHeight,AppDO->do_Gadget.Height,
						WA_IDCMP,IDCMP_MOUSEBUTTONS|IDCMP_CHANGEWINDOW,
						WA_Flags,WFLG_DRAGBAR|WFLG_DEPTHGADGET|WFLG_RMBTRAP,
						WA_PubScreen,	Scr,
						WA_ScreenTitle,GetTrashmanString(MSG_SCREEN_TITLE),
						WA_AutoAdjust, TRUE,
						TAG_DONE)) return 1;
	DrawImage(DropWnd->RPort,(struct Image *)AppDO->do_Gadget.GadgetRender,DropWnd->BorderLeft,DropWnd->BorderTop);
	unless(AppDropWnd=AddAppWindow(APP_DROPWND,0,DropWnd,AppMsgPort,TAG_DONE)) return 2;
	SetupBits();
	return 0;
}

void CloseDropWindow()
{
	if(AppDropWnd) RemoveAppWindow(AppDropWnd);
	AppDropWnd=NULL;
	if(DropWnd) CloseWindow(DropWnd);
	DropWnd=NULL;
}

BOOL IsDir(BPTR lock)
{
	if(Examine(lock,FIB)) if(FIB->fib_DirEntryType>0) return TRUE;
	return FALSE;
}

BPTR __saveds __asm MyOpen(register __d1 STRPTR name, register __d2 LONG accessMode)
{
	BPTR lock,LockSrcDev;
	TEXT FullName[108],Device[108];
	struct TNode *node;
	BOOL dir;

	if(accessMode==MODE_NEWFILE)
	{
		if(lock=Lock(name,ACCESS_READ))
		{
			NameFromLock(lock,FullName,108);
			CopyDeviceName(FullName,Device);
			dir=IsDir(lock);
			UnLock(lock);

			if(!dir)
			{
				if(LockSrcDev=Lock(Device,ACCESS_READ))
				{
					node=(struct TNode *)Trashcan.Trashcans.mlh_Head;
					WhileNode(node)
					{
						if(SameLock(LockSrcDev,node->ln_Lock)==LOCK_SAME_VOLUME)
						{
							UnLock(LockSrcDev);
							MyDeleteFile(name);
							goto ok;
						}
						node=(struct TNode *)NextNode(node);
					}
					UnLock(LockSrcDev);
				}
			}
		}
	}
ok:
	return CallOpen(OldOpen,name,accessMode);
}

#define LinkError()	\
	{	\
		UnLock(LockSrc);	\
		SetIoErr(ERROR_OBJECT_LINKED);	\
		return FALSE;	\
	}	\

BOOL __saveds __asm MyDeleteFile(register __d1 STRPTR name)
{
	BPTR LockSrcDev,LockSrc,LockDest;
	TEXT FullName[108],Device[108],FileDest[108],BumpFileDest[108];
	struct TNode *node;
	struct DeletedFile *df;
	int prev;
	BOOL ret;
	ULONG uid;

	if(LockSrc=Lock(name,ACCESS_READ))
	{
		NameFromLock(LockSrc,FullName,108);
		if(CheckFilter(FullName)) goto deleteorg;
		CopyDeviceName(FullName,Device);
	
		if(LockSrcDev=Lock(Device,ACCESS_READ))
		{
			node=(struct TNode *)Trashcan.Trashcans.mlh_Head;
			WhileNode(node)
			{
				if(SameDevice(LockSrcDev,node->ln_Lock))
				{
					UnLock(LockSrcDev);
					goto same;
				}
				node=(struct TNode *)NextNode(node);
			}
			UnLock(LockSrcDev);
		}

deleteorg:
		InfoPrint(name,ProgressCnt);
		UnLock(LockSrc);
		if(CallDeleteFile(OldDeleteFile,name)) return TRUE;
		else
		{
			if(IoErr()==ERROR_DIRECTORY_NOT_EMPTY) return MyDeleteRecursive(name);
			else return FALSE;
		}

same:
		if(MultiUser)
		{
			if(uid=muGetTaskOwner(NULL))
			{
				muUI->uid=(UWORD)(uid>>16);
				if(muGetUserInfo(muUI,muKeyType_uid))
					sprintf(FileDest,"%sTrashcan/%s/%s",Device,muUI->UserID,FilePart(FullName));
			}
			else
				sprintf(FileDest,"%sTrashcan/%s",Device,FilePart(FullName));
		}
		else
			sprintf(FileDest,"%sTrashcan/%s",Device,FilePart(FullName));

		if(strstr(FullName,":Trashcan")) goto deleteorg;
	
		if(Examine(LockSrc,FIB))
		{
			switch(FIB->fib_DirEntryType)
			{
				case ST_SOFTLINK: if(RequestUser(GetTrashmanString(MSG_FILE_IS_LINK),GetTrashmanString(MSG_DELETE_LINK_GAD),FilePart(name),MSG_SOFT_LINK)) goto deleteorg; else LinkError(); break;
				case ST_LINKDIR: if(RequestUser(GetTrashmanString(MSG_FILE_IS_LINK),GetTrashmanString(MSG_DELETE_LINK_GAD),FilePart(name),MSG_HARD_LINK_DIR)) goto deleteorg; else LinkError(); break;
				case ST_LINKFILE: if(RequestUser(GetTrashmanString(MSG_FILE_IS_LINK),GetTrashmanString(MSG_DELETE_LINK_GAD),FilePart(name),MSG_HARD_LINK_FILE)) goto deleteorg; else LinkError(); break;
				default: break;
			}
			GetFlag(FIB->fib_Protection,FIBF_DELETE)
			{
				UnLock(LockSrc);
				SetIoErr(ERROR_DELETE_PROTECTED);
				return FALSE;
			}
			if((FIB->fib_Size==0) && (FIB->fib_DirEntryType!=ST_USERDIR) && (Prefs.Flags&PREFS_DELETEZEROK)) goto deleteorg;
			if(MainWnd)
			{
				if(FIB->fib_DirEntryType<0) df=DoFile(LockSrc,FIB,FullName);
				if(FIB->fib_DirEntryType>=0) df=DoDirectory(LockSrc,FIB,FullName);
				if(df) strcpy(df->SourceFileName,FileDest);
//				GetProgramName(df->Program,30);
				Trashcan.Entries++;
				DrawWindow();
				UpdateProps();
			}
			else
				InfoPrint(name,ProgressCnt);
		}
		UnLock(LockSrc);

		SetComment(FullName,FullName);
		prev=-1;
tryagain:
		unless(ret=Rename(FullName,FileDest))
		{
			if(IoErr()==ERROR_OBJECT_EXISTS)
			{
					if(FIB->fib_DirEntryType==ST_USERDIR)
					{
						BumpRevision(BumpFileDest,FilePart(FileDest));
						strcpy(FilePart(FileDest),BumpFileDest);
						goto tryagain;
					}
					else
					switch(Prefs.Overwrite)
					{
						case PREFS_OVERWRITE_NEVER:
							BumpRevision(BumpFileDest,FilePart(FileDest));
							strcpy(FilePart(FileDest),BumpFileDest);
							break;
						case PREFS_OVERWRITE_ALWAYS:
							CallDeleteFile(OldDeleteFile,FileDest);
							break;
						case PREFS_OVERWRITE_ASK:
							if(prev==-1)
							{
								switch(RequestUser(GetTrashmanString(MSG_FILE_ALREADY_EXISTS_IN_TRASHCAN),GetTrashmanString(MSG_EXISTS_TRASHCAN_GAD),FilePart(FullName)))
								{
									case 1: // replace
										CallDeleteFile(OldDeleteFile,FileDest);
										break;
									case 2: // rename
										BumpRevision(BumpFileDest,FilePart(FileDest));
										strcpy(FilePart(FileDest),BumpFileDest);
										prev=2;
										break;
									case 3: // delete
										return CallDeleteFile(OldDeleteFile,FullName);
										break;
									case 0: // skip
										SetIoErr(ERROR_DELETE_PROTECTED);
										return FALSE;
										break;
								}
							}
							else
							{
								if(prev==2) // rename
								{
									BumpRevision(BumpFileDest,FilePart(FileDest));
									strcpy(FilePart(FileDest),BumpFileDest);
								}
							}
							break;
						case PREFS_OVERWRITE_SAMELOCATION:
							if(LockDest=Lock(FileDest,ACCESS_READ))
							{
								if(Examine(LockDest,FIB))
								{
									if(!strcmp(FIB->fib_Comment,FullName))
									{
										UnLock(LockDest);
										LockDest=NULL;
										CallDeleteFile(OldDeleteFile,FileDest);
									}
									else
									{
										BumpRevision(BumpFileDest,FilePart(FileDest));
										strcpy(FilePart(FileDest),BumpFileDest);
									}
								}
								if(LockDest) UnLock(LockDest);
							}
							break;
					}
					goto tryagain;
			}
			else return ret;
		}
		return(ret);
	}
	SetIoErr(IoErr());
	return(FALSE);
}

void RemoveSelected()
{
	struct DeletedFile *df,*tdf;
	ULONG all=0;

	LockWindows();
	df=(struct DeletedFile *) Trashcan.Files.mlh_Head;
	WhileNode(df)
	{
		if(df->Selected) all++;
		df=(struct DeletedFile *) NextNode(df);
	}
	if(all>0)
	{
		unless(OpenInfoWindow(GetTrashmanString(MSG_REMOVE_SELECTED),GetTrashmanString(MSG_DELETED_PROGRESS),all))
		{
			df=(struct DeletedFile *) Trashcan.Files.mlh_Head;
			all=0;
			WhileNode(df)
			{
				if(df->Selected)
				{
					InfoPrint(df->FileName,all);
					if(df->Type==TYPE_FILE)
					{
						if(CallDeleteFile(OldDeleteFile,df->SourceFileName))
							if(IoErr()==ERROR_DELETE_PROTECTED)
								SetProtection(df->SourceFileName,NULL);
					}
					if(df->Type==TYPE_DIR) MyDeleteRecursive(df->SourceFileName);
					tdf=df;
					df=(struct DeletedFile *) NextNode(df);
					Remove(Node(tdf));
					FreeVec(tdf);
					Trashcan.Entries--;
					all++;
				}
				else df=(struct DeletedFile *) NextNode(df);
			}
			CloseInfoWindow();
			DrawWindow();
			UpdateProps();
		}
	}
	UnLockWindows();
}

void RemoveAll()
{
	struct TNode *node;
	TEXT name[108];
	ULONG cnt=0,uid;
	BOOL noreq=FALSE;

//printf("CNT: 1\n");
	if(!MainWnd)
	{
		noreq=TRUE;
		node=(struct TNode *)Trashcan.Trashcans.mlh_Head;
		Trashcan.Entries=0;
		WhileNode(node) { Trashcan.Entries++; node=(struct TNode *)NextNode(node); }
	}
//printf("CNT: 2\n");
	LockWindows();
//printf("CNT: 3\n");
	if(Trashcan.Entries>0)
	{
		if(MultiUser)
		{
			if(uid=muGetTaskOwner(NULL))
			{
				muUI->uid=(UWORD)(uid>>16);
				muGetUserInfo(muUI,muKeyType_uid);
			}
//printf("CNT: 4\n");
		}
//printf("CNT: 5\n");
		unless(OpenInfoWindow(GetTrashmanString(MSG_REMOVE_ALL),GetTrashmanString(MSG_DELETED_PROGRESS),Trashcan.Entries))
		{
//printf("CNT: 6\n");
			PlaySound(ErrSoundObj);
//printf("CNT: 7\n");
			node=(struct TNode *)Trashcan.Trashcans.mlh_Head;
//printf("CNT: 8\n");
			WhileNode(node)
			{
//printf("CNT: 9\n");
				if(MultiUser) sprintf(name,"%sTrashcan/%s",node->ln_Name,muUI->UserID);
				else sprintf(name,"%sTrashcan",node->ln_Name);
				if(noreq) InfoPrint(node->ln_Name,cnt);
//printf("CNT: 10\n");
				DeleteDirectory(name,&cnt,noreq);
//printf("CNT: 11\n");
				node=(struct TNode *)NextNode(node);
				if(noreq) cnt++;
			}
//printf("CNT: 12\n");
			FreeFileList();
			Trashcan.Entries=0;
//printf("CNT: 13\n");
			DrawWindow();
//printf("CNT: 14\n");
			UpdateProps();
//printf("CNT: 15\n");
			CloseInfoWindow();
		}
//printf("CNT: 16\n");
	}
//printf("CNT: 17\n");
	if(noreq) Trashcan.Entries=0;
	UnLockWindows();
//printf("CNT: 18\n");
}

void DeleteDirectory(STRPTR dir,ULONG *cnt,BOOL noreq)
{
	struct FileInfoBlock *fib;
	BPTR lock;
	TEXT FullName[108];

	if(fib=AllocDosObjectTags(DOS_FIB,TAG_DONE))
	{
		if(lock=Lock(dir,ACCESS_READ))
		{
			if(Examine(lock,fib))
			{
				while(ExNext(lock,fib))
				{
					sprintf(FullName,"%s/%s",dir,fib->fib_FileName);
					InfoPrint(FullName,*cnt);
					if(fib->fib_DirEntryType<0)
					{
						SetProtection(FullName,NULL);
						CallDeleteFile(OldDeleteFile,FullName);
					}
					if(fib->fib_DirEntryType>=0) MyDeleteRecursive(FullName);
					UpdateProps();
					if(!noreq) (*cnt)++;
				}
			}
			UnLock(lock);
		}
		FreeDosObject(DOS_FIB,fib);
	}
}

void UndeleteSelected()
{
	struct DeletedFile *df,*tdf;
	ULONG all=0;

	LockWindows();
	df=(struct DeletedFile *) Trashcan.Files.mlh_Head;
	WhileNode(df)
	{
		if(df->Selected) all++;
		df=(struct DeletedFile *) NextNode(df);
	}
	unless(OpenInfoWindow(GetTrashmanString(MSG_UNDELETE_SELECTED),GetTrashmanString(MSG_UNDELETED_PROGRESS),all))
	{
		df=(struct DeletedFile *) Trashcan.Files.mlh_Head;
		all=0;
		WhileNode(df)
		{
			if(df->Selected)
			{
				Rename(df->SourceFileName,df->OriginalFileName);
				SetComment(df->OriginalFileName,"");
				InfoPrint(df->OriginalFileName,all);
				tdf=df;
				df=(struct DeletedFile *) NextNode(df);
				Remove(Node(tdf));
				FreeVec(tdf);
				Trashcan.Entries--;
				all++;
			}
			else df=(struct DeletedFile *) NextNode(df);
		}
		CloseInfoWindow();
		DrawWindow();
		UpdateProps();
	}
	UnLockWindows();
}

void UndeleteSelectedOther()
{
	struct DeletedFile *df,*tdf;
	TEXT name[108];
	ULONG all=0;

	LockWindows();
	df=(struct DeletedFile *) Trashcan.Files.mlh_Head;
	WhileNode(df)
	{
		if(df->Selected) all++;
		df=(struct DeletedFile *) NextNode(df);
	}
	if(all>0)
	{
		unless(OpenInfoWindow(GetTrashmanString(MSG_UNDELETE_SELECTED),GetTrashmanString(MSG_UNDELETED_PROGRESS),all))
		{
			if(rtFileRequest(OtherReq,name,GetTrashmanString(MSG_SELECT_DESTINATION),
					RT_Window,PrefsWnd,
					RT_ReqPos,REQPOS_CENTERSCR,
					RT_Underscore,DEF_UNDERSCORE,
					RTFI_OkText,GetTrashmanString(MSG_UNDELETE_GAD),
					RTFI_Flags,FREQF_NOFILES,
					TAG_DONE))
			{
				df=(struct DeletedFile *) Trashcan.Files.mlh_Head;
				all=0;
				WhileNode(df)
				{
					if(df->Selected)
					{
						InfoPrint(df->FileName,all);
						strcpy(name,OtherReq->Dir);
						AddPart(name,df->FileName,108);
						unless(Rename(df->SourceFileName,name))
						{
							if(IoErr()==ERROR_RENAME_ACROSS_DEVICES) { unless(MoveFile(df->SourceFileName,name)) { df=(struct DeletedFile *) NextNode(df); goto skip; } }
							else { ShowError(GetTrashmanString(MSG_UNDELETE_ERROR)); df=(struct DeletedFile *) NextNode(df); goto skip; }
						}
						SetComment(name,"");
						tdf=df;
						df=(struct DeletedFile *) NextNode(df);
						Remove(Node(tdf));
						FreeVec(tdf);
						Trashcan.Entries--;
skip:				all++;
					}
					else df=(struct DeletedFile *) NextNode(df);
				}
			}
			CloseInfoWindow();
			DrawWindow();
			UpdateProps();
		}
	}
	UnLockWindows();
}

void MakeTrashcansList()
{
	struct DeletedFile *df;
	struct TNode *node;
	TEXT name[108];
	ULONG uid;

	if(MainWnd)
	{
		SetWindowTitles(MainWnd,GetTrashmanString(MSG_READING_DIRECTORY),GetTrashmanString(MSG_SCREEN_TITLE));
		LockWindows();
	}
	if(MultiUser)
	{
		if(uid=muGetTaskOwner(NULL))
		{
			muUI->uid=(UWORD)(uid>>16);
			muGetUserInfo(muUI,muKeyType_uid);
		}
	}
	node=(struct TNode *)Trashcan.Trashcans.mlh_Head;
	WhileNode(node)
	{
		if(MultiUser) sprintf(name,"%sTrashcan/%s",node->ln_Name,muUI->UserID);
		else sprintf(name,"%sTrashcan",node->ln_Name);
		DirDirectory(name);
		node=(struct TNode *)NextNode(node);
	}

	df=(struct DeletedFile *) Trashcan.Files.mlh_Head;
	Trashcan.Entries=0;
	WhileNode(df)
	{
		Trashcan.Entries++;
		df=(struct DeletedFile *) NextNode(df);
	}
	SortDirs();
	if(MainWnd)
	{
		SetWindowTitles(MainWnd,WindowTitle,GetTrashmanString(MSG_SCREEN_TITLE));
		UnLockWindows();
	}
}

void DirDirectory(STRPTR dir)
{
	struct DeletedFile *df;
	struct FileInfoBlock *fib;
	BPTR lock;
	STRPTR FullName;

	if(fib=AllocDosObjectTags(DOS_FIB,TAG_DONE))
	{
		if(lock=Lock(dir,ACCESS_READ))
		{
			if(Examine(lock,fib))
			{
				while(ExNext(lock,fib))
				{
					FullName=fib->fib_FileName;
					if(fib->fib_DirEntryType<0) df=DoFile(lock,fib,FullName);
					if(fib->fib_DirEntryType>=0) df=DoDirectory(lock,fib,FullName);
					strcpy(df->OriginalFileName,fib->fib_Comment);
					strcpy(df->SourceFileName,dir);
					AddPart(df->SourceFileName,FullName,108);
				}
			}
			UnLock(lock);
		}
		FreeDosObject(DOS_FIB,fib);
	}
}

void About()
{
	TEXT ble[1024];
	struct TagItem tags[]={RTEZ_ReqTitle,NULL,RTEZ_Flags,EZREQF_CENTERTEXT,	RT_Underscore,DEF_UNDERSCORE,	RT_ReqPos,REQPOS_CENTERSCR,	TAG_DONE};

	tags[0].ti_Data=(ULONG) GetTrashmanString(MSG_REQTITLE);

	sprintf(ble,GetTrashmanString(MSG_ABOUT),VERSION,REVISION);
	LockWindows();
	rtEZRequest(ble,GetTrashmanString(MSG_ABOUT_OK_GAD),NULL,tags);
	UnLockWindows();
}

BOOL CheckFilter(STRPTR name)
{
	struct Node *node;
	TEXT pattern[1024];

	node=(struct Node *)Trashcan.Filters.mlh_Head;
	WhileNode(node)
	{
		ParsePatternNoCase(node->ln_Name,pattern,1024);
		if(MatchPatternNoCase(pattern,name)) return TRUE;
		node=NextNode(node);
	}
	return FALSE;
}

void SortDirs()
{
	struct DeletedFile **Table,*node;
	ULONG a,b;

	if(Trashcan.Entries>1)
	{
		if(Table=AllocVec(Trashcan.Entries*4,MEMF_ANY))
		{
			// Copy pointers from list to table
			a=0;
			while(node=(struct DeletedFile *)RemHead(List(&Trashcan.Files)))
			{
				Table[a]=node;
				a++;
			}
	
			// Sort table
			for(a=0;a<Trashcan.Entries;a++)
			{
				for(b=a;b<Trashcan.Entries;b++)
				{
					switch(Prefs.ViewBy)
					{
						case PREFS_VIEWBY_NAME:
							if(stricmp(Table[a]->FileName,Table[b]->FileName)>0) { node=Table[b]; Table[b]=Table[a]; Table[a]=node; }
							break;
						case PREFS_VIEWBY_SIZE:
							if(Table[a]->Size>Table[b]->Size) { node=Table[b]; Table[b]=Table[a]; Table[a]=node; }
							if(Table[a]->Size==Table[b]->Size) if(stricmp(Table[a]->FileName,Table[b]->FileName)>0) { node=Table[b]; Table[b]=Table[a]; Table[a]=node; }
							break;
						case PREFS_VIEWBY_DATE:
							if(CompareDates(&Table[a]->Date,&Table[b]->Date)<0) { node=Table[b]; Table[b]=Table[a]; Table[a]=node; }
							if(CompareDates(&Table[a]->Date,&Table[b]->Date)==0) if(stricmp(Table[a]->FileName,Table[b]->FileName)>0) { node=Table[b]; Table[b]=Table[a]; Table[a]=node; }
							break;
						case PREFS_VIEWBY_LOCATION:
							if(stricmp(Table[a]->OriginalFileName,Table[b]->OriginalFileName)>0) { node=Table[b]; Table[b]=Table[a]; Table[a]=node; }
							break;
					}
				}
			}

			// Create list
			for(a=0;a<Trashcan.Entries;a++) if(Table[a]->Type==TYPE_DIR) AddTail(List(&Trashcan.Files),Node(Table[a]));
			for(a=0;a<Trashcan.Entries;a++) if(Table[a]->Type==TYPE_FILE) AddTail(List(&Trashcan.Files),Node(Table[a]));
	
			FreeVec(Table);
		}
	}
}

void SelectAll()
{
	struct DeletedFile *node;

	node=(struct DeletedFile *)Trashcan.Files.mlh_Head;
	WhileNode(node)
	{
		node->Selected=TRUE;
		node=(struct DeletedFile *)NextNode(node);
	}
}

BOOL MyDeleteRecursive(STRPTR name)
{
	BPTR lock;
	TEXT newname[108];
	struct FileInfoBlock *fib;

	if(fib=AllocDosObjectTags(DOS_FIB,TAG_DONE))
	{
		if(lock=Lock(name,ACCESS_READ))
		{
			if(Examine(lock,fib))
			{
				while(ExNext(lock,fib))
				{
					strcpy(newname,name);
					AddPart(newname,fib->fib_FileName,108);
					if(fib->fib_DirEntryType<0)
					{
						SetProtection(newname,NULL);
						if(!CallDeleteFile(OldDeleteFile,newname))
						{
							UnLock(lock);
							FreeDosObject(DOS_FIB,fib);
							return FALSE;
						}
					}
					if(fib->fib_DirEntryType>=0)
						MyDeleteRecursive(newname);
				}
			}
			UnLock(lock);
		}
		FreeDosObject(DOS_FIB,fib);
	}
	SetProtection(name,NULL);
	return CallDeleteFile(OldDeleteFile,name);
}

#define ID_TRSH MAKE_ID('T','R','S','H')
#define ID_VOLS MAKE_ID('V','O','L','S')
#define ID_FILT MAKE_ID('F','I','L','T')

#define IFF_Error(num)		\
		{											\
			TEXT temp[100];			\
			sprintf(temp,"IFFError #%d",num);	\
			RequestUser(temp,GetTrashmanString(MSG_OK_GAD),NULL);	\
			goto skip;	\
		}

void SavePrefs(STRPTR name)
{
	struct IFFHandle *iffh;
	struct PrefHeader prefhdr={0,0,0};
	LONG error=0;
	struct TNode *tnode;
	struct Node *node;

	LockWindows();
	if(iffh=AllocIFF())
	{
		if(iffh->iff_Stream=Open(name,MODE_NEWFILE))
		{
			InitIFFasDOS(iffh);
			unless(error=OpenIFF(iffh,IFFF_WRITE))
			{
				unless(PushChunk(iffh,ID_PREF,ID_FORM,IFFSIZE_UNKNOWN))
				{
					if(error=PushChunk(iffh,NULL,ID_PRHD,sizeof(struct PrefHeader))) IFF_Error(error);
					WriteChunkBytes(iffh,(APTR)&prefhdr,sizeof(struct PrefHeader));
					if(error=PopChunk(iffh)) IFF_Error(error);
					if(error=PushChunk(iffh,NULL,ID_TRSH,sizeof(struct Prefs))) IFF_Error(error);
					WriteChunkBytes(iffh,(APTR)&Prefs,sizeof(struct Prefs));
					if(error=PopChunk(iffh)) IFF_Error(error);
					tnode=(struct TNode *)Trashcan.Trashcans.mlh_Head;
					WhileNode(tnode)
					{
						if(error=PushChunk(iffh,NULL,ID_VOLS,IFFSIZE_UNKNOWN)) IFF_Error(error);
						WriteChunkBytes(iffh,(APTR)tnode->ln_Name,strlen(tnode->ln_Name));
						if(error=PopChunk(iffh)) IFF_Error(error);
						tnode=(struct TNode *)NextNode(tnode);
					}
					node=(struct Node *)Trashcan.Filters.mlh_Head;
					WhileNode(node)
					{
						if(error=PushChunk(iffh,NULL,ID_FILT,IFFSIZE_UNKNOWN)) IFF_Error(error);
						WriteChunkBytes(iffh,(APTR)node->ln_Name,strlen(node->ln_Name));
						if(error=PopChunk(iffh)) IFF_Error(error);
						node=NextNode(node);
					}
					if(error=PopChunk(iffh)) IFF_Error(error);
				}
skip:;
				CloseIFF(iffh);
			} else IFF_Error(error);
			Close(iffh->iff_Stream);
		}
		FreeIFF(iffh);
	}
	UnLockWindows();
}

void LoadPrefs()
{
	struct IFFHandle *iffh;
	struct ContextNode *cn;
	LONG error=0,len;
	struct Node *node;
	struct TNode *tnode;
	TEXT buf[108];
	STRPTR membuf;

	NewList((struct List *)&Trashcan.Trashcans);
	NewList((struct List *)&Trashcan.Filters);
	SetFlag(Prefs.Flags,PREFS_APPICON);
	Prefs.FreeDisk=1000;
	Prefs.Overwrite=PREFS_OVERWRITE_NEVER;
	Prefs.ViewBy=PREFS_VIEWBY_NAME;
	Prefs.MainLeft=0;
	Prefs.MainTop=0;
	Prefs.MainWidth=320;
	Prefs.MainHeight=200;
	Prefs.DropLeft=0;
	Prefs.DropTop=0;
	Prefs.IconLeft=0;
	Prefs.IconTop=0;
	if(iffh=AllocIFF())
	{
		if(iffh->iff_Stream=Open(PrefsFrom,MODE_OLDFILE))
		{
			InitIFFasDOS(iffh);
			unless(error=OpenIFF(iffh,IFFF_READ))
			{
				if(error=StopChunk(iffh,ID_PREF,ID_TRSH)) IFF_Error(error);
				if(error=StopChunk(iffh,ID_PREF,ID_VOLS)) IFF_Error(error);
				if(error=StopChunk(iffh,ID_PREF,ID_FILT)) IFF_Error(error);
				while(1)
				{
					error=ParseIFF(iffh,IFFPARSE_SCAN);
					if(error==IFFERR_EOC) continue;       /* enter next context */
					else if(error) break;
					if(cn=CurrentChunk(iffh))
					{
						if(cn->cn_Type==ID_PREF)
						{
							switch(cn->cn_ID)
							{
								case ID_TRSH:
									ReadChunkBytes(iffh,(APTR)&Prefs,sizeof(struct Prefs));
									break;
								case ID_VOLS:
									len=ReadChunkBytes(iffh,(APTR)buf,30);
									buf[len]='\0';
									if(tnode=(struct TNode *)AllocVec(sizeof(struct TNode),MEMF_ANY))
									{
										if(membuf=(STRPTR)AllocVec(strlen(buf)+1,MEMF_ANY))
										{
											tnode->ln_Name=membuf;
											tnode->ln_Lock=tnode->ln_InitNR=FALSE;
											strcpy(membuf,buf);
											AddTail((struct List *)&Trashcan.Trashcans,(struct Node *)tnode);
										}
										else FreeVec(tnode);
									}
									break;
								case ID_FILT:
									len=ReadChunkBytes(iffh,(APTR)buf,108);
									buf[len]='\0';
									if(node=(struct Node *)AllocVec(sizeof(struct Node),MEMF_ANY))
									{
										if(membuf=(STRPTR)AllocVec(strlen(buf)+1,MEMF_ANY))
										{
											node->ln_Name=membuf;
											strcpy(membuf,buf);
											AddTail((struct List *)&Trashcan.Filters,node);
										}
										else FreeVec(node);
									}
									break;
							}
						}
					}
				}
skip:;
				CloseIFF(iffh);
			} else IFF_Error(error);
			Close(iffh->iff_Stream);
		}
		FreeIFF(iffh);
	}
}

void SetTitles()
{
	struct DeletedFile *df;
	int fcount=0,dcount=0,ssize=0,tsize=0,fsel=0,dsel=0;
	ULONG uid;

	df=(struct DeletedFile *)Trashcan.Files.mlh_Head;
	WhileNode(df)
	{
		if(df->Type==TYPE_DIR)
		{
			dcount++;
			if(df->Selected) dsel++;
		}
		if(df->Type==TYPE_FILE)
		{
			fcount++;
			tsize+=df->Size;
			if(df->Selected)
			{
				fsel++;
				ssize+=df->Size;
			}
		}
		df=(struct DeletedFile *)NextNode(df);
	}
	if(MainWnd)
	{
		if(MultiUser)
		{
			if(uid=muGetTaskOwner(NULL))
			{
				muUI->uid=(UWORD)(uid>>16);
				muGetUserInfo(muUI,muKeyType_uid);
				sprintf(WindowTitle,GetTrashmanString(MSG_WINDOW_TITLE),dsel,dcount,fsel,fcount,ssize,tsize,muUI->UserID);
			}
		}
		else
			sprintf(WindowTitle,GetTrashmanString(MSG_WINDOW_TITLE),dsel,dcount,fsel,fcount,ssize,tsize,"");
		SetWindowTitles(MainWnd,WindowTitle,GetTrashmanString(MSG_SCREEN_TITLE));
	}
}

APTR MainWndLock=NULL;
APTR PrefsWndLock=NULL;

void LockWindows()
{
	if(MainWnd)
	{
		if(MainWndLock) UnLockWindows();
		SetWindowPointer(MainWnd,WA_BusyPointer,TRUE,TAG_DONE);
		MainWndLock=rtLockWindow(MainWnd);
	}
	if(PrefsWnd)
	{
		if(PrefsWndLock) UnLockWindows();
		SetWindowPointer(PrefsWnd,WA_BusyPointer,TRUE,TAG_DONE);
		PrefsWndLock=rtLockWindow(PrefsWnd);
	}
}

void UnLockWindows()
{
	if(MainWnd)
	{
		if(MainWndLock) rtUnlockWindow(MainWnd,MainWndLock);
		SetWindowPointer(MainWnd,TAG_DONE);
		MainWndLock=NULL;
	}
	if(PrefsWnd)
	{
		if(PrefsWndLock) rtUnlockWindow(PrefsWnd,PrefsWndLock);
		SetWindowPointer(PrefsWnd,TAG_DONE);
		PrefsWndLock=NULL;
	}
}

int OpenInfoWindow(STRPTR title,STRPTR progresstitle,ULONG max)
{
	ULONG InfoWidth=320,InfoHeight=50,InfoLeft=100,InfoTop=100;
	UWORD wleft = InfoLeft, wtop = InfoTop, ww, wh;

	ComputeFont( InfoWidth, InfoHeight );

	ww = ComputeX( InfoWidth );
	wh = ComputeY( InfoHeight );

	if (( wleft + ww + OffX + Scr->WBorRight ) > Scr->Width ) wleft = Scr->Width - ww;
	if (( wtop + wh + OffY + Scr->WBorBottom ) > Scr->Height ) wtop = Scr->Height - wh;

	unless(ProgressGad=NewObject(ProgressClass,NULL,
						GA_Left,OffX + ComputeX( 20 ),
						GA_Top,OffY + ComputeY( 30 ),
						GA_Width,ComputeX( 280 ),
						GA_Height,ComputeY( 12 ),
						PROG_Placement,PLACETEXT_IN,
						PROG_Border,TRUE,
						PROG_MaxVal,max,
						PROG_CurVal,0,
						PROG_ConvertToPercents,TRUE,
						PROG_Format,progresstitle,
						PROG_TextFont,DRI->dri_Font,
						TAG_DONE)) return 1;
	unless(InfoWnd=OpenWindowTags(NULL,
						WA_Left,wleft,
						WA_Top,wtop,
						WA_Width,ww + OffX + Scr->WBorRight,
						WA_Height,wh + OffY + Scr->WBorBottom,
						WA_Flags,WFLG_DRAGBAR|WFLG_DEPTHGADGET|WFLG_ACTIVATE|WFLG_RMBTRAP,
						WA_PubScreen,	Scr,
						WA_Title,title,
						WA_ScreenTitle,GetTrashmanString(MSG_SCREEN_TITLE),
						WA_AutoAdjust, TRUE,
						TAG_DONE)) return 1;

	RefreshGadgets((struct Gadget *)ProgressGad,InfoWnd,NULL);
	return 0;
}

void CloseInfoWindow()
{
	if(InfoWnd) CloseWindow(InfoWnd);
	InfoWnd=NULL;
	if(ProgressGad) DisposeObject(ProgressGad);
	ProgressGad=NULL;
}

void InfoPrint(STRPTR path,ULONG cur)
{
	if(InfoWnd)
	{
		SetAPen(InfoWnd->RPort,DRI->dri_Pens[BACKGROUNDPEN]);
		RectFill(InfoWnd->RPort,InfoWnd->BorderLeft,InfoWnd->BorderTop,InfoWnd->Width-InfoWnd->BorderRight-1,InfoWnd->BorderTop+ComputeY(29));
		SetAPen(InfoWnd->RPort,DRI->dri_Pens[TEXTPEN]);
		Move(InfoWnd->RPort,OffX+ComputeX(20),OffY+ComputeY(10));
		Text(InfoWnd->RPort,path,FilePart(path)-path);
		Move(InfoWnd->RPort,OffX+ComputeX(20),OffY+ComputeY(20));
		Text(InfoWnd->RPort,FilePart(path),strlen(FilePart(path)));
		SetGadgetAttrs((struct Gadget *)ProgressGad,InfoWnd,NULL,PROG_CurVal,cur,TAG_DONE);
//		Delay(50);
	}
}

void SavePrefsAs()
{
	TEXT name[108];
	TEXT buff[108];

	name[0]='\0';
	buff[0]='\0';
	LockWindows();
	if(rtFileRequest(SaveReq,name,GetTrashmanString(MSG_SAVE_TRASHMAN_PREFS),
			RT_Window,MainWnd,
			RT_ReqPos,REQPOS_CENTERSCR,
			RT_Underscore,DEF_UNDERSCORE,
			RTFI_Flags,FREQF_SAVE,
			TAG_DONE))
	{
		strcpy(buff,SaveReq->Dir);
		AddPart(buff,name,108);
		SavePrefs(buff);
	}
	UnLockWindows();
}

void LoadPrefsAs()
{
	TEXT name[108];
	TEXT buff[108];

	name[0]='\0';
	buff[0]='\0';
	LockWindows();
	if(rtFileRequest(SaveReq,name,GetTrashmanString(MSG_LOAD_TRASHMAN_PREFS),
			RT_Window,MainWnd,
			RT_ReqPos,REQPOS_CENTERSCR,
			RT_Underscore,DEF_UNDERSCORE,
			TAG_DONE))
	{
		FreeTrashcansList();
		FreeFiltersList();
		FreeFileList();

		strcpy(buff,SaveReq->Dir);
		AddPart(buff,name,108);
		strcpy(PrefsFrom,buff);
		LoadPrefs();
		InitTrashcansList();
		MakeTrashcansList();
		DrawWindow();
		UpdateProps();
	}
	UnLockWindows();
}

#define BUFSIZE 64*1024

BOOL MoveFile(STRPTR src,STRPTR dest)
{
	APTR buffer;
	BPTR hsrc,hdest;
	LONG readed=1;

	if(hdest=Lock(dest,ACCESS_READ))
	{
		UnLock(hdest);
		LockWindows();
		if(!RequestUser(GetTrashmanString(MSG_FILE_ALREADY_EXISTS),GetTrashmanString(MSG_EXISTS_GAD),FilePart(dest)))
		{
			UnLockWindows();
			return FALSE;
		}
		UnLockWindows();
	}
	if(buffer=AllocVec(BUFSIZE,MEMF_ANY))
	{
		if(hsrc=Open(src,MODE_OLDFILE))
		{
			if(hdest=Open(dest,MODE_NEWFILE))
			{
				while((readed=Read(hsrc,buffer,BUFSIZE))>0)
					if(Write(hdest,buffer,readed)!=readed)
					{
						readed=-1;
						ShowError(GetTrashmanString(MSG_UNDELETE_ERROR));
						break;
					}
				if(readed==-1) ShowError(GetTrashmanString(MSG_UNDELETE_ERROR));
				Close(hdest);
			} else ShowError(GetTrashmanString(MSG_UNDELETE_ERROR));
			Close(hsrc);
			if(readed==0) CallDeleteFile(OldDeleteFile,src);
		} else ShowError(GetTrashmanString(MSG_UNDELETE_ERROR));
		FreeVec(buffer);
	} else RequestUser(GetTrashmanString(MSG_NOT_ENOUGH_MEMORY_TO),GetTrashmanString(MSG_OK_GAD),GetTrashmanString(MSG_MOVE_FILE));
	if(readed==0) return TRUE; else return FALSE;
}

ULONG RequestUser(STRPTR title,STRPTR gadstr, STRPTR fmt, ...)
{
	struct TagItem tags[]={RTEZ_ReqTitle,NULL,RT_Underscore,DEF_UNDERSCORE,TAG_DONE};

	tags[0].ti_Data=(ULONG) GetTrashmanString(MSG_REQTITLE);
	return rtEZRequest(title,gadstr,NULL,tags,fmt);
}

void ShowError(STRPTR title)
{
	TEXT buffer[256];

	Fault(IoErr(),title,buffer,256);
	RequestUser(buffer,GetTrashmanString(MSG_OK_GAD),NULL);
}

void HideAll()
{
	if(DropWnd) CloseDropWindow();
	if(PrefsWnd) ClosePreferences();
	if(MainWnd) CloseMainWindow();
	if(AppMenuItem) RemoveAppMenuItem(AppMenuItem); AppMenuItem=NULL;
	if(AppIcon) RemoveAppIcon(AppIcon); AppIcon=NULL;
	if(SizeImage) DisposeObject(SizeImage); SizeImage=NULL;
	if(DownImage) DisposeObject(DownImage); DownImage=NULL;
	if(UpImage) DisposeObject(UpImage); UpImage=NULL;
	if(RightImage) DisposeObject(RightImage); RightImage=NULL;
	if(LeftImage) DisposeObject(LeftImage); LeftImage=NULL;
	if(DRI) FreeScreenDrawInfo(Scr, DRI); DRI=NULL;
	if(Scr) CloseDownScreen();
	SetupBits();
}

void ShowAll()
{
	SetupScreen();
	DRI=GetScreenDrawInfo(Scr);
	LeftImage=NewImageObject(LEFTIMAGE);
	RightImage=NewImageObject(RIGHTIMAGE);
	UpImage=NewImageObject(UPIMAGE);
	DownImage=NewImageObject(DOWNIMAGE);
	SizeImage=NewImageObject(SIZEIMAGE);
//	GetFlag(Prefs.Flags,PREFS_HIDECLOSESALL)
//	{
		GetFlag(Prefs.Flags,PREFS_APPWINDOW) OpenDropWindow();
		GetFlag(Prefs.Flags,PREFS_APPICON) if(!AppIcon) AppIcon=AddAppIcon(APP_MAINICON,NULL,GetTrashmanString(MSG_ICON_TITLE),AppMsgPort,NULL,AppDO,TAG_DONE);
		GetFlag(Prefs.Flags,PREFS_APPMENU) if(!AppMenuItem) AppMenuItem=AddAppMenuItem(APP_EMPTYTRASH,NULL,GetTrashmanString(MSG_TOOLS_MENU),AppMsgPort,NULL,TAG_DONE);
//	}
	SetupBits();
}

BOOL CheckForPresent(STRPTR name)
{
	BPTR lock;
	struct FileInfoBlock *fib;

	if(fib=AllocDosObjectTags(DOS_FIB,TAG_DONE))
	{
		if(lock=Lock(name,ACCESS_READ))
		{
			if(Examine(lock,fib))
			{
				if(ExNext(lock,fib))
				{
					UnLock(lock);
					FreeDosObject(DOS_FIB,fib);
					return TRUE;
				}
			}
			UnLock(lock);
		}
		FreeDosObject(DOS_FIB,fib);
	}
	return FALSE;
}

void FreeSpace()
{
	TEXT name[108],ble[1024];
	struct TNode *node;
	struct InfoData *devinfo;
	ULONG uid;
	
	if(devinfo=(struct InfoData *)AllocVec(sizeof(struct InfoData),MEMF_PUBLIC))
	{
		if(MultiUser)
		{
			if(uid=muGetTaskOwner(NULL))
			{
				muUI->uid=(UWORD)(uid>>16);
				muGetUserInfo(muUI,muKeyType_uid);
			}
		}
		node=(struct TNode *)Trashcan.Trashcans.mlh_Head;
		WhileNode(node)
		{
			Info(node->ln_Lock,devinfo);
			if((devinfo->id_NumBlocks-devinfo->id_NumBlocksUsed)*devinfo->id_BytesPerBlock<Prefs.FreeDisk*1024)
			{
				if(MultiUser) sprintf(name,"%sTrashcan/%s",node->ln_Name,muUI->UserID);
				else sprintf(name,"%sTrashcan",node->ln_Name);
				if(CheckForPresent(name))
				{
					sprintf(ble,GetTrashmanString(MSG_DISK_FULL),node->ln_Name,Prefs.FreeDisk,VERSION,REVISION);
					if(RequestUser(ble,GetTrashmanString(MSG_YESNO_GAD),NULL))
						DeleteDirectory(name,NULL,TRUE);
				}
			}
			node=(struct TNode *)NextNode(node);
		}
		FreeVec((APTR)devinfo);
	}
}

/************************************************************************************/

UBYTE *GetKeyStr(APTR x)
{
	UBYTE *str;

	str=GetTrashmanString(x);
	if(str[0]=='\0') return NULL; else return str;
}

void FixLocale()
{
	PREFS_OVERWRITE1Labels[0]=GetTrashmanString(MSG_OVERWRITE_NEVER_GAD);
	PREFS_OVERWRITE1Labels[1]=GetTrashmanString(MSG_OVERWRITE_ALWAYS_GAD);
	PREFS_OVERWRITE1Labels[2]=GetTrashmanString(MSG_OVERWRITE_ASK_GAD);
	PREFS_OVERWRITE1Labels[3]=GetTrashmanString(MSG_OVERWRITE_SAME_LOC_GAD);
	PrefsIText[0].IText=GetTrashmanString(MSG_MISCELAENOUS);
	PrefsIText[1].IText=GetTrashmanString(MSG_VOLUMES);
	PrefsIText[2].IText=GetTrashmanString(MSG_FORCE_DELETE);
	PrefsIText[3].IText=GetTrashmanString(MSG_OVERWRITE);
	PrefsIText[4].IText=GetTrashmanString(MSG_MIN_FREE_DISK);
	DummyNewMenu[0].nm_Label=GetTrashmanString(MSG_PROJECT_MENU);
	DummyNewMenu[1].nm_Label=GetTrashmanString(MSG_PROJECT_OPEN_MENU)+2;
	DummyNewMenu[1].nm_CommKey=GetKeyStr(MSG_PROJECT_OPEN_MENU);
	DummyNewMenu[2].nm_Label=GetTrashmanString(MSG_PROJECT_SAVE_AS_MENU)+2;
	DummyNewMenu[2].nm_CommKey=GetKeyStr(MSG_PROJECT_SAVE_AS_MENU);
	DummyNewMenu[4].nm_Label=GetTrashmanString(MSG_PROJECT_HIDE_MENU)+2;
	DummyNewMenu[4].nm_CommKey=GetKeyStr(MSG_PROJECT_HIDE_MENU);
	DummyNewMenu[5].nm_Label=GetTrashmanString(MSG_PROJECT_QUIT_MENU)+2;
	DummyNewMenu[5].nm_CommKey=GetKeyStr(MSG_PROJECT_QUIT_MENU);
	DummyNewMenu[6].nm_Label=GetTrashmanString(MSG_WINDOW_MENU);
	DummyNewMenu[7].nm_Label=GetTrashmanString(MSG_WINDOW_CLOSE_MENU)+2;
	DummyNewMenu[7].nm_CommKey=GetKeyStr(MSG_WINDOW_CLOSE_MENU);
	DummyNewMenu[8].nm_Label=GetTrashmanString(MSG_WINDOW_UPDATE_MENU)+2;
	DummyNewMenu[8].nm_CommKey=GetKeyStr(MSG_WINDOW_UPDATE_MENU);
	DummyNewMenu[9].nm_Label=GetTrashmanString(MSG_WINDOW_SELECT_CONTENTS_MENU)+2;
	DummyNewMenu[9].nm_CommKey=GetKeyStr(MSG_WINDOW_SELECT_CONTENTS_MENU);
	DummyNewMenu[10].nm_Label=GetTrashmanString(MSG_WINDOW_SNAPSHOT_MENU);
	DummyNewMenu[11].nm_Label=GetTrashmanString(MSG_WINDOW_SNAPSHOT_DESKTOP_MENU)+2;
	DummyNewMenu[11].nm_CommKey=GetKeyStr(MSG_WINDOW_SNAPSHOT_DESKTOP_MENU);
	DummyNewMenu[12].nm_Label=GetTrashmanString(MSG_WINDOW_SNAPSHOT_MAIN_MENU)+2;
	DummyNewMenu[12].nm_CommKey=GetKeyStr(MSG_WINDOW_SNAPSHOT_MAIN_MENU);
	DummyNewMenu[13].nm_Label=GetTrashmanString(MSG_WINDOW_SNAPSHOT_TRASHCAN_MENU)+2;
	DummyNewMenu[13].nm_CommKey=GetKeyStr(MSG_WINDOW_SNAPSHOT_TRASHCAN_MENU);
	DummyNewMenu[14].nm_Label=GetTrashmanString(MSG_WINDOW_VIEW_MENU);
	DummyNewMenu[15].nm_Label=GetTrashmanString(MSG_WINDOW_VIEW_NAME_MENU)+2;
	DummyNewMenu[15].nm_CommKey=GetKeyStr(MSG_WINDOW_VIEW_NAME_MENU);
	DummyNewMenu[16].nm_Label=GetTrashmanString(MSG_WINDOW_VIEW_SIZE_MENU)+2;
	DummyNewMenu[16].nm_CommKey=GetKeyStr(MSG_WINDOW_VIEW_SIZE_MENU);
	DummyNewMenu[17].nm_Label=GetTrashmanString(MSG_WINDOW_VIEW_DATE_MENU)+2;
	DummyNewMenu[17].nm_CommKey=GetKeyStr(MSG_WINDOW_VIEW_DATE_MENU);
	DummyNewMenu[18].nm_Label=GetTrashmanString(MSG_WINDOW_VIEW_LOCATION_MENU)+2;
	DummyNewMenu[18].nm_CommKey=GetKeyStr(MSG_WINDOW_VIEW_LOCATION_MENU);
	DummyNewMenu[19].nm_Label=GetTrashmanString(MSG_FILES_MENU);
	DummyNewMenu[20].nm_Label=GetTrashmanString(MSG_FILES_REMOVE_SEL_MENU)+2;
	DummyNewMenu[20].nm_CommKey=GetKeyStr(MSG_FILES_REMOVE_SEL_MENU);
	DummyNewMenu[21].nm_Label=GetTrashmanString(MSG_FILES_EMPTY_ALL_MENU)+2;
	DummyNewMenu[21].nm_CommKey=GetKeyStr(MSG_FILES_EMPTY_ALL_MENU);
	DummyNewMenu[23].nm_Label=GetTrashmanString(MSG_FILES_UNDELETE_ORG_MENU)+2;
	DummyNewMenu[23].nm_CommKey=GetKeyStr(MSG_FILES_UNDELETE_ORG_MENU);
	DummyNewMenu[24].nm_Label=GetTrashmanString(MSG_FILES_UNDELETE_OTHER_MENU)+2;
	DummyNewMenu[24].nm_CommKey=GetKeyStr(MSG_FILES_UNDELETE_OTHER_MENU);
	DummyNewMenu[25].nm_Label=GetTrashmanString(MSG_SETTINGS_MENU);
	DummyNewMenu[26].nm_Label=GetTrashmanString(MSG_SETTINGS_EDIT_MENU)+2;
	DummyNewMenu[26].nm_CommKey=GetKeyStr(MSG_SETTINGS_EDIT_MENU);
	DummyNewMenu[28].nm_Label=GetTrashmanString(MSG_SETTINGS_ABOUT_MENU)+2;
	DummyNewMenu[28].nm_CommKey=GetKeyStr(MSG_SETTINGS_ABOUT_MENU);
	PrefsNGad[0].ng_GadgetText=GetTrashmanString(MSG_SAVE_GAD);
	PrefsNGad[1].ng_GadgetText=GetTrashmanString(MSG_USE_GAD);
	PrefsNGad[2].ng_GadgetText=GetTrashmanString(MSG_CANCEL_GAD);
	PrefsNGad[3].ng_GadgetText=GetTrashmanString(MSG_PATCH_DELETE_GAD);
	PrefsNGad[4].ng_GadgetText=GetTrashmanString(MSG_PATCH_OPEN_GAD);
	PrefsNGad[6].ng_GadgetText=GetTrashmanString(MSG_PREFS_ADD1_GAD);
	PrefsNGad[7].ng_GadgetText=GetTrashmanString(MSG_PREFS_REMOVE1_GAD);
	PrefsNGad[8].ng_GadgetText=GetTrashmanString(MSG_APP_WINDOW_GAD);
	PrefsNGad[9].ng_GadgetText=GetTrashmanString(MSG_APP_ICON_GAD);
	PrefsNGad[11].ng_GadgetText=GetTrashmanString(MSG_PREFS_ADD2_GAD);
	PrefsNGad[12].ng_GadgetText=GetTrashmanString(MSG_PREFS_REMOVE2_GAD);
	PrefsNGad[14].ng_GadgetText=GetTrashmanString(MSG_KB_ON_DISK_GAD);
	PrefsNGad[15].ng_GadgetText=GetTrashmanString(MSG_ASK_BEFORE_DEL_GAD);
	PrefsNGad[16].ng_GadgetText=GetTrashmanString(MSG_APP_MENU_GAD);
	PrefsNGad[17].ng_GadgetText=GetTrashmanString(MSG_ANIMATIONS_GAD);
	PrefsNGad[18].ng_GadgetText=GetTrashmanString(MSG_HIDE_CLOSES_ALL_GAD);
	PrefsNGad[19].ng_GadgetText=GetTrashmanString(MSG_DELETE_IF_ZEROK_GAD);
	PrefsNGad[20].ng_GadgetText=GetTrashmanString(MSG_SOUND_GAD);
	PrefsWdt=GetTrashmanString(MSG_PREFERENCES_TITLE);
}

Object *OpenSound()
{
	Object *o;

	if(o= NewDTObject ((APTR) Prefs.Sound,
		DTA_SourceType,	DTST_FILE,
		DTA_GroupID,		GID_SOUND,
		SDTA_Volume,		63,
		SDTA_Cycles,		1,
		TAG_DONE))
	{
		return o;
	}
	return FALSE;
}

void PlaySound(Object *o)
{
	struct dtTrigger dtt;

	dtt.MethodID     = DTM_TRIGGER;
	dtt.dtt_GInfo    = NULL;
	dtt.dtt_Function = STM_PLAY;
	dtt.dtt_Data     = NULL;

	if(o) DoMethodA(o, (Msg) &dtt);
}

void CloseSound(Object *o)
{
	DisposeDTObject(o);
}

void MakeUserDir()
{
	ULONG uid;
	TEXT path[108];
	struct TNode *node;
	BPTR lock;

	if(MultiUser)
	{
		if(uid=muGetTaskOwner(NULL))
		{
			muUI->uid=(UWORD)(uid>>16);
			if(muGetUserInfo(muUI,muKeyType_uid))
			{
				node=(struct TNode *)Trashcan.Trashcans.mlh_Head;
				WhileNode(node)
				{
					sprintf(path,"%sTrashcan/%s",node->ln_Name,muUI->UserID);
					if(lock=Lock(path,ACCESS_READ))
						UnLock(lock);
					else
					{
						if(CreateDir(path))
							muSetProtection(path,FIBF_READ|FIBF_WRITE);
					}
					node=(struct TNode *)NextNode(node);
				}
			}
		}
	}
}

ULONG FindMaxPen()
{
	ULONG MaxPen=1,a;

	for(a=0;a<NUMDRIPENS;a++) if(DRI->dri_Pens[a]>MaxPen) MaxPen=DRI->dri_Pens[a];
	return MaxPen;
}
