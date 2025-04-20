#include "pch.h"

#include <VGE_engine.hpp>
#include "VGE_app.hpp"

int main()
{
    std::shared_ptr<VGE::VgeEngine> engine = std::make_shared<VGE::VgeEngine>(VGE::VgeApp::WIDTH, VGE::VgeApp::HEIGHT, "Vulkan Game Engine");
    VGE::VgeApp app{*engine};

    try
    {
        app.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
