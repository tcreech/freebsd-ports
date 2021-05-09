--- src/vlserver/vldb_check.c.orig	2021-01-14 21:08:41 UTC
+++ src/vlserver/vldb_check.c
@@ -1465,6 +1465,14 @@ WorkerBee(struct cmd_syndesc *as, void *arock)
 		log_error(VLDB_CHECK_ERROR,"address %u (offset 0x%0x): Record not in a BK chain (type 0x%0x)\n",
 		       record[i].addr, OFFSET(record[i].addr), record[i].type);
 	    }
+	    if ((vlentry.LockTimestamp == 0 && (vlentry.flags & VLOP_ALLOPERS) != 0) ||
+	       (vlentry.LockTimestamp != 0 && (vlentry.flags & VLOP_ALLOPERS) == 0)) {
+		log_error(VLDB_CHECK_ERROR,
+		    "address %u (offset 0x%0x): Lock inconsistency in volume '%s'; timestamp %d, lock flags 0x%0x\n",
+		    record[i].addr, OFFSET(record[i].addr), vlentry.name,
+		    vlentry.LockTimestamp, (vlentry.flags & VLOP_ALLOPERS));
+	    }
+
 	    if (fix) {
 		afs_uint32 oldhash, newhash;
 		char oldNameBuffer[10 + VL_MAXNAMELEN];
@@ -1564,6 +1572,21 @@ WorkerBee(struct cmd_syndesc *as, void *arock)
 		    vlentry.nextIdHash[j] = header.VolidHash[j][hash];
 		    header.VolidHash[j][hash] = record[i].addr;
 		}
+
+		/*
+		 * Fix lock timestamp and flag inconsistencies.
+		 */
+		if (vlentry.LockTimestamp == 0 && (vlentry.flags & VLOP_ALLOPERS) != 0) {
+		    quiet_println("FIX: Record %u: Inconsistent lock in volume %s; setting timestamp.\n",
+				  record[i].addr, vlentry.name);
+		    vlentry.LockTimestamp = time(NULL);
+		}
+		if (vlentry.LockTimestamp != 0 && (vlentry.flags & VLOP_ALLOPERS) == 0) {
+		    quiet_println("FIX: Record %u: Inconsistent lock in volume %s; setting misc lock flag.\n",
+				  record[i].addr, vlentry.name);
+		    vlentry.flags |= VLOP_DELETE; /* Shown by vos as "delete/misc". */
+		}
+
 		writeentry(record[i].addr, &vlentry);
 	    }
 	}
