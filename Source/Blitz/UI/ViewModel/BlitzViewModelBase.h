// Copyright sumikiru. All Rights Reserved,

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "BlitzViewModelBase.generated.h"

/*
 * https://youtrack.jetbrains.com/issue/RIDER-120260/Support-UMG-Viewmodel
 * Adjusted UE_MVVM macros that don't produce false-positive 'Cannot resolve symbol' errors in IDE during development
 * Use FF_MVVM_SET_PROPERTY_VALUE and FF_MVVM_BROADCAST_FIELD_VALUE_CHANGED instead of UE_MVVM_SET_PROPERTY_VALUE and UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED
 * Uses the FFieldNotificationClassDescriptor to get the field id via GetField() function call,
 * rather than reading directly off ThisClass::FFieldNotificationClassDescriptor::MemberName
 * This allows the IDE to resolve the symbol and provide code completion
 * Reverts to using original UE_MVVM macros in shipping builds.
 */
#if UE_BUILD_SHIPPING
#define FF_MVVM_SET_PROPERTY_VALUE(MemberName, NewValue) \
UE_MVVM_SET_PROPERTY_VALUE(MemberName, NewValue)

#define FF_MVVM_BROADCAST_FIELD_VALUE_CHANGED(MemberName) \
UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(MemberName)
#else
#define FF_MVVM_SET_PROPERTY_VALUE(MemberName, NewValue) \
SetPropertyValue(MemberName, NewValue, GetFieldNotificationDescriptor().GetField(StaticClass(), FName(TEXT(#MemberName))))
#define FF_MVVM_BROADCAST_FIELD_VALUE_CHANGED(MemberName) \
BroadcastFieldValueChanged(GetFieldNotificationDescriptor().GetField(StaticClass(), FName(TEXT(#MemberName))))
#endif

#define FF_MVVM_BROADCAST_FIELD_VALUE_CHANGED_ALL() \
GetFieldNotificationDescriptor().ForEachField( \
StaticClass(), \
[this](const auto FieldId) \
{ \
this->BroadcastFieldValueChanged(FieldId); \
return true;

/**
 * Blueprintable必须写，默认的VM不支持蓝图，这样蓝图ViewModel类才能继承
 * 需要继承UMVVMViewModelBase
 */
UCLASS(Blueprintable)
class BLITZ_API UBlitzViewModelBase : public UMVVMViewModelBase
{
	GENERATED_BODY()
};
