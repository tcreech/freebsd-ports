--- src/afs/LINUX/osi_vfsops.c.orig	2021-01-14 21:08:41 UTC
+++ src/afs/LINUX/osi_vfsops.c
@@ -121,7 +121,9 @@ afs_fill_super(struct super_block *sb, void *data, int
     code = super_setup_bdi(sb);
     if (code)
         goto out;
+# if defined(STRUCT_BACKING_DEV_INFO_HAS_NAME)
     sb->s_bdi->name = "openafs";
+# endif
     sb->s_bdi->ra_pages = 32;
 #else
     /* used for inodes backing_dev_info field, also */
