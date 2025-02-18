// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IInteractorInterface.h"
#include "Components/ActorComponent.h"
#include "InteractableComponent.generated.h"

class URaycastInteractorComponent;

UCLASS(Blueprintable, ClassGroup=(InteractableSystem), meta=(BlueprintSpawnableComponent))
class INTERACTABLE_API UInteractableComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DisplayDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanInteract;

	/* Enabling this will enable render custom depth pass for this actor's meshes. This requires Custom Depth to be
	 * enabled with stencil in project settings. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bEnableHighlightEffects;
	/* Control if it should apply highlight effect to owner actor to how it can be interacted with */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bStartWithHighlightedActor;
	UPROPERTY(BlueprintReadOnly)
	bool bIsInteracting;
	
public:
	UInteractableComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void EnableHighlightEffects(bool Enabled);
	FString& GetDisplayDescription() { return DisplayDescription; }

	bool CanInteract() const { return bCanInteract && !bIsInteracting; }

	UFUNCTION(BlueprintCallable)
	void ActivateInteractionHighlight() const;
	UFUNCTION(BlueprintCallable)
	void DeactivateInteractionHighlight() const;
	UFUNCTION(BlueprintCallable)
	void ActivateInteractionOutline() const;
	UFUNCTION(BlueprintCallable)
	void DeactivateInteractionOutline() const;
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, meta=(DisplayName="OnBeginInteraction"))
	void ReceiveOnBeginInteraction(AActor* Interactor, const TScriptInterface<IInteractorInterface>& InteractorComponent);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, meta=(DisplayName="OnEndInteraction"))
	void ReceiveOnEndInteraction(AActor* Interactor, const TScriptInterface<IInteractorInterface>& InteractorComponent);

	UFUNCTION(BlueprintCallable, meta=(DisplayName="OnBeginInteraction"))
	bool BeginInteraction(AActor* Interactor, const TScriptInterface<IInteractorInterface>& InteractorComponent);
	UFUNCTION(BlueprintCallable, meta=(DisplayName="OnEndInteraction"))
	bool EndInteraction(AActor* Interactor, const TScriptInterface<IInteractorInterface>& InteractorComponent);


protected:
	virtual void BeginPlay() override;
};
