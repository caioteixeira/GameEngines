// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

//=============================================================================
// StaticMeshExporterOBJ
//=============================================================================

#pragma once
#include "Exporters/Exporter.h"
#include "AnimExporterITP.generated.h"

UCLASS()
class UAnimExporterITP : public UExporter
{
	GENERATED_UCLASS_BODY()


	// Begin UExporter Interface
	virtual bool ExportText(const FExportObjectInnerContext* Context, UObject* Object, const TCHAR* Type, FOutputDevice& Ar, FFeedbackContext* Warn, uint32 PortFlags = 0) override;
};



