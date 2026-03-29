#ifndef CONFIG_H
#define CONFIG_H
#include <optional>
#include <string>

struct Config
{
    std::string cert_path;
    std::string key_path;
    std::string cert_node_path;
    std::string key_node_path;
    int primary_server_port{};
    int client_thread_max{};
    std::optional<int> standby_server_port;
    std::optional<int> node_thread_max;
};

struct CLIArgs
{
    std::string config_path;
};

#endif //CONFIG_H
