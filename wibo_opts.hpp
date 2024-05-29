#ifndef WIBO_OPTS_HPP
#define WIBO_OPTS_HPP

#include <argp.h>
#include <memory>
#include <string>

class WiboOpts {
public:
    // @brief A flag to enable debug mode
    bool debug = false;
    // @brief The path of the portable executable
    std::string pe_path;
    // @brief portable executable name
    std::string pe_name;

    /**
     * @brief Get a WiboOpts instance
     * 
     * @return WiboOpts* a pointer to the WiboOpts instance
     */
    static WiboOpts *get_instance();

    /**
     * @brief Function to parse command line options for Wibo
     * 
     * @param key a number associated with the option
     * @param arg pointer to a char array containing the option 
     * @param state the current state of the parser
     * @return error_t 0 if successful, otherwise an error code
     */
    error_t parse_option(int key, char *arg, struct argp_state *state);

    /**
     * @brief Function to parse command line arguments recieved
     * 
     * @param argc the number of args/opts to parse
     * @param argv the list of char arrays containing the args/opts
     * @return error_t 0 if successful, otherwise an error code
     */
    error_t parse_arguments(int argc, char **argv);
    ~WiboOpts();

private:
    // @brief Singleton instance of WiboOpts
    static std::unique_ptr<WiboOpts> instance;
    const char *doc;
    const char *args_doc;
    struct argp_option *options;
    struct argp argp;
    WiboOpts();
};

#endif