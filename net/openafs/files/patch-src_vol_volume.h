--- src/vol/volume.h.orig	2021-01-14 21:08:41 UTC
+++ src/vol/volume.h
@@ -29,10 +29,7 @@ typedef bit32 FileOffset;	/* Offset in this file */
 #include "daemon_com.h"
 #include "fssync.h"
 
-#if 0
-/** turn this on if you suspect a volume package locking bug */
-#define VOL_LOCK_DEBUG 1
-#endif
+/** turn on VOL_LOCK_DEBUG if you suspect a volume package locking bug */
 
 #ifdef VOL_LOCK_DEBUG
 #define VOL_LOCK_ASSERT_HELD \
@@ -413,11 +410,7 @@ typedef struct VolumeDiskData {
 				 * this volume; it is bogus (left over from an aborted  volume move,
 				 * for example).  Note:  if this flag is on, then inService should
 				 * be OFF--only the salvager checks this flag */
-#ifdef ALPHA_DUX40_ENV
-#define DONT_SALVAGE	0xE6
-#else				/* ALPHA_DUX40_ENV */
 #define DONT_SALVAGE	0xE5
-#endif				/* ALPHA_DUX40_ENV */
     byte dontSalvage;		/* If this is on, then don't bother salvaging this volume */
     byte reserveb3;
 
