#ifndef __3dsLoader_h__
#define __3dsLoader_h__

#include "3dsTypes.h"

namespace n3ds
{
	struct s3dsHeader;
	class c3ds;

	typedef bool (*ptChunkReader) (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object);
	typedef tVec (*ptChunkReaderColor) (tistream & a_istream, s3dsHeader & a_header);
	typedef tFloat (*ptChunkReaderPercent) (tistream & a_istream, s3dsHeader & a_header);
	typedef bool (*ptChunkReaderMaterial) (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object);

	class c3dsLoader
	{
		static ptChunkReader cm_getChunkReader(s3dsHeader & a_header);

		static bool cm_chunkReaderUnknown (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object);
		static bool cm_chunkReaderMain (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object);
		static bool cm_chunkReaderVersion (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object);
		static bool cm_chunkReaderEditor (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object);
		static bool cm_chunkReaderMeshVersion (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object);
		static bool cm_chunkReaderMaterialBlock (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object);
		static bool cm_chunkReaderOneUnit (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object);

		#pragma region MaterialChunks
		static ptChunkReaderMaterial cm_getChunkReaderMaterial(s3dsHeader & a_header);
		static bool cm_chunkReaderMaterial (tistream & a_istream, std::streamoff & a_maxoffset, c3ds & a_object);
		static bool cm_chunkReaderMaterialName (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object);
		static bool cm_chunkReaderMaterialAmbient (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object);
		static bool cm_chunkReaderMaterialDiffuse (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object);
		static bool cm_chunkReaderMaterialSpecular (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object);
		static bool cm_chunkReaderMaterialShininess (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object);
		static bool cm_chunkReaderMaterialShininessStrength (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object);
		static bool cm_chunkReaderMaterialTransparency (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object);
		static bool cm_chunkReaderMaterialTransparencyFalloff (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object);
		static bool cm_chunkReaderMaterialReflectionBlur (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object);
		static bool cm_chunkReaderMaterialUnknown (tistream & a_istream, s3dsHeader & a_header, c3ds & a_object);
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

	public:
		static bool load (tistream & a_istream, c3ds & a_object);
	};
}

#include "3dsHeader.h"
#include "3ds.h"

#endif