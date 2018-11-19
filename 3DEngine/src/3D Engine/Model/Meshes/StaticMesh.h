#pragma once
#include <vector>

struct StaticVertex;

class StaticMesh
{
private:
	ID3D11Buffer * m_vertexBuffer = nullptr;
	void _createVertexBuffer();
private:
	std::string m_meshName;
	std::vector<StaticVertex> m_staticVertex;
public:
	StaticMesh();
	~StaticMesh();

	const StaticVertex * GetRawVertice() const;
	const std::vector<StaticVertex> & GetVertice() const;
	void SetVertices(std::vector<StaticVertex>& input);

	void SetName(const std::string & name);
	const std::string & GetName() const;

	void LoadMesh(const std::string & path);

	ID3D11Buffer * GetBuffer();

};

