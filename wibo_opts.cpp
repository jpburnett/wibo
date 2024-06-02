#include "wibo_opts.hpp"
#include <iostream>
#include <stdio.h>
#include <string.h>

struct argp_option g_options[] = {
	{ .name = "binary-path",
	  .key = 'b',
	  .arg = "FILENAME",
	  .flags = 0,
	  .doc = "Application/binary path",
	  .group = 0 },
	{ .name = "debug",
	  .key = 'd',
	  .arg = NULL,
	  .flags = 0,
	  .doc = "Enable debug mode",
	  .group = 0 },
	{ NULL, 0, NULL, 0, NULL, 0 }
};

/**
 * @brief This function parses command-line arguments for the program.
 * 
 * @param key The key associated with the option.
 * @param arg Pointer to the argument string.
 * @param state The state of the argument parser.
 * @return error_t Returns 0 if successful, otherwise returns an error code.
 */
static error_t opt_parse(int key, char *arg, struct argp_state *state)
{
    WiboOpts *wopts = static_cast<WiboOpts *>(state->input);
    return wopts->parse_option(key, arg, state);
}

std::unique_ptr<WiboOpts> WiboOpts::instance = nullptr;

/**
 * @brief Helper function to convert a path to a Windows path
 * @param path the path to convert
 */
std::string convertPath(const std::string& path) {
    std::string convertedPath;

    for (char c : path) {
        if (c == '/') {
            convertedPath += '\\';  // Replace '/' with '\\'
        } else {
            convertedPath += c;
        }
    }

    return convertedPath;
}

error_t WiboOpts::parse_option(int key, char *arg, struct argp_state *state)
{
	// Convert arg to a string
	std::string sarg = (arg == NULL) ? "" : arg;
	char resolved_path[PATH_MAX] = { '\0' };
	std::string err_msg;

	switch (key) {
	case 'b': {
		// Get the binary name
		pe_name = sarg;
		std::cout << "Windows binary: " << pe_name << std::endl;

		if (arg == NULL) {
			argp_error(state, "No argument provided for binary path.");
			return ARGP_ERR_UNKNOWN;
		}

		// Get the binary path
		char *path = realpath(arg, resolved_path);
		if (path == NULL) {
			err_msg = "Failed to resolve path: ";
			err_msg += arg;
			argp_error(state, "%s", err_msg.c_str());
		    return ARGP_ERR_UNKNOWN;
		}

		std::string convertedPath = convertPath(path);
		convertedPath.insert(0, "Z:");
		pe_path = convertedPath;
		std::cout << "Binary path: " << pe_path << std::endl;

		break;
	}

	case 'd': {
		std::cout << "Enabling Debug" << std::endl;
		debug = true;
		break;
	}

	case ARGP_KEY_ARG:
		if (state->arg_num != 0) {
			// Too many arguments
			argp_usage(state);
		}
		break;

	case ARGP_KEY_END:
		if (state->arg_num != 0) {
			// Incorrect number of arguments
			argp_usage(state);
		}
		break;

	default:
		return ARGP_ERR_UNKNOWN;
	}

	return 0;
}

WiboOpts *WiboOpts::get_instance() 
{
    if (instance == nullptr) {
        WiboOpts *w = new WiboOpts();
        instance.reset(w);
    }
    return instance.get();
}

WiboOpts::WiboOpts()
	: debug(false), pe_path(""), pe_name("")
{
	// Set up argp
    doc = "Wibo - Windows is basically obsolete.\n";
    args_doc = "";
    options = g_options;
    argp.options = options;
    argp.parser = opt_parse;
    argp.args_doc = args_doc;
    argp.doc = doc;
    argp.children = NULL;
    argp.help_filter = NULL;
    argp.argp_domain = NULL;
}

error_t WiboOpts::parse_arguments(int argc, char **argv) 
{
    return argp_parse(&argp, argc, argv, 0, 0, this);
}

WiboOpts::~WiboOpts() = default;