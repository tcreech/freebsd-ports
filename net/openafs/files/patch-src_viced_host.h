--- src/viced/host.h.orig	2021-01-14 21:08:41 UTC
+++ src/viced/host.h
@@ -254,7 +254,7 @@ extern int h_RestoreState(void);
 #define H_ENUMERATE_BAIL(flags)        ((flags)|0x80000000)
 #define H_ENUMERATE_ISSET_BAIL(flags)  ((flags)&0x80000000)
 
-struct host *(hosttableptrs[h_MAXHOSTTABLES]);	/* Used by h_itoh */
+extern struct host *(hosttableptrs[h_MAXHOSTTABLES]);	/* Used by h_itoh */
 #define h_htoi(host) ((host)->index)	/* index isn't zeroed, no need to lock */
 #define h_itoh(hostindex) (hosttableptrs[(hostindex)>>h_HTSHIFT]+((hostindex)&(h_HTSPERBLOCK-1)))
 
