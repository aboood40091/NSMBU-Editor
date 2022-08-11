#pragma once

#include <types.h>

// -----------------------------------------------------------------------------------------------------

// From WUT
#include <gx2/enum.h>

#define GX2_SHADER_ALIGNMENT GX2_SHADER_PROGRAM_ALIGNMENT

// -----------------------------------------------------------------------------------------------------

// From WUT
#include <gx2/draw.h>

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

inline void GX2Draw(GX2PrimitiveMode mode, uint32_t count)
{
    GX2DrawEx(mode, count, 0, 1);
}

#ifdef __cplusplus
}
#endif // __cplusplus

// -----------------------------------------------------------------------------------------------------

// From WUT
#include <gx2/enum.h>

#define GX2_COMPARE_NEVER    GX2_COMPARE_FUNC_NEVER
#define GX2_COMPARE_LESS     GX2_COMPARE_FUNC_LESS
#define GX2_COMPARE_EQUAL    GX2_COMPARE_FUNC_EQUAL
#define GX2_COMPARE_LEQUAL   GX2_COMPARE_FUNC_LEQUAL
#define GX2_COMPARE_GREATER  GX2_COMPARE_FUNC_GREATER
#define GX2_COMPARE_NOTEQUAL GX2_COMPARE_FUNC_NOT_EQUAL
#define GX2_COMPARE_GEQUAL   GX2_COMPARE_FUNC_GEQUAL
#define GX2_COMPARE_ALWAYS   GX2_COMPARE_FUNC_ALWAYS
#define GX2_COMPARE_FIRST    GX2_COMPARE_NEVER
#define GX2_COMPARE_LAST     GX2_COMPARE_ALWAYS

#define GX2_BLEND_ZERO                     GX2_BLEND_MODE_ZERO
#define GX2_BLEND_ONE                      GX2_BLEND_MODE_ONE
#define GX2_BLEND_SRC_COLOR                GX2_BLEND_MODE_SRC_COLOR
#define GX2_BLEND_ONE_MINUS_SRC_COLOR      GX2_BLEND_MODE_INV_SRC_COLOR
#define GX2_BLEND_SRC_ALPHA                GX2_BLEND_MODE_SRC_ALPHA
#define GX2_BLEND_ONE_MINUS_SRC_ALPHA      GX2_BLEND_MODE_INV_SRC_ALPHA
#define GX2_BLEND_DST_ALPHA                GX2_BLEND_MODE_DST_ALPHA
#define GX2_BLEND_ONE_MINUS_DST_ALPHA      GX2_BLEND_MODE_INV_DST_ALPHA
#define GX2_BLEND_DST_COLOR                GX2_BLEND_MODE_DST_COLOR
#define GX2_BLEND_ONE_MINUS_DST_COLOR      GX2_BLEND_MODE_INV_DST_COLOR
#define GX2_BLEND_SRC_ALPHA_SATURATE       GX2_BLEND_MODE_SRC_ALPHA_SAT
#define GX2_BLEND_CONSTANT_COLOR           GX2_BLEND_MODE_BLEND_FACTOR
#define GX2_BLEND_ONE_MINUS_CONSTANT_COLOR GX2_BLEND_MODE_INV_BLEND_FACTOR
#define GX2_BLEND_SRC1_COLOR               GX2_BLEND_MODE_SRC1_COLOR
#define GX2_BLEND_ONE_MINUS_SRC1_COLOR     GX2_BLEND_MODE_INV_SRC1_COLOR
#define GX2_BLEND_SRC1_ALPHA               GX2_BLEND_MODE_SRC1_ALPHA
#define GX2_BLEND_ONE_MINUS_SRC1_ALPHA     GX2_BLEND_MODE_INV_SRC1_ALPHA
#define GX2_BLEND_CONSTANT_ALPHA           ((GX2BlendMode)19)
#define GX2_BLEND_ONE_MINUS_CONSTANT_ALPHA ((GX2BlendMode)20)
#define GX2_BLEND_FIRST                    GX2_BLEND_ZERO
#define GX2_BLEND_LAST                     GX2_BLEND_ONE_MINUS_CONSTANT_ALPHA
typedef GX2BlendMode GX2BlendFunction;

#define GX2_BLEND_COMBINE_ADD           GX2_BLEND_COMBINE_MODE_ADD
#define GX2_BLEND_COMBINE_SRC_MINUS_DST GX2_BLEND_COMBINE_MODE_SUB
#define GX2_BLEND_COMBINE_MIN           GX2_BLEND_COMBINE_MODE_SUB
#define GX2_BLEND_COMBINE_MAX           GX2_BLEND_COMBINE_MODE_MIN
#define GX2_BLEND_COMBINE_DST_MINUS_SRC GX2_BLEND_COMBINE_MODE_REV_SUB
#define GX2_BLEND_COMBINE_FIRST         GX2_BLEND_COMBINE_ADD
#define GX2_BLEND_COMBINE_LAST          GX2_BLEND_COMBINE_DST_MINUS_SRC
typedef GX2BlendCombineMode GX2BlendCombine;

#define GX2_ATTRIB_FORMAT_8_UNORM                   GX2_ATTRIB_FORMAT_UNORM_8
#define GX2_ATTRIB_FORMAT_8_UINT                    GX2_ATTRIB_FORMAT_UINT_8
#define GX2_ATTRIB_FORMAT_8_SNORM                   GX2_ATTRIB_FORMAT_SNORM_8
#define GX2_ATTRIB_FORMAT_8_SINT                    GX2_ATTRIB_FORMAT_SINT_8
#define GX2_ATTRIB_FORMAT_8_UINT_TO_FLOAT           ((GX2AttribFormat)(GX2_ATTRIB_FLAG_SCALED | GX2_ATTRIB_TYPE_8))
#define GX2_ATTRIB_FORMAT_8_SINT_TO_FLOAT           ((GX2AttribFormat)(GX2_ATTRIB_FLAG_SCALED | GX2_ATTRIB_FLAG_SIGNED | GX2_ATTRIB_TYPE_8))
#define GX2_ATTRIB_FORMAT_4_4_UNORM                 GX2_ATTRIB_TYPE_4_4
#define GX2_ATTRIB_FORMAT_16_UNORM                  GX2_ATTRIB_TYPE_16
#define GX2_ATTRIB_FORMAT_16_UINT                   ((GX2AttribFormat)(GX2_ATTRIB_FLAG_INTEGER | GX2_ATTRIB_TYPE_16))
#define GX2_ATTRIB_FORMAT_16_SNORM                  ((GX2AttribFormat)(GX2_ATTRIB_FLAG_SIGNED | GX2_ATTRIB_TYPE_16))
#define GX2_ATTRIB_FORMAT_16_SINT                   ((GX2AttribFormat)(GX2_ATTRIB_FLAG_SIGNED | GX2_ATTRIB_FLAG_INTEGER | GX2_ATTRIB_TYPE_16))
#define GX2_ATTRIB_FORMAT_16_FLOAT                  ((GX2AttribFormat)(GX2_ATTRIB_FLAG_SCALED | GX2_ATTRIB_TYPE_16_FLOAT))
#define GX2_ATTRIB_FORMAT_16_UINT_TO_FLOAT          ((GX2AttribFormat)(GX2_ATTRIB_FLAG_SCALED | GX2_ATTRIB_TYPE_16))
#define GX2_ATTRIB_FORMAT_16_SINT_TO_FLOAT          ((GX2AttribFormat)(GX2_ATTRIB_FLAG_SCALED | GX2_ATTRIB_FLAG_SIGNED | GX2_ATTRIB_TYPE_16))
#define GX2_ATTRIB_FORMAT_8_8_UNORM                 GX2_ATTRIB_FORMAT_UNORM_8_8
#define GX2_ATTRIB_FORMAT_8_8_UINT                  GX2_ATTRIB_FORMAT_UINT_8_8
#define GX2_ATTRIB_FORMAT_8_8_SNORM                 GX2_ATTRIB_FORMAT_SNORM_8_8
#define GX2_ATTRIB_FORMAT_8_8_SINT                  GX2_ATTRIB_FORMAT_SINT_8_8
#define GX2_ATTRIB_FORMAT_8_8_UINT_TO_FLOAT         ((GX2AttribFormat)(GX2_ATTRIB_FLAG_SCALED | GX2_ATTRIB_TYPE_8_8))
#define GX2_ATTRIB_FORMAT_8_8_SINT_TO_FLOAT         ((GX2AttribFormat)(GX2_ATTRIB_FLAG_SCALED | GX2_ATTRIB_FLAG_SIGNED | GX2_ATTRIB_TYPE_8_8))
#define GX2_ATTRIB_FORMAT_32_UINT                   ((GX2AttribFormat)(GX2_ATTRIB_FLAG_INTEGER | GX2_ATTRIB_TYPE_32))
#define GX2_ATTRIB_FORMAT_32_SINT                   ((GX2AttribFormat)(GX2_ATTRIB_FLAG_SIGNED | GX2_ATTRIB_FLAG_INTEGER | GX2_ATTRIB_TYPE_32))
#define GX2_ATTRIB_FORMAT_32_FLOAT                  GX2_ATTRIB_FORMAT_FLOAT_32
#define GX2_ATTRIB_FORMAT_16_16_UNORM               GX2_ATTRIB_TYPE_16_16
#define GX2_ATTRIB_FORMAT_16_16_UINT                ((GX2AttribFormat)(GX2_ATTRIB_FLAG_INTEGER | GX2_ATTRIB_TYPE_16_16))
#define GX2_ATTRIB_FORMAT_16_16_SNORM               ((GX2AttribFormat)(GX2_ATTRIB_FLAG_SIGNED | GX2_ATTRIB_TYPE_16_16))
#define GX2_ATTRIB_FORMAT_16_16_SINT                ((GX2AttribFormat)(GX2_ATTRIB_FLAG_SIGNED | GX2_ATTRIB_FLAG_INTEGER | GX2_ATTRIB_TYPE_16_16))
#define GX2_ATTRIB_FORMAT_16_16_FLOAT               ((GX2AttribFormat)(GX2_ATTRIB_FLAG_SCALED | GX2_ATTRIB_TYPE_16_16_FLOAT))
#define GX2_ATTRIB_FORMAT_16_16_UINT_TO_FLOAT       ((GX2AttribFormat)(GX2_ATTRIB_FLAG_SCALED | GX2_ATTRIB_TYPE_16_16))
#define GX2_ATTRIB_FORMAT_16_16_SINT_TO_FLOAT       ((GX2AttribFormat)(GX2_ATTRIB_FLAG_SCALED | GX2_ATTRIB_FLAG_SIGNED | GX2_ATTRIB_TYPE_16_16))
#define GX2_ATTRIB_FORMAT_10_11_11_FLOAT            ((GX2AttribFormat)(GX2_ATTRIB_FLAG_SCALED | GX2_ATTRIB_TYPE_10_11_11_FLOAT))
#define GX2_ATTRIB_FORMAT_8_8_8_8_UNORM             GX2_ATTRIB_FORMAT_UNORM_8_8_8_8
#define GX2_ATTRIB_FORMAT_8_8_8_8_UINT              GX2_ATTRIB_FORMAT_UINT_8_8_8_8
#define GX2_ATTRIB_FORMAT_8_8_8_8_SNORM             GX2_ATTRIB_FORMAT_SNORM_8_8_8_8
#define GX2_ATTRIB_FORMAT_8_8_8_8_SINT              GX2_ATTRIB_FORMAT_SINT_8_8_8_8
#define GX2_ATTRIB_FORMAT_8_8_8_8_UINT_TO_FLOAT     ((GX2AttribFormat)(GX2_ATTRIB_FLAG_SCALED | GX2_ATTRIB_TYPE_8_8_8_8))
#define GX2_ATTRIB_FORMAT_8_8_8_8_SINT_TO_FLOAT     ((GX2AttribFormat)(GX2_ATTRIB_FLAG_SCALED | GX2_ATTRIB_FLAG_SIGNED | GX2_ATTRIB_TYPE_8_8_8_8))
#define GX2_ATTRIB_FORMAT_10_10_10_2_UNORM          GX2_ATTRIB_TYPE_10_10_10_2
#define GX2_ATTRIB_FORMAT_10_10_10_2_UINT           ((GX2AttribFormat)(GX2_ATTRIB_FLAG_INTEGER | GX2_ATTRIB_TYPE_10_10_10_2))
#define GX2_ATTRIB_FORMAT_10_10_10_2_SNORM          ((GX2AttribFormat)(GX2_ATTRIB_FLAG_SIGNED | GX2_ATTRIB_TYPE_10_10_10_2))
#define GX2_ATTRIB_FORMAT_10_10_10_2_SINT           ((GX2AttribFormat)(GX2_ATTRIB_FLAG_SIGNED | GX2_ATTRIB_FLAG_INTEGER | GX2_ATTRIB_TYPE_10_10_10_2))
#define GX2_ATTRIB_FORMAT_32_32_UINT                ((GX2AttribFormat)(GX2_ATTRIB_FLAG_INTEGER | GX2_ATTRIB_TYPE_32_32))
#define GX2_ATTRIB_FORMAT_32_32_SINT                ((GX2AttribFormat)(GX2_ATTRIB_FLAG_SIGNED | GX2_ATTRIB_FLAG_INTEGER | GX2_ATTRIB_TYPE_32_32))
#define GX2_ATTRIB_FORMAT_32_32_FLOAT               GX2_ATTRIB_FORMAT_FLOAT_32_32
#define GX2_ATTRIB_FORMAT_16_16_16_16_UNORM         GX2_ATTRIB_TYPE_16_16_16_16
#define GX2_ATTRIB_FORMAT_16_16_16_16_UINT          ((GX2AttribFormat)(GX2_ATTRIB_FLAG_INTEGER | GX2_ATTRIB_TYPE_16_16_16_16))
#define GX2_ATTRIB_FORMAT_16_16_16_16_SNORM         ((GX2AttribFormat)(GX2_ATTRIB_FLAG_SIGNED | GX2_ATTRIB_TYPE_16_16_16_16))
#define GX2_ATTRIB_FORMAT_16_16_16_16_SINT          ((GX2AttribFormat)(GX2_ATTRIB_FLAG_SIGNED | GX2_ATTRIB_FLAG_INTEGER | GX2_ATTRIB_TYPE_16_16_16_16))
#define GX2_ATTRIB_FORMAT_16_16_16_16_FLOAT         ((GX2AttribFormat)(GX2_ATTRIB_FLAG_SCALED | GX2_ATTRIB_TYPE_16_16_16_16_FLOAT))
#define GX2_ATTRIB_FORMAT_16_16_16_16_UINT_TO_FLOAT ((GX2AttribFormat)(GX2_ATTRIB_FLAG_SCALED | GX2_ATTRIB_TYPE_16_16_16_16))
#define GX2_ATTRIB_FORMAT_16_16_16_16_SINT_TO_FLOAT ((GX2AttribFormat)(GX2_ATTRIB_FLAG_SCALED | GX2_ATTRIB_FLAG_SIGNED | GX2_ATTRIB_TYPE_16_16_16_16))
#define GX2_ATTRIB_FORMAT_32_32_32_UINT             ((GX2AttribFormat)(GX2_ATTRIB_FLAG_INTEGER | GX2_ATTRIB_TYPE_32_32_32))
#define GX2_ATTRIB_FORMAT_32_32_32_SINT             ((GX2AttribFormat)(GX2_ATTRIB_FLAG_SIGNED | GX2_ATTRIB_FLAG_INTEGER | GX2_ATTRIB_TYPE_32_32_32))
#define GX2_ATTRIB_FORMAT_32_32_32_FLOAT            GX2_ATTRIB_FORMAT_FLOAT_32_32_32
#define GX2_ATTRIB_FORMAT_32_32_32_32_UINT          ((GX2AttribFormat)(GX2_ATTRIB_FLAG_INTEGER | GX2_ATTRIB_TYPE_32_32_32_32))
#define GX2_ATTRIB_FORMAT_32_32_32_32_SINT          ((GX2AttribFormat)(GX2_ATTRIB_FLAG_SIGNED | GX2_ATTRIB_FLAG_INTEGER | GX2_ATTRIB_TYPE_32_32_32_32))
#define GX2_ATTRIB_FORMAT_32_32_32_32_FLOAT         GX2_ATTRIB_FORMAT_FLOAT_32_32_32_32
#define GX2_ATTRIB_FORMAT_FIRST                     GX2_ATTRIB_FORMAT_8_UNORM
#define GX2_ATTRIB_FORMAT_LAST                      GX2_ATTRIB_FORMAT_16_16_16_16_SINT_TO_FLOAT

#define GX2_ATTRIB_INDEX_VERTEX_ID   GX2_ATTRIB_INDEX_PER_VERTEX
#define GX2_ATTRIB_INDEX_INSTANCE_ID GX2_ATTRIB_INDEX_PER_INSTANCE
#define GX2_ATTRIB_INDEX_FIRST       GX2_ATTRIB_INDEX_VERTEX_ID
#define GX2_ATTRIB_INDEX_LAST        GX2_ATTRIB_INDEX_INSTANCE_ID

#define GX2_INDEX_FORMAT_U16_LE GX2_INDEX_TYPE_U16_LE
#define GX2_INDEX_FORMAT_U32_LE GX2_INDEX_TYPE_U32_LE
#define GX2_INDEX_FORMAT_U16    GX2_INDEX_TYPE_U16
#define GX2_INDEX_FORMAT_U32    GX2_INDEX_TYPE_U32
#define GX2_INDEX_FORMAT_FIRST  GX2_INDEX_FORMAT_U16_LE
#define GX2_INDEX_FORMAT_LAST   GX2_INDEX_FORMAT_U32
typedef GX2IndexType GX2IndexFormat;

#define GX2_PRIMITIVE_POINTS                    GX2_PRIMITIVE_MODE_POINTS
#define GX2_PRIMITIVE_LINES                     GX2_PRIMITIVE_MODE_LINES
#define GX2_PRIMITIVE_LINE_STRIP                GX2_PRIMITIVE_MODE_LINE_STRIP
#define GX2_PRIMITIVE_TRIANGLES                 GX2_PRIMITIVE_MODE_TRIANGLES
#define GX2_PRIMITIVE_TRIANGLE_FAN              GX2_PRIMITIVE_MODE_TRIANGLE_FAN
#define GX2_PRIMITIVE_TRIANGLE_STRIP            GX2_PRIMITIVE_MODE_TRIANGLE_STRIP
#define GX2_PRIMITIVE_LINES_ADJACENCY           GX2_PRIMITIVE_MODE_LINES_ADJACENCY
#define GX2_PRIMITIVE_LINE_STRIP_ADJACENCY      GX2_PRIMITIVE_MODE_LINE_STRIP_ADJACENCY
#define GX2_PRIMITIVE_TRIANGLES_ADJACENCY       GX2_PRIMITIVE_MODE_TRIANGLES_ADJACENCY
#define GX2_PRIMITIVE_TRIANGLE_STRIP_ADJACENCY  GX2_PRIMITIVE_MODE_TRIANGLE_STRIP_ADJACENCY
#define GX2_PRIMITIVE_RECTS                     GX2_PRIMITIVE_MODE_RECTS
#define GX2_PRIMITIVE_LINE_LOOP                 GX2_PRIMITIVE_MODE_LINE_LOOP
#define GX2_PRIMITIVE_QUADS                     GX2_PRIMITIVE_MODE_QUADS
#define GX2_PRIMITIVE_QUAD_STRIP                GX2_PRIMITIVE_MODE_QUAD_STRIP
#define GX2_PRIMITIVE_TESSELLATE_LINES          ((GX2PrimitiveMode)(0x80 | GX2_PRIMITIVE_MODE_LINES))
#define GX2_PRIMITIVE_TESSELLATE_LINE_STRIP     ((GX2PrimitiveMode)(0x80 | GX2_PRIMITIVE_MODE_LINE_STRIP))
#define GX2_PRIMITIVE_TESSELLATE_TRIANGLES      ((GX2PrimitiveMode)(0x80 | GX2_PRIMITIVE_MODE_TRIANGLES))
#define GX2_PRIMITIVE_TESSELLATE_TRIANGLE_STRIP ((GX2PrimitiveMode)(0x80 | GX2_PRIMITIVE_MODE_TRIANGLE_STRIP))
#define GX2_PRIMITIVE_TESSELLATE_QUADS          ((GX2PrimitiveMode)(0x80 | GX2_PRIMITIVE_MODE_QUADS))
#define GX2_PRIMITIVE_TESSELLATE_QUAD_STRIP     ((GX2PrimitiveMode)(0x80 | GX2_PRIMITIVE_MODE_QUAD_STRIP))
#define GX2_PRIMITIVE_FIRST                     GX2_PRIMITIVE_POINTS
#define GX2_PRIMITIVE_LAST                      GX2_PRIMITIVE_TESSELLATE_QUAD_STRIP
typedef GX2PrimitiveMode GX2PrimitiveType;

#define GX2_SURFACE_FORMAT_TC_R8_UNORM              GX2_SURFACE_FORMAT_UNORM_R8
#define GX2_SURFACE_FORMAT_TC_R8_UINT               GX2_SURFACE_FORMAT_UINT_R8
#define GX2_SURFACE_FORMAT_TC_R8_SNORM              GX2_SURFACE_FORMAT_SNORM_R8
#define GX2_SURFACE_FORMAT_TC_R8_SINT               GX2_SURFACE_FORMAT_SINT_R8
#define GX2_SURFACE_FORMAT_T_R4_G4_UNORM            GX2_SURFACE_FORMAT_UNORM_R4_G4
#define GX2_SURFACE_FORMAT_TCD_R16_UNORM            GX2_SURFACE_FORMAT_UNORM_R16
#define GX2_SURFACE_FORMAT_TC_R16_UINT              GX2_SURFACE_FORMAT_UINT_R16
#define GX2_SURFACE_FORMAT_TC_R16_SNORM             GX2_SURFACE_FORMAT_SNORM_R16
#define GX2_SURFACE_FORMAT_TC_R16_SINT              GX2_SURFACE_FORMAT_SINT_R16
#define GX2_SURFACE_FORMAT_TC_R16_FLOAT             GX2_SURFACE_FORMAT_FLOAT_R16
#define GX2_SURFACE_FORMAT_TC_R8_G8_UNORM           GX2_SURFACE_FORMAT_UNORM_R8_G8
#define GX2_SURFACE_FORMAT_TC_R8_G8_UINT            GX2_SURFACE_FORMAT_UINT_R8_G8
#define GX2_SURFACE_FORMAT_TC_R8_G8_SNORM           GX2_SURFACE_FORMAT_SNORM_R8_G8
#define GX2_SURFACE_FORMAT_TC_R8_G8_SINT            GX2_SURFACE_FORMAT_SINT_R8_G8
#define GX2_SURFACE_FORMAT_TCS_R5_G6_B5_UNORM       GX2_SURFACE_FORMAT_UNORM_R5_G6_B5
#define GX2_SURFACE_FORMAT_TC_R5_G5_B5_A1_UNORM     GX2_SURFACE_FORMAT_UNORM_R5_G5_B5_A1
#define GX2_SURFACE_FORMAT_TC_R4_G4_B4_A4_UNORM     GX2_SURFACE_FORMAT_UNORM_R4_G4_B4_A4
#define GX2_SURFACE_FORMAT_TC_A1_B5_G5_R5_UNORM     GX2_SURFACE_FORMAT_UNORM_A1_B5_G5_R5
#define GX2_SURFACE_FORMAT_TC_R32_UINT              GX2_SURFACE_FORMAT_UINT_R32
#define GX2_SURFACE_FORMAT_TC_R32_SINT              GX2_SURFACE_FORMAT_SINT_R32
#define GX2_SURFACE_FORMAT_TCD_R32_FLOAT            GX2_SURFACE_FORMAT_FLOAT_R32
#define GX2_SURFACE_FORMAT_TC_R16_G16_UNORM         GX2_SURFACE_FORMAT_UNORM_R16_G16
#define GX2_SURFACE_FORMAT_TC_R16_G16_UINT          GX2_SURFACE_FORMAT_UINT_R16_G16
#define GX2_SURFACE_FORMAT_TC_R16_G16_SNORM         GX2_SURFACE_FORMAT_SNORM_R16_G16
#define GX2_SURFACE_FORMAT_TC_R16_G16_SINT          GX2_SURFACE_FORMAT_SINT_R16_G16
#define GX2_SURFACE_FORMAT_TC_R16_G16_FLOAT         GX2_SURFACE_FORMAT_FLOAT_R16_G16
#define GX2_SURFACE_FORMAT_D_D24_S8_UNORM           GX2_SURFACE_FORMAT_UNORM_R24_X8
#define GX2_SURFACE_FORMAT_T_R24_UNORM_X8           GX2_SURFACE_FORMAT_UNORM_R24_X8
#define GX2_SURFACE_FORMAT_T_X24_G8_UINT            GX2_SURFACE_FORMAT_UINT_X24_G8
#define GX2_SURFACE_FORMAT_D_D24_S8_FLOAT           GX2_SURFACE_FORMAT_FLOAT_D24_S8
#define GX2_SURFACE_FORMAT_TC_R11_G11_B10_FLOAT     GX2_SURFACE_FORMAT_FLOAT_R11_G11_B10
#define GX2_SURFACE_FORMAT_TCS_R10_G10_B10_A2_UNORM GX2_SURFACE_FORMAT_UNORM_R10_G10_B10_A2
#define GX2_SURFACE_FORMAT_TC_R10_G10_B10_A2_UINT   GX2_SURFACE_FORMAT_UINT_R10_G10_B10_A2
#define GX2_SURFACE_FORMAT_T_R10_G10_B10_A2_SNORM   GX2_SURFACE_FORMAT_SNORM_R10_G10_B10_A2
#define GX2_SURFACE_FORMAT_TC_R10_G10_B10_A2_SNORM  GX2_SURFACE_FORMAT_T_R10_G10_B10_A2_SNORM
#define GX2_SURFACE_FORMAT_TC_R10_G10_B10_A2_SINT   GX2_SURFACE_FORMAT_SINT_R10_G10_B10_A2
#define GX2_SURFACE_FORMAT_TCS_R8_G8_B8_A8_UNORM    GX2_SURFACE_FORMAT_UNORM_R8_G8_B8_A8
#define GX2_SURFACE_FORMAT_TC_R8_G8_B8_A8_UINT      GX2_SURFACE_FORMAT_UINT_R8_G8_B8_A8
#define GX2_SURFACE_FORMAT_TC_R8_G8_B8_A8_SNORM     GX2_SURFACE_FORMAT_SNORM_R8_G8_B8_A8
#define GX2_SURFACE_FORMAT_TC_R8_G8_B8_A8_SINT      GX2_SURFACE_FORMAT_SINT_R8_G8_B8_A8
#define GX2_SURFACE_FORMAT_TCS_R8_G8_B8_A8_SRGB     GX2_SURFACE_FORMAT_SRGB_R8_G8_B8_A8
#define GX2_SURFACE_FORMAT_TCS_A2_B10_G10_R10_UNORM GX2_SURFACE_FORMAT_UNORM_A2_B10_G10_R10
#define GX2_SURFACE_FORMAT_TC_A2_B10_G10_R10_UINT   GX2_SURFACE_FORMAT_UINT_A2_B10_G10_R10
#define GX2_SURFACE_FORMAT_D_D32_FLOAT_S8_UINT_X24  GX2_SURFACE_FORMAT_FLOAT_X8_X24
#define GX2_SURFACE_FORMAT_T_R32_FLOAT_X8_X24       GX2_SURFACE_FORMAT_FLOAT_X8_X24
#define GX2_SURFACE_FORMAT_T_X32_G8_UINT_X24        GX2_SURFACE_FORMAT_UINT_G8_X24
#define GX2_SURFACE_FORMAT_TC_R32_G32_UINT          GX2_SURFACE_FORMAT_UINT_R32_G32
#define GX2_SURFACE_FORMAT_TC_R32_G32_SINT          GX2_SURFACE_FORMAT_SINT_R32_G32
#define GX2_SURFACE_FORMAT_TC_R32_G32_FLOAT         GX2_SURFACE_FORMAT_FLOAT_R32_G32
#define GX2_SURFACE_FORMAT_TC_R16_G16_B16_A16_UNORM GX2_SURFACE_FORMAT_UNORM_R16_G16_B16_A16
#define GX2_SURFACE_FORMAT_TC_R16_G16_B16_A16_UINT  GX2_SURFACE_FORMAT_UINT_R16_G16_B16_A16
#define GX2_SURFACE_FORMAT_TC_R16_G16_B16_A16_SNORM GX2_SURFACE_FORMAT_SNORM_R16_G16_B16_A16
#define GX2_SURFACE_FORMAT_TC_R16_G16_B16_A16_SINT  GX2_SURFACE_FORMAT_SINT_R16_G16_B16_A16
#define GX2_SURFACE_FORMAT_TC_R16_G16_B16_A16_FLOAT GX2_SURFACE_FORMAT_FLOAT_R16_G16_B16_A16
#define GX2_SURFACE_FORMAT_TC_R32_G32_B32_A32_UINT  GX2_SURFACE_FORMAT_UINT_R32_G32_B32_A32
#define GX2_SURFACE_FORMAT_TC_R32_G32_B32_A32_SINT  GX2_SURFACE_FORMAT_SINT_R32_G32_B32_A32
#define GX2_SURFACE_FORMAT_TC_R32_G32_B32_A32_FLOAT GX2_SURFACE_FORMAT_FLOAT_R32_G32_B32_A32
#define GX2_SURFACE_FORMAT_T_BC1_UNORM              GX2_SURFACE_FORMAT_UNORM_BC1
#define GX2_SURFACE_FORMAT_T_BC1_SRGB               GX2_SURFACE_FORMAT_SRGB_BC1
#define GX2_SURFACE_FORMAT_T_BC2_UNORM              GX2_SURFACE_FORMAT_UNORM_BC2
#define GX2_SURFACE_FORMAT_T_BC2_SRGB               GX2_SURFACE_FORMAT_SRGB_BC2
#define GX2_SURFACE_FORMAT_T_BC3_UNORM              GX2_SURFACE_FORMAT_UNORM_BC3
#define GX2_SURFACE_FORMAT_T_BC3_SRGB               GX2_SURFACE_FORMAT_SRGB_BC3
#define GX2_SURFACE_FORMAT_T_BC4_UNORM              GX2_SURFACE_FORMAT_UNORM_BC4
#define GX2_SURFACE_FORMAT_T_BC4_SNORM              GX2_SURFACE_FORMAT_SNORM_BC4
#define GX2_SURFACE_FORMAT_T_BC5_UNORM              GX2_SURFACE_FORMAT_UNORM_BC5
#define GX2_SURFACE_FORMAT_T_BC5_SNORM              GX2_SURFACE_FORMAT_SNORM_BC5
#define GX2_SURFACE_FORMAT_T_NV12_UNORM             GX2_SURFACE_FORMAT_UNORM_NV12
#define GX2_SURFACE_FORMAT_FIRST                    GX2_SURFACE_FORMAT_TC_R8_UNORM
#define GX2_SURFACE_FORMAT_LAST                     ((GX2SurfaceFormat)0x0000083f)

#define GX2_SURFACE_DIM_1D            GX2_SURFACE_DIM_TEXTURE_1D
#define GX2_SURFACE_DIM_2D            GX2_SURFACE_DIM_TEXTURE_2D
#define GX2_SURFACE_DIM_3D            GX2_SURFACE_DIM_TEXTURE_3D
#define GX2_SURFACE_DIM_CUBE          GX2_SURFACE_DIM_TEXTURE_CUBE
#define GX2_SURFACE_DIM_1D_ARRAY      GX2_SURFACE_DIM_TEXTURE_1D_ARRAY
#define GX2_SURFACE_DIM_2D_ARRAY      GX2_SURFACE_DIM_TEXTURE_2D_ARRAY
#define GX2_SURFACE_DIM_2D_MSAA       GX2_SURFACE_DIM_TEXTURE_2D_MSAA
#define GX2_SURFACE_DIM_2D_MSAA_ARRAY GX2_SURFACE_DIM_TEXTURE_2D_MSAA_ARRAY
#define GX2_SURFACE_DIM_FIRST         GX2_SURFACE_DIM_1D
#define GX2_SURFACE_DIM_LAST          GX2_SURFACE_DIM_2D_MSAA_ARRAY

#define GX2_AA_MODE_1X    GX2_AA_MODE1X
#define GX2_AA_MODE_2X    GX2_AA_MODE2X
#define GX2_AA_MODE_4X    GX2_AA_MODE4X
#define GX2_AA_MODE_8X    ((GX2AAMode)0x003)
#define GX2_AA_MODE_FIRST GX2_AA_MODE_1X
#define GX2_AA_MODE_LAST  GX2_AA_MODE_8X

#define GX2_TEX_CLAMP_WRAP                    GX2_TEX_CLAMP_MODE_WRAP
#define GX2_TEX_CLAMP_MIRROR                  GX2_TEX_CLAMP_MODE_MIRROR
#define GX2_TEX_CLAMP_CLAMP                   GX2_TEX_CLAMP_MODE_CLAMP
#define GX2_TEX_CLAMP_MIRROR_ONCE             GX2_TEX_CLAMP_MODE_MIRROR_ONCE
#define GX2_TEX_CLAMP_CLAMP_HALF_BORDER       ((GX2TexClampMode)4)
#define GX2_TEX_CLAMP_MIRROR_ONCE_HALF_BORDER ((GX2TexClampMode)5)
#define GX2_TEX_CLAMP_CLAMP_BORDER            GX2_TEX_CLAMP_MODE_CLAMP_BORDER
#define GX2_TEX_CLAMP_MIRROR_ONCE_BORDER      ((GX2TexClampMode)7)
#define GX2_TEX_CLAMP_FIRST                   GX2_TEX_CLAMP_WRAP
#define GX2_TEX_CLAMP_LAST                    GX2_TEX_CLAMP_MIRROR_ONCE_BORDER
typedef GX2TexClampMode GX2TexClamp;

#define GX2_TEX_XY_FILTER_POINT    GX2_TEX_XY_FILTER_MODE_POINT
#define GX2_TEX_XY_FILTER_BILINEAR GX2_TEX_XY_FILTER_MODE_LINEAR
#define GX2_TEX_XY_FILTER_FIRST    GX2_TEX_XY_FILTER_POINT
#define GX2_TEX_XY_FILTER_LAST     GX2_TEX_XY_FILTER_BILINEAR
typedef GX2TexXYFilterMode GX2TexXYFilterType;

#define GX2_TEX_ANISO_1_TO_1  GX2_TEX_ANISO_RATIO_NONE
#define GX2_TEX_ANISO_2_TO_1  ((GX2TexAnisoRatio)1)
#define GX2_TEX_ANISO_4_TO_1  ((GX2TexAnisoRatio)2)
#define GX2_TEX_ANISO_8_TO_1  ((GX2TexAnisoRatio)3)
#define GX2_TEX_ANISO_16_TO_1 ((GX2TexAnisoRatio)4)
#define GX2_TEX_ANISO_FIRST   GX2_TEX_ANISO_1_TO_1
#define GX2_TEX_ANISO_LAST    GX2_TEX_ANISO_16_TO_1

#define GX2_INVALIDATE_ATTRIB_BUFFER       GX2_INVALIDATE_MODE_ATTRIBUTE_BUFFER
#define GX2_INVALIDATE_TEXTURE             GX2_INVALIDATE_MODE_TEXTURE
#define GX2_INVALIDATE_UNIFORM_BLOCK       GX2_INVALIDATE_MODE_UNIFORM_BLOCK
#define GX2_INVALIDATE_SHADER              GX2_INVALIDATE_MODE_SHADER
#define GX2_INVALIDATE_COLOR_BUFFER        GX2_INVALIDATE_MODE_COLOR_BUFFER
#define GX2_INVALIDATE_DEPTH_BUFFER        GX2_INVALIDATE_MODE_DEPTH_BUFFER
#define GX2_INVALIDATE_CPU                 GX2_INVALIDATE_MODE_CPU
#define GX2_INVALIDATE_STREAMOUT_BUFFER    GX2_INVALIDATE_MODE_STREAM_OUT_BUFFER
#define GX2_INVALIDATE_EXPORT_BUFFER       GX2_INVALIDATE_MODE_EXPORT_BUFFER
typedef GX2InvalidateMode GX2InvalidateType;
#define GX2_INVALIDATE_CPU_ATTRIB_BUFFER   GX2_INVALIDATE_MODE_CPU_ATTRIBUTE_BUFFER
#define GX2_INVALIDATE_CPU_TEXTURE         GX2_INVALIDATE_MODE_CPU_TEXTURE
#define GX2_INVALIDATE_CPU_UNIFORM_BLOCK   ((GX2InvalidateType)(GX2_INVALIDATE_UNIFORM_BLOCK   | GX2_INVALIDATE_CPU))
#define GX2_INVALIDATE_CPU_SHADER          GX2_INVALIDATE_MODE_CPU_SHADER
#define GX2_INVALIDATE_CONSTANT_BUFFER     GX2_INVALIDATE_MODE_UNIFORM_BLOCK
#define GX2_INVALIDATE_CPU_CONSTANT_BUFFER ((GX2InvalidateType)(GX2_INVALIDATE_CONSTANT_BUFFER | GX2_INVALIDATE_CPU))
#define GX2_INVALIDATE_FIRST               GX2_INVALIDATE_ATTRIB_BUFFER
#define GX2_INVALIDATE_LAST                ((GX2InvalidateType)0x1ff)

// -----------------------------------------------------------------------------------------------------

// From WUT
#include <gx2/displaylist.h>

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

inline void GX2BeginDisplayList(void* p_dl, uint32_t size)
{
    GX2BeginDisplayListEx(p_dl, size, GX2_ENABLE);
}

#ifdef __cplusplus
}
#endif // __cplusplus

// -----------------------------------------------------------------------------------------------------

// From WUT
#include <gx2/mem.h>

typedef uint32_t GX2CompSel;

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

inline void GX2EndianSwap(void* ptr, uint32_t size)
{
    uint32_t*       src     = (uint32_t*)ptr;
    const uint32_t  count   = size / sizeof(uint32_t);

    for (uint32_t i = 0; i < count; i++)
        src[i] = __builtin_bswap32(src[i]);
}

#ifdef __cplusplus
}
#endif // __cplusplus

// -----------------------------------------------------------------------------------------------------

// From WUT
#include <gx2/registers.h>

// -----------------------------------------------------------------------------------------------------

// From WUT
#include <gx2/shaders.h>

#define shaderPtr program
#define shaderSize size
#define copyShaderPtr vertexProgram
#define copyShaderSize vertexProgramSize
#define destSel mask
#define indexType type

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

inline void GX2SetShaders(const GX2FetchShader* fs,
                          const GX2VertexShader* vs,
                          const GX2PixelShader* ps)
{
    GX2SetFetchShader(fs);
    GX2SetVertexShader(vs);
    GX2SetPixelShader(ps);
}

inline void GX2SetShadersEx(const GX2FetchShader* fs,
                            const GX2VertexShader* vs,
                            const GX2GeometryShader* gs,
                            const GX2PixelShader* ps)
{
    GX2SetFetchShader(fs);
    GX2SetVertexShader(vs);
    GX2SetGeometryShader(gs);
    GX2SetPixelShader(ps);
}

inline void GX2InitAttribStream(GX2AttribStream* attribStream,
                                uint32_t location,
                                uint32_t buffer,
                                uint32_t offset,
                                GX2AttribFormat format)
{
    const GX2CompSel dstSel[20] = {
        0x00040405, 0x00010405,
        0x00040405, 0x00040405,
        0x00010405, 0x00040405,
        0x00040405, 0x00010405,
        0x00010405, 0x00010205,
        0x00010203, 0x00010203,
        0x00010405, 0x00010405,
        0x00010203, 0x00010203,
        0x00010205, 0x00010205,
        0x00010203, 0x00010203
    };

    attribStream->buffer        = buffer;
    attribStream->offset        = offset;
    attribStream->location      = location;
    attribStream->format        = format;
    attribStream->mask          = dstSel[format & 0xff];
    attribStream->indexType     = GX2_ATTRIB_INDEX_PER_VERTEX;
    attribStream->aluDivisor    = 0;
    attribStream->endianSwap    = GX2_ENDIAN_SWAP_DEFAULT;
}

inline GX2SamplerVar* GX2GetPixelSamplerVar(const GX2PixelShader* shader, const char* name)
{
    for (uint32_t i = 0; i < shader->samplerVarCount; i++)
       if (strcmp(shader->samplerVars[i].name, name) == 0)
           return &(shader->samplerVars[i]);

    return nullptr;
}

inline int32_t GX2GetPixelSamplerVarLocation(const GX2PixelShader* shader, const char* name)
{
    GX2SamplerVar* sampler = GX2GetPixelSamplerVar(shader, name);
    if (!sampler)
        return -1;

    return sampler->location;
}

inline GX2AttribVar* GX2GetVertexAttribVar(const GX2VertexShader* shader, const char* name)
{
    for (uint32_t i = 0; i < shader->attribVarCount; i++)
       if (strcmp(shader->attribVars[i].name, name) == 0)
           return &(shader->attribVars[i]);

    return nullptr;
}

inline int32_t GX2GetVertexAttribVarLocation(const GX2VertexShader* shader, const char* name)
{
    GX2AttribVar* attrib = GX2GetVertexAttribVar(shader, name);
    if (!attrib)
        return -1;

    return attrib->location;
}

inline uint32_t GX2CalcFetchShaderSize(uint32_t num_attrib)
{
    return GX2CalcFetchShaderSizeEx(num_attrib,
                                    GX2_FETCH_SHADER_TESSELLATION_NONE,
                                    GX2_TESSELLATION_MODE_DISCRETE);
}

inline void GX2InitFetchShader(GX2FetchShader* fs,
                               void* fs_buffer,
                               uint32_t count,
                               const GX2AttribStream* attribs)
{
    GX2InitFetchShaderEx(fs,
                         (uint8_t*)fs_buffer,
                         count,
                         attribs,
                         GX2_FETCH_SHADER_TESSELLATION_NONE,
                         GX2_TESSELLATION_MODE_DISCRETE);

}

#ifdef __cplusplus
}
#endif // __cplusplus

// -----------------------------------------------------------------------------------------------------

// From WUT
#include <gx2/surface.h>

#define numMips mipLevels
#define imagePtr image
#define mipPtr mipmaps
#define mipSize mipmapSize
#define mipOffset mipLevelOffset

// -----------------------------------------------------------------------------------------------------

// From WUT
#include <gx2/sampler.h>
#include <gx2/texture.h>

#define compSel compMap

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

inline void GX2InitTexture(GX2Texture* texture,
                           uint32_t width,
                           uint32_t height,
                           uint32_t depth,
                           uint32_t numMips,
                           GX2SurfaceFormat format,
                           GX2SurfaceDim dim)
{
    const GX2CompSel dstSel[54] = {
        0x04040405, 0x00040405, 0x00010405, 0x04040405,
        0x04040405, 0x00040405, 0x00040405, 0x00010405,
        0x00010205, 0x00010205, 0x00010203, 0x00010203,
        0x03020100, 0x00040405, 0x00040405, 0x00010405,
        0x00010405, 0x04040405, 0x04040405, 0x04040405,
        0x04040405, 0x04040405, 0x00010205, 0x04040405,
        0x04040405, 0x00010203, 0x00010203, 0x03020100,
        0x00010405, 0x00010405, 0x00010405, 0x00010203,
        0x00010203, 0x04040405, 0x00010203, 0x00010203,
        0x04040405, 0x04040405, 0x04040405, 0x00010205,
        0x00010205, 0x00040405, 0x00010405, 0x00010205,
        0x04040405, 0x04040405, 0x04040405, 0x00010205,
        0x00010205, 0x00010203, 0x00010203, 0x00010203,
        0x00040405, 0x00010405
    };

    texture->surface.dim        = dim;
    texture->surface.width      = width;
    texture->surface.height     = height;
    texture->surface.depth      = depth;
    texture->surface.numMips    = numMips;
    texture->surface.format     = format;
    texture->surface.aa         = GX2_AA_MODE_1X;
    texture->surface.use        = GX2_SURFACE_USE_TEXTURE;
    texture->surface.tileMode   = GX2_TILE_MODE_DEFAULT;
    texture->surface.swizzle    = 0;
    texture->viewFirstMip       = 0;
    texture->viewNumMips        = numMips;
    texture->viewFirstSlice     = 0;
    texture->viewNumSlices      = depth;
    texture->compSel            = dstSel[format & 0x3f];

    GX2CalcSurfaceSizeAndAlignment(&texture->surface);

    GX2InitTextureRegs(texture);
}

inline void GX2InitTexturePtrs(GX2Texture* texture, void* imagePtr, void* mipPtr)
{
    texture->surface.imagePtr = imagePtr;

    if (texture->surface.numMips > 1)
    {
        if (mipPtr)
            texture->surface.mipPtr = mipPtr;
        else
            texture->surface.mipPtr = (void*)((uint32_t)imagePtr + texture->surface.mipOffset[0]);
    }
    else
    {
        texture->surface.mipPtr = mipPtr;
    }
}

inline void GX2InitTextureCompSel(GX2Texture* texture, GX2CompSel compSel)
{
    texture->compSel = compSel;
}

#ifdef __cplusplus
}
#endif // __cplusplus

// -----------------------------------------------------------------------------------------------------
