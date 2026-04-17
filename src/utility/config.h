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
    std::string server_cert_path;
    std::string server_key_path;
    int type{1};
    std::string server_address{};
    int server_port{};
    int server_threads{};
    std::optional<std::string> standby_cert_path;
    std::optional<std::string> standby_address;
    std::optional<int> standby_port{};
    std::optional<int> standby_threads{};
    bool is_standby_configured{false};
};

struct CLIArgs
{
    std::string config_path;
};

#endif //CONFIG_H
