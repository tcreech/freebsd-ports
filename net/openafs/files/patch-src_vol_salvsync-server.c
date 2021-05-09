--- src/vol/salvsync-server.c.orig	2021-01-14 21:08:41 UTC
+++ src/vol/salvsync-server.c
@@ -232,21 +232,6 @@ AddNodeToHash(struct SalvageQueueNode * node)
     SalvageHashTable[idx].len++;
 }
 
-#if 0
-static void
-DeleteNodeFromHash(struct SalvageQueueNode * node)
-{
-    int idx = VSHASH(node->command.sop.volume);
-
-    if (queue_IsNotOnQueue(&node->hash_chain)) {
-	return;
-    }
-
-    queue_Remove(&node->hash_chain);
-    SalvageHashTable[idx].len--;
-}
-#endif
-
 void
 SALVSYNC_salvInit(void)
 {
@@ -1084,25 +1069,6 @@ DeleteFromPendingQueue(struct SalvageQueueNode * node)
 	CV_BROADCAST(&pendingQueue.queue_change_cv);
     }
 }
-
-#if 0
-static struct SalvageQueueNode *
-LookupPendingCommand(SALVSYNC_command_hdr * qry)
-{
-    struct SalvageQueueNode * np, * nnp;
-
-    for (queue_Scan(&pendingQueue, np, nnp, SalvageQueueNode)) {
-	if ((np->command.sop.volume == qry->volume) &&
-	    !strncmp(np->command.sop.partName, qry->partName,
-		     sizeof(qry->partName)))
-	    break;
-    }
-
-    if (queue_IsEnd(&pendingQueue, np))
-	np = NULL;
-    return np;
-}
-#endif
 
 static struct SalvageQueueNode *
 LookupPendingCommandByPid(int pid)
