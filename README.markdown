The best trashcan manager

AmigaOS has a very good feature - Trashcan.  But it is not convenient if
you use multiple partitions.  Also you must use Workbench to manage files.
I was looking for better programs.  I found a few, but they were very
simple and only made AppIcon on Workbench and nothing more.  Have you seen
Trash on Mac?  Or even trashcan in Windows 95?  I missed them so I wrote
my own trashcan replacement.

It has following features:

- Deleted and replaced files are moved to trashcan (works with all programs, performing delete operations on files).
- Remembers original location of deleted files.
- Patches Dos DeleteFile() function for increased functionality (now it can delete non empty directory).
- You can use enhanced trashcan only on selected devices (eg. you can perform operations in old way with, for instance, floppy disks).
- Automatic trashcan emptying if there is no free space on the device.
- MultiUser support (similiar to WindowsNT).
- And many more...

Changes:
--------
97.03.31 - v1.3
- New catalogs added (Czech, Deutsch, French).
- Some bugs removed.
- Speeded up scrolling window contents (now uses blitter for it).
- Removed major bug: writing file on directory was causing to move that directory to trashcan, instead of displaying that the file already exists.
- polski.catalog: removed string bug.
- Fixed bug when choosing 'Remove Selected' menu item, when there wasn't selected files - caused 'Divide by Zero' guru.
- Fixed bug when deleting files on non trashman partitions.


Copyright
---------
This software is provided AS IS. You can use it freely.
