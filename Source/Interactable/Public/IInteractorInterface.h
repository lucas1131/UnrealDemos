#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IInteractorInterface.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnInteractableUpdatedSignature, UInteractableComponent*, InteractableComponent);

UINTERFACE(MinimalAPI, BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class UInteractorInterface : public UInterface
{
	GENERATED_BODY()
};

class IInteractorInterface
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	virtual bool TryBeginInteraction() = 0;
	UFUNCTION(BlueprintCallable)
	virtual bool TryEndInteraction() = 0;
	UFUNCTION(BlueprintCallable)
	virtual void BindOnInteractableUpdatedEvent(const FOnInteractableUpdatedSignature& Callback) = 0;
	UFUNCTION(BlueprintCallable)
	virtual float GetInteractionDistance() = 0;
	UFUNCTION(BlueprintCallable)
	virtual FVector GetHoldInFrontLocation() = 0;
	UFUNCTION(BlueprintCallable)
	virtual FRotator GetHoldInFrontRotation() = 0;
};