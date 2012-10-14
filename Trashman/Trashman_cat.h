/****************************************************************
   This file was created automatically by `FlexCat V1.3'
   Do NOT edit by hand!
****************************************************************/

#ifndef Trashman_CAT_H
#define Trashman_CAT_H


#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif	/*  !EXEC_TYPES_H	    */
#ifndef LIBRARIES_LOCALE_H
#include <libraries/locale.h>
#endif	/*  !LIBRARIES_LOCALE_H     */


/*  Prototypes	*/
extern void OpenTrashmanCatalog(struct Locale *, STRPTR);
extern void CloseTrashmanCatalog(void);
extern STRPTR GetTrashmanString(APTR);

/*  Definitions */
extern const APTR _MSG_BAD_SYSTEM_VERSION;
#define MSG_BAD_SYSTEM_VERSION ((APTR) &_MSG_BAD_SYSTEM_VERSION)
extern const APTR _MSG_ICON_TITLE;
#define MSG_ICON_TITLE ((APTR) &_MSG_ICON_TITLE)
extern const APTR _MSG_TOOLS_MENU;
#define MSG_TOOLS_MENU ((APTR) &_MSG_TOOLS_MENU)
extern const APTR _MSG_REQTITLE;
#define MSG_REQTITLE ((APTR) &_MSG_REQTITLE)
extern const APTR _MSG_EMPTYALL;
#define MSG_EMPTYALL ((APTR) &_MSG_EMPTYALL)
extern const APTR _MSG_YESNO_GAD;
#define MSG_YESNO_GAD ((APTR) &_MSG_YESNO_GAD)
extern const APTR _MSG_REMOVESELECTED;
#define MSG_REMOVESELECTED ((APTR) &_MSG_REMOVESELECTED)
extern const APTR _MSG_ADD_VOLUMES;
#define MSG_ADD_VOLUMES ((APTR) &_MSG_ADD_VOLUMES)
extern const APTR _MSG_ADD_GAD;
#define MSG_ADD_GAD ((APTR) &_MSG_ADD_GAD)
extern const APTR _MSG_FORCE_DELETE_TITLE;
#define MSG_FORCE_DELETE_TITLE ((APTR) &_MSG_FORCE_DELETE_TITLE)
extern const APTR _MSG_DIR;
#define MSG_DIR ((APTR) &_MSG_DIR)
extern const APTR _MSG_SCREEN_TITLE;
#define MSG_SCREEN_TITLE ((APTR) &_MSG_SCREEN_TITLE)
extern const APTR _MSG_MOVING_FILES;
#define MSG_MOVING_FILES ((APTR) &_MSG_MOVING_FILES)
extern const APTR _MSG_FILE_IS_LINK;
#define MSG_FILE_IS_LINK ((APTR) &_MSG_FILE_IS_LINK)
extern const APTR _MSG_DELETE_LINK_GAD;
#define MSG_DELETE_LINK_GAD ((APTR) &_MSG_DELETE_LINK_GAD)
extern const APTR _MSG_SOFT_LINK;
#define MSG_SOFT_LINK ((APTR) &_MSG_SOFT_LINK)
extern const APTR _MSG_HARD_LINK_DIR;
#define MSG_HARD_LINK_DIR ((APTR) &_MSG_HARD_LINK_DIR)
extern const APTR _MSG_HARD_LINK_FILE;
#define MSG_HARD_LINK_FILE ((APTR) &_MSG_HARD_LINK_FILE)
extern const APTR _MSG_FILE_ALREADY_EXISTS_IN_TRASHCAN;
#define MSG_FILE_ALREADY_EXISTS_IN_TRASHCAN ((APTR) &_MSG_FILE_ALREADY_EXISTS_IN_TRASHCAN)
extern const APTR _MSG_EXISTS_TRASHCAN_GAD;
#define MSG_EXISTS_TRASHCAN_GAD ((APTR) &_MSG_EXISTS_TRASHCAN_GAD)
extern const APTR _MSG_SELECT_DESTINATION;
#define MSG_SELECT_DESTINATION ((APTR) &_MSG_SELECT_DESTINATION)
extern const APTR _MSG_UNDELETE_GAD;
#define MSG_UNDELETE_GAD ((APTR) &_MSG_UNDELETE_GAD)
extern const APTR _MSG_READING_DIRECTORY;
#define MSG_READING_DIRECTORY ((APTR) &_MSG_READING_DIRECTORY)
extern const APTR _MSG_ABOUT;
#define MSG_ABOUT ((APTR) &_MSG_ABOUT)
extern const APTR _MSG_ABOUT_OK_GAD;
#define MSG_ABOUT_OK_GAD ((APTR) &_MSG_ABOUT_OK_GAD)
extern const APTR _MSG_WINDOW_TITLE;
#define MSG_WINDOW_TITLE ((APTR) &_MSG_WINDOW_TITLE)
extern const APTR _MSG_PREFERENCES_TITLE;
#define MSG_PREFERENCES_TITLE ((APTR) &_MSG_PREFERENCES_TITLE)
extern const APTR _MSG_OVERWRITE_NEVER_GAD;
#define MSG_OVERWRITE_NEVER_GAD ((APTR) &_MSG_OVERWRITE_NEVER_GAD)
extern const APTR _MSG_OVERWRITE_ALWAYS_GAD;
#define MSG_OVERWRITE_ALWAYS_GAD ((APTR) &_MSG_OVERWRITE_ALWAYS_GAD)
extern const APTR _MSG_OVERWRITE_ASK_GAD;
#define MSG_OVERWRITE_ASK_GAD ((APTR) &_MSG_OVERWRITE_ASK_GAD)
extern const APTR _MSG_OVERWRITE_SAME_LOC_GAD;
#define MSG_OVERWRITE_SAME_LOC_GAD ((APTR) &_MSG_OVERWRITE_SAME_LOC_GAD)
extern const APTR _MSG_MISCELAENOUS;
#define MSG_MISCELAENOUS ((APTR) &_MSG_MISCELAENOUS)
extern const APTR _MSG_VOLUMES;
#define MSG_VOLUMES ((APTR) &_MSG_VOLUMES)
extern const APTR _MSG_FORCE_DELETE;
#define MSG_FORCE_DELETE ((APTR) &_MSG_FORCE_DELETE)
extern const APTR _MSG_OVERWRITE;
#define MSG_OVERWRITE ((APTR) &_MSG_OVERWRITE)
extern const APTR _MSG_MIN_FREE_DISK;
#define MSG_MIN_FREE_DISK ((APTR) &_MSG_MIN_FREE_DISK)
extern const APTR _MSG_KB_ON_DISK_GAD;
#define MSG_KB_ON_DISK_GAD ((APTR) &_MSG_KB_ON_DISK_GAD)
extern const APTR _MSG_PROJECT_MENU;
#define MSG_PROJECT_MENU ((APTR) &_MSG_PROJECT_MENU)
extern const APTR _MSG_PROJECT_OPEN_MENU;
#define MSG_PROJECT_OPEN_MENU ((APTR) &_MSG_PROJECT_OPEN_MENU)
extern const APTR _MSG_PROJECT_SAVE_AS_MENU;
#define MSG_PROJECT_SAVE_AS_MENU ((APTR) &_MSG_PROJECT_SAVE_AS_MENU)
extern const APTR _MSG_PROJECT_HIDE_MENU;
#define MSG_PROJECT_HIDE_MENU ((APTR) &_MSG_PROJECT_HIDE_MENU)
extern const APTR _MSG_PROJECT_QUIT_MENU;
#define MSG_PROJECT_QUIT_MENU ((APTR) &_MSG_PROJECT_QUIT_MENU)
extern const APTR _MSG_WINDOW_MENU;
#define MSG_WINDOW_MENU ((APTR) &_MSG_WINDOW_MENU)
extern const APTR _MSG_WINDOW_CLOSE_MENU;
#define MSG_WINDOW_CLOSE_MENU ((APTR) &_MSG_WINDOW_CLOSE_MENU)
extern const APTR _MSG_WINDOW_UPDATE_MENU;
#define MSG_WINDOW_UPDATE_MENU ((APTR) &_MSG_WINDOW_UPDATE_MENU)
extern const APTR _MSG_WINDOW_SELECT_CONTENTS_MENU;
#define MSG_WINDOW_SELECT_CONTENTS_MENU ((APTR) &_MSG_WINDOW_SELECT_CONTENTS_MENU)
extern const APTR _MSG_WINDOW_SNAPSHOT_MENU;
#define MSG_WINDOW_SNAPSHOT_MENU ((APTR) &_MSG_WINDOW_SNAPSHOT_MENU)
extern const APTR _MSG_WINDOW_SNAPSHOT_DESKTOP_MENU;
#define MSG_WINDOW_SNAPSHOT_DESKTOP_MENU ((APTR) &_MSG_WINDOW_SNAPSHOT_DESKTOP_MENU)
extern const APTR _MSG_WINDOW_SNAPSHOT_MAIN_MENU;
#define MSG_WINDOW_SNAPSHOT_MAIN_MENU ((APTR) &_MSG_WINDOW_SNAPSHOT_MAIN_MENU)
extern const APTR _MSG_WINDOW_SNAPSHOT_TRASHCAN_MENU;
#define MSG_WINDOW_SNAPSHOT_TRASHCAN_MENU ((APTR) &_MSG_WINDOW_SNAPSHOT_TRASHCAN_MENU)
extern const APTR _MSG_WINDOW_VIEW_MENU;
#define MSG_WINDOW_VIEW_MENU ((APTR) &_MSG_WINDOW_VIEW_MENU)
extern const APTR _MSG_WINDOW_VIEW_NAME_MENU;
#define MSG_WINDOW_VIEW_NAME_MENU ((APTR) &_MSG_WINDOW_VIEW_NAME_MENU)
extern const APTR _MSG_WINDOW_VIEW_SIZE_MENU;
#define MSG_WINDOW_VIEW_SIZE_MENU ((APTR) &_MSG_WINDOW_VIEW_SIZE_MENU)
extern const APTR _MSG_WINDOW_VIEW_DATE_MENU;
#define MSG_WINDOW_VIEW_DATE_MENU ((APTR) &_MSG_WINDOW_VIEW_DATE_MENU)
extern const APTR _MSG_WINDOW_VIEW_LOCATION_MENU;
#define MSG_WINDOW_VIEW_LOCATION_MENU ((APTR) &_MSG_WINDOW_VIEW_LOCATION_MENU)
extern const APTR _MSG_FILES_MENU;
#define MSG_FILES_MENU ((APTR) &_MSG_FILES_MENU)
extern const APTR _MSG_FILES_REMOVE_SEL_MENU;
#define MSG_FILES_REMOVE_SEL_MENU ((APTR) &_MSG_FILES_REMOVE_SEL_MENU)
extern const APTR _MSG_FILES_EMPTY_ALL_MENU;
#define MSG_FILES_EMPTY_ALL_MENU ((APTR) &_MSG_FILES_EMPTY_ALL_MENU)
extern const APTR _MSG_FILES_UNDELETE_ORG_MENU;
#define MSG_FILES_UNDELETE_ORG_MENU ((APTR) &_MSG_FILES_UNDELETE_ORG_MENU)
extern const APTR _MSG_FILES_UNDELETE_OTHER_MENU;
#define MSG_FILES_UNDELETE_OTHER_MENU ((APTR) &_MSG_FILES_UNDELETE_OTHER_MENU)
extern const APTR _MSG_SETTINGS_MENU;
#define MSG_SETTINGS_MENU ((APTR) &_MSG_SETTINGS_MENU)
extern const APTR _MSG_SETTINGS_EDIT_MENU;
#define MSG_SETTINGS_EDIT_MENU ((APTR) &_MSG_SETTINGS_EDIT_MENU)
extern const APTR _MSG_SETTINGS_ABOUT_MENU;
#define MSG_SETTINGS_ABOUT_MENU ((APTR) &_MSG_SETTINGS_ABOUT_MENU)
extern const APTR _MSG_SAVE_GAD;
#define MSG_SAVE_GAD ((APTR) &_MSG_SAVE_GAD)
extern const APTR _MSG_USE_GAD;
#define MSG_USE_GAD ((APTR) &_MSG_USE_GAD)
extern const APTR _MSG_CANCEL_GAD;
#define MSG_CANCEL_GAD ((APTR) &_MSG_CANCEL_GAD)
extern const APTR _MSG_PATCH_DELETE_GAD;
#define MSG_PATCH_DELETE_GAD ((APTR) &_MSG_PATCH_DELETE_GAD)
extern const APTR _MSG_PATCH_OPEN_GAD;
#define MSG_PATCH_OPEN_GAD ((APTR) &_MSG_PATCH_OPEN_GAD)
extern const APTR _MSG_PREFS_ADD1_GAD;
#define MSG_PREFS_ADD1_GAD ((APTR) &_MSG_PREFS_ADD1_GAD)
extern const APTR _MSG_PREFS_REMOVE1_GAD;
#define MSG_PREFS_REMOVE1_GAD ((APTR) &_MSG_PREFS_REMOVE1_GAD)
extern const APTR _MSG_PREFS_ADD2_GAD;
#define MSG_PREFS_ADD2_GAD ((APTR) &_MSG_PREFS_ADD2_GAD)
extern const APTR _MSG_PREFS_REMOVE2_GAD;
#define MSG_PREFS_REMOVE2_GAD ((APTR) &_MSG_PREFS_REMOVE2_GAD)
extern const APTR _MSG_APP_WINDOW_GAD;
#define MSG_APP_WINDOW_GAD ((APTR) &_MSG_APP_WINDOW_GAD)
extern const APTR _MSG_APP_ICON_GAD;
#define MSG_APP_ICON_GAD ((APTR) &_MSG_APP_ICON_GAD)
extern const APTR _MSG_APP_MENU_GAD;
#define MSG_APP_MENU_GAD ((APTR) &_MSG_APP_MENU_GAD)
extern const APTR _MSG_ASK_BEFORE_DEL_GAD;
#define MSG_ASK_BEFORE_DEL_GAD ((APTR) &_MSG_ASK_BEFORE_DEL_GAD)
extern const APTR _MSG_ANIMATIONS_GAD;
#define MSG_ANIMATIONS_GAD ((APTR) &_MSG_ANIMATIONS_GAD)
extern const APTR _MSG_HIDE_CLOSES_ALL_GAD;
#define MSG_HIDE_CLOSES_ALL_GAD ((APTR) &_MSG_HIDE_CLOSES_ALL_GAD)
extern const APTR _MSG_DELETE_IF_ZEROK_GAD;
#define MSG_DELETE_IF_ZEROK_GAD ((APTR) &_MSG_DELETE_IF_ZEROK_GAD)
extern const APTR _MSG_REMOVE_SELECTED;
#define MSG_REMOVE_SELECTED ((APTR) &_MSG_REMOVE_SELECTED)
extern const APTR _MSG_REMOVE_ALL;
#define MSG_REMOVE_ALL ((APTR) &_MSG_REMOVE_ALL)
extern const APTR _MSG_UNDELETE_SELECTED;
#define MSG_UNDELETE_SELECTED ((APTR) &_MSG_UNDELETE_SELECTED)
extern const APTR _MSG_DELETED_PROGRESS;
#define MSG_DELETED_PROGRESS ((APTR) &_MSG_DELETED_PROGRESS)
extern const APTR _MSG_UNDELETED_PROGRESS;
#define MSG_UNDELETED_PROGRESS ((APTR) &_MSG_UNDELETED_PROGRESS)
extern const APTR _MSG_OK_GAD;
#define MSG_OK_GAD ((APTR) &_MSG_OK_GAD)
extern const APTR _MSG_SAVE_TRASHMAN_PREFS;
#define MSG_SAVE_TRASHMAN_PREFS ((APTR) &_MSG_SAVE_TRASHMAN_PREFS)
extern const APTR _MSG_LOAD_TRASHMAN_PREFS;
#define MSG_LOAD_TRASHMAN_PREFS ((APTR) &_MSG_LOAD_TRASHMAN_PREFS)
extern const APTR _MSG_FILE_ALREADY_EXISTS;
#define MSG_FILE_ALREADY_EXISTS ((APTR) &_MSG_FILE_ALREADY_EXISTS)
extern const APTR _MSG_EXISTS_GAD;
#define MSG_EXISTS_GAD ((APTR) &_MSG_EXISTS_GAD)
extern const APTR _MSG_UNDELETE_ERROR;
#define MSG_UNDELETE_ERROR ((APTR) &_MSG_UNDELETE_ERROR)
extern const APTR _MSG_NOT_ENOUGH_MEMORY_TO;
#define MSG_NOT_ENOUGH_MEMORY_TO ((APTR) &_MSG_NOT_ENOUGH_MEMORY_TO)
extern const APTR _MSG_MOVE_FILE;
#define MSG_MOVE_FILE ((APTR) &_MSG_MOVE_FILE)
extern const APTR _MSG_COMMODITY_TITLE;
#define MSG_COMMODITY_TITLE ((APTR) &_MSG_COMMODITY_TITLE)
extern const APTR _MSG_COMMODITY_DESCRIPTION;
#define MSG_COMMODITY_DESCRIPTION ((APTR) &_MSG_COMMODITY_DESCRIPTION)
extern const APTR _MSG_DELETE_ERROR;
#define MSG_DELETE_ERROR ((APTR) &_MSG_DELETE_ERROR)
extern const APTR _MSG_ERROR_CANT_OPEN_REQTOOLS;
#define MSG_ERROR_CANT_OPEN_REQTOOLS ((APTR) &_MSG_ERROR_CANT_OPEN_REQTOOLS)
extern const APTR _MSG_ERROR_CANT_OPEN_LOCALE;
#define MSG_ERROR_CANT_OPEN_LOCALE ((APTR) &_MSG_ERROR_CANT_OPEN_LOCALE)
extern const APTR _MSG_ERROR_CANT_CREATE_PORT;
#define MSG_ERROR_CANT_CREATE_PORT ((APTR) &_MSG_ERROR_CANT_CREATE_PORT)
extern const APTR _MSG_ERROR_CANT_CREATE_HOTKEY;
#define MSG_ERROR_CANT_CREATE_HOTKEY ((APTR) &_MSG_ERROR_CANT_CREATE_HOTKEY)
extern const APTR _MSG_ERROR_CANT_OPEN_FONT;
#define MSG_ERROR_CANT_OPEN_FONT ((APTR) &_MSG_ERROR_CANT_OPEN_FONT)
extern const APTR _MSG_ERROR_CANT_OPEN_PROGRESS;
#define MSG_ERROR_CANT_OPEN_PROGRESS ((APTR) &_MSG_ERROR_CANT_OPEN_PROGRESS)
extern const APTR _MSG_ERROR_CANT_SETUP_SCREEN;
#define MSG_ERROR_CANT_SETUP_SCREEN ((APTR) &_MSG_ERROR_CANT_SETUP_SCREEN)
extern const APTR _MSG_ERROR_CANT_GET_DRI;
#define MSG_ERROR_CANT_GET_DRI ((APTR) &_MSG_ERROR_CANT_GET_DRI)
extern const APTR _MSG_ERROR_CANT_ALLOC_REQ;
#define MSG_ERROR_CANT_ALLOC_REQ ((APTR) &_MSG_ERROR_CANT_ALLOC_REQ)
extern const APTR _MSG_ERROR_CANT_OPEN_ICON;
#define MSG_ERROR_CANT_OPEN_ICON ((APTR) &_MSG_ERROR_CANT_OPEN_ICON)
extern const APTR _MSG_ERROR_CANT_ALLOC_DOSOBJ;
#define MSG_ERROR_CANT_ALLOC_DOSOBJ ((APTR) &_MSG_ERROR_CANT_ALLOC_DOSOBJ)
extern const APTR _MSG_ERROR_CANT_CREATE_APPWINDOW;
#define MSG_ERROR_CANT_CREATE_APPWINDOW ((APTR) &_MSG_ERROR_CANT_CREATE_APPWINDOW)
extern const APTR _MSG_ERROR_CANT_CREATE_APPICON;
#define MSG_ERROR_CANT_CREATE_APPICON ((APTR) &_MSG_ERROR_CANT_CREATE_APPICON)
extern const APTR _MSG_ERROR_CANT_CREATE_APPMENU;
#define MSG_ERROR_CANT_CREATE_APPMENU ((APTR) &_MSG_ERROR_CANT_CREATE_APPMENU)
extern const APTR _MSG_ERROR_CANT_CREATE_COMMODITY;
#define MSG_ERROR_CANT_CREATE_COMMODITY ((APTR) &_MSG_ERROR_CANT_CREATE_COMMODITY)
extern const APTR _MSG_ERROR_CANT_CREATE_OBJECT;
#define MSG_ERROR_CANT_CREATE_OBJECT ((APTR) &_MSG_ERROR_CANT_CREATE_OBJECT)
extern const APTR _MSG_DISK_FULL;
#define MSG_DISK_FULL ((APTR) &_MSG_DISK_FULL)
extern const APTR _MSG_ERROR_CANT_OPEN_MULTIUSER;
#define MSG_ERROR_CANT_OPEN_MULTIUSER ((APTR) &_MSG_ERROR_CANT_OPEN_MULTIUSER)
extern const APTR _MSG_ERROR_CANT_ALLOC_USERINFO;
#define MSG_ERROR_CANT_ALLOC_USERINFO ((APTR) &_MSG_ERROR_CANT_ALLOC_USERINFO)
extern const APTR _MSG_SOUND_GAD;
#define MSG_SOUND_GAD ((APTR) &_MSG_SOUND_GAD)
extern const APTR _MSG_SELECT_SOUND;
#define MSG_SELECT_SOUND ((APTR) &_MSG_SELECT_SOUND)

#endif /*   !Trashman_CAT_H  */