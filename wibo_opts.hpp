#ifndef WIBO_OPTS_HPP
#define WIBO_OPTS_HPP

#include <argp.h>
#include <memory>
#include <string>

/**
 * @class WiboOpts
 * @brief Class to handle command-line options for the Wibo application
 */
class WiboOpts {
  public:
	/**
	 * @brief Debug mode flag
	 */
	bool debug = false;

	/**
	 * @brief Portable executable path
	 */
	std::string pe_path;

	/**
	 * @brief Portable executable name
	 */
	std::string pe_name;

	/**
	 * @brief Get a WiboOpts instance
	 * @return WiboOpts* A pointer to the WiboOpts instance
	 */
	static WiboOpts *get_instance();

	/**
	 * @brief Function to parse command line options for Wibo
	 * @param key A number associated with the option
	 * @param arg Pointer to a char array containing the option
	 * @param state The current state of the parser
	 * @return error_t 0 if successful, otherwise an error code
	 */
	error_t parse_option(int key, char *arg, struct argp_state *state);

	/**
	 * @brief Function to parse command line arguments received
	 * @param argc The number of args/opts to parse
	 * @param argv The list of char arrays containing the args/opts
	 * @return error_t 0 if successful, otherwise an error code
	 */
	error_t parse_arguments(int argc, char **argv);
	~WiboOpts();

  private:
	/// @brief Singleton instance of WiboOpts
	static std::unique_ptr<WiboOpts> instance;
	const char *doc;
	const char *args_doc;
	struct argp_option *options;
	struct argp argp;
	WiboOpts();
};

#endif