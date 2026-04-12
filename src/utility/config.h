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
    std::string primary_cert_path;
    std::string primary_key_path;
    std::string standby_cert_path;
    std::string standby_key_path;
    int primary_server_port{};
    int client_thread_max{};
    std::optional<int> standby_server_port;
    std::optional<int> standby_thread_max;
};

struct CLIArgs
{
    std::string config_path;
};

#endif //CONFIG_H
