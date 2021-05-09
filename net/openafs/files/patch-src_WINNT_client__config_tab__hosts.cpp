--- src/WINNT/client_config/tab_hosts.cpp.orig	2021-01-14 21:08:41 UTC
+++ src/WINNT/client_config/tab_hosts.cpp
@@ -321,11 +321,6 @@ void HostsTab_FillList (HWND hDlg)
       TCHAR szText[ MAX_PATH ];
       lstrcpy (szText, Info.szCell);
 
-#if 0 // Add this if you like a more verbose Cell Hosts tab
-      if (Info.szComment)
-         wsprintf (&szText[ lstrlen(szText) ], TEXT(" (%s)"), Info.szComment);
-#endif
-
       FASTLISTADDITEM ai;
       memset (&ai, 0x00, sizeof(ai));
       ai.iFirstImage = IMAGE_NOIMAGE;
