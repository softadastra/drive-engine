#include <vix.hpp>
#include "handlers.hpp"
using namespace vix;

int main()
{
    App app;

    register_routes(app);

    app.run(8080);
}
