#pragma once

#include "CoreMinimal.h"
#include "LevelSelectData.generated.h"

UCLASS(BlueprintType)
class ULevelSelectData : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName LevelName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UWorld> Level;
};
