#ifndef CONFIG_H
#define CONFIG_H
#include <optional>

struct ServerConfig
{
    int primary_server_port{};
    int primary_node{};
    int client_thread_max{};
    std::optional<int> standby_server_port;
    std::optional<int> NODE2_PORT;
    std::optional<int> NODE_THREAD_MAX;
};

#endif //CONFIG_H
