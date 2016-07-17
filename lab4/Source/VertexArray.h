// VertexArray.h
// Wraps vertex/index buffers

#pragma once
#include "Math.h"
#include <memory>
#include "GraphicsDriver.h"
#include "InputLayoutCache.h"

typedef std::shared_ptr<class VertexArray> VertexArrayPtr;

class VertexArray
{
public:
	static VertexArrayPtr Create(GraphicsDriver& graphics, InputLayoutCache& inputCache,
		const void* verts, size_t vertCount, size_t vertSize, const std::string& inputLayoutName, 
		const void* indices, size_t indexCount, size_t indexSize = 2);
	VertexArray(GraphicsDriver& graphics, InputLayoutCache& inputCache,
		const void* verts, size_t vertCount, size_t vertSize, const std::string& inputLayoutName,
		const void* indices, size_t indexCount, size_t indexSize);
public:
	~VertexArray();
	void SetActive();

	size_t GetVertexCount() const { return mVertexCount; }
	size_t GetVertexSize() const { return mVertexSize; }
	size_t GetIndexCount() const { return mIndexCount; }
	GraphicsBufferPtr GetVertexBuffer() const { return mVertexBuffer; }
	GraphicsBufferPtr GetIndexBuffer() const { return mIndexBuffer; }
private:
	GraphicsDriver& mGraphics;
	GraphicsBufferPtr mVertexBuffer;
	GraphicsBufferPtr mIndexBuffer;
	InputLayoutPtr mInputLayout;
	size_t mVertexCount;
	size_t mVertexSize;
	size_t mIndexCount;
};
