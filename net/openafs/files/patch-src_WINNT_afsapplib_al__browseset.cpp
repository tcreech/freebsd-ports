--- src/WINNT/afsapplib/al_browseset.cpp.orig	2021-01-14 21:08:41 UTC
+++ src/WINNT/afsapplib/al_browseset.cpp
@@ -301,46 +301,6 @@ DWORD WINAPI BrowseSet_Init_ThreadProc (LPARAM lp)
    // Fill in hList with the names of all filesets in the cell.
    //
    ULONG status = 0;
-#if 0
-   // TODO
-   if (OpenFTS (&status))
-      {
-      PVOID hCell;
-      if ((status = FtsOpenCell (szCellA, &hCell)) == 0)
-         {
-         fldb_short_ft_info_t *fti;
-         if ((status = FtsAllocateShortFtInfoBuffer (&fti)) == 0)
-            {
-            PVOID cookie = 0;
-            ULONG nEntries;
-
-            while ( (FtsListFilesetsFromFldb (hCell, fti, &nEntries, &cookie) == 0) && (nEntries > 0) )
-               {
-               if (!IsWindow ((HWND)(lpp->pInternal)))
-                  break;
-
-               for (ULONG ii = 0; ii < nEntries; ++ii)
-                  {
-#define DECODE_SHINFO_TYPE_FLAGS(_fl) ((_fl) >> 12)
-                  if (DECODE_SHINFO_TYPE_FLAGS(fti[ ii ].flags) & FTS_FT_SHINFO_TYPE_RW)
-                     {
-                     LPTSTR pszFileset;
-                     if ((pszFileset = CloneString (fti[ii].name)) != NULL)
-                        {
-                        PostMessage ((HWND)(lpp->pInternal), WM_FOUNDNAME, (WPARAM)pszFileset, 0);
-                        // string memory is freed by recipient of message
-                        }
-                     }
-                  }
-               }
-            FtsFreeShortFtInfoBuffer (fti);
-            }
-         FtsCloseCell (hCell);
-         }
-      CloseFTS();
-      }
-#endif
-
    if (IsWindow ((HWND)(lpp->pInternal)))
       PostMessage ((HWND)(lpp->pInternal), WM_THREADDONE, status, 0);
 
