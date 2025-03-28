#include "mod/AlphaFactions.h"
#include "./bans/ban.h"

#include <memory>

#include "AlphaFactions.h"
#include "claims/claims.h"
#include "commands/commands.h"
#include "commands/warp/warp.h"

#include "ll/api/mod/RegisterHelper.h"

#include "ll/api/mod/RegisterHelper.h"
#include "mc/deps/json/Value.h"
#include "mc/world/actor/Actor.h"
#include <array>
#include <cassert>
#include <filesystem>
#include <functional>
#include <iostream>
#include <iterator>
#include <ll/api/Config.h>
#include <ll/api/data/KeyValueDB.h>
#include <ll/api/event/EventBus.h>
#include <ll/api/event/ListenerBase.h>

#include <ll/api/event/player/PlayerUseItemEvent.h>

#include <ll/api/io/FileUtils.h>
#include <ll/api/mod/ModManagerRegistry.h>
#include <ll/api/mod/NativeMod.h>
#include <ll/api/service/Bedrock.h>


#include <ll/api/command/Command.h>
#include <ll/api/command/CommandHandle.h>
#include <ll/api/command/CommandRegistrar.h>
#include <mc/server/commands/CommandOrigin.h>
#include <mc/server/commands/CommandOutput.h>
#include <mc/server/commands/CommandPermissionLevel.h>
#include <mc/world/actor/player/Player.h>


namespace alpha_factions {

AlphaFactions& AlphaFactions::getInstance() {
    static AlphaFactions instance;
    return instance;
}

bool AlphaFactions::load() {
    getSelf().getLogger().debug("Loading...");
    // Code for loading the mod goes here.
    return true;
}

bool AlphaFactions::enable() {
    getSelf().getLogger().debug("Enabling...");
    AlphaCommands::registerCommands();
    AlphaBans::loadBans();
    AlphaWarps::loadWarps();
    AlphaBans::banManager();
    // AlphaClaims::claimManager();
    getSelf().getLogger().info("Commands Registered");
    // Code for enabling the mod goes here.
    return true;
}

bool AlphaFactions::disable() {
    getSelf().getLogger().debug("Disabling...");
    // AlphaClaims::stopManager();
    // Code for disabling the mod goes here.
    return true;
}


} // namespace alpha_factions

LL_REGISTER_MOD(alpha_factions::AlphaFactions, alpha_factions::AlphaFactions::getInstance());
