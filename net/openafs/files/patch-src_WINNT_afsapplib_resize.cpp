--- src/WINNT/afsapplib/resize.cpp.orig	2021-01-14 21:08:41 UTC
+++ src/WINNT/afsapplib/resize.cpp
@@ -636,18 +636,6 @@ LONG APIENTRY SplitterWndProc (HWND hWnd, UINT msg, WP
                   SetWindowLong (hWnd, GWL_USER, 0);
                   }
                break;
-
-#if 0 // Enable me to make the splitters draw in black
-         case WM_PAINT:
-               {
-               PAINTSTRUCT ps;
-               HDC hdc = BeginPaint (hWnd, &ps);
-               FillRect (hdc, &ps.rcPaint, GetStockObject(BLACK_BRUSH));
-               EndPaint (hWnd, &ps);
-               return 0;
-               }
-               break;
-#endif
          }
       }
 
