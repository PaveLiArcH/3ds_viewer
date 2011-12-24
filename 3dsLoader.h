#ifndef __3dsLoader_h__
#define __3dsLoader_h__

#include "3dsTypes.h"

namespace ns_3ds
{
	struct s3dsHeader;
	class c3ds;
	class c3dsMaterial;
	class c3dsMap;

	typedef bool (*ptChunkReader) (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object);
	typedef tVec (*ptChunkReaderColor) (tistream & a_istream, s3dsHeader & a_header);
	typedef tFloat (*ptChunkReaderPercent) (tistream & a_istream, s3dsHeader & a_header);
	typedef bool (*ptChunkReaderMaterial) (tistream & a_istream, s3dsHeader & a_header, c3dsMaterial * a_material);
	typedef bool (*ptChunkReaderObject) (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object);
	typedef bool (*ptChunkReaderObjectTrimesh) (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object);
	typedef bool (*ptChunkReaderObjectTrimeshFace) (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object);
	typedef bool (*ptChunkReaderObjectLight) (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object);
	typedef bool (*ptChunkReaderMap) (tistream & a_istream, s3dsHeader & a_header, c3dsMap *a_map);
	class c3dsLoader
	{
		static std::wstring cf_resourcesDir;

		static ptChunkReader cm_getChunkReader(s3dsHeader & a_header);

		static bool cm_chunkReaderUnknown (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object);
		static bool cm_chunkReaderMain (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object);
		static bool cm_chunkReaderVersion (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object);
		static bool cm_chunkReaderEditor (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object);
		static bool cm_chunkReaderMeshVersion (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object);
		static bool cm_chunkReaderMaterialBlock (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object);
		static bool cm_chunkReaderOneUnit (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object);
		static bool cm_chunkReaderObjectBlock (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object);

		#pragma region ObjectChunks
		static ptChunkReaderObject cm_getChunkReaderObject(s3dsHeader & a_header);
		static bool cm_chunkReaderObject (tistream & a_istream, std::streamoff & a_maxoffset, c3ds & a_object);
		static bool cm_chunkReaderObjectHidden (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object);
		static bool cm_chunkReaderObjectNotShadowing (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object);
		static bool cm_chunkReaderObjectNotCast (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object);
		static bool cm_chunkReaderObjectMatte (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object);
		static bool cm_chunkReaderObjectExternalProcessed (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object);
		static bool cm_chunkReaderObjectTrimeshBlock (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object);
		static bool cm_chunkReaderObjectCamera (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object);
		static bool cm_chunkReaderObjectLightBlock (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object);
		static bool cm_chunkReaderObjectUnknown (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object);
		#pragma endregion Объектные чанки

		#pragma region ObjectLightingChunks
		static ptChunkReaderObjectLight cm_getChunkReaderObjectLight(s3dsHeader & a_header);
		static bool cm_chunkReaderObjectLight (tistream & a_istream, std::streamoff & a_maxoffset, c3ds & a_object);
		static bool cm_chunkReaderObjectLightUnknown (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object);
		#pragma endregion Объектные чанки освещения

		#pragma region ObjectTrimeshChunks
		static ptChunkReaderObjectTrimesh cm_getChunkReaderObjectTrimesh(s3dsHeader & a_header);
		static bool cm_chunkReaderObjectTrimesh (tistream & a_istream, std::streamoff & a_maxoffset, c3ds & a_object);
		static bool cm_chunkReaderObjectTrimeshVertexList (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object);
		static bool cm_chunkReaderObjectTrimeshFaceList (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object);
		static bool cm_chunkReaderObjectTrimeshMappingList (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object);
		static bool cm_chunkReaderObjectTrimeshUnknown (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object);
		#pragma endregion Объектные тримеш-чанки

		#pragma region ObjectTrimeshFaceChunks
		static ptChunkReaderObjectTrimeshFace cm_getChunkReaderObjectTrimeshFace(s3dsHeader & a_header);
		static bool cm_chunkReaderObjectTrimeshFace (tistream & a_istream, std::streamoff & a_maxoffset, c3ds & a_object);
		static bool cm_chunkReaderObjectTrimeshFaceMaterialList (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object);
		static bool cm_chunkReaderObjectTrimeshFaceSmoothList (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object);
		static bool cm_chunkReaderObjectTrimeshFaceUnknown (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object);
		#pragma endregion Объектные тримеш чанки сторон

		#pragma region MaterialChunks
		static ptChunkReaderMaterial cm_getChunkReaderMaterial(s3dsHeader & a_header);
		static bool cm_chunkReaderMaterial (tistream & a_istream, std::streamoff & a_maxoffset, c3dsMaterial * a_material);
		static bool cm_chunkReaderMaterialName (tistream & a_istream, s3dsHeader & a_header, c3dsMaterial * a_material);
		static bool cm_chunkReaderMaterialAmbient (tistream & a_istream, s3dsHeader & a_header, c3dsMaterial * a_material);
		static bool cm_chunkReaderMaterialDiffuse (tistream & a_istream, s3dsHeader & a_header, c3dsMaterial * a_material);
		static bool cm_chunkReaderMaterialSpecular (tistream & a_istream, s3dsHeader & a_header, c3dsMaterial * a_material);
		static bool cm_chunkReaderMaterialShininess (tistream & a_istream, s3dsHeader & a_header, c3dsMaterial * a_material);
		static bool cm_chunkReaderMaterialShininessStrength (tistream & a_istream, s3dsHeader & a_header, c3dsMaterial * a_material);
		static bool cm_chunkReaderMaterialTransparency (tistream & a_istream, s3dsHeader & a_header, c3dsMaterial * a_material);
		static bool cm_chunkReaderMaterialTransparencyFalloff (tistream & a_istream, s3dsHeader & a_header, c3dsMaterial * a_material);
		static bool cm_chunkReaderMaterialReflectionBlur (tistream & a_istream, s3dsHeader & a_header, c3dsMaterial * a_material);
		static bool cm_chunkReaderMaterialTextureMap1 (tistream & a_istream, s3dsHeader & a_header, c3dsMaterial * a_material);
		static bool cm_chunkReaderMaterialUnknown (tistream & a_istream, s3dsHeader & a_header, c3dsMaterial * a_material);
		#pragma endregion Материальные чанки

		#pragma region ColorChunks
		static ptChunkReaderColor cm_getChunkReaderColor(s3dsHeader & a_header);
		static tVec cm_chunkReaderColor (tistream & a_istream, std::streamoff & a_maxoffset);
		static tVec cm_chunkReaderColorRGBf (tistream & a_istream, s3dsHeader & a_header);
		static tVec cm_chunkReaderColorRGBb (tistream & a_istream, s3dsHeader & a_header);
		static tVec cm_chunkReaderColorRGBgf (tistream & a_istream, s3dsHeader & a_header);
		static tVec cm_chunkReaderColorRGBgb (tistream & a_istream, s3dsHeader & a_header);
		static tVec cm_chunkReaderColorUnknown (tistream & a_istream, s3dsHeader & a_header);
		#pragma endregion Цветовые чанки

		#pragma region PercentChunks
		static ptChunkReaderPercent cm_getChunkReaderPercent(s3dsHeader & a_header);
		static tFloat cm_chunkReaderPercent (tistream & a_istream, std::streamoff & a_maxoffset);
		static tFloat cm_chunkReaderPercentf (tistream & a_istream, s3dsHeader & a_header);
		static tFloat cm_chunkReaderPercenti (tistream & a_istream, s3dsHeader & a_header);
		static tFloat cm_chunkReaderPercentUnknown (tistream & a_istream, s3dsHeader & a_header);
		#pragma endregion Процентные чанки

		#pragma region MapChunks
		static ptChunkReaderMap cm_getChunkReaderMap(s3dsHeader & a_header);
		static c3dsMap *cm_chunkReaderMap (tistream & a_istream, std::streamoff & a_maxoffset);
		static bool cm_chunkReaderMapFile (tistream & a_istream, s3dsHeader & a_header, c3dsMap *a_map);
		static bool cm_chunkReaderMapUnknown (tistream & a_istream, s3dsHeader & a_header, c3dsMap *a_map);
		#pragma endregion Map чанки

	public:
		static bool load (tistream & a_istream, c3ds & a_object, std::wstring a_resourcesDir=L"");
	};
}

#include "3dsHeader.h"
#include "3dsMap.h"
#include "3ds.h"

#endif