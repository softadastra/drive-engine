#include "handlers.hpp"

void register_routes(vix::App &app)
{
    app.get("/health", [](auto &, auto &res)
            { 
                nlohmann::json response = "ok";
                res.status(200).json(response); });

    app.get("/", [](auto &, auto &res)
            { 
                nlohmann::json response =  "Welcome to Softadastra OS (vix)";
                res.status(200).json(response); });

    app.get("/api/info", [](auto &, auto &res)
            { res.json({"name", "softadastra-os",
                        "version", "0.1.0"}); });

    std::cout << "[routes] Registered / , /health and /api/info\n";
}