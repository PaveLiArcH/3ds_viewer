#ifndef __3dsChunks_h__
#define __3dsChunks_h__

namespace n3ds
{
	namespace chunks
	{
		// chunk numbers
		enum chunks {
			/*color chunks*/
			RGBf=0x0010, // RGB float
			RGBb=0x0011, // RGB byte
			RGBGAMMACORRECTEDb=0x0012, // RGB (byte) gamma corrected
			RGBGAMMACORRECTEDf=0x0013, // RGB (float) gamma corrected

			/*percent chunks*/
			PERCENTi=0x0030, // percent (int)
			PERCENTf=0x0031, // percent (float)

			PROJECT=0xC23D, // project
			MATERIALLIB=0x3DAA, // material library

			/*main chunk*/
			MAIN=0x4D4D, // main
				VERSION=0x0002, // 3DS-version
				/*3D editor chunk*/
				EDITOR=0x3D3D, // 3D editor chunk (object Mesh)
					ONEUNIT=0x0100, // one unit

					/*background chunks*/
					BKGBMP=0x1100, // background bitmap
					BKGBMPUSE=0x1101, // use background bitmap
					BKGCOLOR=0x1200, // background color
					BKGCOLORUSE=0x1201, // use background color

					/*gradient chunks*/
					GRADIENTCOLORS=0x1300, // gradient colors
					GRADIENTCOLORSUSE=0x1301, // use gradient

					/*shadow map chunks*/
					SHADOWMAPBIAS=0x1400, // shadow map bias
					SHADOWMAPSIZE=0x1420, // shadow map size
					SHADOWMAPSAMPLERANGE=0x1450, // shadow map sample range

					/*raytrace chunks*/
					RAYTRACEBIAS=0x1460, // raytrace bias
					RAYTRACEON=0x1470, // raytrace on
					AMBIENTCOLOR=0x2100, // ambient color

					FOG=0x2200, // fog
						FOGBKG=0x2210, // fog background
					FOGUSE=0x2201, // use fog
					FOGBKGCOPY=0x2210, // fog background

					DISTANCEQUEUE=0x2300, // distance queue
						DIMBKG=0x2310, // dim background
					DISTANCEUSE=0x2301, // use distance queue
					FOGLAYEREDOPTIONS=0x2302, // layered fog options
					FOGLAYEREDUSE=0x2303, // use layered fog

					MESHVERSION=0x3D3E, // mesh version

					/*object chunks*/
					OBJECT=0x4000, // object block
						HIDDEN=0x4010, // object hidden
						NOTCAST=0x4012, // object doesn't cast
						MATTE=0x4013, // matte object
						EXTERNALPROCESS=0x4015, // external process on
						SHADOWLESS=0x4017, // object doesn't receive shadows

						/*triangular mesh*/
						TRIMESH=0x4100, // triangular mesh
							VERTEXLIST=0x4110, // vertices list
							VERTEXOPTIONS=0x4111, // vertex flag list
							FACESLIST=0x4120, // faces description
								MATERIALLIST=0x4130, // faces material list
								SMOOTHLIST=0x4150, // smoothing group list
							MAPPINGCOORDS=0x4140, // mappings list
							LOCALMATRIX=0x4160, // local coordinate system
							MAPPINGSTANDARD=0x4170, // texture info
							COLOR=0x4165, // object color in editor
							VISIBLE=0x4165, // visibility

							EXTERNALPROCESSNAME=0x4181, // external process name
							EXTERNALPROCESSPARAMS=0x4182, // external process parameters

						/*light*/
						LIGHT=0x4600, // light
							SPOTLIGHT=0x4610, // spotlight
								SPOTRAYTRACE=0x4627, // spot raytrace
								LIGHTSHADOWED=0x4630, // light shadowed
								SPOTSHADOWMAP=0x4641, // spot shadow map
								SPOTSHOWCONE=0x4650, // spot show cone
								SPOTRECTANGULAR=0x4651, // spot is rectangular
								SPOTOVERSHOOT=0x4652, // spot overshoot
								SPOTMAP=0x4653, // spot map
								SPOTROLL=0x4656, // spot roll
								SPOTRAYTRACEBIAS=0x4658, // spot ray trace bias
							LIGHTOFF=0x4620, // light off
							ATTENUATION=0x4625, // attenuation on
							RANGESTART=0x4659, // range start
							RANGEEND=0x465A, // range end
							MULTIPLIER=0x465B, // multiplier

						CAMERA3DS=0x4700, // camera

						HIERARCHY3DS=0x4F00, // hierarchy

					/*window settings*/
					VIEWPORT=0x7001, // window settings
					WINDOWSETTINGS=0x7001, // window settings
						WINDOWDESCR1=0x7011, // window description #1 ...
						WINDOWDESCR2=0x7012, // window description #2 ...
						MESHWINDOWS=0x7020, // mesh windows ...

					/*material block*/
					MATERIAL=0xAFFF, // material block
						MATERIALNAME=0xA000, // material name

						MATERIALAMBIENTCOLOR=0xA010, // ambient color
						DIFFUSECOLOR=0xA020, // diffuse color
						SPECULARCOLOR=0xA030, // specular color

						SHININESSPERCENT=0xA040, // shininess percent
						SHININESSSTRENGTHPERCENT=0xA041, // shininess strength percent

						TRANSPARENCYPERCENT=0xA050, // transparency percent
						TRANSPARENCYFALLOFFPERCENT=0xA052, // transparency falloff percent
						REFLECTIONBLURPERCENT=0xA053, // reflection blur percent

						TWOSIDED=0xA081, // 2 sided
						ADDTRANS=0xA083, // Add trans
						SELFILLUM=0xA084, // self illum.
						WIREFRAMEON=0xA085, // wire frame on
						WIRETHICKNESS=0xA087, // wire thickness
						FACEMAP=0xA088, // face map
						INTRANC=0xA08A, // in tranc
						SOFTEN=0xA08C, // soften
						WIREINUNITS=0xA08E, // wire in units

						RENDERTYPE=0xA100, // render type

						TRANSPARENCYFALLOFFPERCENTPERCENT=0xA240, // transparency falloff percent present
						REFLECTIONBLURPERCENTPERCENT=0xA250, // reflection blur percent present
						BUMPMAPPERCENT=0xA252, // bump map present (true percent)

						/*maps*/
						TEXTUREMAP1=0xA200, // texture map 1
						TEXTUREMAP2=0xA33A, // texture map 2
						OPACITYMAP=0xA210, // opacity map
						BUMPMAP=0xA230, // bump map
						SHININESSMAP=0xA33C, // shininess map
						SPECULARMAP=0xA204, // specular map
						SELFILLUMMAP=0xA33D, // self illum. map
						REFLECTIONMAP=0xA220, // reflection map

						/*masks*/
						MASK4TEXTUREMAP1=0xA33E, // mask for texture map 1
						MASK4TEXTUREMAP2=0xA340, // mask for texture map 2
						MASK4OPACITYMAP=0xA342, // mask for opacity map
						MASK4BUMPMAP=0xA344, // mask for bump map
						MASK4SHININESSMAP=0xA346, // mask for shininess map
						MASK4SPECULARMAP=0xA348, // mask for specular map
						MASK4SELFILLUMMAP=0xA34A, // mask for self illum. map
						MASK4REFLECTIONMAP=0xA34C, // mask for reflection map

							/*sub-chunks for all maps:*/
							MAPFILENAME=0xA300, // mapping filename
							MAPPARAMS=0xA351, // mapping parameters
							BLURPERCENT=0xA353, // blur percent
							VSCALE=0xA354, // V scale
							USCALE=0xA356, // U scale
							UOFFSET=0xA358, // U offset
							VOFFSET=0xA35A, // V offset
							ROTATIONANGLE=0xA35C, // rotation angle
							RGBTINT1=0xA360, // RGB Luma/Alpha tint 1
							RGBTINT2=0xA362, // RGB Luma/Alpha tint 2
							RTINT=0xA364, // RGB tint R
							GTINT=0xA366, // RGB tint G
							BTINT=0xA368, // RGB tint B

					/*keyframer*/
					KEYFRAMER=0xB000, // keyframer data
						AMBIENTLIGHTINFO=0xB001, // ambient light information block
						MESHINFO=0xB002, // mesh information block
						CAMERAINFO=0xB003, // camera information block
						CAMERATARGETINFO=0xB004, // camera target information block
						OMNILIGHTINFO=0xB005, // omni light information block
						SPOTLIGHTTARGETINFO=0xB006, // spot light target information block
						SPOTLIGHTINFO=0xB007, // spot light information block
						FRAMES=0xB008, // frames (start and end)
					
							/*possible sub-chunks for info blocks*/
							OBJECTNAMEPARAMSHIERARCHY=0xB010, // object name, parameters and hierarchy father
							PIVOTPOINT=0xB013, // object pivot point
							MORPHANGLE=0xB015, // object morph angle
							POSITIONTRACK=0xB020, // position track
							ROTATIONTRACK=0xB021, // rotation track
							SCALETRACK=0xB022, // scale track
							FOVTRACK=0xB023, // FOV track
							ROLLTRACK=0xB024, // roll track
							COLORTRACK=0xB025, // color track
							MORPHTRACK=0xB026, // morph track
							HOTSPOTTRACK=0xB027, // hotspot track
							FALLOFFTRACK=0xB028, // falloff track
							HIDETRACK=0xB029, // hide track
							HIERARCHYPOSITION=0xB030, // hierarchy position (object number)
		};
	}
}

#endif

/*
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
This is the representation of a chunk :

Offset   Length    Name

0        2         Chunk-ID
2        4         Chunk-length = 6+n+m
6        n         Data
6+n      m         Sub-chunks

So don't forget to read or skip the sub-chunks if the header and data size
is less than the chunk-length (6+n < chunk-length).

-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
Data types

Name        Description

word        2 byte
dword       4 byte
float       4 byte floating point number
strz        zero terminated string (C-string)
vector      3 floats (X,Y,Z)
BOOLEAN     a BOOLEAN chunk acts as a flag and contains no data or sub-chunks

degree      angle from 0 to 360 degree
rad         angle from 0 to 2*pi

Note: If X and Y are the horizontal plane and Z is the height in your
world, you don't have to exchange Y and Z. Some other infos say that you
have to exchange Y and Z only in the keyframer chunk, but you have to treat
all vectors the same way.


Chunks description
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------

                               Color chunks
                               ------------

These chunks are used several times in the format, they are "global
chunks".

-----------------------------------------------------------------------------
Chunk #    : 0x0010
Name       : Rgb color (float format)
Level      : global chunk
Size       : 12
Format     :

  float    red
  float    green
  float    blue

This chunk contains 3 floats : the red, green and blue components of a
color. The values range from 0.0 to 1.0 (full intensity).

-----------------------------------------------------------------------------
Chunk #    : 0x0011
Name       : Rgb color (byte format)
Level      : global chunk
Size       : 3
Format     :

  byte     red
  byte     green
  byte     blue

This chunk contains 3 bytes : the red, green and blue components of a
color. The values range from 0 to 255 (full intensity).

-----------------------------------------------------------------------------
Chunk #    : 0x0012
Name       : Rgb color gamma corrected (byte format)
Level      : global chunk
Size       : 3
Format     :

  byte     red
  byte     green
  byte     blue

This chunk contains 3 bytes : the red, green and blue components of a gamma
corrected color. The values range from 0 to 255 (full intensity).

-----------------------------------------------------------------------------
Chunk #    : 0x0013
Name       : Rgb color gamma corrected (float format)
Level      : global chunk
Size       : 12
Format     :

  float    red
  float    green
  float    blue

This chunk contains 3 floats : the red, green and blue components of a
gamma corrected color. The values range from 0.0 to 1.0 (full intensity).

-----------------------------------------------------------------------------

                              Percent chunks
                              --------------

-----------------------------------------------------------------------------
Chunk #    : 0x0030
Name       : percent (int format)
Level      : global chunk
Size       : 2
Format     :

  word     percent

This chunk contains a word. The values range from 0 to 100.

-----------------------------------------------------------------------------
Chunk #    : 0x0031
Name       : percent (float format)
Level      : global chunk
Size       : 4
Format     :

  float    percent

This chunk contains a float. The values range from 0 to 100.

=============================================================================

                            M A I N  C H U N K
                            ==================

=============================================================================
Chunk #    : 0x4D4D
Name       : Main chunk
Level      : 0
Size       : 0 + sub-chunks
Father     : none
Format     :

-----------------------------------------------------------------------------
Chunk #    : 0x0002
Name       : 3DS-Version
Level      : 1
Size       : 4
Father     : 0x4D4D (Main chunk)
Format     :

  dword    Version

This dword specifies the number of your 3DS-Version.

=============================================================================

                        3 D  E D I T O R  C H U N K
                        ===========================

=============================================================================
Chunk #    : 0x3D3D
Name       : 3D Editor chunk
Level      : 1
Size       : 0 + sub-chunks
Father     : 0x4D4D (Main chunk)
Format     :

-----------------------------------------------------------------------------
Chunk #    : 0x0100
Name       : One unit
Level      : 2
Size       : 4
Father     : 3D Editor chunk
Format     :

  float    One unit

-----------------------------------------------------------------------------
Chunk #    : 0x1100
Name       : Background bitmap
Level      : 2
Size       : varying
Father     : 3D Editor chunk
Format     :

  strz     Name

This chunk contains the name of the background bitmap choosen in 3DS even
if not used (i.e. you have selected a bitmap but you choose another thing).

-----------------------------------------------------------------------------
Chunk #    : 0x1101
Name       : Use background bitmap
Level      : 2
Size       : 0
Father     : 3D Editor chunk
Format     : BOOLEAN

This chunk is a flag indicating that the background bitmap (see chunk
0x1100) is used. When this chunk is not present, the background bitmap is
not used.

-----------------------------------------------------------------------------
Chunk #    : 0x1200
Name       : Background color
Level      : 2
Size       : 0 + sub-chunks
Father     : 3D Editor chunk
Format     :

This chunk contains the color of the background even if not used. You must
read the color sub-chunks. Generaly, the format is :
 - Background color chunk (0x1200)
 - RGB float color chunk (0x0010)
 - RGB float gamma corected color chunk (0x0013)

-----------------------------------------------------------------------------
Chunk #    : 0x1201
Name       : Use background color
Level      : 2
Size       : 0
Father     : 3D Editor chunk
Format     : BOOLEAN

This chunk is a flag indicating that the background color (see chunk
0x1200) is used. When this chunk is not present, the background color is
not used.

-----------------------------------------------------------------------------
Chunk #    : 0x1300
Name       : Background gradient colors
Level      : 2
Size       : 4 + sub-chunks
Father     : 3D Editor chunk
Format     :

  float        Gradient position
  color chunk  RGBF1
  color chunk  RGBFG1
  color chunk  RGBF2
  color chunk  RGBFG2
  color chunk  RGBF3
  color chunk  RGBFG3

RGBF = RGB float color CHUNK
RGBFG = RGB gamma corrected float color CHUNK

  So, if hexa you have something like that

  00 13 76 00 00 00 21 CE 4A 3F 10 00 12 00 00 00 .....

* 00 13 is the chunk ID (0x1300) and 76 00 00 00 is the length of this chunk.

* 21 CE 4A 3F are values which contain the "position" of the three gradient
  colors. It's a float (from 0.0 to 1.0) indicating the position of the
  middle color.

* 10 00 is the chunk ID (0x0010) of a rgb float color and 12 00 00 00 is the
  length of this sub-chunk.
-----------------------------------------------------------------------------

Chunk #    : 0x1301
Name       : Use background gradient colors
Level      : 2
Size       : 0
Father     : 3D Editor chunk
Format     : BOOLEAN

This chunk is a flag indicating that the background gradient colors (see
chunk 0x1300) are used. When this chunk is not present, the gradient
colors are not used.

-----------------------------------------------------------------------------
Chunk #    : 0x1400
Name       : Shadow map bias
Level      : 2
Size       : 4
Father     : 3D Editor chunk
Format     :

  float    Shadow map bias

-----------------------------------------------------------------------------
Chunk #    : 0x1420
Name       : Shadow map size
Level      : 2
Size       : 2
Father     : 3D Editor chunk
Format     :

  word     Shadow map size

-----------------------------------------------------------------------------
Chunk #    : 0x1450
Name       : Shadow map sample range
Level      : 2
Size       : 4
Father     : 3D Editor chunk
Format     :

  float    Shadow map sample range
-----------------------------------------------------------------------------

Chunk #    : 0x1460
Name       : Raytrace bias
Level      : 2
Size       : 4
Father     : 3D Editor chunk
Format     :

  float    Raytrace bias

-----------------------------------------------------------------------------
Chunk #    : 0x1470
Name       : Use raytrace
Level      : 2
Size       : 0
Father     : 3D Editor chunk
Format     : BOOLEAN

This chunk is a flag indicating that raytrace (see chunk 0x1460) is used.
When this chunk is not present, the raytrace is not used.

-----------------------------------------------------------------------------
Chunk #    : 0x2100
Name       : Ambient color
Level      : 2
Size       : 0 + sub-chunk
Father     : 3D Editor chunk
Format     :

This chunk contains a color chunk (in general RGB float)

-----------------------------------------------------------------------------

                           0x4000 : OBJECT BLOCK
                           ---------------------

-----------------------------------------------------------------------------
Chunk #    : 0x4000
Name       : OBJECT BLOCK
Level      : 2
Size       : varying + sub-chunks
Father     : 0x3D3D (3D Editor chunk)
Format     :

  strz     Object name

-----------------------------------------------------------------------------
Chunk #    : 0x4010
Name       : Object hidden
Level      : 3
Size       : 0
Father     : 0x4000 (Object block)
Format     : BOOLEAN

-----------------------------------------------------------------------------
Chunk #    : 0x4012
Name       : Object doesn't cast
Level      : 3
Size       : 0
Father     : 0x4000 (Object block)
Format     : BOOLEAN

-----------------------------------------------------------------------------
Chunk #    : 0x4013
Name       : Matte object
Level      : 3
Size       : 0
Father     : 0x4000 (Object block)
Format     : BOOLEAN

-----------------------------------------------------------------------------
Chunk #    : 0x4015
Name       : External process on
Level      : 3
Size       : 0
Father     : 0x4000 (Object block)
Format     : BOOLEAN

-----------------------------------------------------------------------------
Chunk #    : 0x4017
Name       : Object doesn't receive shadows
Level      : 3
Size       : 0
Father     : 0x4000 (Object block)
Format     : BOOLEAN

-----------------------------------------------------------------------------

                         0x4100 : Triangular mesh
                         ------------------------

-----------------------------------------------------------------------------
Chunk #    : 0x4100
Name       : Triangular Mesh
Level      : 3
Size       : 0 + sub-chunks
Father     : 0x4000 (Object block)
Format     :

-----------------------------------------------------------------------------
Chunk #    : 0x4110
Name       : Vertices list
Level      : 4
Size       : varying
Father     : 0x4100 (Triangular mesh)
Format     :

  word     Number of vertices

           Then, for each vertex
  vector   Position

-----------------------------------------------------------------------------
Chunk #    : 0x4120
Name       : Faces description
Level      : 4
Size       : varying + sub-chunks
Father     : 0x4100 (Triangular mesh)
Format     :

  word     Number of faces

           Then, for each face:
  word     Vertex for corner A (number reference)
  word     Vertex for corner B (number reference)
  word     Vertex for corner C (number reference)
  word     Face flag
            * bit 0 : CA visible
            * bit 1 : BC visible
            * bit 2 : AB visible

After datas, parse sub-chunks (0x4130, 0x4150).

-----------------------------------------------------------------------------
Chunk #    : 0x4130
Name       : Faces material list
Level      : 5
Size       : varying
Father     : 0x4120 (Faces description)
Format     :

  strz     Material name
  word     Number of entries

           Then, for each entry:
  word     Face assigned to this material (number reference)

I think the faces of one object can have different materials. Therefore,
this chunk can be present more than once.

-----------------------------------------------------------------------------
Chunk #    : 0x4140
Name       : Mapping coordinates list for each vertex
Level      : 4
Size       : varying
Father     : 0x4100 (Triangular mesh)
Format     :

  word     Number of vertices

           Then, for each vertex
  float    U coordinate
  float    V coordinate

-----------------------------------------------------------------------------
Chunk #    : 0x4150
Name       : Smoothing groups list
Level      : 5
Size       :
Father     : 0x4120 (Faces description)
Format     :

-----------------------------------------------------------------------------
Chunk #    : 0x4160
Name       : Local coordinate system
Level      : 4
Size       : 48
Father     : 0x4100 (Triangular mesh)
Format     :

  vector   X1
  vector   X2
  vector   X3
  vector   O

X1, X2 and X3 represent the axes, O the origin.

-----------------------------------------------------------------------------
Chunk #    : 0x4165
Name       : Object color in 3D Editor
Level      : 4
Size       : 1
Father     : 0x4100 (Triangular mesh)
Format     :

  byte     Color

-----------------------------------------------------------------------------
Chunk #    : 0x4600
Name       : Light
Level      : 3
Size       : 12 + sub-chunks
Father     : 0x4000 (Object block)
Format     :

  vector   Position

-----------------------------------------------------------------------------
Chunk #    : 0x4610
Name       : Spotlight
Level      : 4
Size       : 20 + sub-chunks
Father     : 0x4600 (Light)
Format     :

  vector   Target
  float    HotSpot
  float    FallOff

If this chunk is found, the light is a spot light and not an omni
directional light.

-----------------------------------------------------------------------------
Chunk #    : 0x4651
Name       : Spot is rectangular
Level      : 5
Size       : 0
Father     : 0x4610 (Spotlight)
Format     : BOOLEAN

-----------------------------------------------------------------------------
Chunk #    : 0x4653
Name       : Spot map
Level      : 5
Size       : varying
Father     : 0x4610 (Spotlight)
Format     :

  strz     Filename

-----------------------------------------------------------------------------
Chunk #    : 0x4656
Name       : Spot roll
Level      : 5
Size       : 4
Father     : 0x4610 (Spotlight)
Format     :

  float    Roll (degree)

-----------------------------------------------------------------------------
Chunk #    : 0x4700
Name       : CAMERA
Level      : 3
Size       : 32
Father     : 0x4000 (Object block)
Format     :

  vector   Position
  vector   Target
  float    Bank (degree)
  float    Lens

-----------------------------------------------------------------------------
Chunk #    : 0x7001
Name       : Window settings
Level      : 2
Size       : varying
Father     : 0x3D3D (3D editor chunk)
Format     :

-----------------------------------------------------------------------------
Chunk #    : 0x3D3E
Name       : Mesh version
Level      : 2
Size       : 4
Father     : 0x3D3D (3D editor chunk)
Format     :

  dword    Version

-----------------------------------------------------------------------------

                          0xAFFF : Material block
                          -----------------------

-----------------------------------------------------------------------------
Chunk #    : 0xAFFF
Name       : Material editor chunk
Level      : 2
Size       : 0 + sub-chunks
Father     : 0x3D3D (3D editor chunk)
Format     :

-----------------------------------------------------------------------------
Chunk #    : 0xA000
Name       : Material name
Level      : 3
Size       : varying
Father     : 0xAFFF (Material block)
Format     :

  strz     Material name

-----------------------------------------------------------------------------
Chunk #    : 0xA010
Name       : Material ambient color
Level      : 3
Size       : 0 + sub-chunks
Father     : 0xAFFF (Material block)
Format     :

This chunk contains color chunks (in gen. rgb byte & rgb byte gamma)

-----------------------------------------------------------------------------
Chunk #    : 0xA020
Name       : Material diffuse color
Level      : 3
Size       : 0 + sub-chunks
Father     : 0xAFFF (Material block)
Format     :

This chunk contains color chunks (in gen. rgb byte & rgb byte gamma)

-----------------------------------------------------------------------------
Chunk #    : 0xA030
Name       : Material specular color
Level      : 3
Size       : 0 + sub-chunks
Father     : 0xAFFF (Material block)
Format     :

This chunk contains color chunks (in gen. rgb byte & rgb byte gamma)

-----------------------------------------------------------------------------
Chunk #    : 0xA040
Name       : Material shininess percent
Level      : 3
Size       : 0 + sub-chunk
Father     : 0xAFFF (Material block)
Format     :

This chunk contains a percent chunk (in gen. int format)

-----------------------------------------------------------------------------
Chunk #    : 0xA041
Name       : Material shininess strength percent
Level      : 3
Size       : 0 + sub-chunk
Father     : 0xAFFF (Material block)
Format     :

This chunk contains a percent chunk (in gen. int format)

-----------------------------------------------------------------------------
Chunk #    : 0xA200 - 0xA34C
Name       : Map
Level      : 3
Size       : 0 + sub-chunk
Father     : 0xAFFF (Material block)
Format     :

These chunks define the different maps (see chunk tree). They contain the
sub-chunks for all maps, such as mapping filename name or U/V scale.

-----------------------------------------------------------------------------
Chunk #    : 0xA300
Name       : Mapping filename
Level      : 4
Size       : varying
Father     : 0xA200 - 0xA34C (Map)
Format     :

  strz     Filename

-----------------------------------------------------------------------------
Chunk #    : 0xA354
Name       : V scale
Level      : 4
Size       : 4
Father     : 0xA200 - 0xA34C (Map)
Format     :

  float    V scale

-----------------------------------------------------------------------------
Chunk #    : 0xA356
Name       : U scale
Level      : 4
Size       : 4
Father     : 0xA200 - 0xA34C (Map)
Format     :

  float    U scale

-----------------------------------------------------------------------------
Chunk #    : 0xA358
Name       : U offst
Level      : 4
Size       : 4
Father     : 0xA200 - 0xA34C (Map)
Format     :

  float    U offset

-----------------------------------------------------------------------------
Chunk #    : 0xA35A
Name       : V offst
Level      : 4
Size       : 4
Father     : 0xA200 - 0xA34C (Map)
Format     :

  float    V offset

-----------------------------------------------------------------------------
Chunk #    : 0xA35A
Name       : Rotation angle
Level      : 4
Size       : 4
Father     : 0xA200 - 0xA34C (Map)
Format     :

  float    Rotation angle

-----------------------------------------------------------------------------

                      K E Y F R A M E R    C H U N K
                      ------------------------------

-----------------------------------------------------------------------------
Chunk #    : 0xB000
Name       : Keyframer
Level      : 1
Size       : 0 + sub-chunks
Father     : 0x4D4D (Main chunk)
Format     :

The goal of the keyframer datas is to describe the move of
 - The objects in the scene (like a cube ...)
 - The lights (ambient, omni or spot)
 - The cameras

-----------------------------------------------------------------------------
Chunk #    : 0xB001..0xB007
Name       : Information block
Level      : 2
Size       : 0 + sub-chunks
Father     : 0xB000 (Keyframer chunk)
Format     :

The information block is a chunk constituted of sub-chunks describing the
move of a particular object, or camera:

  0xB001 : Ambient light information block
  0xB002 : Mesh information block
  0xB003 : Camera information block
  0xB004 : Camera target information block
  0xB005 : Omni light information block
  0xB006 : Spot light target information block
  0xB007 : Spot light information block

-----------------------------------------------------------------------------
Chunk #    : 0xB008
Name       : Frames (Start and End)
Level      : 2
Size       : 8
Father     : 0xB000 (Keyframer chunk)
Format     :

  dword    Start
  dword    End

-----------------------------------------------------------------------------
Chunk #    : 0xB010
Name       : Object name, parameters and hierarchy father
Level      : 3
Size       : varying
Father     : 0xB001..0xB007 Information block
Format     :

  strz     Object Name
  word     Flag1
            * Bit 11 : Hidden
  word     Flag2
            * Bit 0 : Show path
            * Bit 1 : Animate smoothing
            * Bit 4 : Object motion blur
            * Bit 6 : Morph materials
  word     Hierarchy father, link to the parent object (-1 for none)

-----------------------------------------------------------------------------
Chunk #    : 0xB013
Name       : Object pivot point
Level      : 3
Size       : 12
Father     : 0xB001..0xB007 Information block
Format     :

  vector   Pivot point

-----------------------------------------------------------------------------
Chunk #    : 0xB020..0xB029
Name       : Track
Level      : 3
Size       : varying
Father     : 0xB001..0xB007 Information block
Format     :

  word     Flag
            * Bits 0-1 : 0 = single
                         2 = repeat
                         3 = loop

            * Bit 3 : lock X
            * Bit 4 : lock Y
            * Bit 5 : lock Z

            * Bit 7 : unlink X
            * Bit 8 : unlink Y
            * Bit 9 : unlink Z

  8 byte   Unknown
  dword    Number of keys in this track

           Then, for each key:
  dword    Key number (position in track)
  word     Acceleration data present (flag)   Range:
            * Bit 0 : Tension follows         [-1.0, 1.0]
            * Bit 1 : Continuity follows      [-1.0, 1.0]
            * Bit 2 : Bias follows            [-1.0, 1.0]
            * Bit 3 : Ease to follows         [ 0.0, 1.0]
            * Bit 4 : Ease from follows       [ 0.0, 1.0]
  n floats Acceleration data
  ?        Track specific data

  Track specific data is:
   0xB020 : Position track : 1 vector   Position
   0xB021 : Rotation track : 1 float    Angle (rad)
                             1 vector   Axis
   0xB022 : Scale track    : 3 floats   Size
   0xB023 : FOV track      : 1 float    Angle (degree)
   0xB024 : Roll track     : 1 float    Angle (degree)
   0xB025 : Color track    :
   0xB026 : Morph track    : 1 strz     Object name
   0xB027 : Hotspot track  : 1 float    Angle (degree)
   0xB028 : Falloff track  : 1 float    Angle (degree)
   0xB029 : Hide track     : nothing


-----------------------------------------------------------------------------
Chunk #    : 0xB030
Name       : Hierarchy position
Level      : 3
Size       : 2
Father     : 0xB001..0xB007 Information block
Format     :

  word     Hierarchy

This word contains a unique value for the object and is used for the
hierarchy tree links.

-----------------------------------------------------------------------------
*/