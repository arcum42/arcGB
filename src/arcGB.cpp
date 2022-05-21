#include "arcGB.h"
#include "cartridge.h"
#include "cpu.h"

// Our state
bool show_registers = false;
bool show_memory = false;
bool show_cart_info = false;

ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

void render(SDL_Renderer *renderer, bool &done)
{
    // Start the Dear ImGui frame
    ImGui_ImplSDLRenderer_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Main", nullptr, ImGuiWindowFlags_MenuBar); // Create the main window.
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Load", nullptr, false, true))
            {
                ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".gb,.gbc,.bin", ".");
            }
            if (ImGui::MenuItem("Quit", nullptr, false, true))
            {
                done = true;
            }
            ImGui::EndMenu();
        }
    }
    ImGui::EndMenuBar();

    ImGui::Checkbox("View Registers", &show_registers);
    ImGui::Checkbox("View Memory", &show_memory);
    ImGui::Checkbox("View Cartridge Information", &show_cart_info);
    ImGui::End();

    if (show_registers)
    {
        ImGui::Begin("Registers", &show_registers);
        ImGui::Text("Registers");
        ImGui::Text("AF: 0x%x    BC: 0x%x", registers::af, registers::bc);
        ImGui::Text("DE: 0x%x    HL: 0x%x", registers::de, registers::hl);
        ImGui::Text("SP: 0x%x    PC: 0x%x", registers::sp, registers::pc);
        ImGui::End();
    }

    if (show_memory)
    {
        //ImGui::Begin("Memory", &show_memory);
        //ImGui::Text("Memory");
        //ImGui::End();
        static MemoryEditor mem_edit;
        mem_edit.DrawWindow("Memory Editor", cartridge.data(), cartridge.size());
    }

    if (show_cart_info)
    {
        ImGui::Begin("Cartridge Information", &show_cart_info);
        ImGui::Text("Cartridge Information:");
        ImGui::Text("Name: '%s'", cart_name.c_str());
        ImGui::Text("Size: 0x%lx", cartridge.size());
        ImGui::End();
    }

    if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
    {
        // action if OK
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
            std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();

            cartridge = load_cartridge(filePathName);
            cart_name.clear();

            for (int i = 0x134; i < 0x143; i++)
                cart_name += cartridge[i];
        }
        // close
        ImGuiFileDialog::Instance()->Close();
    }

    // Rendering
    ImGui::Render();
    SDL_SetRenderDrawColor(renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
    SDL_RenderClear(renderer);
    ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
    SDL_RenderPresent(renderer);
}

int main(int /*argc*/, char ** /*argv*/)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // Setup window
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window *window = SDL_CreateWindow("ArcGB", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);

    // Setup SDL_Renderer instance
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        SDL_Log("Error creating SDL_Renderer!");
        return false;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer_Init(renderer);

    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }
        render(renderer, done);
    }

    // Cleanup
    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
