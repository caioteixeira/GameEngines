#pragma once
#include "Math.h"
#if _WIN32
#define WIN32_LEAN_AND_MEAN
#include <d3d11_1.h>
#endif
#include <memory>
#include <vector>

enum EBindflags
{
	EBF_VertexBuffer = D3D11_BIND_VERTEX_BUFFER,
	EBF_IndexBuffer = D3D11_BIND_INDEX_BUFFER,
	EBF_ConstantBuffer = D3D11_BIND_CONSTANT_BUFFER,
	EBF_ShaderResource = D3D11_BIND_SHADER_RESOURCE,
	EBF_RenderTarget = D3D11_BIND_RENDER_TARGET,
	EBF_DeptStencil = D3D11_BIND_DEPTH_STENCIL
};

enum EGraphicsBufferUsage
{
	EGBU_Default = D3D11_USAGE_DEFAULT,
	EGBU_Immutable = D3D11_USAGE_IMMUTABLE,
	EGBU_Dynamic = D3D11_USAGE_DYNAMIC,
	EGBU_Staging = D3D11_USAGE_STAGING,
};

enum ECPUAccessFlags
{
	ECPUAF_Neither = 0,
	ECPUAF_CanWrite = D3D11_CPU_ACCESS_WRITE,
	ECPUAF_CanRead = D3D11_CPU_ACCESS_READ,
};

enum EGFormat
{
	EGF_R32G32B32A32_Float = DXGI_FORMAT_R32G32B32A32_FLOAT,
	EGF_R32G32B32_Float = DXGI_FORMAT_R32G32B32_FLOAT,
	EGF_R32G32_Float = DXGI_FORMAT_R32G32_FLOAT,
	EGF_R8G8B8A8_UNorm = DXGI_FORMAT_R8G8B8A8_UNORM,
	EGF_R8G8B8A8_UInt = DXGI_FORMAT_R8G8B8A8_UINT,
};

enum EFillMode
{
	EFM_Wireframe = D3D11_FILL_WIREFRAME,
	EFM_Solid = D3D11_FILL_SOLID,
};

enum EPrimitiveTopology
{
	EPT_TriangleList = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
	EPT_TriangleStrip = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
	EPT_LineList = D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
	EPT_PointList = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,
};

enum EComparisonFunc
{
	ECF_Never = D3D11_COMPARISON_NEVER,
	ECF_Less = D3D11_COMPARISON_LESS,
	ECF_Equal = D3D11_COMPARISON_EQUAL,
	ECF_LessEqual = D3D11_COMPARISON_LESS_EQUAL,
	ECF_Greater = D3D11_COMPARISON_GREATER,
	ECF_NotEqual = D3D11_COMPARISON_NOT_EQUAL,
	ECF_GreaterEqual = D3D11_COMPARISON_GREATER_EQUAL,
	ECF_Always = D3D11_COMPARISON_ALWAYS,
};

enum ETextureFormat
{
	ETF_RGBA = DXGI_FORMAT_R8G8B8A8_UNORM,
	EXT_BGRA = DXGI_FORMAT_B8G8R8A8_UNORM,
};

struct InputLayoutElement : D3D11_INPUT_ELEMENT_DESC
{
	InputLayoutElement(const char* inSemanticName, uint32_t inSemanticIndex, EGFormat inFormat, uint32_t inByteOffset) 
	{
		SemanticName = inSemanticName;
		SemanticIndex = inSemanticIndex;
		Format = static_cast<DXGI_FORMAT>(inFormat);
		InputSlot = 0;
		AlignedByteOffset = inByteOffset;
		InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		InstanceDataStepRate = 0;
	}
};

typedef std::shared_ptr<ID3D11Buffer>				GraphicsBufferPtr;
typedef std::shared_ptr<ID3D11InputLayout>			InputLayoutPtr;
typedef std::shared_ptr<ID3D11PixelShader>			PixelShaderPtr;
typedef std::shared_ptr<ID3D11VertexShader>			VertexShaderPtr;
typedef std::shared_ptr<ID3D11RenderTargetView>		RenderTargetPtr;
typedef std::shared_ptr<ID3D11DepthStencilView>		DepthStencilPtr;
typedef std::shared_ptr<ID3D11SamplerState>			SamplerStatePtr;
typedef std::shared_ptr<ID3D11ShaderResourceView>	GraphicsTexturePtr;
typedef std::shared_ptr<ID3D11DepthStencilState>	DepthStencilStatePtr;
typedef std::shared_ptr<ID3D11RasterizerState>		RasterizerStatePtr;
typedef std::shared_ptr<ID3D11BlendState>			BlendStatePtr;


class GraphicsDriver 
{
public:

	GraphicsDriver(void* inWindow);
	~GraphicsDriver();

	RenderTargetPtr GetBackBufferRenderTarget() const
	{
		return mBackBufferRenderTarget;
	}

	bool CompileShaderFromFile(const char* inFileName, const char* szEntryPoint, const char* szShaderModel, std::vector<char>& outCompiledShaderCode);
	VertexShaderPtr CreateVertexShader(const std::vector<char>& inCompiledShaderCode);
	PixelShaderPtr CreatePixelShader(const std::vector<char>& inCompiledShaderCode);
	InputLayoutPtr CreateInputLayout(const InputLayoutElement* inElements, int inNumElements, const std::vector<char>& inCompiledVertexShader);
	GraphicsBufferPtr CreateGraphicsBuffer(const void* inRawData, int inRawDataSize, EBindflags inBindFlags, ECPUAccessFlags inCPUAccessFlags, EGraphicsBufferUsage inUsage);
	SamplerStatePtr CreateSamplerState();
	GraphicsTexturePtr CreateTextureFromFile(const char* inFileName, int& outWidth, int& outHeight);
	GraphicsTexturePtr CreateTextureFromMemory(const void* inPixels, int inWidth, int inHeight, ETextureFormat inTextureFormat);
	DepthStencilPtr CreateDepthStencil(int inWidth, int inHeight);
	DepthStencilStatePtr CreateDepthStencilState(bool inDepthTestEnable, EComparisonFunc inDepthComparisonFunction);
	RasterizerStatePtr CreateRasterizerState(EFillMode inFillMode);
	BlendStatePtr CreateBlendState(bool inEnableBlend);

	void SetRenderTarget(RenderTargetPtr inRenderTarget);
	void SetDepthStencil(DepthStencilPtr inDepthStencil);
	void SetViewport(float inX, float inY, float inWidth, float inHeight);
	void SetInputLayout(InputLayoutPtr inLayout);
	void SetPrimitiveTopology(EPrimitiveTopology inTopology);
	void SetVertexBuffer(GraphicsBufferPtr inBuffer, uint32_t inVertexSize);
	void SetIndexBuffer(GraphicsBufferPtr inBuffer);
	void SetVertexShader(VertexShaderPtr inVertexShader);
	void SetVSConstantBuffer(GraphicsBufferPtr inBuffer, int inStartSlot);
	void SetPixelShader(PixelShaderPtr inPixelShader);
	void SetPSConstantBuffer(GraphicsBufferPtr inBuffer, int inStartSlot);
	void SetPSSamplerState(SamplerStatePtr inSamplerState, int inStartSlot);
	void SetPSTexture(GraphicsTexturePtr inTexture, int inStartSlot);
	void SetDepthStencilState(DepthStencilStatePtr inDepthStencilState);
	void SetRasterizerState(RasterizerStatePtr inRasterizerStatePtr);
	void SetBlendState(BlendStatePtr inBlendState);

	void* MapBuffer(GraphicsBufferPtr inBuffer);
	void UnmapBuffer(GraphicsBufferPtr inBuffer);

	template<typename T>
	T* MapBuffer(GraphicsBufferPtr inBuffer)
	{
		return static_cast<T*>(MapBuffer(inBuffer));
	}


	void ClearBackBuffer(const Vector3& inColor, float inAlpha);
	void ClearRenderTarget(RenderTargetPtr inRenderTarget, const Vector3& inColor, float inAlpha);
	void ClearDepthStencil(DepthStencilPtr inDepthStencil, float inDepth);
	void Draw(int inVertexCount, int inStartVertexIndex);
	void DrawIndexed(int inIndexCount, int inStartIndexLocation, int inBaseVertexLocation);
	void Present();

	uint32_t GetWindowWidth() const { return mWindowWidth; }
	uint32_t GetWindowHeight() const { return mWindowHeight; }

private:
	uint32_t mWindowWidth, mWindowHeight;

	RenderTargetPtr mBackBufferRenderTarget;

	RenderTargetPtr	mCurrentRenderTarget;
	DepthStencilPtr mCurrentDepthStencil;

};
