#include "common.h"
#include "file_handler.hpp"
#include <algorithm>
#include <map>

namespace wibo {

	FileHandler::FileHandler() {
		init();
	};
	
	FileHandler::~FileHandler() {};

	fs::path FileHandler::pathFromWindows(const char *inStr) {
		// Convert to forward slashes
		std::string str = inStr;
		std::replace(str.begin(), str.end(), '\\', '/');

		// Remove the drive letter
		if (str.rfind("z:/", 0) == 0 || str.rfind("Z:/", 0) == 0) {
			str.erase(0, 2);
		}

		// Return as-is if it exists, else traverse the filesystem looking for
		// a path that matches case insensitively
		fs::path path = fs::path(str).lexically_normal();
		if (fs::exists(path)) {
			return path;
		}

		path = path.lexically_normal();
		fs::path newPath = ".";
		bool followingExisting = true;
		for (const auto& component : path) {
			fs::path newPath2 = newPath / component;
			if (followingExisting && !fs::exists(newPath2) && (component != ".." && component != "." && component != "")) {
				followingExisting = false;
				try {
					for (fs::path entry : fs::directory_iterator{newPath}) {
						if (strcasecmp(entry.filename().c_str(), component.c_str()) == 0) {
							followingExisting = true;
							newPath2 = entry;
							break;
						}
					}
				} catch (const fs::filesystem_error&) {
					// not a directory
				}
			}
			newPath = newPath2;
		}
		if (followingExisting) {
			DEBUG_LOG("Resolved case-insensitive path: %s\n", newPath.c_str());
		} else {
			DEBUG_LOG("Failed to resolve path: %s\n", newPath.c_str());
		}

		return newPath;
	}

	std::string FileHandler::pathToWindows(const fs::path &path) {
		std::string str = path.lexically_normal();

		if (path.is_absolute()) {
			str.insert(0, "Z:");
		}

		std::replace(str.begin(), str.end(), '/', '\\');
		return str;
	}

	FILE *FileHandler::fpFromHandle(void *handle, bool pop) {
		Data data = dataFromHandle(handle, pop);
		if (data.type == TYPE_FILE) {
			return (FILE*)data.ptr;
		} else if (data.type == TYPE_UNUSED && pop) {
			return 0;
		} else {
			printf("Invalid file handle %p\n", handle);
			assert(0);
		}
	}

	void* FileHandler::allocFpHandle(FILE *fp) {
		return allocDataHandle(Data{TYPE_FILE, fp, 0});
	}

	void* FileHandler::getStdHandle(uint32_t nStdHandle) {
		switch (nStdHandle) {
			case ((uint32_t) STD_INPUT_HANDLE): // STD_INPUT_HANDLE
				return stdinHandle;
			case ((uint32_t) STD_OUTPUT_HANDLE): // STD_OUTPUT_HANDLE
				return stdoutHandle;
			case ((uint32_t) STD_ERROR_HANDLE): // STD_ERROR_HANDLE
				return stderrHandle;
			default:
				return (void *) 0xFFFFFFFF;
		}
	}

	unsigned int FileHandler::setStdHandle(uint32_t nStdHandle, void *hHandle) {
		switch (nStdHandle) {
			case ((uint32_t) STD_INPUT_HANDLE): // STD_INPUT_HANDLE
				stdinHandle = hHandle;
				break;
			case ((uint32_t) STD_OUTPUT_HANDLE): // STD_OUTPUT_HANDLE
				stdoutHandle = hHandle;
				break;
			case ((uint32_t) STD_ERROR_HANDLE): // STD_ERROR_HANDLE
				stderrHandle = hHandle;
				break;
			default:
				return 0; // fail
		}
		return 1; // success
	}

	void FileHandler::init() {
		stdinHandle = allocFpHandle(stdin);
		stdoutHandle = allocFpHandle(stdout);
		stderrHandle = allocFpHandle(stderr);
	}

    Data dataFromHandle(void *handle, bool pop) {
		uintptr_t index = (uintptr_t)handle;
		if (index > 0 && index < DATA_SIZE) {
			Data ret = datas[index];
			if (pop)
				datas[index] = Data{};
			return ret;
		}
		if (pop) {
			return Data{};
        }
		printf("Invalid file handle %p\n", handle);
		assert(0);
	}

	void *allocDataHandle(Data data) {
		for (int i = 1; i < DATA_SIZE; i++) {
			if (datas[i].type == TYPE_UNUSED) {
				datas[i] = data;
				return (void*)i;
			}
		}
		printf("Out of handles\n");
		assert(0);
	}
}
