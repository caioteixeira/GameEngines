// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

//=============================================================================
// StaticMeshExporterOBJ
//=============================================================================

#pragma once
#include "Exporters/Exporter.h"
#include "SkeletalMeshExporterITP.generated.h"

UCLASS()
class USkeletalMeshExporterITP : public UExporter
{
	GENERATED_UCLASS_BODY()


	// Begin UExporter Interface
	virtual bool ExportText(const FExportObjectInnerContext* Context, UObject* Object, const TCHAR* Type, FOutputDevice& Ar, FFeedbackContext* Warn, uint32 PortFlags = 0) override;
	// End UExporter Interface
	virtual bool ExportSkeleton(const struct FReferenceSkeleton& RefSkeleton);

	void GetVertices(const class FStaticLODModel* Model, TArray<FSoftSkinVertex>& Vertices) const;

	class UITPTextureExporterBMP* BMPExporter;
};



