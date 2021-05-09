--- src/rx/rx_call.c.orig	2021-01-14 21:08:41 UTC
+++ src/rx/rx_call.c
@@ -70,7 +70,7 @@ rx_RecordCallStatistics(struct rx_call *call, unsigned
 
     rxi_IncrementTimeAndCount(call->conn->peer, rxInterface, currentFunc,
 			     totalFunc, &queue, &exec, call->app.bytesSent,
-			     call->app.bytesRcvd, 1);
+			     call->app.bytesRcvd, isServer);
 }
 
 /*
