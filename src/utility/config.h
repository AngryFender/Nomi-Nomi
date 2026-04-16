#ifndef CONFIG_H
#define CONFIG_H
#include <optional>
#include <string>

struct ClientConfig
{
    std::string self_signed_crt_path;
    std::string server_address;
    int server_port{};
    std::optional<std::string> standby_address;
    std::optional<int> standby_port{};
};


struct Config
{
    std::string cert_path;
    std::string key_path;
    int server_type;
    int server_port{};
    int client_thread_max{};
    int node_port{};
    int node_thread_max{};
};

struct CLIArgs
{
    std::string config_path;
};

#endif //CONFIG_H
