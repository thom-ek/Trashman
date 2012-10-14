/****************************************************************
   This file was created automatically by `FlexCat V1.1'
   Do NOT edit by hand!
****************************************************************/

#include <clib/locale_protos.h>

#ifdef AZTEC_C
#include <pragmas/locale_lib.h>
#endif /*   AZTEC_C		    */

#if defined(__SASC)  ||  defined(_DCC)  ||  defined(__MAXON__)
#include <pragmas/locale_pragmas.h>
#endif	/*  __SASC  ||	_DCC  ||  __MAXON__	*/

#ifdef __GNUC__
#include <inline/locale.h>
#endif	/*  __GNUC__	*/




static LONG Trashman_Version = 0;
static const STRPTR Trashman_BuiltInLanguage = (STRPTR) "english";

struct FC_Type
{   LONG   ID;
    STRPTR Str;
};


const struct FC_Type _MSG_BAD_SYSTEM_VERSION = { 0, "You need an Amiga\nwith OS 3.0" };
const struct FC_Type _MSG_ICON_TITLE = { 1, "Trash" };
const struct FC_Type _MSG_TOOLS_MENU = { 2, "Empty Trash" };
const struct FC_Type _MSG_REQTITLE = { 3, "Trashman" };
const struct FC_Type _MSG_EMPTYALL = { 4, "Do you really want to\nempty all trashcans?" };
const struct FC_Type _MSG_YESNO_GAD = { 5, "_Yes|_No" };
const struct FC_Type _MSG_REMOVESELECTED = { 6, "Do you really want to remove\nselected files?" };
const struct FC_Type _MSG_ADD_VOLUMES = { 7, "Select a disk with trashcan" };
const struct FC_Type _MSG_ADD_GAD = { 8, "_Add" };
const struct FC_Type _MSG_FORCE_DELETE_TITLE = { 9, "Enter file pattern to force delete" };
const struct FC_Type _MSG_DIR = { 10, "(dir)" };
const struct FC_Type _MSG_SCREEN_TITLE = { 11, "Trashman by Tomasz Muszynski" };
const struct FC_Type _MSG_MOVING_FILES = { 12, "Moving to Trashcan..." };
const struct FC_Type _MSG_FILE_IS_LINK = { 13, "File:\n%s\nis a %s\nand can't be moved to trashcan" };
const struct FC_Type _MSG_DELETE_LINK_GAD = { 14, "_Delete link|_Skip" };
const struct FC_Type _MSG_SOFT_LINK = { 15, "soft link" };
const struct FC_Type _MSG_HARD_LINK_DIR = { 16, "hard link to dir" };
const struct FC_Type _MSG_HARD_LINK_FILE = { 17, "hard link to file" };
const struct FC_Type _MSG_FILE_ALREADY_EXISTS_IN_TRASHCAN = { 18, "File:\n%s\nalready exists in trashcan" };
const struct FC_Type _MSG_EXISTS_TRASHCAN_GAD = { 19, "_Replace|_Rename|_Delete|_Skip" };
const struct FC_Type _MSG_SELECT_DESTINATION = { 20, "Select destination..." };
const struct FC_Type _MSG_UNDELETE_GAD = { 21, "_Undelete" };
const struct FC_Type _MSG_READING_DIRECTORY = { 22, "Reading Directory..." };
const struct FC_Type _MSG_ABOUT = { 23, "Trashman v%d.%d\nby\nTomasz Muszynski\n\nThis program is\nFREEWARE" };
const struct FC_Type _MSG_ABOUT_OK_GAD = { 24, "_Ok" };
const struct FC_Type _MSG_WINDOW_TITLE = { 25, "Trashcan, D:[%d/%d] F:[%d/%d] B:[%d/%d] %s" };
const struct FC_Type _MSG_PREFERENCES_TITLE = { 26, "Trashman Preferences" };
const struct FC_Type _MSG_OVERWRITE_NEVER_GAD = { 27, "Never" };
const struct FC_Type _MSG_OVERWRITE_ALWAYS_GAD = { 28, "Always" };
const struct FC_Type _MSG_OVERWRITE_ASK_GAD = { 29, "Ask" };
const struct FC_Type _MSG_OVERWRITE_SAME_LOC_GAD = { 30, "Same Location" };
const struct FC_Type _MSG_MISCELAENOUS = { 31, "Miscelaenous" };
const struct FC_Type _MSG_VOLUMES = { 32, "Volumes" };
const struct FC_Type _MSG_FORCE_DELETE = { 33, "Force Delete" };
const struct FC_Type _MSG_OVERWRITE = { 34, "Overwrite" };
const struct FC_Type _MSG_MIN_FREE_DISK = { 35, "Min.free on disk" };
const struct FC_Type _MSG_KB_ON_DISK_GAD = { 36, "KB on disk" };
const struct FC_Type _MSG_PROJECT_MENU = { 37, "Project" };
const struct FC_Type _MSG_PROJECT_OPEN_MENU = { 38, "O\0Open..." };
const struct FC_Type _MSG_PROJECT_SAVE_AS_MENU = { 39, "A\0Save As..." };
const struct FC_Type _MSG_PROJECT_HIDE_MENU = { 40, "H\0Hide" };
const struct FC_Type _MSG_PROJECT_QUIT_MENU = { 41, "Q\0Quit" };
const struct FC_Type _MSG_WINDOW_MENU = { 42, "Window" };
const struct FC_Type _MSG_WINDOW_CLOSE_MENU = { 43, "K\0Close" };
const struct FC_Type _MSG_WINDOW_UPDATE_MENU = { 44, "\0\0Update" };
const struct FC_Type _MSG_WINDOW_SELECT_CONTENTS_MENU = { 45, "\0\0Select Contents" };
const struct FC_Type _MSG_WINDOW_SNAPSHOT_MENU = { 46, "Snapshot" };
const struct FC_Type _MSG_WINDOW_SNAPSHOT_DESKTOP_MENU = { 47, "\0\0Desktop Icon" };
const struct FC_Type _MSG_WINDOW_SNAPSHOT_MAIN_MENU = { 48, "\0\0Main Window" };
const struct FC_Type _MSG_WINDOW_SNAPSHOT_TRASHCAN_MENU = { 49, "\0\0Trashcan Window" };
const struct FC_Type _MSG_WINDOW_VIEW_MENU = { 50, "View by" };
const struct FC_Type _MSG_WINDOW_VIEW_NAME_MENU = { 51, "\0\0Name" };
const struct FC_Type _MSG_WINDOW_VIEW_SIZE_MENU = { 52, "\0\0Size" };
const struct FC_Type _MSG_WINDOW_VIEW_DATE_MENU = { 53, "\0\0Date" };
const struct FC_Type _MSG_WINDOW_VIEW_LOCATION_MENU = { 54, "\0\0Location" };
const struct FC_Type _MSG_FILES_MENU = { 55, "Files" };
const struct FC_Type _MSG_FILES_REMOVE_SEL_MENU = { 56, "S\0Remove Selected" };
const struct FC_Type _MSG_FILES_EMPTY_ALL_MENU = { 57, "E\0Empty All Trashcans" };
const struct FC_Type _MSG_FILES_UNDELETE_ORG_MENU = { 58, "U\0Undelete to Original" };
const struct FC_Type _MSG_FILES_UNDELETE_OTHER_MENU = { 59, "T\0Undelete to Other..." };
const struct FC_Type _MSG_SETTINGS_MENU = { 60, "Settings" };
const struct FC_Type _MSG_SETTINGS_EDIT_MENU = { 61, "P\0Edit..." };
const struct FC_Type _MSG_SETTINGS_ABOUT_MENU = { 62, "\0\0About..." };
const struct FC_Type _MSG_SAVE_GAD = { 63, "Save" };
const struct FC_Type _MSG_USE_GAD = { 64, "Use" };
const struct FC_Type _MSG_CANCEL_GAD = { 65, "Cancel" };
const struct FC_Type _MSG_PATCH_DELETE_GAD = { 66, "Patch Delete()" };
const struct FC_Type _MSG_PATCH_OPEN_GAD = { 67, "Patch Open()" };
const struct FC_Type _MSG_PREFS_ADD1_GAD = { 68, "Add" };
const struct FC_Type _MSG_PREFS_REMOVE1_GAD = { 69, "Remove" };
const struct FC_Type _MSG_PREFS_ADD2_GAD = { 70, "Add" };
const struct FC_Type _MSG_PREFS_REMOVE2_GAD = { 71, "Remove" };
const struct FC_Type _MSG_APP_WINDOW_GAD = { 72, "App Window" };
const struct FC_Type _MSG_APP_ICON_GAD = { 73, "App Icon" };
const struct FC_Type _MSG_APP_MENU_GAD = { 74, "App Menu" };
const struct FC_Type _MSG_ASK_BEFORE_DEL_GAD = { 75, "Ask before Del." };
const struct FC_Type _MSG_ANIMATIONS_GAD = { 76, "Animations" };
const struct FC_Type _MSG_HIDE_CLOSES_ALL_GAD = { 77, "Hide closes all" };
const struct FC_Type _MSG_DELETE_IF_ZEROK_GAD = { 78, "Delete if size 0" };
const struct FC_Type _MSG_REMOVE_SELECTED = { 79, "Removing Selected..." };
const struct FC_Type _MSG_REMOVE_ALL = { 80, "Emptying Trashcan..." };
const struct FC_Type _MSG_UNDELETE_SELECTED = { 81, "Undeleting files..." };
const struct FC_Type _MSG_DELETED_PROGRESS = { 82, "Deleted: %ld%%" };
const struct FC_Type _MSG_UNDELETED_PROGRESS = { 83, "Undeleted: %ld%%" };
const struct FC_Type _MSG_OK_GAD = { 84, "_Ok" };
const struct FC_Type _MSG_SAVE_TRASHMAN_PREFS = { 85, "Save Trashman Preferences" };
const struct FC_Type _MSG_LOAD_TRASHMAN_PREFS = { 86, "Load Trashman Preferences" };
const struct FC_Type _MSG_FILE_ALREADY_EXISTS = { 87, "File:\n%s\nalready exists" };
const struct FC_Type _MSG_EXISTS_GAD = { 88, "_Replace|_Skip" };
const struct FC_Type _MSG_UNDELETE_ERROR = { 89, "Can't undelete file because\n" };
const struct FC_Type _MSG_NOT_ENOUGH_MEMORY_TO = { 90, "Not enough memory to\n%s." };
const struct FC_Type _MSG_MOVE_FILE = { 91, "move file" };
const struct FC_Type _MSG_COMMODITY_TITLE = { 92, "Trashman v%d.%d by Tomasz Muszynski" };
const struct FC_Type _MSG_COMMODITY_DESCRIPTION = { 93, "Trashcan Manager" };
const struct FC_Type _MSG_DELETE_ERROR = { 94, "Can't delete file because\n" };
const struct FC_Type _MSG_ERROR_CANT_OPEN_REQTOOLS = { 95, "Can't open reqtools.library" };
const struct FC_Type _MSG_ERROR_CANT_OPEN_LOCALE = { 96, "Can't open locale" };
const struct FC_Type _MSG_ERROR_CANT_CREATE_PORT = { 97, "Can't create message port" };
const struct FC_Type _MSG_ERROR_CANT_CREATE_HOTKEY = { 98, "Can't create hotkey" };
const struct FC_Type _MSG_ERROR_CANT_OPEN_FONT = { 99, "Can't open disk font" };
const struct FC_Type _MSG_ERROR_CANT_OPEN_PROGRESS = { 100, "Can't open progress.gadget" };
const struct FC_Type _MSG_ERROR_CANT_SETUP_SCREEN = { 101, "Can't setup screen" };
const struct FC_Type _MSG_ERROR_CANT_GET_DRI = { 102, "Can't get draw info" };
const struct FC_Type _MSG_ERROR_CANT_ALLOC_REQ = { 103, "Can't allocate requester" };
const struct FC_Type _MSG_ERROR_CANT_OPEN_ICON = { 104, "Can't open trashcan icon" };
const struct FC_Type _MSG_ERROR_CANT_ALLOC_DOSOBJ = { 105, "Can't allocate DOS object" };
const struct FC_Type _MSG_ERROR_CANT_CREATE_APPWINDOW = { 106, "Can't create AppWindow" };
const struct FC_Type _MSG_ERROR_CANT_CREATE_APPICON = { 107, "Can't create AppIcon" };
const struct FC_Type _MSG_ERROR_CANT_CREATE_APPMENU = { 108, "Can't create AppMenu" };
const struct FC_Type _MSG_ERROR_CANT_CREATE_COMMODITY = { 109, "Can't create commodity" };
const struct FC_Type _MSG_ERROR_CANT_CREATE_OBJECT = { 110, "Can't create image object" };
const struct FC_Type _MSG_DISK_FULL = { 111, "Volume %s has less than %ldKB of free space.\nDo you want to empty trashcan on this volume?" };
const struct FC_Type _MSG_ERROR_CANT_OPEN_MULTIUSER = { 112, "Can't open multiuser.library" };
const struct FC_Type _MSG_ERROR_CANT_ALLOC_USERINFO = { 113, "Can't allocate UserInfo" };
const struct FC_Type _MSG_SOUND_GAD = { 114, "Sound" };
const struct FC_Type _MSG_SELECT_SOUND = { 115, "Select Empty Trash Sound..." };


static struct Catalog *Trashman_Catalog = NULL;

void OpenTrashmanCatalog(struct Locale *loc, STRPTR language)
{ LONG tag, tagarg;
  extern struct Library *LocaleBase;
  extern void CloseTrashmanCatalog(void);

  CloseTrashmanCatalog(); /* Not needed if the programmer pairs OpenTrashmanCatalog
		       and CloseTrashmanCatalog right, but does no harm.  */

  if (LocaleBase != NULL  &&  Trashman_Catalog == NULL)
  { if (language == NULL)
    { tag = TAG_IGNORE;
    }
    else
    { tag = OC_Language;
      tagarg = (LONG) language;
    }
    Trashman_Catalog = OpenCatalog(loc, (STRPTR) "Trashman.catalog",
				OC_BuiltInLanguage, Trashman_BuiltInLanguage,
				tag, tagarg,
				OC_Version, Trashman_Version,
				TAG_DONE);
  }
}

void CloseTrashmanCatalog(void)
{ if (LocaleBase != NULL)
  { CloseCatalog(Trashman_Catalog);
  }
  Trashman_Catalog = NULL;
}

STRPTR GetTrashmanString(APTR fcstr)
{ STRPTR defaultstr;
  LONG strnum;

  strnum = ((struct FC_Type *) fcstr)->ID;
  defaultstr = ((struct FC_Type *) fcstr)->Str;

  return(Trashman_Catalog ? GetCatalogStr(Trashman_Catalog, strnum, defaultstr) :
		      defaultstr);
}
