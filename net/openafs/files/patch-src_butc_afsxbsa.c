--- src/butc/afsxbsa.c.orig	2021-01-14 21:08:41 UTC
+++ src/butc/afsxbsa.c
@@ -66,26 +66,6 @@ dsInt16_t (* AFSdsmRegisterFS)( dsUint32_t dsmHandle, 
 dsInt16_t (* AFSdsmBindMC)( dsUint32_t dsmHandle, dsmObjName *objNameP, dsmSendType sendType, mcBindKey *mcBindKeyP);
 dsInt16_t (* AFSdsmSendObj)( dsUint32_t dsmHandle, dsmSendType sendType, void *sendBuff, dsmObjName *objNameP, ObjAttr *objAttrPtr, DataBlk *dataBlkPtr);
 dsInt16_t (* AFSdsmChangePW)( dsUint32_t dsmHandle, char *oldPW, char *newPW);
-#if 0
-dsInt16_t (* AFSdsmCleanUp)( dsBool_t mtFlag);
-dsInt16_t (* AFSdsmDeleteAccess)( dsUint32_t dsmHandle, dsUint32_t ruleNum);
-dsInt16_t (* AFSdsmDeleteFS)( dsUint32_t dsmHandle, char *fsName, dsUint8_t repository);
-dsInt16_t (* AFSdsmEndGetDataEx)( dsmEndGetDataExIn_t *dsmEndGetDataExInP, dsmEndGetDataExOut_t *dsmEndGetDataExOutP);
-dsInt16_t (* AFSdsmEndSendObjEx)( dsmEndSendObjExIn_t *dsmEndSendObjExInP, dsmEndSendObjExOut_t *dsmEndSendObjExOutP);
-dsInt16_t (* AFSdsmEndTxnEx)( dsmEndTxnExIn_t *dsmEndTxnExInP, dsmEndTxnExOut_t *dsmEndTxnExOutP);
-dsInt16_t (* AFSdsmGroupHandler)( dsmGroupHandlerIn_t *dsmGroupHandlerInP, dsmGroupHandlerOut_t  *dsmGroupHandlerOutP);
-dsInt16_t (* AFSdsmInitEx)( dsUint32_t *dsmHandleP, dsmInitExIn_t *dsmInitExInP, dsmInitExOut_t *dsmInitExOutP);
-dsInt16_t (* AFSdsmLogEvent)( dsUint32_t dsmHandle, logInfo *lopInfoP);
-dsInt16_t (* AFSdsmQueryAccess)( dsUint32_t dsmHandle, qryRespAccessData **accessListP, dsUint16_t *numberOfRules);
-void      (* AFSdsmQueryApiVersionEx)( dsmApiVersionEx *apiVersionP);
-dsInt16_t (* AFSdsmQueryCliOptions)( optStruct *optstructP);
-dsInt16_t (* AFSdsmQuerySessOptions)( dsUint32_t dsmHandle, optStruct *optstructP);
-dsInt16_t (* AFSdsmRenameObj)( dsmRenameIn_t *dsmRenameInP, dsmRenameOut_t *dsmRenameOutP);
-dsInt16_t (* AFSdsmSetAccess)( dsUint32_t dsmHandle, dsmAccessType accessType, dsmObjName *objNameP, char *node, char *owner);
-dsInt16_t (* AFSdsmSetUp)( dsBool_t mtFlag, envSetUp *envSetUpPi);
-dsInt16_t (* AFSdsmUpdateFS)( dsUint32_t dsmHandle, char *fs, dsmFSUpd *fsUpdP, dsUint32_t fsUpdAct);
-dsInt16_t (* AFSdsmUpdateObj)( dsUint32_t dsmHandle, dsmSendType sendType, void *sendBuff, dsmObjName *objNameP, ObjAttr *objAttrPtr, dsUint32_t objUpdAct);
-#endif
 /* <<< TSM function pointers. */
 
 typedef struct s_delList {
