// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#include "ITPMeshExporterPrivatePCH.h"
#include "SkeletalMeshExporterITP.h"
#include "ITPTextureExporterBMP.h"


USkeletalMeshExporterITP::USkeletalMeshExporterITP(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SupportedClass = USkeletalMesh::StaticClass();
	bText = true;
	PreferredFormatIndex = 0;
	FormatExtension.Add(TEXT("itpmesh2"));
	FormatDescription.Add(TEXT("ITP Mesh File"));

	BMPExporter = CreateDefaultSubobject<UITPTextureExporterBMP>(TEXT("BMPExporter"));
}

bool USkeletalMeshExporterITP::ExportText(const FExportObjectInnerContext* Context, UObject* Object, const TCHAR* Type, FOutputDevice& Ar, FFeedbackContext* Warn, uint32 PortFlags /*= 0*/)
{
	USkeletalMesh* SkeletalMesh = CastChecked<USkeletalMesh>(Object);

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
	if (SkeletalMesh->Materials.Num() > 0 && SkeletalMesh->Materials[0].MaterialInterface &&
		SkeletalMesh->Materials[0].MaterialInterface->GetTexturesInPropertyChain(MP_BaseColor, textures, nullptr, nullptr))
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
	File->Logf(TEXT("\t\t{ \"name\":\"bones\", \"type\":\"byte\", \"count\":4 },"));
	File->Logf(TEXT("\t\t{ \"name\":\"weights\", \"type\":\"byte\", \"count\":4 },"));
	File->Logf(TEXT("\t\t{ \"name\":\"texcoord\", \"type\":\"float\", \"count\":2 }"));
	File->Logf(TEXT("\t],"));

	File->Logf(TEXT("\t\"shader\":{"));
	File->Logf(TEXT("\t\t\"name\":\"Skinned\""));
	File->Logf(TEXT("\t},"));

	const FSkeletalMeshResource* MeshResource = SkeletalMesh->GetImportedResource();
	const FStaticLODModel* Model = &(MeshResource->LODModels[0]);
	TArray<FSoftSkinVertex> Vertices;
	GetVertices(Model, Vertices);
	uint32 VertexCount = Vertices.Num();

	// Write the vertices
	File->Logf(TEXT("\t\"vertices\":["));
	for (uint32 i = 0; i < VertexCount; i++)
	{
		FSoftSkinVertex& v = Vertices[i];
		FVector Norm(v.TangentZ);
		uint8* b = Vertices[i].InfluenceBones;
		uint8* w = Vertices[i].InfluenceWeights;
		if (i < VertexCount - 1)
		{
			File->Logf(TEXT("\t\t[%f,%f,%f,%f,%f,%f,%u,%u,%u,%u,%u,%u,%u,%u,%f,%f],"),
				v.Position.X, v.Position.Y, v.Position.Z,
				Norm.X, Norm.Y, Norm.Z,
				b[0], b[1], b[2], b[3],
				w[0], w[1], w[2], w[3],
				v.UVs[0].X, v.UVs[0].Y);
		}
		else
		{
			File->Logf(TEXT("\t\t[%f,%f,%f,%f,%f,%f,%u,%u,%u,%u,%u,%u,%u,%u,%f,%f]"),
				v.Position.X, v.Position.Y, v.Position.Z,
				Norm.X, Norm.Y, Norm.Z,
				b[0], b[1], b[2], b[3],
				w[0], w[1], w[2], w[3],
				v.UVs[0].X, v.UVs[0].Y);
		}
	}

	File->Logf(TEXT("\t],"));

 
 	// Write the indices
 	File->Logf(TEXT("\t\"indices\":["));
 
	uint32 tempSize = Model->Size;
	TArray<uint32> Indices;
	Model->MultiSizeIndexContainer.GetIndexBuffer(Indices);
	uint8 dataTypeSize = Model->MultiSizeIndexContainer.GetDataTypeSize();
 	uint32 NumIndices = Indices.Num();
	uint32 CurrIdx = 0;
 
 	check(NumIndices % 3 == 0);

	int32 SectionCount = Model->Sections.Num();
	for (int32 SectionIndex = 0; SectionIndex < SectionCount; ++SectionIndex)
	{
		const FSkelMeshSection& Section = Model->Sections[SectionIndex];

		// Static meshes contain one triangle list per element.
		int32 TriangleCount = Section.NumTriangles;

		// Copy over the index buffer into the FBX polygons set.
		for (int32 TriangleIndex = 0; TriangleIndex < TriangleCount; ++TriangleIndex)
		{
			uint32 a = Indices[Section.BaseIndex + ((TriangleIndex * 3) + 0)];
			uint32 b = Indices[Section.BaseIndex + ((TriangleIndex * 3) + 1)];
			uint32 c = Indices[Section.BaseIndex + ((TriangleIndex * 3) + 2)];
			// Is this the last one?
			if (SectionIndex == (SectionCount - 1) && TriangleIndex == (TriangleCount - 1))
			{
				File->Logf(TEXT("\t\t[%u,%u,%u]"), a, b, c);
			}
			else
			{
				File->Logf(TEXT("\t\t[%u,%u,%u],"), a, b, c);
			}
		}
	}

	File->Logf(TEXT("\t]"));
 	File->Logf(TEXT("}"));
 	delete File;

	// Now export the skeleton
	return ExportSkeleton(SkeletalMesh->RefSkeleton);
}

bool USkeletalMeshExporterITP::ExportSkeleton(const FReferenceSkeleton& RefSkeleton)
{
	if (RefSkeleton.GetNum() == 0)
	{
		return false;
	}

	FString FileName;
	int32 startIdx;
	int32 endIdx;
	if (UExporter::CurrentFilename.FindLastChar('/', startIdx) &&
		UExporter::CurrentFilename.FindLastChar('.', endIdx))
	{
		FString skeletonName = UExporter::CurrentFilename.Mid(startIdx + 1, endIdx - startIdx - 1);

		FileName = UExporter::CurrentFilename.Mid(0, startIdx + 1);
		FileName += skeletonName;
		FileName += ".itpskel";
	}

	// Open file for output
	FArchive* File = IFileManager::Get().CreateFileWriter(*FileName);

	File->Logf(TEXT("{"));
	File->Logf(TEXT("\t\"metadata\":{"));
	File->Logf(TEXT("\t\t\"type\":\"itpskel\","));
	File->Logf(TEXT("\t\t\"version\":1"));
	File->Logf(TEXT("\t},"));

	File->Logf(TEXT("\t\"bonecount\":%d,"), RefSkeleton.GetNum());

	File->Logf(TEXT("\t\"bones\":["));

	for (int32 BoneIndex = 0; BoneIndex < RefSkeleton.GetNum(); ++BoneIndex)
	{
		const FMeshBoneInfo& CurrentBone = RefSkeleton.GetRefBoneInfo()[BoneIndex];
		const FTransform& BoneTransform = RefSkeleton.GetRefBonePose()[BoneIndex];
		File->Logf(TEXT("\t\t{"));

		File->Logf(TEXT("\t\t\t\"name\":\"%s\","), *CurrentBone.ExportName);

		if (BoneIndex != 0)
		{
			File->Logf(TEXT("\t\t\t\"parent\":%d,"), CurrentBone.ParentIndex);
		}
		else
		{
			// Root node
			File->Logf(TEXT("\t\t\t\"parent\":-1,"));
		}

		File->Logf(TEXT("\t\t\t\"bindpose\":{"));
		// I'm assuming we don't need scale, because the FBX export doesn't seem to export scale, either...
		FQuat rot = BoneTransform.GetRotation();
		File->Logf(TEXT("\t\t\t\t\"rot\":[%f,%f,%f,%f],"), rot.X, rot.Y, rot.Z, rot.W);
		FVector trans = BoneTransform.GetTranslation();
		File->Logf(TEXT("\t\t\t\t\"trans\":[%f,%f,%f]"), trans.X, trans.Y, trans.Z);
		File->Logf(TEXT("\t\t\t}"));

		if (BoneIndex == RefSkeleton.GetNum() - 1)
		{
			File->Logf(TEXT("\t\t}"));
		}
		else
		{
			File->Logf(TEXT("\t\t},"));
		}
	}

	File->Logf(TEXT("\t]"));

	File->Logf(TEXT("}"));
	delete File;

	return true;
}

void USkeletalMeshExporterITP::GetVertices(const class FStaticLODModel* Model, TArray<FSoftSkinVertex>& Vertices) const
{
	Vertices.Empty(Model->NumVertices);
	Vertices.AddUninitialized(Model->NumVertices);

	// Initialize the vertex data
	// All chunks are combined into one (rigid first, soft next)
	FSoftSkinVertex* DestVertex = (FSoftSkinVertex*)Vertices.GetData();
	for (int32 ChunkIndex = 0; ChunkIndex < Model->Chunks.Num(); ChunkIndex++)
	{
		const FSkelMeshChunk& Chunk = Model->Chunks[ChunkIndex];
		//check(Chunk.NumRigidVertices == Chunk.RigidVertices.Num());
		//check(Chunk.NumSoftVertices == Chunk.SoftVertices.Num());
		for (int32 VertexIndex = 0; VertexIndex < Chunk.RigidVertices.Num(); VertexIndex++)
		{
			const FRigidSkinVertex& SourceVertex = Chunk.RigidVertices[VertexIndex];
			DestVertex->Position = SourceVertex.Position;
			DestVertex->TangentX = SourceVertex.TangentX;
			DestVertex->TangentY = SourceVertex.TangentY;
			DestVertex->TangentZ = SourceVertex.TangentZ;
			// store the sign of the determinant in TangentZ.W
			DestVertex->TangentZ.Vector.W = GetBasisDeterminantSignByte(SourceVertex.TangentX, SourceVertex.TangentY, SourceVertex.TangentZ);

			// copy all texture coordinate sets
			FMemory::Memcpy(DestVertex->UVs, SourceVertex.UVs, sizeof(FVector2D)*MAX_TEXCOORDS);

			DestVertex->Color = SourceVertex.Color;
			// Sanjay: Changed this to lookup in the bone map for the chunk
			DestVertex->InfluenceBones[0] = Chunk.BoneMap[SourceVertex.Bone];
			DestVertex->InfluenceWeights[0] = 255;
			for (int32 InfluenceIndex = 1; InfluenceIndex < MAX_TOTAL_INFLUENCES; InfluenceIndex++)
			{
				DestVertex->InfluenceBones[InfluenceIndex] = 0;
				DestVertex->InfluenceWeights[InfluenceIndex] = 0;
			}
			DestVertex++;
		}
		//FMemory::Memcpy(DestVertex, Chunk.SoftVertices.GetData(), Chunk.SoftVertices.Num() * sizeof(FSoftSkinVertex));
		//DestVertex += Chunk.SoftVertices.Num();
		// Sanjay: Manually copy the soft vertex data as well, so we can use the bone map
		for (int32 VertexIndex = 0; VertexIndex < Chunk.SoftVertices.Num(); VertexIndex++)
		{
			const FSoftSkinVertex& SourceVertex = Chunk.SoftVertices[VertexIndex];
			DestVertex->Position = SourceVertex.Position;
			DestVertex->TangentX = SourceVertex.TangentX;
			DestVertex->TangentY = SourceVertex.TangentY;
			DestVertex->TangentZ = SourceVertex.TangentZ;

			// copy all texture coordinate sets
			FMemory::Memcpy(DestVertex->UVs, SourceVertex.UVs, sizeof(FVector2D)*MAX_TEXCOORDS);

			DestVertex->Color = SourceVertex.Color;
			// Sanjay: Changed this to lookup in the bone map for the chunk
			for (int32 InfluenceIndex = 0; InfluenceIndex < MAX_TOTAL_INFLUENCES; InfluenceIndex++)
			{
				DestVertex->InfluenceBones[InfluenceIndex] = Chunk.BoneMap[SourceVertex.InfluenceBones[InfluenceIndex]];
				DestVertex->InfluenceWeights[InfluenceIndex] = SourceVertex.InfluenceWeights[InfluenceIndex];
			}
			DestVertex++;
		}
	}
}
