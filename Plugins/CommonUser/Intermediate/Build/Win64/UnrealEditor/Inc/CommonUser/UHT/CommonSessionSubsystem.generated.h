// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "CommonSessionSubsystem.h"

#ifdef COMMONUSER_CommonSessionSubsystem_generated_h
#error "CommonSessionSubsystem.generated.h already included, missing '#pragma once' in CommonSessionSubsystem.h"
#endif
#define COMMONUSER_CommonSessionSubsystem_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

class APlayerController;
class UCommonSession_HostSessionRequest;

// ********** Begin Class UCommonSession_HostSessionRequest ****************************************
COMMONUSER_API UClass* Z_Construct_UClass_UCommonSession_HostSessionRequest_NoRegister();

#define FID_UE_Study_MGP_Plugins_CommonUser_Source_CommonUser_Public_CommonSessionSubsystem_h_12_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUCommonSession_HostSessionRequest(); \
	friend struct Z_Construct_UClass_UCommonSession_HostSessionRequest_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend COMMONUSER_API UClass* Z_Construct_UClass_UCommonSession_HostSessionRequest_NoRegister(); \
public: \
	DECLARE_CLASS2(UCommonSession_HostSessionRequest, UObject, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/CommonUser"), Z_Construct_UClass_UCommonSession_HostSessionRequest_NoRegister) \
	DECLARE_SERIALIZER(UCommonSession_HostSessionRequest)


#define FID_UE_Study_MGP_Plugins_CommonUser_Source_CommonUser_Public_CommonSessionSubsystem_h_12_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UCommonSession_HostSessionRequest(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	/** Deleted move- and copy-constructors, should never be used */ \
	UCommonSession_HostSessionRequest(UCommonSession_HostSessionRequest&&) = delete; \
	UCommonSession_HostSessionRequest(const UCommonSession_HostSessionRequest&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UCommonSession_HostSessionRequest); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UCommonSession_HostSessionRequest); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UCommonSession_HostSessionRequest) \
	NO_API virtual ~UCommonSession_HostSessionRequest();


#define FID_UE_Study_MGP_Plugins_CommonUser_Source_CommonUser_Public_CommonSessionSubsystem_h_9_PROLOG
#define FID_UE_Study_MGP_Plugins_CommonUser_Source_CommonUser_Public_CommonSessionSubsystem_h_12_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_UE_Study_MGP_Plugins_CommonUser_Source_CommonUser_Public_CommonSessionSubsystem_h_12_INCLASS_NO_PURE_DECLS \
	FID_UE_Study_MGP_Plugins_CommonUser_Source_CommonUser_Public_CommonSessionSubsystem_h_12_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class UCommonSession_HostSessionRequest;

// ********** End Class UCommonSession_HostSessionRequest ******************************************

// ********** Begin Class UCommonSessionSubsystem **************************************************
#define FID_UE_Study_MGP_Plugins_CommonUser_Source_CommonUser_Public_CommonSessionSubsystem_h_31_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execHostSession);


COMMONUSER_API UClass* Z_Construct_UClass_UCommonSessionSubsystem_NoRegister();

#define FID_UE_Study_MGP_Plugins_CommonUser_Source_CommonUser_Public_CommonSessionSubsystem_h_31_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUCommonSessionSubsystem(); \
	friend struct Z_Construct_UClass_UCommonSessionSubsystem_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend COMMONUSER_API UClass* Z_Construct_UClass_UCommonSessionSubsystem_NoRegister(); \
public: \
	DECLARE_CLASS2(UCommonSessionSubsystem, UGameInstanceSubsystem, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/CommonUser"), Z_Construct_UClass_UCommonSessionSubsystem_NoRegister) \
	DECLARE_SERIALIZER(UCommonSessionSubsystem)


#define FID_UE_Study_MGP_Plugins_CommonUser_Source_CommonUser_Public_CommonSessionSubsystem_h_31_ENHANCED_CONSTRUCTORS \
	/** Deleted move- and copy-constructors, should never be used */ \
	UCommonSessionSubsystem(UCommonSessionSubsystem&&) = delete; \
	UCommonSessionSubsystem(const UCommonSessionSubsystem&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UCommonSessionSubsystem); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UCommonSessionSubsystem); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(UCommonSessionSubsystem) \
	NO_API virtual ~UCommonSessionSubsystem();


#define FID_UE_Study_MGP_Plugins_CommonUser_Source_CommonUser_Public_CommonSessionSubsystem_h_28_PROLOG
#define FID_UE_Study_MGP_Plugins_CommonUser_Source_CommonUser_Public_CommonSessionSubsystem_h_31_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_UE_Study_MGP_Plugins_CommonUser_Source_CommonUser_Public_CommonSessionSubsystem_h_31_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_UE_Study_MGP_Plugins_CommonUser_Source_CommonUser_Public_CommonSessionSubsystem_h_31_INCLASS_NO_PURE_DECLS \
	FID_UE_Study_MGP_Plugins_CommonUser_Source_CommonUser_Public_CommonSessionSubsystem_h_31_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class UCommonSessionSubsystem;

// ********** End Class UCommonSessionSubsystem ****************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_UE_Study_MGP_Plugins_CommonUser_Source_CommonUser_Public_CommonSessionSubsystem_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
