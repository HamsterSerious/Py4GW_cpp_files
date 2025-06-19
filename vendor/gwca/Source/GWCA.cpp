#include "stdafx.h"

#include <GWCA/GWCA.h>

#include <GWCA/Utilities/Debug.h>
#include <GWCA/Utilities/Hooker.h>
#include <GWCA/Utilities/Macros.h>
#include <GWCA/Utilities/Scanner.h>
#include <GWCA/Utilities/MemoryPatcher.h>

#include <GWCA/Context/PreGameContext.h>
#include <GWCA/Context/GameContext.h>
#include <GWCA/Context/WorldContext.h>
#include <GWCA/Context/PartyContext.h>
#include <GWCA/Context/CharContext.h>
#include <GWCA/Context/GuildContext.h>
#include <GWCA/Context/ItemContext.h>
#include <GWCA/Context/AgentContext.h>
#include <GWCA/Context/AccountContext.h>
#include <GWCA/Context/GameplayContext.h>

#include <GWCA/Managers/Module.h>

#include <GWCA/Managers/UIMgr.h>
#include <GWCA/Managers/MapMgr.h>
#include <GWCA/Managers/ChatMgr.h>
#include <GWCA/Managers/ItemMgr.h>
#include <GWCA/Managers/StoCMgr.h>
#include <GWCA/Managers/AgentMgr.h>
#include <GWCA/Managers/GuildMgr.h>
#include <GWCA/Managers/PartyMgr.h>
#include <GWCA/Managers/TradeMgr.h>
#include <GWCA/Managers/CameraMgr.h>
#include <GWCA/Managers/EffectMgr.h>
#include <GWCA/Managers/RenderMgr.h>
#include <GWCA/Managers/PlayerMgr.h>
#include <GWCA/Managers/MemoryMgr.h>
#include <GWCA/Managers/MerchantMgr.h>
#include <GWCA/Managers/SkillbarMgr.h>
#include <GWCA/Managers/GameThreadMgr.h>
#include <GWCA/Managers/FriendListMgr.h>
#include <GWCA/Managers/EventMgr.h>
#include <GWCA/Managers/QuestMgr.h>
#include <thread>
#include <GWCA/Logger/Logger.h>



namespace {
    std::thread terminating_thread;

    void Terminate() {

    }
}


namespace GW
{
    static std::vector<Module *> modules;
    static uintptr_t base_ptr;
    static uintptr_t PreGameContext_addr = 0;
    static uintptr_t GameplayContext_addr = 0;
    static bool _initialized = false;

    bool Initialize()
    {
        Logger::Instance().SetLogFile("Py4GW_injection_log.txt");
        Logger::Instance().LogInfo("Attempting to initialize GWCA");
        if (_initialized) return true;
		Logger::Instance().LogInfo("Initializing GWCA GamethreadModule");
        modules.push_back(&GameThreadModule);
		Logger::Instance().LogInfo("Initializing GWCA RenderModule");
        modules.push_back(&RenderModule);
		Logger::Instance().LogInfo("Initializing GWCA UIModule");
        modules.push_back(&UIModule);
		Logger::Instance().LogInfo("Initializing GWCA CameraModule");
        modules.push_back(&CameraModule);
		Logger::Instance().LogInfo("Initializing GWCA AgentModule");
        modules.push_back(&AgentModule);
		Logger::Instance().LogInfo("Initializing GWCA MapModule");
        modules.push_back(&MapModule);
		Logger::Instance().LogInfo("Initializing GWCA ChatModule");
        modules.push_back(&ChatModule);
		Logger::Instance().LogInfo("Initializing GWCA ItemModule");
        modules.push_back(&ItemModule);
		Logger::Instance().LogInfo("Initializing GWCA StoCModule");
        modules.push_back(&StoCModule);
		Logger::Instance().LogInfo("Initializing GWCA GuildModule");
        modules.push_back(&GuildModule);
		Logger::Instance().LogInfo("Initializing GWCA PartyModule");
        modules.push_back(&PartyModule);
		Logger::Instance().LogInfo("Initializing GWCA TradeModule");
        modules.push_back(&TradeModule);
		Logger::Instance().LogInfo("Initializing GWCA EffectModule");
        modules.push_back(&EffectModule);
		Logger::Instance().LogInfo("Initializing GWCA PlayerModule");
        modules.push_back(&PlayerModule);
		Logger::Instance().LogInfo("Initializing GWCA MerchantModule");
        modules.push_back(&MerchantModule);
		Logger::Instance().LogInfo("Initializing GWCA SkillbarModule");
        modules.push_back(&SkillbarModule);
		Logger::Instance().LogInfo("Initializing GWCA FriendListModule");
        modules.push_back(&FriendListModule);
		Logger::Instance().LogInfo("Initializing GWCA EventMgrModule");
        modules.push_back(&EventMgrModule);
		Logger::Instance().LogInfo("Initializing GWCA QuestModule");
        modules.push_back(&QuestModule);
        

		Logger::Instance().LogInfo("Initializing MemoryMgr");
        if (!MemoryMgr::Scan())
            return false;
        
        // We could get it from thread ctx
        uintptr_t address = Scanner::Find("\x50\x6A\x0F\x6A\x00\xFF\x35", "xxxxxxx", +7);

        if (Verify(address))
            base_ptr = *(uintptr_t *)address;
        GWCA_INFO("[SCAN] base_ptr = %p, %p", (void *)base_ptr);
		Logger::AssertAddress("base_ptr", base_ptr);
		// Initialize the hook system

		Logger::Instance().LogInfo("Initializing HookBase");
        HookBase::Initialize();
        

        address = Scanner::FindAssertion("\\Code\\Gw\\Ui\\Game\\GmContext.cpp", "!s_context",0, -0x9);
        if (address && Scanner::IsValidPtr(*(uintptr_t*)address, ScannerSection::Section_DATA))
            GameplayContext_addr = *(uintptr_t*)address;

        if (!GameplayContext_addr) {
            std::ostringstream oss;
            oss << "GwGetTransform_func  = " << (void*)GameplayContext_addr;
            Logger::Instance().LogError(oss.str());
        }


        address = Scanner::FindAssertion("UiPregame.cpp", "!s_scene", 0,0x34);
        if (address && Scanner::IsValidPtr(*(uintptr_t*)address, ScannerSection::Section_DATA))
            PreGameContext_addr = *(uintptr_t*)address;

        if (!PreGameContext_addr) {
            std::ostringstream oss;
            oss << "GwGetTransform_func  = " << (void*)PreGameContext_addr;
            Logger::Instance().LogError(oss.str());
        }

        GWCA_INFO("[SCAN] GameplayContext_addr = %p", GameplayContext_addr);
        GWCA_INFO("[SCAN] PreGameContext_addr = %p", PreGameContext_addr);

#ifdef _DEBUG
        GWCA_ASSERT(GameplayContext_addr);
        GWCA_ASSERT(PreGameContext_addr);
#endif
		Logger::AssertAddress("GameplayContext_addr", GameplayContext_addr);
		Logger::AssertAddress("PreGameContext_addr", PreGameContext_addr);


		Logger::Instance().LogInfo("Initializing MemoryPatcher");
        for (const Module* module : modules) {
            GWCA_INFO("\nInitializing module '%s'\n", module->name);
            if (module->init_module)
                module->init_module();
        }
        _initialized = true;

		Logger::Instance().LogInfo("Enabling hooks");
        EnableHooks();
        return true;
    }

    void EnableHooks()
    {
        if (!_initialized) return;
        HookBase::EnableHooks();
        for (const Module* module : modules) {
            if (module->enable_hooks)
                module->enable_hooks();
        }
        MemoryPatcher::EnableHooks();

    }

    void DisableHooks()
    {
        HookBase::DisableHooks();
        for (const Module* module : modules) {
            if (module->disable_hooks)
                module->disable_hooks();
        }
        MemoryPatcher::DisableHooks();
    }

    void Terminate()
    {
        if (!_initialized) return;
        DisableHooks();
        for (const Module* module : modules) {
            if (module->exit_module)
                module->exit_module();
        }

        for (size_t i = 0; i < 10; i++) {
            if (!HookBase::GetInHookCount())
                break;
            Sleep(16);
        }
        // If after waiting, we're still in a hook, presume that we're trying to disconnect GWCA inside one!
        GWCA_ASSERT(!HookBase::GetInHookCount());

        HookBase::Deinitialize();
        _initialized = false;
    }

    GameContext* GetGameContext()
    {
        uintptr_t** base_context = base_ptr ? *(uintptr_t***)base_ptr : nullptr;
        return base_context ? (GameContext*)base_context[0x6] : nullptr;
    }
    PreGameContext* GetPreGameContext()
    {
        return *(PreGameContext**)PreGameContext_addr;
    }
    WorldContext* GetWorldContext()
    {
        const auto* g = GetGameContext();
        return g ? g->world : nullptr;
    }
    PartyContext* GetPartyContext()
    {
        const auto* g = GetGameContext();
        return g ? g->party : nullptr;
    }
    CharContext* GetCharContext()
    {
        const auto* g = GetGameContext();
        return g ? g->character : nullptr;
    }
    GuildContext* GetGuildContext()
    {
        const auto* g = GetGameContext();
        return g ? g->guild : nullptr;
    }
    ItemContext* GetItemContext()
    {
        const auto* g = GetGameContext();
        return g ? g->items : nullptr;
    }
    AgentContext* GetAgentContext()
    {
        const auto* g = GetGameContext();
        return g ? g->agent : nullptr;
    }
    MapContext* GetMapContext()
    {
        const auto* g = GetGameContext();
        return g ? g->map : nullptr;
    }
    AccountContext* GetAccountContext()
    {
        const auto* g = GetGameContext();
        return g ? g->account : nullptr;
    }
    TradeContext* GetTradeContext()
    {
        const auto* g = GetGameContext();
        return g ? g->trade : nullptr;
    }
    GameplayContext* GetGameplayContext() {
        return *(GameplayContext**)GameplayContext_addr;
    }
} // namespace GW
