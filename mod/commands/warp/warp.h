#ifndef WARPS
#define WARPS

#include "mc/deps/core/math/Vec3.h"
#include <string>
#include <vector>
namespace AlphaWarps {

void spawn();

void home();

void customWarps();


void loadWarps();

struct warpData {
    std::string warpName;
    Vec3        location;
};
struct warpPlayer {
    std::string           xuid;
    std::vector<warpData> storedWarps;
};

struct storedWarps {
    int                     version = 1;
    std::vector<warpPlayer> warps;
};

#ifdef WARP_LOCATIONS
static inline storedWarps warpDb{};
#endif

} // namespace AlphaWarps


#endif