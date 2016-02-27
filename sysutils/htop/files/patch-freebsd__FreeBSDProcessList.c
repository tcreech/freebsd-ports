--- freebsd/FreeBSDProcessList.c.orig	2016-02-10 20:48:39 UTC
+++ freebsd/FreeBSDProcessList.c
@@ -84,6 +84,8 @@ static int MIB_kern_cp_time[2];
 static int MIB_kern_cp_times[2];
 static int kernelFScale;
 
+// XXX hack
+static unsigned long long int Global_totalMem;
 
 ProcessList* ProcessList_new(UsersTable* usersTable, Hashtable* pidWhiteList, uid_t userId) {
    FreeBSDProcessList* fpl = xCalloc(1, sizeof(FreeBSDProcessList));
@@ -212,9 +214,6 @@ static inline void FreeBSDProcessList_sc
    unsigned long     *cp_time_n; // old clicks state
    unsigned long     *cp_time_o; // current clicks state
 
-   unsigned long long total_o = 0;
-   unsigned long long total_n = 0;
-   unsigned long long total_d = 0;
    unsigned long cp_time_d[CPUSTATES];
    double        cp_time_p[CPUSTATES];
 
@@ -251,6 +250,9 @@ static inline void FreeBSDProcessList_sc
       }
 
       // diff old vs new
+      unsigned long long total_o = 0;
+      unsigned long long total_n = 0;
+      unsigned long long total_d = 0;
       for (int s = 0; s < CPUSTATES; s++) {
         cp_time_d[s] = cp_time_n[s] - cp_time_o[s];
         total_o += cp_time_o[s];
@@ -301,6 +303,8 @@ static inline void FreeBSDProcessList_sc
    //pl->totalMem *= pageSizeKb;
    sysctl(MIB_hw_physmem, 2, &(pl->totalMem), &len, NULL, 0);
    pl->totalMem /= 1024;
+   // XXX hack
+   Global_totalMem = pl->totalMem;
 
    sysctl(MIB_vm_stats_vm_v_active_count, 4, &(fpl->memActive), &len, NULL, 0);
    fpl->memActive *= pageSizeKb;
@@ -477,8 +481,9 @@ void ProcessList_goThroughEntries(Proces
       }
 
       // from FreeBSD source /src/usr.bin/top/machine.c
-      proc->m_size = kproc->ki_size / 1024;
-      proc->m_resident = kproc->ki_rssize * pageSizeKb;
+      proc->m_size = kproc->ki_size / 1024 / pageSizeKb;
+      proc->m_resident = kproc->ki_rssize;
+      proc->percent_mem = (proc->m_resident * PAGE_SIZE_KB) / (double)(Global_totalMem) * 100.0;
       proc->nlwp = kproc->ki_numthreads;
       proc->time = (kproc->ki_runtime + 5000) / 10000;
 
@@ -487,9 +492,6 @@ void ProcessList_goThroughEntries(Proces
          // system idle process should own all CPU time left regardless of CPU count
          if ( strcmp("idle", kproc->ki_comm) == 0 ) {
             isIdleProcess = true;
-         } else {
-            if (cpus > 1)
-               proc->percent_cpu = proc->percent_cpu / (double) cpus;
          }
       }
       if (isIdleProcess == false && proc->percent_cpu >= 99.8) {
