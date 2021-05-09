--- src/volser/volprocs.c.orig	2021-01-14 21:08:41 UTC
+++ src/volser/volprocs.c
@@ -3033,17 +3033,40 @@ SAFSVolConvertROtoRWvolume(struct rx_call *acid, afs_i
 #endif
             ttc = NewTrans(volumeId, partId);
             if (!ttc) {
-		return VOLSERVOLBUSY;
+		ret = VOLSERVOLBUSY;
+		goto done;
             }
+	    ret = FSYNC_VolOp(volumeId, pname, FSYNC_VOL_NEEDVOLUME, V_VOLUPD,
+			      NULL);
+	    if (ret != SYNC_OK) {
+		Log("SAFSVolConvertROtoRWvolume: Error %ld trying to check out "
+		    "vol %lu part %s.\n", afs_printable_int32_ld(ret),
+		    afs_printable_uint32_lu(volumeId), pname);
+		ret = VOLSERFAILEDOP;
+		goto done;
+	    }
 #ifdef AFS_NAMEI_ENV
 	    ret = namei_ConvertROtoRWvolume(pname, volumeId);
 #else
 	    ret = inode_ConvertROtoRWvolume(pname, volumeId);
 #endif
+	    if (ret != 0) {
+		Log("SAFSVolConvertROtoRWvolume: Error %ld trying to convert "
+		    "RO vol %lu to RW. The volume may be in an inconsistent or "
+		    "partially converted state; if the volume seems unusable, "
+		    "you can try salvaging it or restoring from another RO "
+		    "copy.\n", afs_printable_int32_ld(ret),
+		    afs_printable_uint32_lu(volumeId));
+		/*
+		 * the volume may or may not be usable at this point; let the
+		 * fileserver try to attach and decide.
+		 */
+		FSYNC_VolOp(volumeId, pname, FSYNC_VOL_ON, 0, NULL);
+	    }
 	    break;
 	}
     }
-
+  done:
     if (ttc)
         DeleteTrans(ttc, 1);
 
