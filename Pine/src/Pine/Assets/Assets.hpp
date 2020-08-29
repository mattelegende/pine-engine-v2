#pragma once

#include "IAsset/IAsset.hpp"
#include <memory>

namespace Pine::Assets
{
	// Used for assets preview
	struct DirectoryCache_t {
		std::string path = "";
		bool isDirectory = false;
		bool readOnly = false;
		IAsset* assetPointer = nullptr;
		std::vector<std::unique_ptr<DirectoryCache_t>> items;
	};

	DirectoryCache_t* GetAssetsDirectoryCache();
	void RefreshDirectoryCache();

	// Loads a asset file from the disk
	// Uses the file extension to determine asset type
	// Returns nullptr on failure.
	IAsset* LoadFromFile(const std::string& filePath);

	// Loads all asset in a directory recursively, returns amount of assets loaded.
	int LoadFromDirectory(const std::string& directoryPath);

	// Returns a loaded asset by file path, your job to cast to the right type.
	IAsset* GetAsset(const std::string& assetPath);

	template <class T>
	T* GetAsset(const std::string& assetPath) {
		auto asset = GetAsset(assetPath);
		return dynamic_cast<T*>(asset);
	}

	// Disposes all loaded assets
	void Dispose();
}