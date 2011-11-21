#include "3dsVertex.h"

namespace ns_3ds
{
	sVertex::sVertex()
	{
		sf_coordinate[0]=0.0f; sf_coordinate[1]=0.0f; sf_coordinate[2]=0.0f;
	};
	sVertex::sVertex(tFloat a_x, tFloat a_y, tFloat a_z)
	{
		sf_coordinate[0]=a_x; sf_coordinate[1]=a_y; sf_coordinate[2]=a_z;
	}
	sVertex::sVertex(const sVertex &a_vertex)
	{
		memcpy(sf_coordinate,a_vertex.sf_coordinate,sizeof(tFloat)*3);
	};

	void sVertex::SetCoord(tFloat *a_coords)
	{
		memcpy(sf_coordinate,a_coords,sizeof(tFloat)*3);
	}

	glm::vec3 sVertex::Vec3Coordinate()
	{
		return glm::vec3(sf_coordinate[0],sf_coordinate[1],sf_coordinate[2]);
	}

	sVertexColor::sVertexColor():sVertex()
	{
		sf_color[0]=0.0f; sf_color[1]=0.0f; sf_color[2]=0.0f;
	}
	sVertexColor::sVertexColor(tFloat a_x, tFloat a_y, tFloat a_z):sVertex(a_x, a_y, a_z)
	{
		sf_color[0]=0.0f; sf_color[1]=1.0f; sf_color[2]=0.0f;
	}
	sVertexColor::sVertexColor(tFloat a_x, tFloat a_y, tFloat a_z, tFloat a_cr,  tFloat a_cg, tFloat a_cb):sVertex(a_x, a_y, a_z)
	{
		sf_color[0]=a_cr; sf_color[1]=a_cg; sf_color[2]=a_cb;
	}
	sVertexColor::sVertexColor(const sVertexColor &a_vertexColor):sVertex(a_vertexColor)
	{
		memcpy(sf_color,a_vertexColor.sf_color,sizeof(tFloat)*3);
	}

	void sVertexColor::SetColor(tFloat *a_color)
	{
		memcpy(sf_color,a_color,sizeof(tFloat)*3);
	}
	glm::vec3 sVertexColor::Vec3Color()
	{
		return glm::vec3(sf_color[0],sf_color[1],sf_color[2]);
	}

	sVertexNormal::sVertexNormal():sVertex()
	{
		sf_normal[0]=0.0f; sf_normal[1]=1.0f; sf_normal[2]=0.0f;
	};
	sVertexNormal::sVertexNormal(tFloat a_x, tFloat a_y, tFloat a_z):sVertex(a_x, a_y, a_z)
	{
		sf_normal[0]=0.0f; sf_normal[1]=1.0f; sf_normal[2]=0.0f;
	}
	sVertexNormal::sVertexNormal(tFloat a_x, tFloat a_y, tFloat a_z, tFloat a_nx,  tFloat a_ny, tFloat a_nz):sVertex(a_x, a_y, a_z)
	{
		sf_normal[0]=a_nx; sf_normal[1]=a_ny; sf_normal[2]=a_nz;
	}
	sVertexNormal::sVertexNormal(const sVertexNormal &a_vertexNormal):sVertex(a_vertexNormal)
	{
		memcpy(sf_normal,a_vertexNormal.sf_normal,sizeof(tFloat)*3);
	}

	void sVertexNormal::SetNormal(tFloat *a_normal)
	{
		memcpy(sf_normal,a_normal,sizeof(tFloat)*3);
	}
	glm::vec3 sVertexNormal::Vec3Normal()
	{
		return glm::vec3(sf_normal[0],sf_normal[1],sf_normal[2]);
	}

	sVertexNormalTex::sVertexNormalTex():sVertexNormal()
	{
		sf_tex[0]=0.0f; sf_tex[1]=0.0f;
	};
	sVertexNormalTex::sVertexNormalTex(tFloat a_x, tFloat a_y, tFloat a_z):sVertexNormal(a_x, a_y, a_z)
	{
		sf_tex[0]=0.0f; sf_tex[1]=0.0f;
	}
	sVertexNormalTex::sVertexNormalTex(tFloat a_x, tFloat a_y, tFloat a_z, tFloat a_nx, tFloat a_ny, tFloat a_nz):sVertexNormal(a_x, a_y, a_z, a_nx, a_ny, a_nz)
	{
		sf_tex[0]=0.0f; sf_tex[1]=0.0f;
	}
	sVertexNormalTex::sVertexNormalTex(tFloat a_x, tFloat a_y, tFloat a_z, tFloat a_nx, tFloat a_ny, tFloat a_nz, tFloat a_u, tFloat a_v):sVertexNormal(a_x, a_y, a_z, a_nx, a_ny, a_nz)
	{
		sf_tex[0]=a_u; sf_tex[1]=a_v;
	}
	sVertexNormalTex::sVertexNormalTex(const sVertexNormalTex &a_vertexNormalTex):sVertexNormal(a_vertexNormalTex)
	{
		memcpy(sf_tex,a_vertexNormalTex.sf_tex,sizeof(tFloat)*2);
	}

	void sVertexNormalTex::SetTex(tFloat *a_tex)
	{
		memcpy(sf_tex,a_tex,sizeof(tFloat)*2);
	}
}