#include "ban.h"
#include <ll/api/Config.h>
#include <ll/api/command/Command.h>


#include <ll/api/command/CommandHandle.h>
#include <ll/api/command/CommandRegistrar.h>
#include <mc/server/commands/CommandOrigin.h>
#include <mc/server/commands/CommandOutput.h>
#include <mc/server/commands/CommandPermissionLevel.h>

#include <ll/api/event/EventBus.h>
#include <ll/api/event/ListenerBase.h>
#include <ll/api/event/player/PlayerJoinEvent.h>

#include "ll/api/event/EventBus.h"
#include "ll/api/event/player/PlayerEvent.h"
#include "mc/network/ServerNetworkHandler.h"
#include "mc/network/connection/DisconnectFailReason.h"


#include "ll/api/service/Bedrock.h"
#include "mc/world/actor/player/Player.h"
#include "mod/AlphaFactions.h"
#include <filesystem>
#include <string>

const auto& path = alpha_factions::AlphaFactions::getInstance().getSelf().getDataDir() / "banned_players.json";

void AlphaBans::loadBans() {

    if (std::filesystem::exists(path)) ll::config::loadConfig(AlphaBans::banDb, path);
    else ll::config::saveConfig(AlphaBans::banDb, path);
}

void AlphaBans::banPlayer() {


    auto& banPlayerCommand = ll::command::CommandRegistrar::getInstance()
                                 .getOrCreateCommand("ban", "Ban A Player", CommandPermissionLevel::GameDirectors);

    banPlayerCommand.overload<AlphaBans::banParams>().required("player").execute(
        [](CommandOrigin const& origin, CommandOutput& output, AlphaBans::banParams const& param) {
            Player* runner = ((Player*)origin.getEntity());

            Player* bannedPlayer = ((Player*)param.player.results(origin).begin()[0]);

            runner->sendMessage(bannedPlayer->getRealName());

            const AlphaBans::banData data = {
                bannedPlayer->getRealName(),
                bannedPlayer->getXuid(),
                std::to_string(bannedPlayer->getUserId()),
                bannedPlayer->getIPAndPort()
            };

            banDb.data.push_back(data);

            ll::config::saveConfig(banDb, path);

            ll::service::getServerNetworkHandler()->disconnectClient(
                bannedPlayer->getNetworkIdentifier(),
                Connection::DisconnectFailReason::Kicked,
                "You Are Banned, Go To https://discord.gg/yK66xcUUGp To Appeal It",
                "",
                false
            );
        }
    );
}

void AlphaBans::unbanPlayer() {

    auto& unbanPlayerCommand = ll::command::CommandRegistrar::getInstance().getOrCreateCommand(
        "unban",
        "unban a player",
        CommandPermissionLevel::GameDirectors
    );

    unbanPlayerCommand.overload<AlphaBans::unbanParams>().required("player").execute(
        [](CommandOrigin const& origin, CommandOutput& output, AlphaBans::unbanParams const& params) {
            for (auto i = AlphaBans::banDb.data.begin(); i != AlphaBans::banDb.data.end(); i++) {
                if (i->realName == params.player) {
                    AlphaBans::banDb.data.erase(i);
                    ll::config::saveConfig(AlphaBans::banDb, path);
                    ((Player*)origin.getEntity())->sendMessage(i->realName + "Was Unbanned");
                }
            }
        }
    );
}


void AlphaBans::banManager() {
    auto& eventBus = ll::event::EventBus::getInstance();

    eventBus.emplaceListener<ll::event::PlayerJoinEvent>([&](ll::event::PlayerEvent& ev) {
        if (ev.self().isSimulatedPlayer()) return;

        for (auto i = AlphaBans::banDb.data.begin(); i != AlphaBans::banDb.data.end(); i++) {
            if (i->realName == ev.self().getRealName()) {

                alpha_factions::AlphaFactions::getInstance().getSelf().getLogger().info(
                    ev.self().getRealName() + "Tried To Join While Banned"
                );

                ll::service::getServerNetworkHandler()->disconnectClient(
                    ev.self().getNetworkIdentifier(),
                    Connection::DisconnectFailReason::Kicked,
                    "You Are Banned, Go To https://discord.gg/yK66xcUUGp To Appeal It",
                    "",
                    false
                );
            }
        }
    });
}