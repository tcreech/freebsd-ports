--- src/WINNT/afsrdr/kernel/lib/AFSInit.cpp.orig	2021-01-14 21:08:41 UTC
+++ src/WINNT/afsrdr/kernel/lib/AFSInit.cpp
@@ -122,32 +122,6 @@ DriverEntry( PDRIVER_OBJECT DriverObject,
 
 	RtlGetVersion( &AFSRtlSysVersion);
 
-#if 0
-        //
-        // By not fetching the RtlSetSaclSecurityDescriptor function
-        // pointer it disables the additional of a mandatory label
-        // to the default acl which is returned by AFSRedir for all
-        // security information queries.   The addition of the
-        // mandatory label appears to have a negative consequence
-        // for roaming profiles and redirected folders.  All links
-        // become untrusted and IE9 is unable to open a new instance
-        // to a non-default home page.
-        //
-        //
-        // Only retrieve this function for Vista and above since
-        // Mandatory Labels only exist on those operating systems.
-        //
-
-	if( AFSRtlSysVersion.dwMajorVersion >= 6)
-        {
-            RtlInitUnicodeString( &uniRoutine,
-                                  L"RtlSetSaclSecurityDescriptor");
-
-            AFSRtlSetSaclSecurityDescriptor = (PAFSRtlSetSaclSecurityDescriptor)MmGetSystemRoutineAddress( &uniRoutine);
-        }
-
-#endif
-
         RtlInitUnicodeString( &uniRoutine,
                               L"RtlSetGroupSecurityDescriptor");
 
