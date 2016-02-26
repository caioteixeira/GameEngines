// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#include "ITPMeshExporterPrivatePCH.h"
#include "StaticMeshExporterITP.h"
#include "ITPTextureExporterBMP.h"


UStaticMeshExporterITP::UStaticMeshExporterITP(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SupportedClass = UStaticMesh::StaticClass();
	bText = true;
	PreferredFormatIndex = 0;
	FormatExtension.Add(TEXT("itpmesh2"));
	FormatDescription.Add(TEXT("ITP Mesh File"));

	BMPExporter = CreateDefaultSubobject<UITPTextureExporterBMP>(TEXT("BMPExporter"));
}

bool UStaticMeshExporterITP::ExportText(const FExportObjectInnerContext* Context, UObject* Object, const TCHAR* Type, FOutputDevice& Ar, FFeedbackContext* Warn, uint32 PortFlags /*= 0*/)
{
	UStaticMesh* StaticMesh = CastChecked<UStaticMesh>(Object);

	// Open another archive
	FArchive* File = IFileManager::Get().CreateFileWriter(*UExporter::CurrentFilename);

	// Let's try the header...
	File->Logf(TEXT("{"));
	File->Logf(TEXT("\t\"metadata\":{"));
	File->Logf(TEXT("\t\t\"type\":\"itpmesh\","));
	File->Logf(TEXT("\t\t\"version\":2"));
	File->Logf(TEXT("\t},"));

	File->Logf(TEXT("\t\"textures\":["));
	
	// See if there are any textures
	FString texturePath;
	TArray<UTexture*> textures;
	if (StaticMesh->GetMaterial(0) &&
		StaticMesh->GetMaterial(0)->GetTexturesInPropertyChain(MP_BaseColor, textures, nullptr, nullptr))
	{
		if (textures.Num() > 0)
		{
			int32 startIdx;
			int32 endIdx;
			if (UExporter::CurrentFilename.FindLastChar('/', startIdx) &&
				UExporter::CurrentFilename.FindLastChar('.', endIdx))
			{
				FString textureName = UExporter::CurrentFilename.Mid(startIdx + 1, endIdx - startIdx - 1);
				texturePath = "Textures/";
				texturePath += textureName;
				texturePath += ".bmp";

				FString outputPath = UExporter::CurrentFilename.Mid(0, startIdx + 1);
				outputPath += textureName;
				outputPath += ".bmp";

				UExporter::ExportToFile(textures[0], BMPExporter, *outputPath, false);
			}
		}
	}

	if (texturePath.IsEmpty())
	{
		File->Logf(TEXT("\t\t\"Textures/Default.png\""));
	}
	else
	{
		File->Logf(TEXT("\t\t\"%s\""), *texturePath);
	}

	File->Logf(TEXT("\t],"));

	File->Logf(TEXT("\t\"vertexformat\":["));
	File->Logf(TEXT("\t\t{ \"name\":\"position\", \"type\":\"float\", \"count\":3 },"));
	File->Logf(TEXT("\t\t{ \"name\":\"normal\", \"type\":\"float\", \"count\":3 },"));
	File->Logf(TEXT("\t\t{ \"name\":\"texcoord\", \"type\":\"float\", \"count\":2 }"));
	File->Logf(TEXT("\t],"));

	File->Logf(TEXT("\t\"shader\":{"));
	File->Logf(TEXT("\t\t\"name\":\"BasicMesh\""));
	File->Logf(TEXT("\t},"));

	// Currently, we only export LOD 0 of the static mesh. In the future, we could potentially export all available LODs
	const FStaticMeshLODResources& RenderData = StaticMesh->GetLODForExport(0);
	uint32 VertexCount = RenderData.GetNumVertices();

	check(VertexCount == RenderData.VertexBuffer.GetNumVertices());

	// Write the vertices
	File->Logf(TEXT("\t\"vertices\":["));
	for (uint32 i = 0; i < VertexCount; i++)
	{
		const FVector& Pos = RenderData.PositionVertexBuffer.VertexPosition(i);
		const FVector& Normal = RenderData.VertexBuffer.VertexTangentZ(i);
		const FVector2D UV = RenderData.VertexBuffer.GetVertexUV(i, 0);

		if (i < VertexCount - 1)
		{
			File->Logf(TEXT("\t\t[%f,%f,%f,%f,%f,%f,%f,%f],"),
				Pos.X, Pos.Y, Pos.Z,
				Normal.X, Normal.Y, Normal.Z,
				UV.X, UV.Y);
		}
		else
		{
			File->Logf(TEXT("\t\t[%f,%f,%f,%f,%f,%f,%f,%f]"),
				Pos.X, Pos.Y, Pos.Z,
				Normal.X, Normal.Y, Normal.Z,
				UV.X, UV.Y);
		}
	}

	File->Logf(TEXT("\t],"));

	// Write the indices
	File->Logf(TEXT("\t\"indices\":["));

	FIndexArrayView Indices = RenderData.IndexBuffer.GetArrayView();
	uint32 NumIndices = Indices.Num();

	check(NumIndices % 3 == 0);
	for (uint32 i = 0; i < NumIndices / 3; i++)
	{
		// Wavefront indices are 1 based
		uint32 a = Indices[3 * i + 0];
		uint32 b = Indices[3 * i + 1];
		uint32 c = Indices[3 * i + 2];

		if (i < (NumIndices / 3 - 1))
		{
			File->Logf(TEXT("\t\t[%u,%u,%u],"), a, b, c);
		}
		else
		{
			File->Logf(TEXT("\t\t[%u,%u,%u]"), a, b, c);
		}
	}

	File->Logf(TEXT("\t]"));
	File->Logf(TEXT("}"));
	delete File;

	return true;
}
