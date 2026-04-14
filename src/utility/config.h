#ifndef CONFIG_H
#define CONFIG_H
#include <optional>
#include <string>

struct ClientConfig
{
    std::string ssl_path;
    std::string primary_address;
    int primary_port{};
    std::string secondary_address;
    int secondary_port{};
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
