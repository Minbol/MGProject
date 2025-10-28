// Fill out your copyright notice in the Description page of Project Settings.


#include "UIExtensionSystem.h"

#include "Blueprint/UserWidget.h"

void FUIExtensionHandle::Unregister()
{
	if ( UUIExtensionSubsystem* ExtensionSubsystem = ExtensionSource.Get() )
	{
		ExtensionSubsystem->UnRegisterExtension( *this );
	}
}

bool FUIExtensionPoint::DoesExtensionPassContract(const FUIExtension* InExtension)
{
	if ( UObject* DataPtr = InExtension->Data )
	{
		const bool bMatchContext = ( ContextObject.IsExplicitlyNull() && InExtension->ContextObject.IsExplicitlyNull() ) || ( ContextObject == InExtension->ContextObject );

		if ( bMatchContext )
		{
			const UClass* DataClass = DataPtr->IsA(UClass::StaticClass()) ? Cast<UClass>(DataPtr) : DataPtr->GetClass();
			for ( const auto* AllowedClass : AllowedDataClasses )
			{
				if ( DataClass->IsChildOf( AllowedClass ) || DataClass->ImplementsInterface( AllowedClass ) )
				{
					return true;
				}
			}
		}
	}

	return false;
}

void FUIExtensionPointHandle::Unregister()
{
	if ( UUIExtensionSubsystem* ExtensionSubsystem = ExtensionSource.Get() )
	{
		ExtensionSubsystem->UnRegisterExtensionPoint( *this );
	}
}

void UUIExtensionSubsystem::UnRegisterExtension(const FUIExtensionHandle& InHandle)
{
	if ( InHandle.IsValid() )
	{
		TSharedPtr<FUIExtension> Extension = InHandle.DataPtr;
		if ( FExtensionList* ListPtr = ExtensionMap.Find( Extension->ExtensionPointTag ) )
		{
			NotifyExtensionPointsOfExtension( EUIExtensionAction::Removed, Extension );
			
			ListPtr->RemoveSwap( Extension );
			if ( ListPtr->Num() == 0 )
			{
				ExtensionMap.Remove( Extension->ExtensionPointTag );
			}
		}
	}
}

FUIExtensionHandle UUIExtensionSubsystem::RegisterExtensionAsWidgetForContext(const FGameplayTag& InExtensionPointTag, UObject* InContextObject, TSubclassOf<UUserWidget> WidgetClass, int32 Priority)
{
	return RegisterExtensionAsData( InExtensionPointTag, InContextObject, WidgetClass, Priority );
}

FUIExtensionHandle UUIExtensionSubsystem::RegisterExtensionAsData(const FGameplayTag& InExtensionPointTag, UObject* InContextObject, UObject* InData, int32 Priority)
{
	if ( !InExtensionPointTag.IsValid() )
	{
		return FUIExtensionHandle();
	}

	if ( InData == nullptr )
	{
		return FUIExtensionHandle();
	}

	FExtensionList& List = ExtensionMap.FindOrAdd( InExtensionPointTag );

	TSharedPtr<FUIExtension>& Entry = List.Add_GetRef( MakeShared<FUIExtension>() );
	Entry->ExtensionPointTag = InExtensionPointTag;
	Entry->Data = InData;
	Entry->ContextObject = InContextObject;
	Entry->Priority = Priority;

	NotifyExtensionPointsOfExtension( EUIExtensionAction::Added, Entry );

	return FUIExtensionHandle( this, Entry );
}

void UUIExtensionSubsystem::UnRegisterExtensionPoint(const FUIExtensionPointHandle& InHandle)
{
	if ( InHandle.IsValid() )
	{
		TSharedPtr<FUIExtensionPoint> ExtensionPoint = InHandle.DataPtr;
		if ( FExtensionPointList* ListPtr = ExtensionPointMap.Find( ExtensionPoint->ExtensionPointTag ) )
		{
			ListPtr->RemoveSwap( ExtensionPoint );
			if ( ListPtr->Num() == 0 )
			{
				ExtensionMap.Remove( ExtensionPoint->ExtensionPointTag );
			}
		}
	}
}

FUIExtensionPointHandle UUIExtensionSubsystem::RegisterExtensionPointForContext(const FGameplayTag& InExtensionPointTag,
	UObject* InContextObject, EUIExtensionPointMatch InExtensionPointTagMatchType,
	const TArray<UClass*>& InAllowedDataClasses, FExtendExtensionPointDelegate InCallback)
{
	if ( !InExtensionPointTag.IsValid() )
	{
		return FUIExtensionPointHandle();
	}

	if ( !InCallback.IsBound() )
	{
		return FUIExtensionPointHandle();
	}

	if ( InAllowedDataClasses.Num() == 0 )
	{
		return FUIExtensionPointHandle();
	}

	FExtensionPointList& List = ExtensionPointMap.FindOrAdd( InExtensionPointTag );
	TSharedPtr<FUIExtensionPoint>& Entry = List.Add_GetRef( MakeShared<FUIExtensionPoint>() );
	Entry->ExtensionPointTag = InExtensionPointTag;
	Entry->ContextObject = InContextObject;
	Entry->ExtensionPointTagMatchType = InExtensionPointTagMatchType;
	Entry->AllowedDataClasses = InAllowedDataClasses;
	Entry->Callback = InCallback;

	NotifyExtensionPointOfExtensions( Entry );
	
	return FUIExtensionPointHandle( this, Entry );
}

FUIExtensionPointHandle UUIExtensionSubsystem::RegisterExtensionPoint(const FGameplayTag& InExtensionPointTag,
	EUIExtensionPointMatch InExtensionPointTagMatchType, const TArray<UClass*>& InAllowedDataClasses,
	FExtendExtensionPointDelegate InCallback)
{
	return RegisterExtensionPointForContext( InExtensionPointTag, nullptr, InExtensionPointTagMatchType, InAllowedDataClasses, InCallback );
}

void UUIExtensionSubsystem::NotifyExtensionPointOfExtensions(TSharedPtr<FUIExtensionPoint>& InExtensionPoint)
{
	for ( FGameplayTag DataTag = InExtensionPoint->ExtensionPointTag; DataTag.IsValid(); DataTag = DataTag.RequestDirectParent() )
	{
		if ( const FExtensionList* ListPtr = ExtensionMap.Find( DataTag ) )
		{
			FExtensionList ExtensionArray( *ListPtr );
			for ( const auto& Extension : ExtensionArray )
			{
				if ( InExtensionPoint->DoesExtensionPassContract( Extension.Get() ) )
				{
					FUIExtensionRequest Request = CreateExtensionRequest( Extension );
					InExtensionPoint->Callback.ExecuteIfBound( EUIExtensionAction::Added, Request );
				}
			}
		}

		if ( InExtensionPoint->ExtensionPointTagMatchType == EUIExtensionPointMatch::ExactMatch )
		{
			break;
		}
	}
}

void UUIExtensionSubsystem::NotifyExtensionPointsOfExtension(EUIExtensionAction InAction, TSharedPtr<FUIExtension>& InExtension)
{
	bool bInInitialTag = true;
	for ( FGameplayTag DataTag = InExtension->ExtensionPointTag; DataTag.IsValid(); DataTag = DataTag.RequestDirectParent() )
	{
		if ( const FExtensionPointList* ListPtr = ExtensionPointMap.Find( DataTag ) )
		{
			FExtensionPointList ExtensionArray( *ListPtr );
			for ( const auto& ExtensionPoint : ExtensionArray )
			{
				if ( bInInitialTag || ExtensionPoint->ExtensionPointTagMatchType == EUIExtensionPointMatch::PartialMatch )
				{
					if ( ExtensionPoint->DoesExtensionPassContract( InExtension.Get() ) )
					{
						FUIExtensionRequest Request = CreateExtensionRequest( InExtension );
						ExtensionPoint->Callback.ExecuteIfBound( InAction, Request );
					}
				}
			}
		}

		bInInitialTag = false;
	}
}

FUIExtensionRequest UUIExtensionSubsystem::CreateExtensionRequest(const TSharedPtr<FUIExtension>& InExtension)
{
	FUIExtensionRequest Request;
	Request.ExtensionHandle = FUIExtensionHandle( this, InExtension );
	Request.ExtensionPointTag = InExtension->ExtensionPointTag;
	Request.Priority = InExtension->Priority;
	Request.Data = InExtension->Data;
	Request.ContextObject = InExtension->ContextObject.Get();
	
	return Request;
}
