--- v8/src/wasm/baseline/ia32/liftoff-assembler-ia32-inl.h.orig	2025-03-24 20:50:14 UTC
+++ v8/src/wasm/baseline/ia32/liftoff-assembler-ia32-inl.h
@@ -578,7 +578,7 @@ void LiftoffAssembler::Load(LiftoffRegister dst, Regis
 }
 
 void LiftoffAssembler::Load(LiftoffRegister dst, Register src_addr,
-                            Register offset_reg, uint32_t offset_imm,
+                            Register offset_reg, uintptr_t offset_imm,
                             LoadType type, uint32_t* protected_load_pc,
                             bool /* is_load_mem */, bool /* i64_offset */,
                             bool needs_shift) {
@@ -658,7 +658,7 @@ void LiftoffAssembler::Store(Register dst_addr, Regist
 }
 
 void LiftoffAssembler::Store(Register dst_addr, Register offset_reg,
-                             uint32_t offset_imm, LiftoffRegister src,
+                             uintptr_t offset_imm, LiftoffRegister src,
                              StoreType type, LiftoffRegList pinned,
                              uint32_t* protected_store_pc,
                              bool /* is_store_mem */, bool /* i64_offset */) {
@@ -737,7 +737,7 @@ void LiftoffAssembler::AtomicLoad(LiftoffRegister dst,
 }
 
 void LiftoffAssembler::AtomicLoad(LiftoffRegister dst, Register src_addr,
-                                  Register offset_reg, uint32_t offset_imm,
+                                  Register offset_reg, uintptr_t offset_imm,
                                   LoadType type, LiftoffRegList /* pinned */,
                                   bool /* i64_offset */) {
   if (type.value() != LoadType::kI64Load) {
@@ -755,7 +755,7 @@ void LiftoffAssembler::AtomicStore(Register dst_addr, 
 }
 
 void LiftoffAssembler::AtomicStore(Register dst_addr, Register offset_reg,
-                                   uint32_t offset_imm, LiftoffRegister src,
+                                   uintptr_t offset_imm, LiftoffRegister src,
                                    StoreType type, LiftoffRegList pinned,
                                    bool /* i64_offset */) {
   DCHECK_LE(offset_imm, std::numeric_limits<int32_t>::max());
@@ -825,7 +825,7 @@ inline void AtomicAddOrSubOrExchange32(LiftoffAssemble
 
 inline void AtomicAddOrSubOrExchange32(LiftoffAssembler* lasm, Binop binop,
                                        Register dst_addr, Register offset_reg,
-                                       uint32_t offset_imm,
+                                       uintptr_t offset_imm,
                                        LiftoffRegister value,
                                        LiftoffRegister result, StoreType type) {
   DCHECK_EQ(value, result);
@@ -893,7 +893,7 @@ inline void AtomicBinop32(LiftoffAssembler* lasm, Bino
 }
 
 inline void AtomicBinop32(LiftoffAssembler* lasm, Binop op, Register dst_addr,
-                          Register offset_reg, uint32_t offset_imm,
+                          Register offset_reg, uintptr_t offset_imm,
                           LiftoffRegister value, LiftoffRegister result,
                           StoreType type) {
   DCHECK_EQ(value, result);
@@ -1008,7 +1008,7 @@ inline void AtomicBinop64(LiftoffAssembler* lasm, Bino
 }
 
 inline void AtomicBinop64(LiftoffAssembler* lasm, Binop op, Register dst_addr,
-                          Register offset_reg, uint32_t offset_imm,
+                          Register offset_reg, uintptr_t offset_imm,
                           LiftoffRegister value, LiftoffRegister result) {
   // We need {ebx} here, which is the root register. As the root register it
   // needs special treatment. As we use {ebx} directly in the code below, we
@@ -1104,7 +1104,7 @@ void LiftoffAssembler::AtomicAdd(Register dst_addr, Re
 }  // namespace liftoff
 
 void LiftoffAssembler::AtomicAdd(Register dst_addr, Register offset_reg,
-                                 uint32_t offset_imm, LiftoffRegister value,
+                                 uintptr_t offset_imm, LiftoffRegister value,
                                  LiftoffRegister result, StoreType type,
                                  bool /* i64_offset */) {
   if (type.value() == StoreType::kI64Store) {
@@ -1118,7 +1118,7 @@ void LiftoffAssembler::AtomicSub(Register dst_addr, Re
 }
 
 void LiftoffAssembler::AtomicSub(Register dst_addr, Register offset_reg,
-                                 uint32_t offset_imm, LiftoffRegister value,
+                                 uintptr_t offset_imm, LiftoffRegister value,
                                  LiftoffRegister result, StoreType type,
                                  bool /* i64_offset */) {
   if (type.value() == StoreType::kI64Store) {
@@ -1131,7 +1131,7 @@ void LiftoffAssembler::AtomicAnd(Register dst_addr, Re
 }
 
 void LiftoffAssembler::AtomicAnd(Register dst_addr, Register offset_reg,
-                                 uint32_t offset_imm, LiftoffRegister value,
+                                 uintptr_t offset_imm, LiftoffRegister value,
                                  LiftoffRegister result, StoreType type,
                                  bool /* i64_offset */) {
   if (type.value() == StoreType::kI64Store) {
@@ -1145,7 +1145,7 @@ void LiftoffAssembler::AtomicOr(Register dst_addr, Reg
 }
 
 void LiftoffAssembler::AtomicOr(Register dst_addr, Register offset_reg,
-                                uint32_t offset_imm, LiftoffRegister value,
+                                uintptr_t offset_imm, LiftoffRegister value,
                                 LiftoffRegister result, StoreType type,
                                 bool /* i64_offset */) {
   if (type.value() == StoreType::kI64Store) {
@@ -1159,7 +1159,7 @@ void LiftoffAssembler::AtomicXor(Register dst_addr, Re
 }
 
 void LiftoffAssembler::AtomicXor(Register dst_addr, Register offset_reg,
-                                 uint32_t offset_imm, LiftoffRegister value,
+                                 uintptr_t offset_imm, LiftoffRegister value,
                                  LiftoffRegister result, StoreType type,
                                  bool /* i64_offset */) {
   if (type.value() == StoreType::kI64Store) {
@@ -1173,7 +1173,7 @@ void LiftoffAssembler::AtomicExchange(Register dst_add
 }
 
 void LiftoffAssembler::AtomicExchange(Register dst_addr, Register offset_reg,
-                                      uint32_t offset_imm,
+                                      uintptr_t offset_imm,
                                       LiftoffRegister value,
                                       LiftoffRegister result, StoreType type,
                                       bool /* i64_offset */) {
@@ -1188,7 +1188,7 @@ void LiftoffAssembler::AtomicCompareExchange(
 }
 
 void LiftoffAssembler::AtomicCompareExchange(
-    Register dst_addr, Register offset_reg, uint32_t offset_imm,
+    Register dst_addr, Register offset_reg, uintptr_t offset_imm,
     LiftoffRegister expected, LiftoffRegister new_value, LiftoffRegister result,
     StoreType type, bool /* i64_offset */) {
   // We expect that the offset has already been added to {dst_addr}, and no
