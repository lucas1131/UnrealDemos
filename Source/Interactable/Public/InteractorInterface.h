#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractorInterface.generated.h"

struct FInteractionData;
class UInteractableComponent;

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnInteractableUpdatedSignature, UInteractableComponent*, InteractableComponent);

// TODO seeing how the classes that implement this turned out, this should probably become a base class instead of an Interface
// the only difference between the classes is how they find their interactables and that logic is 100% private in the class
// A better design for this would be to make the detector a component (Raycast or Volume detector) and keep just a single interactor class 
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
	virtual void AttachInteractable(UInteractableComponent* Interactable) = 0;
	UFUNCTION(BlueprintCallable)
	virtual void DetachInteractable() = 0;
	UFUNCTION(BlueprintCallable)
	virtual void SendDataToInteractable(const FInteractionData& Data) = 0;
	UFUNCTION(BlueprintCallable)
	virtual void BindOnInteractableUpdatedEvent(const FOnInteractableUpdatedSignature& Callback) = 0;
};

