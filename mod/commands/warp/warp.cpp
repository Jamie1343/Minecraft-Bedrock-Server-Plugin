
#include "warp.h"
#include "ll/api/Config.h"
#include "mc/math/vector/component/IntN.h"
#include "mc/server/commands/CommandSelector.h"
#include "mod/AlphaFactions.h"

#include "ll/api/command/CommandHandle.h"
#include "ll/api/command/CommandRegistrar.h"
#include "mc/deps/core/math/Vec3.h"
#include "mc/server/commands/CommandOrigin.h"
#include "mc/server/commands/CommandOutput.h"
#include "mc/server/commands/CommandPermissionLevel.h"
#include "mc/world/actor/Actor.h"
#include <algorithm>
#include <cstddef>
#include <filesystem>
#include <iterator>
#include <mc/world/actor/player/Player.h>
#include <string>
#include <vector>

const auto& warpPath = alpha_factions::AlphaFactions::getInstance().getSelf().getDataDir() / "warps.json";

void AlphaWarps::spawn() {
    auto& spawnCommand = ll::command::CommandRegistrar::getInstance()
                             .getOrCreateCommand("spawn", "Warp To Spawn", CommandPermissionLevel::Any);


    spawnCommand.overload().execute([](CommandOrigin const& origin, CommandOutput& output) {
        const auto player = static_cast<Player*>(origin.getEntity());

        // 500107.75 69.00 500056.81
        const Vec3 spawn = {500107, 70, 500056};

        player->teleportTo(spawn, true, 0, 0, false);

        output.success();
    });
}

void AlphaWarps::home() {}

void AlphaWarps::customWarps() {


    auto& warpListCommand = ll::command::CommandRegistrar::getInstance()
                                .getOrCreateCommand("warplist", "List Of Set Waros", CommandPermissionLevel::Any);

    warpListCommand.overload().execute([](CommandOrigin const& origin, CommandOutput& output) {
        const auto        player = static_cast<Player*>(origin.getEntity());
        const std::string xuid   = player->getXuid();


        auto pred = [xuid](const AlphaWarps::warpPlayer& warps) { return warps.xuid == xuid; };

        auto exists = std::find_if(AlphaWarps::warpDb.warps.begin(), AlphaWarps::warpDb.warps.end(), pred);

        if (exists != std::end(AlphaWarps::warpDb.warps)) {
            std::size_t index    = std::distance(AlphaWarps::warpDb.warps.begin(), exists);
            const auto  warpList = AlphaWarps::warpDb.warps[index].storedWarps;

            std::string outputList;

            for (auto warp : warpList) {
                outputList += "§a" + warp.warpName + "§f - (§b" + std::to_string(std::round(warp.location.x)) + "§f, §b"
                            + std::to_string(std::round(warp.location.y)) + "§f, §b"
                            + std::to_string(std::round(warp.location.z)) + "§f)\n";
            }

            player->sendMessage(outputList);
            output.success();
        }
    });

    struct warpParams {
        std::string warpName;
    };


    auto& warpToCommand = ll::command::CommandRegistrar::getInstance()
                              .getOrCreateCommand("warp", "Warp To A Set Location", CommandPermissionLevel::Any);

    warpToCommand.overload<warpParams>()
        .required("warpName")
        .execute([](CommandOrigin const& origin, CommandOutput& output, warpParams params) {
            const auto        player   = static_cast<Player*>(origin.getEntity());
            const std::string xuid     = player->getXuid();
            const std::string warpName = params.warpName;

            auto pred = [xuid](const AlphaWarps::warpPlayer& warps) { return warps.xuid == xuid; };

            auto exists = std::find_if(AlphaWarps::warpDb.warps.begin(), AlphaWarps::warpDb.warps.end(), pred);

            if (exists != std::end(AlphaWarps::warpDb.warps)) {
                std::size_t index = std::distance(AlphaWarps::warpDb.warps.begin(), exists);

                auto nameCheck  = [warpName](const AlphaWarps::warpData warp) { return warp.warpName == warpName; };
                auto warpExists = std::find_if(
                    AlphaWarps::warpDb.warps[index].storedWarps.begin(),
                    AlphaWarps::warpDb.warps[index].storedWarps.end(),
                    nameCheck
                );
                std::size_t warpIndex = std::distance(AlphaWarps::warpDb.warps[index].storedWarps.begin(), warpExists);
                const Vec3  location  = AlphaWarps::warpDb.warps[index].storedWarps[warpIndex].location;
                player->teleportTo(location, true, 0, 0, false);
                output.success();
            }
        });


    struct setWarpParams {
        std::string name;
    };


    auto& setWarpCommand = ll::command::CommandRegistrar::getInstance()
                               .getOrCreateCommand("warpset", "Add A Warp", CommandPermissionLevel::Any);


    setWarpCommand.overload<setWarpParams>().required("name").execute(
        [](CommandOrigin const& origin, CommandOutput& output, setWarpParams params) {
            const auto        player       = static_cast<Player*>(origin.getEntity());
            const std::string xuid         = player->getXuid();
            const std::string warpName     = params.name;
            const Vec3        warpLocation = player->getPosition();

            auto pred = [xuid](const AlphaWarps::warpPlayer& warps) { return warps.xuid == xuid; };

            auto exists = std::find_if(AlphaWarps::warpDb.warps.begin(), AlphaWarps::warpDb.warps.end(), pred);

            if (exists != std::end(AlphaWarps::warpDb.warps)) {
                std::size_t index = std::distance(AlphaWarps::warpDb.warps.begin(), exists);
                auto nameCheck    = [warpName](const AlphaWarps::warpData warp) { return warp.warpName == warpName; };
                auto warpExists   = std::find_if(
                    AlphaWarps::warpDb.warps[index].storedWarps.begin(),
                    AlphaWarps::warpDb.warps[index].storedWarps.end(),
                    nameCheck
                );
                if (warpExists != std::end(AlphaWarps::warpDb.warps[index].storedWarps)) {
                    output.error("You Already Have A Warp Called " + warpName);
                    return;
                }

                const Vec3 roundedLocation =
                    {std::round(warpLocation.x), std::round(warpLocation.y), std::round(warpLocation.z)};

                AlphaWarps::warpData data = {warpName, roundedLocation};

                AlphaWarps::warpDb.warps[index].storedWarps.push_back(data);
                ll::config::saveConfig(AlphaWarps::warpDb, warpPath);
                // auto duplicateName = std
                // exists
            } else {

                const AlphaWarps::warpData warpInitData = {warpName, warpLocation};
                // not exist
                const std::vector<AlphaWarps::warpData> warpData = {warpInitData};

                const AlphaWarps::warpPlayer data = {xuid, warpData};

                AlphaWarps::warpDb.warps.push_back(data);

                ll::config::saveConfig(AlphaWarps::warpDb, warpPath);
            }
        }
    );
}

void AlphaWarps::loadWarps() {

    if (std::filesystem::exists(warpPath)) {
        ll::config::loadConfig(AlphaWarps::warpDb, warpPath);
        alpha_factions::AlphaFactions::getInstance().getSelf().getLogger().info("Warps Loaded");
    } else {
        ll::config::saveConfig(AlphaWarps::warpDb, warpPath);
        alpha_factions::AlphaFactions::getInstance().getSelf().getLogger().info("Warps Not Found. Created");
    }
}