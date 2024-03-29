#include<stdio.h>
#include<Windows.h>

typedef unsigned char       BYTE;
typedef unsigned IL_OFFSET;
template <typename T, int size>
inline constexpr unsigned ArrLen(T(&)[size])
{
    return size;
}

typedef enum opcode_t
{
#define OPDEF(c,s,pop,push,args,type,l,s1,s2,ctrl) c,
#include "opcode.def"
#undef OPDEF
    CEE_COUNT,        /* number of instructions and macros pre-defined */
} OPCODE;
inline unsigned __int8 getU1LittleEndian(const BYTE* ptr)
{
    return *(__unaligned unsigned __int8*)ptr;
}


extern const signed char opcodeSizes[];
extern
const signed char       opcodeSizes[] =
{
    #define InlineNone_size           0
    #define ShortInlineVar_size       1
    #define InlineVar_size            2
    #define ShortInlineI_size         1
    #define InlineI_size              4
    #define InlineI8_size             8
    #define ShortInlineR_size         4
    #define InlineR_size              8
    #define ShortInlineBrTarget_size  1
    #define InlineBrTarget_size       4
    #define InlineMethod_size         4
    #define InlineField_size          4
    #define InlineType_size           4
    #define InlineString_size         4
    #define InlineSig_size            4
    #define InlineRVA_size            4
    #define InlineTok_size            4
    #define InlineSwitch_size         0       // for now
    #define InlinePhi_size            0       // for now
    #define InlineVarTok_size         0       // remove

    #define OPDEF(name,string,pop,push,oprType,opcType,l,s1,s2,ctrl) oprType ## _size ,
    #include "opcode.def"
    #undef OPDEF

    #undef InlineNone_size
    #undef ShortInlineVar_size
    #undef InlineVar_size
    #undef ShortInlineI_size
    #undef InlineI_size
    #undef InlineI8_size
    #undef ShortInlineR_size
    #undef InlineR_size
    #undef ShortInlineBrTarget_size
    #undef InlineBrTarget_size
    #undef InlineMethod_size
    #undef InlineField_size
    #undef InlineType_size
    #undef InlineString_size
    #undef InlineSig_size
    #undef InlineRVA_size
    #undef InlineTok_size
    #undef InlineSwitch_size
    #undef InlinePhi_size
};
typedef enum opcode_format_t
{
    InlineNone = 0,	// no inline args
    InlineVar = 1,	// local variable       (U2 (U1 if Short on))
    InlineI = 2,	// an signed integer    (I4 (I1 if Short on))
    InlineR = 3,	// a real number        (R8 (R4 if Short on))
    InlineBrTarget = 4,    // branch target        (I4 (I1 if Short on))
    InlineI8 = 5,
    InlineMethod = 6,   // method token (U4)
    InlineField = 7,   // field token  (U4)
    InlineType = 8,   // type token   (U4)
    InlineString = 9,   // string TOKEN (U4)
    InlineSig = 10,  // signature tok (U4)
    InlineRVA = 11,  // ldptr token  (U4)
    InlineTok = 12,  // a meta-data token of unknown type (U4)
    InlineSwitch = 13,  // count (U4), pcrel1 (U4) .... pcrelN (U4)
    InlinePhi = 14,  // count (U1), var1 (U2) ... varN (U2)

    // WATCH OUT we are close to the limit here, if you add
    // more enumerations you need to change ShortIline definition below

    // The extended enumeration also encodes the size in the IL stream
    ShortInline = 16,						// if this bit is set, the format is the 'short' format
    PrimaryMask = (ShortInline - 1),			// mask these off to get primary enumeration above
    ShortInlineVar = (ShortInline + InlineVar),
    ShortInlineI = (ShortInline + InlineI),
    ShortInlineR = (ShortInline + InlineR),
    ShortInlineBrTarget = (ShortInline + InlineBrTarget),
    InlineOpcode = (ShortInline + InlineNone),    // This is only used internally.  It means the 'opcode' is two byte instead of 1
} OPCODE_FORMAT;

extern const char* const opcodeNames[] = {
#define OPDEF(name, string, pop, push, oprType, opcType, l, s1, s2, ctrl) string,
OPDEF(CEE_NOP,                        "nop",              Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0x00,    NEXT)
OPDEF(CEE_BREAK,                      "break",            Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0x01,    BREAK)
OPDEF(CEE_LDARG_0,                    "ldarg.0",          Pop0,               Push1,       InlineNone,         IMacro,      1,  0xFF,    0x02,    NEXT)
OPDEF(CEE_LDARG_1,                    "ldarg.1",          Pop0,               Push1,       InlineNone,         IMacro,      1,  0xFF,    0x03,    NEXT)
OPDEF(CEE_LDARG_2,                    "ldarg.2",          Pop0,               Push1,       InlineNone,         IMacro,      1,  0xFF,    0x04,    NEXT)
OPDEF(CEE_LDARG_3,                    "ldarg.3",          Pop0,               Push1,       InlineNone,         IMacro,      1,  0xFF,    0x05,    NEXT)
OPDEF(CEE_LDLOC_0,                    "ldloc.0",          Pop0,               Push1,       InlineNone,         IMacro,      1,  0xFF,    0x06,    NEXT)
OPDEF(CEE_LDLOC_1,                    "ldloc.1",          Pop0,               Push1,       InlineNone,         IMacro,      1,  0xFF,    0x07,    NEXT)
OPDEF(CEE_LDLOC_2,                    "ldloc.2",          Pop0,               Push1,       InlineNone,         IMacro,      1,  0xFF,    0x08,    NEXT)
OPDEF(CEE_LDLOC_3,                    "ldloc.3",          Pop0,               Push1,       InlineNone,         IMacro,      1,  0xFF,    0x09,    NEXT)
OPDEF(CEE_STLOC_0,                    "stloc.0",          Pop1,               Push0,       InlineNone,         IMacro,      1,  0xFF,    0x0A,    NEXT)
OPDEF(CEE_STLOC_1,                    "stloc.1",          Pop1,               Push0,       InlineNone,         IMacro,      1,  0xFF,    0x0B,    NEXT)
OPDEF(CEE_STLOC_2,                    "stloc.2",          Pop1,               Push0,       InlineNone,         IMacro,      1,  0xFF,    0x0C,    NEXT)
OPDEF(CEE_STLOC_3,                    "stloc.3",          Pop1,               Push0,       InlineNone,         IMacro,      1,  0xFF,    0x0D,    NEXT)
OPDEF(CEE_LDARG_S,                    "ldarg.s",          Pop0,               Push1,       ShortInlineVar,     IMacro,      1,  0xFF,    0x0E,    NEXT)
OPDEF(CEE_LDARGA_S,                   "ldarga.s",         Pop0,               PushI,       ShortInlineVar,     IMacro,      1,  0xFF,    0x0F,    NEXT)
OPDEF(CEE_STARG_S,                    "starg.s",          Pop1,               Push0,       ShortInlineVar,     IMacro,      1,  0xFF,    0x10,    NEXT)
OPDEF(CEE_LDLOC_S,                    "ldloc.s",          Pop0,               Push1,       ShortInlineVar,     IMacro,      1,  0xFF,    0x11,    NEXT)
OPDEF(CEE_LDLOCA_S,                   "ldloca.s",         Pop0,               PushI,       ShortInlineVar,     IMacro,      1,  0xFF,    0x12,    NEXT)
OPDEF(CEE_STLOC_S,                    "stloc.s",          Pop1,               Push0,       ShortInlineVar,     IMacro,      1,  0xFF,    0x13,    NEXT)
OPDEF(CEE_LDNULL,                     "ldnull",           Pop0,               PushRef,     InlineNone,         IPrimitive,  1,  0xFF,    0x14,    NEXT)
OPDEF(CEE_LDC_I4_M1,                  "ldc.i4.m1",        Pop0,               PushI,       InlineNone,         IMacro,      1,  0xFF,    0x15,    NEXT)
OPDEF(CEE_LDC_I4_0,                   "ldc.i4.0",         Pop0,               PushI,       InlineNone,         IMacro,      1,  0xFF,    0x16,    NEXT)
OPDEF(CEE_LDC_I4_1,                   "ldc.i4.1",         Pop0,               PushI,       InlineNone,         IMacro,      1,  0xFF,    0x17,    NEXT)
OPDEF(CEE_LDC_I4_2,                   "ldc.i4.2",         Pop0,               PushI,       InlineNone,         IMacro,      1,  0xFF,    0x18,    NEXT)
OPDEF(CEE_LDC_I4_3,                   "ldc.i4.3",         Pop0,               PushI,       InlineNone,         IMacro,      1,  0xFF,    0x19,    NEXT)
OPDEF(CEE_LDC_I4_4,                   "ldc.i4.4",         Pop0,               PushI,       InlineNone,         IMacro,      1,  0xFF,    0x1A,    NEXT)
OPDEF(CEE_LDC_I4_5,                   "ldc.i4.5",         Pop0,               PushI,       InlineNone,         IMacro,      1,  0xFF,    0x1B,    NEXT)
OPDEF(CEE_LDC_I4_6,                   "ldc.i4.6",         Pop0,               PushI,       InlineNone,         IMacro,      1,  0xFF,    0x1C,    NEXT)
OPDEF(CEE_LDC_I4_7,                   "ldc.i4.7",         Pop0,               PushI,       InlineNone,         IMacro,      1,  0xFF,    0x1D,    NEXT)
OPDEF(CEE_LDC_I4_8,                   "ldc.i4.8",         Pop0,               PushI,       InlineNone,         IMacro,      1,  0xFF,    0x1E,    NEXT)
OPDEF(CEE_LDC_I4_S,                   "ldc.i4.s",         Pop0,               PushI,       ShortInlineI,       IMacro,      1,  0xFF,    0x1F,    NEXT)
OPDEF(CEE_LDC_I4,                     "ldc.i4",           Pop0,               PushI,       InlineI,            IPrimitive,  1,  0xFF,    0x20,    NEXT)
OPDEF(CEE_LDC_I8,                     "ldc.i8",           Pop0,               PushI8,      InlineI8,           IPrimitive,  1,  0xFF,    0x21,    NEXT)
OPDEF(CEE_LDC_R4,                     "ldc.r4",           Pop0,               PushR4,      ShortInlineR,       IPrimitive,  1,  0xFF,    0x22,    NEXT)
OPDEF(CEE_LDC_R8,                     "ldc.r8",           Pop0,               PushR8,      InlineR,            IPrimitive,  1,  0xFF,    0x23,    NEXT)
OPDEF(CEE_UNUSED49,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0x24,    NEXT)
OPDEF(CEE_DUP,                        "dup",              Pop1,               Push1 + Push1, InlineNone,         IPrimitive,  1,  0xFF,    0x25,    NEXT)
OPDEF(CEE_POP,                        "pop",              Pop1,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0x26,    NEXT)
OPDEF(CEE_JMP,                        "jmp",              Pop0,               Push0,       InlineMethod,       IPrimitive,  1,  0xFF,    0x27,    CALL)
OPDEF(CEE_CALL,                       "call",             VarPop,             VarPush,     InlineMethod,       IPrimitive,  1,  0xFF,    0x28,    CALL)
OPDEF(CEE_CALLI,                      "calli",            VarPop,             VarPush,     InlineSig,          IPrimitive,  1,  0xFF,    0x29,    CALL)
OPDEF(CEE_RET,                        "ret",              VarPop,             Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0x2A,    RETURN)
OPDEF(CEE_BR_S,                       "br.s",             Pop0,               Push0,       ShortInlineBrTarget,IMacro,      1,  0xFF,    0x2B,    BRANCH)
OPDEF(CEE_BRFALSE_S,                  "brfalse.s",        PopI,               Push0,       ShortInlineBrTarget,IMacro,      1,  0xFF,    0x2C,    COND_BRANCH)
OPDEF(CEE_BRTRUE_S,                   "brtrue.s",         PopI,               Push0,       ShortInlineBrTarget,IMacro,      1,  0xFF,    0x2D,    COND_BRANCH)
OPDEF(CEE_BEQ_S,                      "beq.s",            Pop1 + Pop1,          Push0,       ShortInlineBrTarget,IMacro,      1,  0xFF,    0x2E,    COND_BRANCH)
OPDEF(CEE_BGE_S,                      "bge.s",            Pop1 + Pop1,          Push0,       ShortInlineBrTarget,IMacro,      1,  0xFF,    0x2F,    COND_BRANCH)
OPDEF(CEE_BGT_S,                      "bgt.s",            Pop1 + Pop1,          Push0,       ShortInlineBrTarget,IMacro,      1,  0xFF,    0x30,    COND_BRANCH)
OPDEF(CEE_BLE_S,                      "ble.s",            Pop1 + Pop1,          Push0,       ShortInlineBrTarget,IMacro,      1,  0xFF,    0x31,    COND_BRANCH)
OPDEF(CEE_BLT_S,                      "blt.s",            Pop1 + Pop1,          Push0,       ShortInlineBrTarget,IMacro,      1,  0xFF,    0x32,    COND_BRANCH)
OPDEF(CEE_BNE_UN_S,                   "bne.un.s",         Pop1 + Pop1,          Push0,       ShortInlineBrTarget,IMacro,      1,  0xFF,    0x33,    COND_BRANCH)
OPDEF(CEE_BGE_UN_S,                   "bge.un.s",         Pop1 + Pop1,          Push0,       ShortInlineBrTarget,IMacro,      1,  0xFF,    0x34,    COND_BRANCH)
OPDEF(CEE_BGT_UN_S,                   "bgt.un.s",         Pop1 + Pop1,          Push0,       ShortInlineBrTarget,IMacro,      1,  0xFF,    0x35,    COND_BRANCH)
OPDEF(CEE_BLE_UN_S,                   "ble.un.s",         Pop1 + Pop1,          Push0,       ShortInlineBrTarget,IMacro,      1,  0xFF,    0x36,    COND_BRANCH)
OPDEF(CEE_BLT_UN_S,                   "blt.un.s",         Pop1 + Pop1,          Push0,       ShortInlineBrTarget,IMacro,      1,  0xFF,    0x37,    COND_BRANCH)
OPDEF(CEE_BR,                         "br",               Pop0,               Push0,       InlineBrTarget,     IPrimitive,  1,  0xFF,    0x38,    BRANCH)
OPDEF(CEE_BRFALSE,                    "brfalse",          PopI,               Push0,       InlineBrTarget,     IPrimitive,  1,  0xFF,    0x39,    COND_BRANCH)
OPDEF(CEE_BRTRUE,                     "brtrue",           PopI,               Push0,       InlineBrTarget,     IPrimitive,  1,  0xFF,    0x3A,    COND_BRANCH)
OPDEF(CEE_BEQ,                        "beq",              Pop1 + Pop1,          Push0,       InlineBrTarget,     IMacro,      1,  0xFF,    0x3B,    COND_BRANCH)
OPDEF(CEE_BGE,                        "bge",              Pop1 + Pop1,          Push0,       InlineBrTarget,     IMacro,      1,  0xFF,    0x3C,    COND_BRANCH)
OPDEF(CEE_BGT,                        "bgt",              Pop1 + Pop1,          Push0,       InlineBrTarget,     IMacro,      1,  0xFF,    0x3D,    COND_BRANCH)
OPDEF(CEE_BLE,                        "ble",              Pop1 + Pop1,          Push0,       InlineBrTarget,     IMacro,      1,  0xFF,    0x3E,    COND_BRANCH)
OPDEF(CEE_BLT,                        "blt",              Pop1 + Pop1,          Push0,       InlineBrTarget,     IMacro,      1,  0xFF,    0x3F,    COND_BRANCH)
OPDEF(CEE_BNE_UN,                     "bne.un",           Pop1 + Pop1,          Push0,       InlineBrTarget,     IMacro,      1,  0xFF,    0x40,    COND_BRANCH)
OPDEF(CEE_BGE_UN,                     "bge.un",           Pop1 + Pop1,          Push0,       InlineBrTarget,     IMacro,      1,  0xFF,    0x41,    COND_BRANCH)
OPDEF(CEE_BGT_UN,                     "bgt.un",           Pop1 + Pop1,          Push0,       InlineBrTarget,     IMacro,      1,  0xFF,    0x42,    COND_BRANCH)
OPDEF(CEE_BLE_UN,                     "ble.un",           Pop1 + Pop1,          Push0,       InlineBrTarget,     IMacro,      1,  0xFF,    0x43,    COND_BRANCH)
OPDEF(CEE_BLT_UN,                     "blt.un",           Pop1 + Pop1,          Push0,       InlineBrTarget,     IMacro,      1,  0xFF,    0x44,    COND_BRANCH)
OPDEF(CEE_SWITCH,                     "switch",           PopI,               Push0,       InlineSwitch,       IPrimitive,  1,  0xFF,    0x45,    COND_BRANCH)
OPDEF(CEE_LDIND_I1,                   "ldind.i1",         PopI,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0x46,    NEXT)
OPDEF(CEE_LDIND_U1,                   "ldind.u1",         PopI,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0x47,    NEXT)
OPDEF(CEE_LDIND_I2,                   "ldind.i2",         PopI,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0x48,    NEXT)
OPDEF(CEE_LDIND_U2,                   "ldind.u2",         PopI,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0x49,    NEXT)
OPDEF(CEE_LDIND_I4,                   "ldind.i4",         PopI,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0x4A,    NEXT)
OPDEF(CEE_LDIND_U4,                   "ldind.u4",         PopI,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0x4B,    NEXT)
OPDEF(CEE_LDIND_I8,                   "ldind.i8",         PopI,               PushI8,      InlineNone,         IPrimitive,  1,  0xFF,    0x4C,    NEXT)
OPDEF(CEE_LDIND_I,                    "ldind.i",          PopI,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0x4D,    NEXT)
OPDEF(CEE_LDIND_R4,                   "ldind.r4",         PopI,               PushR4,      InlineNone,         IPrimitive,  1,  0xFF,    0x4E,    NEXT)
OPDEF(CEE_LDIND_R8,                   "ldind.r8",         PopI,               PushR8,      InlineNone,         IPrimitive,  1,  0xFF,    0x4F,    NEXT)
OPDEF(CEE_LDIND_REF,                  "ldind.ref",        PopI,               PushRef,     InlineNone,         IPrimitive,  1,  0xFF,    0x50,    NEXT)
OPDEF(CEE_STIND_REF,                  "stind.ref",        PopI + PopI,          Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0x51,    NEXT)
OPDEF(CEE_STIND_I1,                   "stind.i1",         PopI + PopI,          Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0x52,    NEXT)
OPDEF(CEE_STIND_I2,                   "stind.i2",         PopI + PopI,          Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0x53,    NEXT)
OPDEF(CEE_STIND_I4,                   "stind.i4",         PopI + PopI,          Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0x54,    NEXT)
OPDEF(CEE_STIND_I8,                   "stind.i8",         PopI + PopI8,         Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0x55,    NEXT)
OPDEF(CEE_STIND_R4,                   "stind.r4",         PopI + PopR4,         Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0x56,    NEXT)
OPDEF(CEE_STIND_R8,                   "stind.r8",         PopI + PopR8,         Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0x57,    NEXT)
OPDEF(CEE_ADD,                        "add",              Pop1 + Pop1,          Push1,       InlineNone,         IPrimitive,  1,  0xFF,    0x58,    NEXT)
OPDEF(CEE_SUB,                        "sub",              Pop1 + Pop1,          Push1,       InlineNone,         IPrimitive,  1,  0xFF,    0x59,    NEXT)
OPDEF(CEE_MUL,                        "mul",              Pop1 + Pop1,          Push1,       InlineNone,         IPrimitive,  1,  0xFF,    0x5A,    NEXT)
OPDEF(CEE_DIV,                        "div",              Pop1 + Pop1,          Push1,       InlineNone,         IPrimitive,  1,  0xFF,    0x5B,    NEXT)
OPDEF(CEE_DIV_UN,                     "div.un",           Pop1 + Pop1,          Push1,       InlineNone,         IPrimitive,  1,  0xFF,    0x5C,    NEXT)
OPDEF(CEE_REM,                        "rem",              Pop1 + Pop1,          Push1,       InlineNone,         IPrimitive,  1,  0xFF,    0x5D,    NEXT)
OPDEF(CEE_REM_UN,                     "rem.un",           Pop1 + Pop1,          Push1,       InlineNone,         IPrimitive,  1,  0xFF,    0x5E,    NEXT)
OPDEF(CEE_AND,                        "and",              Pop1 + Pop1,          Push1,       InlineNone,         IPrimitive,  1,  0xFF,    0x5F,    NEXT)
OPDEF(CEE_OR,                         "or",               Pop1 + Pop1,          Push1,       InlineNone,         IPrimitive,  1,  0xFF,    0x60,    NEXT)
OPDEF(CEE_XOR,                        "xor",              Pop1 + Pop1,          Push1,       InlineNone,         IPrimitive,  1,  0xFF,    0x61,    NEXT)
OPDEF(CEE_SHL,                        "shl",              Pop1 + Pop1,          Push1,       InlineNone,         IPrimitive,  1,  0xFF,    0x62,    NEXT)
OPDEF(CEE_SHR,                        "shr",              Pop1 + Pop1,          Push1,       InlineNone,         IPrimitive,  1,  0xFF,    0x63,    NEXT)
OPDEF(CEE_SHR_UN,                     "shr.un",           Pop1 + Pop1,          Push1,       InlineNone,         IPrimitive,  1,  0xFF,    0x64,    NEXT)
OPDEF(CEE_NEG,                        "neg",              Pop1,               Push1,       InlineNone,         IPrimitive,  1,  0xFF,    0x65,    NEXT)
OPDEF(CEE_NOT,                        "not",              Pop1,               Push1,       InlineNone,         IPrimitive,  1,  0xFF,    0x66,    NEXT)
OPDEF(CEE_CONV_I1,                    "conv.i1",          Pop1,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0x67,    NEXT)
OPDEF(CEE_CONV_I2,                    "conv.i2",          Pop1,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0x68,    NEXT)
OPDEF(CEE_CONV_I4,                    "conv.i4",          Pop1,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0x69,    NEXT)
OPDEF(CEE_CONV_I8,                    "conv.i8",          Pop1,               PushI8,      InlineNone,         IPrimitive,  1,  0xFF,    0x6A,    NEXT)
OPDEF(CEE_CONV_R4,                    "conv.r4",          Pop1,               PushR4,      InlineNone,         IPrimitive,  1,  0xFF,    0x6B,    NEXT)
OPDEF(CEE_CONV_R8,                    "conv.r8",          Pop1,               PushR8,      InlineNone,         IPrimitive,  1,  0xFF,    0x6C,    NEXT)
OPDEF(CEE_CONV_U4,                    "conv.u4",          Pop1,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0x6D,    NEXT)
OPDEF(CEE_CONV_U8,                    "conv.u8",          Pop1,               PushI8,      InlineNone,         IPrimitive,  1,  0xFF,    0x6E,    NEXT)
OPDEF(CEE_CALLVIRT,                   "callvirt",         VarPop,             VarPush,     InlineMethod,       IObjModel,   1,  0xFF,    0x6F,    CALL)
OPDEF(CEE_CPOBJ,                      "cpobj",            PopI + PopI,          Push0,       InlineType,         IObjModel,   1,  0xFF,    0x70,    NEXT)
OPDEF(CEE_LDOBJ,                      "ldobj",            PopI,               Push1,       InlineType,         IObjModel,   1,  0xFF,    0x71,    NEXT)
OPDEF(CEE_LDSTR,                      "ldstr",            Pop0,               PushRef,     InlineString,       IObjModel,   1,  0xFF,    0x72,    NEXT)
OPDEF(CEE_NEWOBJ,                     "newobj",           VarPop,             PushRef,     InlineMethod,       IObjModel,   1,  0xFF,    0x73,    CALL)
OPDEF(CEE_CASTCLASS,                  "castclass",        PopRef,             PushRef,     InlineType,         IObjModel,   1,  0xFF,    0x74,    NEXT)
OPDEF(CEE_ISINST,                     "isinst",           PopRef,             PushI,       InlineType,         IObjModel,   1,  0xFF,    0x75,    NEXT)
OPDEF(CEE_CONV_R_UN,                  "conv.r.un",        Pop1,               PushR8,      InlineNone,         IPrimitive,  1,  0xFF,    0x76,    NEXT)
OPDEF(CEE_UNUSED58,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0x77,    NEXT)
OPDEF(CEE_UNUSED1,                    "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0x78,    NEXT)
OPDEF(CEE_UNBOX,                      "unbox",            PopRef,             PushI,       InlineType,         IPrimitive,  1,  0xFF,    0x79,    NEXT)
OPDEF(CEE_THROW,                      "throw",            PopRef,             Push0,       InlineNone,         IObjModel,   1,  0xFF,    0x7A,    THROW)
OPDEF(CEE_LDFLD,                      "ldfld",            PopRef,             Push1,       InlineField,        IObjModel,   1,  0xFF,    0x7B,    NEXT)
OPDEF(CEE_LDFLDA,                     "ldflda",           PopRef,             PushI,       InlineField,        IObjModel,   1,  0xFF,    0x7C,    NEXT)
OPDEF(CEE_STFLD,                      "stfld",            PopRef + Pop1,        Push0,       InlineField,        IObjModel,   1,  0xFF,    0x7D,    NEXT)
OPDEF(CEE_LDSFLD,                     "ldsfld",           Pop0,               Push1,       InlineField,        IObjModel,   1,  0xFF,    0x7E,    NEXT)
OPDEF(CEE_LDSFLDA,                    "ldsflda",          Pop0,               PushI,       InlineField,        IObjModel,   1,  0xFF,    0x7F,    NEXT)
OPDEF(CEE_STSFLD,                     "stsfld",           Pop1,               Push0,       InlineField,        IObjModel,   1,  0xFF,    0x80,    NEXT)
OPDEF(CEE_STOBJ,                      "stobj",            PopI + Pop1,          Push0,       InlineType,         IPrimitive,  1,  0xFF,    0x81,    NEXT)
OPDEF(CEE_CONV_OVF_I1_UN,             "conv.ovf.i1.un",   Pop1,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0x82,    NEXT)
OPDEF(CEE_CONV_OVF_I2_UN,             "conv.ovf.i2.un",   Pop1,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0x83,    NEXT)
OPDEF(CEE_CONV_OVF_I4_UN,             "conv.ovf.i4.un",   Pop1,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0x84,    NEXT)
OPDEF(CEE_CONV_OVF_I8_UN,             "conv.ovf.i8.un",   Pop1,               PushI8,      InlineNone,         IPrimitive,  1,  0xFF,    0x85,    NEXT)
OPDEF(CEE_CONV_OVF_U1_UN,             "conv.ovf.u1.un",   Pop1,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0x86,    NEXT)
OPDEF(CEE_CONV_OVF_U2_UN,             "conv.ovf.u2.un",   Pop1,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0x87,    NEXT)
OPDEF(CEE_CONV_OVF_U4_UN,             "conv.ovf.u4.un",   Pop1,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0x88,    NEXT)
OPDEF(CEE_CONV_OVF_U8_UN,             "conv.ovf.u8.un",   Pop1,               PushI8,      InlineNone,         IPrimitive,  1,  0xFF,    0x89,    NEXT)
OPDEF(CEE_CONV_OVF_I_UN,              "conv.ovf.i.un",    Pop1,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0x8A,    NEXT)
OPDEF(CEE_CONV_OVF_U_UN,              "conv.ovf.u.un",    Pop1,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0x8B,    NEXT)
OPDEF(CEE_BOX,                        "box",              Pop1,               PushRef,     InlineType,         IPrimitive,  1,  0xFF,    0x8C,    NEXT)
OPDEF(CEE_NEWARR,                     "newarr",           PopI,               PushRef,     InlineType,         IObjModel,   1,  0xFF,    0x8D,    NEXT)
OPDEF(CEE_LDLEN,                      "ldlen",            PopRef,             PushI,       InlineNone,         IObjModel,   1,  0xFF,    0x8E,    NEXT)
OPDEF(CEE_LDELEMA,                    "ldelema",          PopRef + PopI,        PushI,       InlineType,         IObjModel,   1,  0xFF,    0x8F,    NEXT)
OPDEF(CEE_LDELEM_I1,                  "ldelem.i1",        PopRef + PopI,        PushI,       InlineNone,         IObjModel,   1,  0xFF,    0x90,    NEXT)
OPDEF(CEE_LDELEM_U1,                  "ldelem.u1",        PopRef + PopI,        PushI,       InlineNone,         IObjModel,   1,  0xFF,    0x91,    NEXT)
OPDEF(CEE_LDELEM_I2,                  "ldelem.i2",        PopRef + PopI,        PushI,       InlineNone,         IObjModel,   1,  0xFF,    0x92,    NEXT)
OPDEF(CEE_LDELEM_U2,                  "ldelem.u2",        PopRef + PopI,        PushI,       InlineNone,         IObjModel,   1,  0xFF,    0x93,    NEXT)
OPDEF(CEE_LDELEM_I4,                  "ldelem.i4",        PopRef + PopI,        PushI,       InlineNone,         IObjModel,   1,  0xFF,    0x94,    NEXT)
OPDEF(CEE_LDELEM_U4,                  "ldelem.u4",        PopRef + PopI,        PushI,       InlineNone,         IObjModel,   1,  0xFF,    0x95,    NEXT)
OPDEF(CEE_LDELEM_I8,                  "ldelem.i8",        PopRef + PopI,        PushI8,      InlineNone,         IObjModel,   1,  0xFF,    0x96,    NEXT)
OPDEF(CEE_LDELEM_I,                   "ldelem.i",         PopRef + PopI,        PushI,       InlineNone,         IObjModel,   1,  0xFF,    0x97,    NEXT)
OPDEF(CEE_LDELEM_R4,                  "ldelem.r4",        PopRef + PopI,        PushR4,      InlineNone,         IObjModel,   1,  0xFF,    0x98,    NEXT)
OPDEF(CEE_LDELEM_R8,                  "ldelem.r8",        PopRef + PopI,        PushR8,      InlineNone,         IObjModel,   1,  0xFF,    0x99,    NEXT)
OPDEF(CEE_LDELEM_REF,                 "ldelem.ref",       PopRef + PopI,        PushRef,     InlineNone,         IObjModel,   1,  0xFF,    0x9A,    NEXT)
OPDEF(CEE_STELEM_I,                   "stelem.i",         PopRef + PopI + PopI,   Push0,       InlineNone,         IObjModel,   1,  0xFF,    0x9B,    NEXT)
OPDEF(CEE_STELEM_I1,                  "stelem.i1",        PopRef + PopI + PopI,   Push0,       InlineNone,         IObjModel,   1,  0xFF,    0x9C,    NEXT)
OPDEF(CEE_STELEM_I2,                  "stelem.i2",        PopRef + PopI + PopI,   Push0,       InlineNone,         IObjModel,   1,  0xFF,    0x9D,    NEXT)
OPDEF(CEE_STELEM_I4,                  "stelem.i4",        PopRef + PopI + PopI,   Push0,       InlineNone,         IObjModel,   1,  0xFF,    0x9E,    NEXT)
OPDEF(CEE_STELEM_I8,                  "stelem.i8",        PopRef + PopI + PopI8,  Push0,       InlineNone,         IObjModel,   1,  0xFF,    0x9F,    NEXT)
OPDEF(CEE_STELEM_R4,                  "stelem.r4",        PopRef + PopI + PopR4,  Push0,       InlineNone,         IObjModel,   1,  0xFF,    0xA0,    NEXT)
OPDEF(CEE_STELEM_R8,                  "stelem.r8",        PopRef + PopI + PopR8,  Push0,       InlineNone,         IObjModel,   1,  0xFF,    0xA1,    NEXT)
OPDEF(CEE_STELEM_REF,                 "stelem.ref",       PopRef + PopI + PopRef, Push0,       InlineNone,         IObjModel,   1,  0xFF,    0xA2,    NEXT)
OPDEF(CEE_LDELEM,                     "ldelem",           PopRef + PopI,        Push1,       InlineType,         IObjModel,   1,  0xFF,    0xA3,    NEXT)
OPDEF(CEE_STELEM,                     "stelem",           PopRef + PopI + Pop1,   Push0,       InlineType,         IObjModel,   1,  0xFF,    0xA4,    NEXT)
OPDEF(CEE_UNBOX_ANY,                  "unbox.any",        PopRef,             Push1,       InlineType,         IObjModel,   1,  0xFF,    0xA5,    NEXT)
OPDEF(CEE_UNUSED5,                    "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xA6,    NEXT)
OPDEF(CEE_UNUSED6,                    "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xA7,    NEXT)
OPDEF(CEE_UNUSED7,                    "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xA8,    NEXT)
OPDEF(CEE_UNUSED8,                    "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xA9,    NEXT)
OPDEF(CEE_UNUSED9,                    "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xAA,    NEXT)
OPDEF(CEE_UNUSED10,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xAB,    NEXT)
OPDEF(CEE_UNUSED11,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xAC,    NEXT)
OPDEF(CEE_UNUSED12,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xAD,    NEXT)
OPDEF(CEE_UNUSED13,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xAE,    NEXT)
OPDEF(CEE_UNUSED14,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xAF,    NEXT)
OPDEF(CEE_UNUSED15,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xB0,    NEXT)
OPDEF(CEE_UNUSED16,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xB1,    NEXT)
OPDEF(CEE_UNUSED17,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xB2,    NEXT)
OPDEF(CEE_CONV_OVF_I1,                "conv.ovf.i1",      Pop1,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0xB3,    NEXT)
OPDEF(CEE_CONV_OVF_U1,                "conv.ovf.u1",      Pop1,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0xB4,    NEXT)
OPDEF(CEE_CONV_OVF_I2,                "conv.ovf.i2",      Pop1,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0xB5,    NEXT)
OPDEF(CEE_CONV_OVF_U2,                "conv.ovf.u2",      Pop1,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0xB6,    NEXT)
OPDEF(CEE_CONV_OVF_I4,                "conv.ovf.i4",      Pop1,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0xB7,    NEXT)
OPDEF(CEE_CONV_OVF_U4,                "conv.ovf.u4",      Pop1,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0xB8,    NEXT)
OPDEF(CEE_CONV_OVF_I8,                "conv.ovf.i8",      Pop1,               PushI8,      InlineNone,         IPrimitive,  1,  0xFF,    0xB9,    NEXT)
OPDEF(CEE_CONV_OVF_U8,                "conv.ovf.u8",      Pop1,               PushI8,      InlineNone,         IPrimitive,  1,  0xFF,    0xBA,    NEXT)
OPDEF(CEE_UNUSED50,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xBB,    NEXT)
OPDEF(CEE_UNUSED18,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xBC,    NEXT)
OPDEF(CEE_UNUSED19,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xBD,    NEXT)
OPDEF(CEE_UNUSED20,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xBE,    NEXT)
OPDEF(CEE_UNUSED21,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xBF,    NEXT)
OPDEF(CEE_UNUSED22,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xC0,    NEXT)
OPDEF(CEE_UNUSED23,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xC1,    NEXT)
OPDEF(CEE_REFANYVAL,                  "refanyval",        Pop1,               PushI,       InlineType,         IPrimitive,  1,  0xFF,    0xC2,    NEXT)
OPDEF(CEE_CKFINITE,                   "ckfinite",         Pop1,               PushR8,      InlineNone,         IPrimitive,  1,  0xFF,    0xC3,    NEXT)
OPDEF(CEE_UNUSED24,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xC4,    NEXT)
OPDEF(CEE_UNUSED25,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xC5,    NEXT)
OPDEF(CEE_MKREFANY,                   "mkrefany",         PopI,               Push1,       InlineType,         IPrimitive,  1,  0xFF,    0xC6,    NEXT)
OPDEF(CEE_UNUSED59,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xC7,    NEXT)
OPDEF(CEE_UNUSED60,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xC8,    NEXT)
OPDEF(CEE_UNUSED61,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xC9,    NEXT)
OPDEF(CEE_UNUSED62,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xCA,    NEXT)
OPDEF(CEE_UNUSED63,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xCB,    NEXT)
OPDEF(CEE_UNUSED64,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xCC,    NEXT)
OPDEF(CEE_UNUSED65,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xCD,    NEXT)
OPDEF(CEE_UNUSED66,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xCE,    NEXT)
OPDEF(CEE_UNUSED67,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xCF,    NEXT)
OPDEF(CEE_LDTOKEN,                    "ldtoken",          Pop0,               PushI,       InlineTok,          IPrimitive,  1,  0xFF,    0xD0,    NEXT)
OPDEF(CEE_CONV_U2,                    "conv.u2",          Pop1,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0xD1,    NEXT)
OPDEF(CEE_CONV_U1,                    "conv.u1",          Pop1,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0xD2,    NEXT)
OPDEF(CEE_CONV_I,                     "conv.i",           Pop1,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0xD3,    NEXT)
OPDEF(CEE_CONV_OVF_I,                 "conv.ovf.i",       Pop1,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0xD4,    NEXT)
OPDEF(CEE_CONV_OVF_U,                 "conv.ovf.u",       Pop1,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0xD5,    NEXT)
OPDEF(CEE_ADD_OVF,                    "add.ovf",          Pop1 + Pop1,          Push1,       InlineNone,         IPrimitive,  1,  0xFF,    0xD6,    NEXT)
OPDEF(CEE_ADD_OVF_UN,                 "add.ovf.un",       Pop1 + Pop1,          Push1,       InlineNone,         IPrimitive,  1,  0xFF,    0xD7,    NEXT)
OPDEF(CEE_MUL_OVF,                    "mul.ovf",          Pop1 + Pop1,          Push1,       InlineNone,         IPrimitive,  1,  0xFF,    0xD8,    NEXT)
OPDEF(CEE_MUL_OVF_UN,                 "mul.ovf.un",       Pop1 + Pop1,          Push1,       InlineNone,         IPrimitive,  1,  0xFF,    0xD9,    NEXT)
OPDEF(CEE_SUB_OVF,                    "sub.ovf",          Pop1 + Pop1,          Push1,       InlineNone,         IPrimitive,  1,  0xFF,    0xDA,    NEXT)
OPDEF(CEE_SUB_OVF_UN,                 "sub.ovf.un",       Pop1 + Pop1,          Push1,       InlineNone,         IPrimitive,  1,  0xFF,    0xDB,    NEXT)
OPDEF(CEE_ENDFINALLY,                 "endfinally",       Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xDC,    RETURN)
OPDEF(CEE_LEAVE,                      "leave",            Pop0,               Push0,       InlineBrTarget,     IPrimitive,  1,  0xFF,    0xDD,    BRANCH)
OPDEF(CEE_LEAVE_S,                    "leave.s",          Pop0,               Push0,       ShortInlineBrTarget,IPrimitive,  1,  0xFF,    0xDE,    BRANCH)
OPDEF(CEE_STIND_I,                    "stind.i",          PopI + PopI,          Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xDF,    NEXT)
OPDEF(CEE_CONV_U,                     "conv.u",           Pop1,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0xE0,    NEXT)
OPDEF(CEE_UNUSED26,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xE1,    NEXT)
OPDEF(CEE_UNUSED27,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xE2,    NEXT)
OPDEF(CEE_UNUSED28,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xE3,    NEXT)
OPDEF(CEE_UNUSED29,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xE4,    NEXT)
OPDEF(CEE_UNUSED30,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xE5,    NEXT)
OPDEF(CEE_UNUSED31,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xE6,    NEXT)
OPDEF(CEE_UNUSED32,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xE7,    NEXT)
OPDEF(CEE_UNUSED33,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xE8,    NEXT)
OPDEF(CEE_UNUSED34,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xE9,    NEXT)
OPDEF(CEE_UNUSED35,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xEA,    NEXT)
OPDEF(CEE_UNUSED36,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xEB,    NEXT)
OPDEF(CEE_UNUSED37,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xEC,    NEXT)
OPDEF(CEE_UNUSED38,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xED,    NEXT)
OPDEF(CEE_UNUSED39,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xEE,    NEXT)
OPDEF(CEE_UNUSED40,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xEF,    NEXT)
OPDEF(CEE_UNUSED41,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xF0,    NEXT)
OPDEF(CEE_UNUSED42,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xF1,    NEXT)
OPDEF(CEE_UNUSED43,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xF2,    NEXT)
OPDEF(CEE_UNUSED44,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xF3,    NEXT)
OPDEF(CEE_UNUSED45,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xF4,    NEXT)
OPDEF(CEE_UNUSED46,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xF5,    NEXT)
OPDEF(CEE_UNUSED47,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xF6,    NEXT)
OPDEF(CEE_UNUSED48,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xF7,    NEXT)
OPDEF(CEE_PREFIX7,                    "prefix7",          Pop0,               Push0,       InlineNone,         IInternal,   1,  0xFF,    0xF8,    META)
OPDEF(CEE_PREFIX6,                    "prefix6",          Pop0,               Push0,       InlineNone,         IInternal,   1,  0xFF,    0xF9,    META)
OPDEF(CEE_PREFIX5,                    "prefix5",          Pop0,               Push0,       InlineNone,         IInternal,   1,  0xFF,    0xFA,    META)
OPDEF(CEE_PREFIX4,                    "prefix4",          Pop0,               Push0,       InlineNone,         IInternal,   1,  0xFF,    0xFB,    META)
OPDEF(CEE_PREFIX3,                    "prefix3",          Pop0,               Push0,       InlineNone,         IInternal,   1,  0xFF,    0xFC,    META)
OPDEF(CEE_PREFIX2,                    "prefix2",          Pop0,               Push0,       InlineNone,         IInternal,   1,  0xFF,    0xFD,    META)
OPDEF(CEE_PREFIX1,                    "prefix1",          Pop0,               Push0,       InlineNone,         IInternal,   1,  0xFF,    0xFE,    META)
OPDEF(CEE_PREFIXREF,                  "prefixref",        Pop0,               Push0,       InlineNone,         IInternal,   1,  0xFF,    0xFF,    META)

OPDEF(CEE_ARGLIST,                    "arglist",          Pop0,               PushI,       InlineNone,         IPrimitive,  2,  0xFE,    0x00,    NEXT)
OPDEF(CEE_CEQ,                        "ceq",              Pop1 + Pop1,          PushI,       InlineNone,         IPrimitive,  2,  0xFE,    0x01,    NEXT)
OPDEF(CEE_CGT,                        "cgt",              Pop1 + Pop1,          PushI,       InlineNone,         IPrimitive,  2,  0xFE,    0x02,    NEXT)
OPDEF(CEE_CGT_UN,                     "cgt.un",           Pop1 + Pop1,          PushI,       InlineNone,         IPrimitive,  2,  0xFE,    0x03,    NEXT)
OPDEF(CEE_CLT,                        "clt",              Pop1 + Pop1,          PushI,       InlineNone,         IPrimitive,  2,  0xFE,    0x04,    NEXT)
OPDEF(CEE_CLT_UN,                     "clt.un",           Pop1 + Pop1,          PushI,       InlineNone,         IPrimitive,  2,  0xFE,    0x05,    NEXT)
OPDEF(CEE_LDFTN,                      "ldftn",            Pop0,               PushI,       InlineMethod,       IPrimitive,  2,  0xFE,    0x06,    NEXT)
OPDEF(CEE_LDVIRTFTN,                  "ldvirtftn",        PopRef,             PushI,       InlineMethod,       IPrimitive,  2,  0xFE,    0x07,    NEXT)
OPDEF(CEE_UNUSED56,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  2,  0xFE,    0x08,    NEXT)
OPDEF(CEE_LDARG,                      "ldarg",            Pop0,               Push1,       InlineVar,          IPrimitive,  2,  0xFE,    0x09,    NEXT)
OPDEF(CEE_LDARGA,                     "ldarga",           Pop0,               PushI,       InlineVar,          IPrimitive,  2,  0xFE,    0x0A,    NEXT)
OPDEF(CEE_STARG,                      "starg",            Pop1,               Push0,       InlineVar,          IPrimitive,  2,  0xFE,    0x0B,    NEXT)
OPDEF(CEE_LDLOC,                      "ldloc",            Pop0,               Push1,       InlineVar,          IPrimitive,  2,  0xFE,    0x0C,    NEXT)
OPDEF(CEE_LDLOCA,                     "ldloca",           Pop0,               PushI,       InlineVar,          IPrimitive,  2,  0xFE,    0x0D,    NEXT)
OPDEF(CEE_STLOC,                      "stloc",            Pop1,               Push0,       InlineVar,          IPrimitive,  2,  0xFE,    0x0E,    NEXT)
OPDEF(CEE_LOCALLOC,                   "localloc",         PopI,               PushI,       InlineNone,         IPrimitive,  2,  0xFE,    0x0F,    NEXT)
OPDEF(CEE_UNUSED57,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  2,  0xFE,    0x10,    NEXT)
OPDEF(CEE_ENDFILTER,                  "endfilter",        PopI,               Push0,       InlineNone,         IPrimitive,  2,  0xFE,    0x11,    RETURN)
OPDEF(CEE_UNALIGNED,                  "unaligned.",       Pop0,               Push0,       ShortInlineI,       IPrefix,     2,  0xFE,    0x12,    META)
OPDEF(CEE_VOLATILE,                   "volatile.",        Pop0,               Push0,       InlineNone,         IPrefix,     2,  0xFE,    0x13,    META)
OPDEF(CEE_TAILCALL,                   "tail.",            Pop0,               Push0,       InlineNone,         IPrefix,     2,  0xFE,    0x14,    META)
OPDEF(CEE_INITOBJ,                    "initobj",          PopI,               Push0,       InlineType,         IObjModel,   2,  0xFE,    0x15,    NEXT)
OPDEF(CEE_CONSTRAINED,                "constrained.",     Pop0,               Push0,       InlineType,         IPrefix,     2,  0xFE,    0x16,    META)
OPDEF(CEE_CPBLK,                      "cpblk",            PopI + PopI + PopI,     Push0,       InlineNone,         IPrimitive,  2,  0xFE,    0x17,    NEXT)
OPDEF(CEE_INITBLK,                    "initblk",          PopI + PopI + PopI,     Push0,       InlineNone,         IPrimitive,  2,  0xFE,    0x18,    NEXT)
OPDEF(CEE_UNUSED69,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  2,  0xFE,    0x19,    NEXT)
OPDEF(CEE_RETHROW,                    "rethrow",          Pop0,               Push0,       InlineNone,         IObjModel,   2,  0xFE,    0x1A,    THROW)
OPDEF(CEE_UNUSED51,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  2,  0xFE,    0x1B,    NEXT)
OPDEF(CEE_SIZEOF,                     "sizeof",           Pop0,               PushI,       InlineType,         IPrimitive,  2,  0xFE,    0x1C,    NEXT)
OPDEF(CEE_REFANYTYPE,                 "refanytype",       Pop1,               PushI,       InlineNone,         IPrimitive,  2,  0xFE,    0x1D,    NEXT)
OPDEF(CEE_READONLY,                   "readonly.",        Pop0,               Push0,       InlineNone,         IPrefix,     2,  0xFE,    0x1E,    META)
OPDEF(CEE_UNUSED53,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  2,  0xFE,    0x1F,    NEXT)
OPDEF(CEE_UNUSED54,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  2,  0xFE,    0x20,    NEXT)
OPDEF(CEE_UNUSED55,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  2,  0xFE,    0x21,    NEXT)
OPDEF(CEE_UNUSED70,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  2,  0xFE,    0x22,    NEXT)

// These are not real opcodes, but they are handy internally in the EE

OPDEF(CEE_ILLEGAL,                    "illegal",          Pop0,               Push0,       InlineNone,         IInternal,   0,  MOOT,    MOOT,    META)
OPDEF(CEE_MACRO_END,                  "endmac",           Pop0,               Push0,       InlineNone,         IInternal,   0,  MOOT,    MOOT,    META)
OPDEF(CEE_CODE_LABEL,                 "codelabel",        Pop0,               Push0,       InlineNone,         IInternal,   0,  MOOT,    MOOT,    META)

};

extern const BYTE opcodeArgKinds[] = {
#define OPDEF(name, string, pop, push, oprType, opcType, l, s1, s2, ctrl) (BYTE) oprType,
OPDEF(CEE_NOP,                        "nop",              Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0x00,    NEXT)
OPDEF(CEE_BREAK,                      "break",            Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0x01,    BREAK)
OPDEF(CEE_LDARG_0,                    "ldarg.0",          Pop0,               Push1,       InlineNone,         IMacro,      1,  0xFF,    0x02,    NEXT)
OPDEF(CEE_LDARG_1,                    "ldarg.1",          Pop0,               Push1,       InlineNone,         IMacro,      1,  0xFF,    0x03,    NEXT)
OPDEF(CEE_LDARG_2,                    "ldarg.2",          Pop0,               Push1,       InlineNone,         IMacro,      1,  0xFF,    0x04,    NEXT)
OPDEF(CEE_LDARG_3,                    "ldarg.3",          Pop0,               Push1,       InlineNone,         IMacro,      1,  0xFF,    0x05,    NEXT)
OPDEF(CEE_LDLOC_0,                    "ldloc.0",          Pop0,               Push1,       InlineNone,         IMacro,      1,  0xFF,    0x06,    NEXT)
OPDEF(CEE_LDLOC_1,                    "ldloc.1",          Pop0,               Push1,       InlineNone,         IMacro,      1,  0xFF,    0x07,    NEXT)
OPDEF(CEE_LDLOC_2,                    "ldloc.2",          Pop0,               Push1,       InlineNone,         IMacro,      1,  0xFF,    0x08,    NEXT)
OPDEF(CEE_LDLOC_3,                    "ldloc.3",          Pop0,               Push1,       InlineNone,         IMacro,      1,  0xFF,    0x09,    NEXT)
OPDEF(CEE_STLOC_0,                    "stloc.0",          Pop1,               Push0,       InlineNone,         IMacro,      1,  0xFF,    0x0A,    NEXT)
OPDEF(CEE_STLOC_1,                    "stloc.1",          Pop1,               Push0,       InlineNone,         IMacro,      1,  0xFF,    0x0B,    NEXT)
OPDEF(CEE_STLOC_2,                    "stloc.2",          Pop1,               Push0,       InlineNone,         IMacro,      1,  0xFF,    0x0C,    NEXT)
OPDEF(CEE_STLOC_3,                    "stloc.3",          Pop1,               Push0,       InlineNone,         IMacro,      1,  0xFF,    0x0D,    NEXT)
OPDEF(CEE_LDARG_S,                    "ldarg.s",          Pop0,               Push1,       ShortInlineVar,     IMacro,      1,  0xFF,    0x0E,    NEXT)
OPDEF(CEE_LDARGA_S,                   "ldarga.s",         Pop0,               PushI,       ShortInlineVar,     IMacro,      1,  0xFF,    0x0F,    NEXT)
OPDEF(CEE_STARG_S,                    "starg.s",          Pop1,               Push0,       ShortInlineVar,     IMacro,      1,  0xFF,    0x10,    NEXT)
OPDEF(CEE_LDLOC_S,                    "ldloc.s",          Pop0,               Push1,       ShortInlineVar,     IMacro,      1,  0xFF,    0x11,    NEXT)
OPDEF(CEE_LDLOCA_S,                   "ldloca.s",         Pop0,               PushI,       ShortInlineVar,     IMacro,      1,  0xFF,    0x12,    NEXT)
OPDEF(CEE_STLOC_S,                    "stloc.s",          Pop1,               Push0,       ShortInlineVar,     IMacro,      1,  0xFF,    0x13,    NEXT)
OPDEF(CEE_LDNULL,                     "ldnull",           Pop0,               PushRef,     InlineNone,         IPrimitive,  1,  0xFF,    0x14,    NEXT)
OPDEF(CEE_LDC_I4_M1,                  "ldc.i4.m1",        Pop0,               PushI,       InlineNone,         IMacro,      1,  0xFF,    0x15,    NEXT)
OPDEF(CEE_LDC_I4_0,                   "ldc.i4.0",         Pop0,               PushI,       InlineNone,         IMacro,      1,  0xFF,    0x16,    NEXT)
OPDEF(CEE_LDC_I4_1,                   "ldc.i4.1",         Pop0,               PushI,       InlineNone,         IMacro,      1,  0xFF,    0x17,    NEXT)
OPDEF(CEE_LDC_I4_2,                   "ldc.i4.2",         Pop0,               PushI,       InlineNone,         IMacro,      1,  0xFF,    0x18,    NEXT)
OPDEF(CEE_LDC_I4_3,                   "ldc.i4.3",         Pop0,               PushI,       InlineNone,         IMacro,      1,  0xFF,    0x19,    NEXT)
OPDEF(CEE_LDC_I4_4,                   "ldc.i4.4",         Pop0,               PushI,       InlineNone,         IMacro,      1,  0xFF,    0x1A,    NEXT)
OPDEF(CEE_LDC_I4_5,                   "ldc.i4.5",         Pop0,               PushI,       InlineNone,         IMacro,      1,  0xFF,    0x1B,    NEXT)
OPDEF(CEE_LDC_I4_6,                   "ldc.i4.6",         Pop0,               PushI,       InlineNone,         IMacro,      1,  0xFF,    0x1C,    NEXT)
OPDEF(CEE_LDC_I4_7,                   "ldc.i4.7",         Pop0,               PushI,       InlineNone,         IMacro,      1,  0xFF,    0x1D,    NEXT)
OPDEF(CEE_LDC_I4_8,                   "ldc.i4.8",         Pop0,               PushI,       InlineNone,         IMacro,      1,  0xFF,    0x1E,    NEXT)
OPDEF(CEE_LDC_I4_S,                   "ldc.i4.s",         Pop0,               PushI,       ShortInlineI,       IMacro,      1,  0xFF,    0x1F,    NEXT)
OPDEF(CEE_LDC_I4,                     "ldc.i4",           Pop0,               PushI,       InlineI,            IPrimitive,  1,  0xFF,    0x20,    NEXT)
OPDEF(CEE_LDC_I8,                     "ldc.i8",           Pop0,               PushI8,      InlineI8,           IPrimitive,  1,  0xFF,    0x21,    NEXT)
OPDEF(CEE_LDC_R4,                     "ldc.r4",           Pop0,               PushR4,      ShortInlineR,       IPrimitive,  1,  0xFF,    0x22,    NEXT)
OPDEF(CEE_LDC_R8,                     "ldc.r8",           Pop0,               PushR8,      InlineR,            IPrimitive,  1,  0xFF,    0x23,    NEXT)
OPDEF(CEE_UNUSED49,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0x24,    NEXT)
OPDEF(CEE_DUP,                        "dup",              Pop1,               Push1 + Push1, InlineNone,         IPrimitive,  1,  0xFF,    0x25,    NEXT)
OPDEF(CEE_POP,                        "pop",              Pop1,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0x26,    NEXT)
OPDEF(CEE_JMP,                        "jmp",              Pop0,               Push0,       InlineMethod,       IPrimitive,  1,  0xFF,    0x27,    CALL)
OPDEF(CEE_CALL,                       "call",             VarPop,             VarPush,     InlineMethod,       IPrimitive,  1,  0xFF,    0x28,    CALL)
OPDEF(CEE_CALLI,                      "calli",            VarPop,             VarPush,     InlineSig,          IPrimitive,  1,  0xFF,    0x29,    CALL)
OPDEF(CEE_RET,                        "ret",              VarPop,             Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0x2A,    RETURN)
OPDEF(CEE_BR_S,                       "br.s",             Pop0,               Push0,       ShortInlineBrTarget,IMacro,      1,  0xFF,    0x2B,    BRANCH)
OPDEF(CEE_BRFALSE_S,                  "brfalse.s",        PopI,               Push0,       ShortInlineBrTarget,IMacro,      1,  0xFF,    0x2C,    COND_BRANCH)
OPDEF(CEE_BRTRUE_S,                   "brtrue.s",         PopI,               Push0,       ShortInlineBrTarget,IMacro,      1,  0xFF,    0x2D,    COND_BRANCH)
OPDEF(CEE_BEQ_S,                      "beq.s",            Pop1 + Pop1,          Push0,       ShortInlineBrTarget,IMacro,      1,  0xFF,    0x2E,    COND_BRANCH)
OPDEF(CEE_BGE_S,                      "bge.s",            Pop1 + Pop1,          Push0,       ShortInlineBrTarget,IMacro,      1,  0xFF,    0x2F,    COND_BRANCH)
OPDEF(CEE_BGT_S,                      "bgt.s",            Pop1 + Pop1,          Push0,       ShortInlineBrTarget,IMacro,      1,  0xFF,    0x30,    COND_BRANCH)
OPDEF(CEE_BLE_S,                      "ble.s",            Pop1 + Pop1,          Push0,       ShortInlineBrTarget,IMacro,      1,  0xFF,    0x31,    COND_BRANCH)
OPDEF(CEE_BLT_S,                      "blt.s",            Pop1 + Pop1,          Push0,       ShortInlineBrTarget,IMacro,      1,  0xFF,    0x32,    COND_BRANCH)
OPDEF(CEE_BNE_UN_S,                   "bne.un.s",         Pop1 + Pop1,          Push0,       ShortInlineBrTarget,IMacro,      1,  0xFF,    0x33,    COND_BRANCH)
OPDEF(CEE_BGE_UN_S,                   "bge.un.s",         Pop1 + Pop1,          Push0,       ShortInlineBrTarget,IMacro,      1,  0xFF,    0x34,    COND_BRANCH)
OPDEF(CEE_BGT_UN_S,                   "bgt.un.s",         Pop1 + Pop1,          Push0,       ShortInlineBrTarget,IMacro,      1,  0xFF,    0x35,    COND_BRANCH)
OPDEF(CEE_BLE_UN_S,                   "ble.un.s",         Pop1 + Pop1,          Push0,       ShortInlineBrTarget,IMacro,      1,  0xFF,    0x36,    COND_BRANCH)
OPDEF(CEE_BLT_UN_S,                   "blt.un.s",         Pop1 + Pop1,          Push0,       ShortInlineBrTarget,IMacro,      1,  0xFF,    0x37,    COND_BRANCH)
OPDEF(CEE_BR,                         "br",               Pop0,               Push0,       InlineBrTarget,     IPrimitive,  1,  0xFF,    0x38,    BRANCH)
OPDEF(CEE_BRFALSE,                    "brfalse",          PopI,               Push0,       InlineBrTarget,     IPrimitive,  1,  0xFF,    0x39,    COND_BRANCH)
OPDEF(CEE_BRTRUE,                     "brtrue",           PopI,               Push0,       InlineBrTarget,     IPrimitive,  1,  0xFF,    0x3A,    COND_BRANCH)
OPDEF(CEE_BEQ,                        "beq",              Pop1 + Pop1,          Push0,       InlineBrTarget,     IMacro,      1,  0xFF,    0x3B,    COND_BRANCH)
OPDEF(CEE_BGE,                        "bge",              Pop1 + Pop1,          Push0,       InlineBrTarget,     IMacro,      1,  0xFF,    0x3C,    COND_BRANCH)
OPDEF(CEE_BGT,                        "bgt",              Pop1 + Pop1,          Push0,       InlineBrTarget,     IMacro,      1,  0xFF,    0x3D,    COND_BRANCH)
OPDEF(CEE_BLE,                        "ble",              Pop1 + Pop1,          Push0,       InlineBrTarget,     IMacro,      1,  0xFF,    0x3E,    COND_BRANCH)
OPDEF(CEE_BLT,                        "blt",              Pop1 + Pop1,          Push0,       InlineBrTarget,     IMacro,      1,  0xFF,    0x3F,    COND_BRANCH)
OPDEF(CEE_BNE_UN,                     "bne.un",           Pop1 + Pop1,          Push0,       InlineBrTarget,     IMacro,      1,  0xFF,    0x40,    COND_BRANCH)
OPDEF(CEE_BGE_UN,                     "bge.un",           Pop1 + Pop1,          Push0,       InlineBrTarget,     IMacro,      1,  0xFF,    0x41,    COND_BRANCH)
OPDEF(CEE_BGT_UN,                     "bgt.un",           Pop1 + Pop1,          Push0,       InlineBrTarget,     IMacro,      1,  0xFF,    0x42,    COND_BRANCH)
OPDEF(CEE_BLE_UN,                     "ble.un",           Pop1 + Pop1,          Push0,       InlineBrTarget,     IMacro,      1,  0xFF,    0x43,    COND_BRANCH)
OPDEF(CEE_BLT_UN,                     "blt.un",           Pop1 + Pop1,          Push0,       InlineBrTarget,     IMacro,      1,  0xFF,    0x44,    COND_BRANCH)
OPDEF(CEE_SWITCH,                     "switch",           PopI,               Push0,       InlineSwitch,       IPrimitive,  1,  0xFF,    0x45,    COND_BRANCH)
OPDEF(CEE_LDIND_I1,                   "ldind.i1",         PopI,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0x46,    NEXT)
OPDEF(CEE_LDIND_U1,                   "ldind.u1",         PopI,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0x47,    NEXT)
OPDEF(CEE_LDIND_I2,                   "ldind.i2",         PopI,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0x48,    NEXT)
OPDEF(CEE_LDIND_U2,                   "ldind.u2",         PopI,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0x49,    NEXT)
OPDEF(CEE_LDIND_I4,                   "ldind.i4",         PopI,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0x4A,    NEXT)
OPDEF(CEE_LDIND_U4,                   "ldind.u4",         PopI,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0x4B,    NEXT)
OPDEF(CEE_LDIND_I8,                   "ldind.i8",         PopI,               PushI8,      InlineNone,         IPrimitive,  1,  0xFF,    0x4C,    NEXT)
OPDEF(CEE_LDIND_I,                    "ldind.i",          PopI,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0x4D,    NEXT)
OPDEF(CEE_LDIND_R4,                   "ldind.r4",         PopI,               PushR4,      InlineNone,         IPrimitive,  1,  0xFF,    0x4E,    NEXT)
OPDEF(CEE_LDIND_R8,                   "ldind.r8",         PopI,               PushR8,      InlineNone,         IPrimitive,  1,  0xFF,    0x4F,    NEXT)
OPDEF(CEE_LDIND_REF,                  "ldind.ref",        PopI,               PushRef,     InlineNone,         IPrimitive,  1,  0xFF,    0x50,    NEXT)
OPDEF(CEE_STIND_REF,                  "stind.ref",        PopI + PopI,          Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0x51,    NEXT)
OPDEF(CEE_STIND_I1,                   "stind.i1",         PopI + PopI,          Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0x52,    NEXT)
OPDEF(CEE_STIND_I2,                   "stind.i2",         PopI + PopI,          Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0x53,    NEXT)
OPDEF(CEE_STIND_I4,                   "stind.i4",         PopI + PopI,          Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0x54,    NEXT)
OPDEF(CEE_STIND_I8,                   "stind.i8",         PopI + PopI8,         Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0x55,    NEXT)
OPDEF(CEE_STIND_R4,                   "stind.r4",         PopI + PopR4,         Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0x56,    NEXT)
OPDEF(CEE_STIND_R8,                   "stind.r8",         PopI + PopR8,         Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0x57,    NEXT)
OPDEF(CEE_ADD,                        "add",              Pop1 + Pop1,          Push1,       InlineNone,         IPrimitive,  1,  0xFF,    0x58,    NEXT)
OPDEF(CEE_SUB,                        "sub",              Pop1 + Pop1,          Push1,       InlineNone,         IPrimitive,  1,  0xFF,    0x59,    NEXT)
OPDEF(CEE_MUL,                        "mul",              Pop1 + Pop1,          Push1,       InlineNone,         IPrimitive,  1,  0xFF,    0x5A,    NEXT)
OPDEF(CEE_DIV,                        "div",              Pop1 + Pop1,          Push1,       InlineNone,         IPrimitive,  1,  0xFF,    0x5B,    NEXT)
OPDEF(CEE_DIV_UN,                     "div.un",           Pop1 + Pop1,          Push1,       InlineNone,         IPrimitive,  1,  0xFF,    0x5C,    NEXT)
OPDEF(CEE_REM,                        "rem",              Pop1 + Pop1,          Push1,       InlineNone,         IPrimitive,  1,  0xFF,    0x5D,    NEXT)
OPDEF(CEE_REM_UN,                     "rem.un",           Pop1 + Pop1,          Push1,       InlineNone,         IPrimitive,  1,  0xFF,    0x5E,    NEXT)
OPDEF(CEE_AND,                        "and",              Pop1 + Pop1,          Push1,       InlineNone,         IPrimitive,  1,  0xFF,    0x5F,    NEXT)
OPDEF(CEE_OR,                         "or",               Pop1 + Pop1,          Push1,       InlineNone,         IPrimitive,  1,  0xFF,    0x60,    NEXT)
OPDEF(CEE_XOR,                        "xor",              Pop1 + Pop1,          Push1,       InlineNone,         IPrimitive,  1,  0xFF,    0x61,    NEXT)
OPDEF(CEE_SHL,                        "shl",              Pop1 + Pop1,          Push1,       InlineNone,         IPrimitive,  1,  0xFF,    0x62,    NEXT)
OPDEF(CEE_SHR,                        "shr",              Pop1 + Pop1,          Push1,       InlineNone,         IPrimitive,  1,  0xFF,    0x63,    NEXT)
OPDEF(CEE_SHR_UN,                     "shr.un",           Pop1 + Pop1,          Push1,       InlineNone,         IPrimitive,  1,  0xFF,    0x64,    NEXT)
OPDEF(CEE_NEG,                        "neg",              Pop1,               Push1,       InlineNone,         IPrimitive,  1,  0xFF,    0x65,    NEXT)
OPDEF(CEE_NOT,                        "not",              Pop1,               Push1,       InlineNone,         IPrimitive,  1,  0xFF,    0x66,    NEXT)
OPDEF(CEE_CONV_I1,                    "conv.i1",          Pop1,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0x67,    NEXT)
OPDEF(CEE_CONV_I2,                    "conv.i2",          Pop1,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0x68,    NEXT)
OPDEF(CEE_CONV_I4,                    "conv.i4",          Pop1,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0x69,    NEXT)
OPDEF(CEE_CONV_I8,                    "conv.i8",          Pop1,               PushI8,      InlineNone,         IPrimitive,  1,  0xFF,    0x6A,    NEXT)
OPDEF(CEE_CONV_R4,                    "conv.r4",          Pop1,               PushR4,      InlineNone,         IPrimitive,  1,  0xFF,    0x6B,    NEXT)
OPDEF(CEE_CONV_R8,                    "conv.r8",          Pop1,               PushR8,      InlineNone,         IPrimitive,  1,  0xFF,    0x6C,    NEXT)
OPDEF(CEE_CONV_U4,                    "conv.u4",          Pop1,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0x6D,    NEXT)
OPDEF(CEE_CONV_U8,                    "conv.u8",          Pop1,               PushI8,      InlineNone,         IPrimitive,  1,  0xFF,    0x6E,    NEXT)
OPDEF(CEE_CALLVIRT,                   "callvirt",         VarPop,             VarPush,     InlineMethod,       IObjModel,   1,  0xFF,    0x6F,    CALL)
OPDEF(CEE_CPOBJ,                      "cpobj",            PopI + PopI,          Push0,       InlineType,         IObjModel,   1,  0xFF,    0x70,    NEXT)
OPDEF(CEE_LDOBJ,                      "ldobj",            PopI,               Push1,       InlineType,         IObjModel,   1,  0xFF,    0x71,    NEXT)
OPDEF(CEE_LDSTR,                      "ldstr",            Pop0,               PushRef,     InlineString,       IObjModel,   1,  0xFF,    0x72,    NEXT)
OPDEF(CEE_NEWOBJ,                     "newobj",           VarPop,             PushRef,     InlineMethod,       IObjModel,   1,  0xFF,    0x73,    CALL)
OPDEF(CEE_CASTCLASS,                  "castclass",        PopRef,             PushRef,     InlineType,         IObjModel,   1,  0xFF,    0x74,    NEXT)
OPDEF(CEE_ISINST,                     "isinst",           PopRef,             PushI,       InlineType,         IObjModel,   1,  0xFF,    0x75,    NEXT)
OPDEF(CEE_CONV_R_UN,                  "conv.r.un",        Pop1,               PushR8,      InlineNone,         IPrimitive,  1,  0xFF,    0x76,    NEXT)
OPDEF(CEE_UNUSED58,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0x77,    NEXT)
OPDEF(CEE_UNUSED1,                    "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0x78,    NEXT)
OPDEF(CEE_UNBOX,                      "unbox",            PopRef,             PushI,       InlineType,         IPrimitive,  1,  0xFF,    0x79,    NEXT)
OPDEF(CEE_THROW,                      "throw",            PopRef,             Push0,       InlineNone,         IObjModel,   1,  0xFF,    0x7A,    THROW)
OPDEF(CEE_LDFLD,                      "ldfld",            PopRef,             Push1,       InlineField,        IObjModel,   1,  0xFF,    0x7B,    NEXT)
OPDEF(CEE_LDFLDA,                     "ldflda",           PopRef,             PushI,       InlineField,        IObjModel,   1,  0xFF,    0x7C,    NEXT)
OPDEF(CEE_STFLD,                      "stfld",            PopRef + Pop1,        Push0,       InlineField,        IObjModel,   1,  0xFF,    0x7D,    NEXT)
OPDEF(CEE_LDSFLD,                     "ldsfld",           Pop0,               Push1,       InlineField,        IObjModel,   1,  0xFF,    0x7E,    NEXT)
OPDEF(CEE_LDSFLDA,                    "ldsflda",          Pop0,               PushI,       InlineField,        IObjModel,   1,  0xFF,    0x7F,    NEXT)
OPDEF(CEE_STSFLD,                     "stsfld",           Pop1,               Push0,       InlineField,        IObjModel,   1,  0xFF,    0x80,    NEXT)
OPDEF(CEE_STOBJ,                      "stobj",            PopI + Pop1,          Push0,       InlineType,         IPrimitive,  1,  0xFF,    0x81,    NEXT)
OPDEF(CEE_CONV_OVF_I1_UN,             "conv.ovf.i1.un",   Pop1,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0x82,    NEXT)
OPDEF(CEE_CONV_OVF_I2_UN,             "conv.ovf.i2.un",   Pop1,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0x83,    NEXT)
OPDEF(CEE_CONV_OVF_I4_UN,             "conv.ovf.i4.un",   Pop1,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0x84,    NEXT)
OPDEF(CEE_CONV_OVF_I8_UN,             "conv.ovf.i8.un",   Pop1,               PushI8,      InlineNone,         IPrimitive,  1,  0xFF,    0x85,    NEXT)
OPDEF(CEE_CONV_OVF_U1_UN,             "conv.ovf.u1.un",   Pop1,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0x86,    NEXT)
OPDEF(CEE_CONV_OVF_U2_UN,             "conv.ovf.u2.un",   Pop1,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0x87,    NEXT)
OPDEF(CEE_CONV_OVF_U4_UN,             "conv.ovf.u4.un",   Pop1,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0x88,    NEXT)
OPDEF(CEE_CONV_OVF_U8_UN,             "conv.ovf.u8.un",   Pop1,               PushI8,      InlineNone,         IPrimitive,  1,  0xFF,    0x89,    NEXT)
OPDEF(CEE_CONV_OVF_I_UN,              "conv.ovf.i.un",    Pop1,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0x8A,    NEXT)
OPDEF(CEE_CONV_OVF_U_UN,              "conv.ovf.u.un",    Pop1,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0x8B,    NEXT)
OPDEF(CEE_BOX,                        "box",              Pop1,               PushRef,     InlineType,         IPrimitive,  1,  0xFF,    0x8C,    NEXT)
OPDEF(CEE_NEWARR,                     "newarr",           PopI,               PushRef,     InlineType,         IObjModel,   1,  0xFF,    0x8D,    NEXT)
OPDEF(CEE_LDLEN,                      "ldlen",            PopRef,             PushI,       InlineNone,         IObjModel,   1,  0xFF,    0x8E,    NEXT)
OPDEF(CEE_LDELEMA,                    "ldelema",          PopRef + PopI,        PushI,       InlineType,         IObjModel,   1,  0xFF,    0x8F,    NEXT)
OPDEF(CEE_LDELEM_I1,                  "ldelem.i1",        PopRef + PopI,        PushI,       InlineNone,         IObjModel,   1,  0xFF,    0x90,    NEXT)
OPDEF(CEE_LDELEM_U1,                  "ldelem.u1",        PopRef + PopI,        PushI,       InlineNone,         IObjModel,   1,  0xFF,    0x91,    NEXT)
OPDEF(CEE_LDELEM_I2,                  "ldelem.i2",        PopRef + PopI,        PushI,       InlineNone,         IObjModel,   1,  0xFF,    0x92,    NEXT)
OPDEF(CEE_LDELEM_U2,                  "ldelem.u2",        PopRef + PopI,        PushI,       InlineNone,         IObjModel,   1,  0xFF,    0x93,    NEXT)
OPDEF(CEE_LDELEM_I4,                  "ldelem.i4",        PopRef + PopI,        PushI,       InlineNone,         IObjModel,   1,  0xFF,    0x94,    NEXT)
OPDEF(CEE_LDELEM_U4,                  "ldelem.u4",        PopRef + PopI,        PushI,       InlineNone,         IObjModel,   1,  0xFF,    0x95,    NEXT)
OPDEF(CEE_LDELEM_I8,                  "ldelem.i8",        PopRef + PopI,        PushI8,      InlineNone,         IObjModel,   1,  0xFF,    0x96,    NEXT)
OPDEF(CEE_LDELEM_I,                   "ldelem.i",         PopRef + PopI,        PushI,       InlineNone,         IObjModel,   1,  0xFF,    0x97,    NEXT)
OPDEF(CEE_LDELEM_R4,                  "ldelem.r4",        PopRef + PopI,        PushR4,      InlineNone,         IObjModel,   1,  0xFF,    0x98,    NEXT)
OPDEF(CEE_LDELEM_R8,                  "ldelem.r8",        PopRef + PopI,        PushR8,      InlineNone,         IObjModel,   1,  0xFF,    0x99,    NEXT)
OPDEF(CEE_LDELEM_REF,                 "ldelem.ref",       PopRef + PopI,        PushRef,     InlineNone,         IObjModel,   1,  0xFF,    0x9A,    NEXT)
OPDEF(CEE_STELEM_I,                   "stelem.i",         PopRef + PopI + PopI,   Push0,       InlineNone,         IObjModel,   1,  0xFF,    0x9B,    NEXT)
OPDEF(CEE_STELEM_I1,                  "stelem.i1",        PopRef + PopI + PopI,   Push0,       InlineNone,         IObjModel,   1,  0xFF,    0x9C,    NEXT)
OPDEF(CEE_STELEM_I2,                  "stelem.i2",        PopRef + PopI + PopI,   Push0,       InlineNone,         IObjModel,   1,  0xFF,    0x9D,    NEXT)
OPDEF(CEE_STELEM_I4,                  "stelem.i4",        PopRef + PopI + PopI,   Push0,       InlineNone,         IObjModel,   1,  0xFF,    0x9E,    NEXT)
OPDEF(CEE_STELEM_I8,                  "stelem.i8",        PopRef + PopI + PopI8,  Push0,       InlineNone,         IObjModel,   1,  0xFF,    0x9F,    NEXT)
OPDEF(CEE_STELEM_R4,                  "stelem.r4",        PopRef + PopI + PopR4,  Push0,       InlineNone,         IObjModel,   1,  0xFF,    0xA0,    NEXT)
OPDEF(CEE_STELEM_R8,                  "stelem.r8",        PopRef + PopI + PopR8,  Push0,       InlineNone,         IObjModel,   1,  0xFF,    0xA1,    NEXT)
OPDEF(CEE_STELEM_REF,                 "stelem.ref",       PopRef + PopI + PopRef, Push0,       InlineNone,         IObjModel,   1,  0xFF,    0xA2,    NEXT)
OPDEF(CEE_LDELEM,                     "ldelem",           PopRef + PopI,        Push1,       InlineType,         IObjModel,   1,  0xFF,    0xA3,    NEXT)
OPDEF(CEE_STELEM,                     "stelem",           PopRef + PopI + Pop1,   Push0,       InlineType,         IObjModel,   1,  0xFF,    0xA4,    NEXT)
OPDEF(CEE_UNBOX_ANY,                  "unbox.any",        PopRef,             Push1,       InlineType,         IObjModel,   1,  0xFF,    0xA5,    NEXT)
OPDEF(CEE_UNUSED5,                    "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xA6,    NEXT)
OPDEF(CEE_UNUSED6,                    "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xA7,    NEXT)
OPDEF(CEE_UNUSED7,                    "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xA8,    NEXT)
OPDEF(CEE_UNUSED8,                    "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xA9,    NEXT)
OPDEF(CEE_UNUSED9,                    "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xAA,    NEXT)
OPDEF(CEE_UNUSED10,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xAB,    NEXT)
OPDEF(CEE_UNUSED11,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xAC,    NEXT)
OPDEF(CEE_UNUSED12,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xAD,    NEXT)
OPDEF(CEE_UNUSED13,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xAE,    NEXT)
OPDEF(CEE_UNUSED14,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xAF,    NEXT)
OPDEF(CEE_UNUSED15,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xB0,    NEXT)
OPDEF(CEE_UNUSED16,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xB1,    NEXT)
OPDEF(CEE_UNUSED17,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xB2,    NEXT)
OPDEF(CEE_CONV_OVF_I1,                "conv.ovf.i1",      Pop1,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0xB3,    NEXT)
OPDEF(CEE_CONV_OVF_U1,                "conv.ovf.u1",      Pop1,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0xB4,    NEXT)
OPDEF(CEE_CONV_OVF_I2,                "conv.ovf.i2",      Pop1,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0xB5,    NEXT)
OPDEF(CEE_CONV_OVF_U2,                "conv.ovf.u2",      Pop1,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0xB6,    NEXT)
OPDEF(CEE_CONV_OVF_I4,                "conv.ovf.i4",      Pop1,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0xB7,    NEXT)
OPDEF(CEE_CONV_OVF_U4,                "conv.ovf.u4",      Pop1,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0xB8,    NEXT)
OPDEF(CEE_CONV_OVF_I8,                "conv.ovf.i8",      Pop1,               PushI8,      InlineNone,         IPrimitive,  1,  0xFF,    0xB9,    NEXT)
OPDEF(CEE_CONV_OVF_U8,                "conv.ovf.u8",      Pop1,               PushI8,      InlineNone,         IPrimitive,  1,  0xFF,    0xBA,    NEXT)
OPDEF(CEE_UNUSED50,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xBB,    NEXT)
OPDEF(CEE_UNUSED18,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xBC,    NEXT)
OPDEF(CEE_UNUSED19,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xBD,    NEXT)
OPDEF(CEE_UNUSED20,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xBE,    NEXT)
OPDEF(CEE_UNUSED21,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xBF,    NEXT)
OPDEF(CEE_UNUSED22,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xC0,    NEXT)
OPDEF(CEE_UNUSED23,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xC1,    NEXT)
OPDEF(CEE_REFANYVAL,                  "refanyval",        Pop1,               PushI,       InlineType,         IPrimitive,  1,  0xFF,    0xC2,    NEXT)
OPDEF(CEE_CKFINITE,                   "ckfinite",         Pop1,               PushR8,      InlineNone,         IPrimitive,  1,  0xFF,    0xC3,    NEXT)
OPDEF(CEE_UNUSED24,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xC4,    NEXT)
OPDEF(CEE_UNUSED25,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xC5,    NEXT)
OPDEF(CEE_MKREFANY,                   "mkrefany",         PopI,               Push1,       InlineType,         IPrimitive,  1,  0xFF,    0xC6,    NEXT)
OPDEF(CEE_UNUSED59,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xC7,    NEXT)
OPDEF(CEE_UNUSED60,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xC8,    NEXT)
OPDEF(CEE_UNUSED61,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xC9,    NEXT)
OPDEF(CEE_UNUSED62,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xCA,    NEXT)
OPDEF(CEE_UNUSED63,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xCB,    NEXT)
OPDEF(CEE_UNUSED64,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xCC,    NEXT)
OPDEF(CEE_UNUSED65,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xCD,    NEXT)
OPDEF(CEE_UNUSED66,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xCE,    NEXT)
OPDEF(CEE_UNUSED67,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xCF,    NEXT)
OPDEF(CEE_LDTOKEN,                    "ldtoken",          Pop0,               PushI,       InlineTok,          IPrimitive,  1,  0xFF,    0xD0,    NEXT)
OPDEF(CEE_CONV_U2,                    "conv.u2",          Pop1,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0xD1,    NEXT)
OPDEF(CEE_CONV_U1,                    "conv.u1",          Pop1,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0xD2,    NEXT)
OPDEF(CEE_CONV_I,                     "conv.i",           Pop1,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0xD3,    NEXT)
OPDEF(CEE_CONV_OVF_I,                 "conv.ovf.i",       Pop1,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0xD4,    NEXT)
OPDEF(CEE_CONV_OVF_U,                 "conv.ovf.u",       Pop1,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0xD5,    NEXT)
OPDEF(CEE_ADD_OVF,                    "add.ovf",          Pop1 + Pop1,          Push1,       InlineNone,         IPrimitive,  1,  0xFF,    0xD6,    NEXT)
OPDEF(CEE_ADD_OVF_UN,                 "add.ovf.un",       Pop1 + Pop1,          Push1,       InlineNone,         IPrimitive,  1,  0xFF,    0xD7,    NEXT)
OPDEF(CEE_MUL_OVF,                    "mul.ovf",          Pop1 + Pop1,          Push1,       InlineNone,         IPrimitive,  1,  0xFF,    0xD8,    NEXT)
OPDEF(CEE_MUL_OVF_UN,                 "mul.ovf.un",       Pop1 + Pop1,          Push1,       InlineNone,         IPrimitive,  1,  0xFF,    0xD9,    NEXT)
OPDEF(CEE_SUB_OVF,                    "sub.ovf",          Pop1 + Pop1,          Push1,       InlineNone,         IPrimitive,  1,  0xFF,    0xDA,    NEXT)
OPDEF(CEE_SUB_OVF_UN,                 "sub.ovf.un",       Pop1 + Pop1,          Push1,       InlineNone,         IPrimitive,  1,  0xFF,    0xDB,    NEXT)
OPDEF(CEE_ENDFINALLY,                 "endfinally",       Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xDC,    RETURN)
OPDEF(CEE_LEAVE,                      "leave",            Pop0,               Push0,       InlineBrTarget,     IPrimitive,  1,  0xFF,    0xDD,    BRANCH)
OPDEF(CEE_LEAVE_S,                    "leave.s",          Pop0,               Push0,       ShortInlineBrTarget,IPrimitive,  1,  0xFF,    0xDE,    BRANCH)
OPDEF(CEE_STIND_I,                    "stind.i",          PopI + PopI,          Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xDF,    NEXT)
OPDEF(CEE_CONV_U,                     "conv.u",           Pop1,               PushI,       InlineNone,         IPrimitive,  1,  0xFF,    0xE0,    NEXT)
OPDEF(CEE_UNUSED26,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xE1,    NEXT)
OPDEF(CEE_UNUSED27,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xE2,    NEXT)
OPDEF(CEE_UNUSED28,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xE3,    NEXT)
OPDEF(CEE_UNUSED29,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xE4,    NEXT)
OPDEF(CEE_UNUSED30,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xE5,    NEXT)
OPDEF(CEE_UNUSED31,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xE6,    NEXT)
OPDEF(CEE_UNUSED32,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xE7,    NEXT)
OPDEF(CEE_UNUSED33,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xE8,    NEXT)
OPDEF(CEE_UNUSED34,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xE9,    NEXT)
OPDEF(CEE_UNUSED35,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xEA,    NEXT)
OPDEF(CEE_UNUSED36,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xEB,    NEXT)
OPDEF(CEE_UNUSED37,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xEC,    NEXT)
OPDEF(CEE_UNUSED38,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xED,    NEXT)
OPDEF(CEE_UNUSED39,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xEE,    NEXT)
OPDEF(CEE_UNUSED40,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xEF,    NEXT)
OPDEF(CEE_UNUSED41,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xF0,    NEXT)
OPDEF(CEE_UNUSED42,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xF1,    NEXT)
OPDEF(CEE_UNUSED43,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xF2,    NEXT)
OPDEF(CEE_UNUSED44,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xF3,    NEXT)
OPDEF(CEE_UNUSED45,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xF4,    NEXT)
OPDEF(CEE_UNUSED46,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xF5,    NEXT)
OPDEF(CEE_UNUSED47,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xF6,    NEXT)
OPDEF(CEE_UNUSED48,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  1,  0xFF,    0xF7,    NEXT)
OPDEF(CEE_PREFIX7,                    "prefix7",          Pop0,               Push0,       InlineNone,         IInternal,   1,  0xFF,    0xF8,    META)
OPDEF(CEE_PREFIX6,                    "prefix6",          Pop0,               Push0,       InlineNone,         IInternal,   1,  0xFF,    0xF9,    META)
OPDEF(CEE_PREFIX5,                    "prefix5",          Pop0,               Push0,       InlineNone,         IInternal,   1,  0xFF,    0xFA,    META)
OPDEF(CEE_PREFIX4,                    "prefix4",          Pop0,               Push0,       InlineNone,         IInternal,   1,  0xFF,    0xFB,    META)
OPDEF(CEE_PREFIX3,                    "prefix3",          Pop0,               Push0,       InlineNone,         IInternal,   1,  0xFF,    0xFC,    META)
OPDEF(CEE_PREFIX2,                    "prefix2",          Pop0,               Push0,       InlineNone,         IInternal,   1,  0xFF,    0xFD,    META)
OPDEF(CEE_PREFIX1,                    "prefix1",          Pop0,               Push0,       InlineNone,         IInternal,   1,  0xFF,    0xFE,    META)
OPDEF(CEE_PREFIXREF,                  "prefixref",        Pop0,               Push0,       InlineNone,         IInternal,   1,  0xFF,    0xFF,    META)

OPDEF(CEE_ARGLIST,                    "arglist",          Pop0,               PushI,       InlineNone,         IPrimitive,  2,  0xFE,    0x00,    NEXT)
OPDEF(CEE_CEQ,                        "ceq",              Pop1 + Pop1,          PushI,       InlineNone,         IPrimitive,  2,  0xFE,    0x01,    NEXT)
OPDEF(CEE_CGT,                        "cgt",              Pop1 + Pop1,          PushI,       InlineNone,         IPrimitive,  2,  0xFE,    0x02,    NEXT)
OPDEF(CEE_CGT_UN,                     "cgt.un",           Pop1 + Pop1,          PushI,       InlineNone,         IPrimitive,  2,  0xFE,    0x03,    NEXT)
OPDEF(CEE_CLT,                        "clt",              Pop1 + Pop1,          PushI,       InlineNone,         IPrimitive,  2,  0xFE,    0x04,    NEXT)
OPDEF(CEE_CLT_UN,                     "clt.un",           Pop1 + Pop1,          PushI,       InlineNone,         IPrimitive,  2,  0xFE,    0x05,    NEXT)
OPDEF(CEE_LDFTN,                      "ldftn",            Pop0,               PushI,       InlineMethod,       IPrimitive,  2,  0xFE,    0x06,    NEXT)
OPDEF(CEE_LDVIRTFTN,                  "ldvirtftn",        PopRef,             PushI,       InlineMethod,       IPrimitive,  2,  0xFE,    0x07,    NEXT)
OPDEF(CEE_UNUSED56,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  2,  0xFE,    0x08,    NEXT)
OPDEF(CEE_LDARG,                      "ldarg",            Pop0,               Push1,       InlineVar,          IPrimitive,  2,  0xFE,    0x09,    NEXT)
OPDEF(CEE_LDARGA,                     "ldarga",           Pop0,               PushI,       InlineVar,          IPrimitive,  2,  0xFE,    0x0A,    NEXT)
OPDEF(CEE_STARG,                      "starg",            Pop1,               Push0,       InlineVar,          IPrimitive,  2,  0xFE,    0x0B,    NEXT)
OPDEF(CEE_LDLOC,                      "ldloc",            Pop0,               Push1,       InlineVar,          IPrimitive,  2,  0xFE,    0x0C,    NEXT)
OPDEF(CEE_LDLOCA,                     "ldloca",           Pop0,               PushI,       InlineVar,          IPrimitive,  2,  0xFE,    0x0D,    NEXT)
OPDEF(CEE_STLOC,                      "stloc",            Pop1,               Push0,       InlineVar,          IPrimitive,  2,  0xFE,    0x0E,    NEXT)
OPDEF(CEE_LOCALLOC,                   "localloc",         PopI,               PushI,       InlineNone,         IPrimitive,  2,  0xFE,    0x0F,    NEXT)
OPDEF(CEE_UNUSED57,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  2,  0xFE,    0x10,    NEXT)
OPDEF(CEE_ENDFILTER,                  "endfilter",        PopI,               Push0,       InlineNone,         IPrimitive,  2,  0xFE,    0x11,    RETURN)
OPDEF(CEE_UNALIGNED,                  "unaligned.",       Pop0,               Push0,       ShortInlineI,       IPrefix,     2,  0xFE,    0x12,    META)
OPDEF(CEE_VOLATILE,                   "volatile.",        Pop0,               Push0,       InlineNone,         IPrefix,     2,  0xFE,    0x13,    META)
OPDEF(CEE_TAILCALL,                   "tail.",            Pop0,               Push0,       InlineNone,         IPrefix,     2,  0xFE,    0x14,    META)
OPDEF(CEE_INITOBJ,                    "initobj",          PopI,               Push0,       InlineType,         IObjModel,   2,  0xFE,    0x15,    NEXT)
OPDEF(CEE_CONSTRAINED,                "constrained.",     Pop0,               Push0,       InlineType,         IPrefix,     2,  0xFE,    0x16,    META)
OPDEF(CEE_CPBLK,                      "cpblk",            PopI + PopI + PopI,     Push0,       InlineNone,         IPrimitive,  2,  0xFE,    0x17,    NEXT)
OPDEF(CEE_INITBLK,                    "initblk",          PopI + PopI + PopI,     Push0,       InlineNone,         IPrimitive,  2,  0xFE,    0x18,    NEXT)
OPDEF(CEE_UNUSED69,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  2,  0xFE,    0x19,    NEXT)
OPDEF(CEE_RETHROW,                    "rethrow",          Pop0,               Push0,       InlineNone,         IObjModel,   2,  0xFE,    0x1A,    THROW)
OPDEF(CEE_UNUSED51,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  2,  0xFE,    0x1B,    NEXT)
OPDEF(CEE_SIZEOF,                     "sizeof",           Pop0,               PushI,       InlineType,         IPrimitive,  2,  0xFE,    0x1C,    NEXT)
OPDEF(CEE_REFANYTYPE,                 "refanytype",       Pop1,               PushI,       InlineNone,         IPrimitive,  2,  0xFE,    0x1D,    NEXT)
OPDEF(CEE_READONLY,                   "readonly.",        Pop0,               Push0,       InlineNone,         IPrefix,     2,  0xFE,    0x1E,    META)
OPDEF(CEE_UNUSED53,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  2,  0xFE,    0x1F,    NEXT)
OPDEF(CEE_UNUSED54,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  2,  0xFE,    0x20,    NEXT)
OPDEF(CEE_UNUSED55,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  2,  0xFE,    0x21,    NEXT)
OPDEF(CEE_UNUSED70,                   "unused",           Pop0,               Push0,       InlineNone,         IPrimitive,  2,  0xFE,    0x22,    NEXT)

// These are not real opcodes, but they are handy internally in the EE
#ifndef OPDEF_REAL_OPCODES_ONLY
OPDEF(CEE_ILLEGAL,                    "illegal",          Pop0,               Push0,       InlineNone,         IInternal,   0,  MOOT,    MOOT,    META)
OPDEF(CEE_MACRO_END,                  "endmac",           Pop0,               Push0,       InlineNone,         IInternal,   0,  MOOT,    MOOT,    META)
OPDEF(CEE_CODE_LABEL,                 "codelabel",        Pop0,               Push0,       InlineNone,         IInternal,   0,  MOOT,    MOOT,    META)
#undef OPDEF
};
#endif

void dumpILBytes(const BYTE* const codeAddr,
    unsigned          codeSize,
    unsigned          alignSize) // number of characters to write, for alignment
{
    for (IL_OFFSET offs = 0; offs < codeSize; ++offs)
    {
        printf(" %02x", *(codeAddr + offs));
    }

    unsigned charsWritten = 3 * codeSize;
    for (unsigned i = charsWritten; i < alignSize; i++)
    {
        printf(" ");
    }
}

inline signed __int8 getI1LittleEndian(const BYTE* ptr)
{
    return *(__unaligned signed __int8*)ptr;
}
typedef unsigned short      UINT16, * PUINT16;
inline unsigned __int16 getU2LittleEndian(const BYTE* ptr)
{
    return (*(UINT16 *)(ptr));
}
class FloatingPointUtils
{
public:
    static double convertUInt64ToDouble(unsigned __int64 u64);

    static float convertUInt64ToFloat(unsigned __int64 u64);

    static unsigned __int64 convertDoubleToUInt64(double d);

    static double convertToDouble(float f);

    static float convertToSingle(double d);

    static double round(double x);

    static float round(float x);

    static bool isNormal(double x);

    static bool isNormal(float x);

    static bool hasPreciseReciprocal(double x);

    static bool hasPreciseReciprocal(float x);

    static double infinite_double();

    static float infinite_float();

    static bool isAllBitsSet(float val);

    static bool isAllBitsSet(double val);

    static bool isFinite(float val);

    static bool isFinite(double val);

    static bool isNegative(float val);

    static bool isNegative(double val);

    static bool isNaN(float val);

    static bool isNaN(double val);

    static bool isNegativeZero(double val);

    static bool isPositiveZero(double val);

    static double maximum(double val1, double val2);

    static double maximumMagnitude(double val1, double val2);

    static double maximumMagnitudeNumber(double val1, double val2);

    static double maximumNumber(double val1, double val2);

    static float maximum(float val1, float val2);

    static float maximumMagnitude(float val1, float val2);

    static float maximumMagnitudeNumber(float val1, float val2);

    static float maximumNumber(float val1, float val2);

    static double minimum(double val1, double val2);

    static double minimumMagnitude(double val1, double val2);

    static double minimumMagnitudeNumber(double val1, double val2);

    static double minimumNumber(double val1, double val2);

    static float minimum(float val1, float val2);

    static float minimumMagnitude(float val1, float val2);

    static float minimumMagnitudeNumber(float val1, float val2);

    static float minimumNumber(float val1, float val2);

    static double normalize(double x);

    static int ilogb(double x);

    static int ilogb(float f);
};


#define GET_UNALIGNED_32(_pObject)  (*(UINT32 UNALIGNED *)(_pObject))
#define GET_UNALIGNED_VAL32(_pObject) VAL32(GET_UNALIGNED_32(_pObject))
inline signed __int32 getI4LittleEndian(const BYTE* ptr)
{
    return (*(UINT32 __unaligned*)(ptr));
}
inline unsigned __int32 getU4LittleEndian(const BYTE* ptr)
{
    return (*(UINT32 __unaligned*)(ptr));
}
inline float getR4LittleEndian(const BYTE* ptr)
{
    __int32 val = getI4LittleEndian(ptr);
    return *(float*)&val;
}
inline signed __int64 getI8LittleEndian(const BYTE* ptr)
{
    return (*(UINT64 __unaligned*)(ptr));
}
inline double getR8LittleEndian(const BYTE* ptr)
{
    __int64 val = getI8LittleEndian(ptr);
    return *(double*)&val;
}
double FloatingPointUtils::convertToDouble(float f)
{
#ifdef HOST_RISCV64
    if (f == f)
    {
        return f;
    }

    uint32_t bits = BitOperations::SingleToUInt32Bits(f);
    uint32_t payload = bits & ((1u << 23) - 1);
    uint64_t newBits = ((uint64_t)(bits >> 31) << 63) | 0x7FF8000000000000ul | ((uint64_t)payload << 29);
    return BitOperations::UInt64BitsToDouble(newBits);
#else
    return f;
#endif
}
unsigned dumpSingleInstr(const BYTE* const codeAddr, IL_OFFSET offs, const char* prefix)
{
    const BYTE* opcodePtr = codeAddr + offs;
    const BYTE* startOpcodePtr = opcodePtr;
    const unsigned ALIGN_WIDTH = 3 * 6; // assume 3 characters * (1 byte opcode + 4 bytes data + 1 prefix byte) for
    // most things

    if (prefix != nullptr)
    {
        printf("%s", prefix);
    }

    OPCODE opcode = (OPCODE)getU1LittleEndian(opcodePtr);
    opcodePtr += sizeof(__int8);

DECODE_OPCODE:

    if (opcode >= CEE_COUNT)
    {
        printf("\nIllegal opcode: %02X\n", (int)opcode);
        return (IL_OFFSET)(opcodePtr - startOpcodePtr);
    }

    /* Get the size of additional parameters */

    size_t   sz = opcodeSizes[opcode];
    unsigned argKind = opcodeArgKinds[opcode];

    /* See what kind of an opcode we have, then */

    switch (opcode)
    {
    case CEE_PREFIX1:
        opcode = OPCODE(getU1LittleEndian(opcodePtr) + 256);
        opcodePtr += sizeof(__int8);
        goto DECODE_OPCODE;

    default:
    {
        __int64 iOp;
        double  dOp;
        int     jOp;
        DWORD   jOp2;

        switch (argKind)
        {
        case InlineNone:
            dumpILBytes(startOpcodePtr, (unsigned)(opcodePtr - startOpcodePtr), ALIGN_WIDTH);
            printf(" %-12s", opcodeNames[opcode]);
            break;

        case ShortInlineVar:
            iOp = getU1LittleEndian(opcodePtr);
            goto INT_OP;
        case ShortInlineI:
            iOp = getI1LittleEndian(opcodePtr);
            goto INT_OP;
        case InlineVar:
            iOp = getU2LittleEndian(opcodePtr);
            goto INT_OP;
        case InlineTok:
        case InlineMethod:
        case InlineField:
        case InlineType:
        case InlineString:
        case InlineSig:
        case InlineI:
            iOp = getI4LittleEndian(opcodePtr);
            goto INT_OP;
        case InlineI8:
            iOp = getU4LittleEndian(opcodePtr);
            iOp |= (__int64)getU4LittleEndian(opcodePtr + 4) << 32;
            goto INT_OP;

        INT_OP:
            dumpILBytes(startOpcodePtr, (unsigned)((opcodePtr - startOpcodePtr) + sz), ALIGN_WIDTH);
            printf(" %-12s 0x%X", opcodeNames[opcode], iOp);
            break;

        case ShortInlineR:
            dOp = FloatingPointUtils::convertToDouble(getR4LittleEndian(opcodePtr));
            goto FLT_OP;
        case InlineR:
            dOp = getR8LittleEndian(opcodePtr);
            goto FLT_OP;

        FLT_OP:
            dumpILBytes(startOpcodePtr, (unsigned)((opcodePtr - startOpcodePtr) + sz), ALIGN_WIDTH);
            printf(" %-12s %f", opcodeNames[opcode], dOp);
            break;

        case ShortInlineBrTarget:
            jOp = getI1LittleEndian(opcodePtr);
            goto JMP_OP;
        case InlineBrTarget:
            jOp = getI4LittleEndian(opcodePtr);
            goto JMP_OP;

        JMP_OP:
            dumpILBytes(startOpcodePtr, (unsigned)((opcodePtr - startOpcodePtr) + sz), ALIGN_WIDTH);
            printf(" %-12s %d (IL_%04x)", opcodeNames[opcode], jOp, (int)(opcodePtr + sz - codeAddr) + jOp);
            break;

        case InlineSwitch:
            jOp2 = getU4LittleEndian(opcodePtr);
            opcodePtr += 4;
            opcodePtr += jOp2 * 4; // Jump over the table
            dumpILBytes(startOpcodePtr, (unsigned)(opcodePtr - startOpcodePtr), ALIGN_WIDTH);
            printf(" %-12s", opcodeNames[opcode]);
            break;

        case InlinePhi:
            jOp2 = getU1LittleEndian(opcodePtr);
            opcodePtr += 1;
            opcodePtr += jOp2 * 2; // Jump over the table
            dumpILBytes(startOpcodePtr, (unsigned)(opcodePtr - startOpcodePtr), ALIGN_WIDTH);
            printf(" %-12s", opcodeNames[opcode]);
            break;

        default:
            printf("Bad argKind");
        }

        opcodePtr += sz;
        break;
    }
    }

    printf("\n");
    return (IL_OFFSET)(opcodePtr - startOpcodePtr);
}


extern "C" __declspec(dllexport) void dumpILRange(const BYTE* const codeAddr, unsigned codeSize) // in bytes
{
    for (IL_OFFSET offs = 0; offs < codeSize;)
    {
        char prefix[100];
        sprintf_s(prefix, ArrLen(prefix), "IL_%04x ", offs);
        unsigned codeBytesDumped = dumpSingleInstr(codeAddr, offs, prefix);
        offs += codeBytesDumped;
    }
}

int main(int argc, char** argv)
{
    const BYTE  bt[13] = { 0x0,0x72,0x01,0x00,0x00,0x70,0x28,0x0d,0x00,0x00,0x0a,0x00,0x2a };
    dumpILRange(bt,13);
    getchar();
}