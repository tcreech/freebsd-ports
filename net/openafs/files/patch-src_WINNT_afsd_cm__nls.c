--- src/WINNT/afsd/cm_nls.c.orig	2021-01-14 21:08:41 UTC
+++ src/WINNT/afsd/cm_nls.c
@@ -1309,23 +1309,6 @@ int cm_stricmp_utf8(const char * str1, const char * st
     }
 }
 
-#if 0
-wchar_t * strupr_utf16(wchar_t * wstr, size_t cbstr)
-{
-    wchar_t wstrd[NLSMAXCCH];
-    int len;
-
-    if (!nls_init)
-        cm_InitNormalization();
-
-    len = cbstr / sizeof(wchar_t);
-    len = LCMapStringW(nls_lcid, LCMAP_UPPERCASE, wstr, len, wstrd, NLSMAXCCH);
-    StringCbCopyW(wstr, cbstr, wstrd);
-
-    return wstr;
-}
-#endif
-
 char * strupr_utf8(char * str, size_t cbstr)
 {
     wchar_t wstr[NLSMAXCCH];
