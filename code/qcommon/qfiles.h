#ifndef __QFILES_H__
#define __QFILES_H__

//
// qfiles.h: quake file formats
// This file must be identical in the quake and utils directories
//

// surface geometry should not exceed these limits
#define SHADER_MAX_VERTEXES 1000
#define SHADER_MAX_INDEXES (6 * SHADER_MAX_VERTEXES)

// the maximum size of game reletive pathnames
#define MAX_QPATH 64

/*
========================================================================

QVM files

========================================================================
*/

#define VM_MAGIC 0x12721444
typedef struct
{
	int vmMagic;

	int instructionCount;

	int codeOffset;
	int codeLength;

	int dataOffset;
	int dataLength;
	int litLength; // ( dataLength - litLength ) should be byteswapped on load
	int bssLength; // zero filled memory appended to datalength
} vmHeader_t;

/*
========================================================================

PCX files are used for 8 bit images

========================================================================
*/

typedef struct
{
	char manufacturer;
	char version;
	char encoding;
	char bits_per_pixel;
	unsigned short xmin, ymin, xmax, ymax;
	unsigned short hres, vres;
	unsigned char palette[48];
	char reserved;
	char color_planes;
	unsigned short bytes_per_line;
	unsigned short palette_type;
	char filler[58];
	unsigned char data; // unbounded
} pcx_t;

/*
========================================================================

TGA files are used for 24/32 bit images

========================================================================
*/

typedef struct _TargaHeader
{
	unsigned char id_length, colormap_type, image_type;
	unsigned short colormap_index, colormap_length;
	unsigned char colormap_size;
	unsigned short x_origin, y_origin, width, height;
	unsigned char pixel_size, attributes;
} TargaHeader;

/*
========================================================================

.MD3 triangle model file format

========================================================================
*/

#define MD3_IDENT (('3' << 24) + ('P' << 16) + ('D' << 8) + 'I')
#define MD3_VERSION 15

// limits
#define MD3_MAX_LODS 3
#define MD3_MAX_TRIANGLES 8192	 // per surface
#define MD3_MAX_VERTS 4096		 // per surface
#define MD3_MAX_SHADERS 256		 // per surface
#define MD3_MAX_FRAMES 1024		 // per model
#define MD3_MAX_SURFACES 32 + 32 // per model
#define MD3_MAX_TAGS 16			 // per frame

// vertex scales
#define MD3_XYZ_SCALE (1.0 / 64)

typedef struct md3Frame_s
{
	vec3_t bounds[2];
	vec3_t localOrigin;
	float radius;
	char name[16];
} md3Frame_t;

typedef struct md3Tag_s
{
	char name[MAX_QPATH]; // tag name
	vec3_t origin;
	vec3_t axis[3];
} md3Tag_t;

/*
** md3Surface_t
**
** CHUNK			SIZE
** header			sizeof( md3Surface_t )
** shaders			sizeof( md3Shader_t ) * numShaders
** triangles[0]		sizeof( md3Triangle_t ) * numTriangles
** st				sizeof( md3St_t ) * numVerts
** XyzNormals		sizeof( md3XyzNormal_t ) * numVerts * numFrames
*/
typedef struct
{
	int ident; //

	char name[MAX_QPATH]; // polyset name

	int flags;
	int numFrames; // all surfaces in a model should have the same

	int numShaders; // all surfaces in a model should have the same
	int numVerts;

	int numTriangles;
	int ofsTriangles;

	int ofsShaders;	   // offset from start of md3Surface_t
	int ofsSt;		   // texture coords are common for all frames
	int ofsXyzNormals; // numVerts * numFrames

	int ofsEnd; // next surface follows
} md3Surface_t;

typedef struct
{
	char name[MAX_QPATH];
	int shaderIndex; // for in-game use
} md3Shader_t;

typedef struct
{
	int indexes[3];
} md3Triangle_t;

typedef struct
{
	float st[2];
} md3St_t;

typedef struct
{
	short xyz[3];
	short normal;
} md3XyzNormal_t;

typedef struct
{
	int ident;
	int version;

	char name[MAX_QPATH]; // model name

	int flags;

	int numFrames;
	int numTags;
	int numSurfaces;

	int numSkins;

	int ofsFrames;	 // offset for first frame
	int ofsTags;	 // numFrames * numTags
	int ofsSurfaces; // first surface, others follow

	int ofsEnd; // end of file
} md3Header_t;

/*
==============================================================================

  .BSP file format

==============================================================================
*/

#define BSP_IDENT (('P' << 24) + ('S' << 16) + ('B' << 8) + 'R')
// little-endian "IBSP"

#define BSP_VERSION 1

// there shouldn't be any problem with increasing these values at the
// expense of more memory allocation in the utilities
#define MAX_MAP_MODELS 0x400
#define MAX_MAP_BRUSHES 0x8000
#define MAX_MAP_ENTITIES 0x800
#define MAX_MAP_ENTSTRING 0x40000
#define MAX_MAP_SHADERS 0x400

#define MAX_MAP_AREAS 0x100 // MAX_MAP_AREA_BYTES in q_shared must match!
#define MAX_MAP_FOGS 0x100
#define MAX_MAP_PLANES 0x20000
#define MAX_MAP_NODES 0x20000
#define MAX_MAP_BRUSHSIDES 0x20000
#define MAX_MAP_LEAFS 0x20000
#define MAX_MAP_LEAFFACES 0x20000
#define MAX_MAP_LEAFBRUSHES 0x40000
#define MAX_MAP_PORTALS 0x20000
#define MAX_MAP_LIGHTING 0x800000
#define MAX_MAP_LIGHTGRID 65535
#define MAX_MAP_LIGHTGRID_ARRAY 0x100000

#define MAX_MAP_VISIBILITY 0x400000

#define MAX_MAP_DRAW_SURFS 0x20000
#define MAX_MAP_DRAW_VERTS 0x80000
#define MAX_MAP_DRAW_INDEXES 0x80000

// key / value pair sizes in the entities lump
#define MAX_KEY 32
#define MAX_VALUE 1024

// the editor uses these predefined yaw angles to orient entities up or down
#define ANGLE_UP -1
#define ANGLE_DOWN -2

#define LIGHTMAP_WIDTH 128
#define LIGHTMAP_HEIGHT 128

//=============================================================================

#ifdef _XBOX

#pragma pack(push, 1)

typedef struct
{
	float mins[3], maxs[3];
	int firstSurface;
	unsigned short numSurfaces;
	int firstBrush;
	unsigned short numBrushes;
} dmodel_t;

typedef struct
{
	char shader[MAX_QPATH];
	int surfaceFlags;
	int contentFlags;
} dshader_t;

// planes x^1 is allways the opposite of plane x

typedef struct
{
	float normal[3];
	float dist;
} dplane_t;

typedef struct
{
	int planeNum;
	short children[2]; // negative numbers are -(leafs+1), not nodes
	short mins[3];	   // for frustom culling
	short maxs[3];
} dnode_t;

typedef struct
{
	short cluster; // -1 = opaque cluster (do I still store these?)
	signed char area;

	short mins[3]; // for frustum culling
	short maxs[3];

	unsigned short firstLeafSurface;
	unsigned short numLeafSurfaces;

	unsigned short firstLeafBrush;
	unsigned short numLeafBrushes;
} dleaf_t;

typedef struct
{
	int planeNum; // positive plane side faces out of the leaf
	byte shaderNum;
} dbrushside_t;

typedef struct
{
	int firstSide;
	byte numSides;
	unsigned short shaderNum; // the shader that determines the contents flags
} dbrush_t;

typedef struct
{
	char shader[MAX_QPATH];
	int brushNum;
	int visibleSide; // the brush side that ray tests need to clip against (-1 == none)
} dfog_t;

// Light Style Constants
#define MAXLIGHTMAPS 4
#define LS_NORMAL 0x00
#define LS_UNUSED 0xfe
#define LS_NONE 0xff
#define MAX_LIGHT_STYLES 64

typedef struct
{
	float lightmap[MAXLIGHTMAPS][2];
	float st[2];
	short xyz[3];
	short normal[3];
	byte color[MAXLIGHTMAPS][4];
} mapVert_t;

#define DRAWVERT_LIGHTMAP_SCALE 32768.0f
// Change texture coordinates for TriSurfs to be even more fine grain.
// See below for note about keeping MIN_ST and MAX_ST up to date with
// ST_SCALE. These are in 4.12. Okay, how about 5.11? Gah. 9.7!
//#define DRAWVERT_ST_SCALE 4096.0f
//#define DRAWVERT_ST_SCALE 2048.0f
#define DRAWVERT_ST_SCALE 128.0f

// We use a slightly different format for the fixed point texture
// coords in Grid/Mesh drawverts: 10.6 rather than 12.4
// To be sure that this is ok, keep the max and min values equal to
// the largest and smallest whole numbers that can be stored using the
// format. (ie: Don't change GRID_DRAWVERT_ST_SCALE without changing
// the other two!) (And don't forget that we're using a bit for sign.)
#define GRID_DRAWVERT_ST_SCALE 64.0f

// This master switch controls whether we use compressed (4-bit per channel)
// vertex colors in draw and surface verts. It saves memory, but I'm switching
// it off, because we end up with that nasty green/purple streaking effect.
// If we ever figure out how to do it better... (1555? 565?)
//#define COMPRESS_VERTEX_COLORS

typedef struct
{
	short xyz[3];
	short dvst[2];
	short dvlightmap[MAXLIGHTMAPS][2];
	short normal[3];
#ifdef _XBOX
	vec3_t tangent;
#endif
#ifdef COMPRESS_VERTEX_COLORS
	byte dvcolor[MAXLIGHTMAPS][1];
#else
	byte dvcolor[MAXLIGHTMAPS][4];
#endif
} drawVert_t;

typedef struct
{
	byte flags;
	byte latLong[2];
} dgrid_t;

typedef struct
{
	int code;
	byte shaderNum;
	signed char fogNum;

	unsigned int verts;	  // high 20 bits are first vert, low 12 are num verts
	unsigned int indexes; // high 20 bits are first index, low 12 are num indices

	byte lightmapStyles[MAXLIGHTMAPS];
	byte lightmapNum[MAXLIGHTMAPS];

	short lightmapVecs[3];
} dface_t;

typedef struct
{
	int code;
	byte shaderNum;
	signed char fogNum;

	unsigned int verts; // high 20 bits are first vert, low 12 are num verts

	byte lightmapStyles[MAXLIGHTMAPS];
	byte lightmapNum[MAXLIGHTMAPS];

	short lightmapVecs[2][3]; // for patches, [0] and [1] are lodbounds

	byte patchWidth;
	byte patchHeight;
} dpatch_t;

typedef struct
{
	int code;
	byte shaderNum;
	signed char fogNum;

	unsigned int verts;	  // high 20 bits are first vert, low 12 are num verts
	unsigned int indexes; // high 20 bits are first index, low 12 are num indices

	byte lightmapStyles[MAXLIGHTMAPS];
} dtrisurf_t;

typedef struct
{
	int code;
	byte shaderNum;
	signed char fogNum;

	short origin[3];
	short normal[3];
	byte color[3];
} dflare_t;

#pragma pack(pop)

#else // _XBOX

typedef struct
{
	int fileofs, filelen;
} lump_t;

#define LUMP_ENTITIES 0
#define LUMP_SHADERS 1
#define LUMP_PLANES 2
#define LUMP_NODES 3
#define LUMP_LEAFS 4
#define LUMP_LEAFSURFACES 5
#define LUMP_LEAFBRUSHES 6
#define LUMP_MODELS 7
#define LUMP_BRUSHES 8
#define LUMP_BRUSHSIDES 9
#define LUMP_DRAWVERTS 10
#define LUMP_DRAWINDEXES 11
#define LUMP_FOGS 12
#define LUMP_SURFACES 13
#define LUMP_LIGHTMAPS 14
#define LUMP_LIGHTGRID 15
#define LUMP_VISIBILITY 16
#define LUMP_LIGHTARRAY 17
#define HEADER_LUMPS 18

typedef struct
{
	int ident;
	int version;

	lump_t lumps[HEADER_LUMPS];
} dheader_t;

typedef struct
{
	float mins[3], maxs[3];
	int firstSurface, numSurfaces;
	int firstBrush, numBrushes;
} dmodel_t;

typedef struct dshader_s
{
	char shader[MAX_QPATH];
	int surfaceFlags;
	int contentFlags;
} dshader_t;

// planes x^1 is allways the opposite of plane x

typedef struct
{
	float normal[3];
	float dist;
} dplane_t;

typedef struct
{
	int planeNum;
	int children[2]; // negative numbers are -(leafs+1), not nodes
	int mins[3];	 // for frustom culling
	int maxs[3];
} dnode_t;

typedef struct
{
	int cluster; // -1 = opaque cluster (do I still store these?)
	int area;

	int mins[3]; // for frustum culling
	int maxs[3];

	int firstLeafSurface;
	int numLeafSurfaces;

	int firstLeafBrush;
	int numLeafBrushes;
} dleaf_t;

typedef struct
{
	int planeNum; // positive plane side faces out of the leaf
	int shaderNum;
	int drawSurfNum;
} dbrushside_t;

typedef struct
{
	int firstSide;
	int numSides;
	int shaderNum; // the shader that determines the contents flags
} dbrush_t;

typedef struct
{
	char shader[MAX_QPATH];
	int brushNum;
	int visibleSide; // the brush side that ray tests need to clip against (-1 == none)
} dfog_t;

// Light Style Constants
#define MAXLIGHTMAPS 4
#define LS_NORMAL 0x00
#define LS_UNUSED 0xfe
#define LS_NONE 0xff
#define MAX_LIGHT_STYLES 64

typedef struct
{
	vec3_t xyz;
	float st[2];
	float lightmap[MAXLIGHTMAPS][2];
	vec3_t normal;
	byte color[MAXLIGHTMAPS][4];
} mapVert_t;

typedef struct
{
	vec3_t xyz;
	float st[2];
	float lightmap[MAXLIGHTMAPS][2];
	vec3_t normal;
	byte color[MAXLIGHTMAPS][4];
} drawVert_t;

typedef struct
{
	byte ambientLight[MAXLIGHTMAPS][3];
	byte directLight[MAXLIGHTMAPS][3];
	byte styles[MAXLIGHTMAPS];
	byte latLong[2];
} dgrid_t;

typedef enum
{
	MST_BAD,
	MST_PLANAR,
	MST_PATCH,
	MST_TRIANGLE_SOUP,
	MST_FLARE
} mapSurfaceType_t;

typedef struct
{
	int shaderNum;
	int fogNum;
	int surfaceType;

	int firstVert;
	int numVerts;

	int firstIndex;
	int numIndexes;

	byte lightmapStyles[MAXLIGHTMAPS], vertexStyles[MAXLIGHTMAPS];
	int lightmapNum[MAXLIGHTMAPS];
	int lightmapX[MAXLIGHTMAPS], lightmapY[MAXLIGHTMAPS];
	int lightmapWidth, lightmapHeight;

	vec3_t lightmapOrigin;
	vec3_t lightmapVecs[3]; // for patches, [0] and [1] are lodbounds

	int patchWidth;
	int patchHeight;
} dsurface_t;

#endif _XBOX

typedef enum //# hunkAllocType_e
{
	HA_MISC,
	HA_MAP,
	HA_SHADERS,
	HA_LIGHTING,
	HA_FOG,
	HA_PATCHES,
	HA_VIS,
	HA_SUBMODELS,
	HA_MODELS,
	MAX_HA_TYPES
} hunkAllocType_t;

/////////////////////////////////////////////////////////////
//
// Defines and structures required for fonts

#define GLYPH_COUNT 256

// Must match define in stmparse.h
#define STYLE_DROPSHADOW 0x80000000
#define STYLE_BLINK 0x40000000
#define SET_MASK 0x00ffffff

typedef struct
{
	short width;		// number of pixels wide
	short height;		// number of scan lines
	short horizAdvance; // number of pixels to advance to the next char
	short horizOffset;	// x offset into space to render glyph
	int baseline;		// y offset
	float s;			// x start tex coord
	float t;			// y start tex coord
	float s2;			// x end tex coord
	float t2;			// y end tex coord
} glyphInfo_t;

// this file corresponds 1:1 with the "*.fontdat" files, so don't change it unless you're going to
//	recompile the fontgen util and regenerate all the fonts!
//
typedef struct dfontdat_s
{
	glyphInfo_t mGlyphs[GLYPH_COUNT];

	short mPointSize;
	short mHeight; // max height of font
	short mAscender;
	short mDescender;

	short mKoreanHack; // unused field, written out by John's fontgen program but we have to leave it there for disk structs <sigh>
} dfontdat_t;

/////////////////// fonts end ////////////////////////////////////

#endif
