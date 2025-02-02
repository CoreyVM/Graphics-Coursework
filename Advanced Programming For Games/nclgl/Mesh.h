#pragma once
#include "..\nclgl\OGLRenderer.h"

enum MeshBuffer
{
	VERTEX_BUFFER, COLOUR_BUFFER, TEXTURE_BUFFER, NORMAL_BUFFER, TANGENT_BUFFER, INDEX_BUFFER, MAX_BUFFER
};
class Mesh
{
public:
	Mesh(void);
	~Mesh(void);

	
	virtual void Draw();
	void SetTexture(GLuint tex) { texture = tex; }
	void SetBumpMap(GLuint tex) { bumpTexture = tex; }
	
	static Mesh* GenerateTriangle();
	static Mesh* GenerateQuad();
	GLuint GetTexture() { return texture; }
	GLuint GetBumpMap() { return bumpTexture; }
	void CreateNormals();
	void CreateTangents();
	void CreateBufferData();
	
protected:
	void BufferData();
	void GenerateNormals();
	void GenerateTangents();
	
	GLuint bumpTexture;
	GLuint arrayObject;
	GLuint bufferObject[MAX_BUFFER];
	GLuint numVertices;
	GLuint type;
	GLuint texture;
	GLuint numIndices;
	unsigned int* indices;

	Vector2* textureCoords;
	Vector3* tangents;
	Vector3 GenerateTangent(const Vector3& a, const Vector3& b,
		const Vector3& c, const Vector2& ta,
		const Vector2& tb, const Vector2& tc);
	Vector3* normals;
	Vector3* vertices;
	Vector4* colours;
};