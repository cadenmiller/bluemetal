#include "Core/Log.hpp"
#include "Engine/Engine.hpp"
#include "Render/Renderer/PresentRenderPass.hpp"
#include "ImGui/ImGui.hpp"
#include "Noodle/Noodle.hpp"
#include "imgui.h"

// Helper to display a little (?) mark which shows a tooltip when hovered.
// In your own code you may want to display an actual icon if you are using a merged icon fonts (see docs/FONTS.md)
static void HelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort) && ImGui::BeginTooltip())
    {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

blEngine::blEngine(const std::string& appName)
    : _close(false)
{
    (void)appName;

    SDL_SetMainReady();

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        throw std::runtime_error(SDL_GetError());
    }

    std::vector<blDisplay> displays = blDisplay::getDisplays();

    
    _window = std::make_shared<blWindow>(displays[0].getDesktopMode());
    _inputSystem = blInputSystem::getInstance();
    _renderDevice = std::make_shared<blRenderDevice>(_window);
    _swapchain = std::make_shared<blSwapchain>(_renderDevice);

    const std::string defaultConfig = 
        #include "defaultConfig.nwdl"
    ;

    blNoodle config = blNoodle::parseFromFile("Save/Config/config.noodle");
    int& value = config["window"]["width"].get<int>();
    config["window"]["height"] = 1600;
    value = value + 100;

    config.dumpToFile("Save/Config/config.noodle");

    _presentPass = std::make_shared<blPresentRenderPass>(_renderDevice, _swapchain, 
    [&](vk::CommandBuffer cmd)
    {
        blImGui::beginFrame();
    
        ImGui::Begin("Debug Info");
        ImGui::Text("Graphics Device: %s", _renderDevice->getDeviceName().c_str());
        ImGui::Text("Graphics Vendor: %s", _renderDevice->getVendorName().c_str());
        ImGui::Text("F/S: %d", _frameCounter.getFramesPerSecond());
        ImGui::Text("MS/F: %.2f", _frameCounter.getMillisecondsPerFrame());
        ImGui::Text("Average F/S (Over 10 Seconds): %.1f", _frameCounter.getAverageFramesPerSecond(10));
        ImGui::Text("Average MS/F (Over 144 Frames): %.2f", _frameCounter.getAverageMillisecondsPerFrame(144));
        ImGui::End();

        ImGui::Begin("Configuration");
        static int presentModeSelected;

        ImGui::SeparatorText("Interface");

        static float alpha = 1.0f;
        if (ImGui::SliderFloat("UI Alpha Transparency", &alpha, 0.1f, 1.0f))
        {
            ImGui::GetStyle().Alpha = alpha;
        }



        ImGui::SeparatorText("Swapchain Options");
        
        if (ImGui::RadioButton("FIFO", &presentModeSelected, 0))
        {
            _postRenderCommands.push([&](){
                _swapchain->recreate(vk::PresentModeKHR::eFifo);
                _renderer->resize(_swapchain->getExtent());
            });
        }
        ImGui::SameLine();
        HelpMarker("Normal VSync, use Mailbox if your system supports it.");
        
        ImGui::BeginDisabled(!_swapchain->isMailboxSupported());
        if (ImGui::RadioButton("Mailbox", &presentModeSelected, 1))
        {      
            _postRenderCommands.push([&](){
                _swapchain->recreate(vk::PresentModeKHR::eMailbox);
                _renderer->resize(_swapchain->getExtent());
            }); 
        }
        ImGui::EndDisabled();

        ImGui::SameLine();
        HelpMarker( (!_swapchain->isMailboxSupported()) ? "Mailbox is not supported on this system." : "Performs fast like Immediate but with no tearing, like FIFO.");

        ImGui::BeginDisabled(!_swapchain->isImmediateSupported());        
        if (ImGui::RadioButton("Immediate", &presentModeSelected, 2))
        {            
            _postRenderCommands.push([&](){
                _swapchain->recreate(vk::PresentModeKHR::eImmediate);
                _renderer->resize(_swapchain->getExtent());
            }); 
            
        }
        ImGui::EndDisabled();
        
        ImGui::SameLine();
        HelpMarker( (!_swapchain->isImmediateSupported()) ?  "Immediate is not supported on this system." : "Renders as fast as possible to screen, may cause screen tearing.");

        ImGui::End();

        ImGui::ShowDemoWindow();
        
        blImGui::endFrame(cmd);
    });

    std::vector<std::shared_ptr<blRenderPass>> passes
    {
        _presentPass
    };

    _renderer = std::make_shared<blRenderer>(_renderDevice, _swapchain, passes);

    blImGui::init(_window, _renderDevice, _presentPass);
}

blEngine::~blEngine()
{
    blImGui::shutdown();
}

bool blEngine::run()
{
    
    while (!_inputSystem->shouldClose())
    {
        _frameCounter.beginFrame();
        _inputSystem->poll();
        
        _renderer->render();
        _frameCounter.endFrame();

        for (size_t i = 0; i < _postRenderCommands.size(); i++)
        {
            _postRenderCommands.front()();
            _postRenderCommands.pop();
        }
    }

    return true;
}