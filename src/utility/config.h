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

struct General
{
    int type{1};
    std::string alias{};
    int init_timeout;
    int init_repeat_period;
};

struct MachineInfo
{
    std::string cert_path;
    std::string key_path;
    std::string address;
    int port{0};
    int threads_max{0};
};

struct Config
{
    General general;
    MachineInfo server;
    std::optional<MachineInfo> standby;
};

struct CLIArgs
{
    std::string config_path;
};

#endif //CONFIG_H
