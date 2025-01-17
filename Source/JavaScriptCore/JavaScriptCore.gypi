{
    'variables': {
        'project_dir': ['.'],
        # These headers are part of JavaScriptCore's public API in the Apple Mac build.
        'javascriptcore_publicheader_files': [
            'API/JSBase.h',
            'API/JSContextRef.h',
            'API/JSObjectRef.h',
            'API/JSStringRef.h',
            'API/JSStringRefCF.h',
            'API/JSValueRef.h',
            'API/JavaScript.h',
            'API/JavaScriptCore.h',
            'API/WebKitAvailability.h',
        ],
        # These headers are part of JavaScriptCore's private API in the Apple Mac build.
        'javascriptcore_privateheader_files': [
            'API/APICast.h',
            'API/APIShims.h',
            'API/JSBasePrivate.h',
            'API/JSContextRefPrivate.h',
            'API/JSObjectRefPrivate.h',
            'API/JSProfilerPrivate.h',
            'API/JSRetainPtr.h',
            'API/JSWeakObjectMapRefInternal.h',
            'API/JSWeakObjectMapRefPrivate.h',
            'API/OpaqueJSString.h',
            'assembler/MacroAssemblerCodeRef.h',
            'bytecode/Opcode.h',
            'heap/CopiedAllocator.h',
            'heap/CopiedBlock.h',
            'heap/CopiedSpace.h',
            'heap/CopiedSpaceInlineMethods.h',
            'heap/ConservativeRoots.h',
            'heap/GCAssertions.h',
            'heap/Handle.h',
            'heap/HandleHeap.h',
            'heap/HeapBlock.h',
            'heap/SlotVisitor.h',
            'heap/HandleStack.h',
            'heap/HandleTypes.h',
            'heap/Heap.h',
            'heap/Local.h',
            'heap/LocalScope.h',
            'heap/Strong.h',
            'heap/StrongInlines.h',
            'heap/Weak.h',
            'config.h',
            'debugger/Debugger.h',
            'debugger/DebuggerActivation.h',
            'debugger/DebuggerCallFrame.h',
            'interpreter/CallFrame.h',
            'interpreter/Interpreter.h',
            'interpreter/Register.h',
            'interpreter/RegisterFile.h',
            'jit/ExecutableAllocator.h',
            'jit/JITCode.h',
            'jit/JITStubs.h',
            'jit/ThunkGenerators.h',
            'parser/ResultType.h',
            'parser/SourceCode.h',
            'parser/SourceProvider.h',
            'parser/SourceProviderCache.h',
            'profiler/CallIdentifier.h',
            'profiler/Profile.h',
            'profiler/ProfileNode.h',
            'profiler/Profiler.h',
            'runtime/ArgList.h',
            'runtime/ArrayPrototype.h',
            'runtime/BooleanObject.h',
            'runtime/CachedTranscendentalFunction.h',
            'runtime/CallData.h',
            'runtime/ClassInfo.h',
            'runtime/CommonIdentifiers.h',
            'runtime/Completion.h',
            'runtime/ConstructData.h',
            'runtime/DateInstance.h',
            'runtime/DateInstanceCache.h',
            'runtime/Error.h',
            'runtime/ExceptionHelpers.h',
            'runtime/FunctionConstructor.h',
            'runtime/FunctionPrototype.h',
            'runtime/GCActivityCallback.h',
            'runtime/Identifier.h',
            'runtime/InitializeThreading.h',
            'runtime/InternalFunction.h',
            'runtime/JSAPIValueWrapper.h',
            'runtime/JSArray.h',
            'runtime/JSByteArray.h',
            'runtime/JSCell.h',
            'runtime/JSDateMath.h',
            'runtime/JSFunction.h',
            'runtime/JSBoundFunction.h',
            'runtime/JSGlobalData.h',
            'runtime/JSGlobalObject.h',
            'runtime/JSGlobalThis.h',
            'runtime/JSLock.h',
            'runtime/JSObject.h',
            'runtime/JSString.h',
            'runtime/JSType.h',
            'runtime/JSTypeInfo.h',
            'runtime/JSValue.h',
            'runtime/JSValueInlineMethods.h',
            'runtime/JSVariableObject.h',
            'runtime/JSWrapperObject.h',
            'runtime/Lookup.h',
            'runtime/MathObject.h',
            'runtime/MemoryStatistics.h',
            'runtime/NumberObject.h',
            'runtime/NumberPrototype.h',
            'runtime/NumericStrings.h',
            'runtime/ObjectPrototype.h',
            'runtime/Operations.h',
            'runtime/PropertyDescriptor.h',
            'runtime/PropertyMapHashTable.h',
            'runtime/PropertyNameArray.h',
            'runtime/PropertySlot.h',
            'runtime/Protect.h',
            'runtime/PutPropertySlot.h',
            'runtime/RegExp.h',
            'runtime/RegExpKey.h',
            'runtime/RegExpCache.h',
            'runtime/RegExpObject.h',
            'runtime/ScopeChain.h',
            'runtime/SmallStrings.h',
            'runtime/StorageBarrier.h',
            'runtime/StringObject.h',
            'runtime/StringPrototype.h',
            'runtime/Structure.h',
            'runtime/StructureChain.h',
            'runtime/StructureTransitionTable.h',
            'runtime/SymbolTable.h',
            'runtime/Terminator.h',
            'runtime/TimeoutChecker.h',
            'runtime/UString.h',
            'runtime/UStringBuilder.h',
            'runtime/WeakGCMap.h',
            'runtime/WeakRandom.h',
            'runtime/WriteBarrier.h',
            'yarr/Yarr.h',
            'yarr/YarrCanonicalizeUCS2.h',
            'yarr/YarrInterpreter.h',
            'yarr/YarrPattern.h',
        ],
        'javascriptcore_files': [
            'API/APIShims.h',
            'API/JSBase.cpp',
            'API/JSCallbackConstructor.cpp',
            'API/JSCallbackConstructor.h',
            'API/JSCallbackFunction.cpp',
            'API/JSCallbackFunction.h',
            'API/JSCallbackObject.cpp',
            'API/JSCallbackObject.h',
            'API/JSCallbackObjectFunctions.h',
            'API/JSClassRef.cpp',
            'API/JSClassRef.h',
            'API/JSContextRef.cpp',
            'API/JSObjectRef.cpp',
            'API/JSProfilerPrivate.cpp',
            'API/JSStringRef.cpp',
            'API/JSStringRefBSTR.cpp',
            'API/JSStringRefBSTR.h',
            'API/JSStringRefCF.cpp',
            'API/JSValueRef.cpp',
            'API/JSWeakObjectMapRefPrivate.cpp',
            'API/OpaqueJSString.cpp',
            'AllInOneFile.cpp',
            'ForwardingHeaders/JavaScriptCore/APICast.h',
            'ForwardingHeaders/JavaScriptCore/APIShims.h',
            'ForwardingHeaders/JavaScriptCore/JSBase.h',
            'ForwardingHeaders/JavaScriptCore/JSContextRef.h',
            'ForwardingHeaders/JavaScriptCore/JSObjectRef.h',
            'ForwardingHeaders/JavaScriptCore/JSRetainPtr.h',
            'ForwardingHeaders/JavaScriptCore/JSStringRef.h',
            'ForwardingHeaders/JavaScriptCore/JSStringRefCF.h',
            'ForwardingHeaders/JavaScriptCore/JSValueRef.h',
            'ForwardingHeaders/JavaScriptCore/JavaScript.h',
            'ForwardingHeaders/JavaScriptCore/JavaScriptCore.h',
            'ForwardingHeaders/JavaScriptCore/OpaqueJSString.h',
            'ForwardingHeaders/JavaScriptCore/WebKitAvailability.h',
            'JavaScriptCorePrefix.h',
            'assembler/ARMAssembler.cpp',
            'assembler/ARMAssembler.h',
            'assembler/ARMv7Assembler.cpp',
            'assembler/ARMv7Assembler.h',
            'assembler/AbstractMacroAssembler.h',
            'assembler/AssemblerBuffer.h',
            'assembler/AssemblerBufferWithConstantPool.h',
            'assembler/CodeLocation.h',
            'assembler/LinkBuffer.h',
            'assembler/MIPSAssembler.h',
            'assembler/MacroAssembler.h',
            'assembler/MacroAssemblerARM.cpp',
            'assembler/MacroAssemblerARM.h',
            'assembler/MacroAssemblerARMv7.h',
            'assembler/MacroAssemblerMIPS.h',
            'assembler/MacroAssemblerX86.h',
            'assembler/MacroAssemblerX86Common.h',
            'assembler/MacroAssemblerX86_64.h',
            'assembler/RepatchBuffer.h',
            'assembler/X86Assembler.h',
            'bytecode/CodeBlock.cpp',
            'bytecode/CodeBlock.h',
            'bytecode/EvalCodeCache.h',
            'bytecode/Instruction.h',
            'bytecode/JumpTable.cpp',
            'bytecode/JumpTable.h',
            'bytecode/Opcode.cpp',
            'bytecode/SamplingTool.cpp',
            'bytecode/SamplingTool.h',
            'bytecode/StructureStubInfo.cpp',
            'bytecode/StructureStubInfo.h',
            'bytecompiler/BytecodeGenerator.cpp',
            'bytecompiler/BytecodeGenerator.h',
            'bytecompiler/Label.h',
            'bytecompiler/LabelScope.h',
            'bytecompiler/NodesCodegen.cpp',
            'bytecompiler/RegisterID.h',
            'heap/ConservativeRoots.cpp',
            'heap/WeakHeap.cpp',
            'heap/WeakHandleOwner.cpp',
            'heap/WeakBlock.cpp',
            'heap/HandleHeap.cpp',
            'heap/HandleStack.cpp',
            'heap/Heap.cpp',
            'heap/MachineStackMarker.cpp',
            'heap/MachineStackMarker.h',
            'heap/MarkStack.cpp',
            'heap/MarkStack.h',
            'heap/HeapRootVisitor.h',
            'heap/MarkedBlock.cpp',
            'heap/MarkedBlock.h',
            'heap/MarkedBlockSet.h',
            'heap/TinyBloomFilter.h',
            'heap/MarkedSpace.cpp',
            'heap/MarkedSpace.h',
            'debugger/Debugger.cpp',
            'debugger/DebuggerActivation.cpp',
            'debugger/DebuggerCallFrame.cpp',
            'dfg/DFGByteCodeParser.cpp',
            'dfg/DFGByteCodeParser.h',
            'dfg/DFGGenerationInfo.h',
            'dfg/DFGGraph.cpp',
            'dfg/DFGGraph.h',
            'dfg/DFGJITCompiler.cpp',
            'dfg/DFGJITCompiler.h',
            'dfg/DFGNode.h',
            'dfg/DFGOperations.cpp',
            'dfg/DFGOperations.h',
            'dfg/DFGRegisterBank.h',
            'dfg/DFGScoreBoard.h',
            'dfg/DFGSpeculativeJIT.cpp',
            'dfg/DFGSpeculativeJIT.h',
            'icu/unicode/parseerr.h',
            'icu/unicode/platform.h',
            'icu/unicode/putil.h',
            'icu/unicode/uchar.h',
            'icu/unicode/ucnv.h',
            'icu/unicode/ucnv_err.h',
            'icu/unicode/ucol.h',
            'icu/unicode/uconfig.h',
            'icu/unicode/uenum.h',
            'icu/unicode/uiter.h',
            'icu/unicode/uloc.h',
            'icu/unicode/umachine.h',
            'icu/unicode/unorm.h',
            'icu/unicode/urename.h',
            'icu/unicode/uscript.h',
            'icu/unicode/uset.h',
            'icu/unicode/ustring.h',
            'icu/unicode/utf.h',
            'icu/unicode/utf16.h',
            'icu/unicode/utf8.h',
            'icu/unicode/utf_old.h',
            'icu/unicode/utypes.h',
            'icu/unicode/uversion.h',
            'interpreter/CachedCall.h',
            'interpreter/CallFrame.cpp',
            'interpreter/CallFrameClosure.h',
            'interpreter/Interpreter.cpp',
            'interpreter/RegisterFile.cpp',
            'jit/ExecutableAllocator.cpp',
            'jit/ExecutableAllocatorFixedVMPool.cpp',
            'jit/JIT.cpp',
            'jit/JIT.h',
            'jit/JITArithmetic.cpp',
            'jit/JITArithmetic32_64.cpp',
            'jit/JITCall.cpp',
            'jit/JITCall32_64.cpp',
            'jit/JITInlineMethods.h',
            'jit/JITOpcodes.cpp',
            'jit/JITOpcodes32_64.cpp',
            'jit/JITPropertyAccess.cpp',
            'jit/JITPropertyAccess32_64.cpp',
            'jit/JITStubCall.h',
            'jit/JITStubs.cpp',
            'jit/JSInterfaceJIT.h',
            'jit/SpecializedThunkJIT.h',
            'jit/ThunkGenerators.cpp',
            'os-win32/WinMain.cpp',
            'os-win32/inttypes.h',
            'os-win32/stdbool.h',
            'os-win32/stdint.h',
            'parser/ASTBuilder.h',
            'parser/Lexer.cpp',
            'parser/Lexer.h',
            'parser/NodeConstructors.h',
            'parser/NodeInfo.h',
            'parser/Nodes.cpp',
            'parser/Nodes.h',
            'parser/Parser.cpp',
            'parser/Parser.h',
            'parser/ParserArena.cpp',
            'parser/ParserArena.h',
            'parser/SourceProviderCache.cpp',
            'parser/SourceProviderCacheItem.h',
            'parser/SyntaxChecker.h',
            'profiler/Profile.cpp',
            'profiler/ProfileGenerator.cpp',
            'profiler/ProfileGenerator.h',
            'profiler/ProfileNode.cpp',
            'profiler/Profiler.cpp',
            'runtime/ArgList.cpp',
            'runtime/Arguments.cpp',
            'runtime/Arguments.h',
            'runtime/ArrayConstructor.cpp',
            'runtime/ArrayConstructor.h',
            'runtime/ArrayPrototype.cpp',
            'runtime/BatchedTransitionOptimizer.h',
            'runtime/BooleanConstructor.cpp',
            'runtime/BooleanConstructor.h',
            'runtime/BooleanObject.cpp',
            'runtime/BooleanPrototype.cpp',
            'runtime/BooleanPrototype.h',
            'runtime/CallData.cpp',
            'runtime/CommonIdentifiers.cpp',
            'runtime/Completion.cpp',
            'runtime/ConstructData.cpp',
            'runtime/DateConstructor.cpp',
            'runtime/DateConstructor.h',
            'runtime/DateConversion.cpp',
            'runtime/DateConversion.h',
            'runtime/DateInstance.cpp',
            'runtime/DatePrototype.cpp',
            'runtime/DatePrototype.h',
            'runtime/Error.cpp',
            'runtime/ErrorConstructor.cpp',
            'runtime/ErrorConstructor.h',
            'runtime/ErrorInstance.cpp',
            'runtime/ErrorInstance.h',
            'runtime/ErrorPrototype.cpp',
            'runtime/ErrorPrototype.h',
            'runtime/ExceptionHelpers.cpp',
            'runtime/Executable.cpp',
            'runtime/Executable.h',
            'runtime/FunctionConstructor.cpp',
            'runtime/FunctionPrototype.cpp',
            'runtime/GCActivityCallback.cpp',
            'runtime/GCActivityCallbackCF.cpp',
            'runtime/GetterSetter.cpp',
            'runtime/GetterSetter.h',
            'runtime/Identifier.cpp',
            'runtime/InitializeThreading.cpp',
            'runtime/InternalFunction.cpp',
            'runtime/JSAPIValueWrapper.cpp',
            'runtime/JSActivation.cpp',
            'runtime/JSActivation.h',
            'runtime/JSArray.cpp',
            'runtime/JSByteArray.cpp',
            'runtime/JSCell.cpp',
            'runtime/JSDateMath.cpp',
            'runtime/JSFunction.cpp',
            'runtime/JSBoundFunction.cpp',
            'runtime/JSGlobalData.cpp',
            'runtime/JSGlobalObject.cpp',
            'runtime/JSGlobalObjectFunctions.cpp',
            'runtime/JSGlobalObjectFunctions.h',
            'runtime/JSGlobalThis.cpp',
            'runtime/JSLock.cpp',
            'runtime/JSNotAnObject.cpp',
            'runtime/JSNotAnObject.h',
            'runtime/JSONObject.cpp',
            'runtime/JSONObject.h',
            'runtime/JSObject.cpp',
            'runtime/JSPropertyNameIterator.cpp',
            'runtime/JSPropertyNameIterator.h',
            'runtime/JSStaticScopeObject.cpp',
            'runtime/JSStaticScopeObject.h',
            'runtime/JSString.cpp',
            'runtime/JSStringBuilder.h',
            'runtime/JSValue.cpp',
            'runtime/JSVariableObject.cpp',
            'runtime/JSWrapperObject.cpp',
            'runtime/LiteralParser.cpp',
            'runtime/LiteralParser.h',
            'runtime/Lookup.cpp',
            'runtime/MathObject.cpp',
            'runtime/MemoryStatistics.cpp',
            'runtime/NativeErrorConstructor.cpp',
            'runtime/NativeErrorConstructor.h',
            'runtime/NativeErrorPrototype.cpp',
            'runtime/NativeErrorPrototype.h',
            'runtime/NumberConstructor.cpp',
            'runtime/NumberConstructor.h',
            'runtime/NumberObject.cpp',
            'runtime/NumberPrototype.cpp',
            'runtime/ObjectConstructor.cpp',
            'runtime/ObjectConstructor.h',
            'runtime/ObjectPrototype.cpp',
            'runtime/Operations.cpp',
            'runtime/PropertyDescriptor.cpp',
            'runtime/PropertyNameArray.cpp',
            'runtime/PropertySlot.cpp',
            'runtime/RegExp.cpp',
            'runtime/RegExpCache.cpp',
            'runtime/RegExpConstructor.cpp',
            'runtime/RegExpConstructor.h',
            'runtime/RegExpCachedResult.cpp',
            'runtime/RegExpCachedResult.h',
            'runtime/RegExpMatchesArray.cpp',
            'runtime/RegExpMatchesArray.h',
            'runtime/RegExpObject.cpp',
            'runtime/RegExpPrototype.cpp',
            'runtime/RegExpPrototype.h',
            'runtime/ScopeChain.cpp',
            'runtime/ScopeChainMark.h',
            'runtime/SmallStrings.cpp',
            'runtime/StrictEvalActivation.cpp',
            'runtime/StrictEvalActivation.h',
            'runtime/StringConstructor.cpp',
            'runtime/StringConstructor.h',
            'runtime/StringObject.cpp',
            'runtime/StringPrototype.cpp',
            'runtime/StringRecursionChecker.cpp',
            'runtime/StringRecursionChecker.h',
            'runtime/Structure.cpp',
            'runtime/StructureChain.cpp',
            'runtime/TimeoutChecker.cpp',
            'runtime/Tracing.d',
            'runtime/Tracing.h',
            'runtime/UString.cpp',
            'runtime/UStringConcatenate.h',
            'yarr/YarrCanonicalizeUCS2.cpp',
            'yarr/YarrInterpreter.cpp',
            'yarr/YarrJIT.cpp',
            'yarr/YarrJIT.h',
            'yarr/YarrParser.h',
            'yarr/YarrPattern.cpp',
            'yarr/YarrSyntaxChecker.cpp',
            'yarr/YarrSyntaxChecker.h',
        ],
        'javascriptcore_derived_source_files': [
            '<(PRODUCT_DIR)/DerivedSources/JavaScriptCore/Lexer.lut.h',
            '<(PRODUCT_DIR)/DerivedSources/JavaScriptCore/RegExpJitTables.h',
            '<(PRODUCT_DIR)/DerivedSources/JavaScriptCore/TracingDtrace.h',
            '<(PRODUCT_DIR)/DerivedSources/JavaScriptCore/ArrayConstructor.lut.h',
            '<(PRODUCT_DIR)/DerivedSources/JavaScriptCore/ArrayPrototype.lut.h',
            '<(PRODUCT_DIR)/DerivedSources/JavaScriptCore/BooleanPrototype.lut.h',
            '<(PRODUCT_DIR)/DerivedSources/JavaScriptCore/DateConstructor.lut.h',
            '<(PRODUCT_DIR)/DerivedSources/JavaScriptCore/DatePrototype.lut.h',
            '<(PRODUCT_DIR)/DerivedSources/JavaScriptCore/ErrorPrototype.lut.h',
            '<(PRODUCT_DIR)/DerivedSources/JavaScriptCore/JSGlobalObject.lut.h',
            '<(PRODUCT_DIR)/DerivedSources/JavaScriptCore/JSONObject.lut.h',
            '<(PRODUCT_DIR)/DerivedSources/JavaScriptCore/MathObject.lut.h',
            '<(PRODUCT_DIR)/DerivedSources/JavaScriptCore/NumberConstructor.lut.h',
            '<(PRODUCT_DIR)/DerivedSources/JavaScriptCore/NumberPrototype.lut.h',
            '<(PRODUCT_DIR)/DerivedSources/JavaScriptCore/ObjectConstructor.lut.h',
            '<(PRODUCT_DIR)/DerivedSources/JavaScriptCore/ObjectPrototype.lut.h',
            '<(PRODUCT_DIR)/DerivedSources/JavaScriptCore/RegExpConstructor.lut.h',
            '<(PRODUCT_DIR)/DerivedSources/JavaScriptCore/RegExpObject.lut.h',
            '<(PRODUCT_DIR)/DerivedSources/JavaScriptCore/RegExpPrototype.lut.h',
            '<(PRODUCT_DIR)/DerivedSources/JavaScriptCore/StringConstructor.lut.h',
            '<(PRODUCT_DIR)/DerivedSources/JavaScriptCore/StringPrototype.lut.h',
        ],
        'minidom_files': [
            'API/tests/JSNode.c',
            'API/tests/JSNode.h',
            'API/tests/JSNodeList.c',
            'API/tests/JSNodeList.h',
            'API/tests/Node.c',
            'API/tests/Node.h',
            'API/tests/NodeList.c',
            'API/tests/NodeList.h',
            'API/tests/minidom.c',
        ],
        'minidom_support_files': [
            'API/tests/minidom.js',
        ],
        'testapi_files': [
            'API/tests/testapi.c',
        ],
        'testapi_support_files': [
            'API/tests/testapi.js',
        ],
        'jsc_files': [
            'jsc.cpp',
        ],
    }
}
