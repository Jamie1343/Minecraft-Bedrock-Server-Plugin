#ifndef BANS
#define BANS


#include "mc/server/commands/CommandSelector.h"
#include "mc/world/actor/Actor.h"
#include "mc/world/actor/player/Player.h"
#include <string>
#include <vector>

namespace AlphaBans {

struct banParams {
    CommandSelector<Player> player;
};

struct unbanParams {
    std::string player;
};

struct banData {
    std::string realName;
    std::string xuid;
    std::string playerId;
    std::string ip;
};

struct playersBanned {
    int                  version = 1;
    std::vector<banData> data;
};

void banPlayer();

void unbanPlayer();

void banManager();

void loadBans();

#ifdef BAN_PLAYERS
static inline playersBanned banDb{};
#endif

} // namespace AlphaBans

#endif