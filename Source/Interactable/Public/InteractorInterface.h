#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractorInterface.generated.h"

class UInteractableComponent;

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
	virtual UInteractableComponent* GetCurrentInteractable() = 0;
	UFUNCTION(BlueprintCallable)
	virtual void SetCurrentInteractable(UInteractableComponent* Interactable) = 0;
	UFUNCTION(BlueprintCallable)
	virtual void ClearCurrentInteractable() = 0;
	UFUNCTION(BlueprintCallable)
	virtual void SendDataToInteractable(const FInteractionData& Data) = 0;
	UFUNCTION(BlueprintCallable)
	virtual void BindOnInteractableUpdatedEvent(const FOnInteractableUpdatedSignature& Callback) = 0;
	UFUNCTION(BlueprintCallable)
	virtual float GetInteractionDistance() = 0;
};