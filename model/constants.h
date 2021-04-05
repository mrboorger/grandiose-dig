#ifndef MODEL_CONSTANTS_H_
#define MODEL_CONSTANTS_H_

#include <QPointF>

namespace constants {

constexpr double kEps = 0.00001;         // in blocks
constexpr double kAirResistance = 0.05;  // in percents
constexpr int kBlockSz = 16;             // in pixels
constexpr int kTickDurationMsec = 10;
constexpr double kAbsoluteMaxSpeedX = 50.0;       // in blocks per tick
constexpr double kAbsoluteMaxSpeedY = 50.0;       // in blocks per tick
constexpr QPointF kPlayerSize{0.75, 1.75};        // in blocks
constexpr double kPlayerWalkAcceleration = 0.01;  // in blocks per tick
constexpr double kPlayerWalkMaxSpeed = 0.1;       // in blocks per tick
constexpr double kPlayerWalkAirAcceleration =
    kPlayerWalkAcceleration;  // in blocks per tick
constexpr double kPlayerWalkMaxAirAcceleration =
    kPlayerWalkMaxSpeed;                      // in blocks per tick
constexpr double kPlayerGravitySpeed = 0.01;  // in blocks per tick
constexpr double kPlayerJumpSpeed = -0.3;     // in blocks per tick

constexpr double kMobWalkAcceleration =
    kPlayerWalkAcceleration / 3;  // in blocks per tick
constexpr double kMobWalkMaxSpeed =
    kPlayerWalkMaxSpeed / 2;  // in blocks per tick
constexpr double kMobWalkMaxAirAcceleration =
    kPlayerWalkMaxAirAcceleration / 2;                    // in blocks per tick
constexpr double kMobJumpSpeed = kPlayerJumpSpeed / 1.5;  // in blocks per tick

constexpr double kBasicStrategyVisionRadius = 12.0;  // in blocks
constexpr int kBasicStrategyWalkTicksCount = 300;
constexpr int kBasicStrategyAttackTicksCount = 100;
constexpr double kBasicStrategyWalkPrecision = 0.1;  // in blocks
constexpr int kBasicStrategyRandomWalkChance =
    100;  // chance^(-1) * 100 percents per tick

constexpr int kDefaultClearTimeMSec = 1'000;

}  // namespace constants

#endif  // MODEL_CONSTANTS_H_
