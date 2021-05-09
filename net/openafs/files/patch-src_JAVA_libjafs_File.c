--- src/JAVA/libjafs/File.c.orig	2021-01-14 21:08:41 UTC
+++ src/JAVA/libjafs/File.c
@@ -394,62 +394,6 @@ JNIEXPORT jlong JNICALL Java_org_openafs_jafs_File_lis
   (JNIEnv *env, jobject obj, jobject buffer)
 {
   return 0;
-#if 0
-    char dirName[FILENAME_MAX];
-    jclass arrayListClass;
-    jmethodID addID;
-    jstring entryJString;
-    usr_DIR *dirp;
-    struct usr_dirent *enp;
-    int i, dirSize;
-
-    *dirName='\0';
-    getAbsolutePath(env, &obj, dirName);
-    if(*dirName == '\0') {
-      fprintf(stderr, "File::listNative(): failed to get dirName\n");
-      setError(env, &obj, -1);
-      return 0;
-    }
-    arrayListClass = (*env)->GetObjectClass(env, buffer);
-    if(arrayListClass == NULL) {
-      fprintf(stderr, "File::listNative(): GetObjectClass failed\n");
-      setError(env, &obj, -1);
-      return 0;
-    }
-    addID = (*env) -> GetMethodID(env, arrayListClass, "add", 
-                                  "(Ljava/lang/Object;)Z");
-    if(addID == 0) {
-      fprintf(stderr, 
-            "File::listNative(): failed to get addID\n");
-      setError(env, &obj, -1);
-      return 0;
-    }
-    dirp = uafs_opendir(dirName);
-    if(dirp == NULL) {
-      fprintf(stderr, "File::listNative(): uafs_opendir(%s) failed(%s)\n",
-                       dirName, afs_error_message(errno));
-      setError(env, &obj, errno);
-      //throwAFSSecurityException( env, errno );
-      return 0;
-    }
-    while((enp = uafs_readdir(dirp)) != NULL) {
-	if(strcmp(enp->d_name, ".") == 0 || strcmp(enp->d_name, "..") == 0) {
-        continue;
-	}
-      entryJString = (*env) -> NewStringUTF(env, enp->d_name);
-      if(entryJString == NULL) {
-        fprintf(stderr, "File::listNative(): NewStringUTF failed\n");
-        setError(env, &obj, -1);
-        return 0;
-      }
-      (*env) -> CallBooleanMethod(env, buffer, addID, entryJString);
-    }
-    /*uafs_closedir(dirp);*/
-
-    setError(env, &obj, 0);
-
-    return (jlong) dirp;
-#endif
 }
 
 /**
@@ -465,19 +409,6 @@ JNIEXPORT jboolean JNICALL Java_org_openafs_jafs_File_
 {
 
   return JNI_TRUE;
-
-#if 0
-  usr_DIR *dirp = (usr_DIR *) dp;
-  int rc;
-  
-  rc = uafs_closedir(dirp);
-  if (rc < 0) {
-    setError(env, &obj, errno);
-    return JNI_FALSE;
-  }
-  else return JNI_TRUE;
-#endif
-
 }
 
 
