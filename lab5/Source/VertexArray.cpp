#include "ITPEnginePCH.h"

VertexArrayPtr VertexArray::Create(GraphicsDriver& graphics, InputLayoutCache& inputCache, 
	const void* verts, size_t vertCount, size_t vertSize, const std::string& inputLayoutName,
	const void* indices, size_t indexCount, size_t indexSize)
{
	return std::make_shared<VertexArray>(graphics, inputCache, 
		verts, vertCount, vertSize, inputLayoutName, 
		indices, indexCount, indexSize);
}

VertexArray::VertexArray(GraphicsDriver& graphics, InputLayoutCache& inputCache,
	const void* verts, size_t vertCount, size_t vertSize, const std::string& inputLayoutName,
	const void* indices, size_t indexCount, size_t indexSize)
	:mGraphics(graphics)
{
	mVertexCount = vertCount;
	mVertexSize = vertSize;
	mIndexCount = indexCount;

	mInputLayout = inputCache.GetLayout(inputLayoutName);
	mVertexBuffer = graphics.CreateGraphicsBuffer(verts, vertCount * vertSize, EBF_VertexBuffer, ECPUAF_Neither, EGBU_Immutable);
	mIndexBuffer = graphics.CreateGraphicsBuffer(indices, indexCount * indexSize, EBF_IndexBuffer, ECPUAF_Neither, EGBU_Immutable);
}

VertexArray::~VertexArray()
{

}

void VertexArray::SetActive()
{
	mGraphics.SetInputLayout(mInputLayout);
	mGraphics.SetVertexBuffer(mVertexBuffer, mVertexSize);
	mGraphics.SetIndexBuffer(mIndexBuffer);
}
