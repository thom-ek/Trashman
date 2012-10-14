#include<string.h>
#include<proto/dos.h>
#include<proto/icon.h>
#include<proto/exec.h>
#include<exec/types.h>
#include<workbench/startup.h>

struct DiskObject *TTGetOurIcon(struct WBStartup *);
void TTFreeOurIcon(struct DiskObject *);
char *TTString(struct DiskObject *,char *, char *);
LONG TTInt(struct DiskObject *,char *, LONG);
int TTBool(struct DiskObject *,char *, BOOL);

/* Read tooltypes */

struct DiskObject *TTGetOurIcon(struct WBStartup *WBenchMsg)
{
	struct DiskObject *keyobj=NULL;

	if (WBenchMsg) keyobj = GetDiskObject(WBenchMsg->sm_ArgList->wa_Name);
	return keyobj;
}

void TTFreeOurIcon(struct DiskObject *keyobj)
{
	if (keyobj) FreeDiskObject(keyobj);
}

char *TTString(struct DiskObject *keyobj,char *name2, char *def)
{
	char *what;
	if (keyobj)
		if (what = FindToolType(keyobj->do_ToolTypes, name2))
			return what;
	return def;
}

LONG TTInt(struct DiskObject *keyobj,char *name2, LONG def)
{
	char *what;
	if (keyobj)
		if (what = FindToolType(keyobj->do_ToolTypes, name2))
			StrToLong(what, &def);
	return def;
}

int TTBool(struct DiskObject *keyobj,char *name2, BOOL def)
{
	char	*s;

	s = TTString(keyobj,name2, def ? "YES" : "NO");

	return	((stricmp(s, "YES") == 0) ||
		(stricmp(s, "TRUE") == 0) ||
		(stricmp(s, "ON") == 0)) ? TRUE : FALSE;
}
