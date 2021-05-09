--- src/WINNT/install/loopback/wmi.cpp.orig	2021-01-14 21:08:41 UTC
+++ src/WINNT/install/loopback/wmi.cpp
@@ -361,20 +361,6 @@ WMIEnableStatic(
 
     ReportMessage(0,"Found Adapter Instance",NULL, InstancePath,0);
 
-#if 0
-    // Use the adapter instance index to set MAXLANA in the registry.
-    {
-        DWORD dwIndex;
-        if (swscanf(InstancePath, L"Win32_NetworkAdapterConfiguration.Index=%u", &dwIndex)==1)
-        {
-            DWORD ret = 0;
-            ReportMessage(1,"Setting MAXLANA",NULL,NULL,dwIndex+1);
-            ret = AdjustMaxLana(dwIndex+1);
-            if (ret) ReportMessage(0,"AdjustMaxLana returned the error code ",NULL,NULL,ret);
-        }
-    }
-#endif
-
     MethodName = SysAllocString(L"EnableStatic");
     CLEANUP_ON_AND_SET(!MethodName, hr, E_OUTOFMEMORY);
 
@@ -955,31 +941,12 @@ BOOL UpdateHostsFile( LPCWSTR swName, LPCWSTR swIp, LP
 }
 
 #ifdef TEST
-#if 0
 int
 wmain(
     int argc,
     wchar_t* argv[]
     )
 {
-    if (argc < 3)
-    {
-        printf("usage: %S ip mask\n"
-               "  example: %S 10.0.0.1 255.0.0.0", argv[0], argv[0]);
-        return 0;
-    }
-
-    return WMIEnableStatic(FindNetworkAdapterConfigurationInstanceByGUID,
-                           L"{B4981E32-551C-4164-96B6-B8874BD2E555}",
-                           argv[1], argv[2]);
-}
-#else
-int
-wmain(
-    int argc,
-    wchar_t* argv[]
-    )
-{
     if (argc < 4)
     {
         printf("usage: %S adapter_guid ip mask\n"
@@ -991,6 +958,5 @@ wmain(
     return WMIEnableStatic(FindNetworkAdapterConfigurationInstanceByGUID,
                            argv[1], argv[2], argv[3]);
 }
-#endif
 #endif
 
