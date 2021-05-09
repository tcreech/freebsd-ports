--- src/WINNT/afsd/cm_nls.h.orig	2021-01-14 21:08:41 UTC
+++ src/WINNT/afsd/cm_nls.h
@@ -274,18 +274,5 @@ cm_is_valid_utf16(__in_z const wchar_t * c, int cch);
 wchar_t * cm_GetRawCharsAlloc(const wchar_t * c, int len);
 #endif
 
-#if 0
-
-extern long cm_NormalizeUtf16StringToUtf8(const wchar_t * src, int cch_src,
-                                          char * adest, int cch_adest);
-
-extern char * char_next_utf8(const char * c);
-
-extern char * char_prev_utf8(const char * c);
-
-extern char * strupr_utf8(char * str, size_t cbstr);
-
-#endif
-
 #define lengthof(a) (sizeof(a)/sizeof(a[0]))
 #endif
