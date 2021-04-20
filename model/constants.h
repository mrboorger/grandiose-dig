#ifndef MODEL_CONSTANTS_H_
#define MODEL_CONSTANTS_H_

#include <QPointF>

namespace constants {

constexpr double pi = 3.141592653589;
constexpr double kEps = 0.00001;         // in blocks
constexpr double kAirResistance = 0.05;  // in percents
constexpr int kBlockSz = 16;             // in pixels
constexpr int kTickDurationMsec = 10;
constexpr double kAbsoluteMaxSpeedX = 50.0;  // in blocks per tick
constexpr double kAbsoluteMaxSpeedY = 50.0;  // in blocks per tick
constexpr int kDamageCooldown = 50;          // in ticks
constexpr double kFallDamageMin = 0.4;
constexpr double kFallDamagePoint = 0.005;

constexpr QPointF kPlayerSize{0.75, 1.75};        // in blocks
constexpr double kPlayerWalkAcceleration = 0.01;  // in blocks per tick
constexpr double kPlayerWalkMaxSpeed = 0.1;       // in blocks per tick
constexpr double kPlayerWalkAirAcceleration =
    kPlayerWalkAcceleration;  // in blocks per tick
constexpr double kPlayerWalkMaxAirAcceleration =
    kPlayerWalkMaxSpeed;                      // in blocks per tick
constexpr double kPlayerGravitySpeed = 0.01;  // in blocks per tick
constexpr double kPlayerJumpSpeed = -0.3;     // in blocks per tick
constexpr QPointF kPlayerDamageAcceleration = {0.1, -0.15};
constexpr int kPlayerHealth = 100;
constexpr int kPlayerDamage = 10;
constexpr double kPlayerUpperAttackAngle = pi / 4.0;
constexpr double kPlayerLowerAttackAngle = -pi / 4.0;
constexpr int kPlayerAttackTime = 30;
constexpr int kPlayerAttackCooldown = 70;
constexpr double kPlayerAngleTick =
    (kPlayerUpperAttackAngle - kPlayerLowerAttackAngle) / kPlayerAttackTime;
constexpr double kPlayerAttackRadius = 5;

constexpr double kMobWalkAcceleration =
    kPlayerWalkAcceleration / 2.5;  // in blocks per tick
constexpr double kMobWalkMaxSpeed =
    kPlayerWalkMaxSpeed / 1.8;  // in blocks per tick
constexpr double kMobWalkMaxAirAcceleration =
    kPlayerWalkMaxAirAcceleration / 2;                    // in blocks per tick
constexpr double kMobJumpSpeed = kPlayerJumpSpeed / 1.4;  // in blocks per tick
constexpr double kMobJumpHeightInBlocks = 2;              // in blocks
constexpr double kMobJumpLengthInBlocks = 2;              // in blocks
constexpr QPointF kMobDamageAcceleration = {0.1, -0.15};
constexpr int kMobHealth = 120;
constexpr int kMobDamage = 15;

constexpr double kBasicStrategyVisionRadius = 12.0;  // in blocks
constexpr int kBasicStrategyWalkTicksCount = 300;
constexpr int kBasicStrategyAttackTicksCount = 100;
constexpr double kBasicStrategyWalkPrecision = 0.1;  // in blocks
constexpr double kBasicStrategyRandomWalkChance = 0.01;

constexpr double kMobSoundChance = 0.001 / 1.5;

constexpr int kDefaultClearTimeMSec = 1'000;

}  // namespace constants

#endif  // MODEL_CONSTANTS_H_
