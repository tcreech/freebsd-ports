--- src/bucoord/bc_status.c.orig	2021-01-14 21:08:41 UTC
+++ src/bucoord/bc_status.c
@@ -69,32 +69,6 @@ nextItem(statusP linkPtr)
     return ((statusP) ptr);
 }
 
-#ifdef notdef
-static statusP
-nextItem(linkPtr)
-     statusP linkPtr;
-{
-    dlqlinkP ptr;
-
-    ptr = (dlqlinkP) linkPtr;
-
-    /* if last known item has terminated, reset ptr */
-    if (ptr == 0) {
-	ptr = &statusHead;
-	if (dlqEmpty(ptr))
-	    return (0);
-    }
-
-    ptr = ptr->dlq_next;
-
-    /* if we're back at the head again */
-    if (ptr == &statusHead) {
-	ptr = ptr->dlq_next;
-    }
-    return ((statusP) ptr);
-}
-#endif /* notdef */
-
 char *cmdLine;
 
 void *
