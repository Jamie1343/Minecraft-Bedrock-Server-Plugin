// #include "../functions/setInterval.h"


// #include "claims.h"
// #include "mod/AlphaFactions.h"


// #include <cassert>
// #include <ll/api/Config.h>
// #include <ll/api/data/KeyValueDB.h>
// #include <ll/api/event/EventBus.h>
// #include <ll/api/event/ListenerBase.h>

// #include <ll/api/event/player/PlayerUseItemEvent.h>

// #include <ll/api/io/FileUtils.h>
// #include <ll/api/mod/ModManagerRegistry.h>
// #include <ll/api/mod/NativeMod.h>
// #include <ll/api/service/Bedrock.h>


// #include <ll/api/command/Command.h>
// #include <ll/api/command/CommandHandle.h>
// #include <ll/api/command/CommandRegistrar.h>
// #include <mc/server/commands/CommandOrigin.h>
// #include <mc/server/commands/CommandOutput.h>
// #include <mc/server/commands/CommandPermissionLevel.h>
// #include <mc/world/actor/player/Player.h>

// Timer t;
// bool  e = true;

// void AlphaClaims::claimManager() {

//     int i = 0;

//     t.setInterval([&]() { i++; }, 1000);


//     while (e);
// }

// void AlphaClaims::stopManager() {
//     t.setTimeout([&]() { t.stop(); }, 3000);
//     e = false;
//     alpha_factions::AlphaFactions::getInstance().getSelf().getLogger().info("Stopping Claim Manager");
// }