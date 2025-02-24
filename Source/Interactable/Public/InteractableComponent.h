// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractorInterface.h"
#include "Components/ActorComponent.h"
#include "InteractableComponent.generated.h"

struct FInteractionData;
class URaycastInteractorComponent;

UCLASS(Blueprintable, ClassGroup=(InteractableSystem), meta=(BlueprintSpawnableComponent))
class INTERACTABLE_API UInteractableComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;
	/* Name or description of the object or action, intended for UI */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString InteractionDescription;
	/* Is component is allowed to start new interaction, similar to being active */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanInteract;
	/* Is component already in the middle of interacting with something else? */
	UPROPERTY(BlueprintReadOnly)
	bool bIsInteracting;

	/* Enabling this will enable render custom depth pass for this actor's meshes. This requires Custom Depth to be
	 * enabled with stencil in project settings. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bEnableHighlightEffects;
	/* Control if it should apply highlight effect to owner actor to how it can be interacted with */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bStartWithHighlightedActor;

public:
	UInteractableComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void EnableHighlightEffects(bool Enabled);
	FString& GetDisplayDescription() { return InteractionDescription; }

	bool CanInteract() const { return bCanInteract && !bIsInteracting; }

	UFUNCTION(BlueprintCallable)
	void ActivateInteractionHighlight() const;
	UFUNCTION(BlueprintCallable)
	void DeactivateInteractionHighlight() const;
	UFUNCTION(BlueprintCallable)
	void ActivateInteractionOutline() const;
	UFUNCTION(BlueprintCallable)
	void DeactivateInteractionOutline() const;

	/* How to deal with different types of interaction
	 * I can think of two types of interaction here:
	 * - A FireAndForget type, you trigger an event and forget about the interaction, the object does what it needs to do, and it happens in a single frame (from the interactor perspective)
	 *    - For example a Collect Item or Push Button just need to trigger something and end interaction automatically, for the interactor it's instantaneous 
	 * - A type of interaction that needs to happen during multiple frames and need to control the state of the interaction
	 *    - For example a Grab Object that needs to keep holding and updating de object position every frame until another input ends the interaction, it's not automatic
	 *
	 *    |---------------------------|
	 *    | FireAndForget | HoldState |
	 *    |---------------------------|
	 *    | Use/Active    | Grab      |
	 *    | Collect       | Talk      |
	 *    | Throw         | Drag      |
	 *    | Break         | Ride      |
	 *    |---------------------------|
	 *    
	 */

	/* Blueprint events */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, meta=(DisplayName="OnBeginInteraction"))
	void ReceiveOnBeginInteraction(AActor* Interactor,
	                               const TScriptInterface<IInteractorInterface>& InteractorComponent);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, meta=(DisplayName="OnEndInteraction"))
	void ReceiveOnEndInteraction(AActor* Interactor, const TScriptInterface<IInteractorInterface>& InteractorComponent);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, meta=(DisplayName="OnReceivedData"))
	void ReceiveOnHandleNewData(const FInteractionData& Data);

	/* Blueprint events entry point */
	UFUNCTION(BlueprintCallable, meta=(DisplayName="OnBeginInteraction"))
	void BeginInteraction(AActor* Interactor, const TScriptInterface<IInteractorInterface>& InteractorComponent);
	UFUNCTION(BlueprintCallable, meta=(DisplayName="OnEndInteraction"))
	void EndInteraction(AActor* Interactor, const TScriptInterface<IInteractorInterface>& InteractorComponent);

protected:
	virtual void BeginPlay() override;
};
