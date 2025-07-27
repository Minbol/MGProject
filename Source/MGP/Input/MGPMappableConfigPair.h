#pragma once

#include "MGPMappableConfigPair.generated.h"

/** forward declaration */
class UPlayerMappableInputConfig;

USTRUCT()
struct FMGPMappableConfigPair
{
	GENERATED_BODY()
public:
	FMGPMappableConfigPair() = default;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UPlayerMappableInputConfig> Config;

	UPROPERTY(EditAnywhere)
	bool bShouldActivateAutomatically = true;
};