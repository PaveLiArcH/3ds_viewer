#ifndef __3dsVertex_h__
#define __3dsVertex_h__

#include "3dsTypes.h"
#include "glm\glm.hpp"

namespace ns_3ds
{
	struct sVertex
	{
		tFloat sf_coordinate[3];

		sVertex();
		sVertex(tFloat a_x, tFloat a_y, tFloat a_z);
		sVertex(const sVertex &a_vertex);

		void SetCoord(tFloat *a_coords);

		glm::vec3 Vec3Coordinate();
	};

	struct sVertexColor:sVertex
	{
		tFloat sf_color[3];

		sVertexColor();
		sVertexColor(tFloat a_x, tFloat a_y, tFloat a_z);
		sVertexColor(tFloat a_x, tFloat a_y, tFloat a_z, tFloat a_cr,  tFloat a_cg, tFloat a_cb);
		sVertexColor(const sVertexColor &a_vertexColor);

		void SetColor(tFloat *a_color);
		glm::vec3 Vec3Color();
	};

	struct sVertexNormal:sVertex
	{
		tFloat sf_normal[3];

		sVertexNormal();
		sVertexNormal(tFloat a_x, tFloat a_y, tFloat a_z);
		sVertexNormal(tFloat a_x, tFloat a_y, tFloat a_z, tFloat a_nx,  tFloat a_ny, tFloat a_nz);
		sVertexNormal(const sVertexNormal &a_vertexNormal);

		void SetNormal(tFloat *a_normal);
		glm::vec3 Vec3Normal();
	};

	struct sVertexNormalTex:sVertexNormal
	{
		tFloat sf_tex[2];

		sVertexNormalTex();
		sVertexNormalTex(tFloat a_x, tFloat a_y, tFloat a_z);
		sVertexNormalTex(tFloat a_x, tFloat a_y, tFloat a_z, tFloat a_nx, tFloat a_ny, tFloat a_nz);
		sVertexNormalTex(tFloat a_x, tFloat a_y, tFloat a_z, tFloat a_nx, tFloat a_ny, tFloat a_nz, tFloat a_u, tFloat a_v);
		sVertexNormalTex(const sVertexNormalTex &a_vertexNormalTex);

		void SetTex(tFloat *a_tex);
	};
};

#endif