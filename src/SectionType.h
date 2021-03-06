#ifndef BMOD_SECTION_TYPE_H
#define BMOD_SECTION_TYPE_H

enum class SectionType : int {
  Text, // Executable code (__text, .text).
  SymbolStubs, // Indirect symbol stubs.
  Symbols, // Symbol table.
  DynSymbols, // Dynamic symbol table.
  CString, // Constant C strings (__cstring).
  String, // String table constants.
  FuncStarts, // Function starts.
  CodeSig, // Code signature.
};

#endif // BMOD_SECTION_TYPE_H
