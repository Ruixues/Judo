llvm-libs := /usr/lib/libLLVMSupport.a \
/usr/lib/libLLVMCore.a /usr/lib/libLLVMIRReader.a \
/usr/lib/libLLVMOrcJIT.a /usr/lib/libLLVMX86CodeGen.a \
/usr/lib/libLLVMX86AsmParser.a /usr/lib/libLLVMX86Desc.a \
/usr/lib/libLLVMX86Disassembler.a /usr/lib/libLLVMX86Info.a \
/usr/lib/libLLVMExecutionEngine.a /usr/lib/libLLVMJITLink.a \
/usr/lib/libLLVMOrcError.a /usr/lib/libLLVMPasses.a \
/usr/lib/libLLVMCoroutines.a /usr/lib/libLLVMipo.a\
/usr/lib/libLLVMIRReader.a /usr/lib/libLLVMAsmParser.a \
/usr/lib/libLLVMInstrumentation.a /usr/lib/libLLVMVectorize.a \
/usr/lib/libLLVMFrontendOpenMP.a /usr/lib/libLLVMLinker.a \
/usr/lib/libLLVMRuntimeDyld.a /usr/lib/libLLVMAsmPrinter.a \
/usr/lib/libLLVMDebugInfoDWARF.a /usr/lib/libLLVMCFGuard.a \
/usr/lib/libLLVMGlobalISel.a /usr/lib/libLLVMSelectionDAG.a \
/usr/lib/libLLVMCodeGen.a /usr/lib/libLLVMTarget.a \
/usr/lib/libLLVMBitWriter.a /usr/lib/libLLVMScalarOpts.a \
/usr/lib/libLLVMAggressiveInstCombine.a /usr/lib/libLLVMInstCombine.a \
/usr/lib/libLLVMTransformUtils.a /usr/lib/libLLVMAnalysis.a \
/usr/lib/libLLVMObject.a /usr/lib/libLLVMBitReader.a /usr/lib/libLLVMTextAPI.a \
/usr/lib/libLLVMProfileData.a /usr/lib/libLLVMCore.a /usr/lib/libLLVMRemarks.a \
/usr/lib/libLLVMBitstreamReader.a /usr/lib/libLLVMMCParser.a /usr/lib/libLLVMMCDisassembler.a \
/usr/lib/libLLVMMC.a /usr/lib/libLLVMBinaryFormat.a /usr/lib/libLLVMDebugInfoCodeView.a \
/usr/lib/libLLVMDebugInfoMSF.a /usr/lib/libLLVMSupport.a -lz -lrt -ldl -ltinfo -lpthread -lm \
/usr/lib/libLLVMDemangle.a

llvm-include := /usr/include/llvm/