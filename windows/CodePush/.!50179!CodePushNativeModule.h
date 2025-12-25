// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once

#include "NativeModules.h"
#include "winrt/Windows.Data.Json.h"
#include "winrt/Windows.Storage.h"

#include "CodePushConfig.h"

// Helper functions for reading and sending JsonValues to and from JavaScript
namespace winrt::Microsoft::ReactNative
{
	void ReadValue(IJSValueReader const& reader, /*out*/ Windows::Data::Json::JsonObject& value) noexcept;
	void ReadValue(IJSValueReader const& reader, /*out*/ Windows::Data::Json::IJsonValue& value) noexcept;
}

namespace Microsoft::CodePush::ReactNative
{
	REACT_MODULE(CodePushNativeModule, L"CodePush");
	struct CodePushNativeModule
	{
		enum class CodePushInstallMode
		{
			Immediate = 0,
			OnNextRestart = 1,
			OnNextResume = 2,
			OnNextSuspend = 3
		};

		enum class CodePushUpdateState
		{
			Running = 0,
			Pending = 1,
			Latest = 2
		};

		static winrt::Windows::Foundation::IAsyncAction LoadBundle();
		static void SetHost(const winrt::Microsoft::ReactNative::ReactNativeHost& host);

		static winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Storage::StorageFile> GetBinaryBundleAsync();
		static winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Storage::StorageFile> GetBundleFileAsync();

		static winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Storage::StorageFolder> GetBundleAssetsFolderAsync();
		static winrt::Windows::Storage::StorageFolder GetLocalStorageFolder();
		static winrt::Windows::Storage::ApplicationDataContainer GetLocalSettings();

		void OverrideAppVersion(std::wstring_view appVersion);
		void SetDeploymentKey(std::wstring_view deploymentKey);

		bool IsFailedHash(std::wstring_view packageHash);

		winrt::Windows::Data::Json::JsonObject GetRollbackInfo();
		int GetRollbackCountForPackage(
			std::wstring_view packageHash,
			const winrt::Windows::Data::Json::JsonObject& latestRollbackInfo);

		static bool IsPendingUpdate(std::wstring_view packageHash);

		winrt::Windows::Foundation::IAsyncAction ClearDebugUpdates();

		REACT_INIT(Initialize);
		void Initialize(winrt::Microsoft::ReactNative::ReactContext const& reactContext) noexcept;

		REACT_CONSTANT_PROVIDER(GetConstants);
		void GetConstants(winrt::Microsoft::ReactNative::ReactConstantProvider& constants) noexcept;

		/*
		 * This is native-side of the JavaScript RemotePackage.download method
		 */
		REACT_METHOD(DownloadUpdateAsync, L"downloadUpdate");
		winrt::fire_and_forget DownloadUpdateAsync(
			winrt::Windows::Data::Json::JsonObject updatePackage,
			bool notifyProgress,
			winrt::Microsoft::ReactNative::ReactPromise<winrt::Windows::Data::Json::IJsonValue> promise) noexcept;

		/*
		 * This is the native side of the CodePush.getConfiguration method. It isn't
		 * currently exposed via the "react-native-code-push" module, and is used
		 * internally only by the CodePush.checkForUpdate method in order to get the
		 * app version, as well as the deployment key that was configured in the Info.plist file.
		 */
		REACT_METHOD(GetConfiguration, L"getConfiguration");
		winrt::fire_and_forget GetConfiguration(winrt::Microsoft::ReactNative::ReactPromise<winrt::Windows::Data::Json::IJsonValue> promise) noexcept;

		/*
		 * This method is the native side of the CodePush.getUpdateMetadata method.
		 */
		REACT_METHOD(GetUpdateMetadataAsync, L"getUpdateMetadata");
		winrt::fire_and_forget GetUpdateMetadataAsync(
			CodePushUpdateState updateState,
			winrt::Microsoft::ReactNative::ReactPromise<winrt::Windows::Data::Json::IJsonValue> promise) noexcept;

		/*
		 * This method is the native side of the LocalPackage.install method.
		 */
		REACT_METHOD(InstallUpdateAsync, L"installUpdate");
		winrt::fire_and_forget InstallUpdateAsync(
			winrt::Windows::Data::Json::JsonObject updatePackage,
			CodePushInstallMode installMode,
			int minimumBackgroundDuration,
			winrt::Microsoft::ReactNative::ReactPromise<void> promise) noexcept;

		/*
		 * This method isn't publicly exposed via the "react-native-code-push"
		 * module, and is only used internally to populate the RemotePackage.failedInstall property.
		 */
		REACT_METHOD(IsFailedUpdate, L"isFailedUpdate");
		void IsFailedUpdate(
			std::wstring packageHash,
			winrt::Microsoft::ReactNative::ReactPromise<bool> promise) noexcept;

		REACT_METHOD(SetLatestRollbackInfo, L"setLatestRollbackInfo");
		void SetLatestRollbackInfo(std::wstring packageHash) noexcept;

		REACT_METHOD(GetLatestRollbackInfo, L"getLatestRollbackInfo");
		void GetLatestRollbackInfo(winrt::Microsoft::ReactNative::ReactPromise<winrt::Windows::Data::Json::IJsonValue> promise) noexcept;

		/*
		 * This method isn't publicly exposed via the "react-native-code-push"
		 * module, and is only used internally to populate the LocalPackage.isFirstRun property.
		 */
		REACT_METHOD(IsFirstRun, L"isFirstRun");
		winrt::fire_and_forget IsFirstRun(
			std::wstring packageHash,
			winrt::Microsoft::ReactNative::ReactPromise<bool> promise) noexcept;

		/*
		 * This method is the native side of the CodePush.notifyApplicationReady() method.
		 */
		REACT_METHOD(NotifyApplicationReady, L"notifyApplicationReady");
		void NotifyApplicationReady(winrt::Microsoft::ReactNative::ReactPromise<winrt::Windows::Data::Json::IJsonValue> promise) noexcept;

		REACT_METHOD(Allow, L"allow");
		void Allow(winrt::Microsoft::ReactNative::ReactPromise<winrt::Microsoft::ReactNative::JSValue> promise) noexcept;

		REACT_METHOD(ClearPendingRestart, L"clearPendingRestart");
		void ClearPendingRestart() noexcept;

		REACT_METHOD(Disallow, L"disallow");
		void Disallow(winrt::Microsoft::ReactNative::ReactPromise<winrt::Microsoft::ReactNative::JSValue> promise) noexcept;

		/*
		 * This method is the native side of the CodePush.restartApp() method.
		 */
		REACT_METHOD(RestartApp, L"restartApp");
		winrt::fire_and_forget RestartApp(
			bool onlyIfUpdateIsPending,
			winrt::Microsoft::ReactNative::ReactPromise<winrt::Microsoft::ReactNative::JSValue> promise) noexcept;

		/*
		 * This method clears CodePush's downloaded updates.
		 * It is needed to switch to a different deployment if the current deployment is more recent.
