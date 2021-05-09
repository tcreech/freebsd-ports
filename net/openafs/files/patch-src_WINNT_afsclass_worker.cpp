--- src/WINNT/afsclass/worker.cpp.orig	2021-01-14 21:08:41 UTC
+++ src/WINNT/afsclass/worker.cpp
@@ -30,21 +30,12 @@ extern "C" {
  *
  */
 
-#if 0
-static LPCTSTR cszAFSVOSDLL = TCHAR("AfsVosAdmin.dll");
-static LPCTSTR cszAFSBOSDLL = TCHAR("AfsBosAdmin.dll");
-static LPCTSTR cszAFSKASDLL = TCHAR("AfsKasAdmin.dll");
-static LPCTSTR cszAFSPTSDLL = TCHAR("AfsPtsAdmin.dll");
-static LPCTSTR cszAFSUTILDLL = TCHAR("AfsAdminUtil.dll");
-static LPCTSTR cszAFSCLIENTDLL = TCHAR("AfsClientAdmin.dll");
-#else
 static LPCTSTR cszAFSVOSDLL = __T("AfsVosAdmin.dll");
 static LPCTSTR cszAFSBOSDLL = __T("AfsBosAdmin.dll");
 static LPCTSTR cszAFSKASDLL = __T("AfsKasAdmin.dll");
 static LPCTSTR cszAFSPTSDLL = __T("AfsPtsAdmin.dll");
 static LPCTSTR cszAFSUTILDLL = __T("AfsAdminUtil.dll");
 static LPCTSTR cszAFSCLIENTDLL = __T("AfsClientAdmin.dll");
-#endif
 
 /*
  * VARIABLES __________________________________________________________________
