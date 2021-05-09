--- src/platform/DARWIN/AFSPreference/Krb5Util.m.orig	2021-01-14 21:08:41 UTC
+++ src/platform/DARWIN/AFSPreference/Krb5Util.m
@@ -176,13 +176,6 @@
 			KLPrincipal klprinc = nil;
 			kstatus = KLRenewInitialTickets ( klprinc, inLoginOptions, nil, nil);
 #endif
-
-#if 0
-			/* handoff to growl agent? */
-			kstatus = KLTicketExpirationTime (nil, kerberosVersion_All, &expireStartTime);
-			expirationDate = [NSDate dateWithTimeIntervalSince1970:expireStartTime];
-			BuildNotificationInfo(@"Ticket Renewed Unitl %@", expirationDate,  callbackInfo->dcref, callbackInfo->regref, callbackInfo->icon);
-#endif
 		}
 	}
 	@catch (NSException * e) {
