
#include "ll/api/command/CommandHandle.h"
#include "ll/api/command/CommandRegistrar.h"
#include "ll/api/service/Bedrock.h"
#include "mc/server/commands/CommandOutput.h"
#include "mc/server/commands/CommandParameterOption.h"
#include "mc/server/commands/CommandPermissionLevel.h"
#include "mc/server/commands/CommandSelectorResults.h"
#include "mc/world/actor/Actor.h"
#include "mc/world/actor/ActorType.h"
#include <mc/world/actor/player/Player.h>
#include <string>
#include <vector>

#include "../alphaFactions.h"
#include "commands.h"
#include "mod/bans/ban.h"
#include "warp/warp.h"


bool AlphaCommands::registerCommands() {
    auto commandReg = ll::service::getCommandRegistry();
    if (!commandReg) {
        throw std::runtime_error("failed to get command registry");
    }

    AlphaCommands::hi();
    AlphaBans::banPlayer();
    AlphaBans::unbanPlayer();
    AlphaWarps::spawn();
    AlphaWarps::customWarps();

    return true;
}

void AlphaCommands::hi() {


    auto& pingCommand =
        ll::command::CommandRegistrar::getInstance().getOrCreateCommand("hi", "hi", CommandPermissionLevel::Any);


    pingCommand.overload().execute([](CommandOrigin const& origin, CommandOutput& output) {
        auto* entity = origin.getEntity();
        if (entity == nullptr || !entity->isType(ActorType::Player)) {
            output.error("Only players can use this");
            return;
        }
        Player* player = ((Player*)entity);

        player->sendMessage("Hi");
    });
}