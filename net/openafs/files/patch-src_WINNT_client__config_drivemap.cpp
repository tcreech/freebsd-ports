--- src/WINNT/client_config/drivemap.cpp.orig	2021-01-14 21:08:41 UTC
+++ src/WINNT/client_config/drivemap.cpp
@@ -198,30 +198,6 @@ static void FreeStringMemory (LPTSTR pszString)
    Free (pszString);
 }
 
-#if 0
-static int lstrncmpi (LPCTSTR pszA, LPCTSTR pszB, size_t cch)
-{
-   if (!pszA || !pszB)
-      {
-      return (!pszB) - (!pszA);   // A,!B:1, !A,B:-1, !A,!B:0
-      }
-
-   for ( ; cch > 0; cch--, pszA = CharNext(pszA), pszB = CharNext(pszB))
-      {
-      TCHAR chA = toupper( *pszA );
-      TCHAR chB = toupper( *pszB );
-
-      if (!chA || !chB)
-         return (!chB) - (!chA);    // A,!B:1, !A,B:-1, !A,!B:0
-
-      if (chA != chB)
-         return (int)(chA) - (int)(chB);   // -1:A<B, 0:A==B, 1:A>B
-      }
-
-   return 0;  // no differences before told to stop comparing, so A==B
-}
-#endif
-
 /*
  * REALLOC ____________________________________________________________________
  *
