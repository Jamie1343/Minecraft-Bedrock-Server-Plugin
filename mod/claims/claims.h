#ifndef CLAIMS
#define CLAIMS

#include "mc/server/commands/CommandSelector.h"
namespace AlphaClaims {

void claimManager();

// void stopManager();

int test();

struct inviteParams {
    CommandSelector<Player> player;
};

struct joinParams {
    CommandSelector<Player> player;
};

} // namespace AlphaClaims


#endif