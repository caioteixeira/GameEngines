#include "ITPEnginePCH.h"
#include <SDL/SDL_log.h>

namespace
{
	union VertPacked
	{
		float f;
		uint8_t b[4];
	};
}

Mesh::Mesh(class Game& game)
	:Asset(game)
	,mShaderType(EMS_Basic)
{
	// Initialize bounding sphere to radius of zero
	mBoundingSphere.mRadius = 0.0f;
	// We assume the center of the sphere is 0,0,0

	// Initialize the bounding box to maximal opposite values
	mBoundingBox.mMin = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
	mBoundingBox.mMax = Vector3(FLT_MIN, FLT_MIN, FLT_MIN);
}

Mesh::~Mesh()
{

}

bool Mesh::Load(const char* fileName, AssetCache* cache)
{
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		SDL_Log("File not found: Mesh %s", fileName);
		return false;
	}
	
	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);

	if (!doc.IsObject())
	{
		SDL_Log("Mesh %s is not valid json", fileName);
		return false;
	}

	std::string str = doc["metadata"]["type"].GetString();
	int ver = doc["metadata"]["version"].GetInt();

 	// Check the metadata
 	if (!doc["metadata"].IsObject() ||
		str != "itpmesh" ||
 		ver != 2)
 	{
 		SDL_Log("Mesh %s unknown format", fileName);
 		return false;
 	}

	// Load textures
	const rapidjson::Value& textures = doc["textures"];
	if (!textures.IsArray() || textures.Size() < 1)
	{
		SDL_Log("Mesh %s has no textures, there should be at least one", fileName);
		return false;
	}
	
	for (rapidjson::SizeType i = 0; i < textures.Size(); i++)
	{
		std::string textureName = textures[i].GetString();
		mTextures.emplace_back(cache->Load<Texture>(textureName.c_str()));
		if (mTextures[i] == nullptr)
		{
			// Failed to load this texture, so use the default
			SDL_Log("Mesh %s: Failed to load texture %s. Using default.", fileName, textureName.c_str());
			mTextures[i] = cache->Load<Texture>("Textures/Default.png");
		}
	}

	// Read the vertex format
	const rapidjson::Value& vertFormat = doc["vertexformat"];
	if (!vertFormat.IsArray() || vertFormat.Size() < 1)
	{
		SDL_Log("Mesh %s vertex format must have at least one element.", fileName);
		return false;
	}

	size_t vertSize = 0;
	size_t vertNumValues = 0;
	std::string inputLayoutName;

	for (rapidjson::SizeType i = 0; i < vertFormat.Size(); i++)
	{
		if (!vertFormat[i].IsObject())
		{
			SDL_Log("Mesh %s: Vertex format element %d is invalid.", fileName, i);
			return false;
		}

		inputLayoutName += vertFormat[i]["name"].GetString();

		std::string vertType = vertFormat[i]["type"].GetString();
		size_t elementSize = 0;
		if (vertType == "float")
		{
			elementSize = 4;
		}
		else
		{
			elementSize = 1;
		}

		vertNumValues += vertFormat[i]["count"].GetUint();
		vertSize += elementSize * vertFormat[i]["count"].GetUint();
	}

	if (vertNumValues < 3)
	{
		SDL_Log("Mesh %s must at least have 3D position coordinates!", fileName);
		return false;
	}

	// Get the shader type
	std::string shaderType = doc["shader"]["name"].GetString();
	if (shaderType == "BasicMesh")
	{
		mShaderType = EMS_Basic;
	}
	else if (shaderType == "Phong")
	{
		mShaderType = EMS_Phong;
	}
	else if (shaderType == "Skinned")
	{
		mShaderType = EMS_Skinned;
	}
	else
	{
		SDL_Log("Mesh %s has unknown shader type. Defaulting to basic mesh.", fileName);
	}

	// Load in the vertices
	const rapidjson::Value& vertsJson = doc["vertices"];
	if (!vertsJson.IsArray() || vertsJson.Size() < 1)
	{
		SDL_Log("Mesh %s has no vertices", fileName);
		return false;
	}

	size_t numVerts = vertsJson.Size();
	std::vector<VertPacked> vertices;
	vertices.reserve(numVerts * vertSize);
	for (rapidjson::SizeType i = 0; i < vertsJson.Size(); i++)
	{
		const rapidjson::Value& vert = vertsJson[i];
		if (!vert.IsArray() || vert.Size() != vertNumValues)
		{
			SDL_Log("Unexpected vertex format for %s", fileName);
			return false;
		}

		Vector3 pos(vert[0].GetDouble(), vert[1].GetDouble(), vert[2].GetDouble());
		// Update the bounding box
		mBoundingBox.UpdateMinMax(pos);
		
		// Now stuff all the vertices into the vertex buffer
		if (mShaderType != EMS_Skinned)
		{
			for (rapidjson::SizeType j = 0; j < vert.Size(); j++)
			{
				VertPacked temp;
				temp.f = vert[j].GetDouble();
				vertices.emplace_back(temp);
			}
		}
		else
		{
			// Special case code for skinned, for now...
			VertPacked temp;
			// Position/Normal
			for (rapidjson::SizeType j = 0; j < 6; j++)
			{
				temp.f = vert[j].GetDouble();
				vertices.emplace_back(temp);
			}
			
			// Uints for bones and weights
			for (rapidjson::SizeType j = 6; j < 14; j += 4)
			{
				temp.b[0] = vert[j].GetUint();
				temp.b[1] = vert[j + 1].GetUint();
				temp.b[2] = vert[j + 2].GetUint();
				temp.b[3] = vert[j + 3].GetUint();
				vertices.emplace_back(temp);
			}

			// Last two texture coordinates
			for (rapidjson::SizeType j = 14; j < vert.Size(); j++)
			{
				temp.f = vert[j].GetDouble();
				vertices.emplace_back(temp);
			}
		}
	}

	// Now that we have a bounding box, make a bounding sphere
	// around it
	mBoundingSphere.ComputeFromBox(mBoundingBox);

	// Load in the indices
	const rapidjson::Value& indJson = doc["indices"];
	if (!indJson.IsArray() || indJson.Size() < 1)
	{
		SDL_Log("Mesh %s has no indices", fileName);
		return false;
	}

	std::vector<uint16_t> indices;
	indices.reserve(indJson.Size() * 3);
	for (rapidjson::SizeType i = 0; i < indJson.Size(); i++)
	{
		const rapidjson::Value& ind = indJson[i];
		if (!ind.IsArray() || ind.Size() != 3)
		{
			SDL_Log("Invalid indices for %s", fileName);
			return false;
		}
		
		indices.emplace_back(static_cast<uint16_t>(ind[0].GetUint()));
		indices.emplace_back(static_cast<uint16_t>(ind[1].GetUint()));
		indices.emplace_back(static_cast<uint16_t>(ind[2].GetUint()));
	}

	// Now create a vertex array
	mVertexArray = VertexArray::Create(mGame.GetRenderer().GetGraphicsDriver(), 
		mGame.GetRenderer().GetInputLayoutCache(),
		vertices.data(), numVerts, vertSize, inputLayoutName,
		indices.data(), indices.size());
	return true;
}

TexturePtr Mesh::GetTexture(size_t index)
{
	if (index < mTextures.size())
	{
		return mTextures[index];
	}
	else
	{
		return nullptr;
	}
}
