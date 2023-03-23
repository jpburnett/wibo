#ifndef FILE_HANDLER_HPP
#define FILE_HANDLER_HPP

#include <filesystem>
#include <string>

namespace wibo {
	namespace fs = std::filesystem;

    static constexpr int DATA_SIZE = 0x10000;
    static Data datas[DATA_SIZE];

    // @brief Type specifier for data
	enum Type {
        TYPE_UNUSED,
		TYPE_FILE,
		TYPE_MAPPED,
	};

	// @brief Handle for the specified standard
	enum HandleParam {
		STD_INPUT_HANDLE = -10,
		STD_OUTPUT_HANDLE = -11,
		STD_ERROR_HANDLE = -12,
	};

    struct Data {
        Type type = TYPE_UNUSED;
        void *ptr;
        size_t size;
    };

	class FileHandler{
	public:
		FileHandler();
		~FileHandler();

		void *getStdHandle(uint32_t nStdHandle);
		FILE *fpFromHandle(void *handle, bool pop = false);
		void *allocFpHandle(FILE *fp);
		fs::path pathFromWindows(const char *inStr);
		std::string pathToWindows(const fs::path &path);
		void init();
		unsigned int setStdHandle(uint32_t nStdHandle, void *hHandle);
        Data dataFromHandle(void *handle, bool pop);
        void *allocDataHandle(Data data);

	protected:
		static void *stdinHandle;
		static void *stdoutHandle;
		static void *stderrHandle;

	};

	static bool endsWith(const std::string &str, const std::string &suffix) {
		return str.size() >= suffix.size() && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
	}

}


#endif
