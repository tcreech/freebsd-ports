--- src/platform/DARWIN/growlagent/main.m.orig	2021-01-14 21:08:41 UTC
+++ src/platform/DARWIN/growlagent/main.m
@@ -118,10 +118,6 @@ BuildNotificationInfo(char *recvbuf, CFNotificationCen
 	CFDictionarySetValue(notificationInfo, GROWL_NOTIFICATION_CLICK_CONTEXT, clickContext);
 	CFDictionarySetValue(notificationInfo, GROWL_NOTIFICATION_TITLE, CFSTR("OpenAFS")/*title*/);
 	//CFRelease(title);
-#if 0
-	/* if fetching ever provides more data we could use this */
-	CFDictionarySetValue(notificationInfo, GROWL_NOTIFICATION_PROGRESS, progressNumber);
-#endif
 	if (!strncmp(recvbuf, FETCHSTR, sizeof(FETCHSTR)-1)) {
 		CFDictionarySetValue(notificationInfo, GROWL_NOTIFICATION_DESCRIPTION, CFStringCreateWithCString(kCFAllocatorDefault, recvbuf+FETCHOFFT, kCFStringEncodingUTF8));
 		priority = -1;
