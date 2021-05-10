#ifndef MODEL_CONSTANTS_H_
#define MODEL_CONSTANTS_H_

#include <QPointF>

namespace constants {

constexpr double pi = 3.141592653589;
constexpr double kEps = 0.00001;         // in blocks
constexpr double kAirResistance = 0.05;  // in percents
constexpr int kBlockSz = 16;             // in pixels
constexpr int kTickDurationMsec = 10;
constexpr double kAbsoluteMaxSpeedX = 5.0;  // in blocks per ms
constexpr double kAbsoluteMaxSpeedY = 5.0;  // in blocks per ms
constexpr int kDamageCooldown = 500;        // in ms
constexpr double kFallDamageMin = 0.04;
constexpr double kFallDamagePoint = 0.0005;

constexpr QPointF kPlayerSize{1.25, 2.5};               // in blocks
constexpr double kPlayerWalkAcceleration = 0.001 / 10;  // in blocks per ms
constexpr double kPlayerWalkMaxSpeed = 0.01;            // in blocks per ms
constexpr double kPlayerWalkAirAcceleration =
    kPlayerWalkAcceleration;  // in blocks per tick
constexpr double kPlayerWalkMaxAirAcceleration =
    kPlayerWalkMaxSpeed;                            // in blocks per ms
constexpr double kPlayerGravitySpeed = 0.001 / 10;  // in blocks per ms
constexpr double kPlayerJumpSpeed = -0.03;          // in blocks per ms
constexpr QPointF kPlayerDamageAcceleration = {0.01, -0.015};
constexpr int kPlayerHealth =
    100000;  // TODO(Wind-Eagle): this is temporary value
constexpr int kPlayerDamage = 10;
constexpr double kPlayerUpperAttackAngle = pi / 4.0;
constexpr double kPlayerLowerAttackAngle = -pi / 4.0;
constexpr int kPlayerAttackTime = 300;
constexpr int kPlayerAttackCooldown = 700;
constexpr double kPlayerAngleTick =
    (kPlayerUpperAttackAngle - kPlayerLowerAttackAngle) / kPlayerAttackTime;
constexpr double kPlayerAttackRadius = 5;
constexpr int kPlayerWalkAnimation = 50;  // in ms
constexpr int kPlayerWalkPictures = 4;
constexpr int kPlayerAttackAnimation = 50;  // in ms
constexpr int kPlayerAttackPictures = 4;    // number of pictures in animation

constexpr double kSpeedEffectMultiplier = 1.2;
constexpr double kSlownessEffectMultiplier = 0.8;
constexpr double kStrengthEffectMultiplier = 1.5;
constexpr double kWeaknessEffectMultiplier = 1.0 / 1.5;
constexpr double kPoisonEffectMultiplier = 5;
constexpr double kRegenerationEffectMultiplier = 5;
constexpr double kLightnessEffectMultiplier = 0.8;
constexpr double kHeavinessEffectMultiplier = 1.0 / 0.8;

constexpr double kZombieWalkAcceleration =
    kPlayerWalkAcceleration / 2.5;  // in blocks per ms
constexpr double kZombieWalkMaxSpeed =
    kPlayerWalkMaxSpeed / 1.8;  // in blocks per ms
constexpr double kZombieWalkMaxAirAcceleration =
    kPlayerWalkMaxAirAcceleration / 2;                       // in blocks per ms
constexpr double kZombieJumpSpeed = kPlayerJumpSpeed / 1.4;  // in blocks per ms
constexpr QPoint kZombieJumpInBlocks = {2, 2};               // in blocks
constexpr QPointF kZombieSize = kPlayerSize;
constexpr QPointF kZombieDamageAcceleration = {0.01, -0.015};
constexpr int kZombieHealth = 120;
constexpr int kZombieDamage = 15;
constexpr int kZombieWalkAnimation = 100;  // in ms
constexpr int kZombieWalkPictures = 4;
constexpr int kZombieAttackAnimation = 100;  // in ms
constexpr int kZombieAttackPictures = 4;     // number of pictures in animation

constexpr double kZombieLordWalkAcceleration =
    kPlayerWalkAcceleration / 1.5;  // in blocks per ms
constexpr double kZombieLordWalkMaxSpeed =
    kPlayerWalkMaxSpeed / 1.3;  // in blocks per ms
constexpr double kZombieLordWalkMaxAirAcceleration =
    kPlayerWalkMaxAirAcceleration / 1.2;  // in blocks per ms
constexpr double kZombieLordJumpSpeed =
    kPlayerJumpSpeed / 1.2;                         // in blocks per ms
constexpr QPoint kZombieLordJumpInBlocks = {4, 3};  // in blocks
constexpr QPointF kZombieLordSize = {1.875, 3.75};
constexpr QPointF kZombieLordDamageAcceleration = {0.025, -0.025};
constexpr int kZombieLordHealth = 200;
constexpr int kZombieLordDamage = 25;
constexpr int kZombieLordWalkAnimation = 100;  // in ms
constexpr int kZombieLordWalkPictures = 4;
constexpr int kZombieLordAttackAnimation = 100;  // in ms
constexpr int kZombieLordAttackPictures = 4;  // number of pictures in animation

constexpr double kQuioxWalkAcceleration =
    kPlayerWalkAcceleration * 1.5;  // in blocks per ms
constexpr double kQuioxWalkMaxSpeed =
    kPlayerWalkMaxSpeed * 0.9;  // in blocks per ms
constexpr double kQuioxWalkMaxAirAcceleration =
    kPlayerWalkMaxAirAcceleration * 1.1;                    // in blocks per ms
constexpr double kQuioxJumpSpeed = kPlayerJumpSpeed / 1.4;  // in blocks per ms
constexpr QPoint kQuioxJumpInBlocks = {4, 2};               // in blocks
constexpr QPointF kQuioxSize = {2.875, 1.375};
constexpr QPointF kQuioxDamageAcceleration = {0.01, -0.015};
constexpr int kQuioxHealth = 150;
constexpr int kQuioxDamage = 15;
constexpr int kQuioxWalkAnimation = 100;  // in ms
constexpr int kQuioxWalkPictures = 2;
constexpr int kQuioxAttackAnimation = 100;  // in ms
constexpr int kQuioxAttackPictures = 2;     // number of pictures in animation

constexpr int kMagicQuioxHealth = 200;
constexpr int kMagicQuioxDamage = 10;
constexpr int kMagicStrategyEffectDuration = 10000;
constexpr double kMagicStrategyEffectChance = 0.2;

constexpr double kBasicStrategyVisionRadius = 16.0;  // in blocks
constexpr double kBasicStrategyWalkTimeCount = 3000;
constexpr double kBasicStrategyAttackTimeCount = 1000;
constexpr double kBasicStrategyWalkPrecision = 0.1;  // in blocks
constexpr double kBasicStrategyRandomWalkChance = 0.01;
constexpr double kBasicStrategyRandomWalkDistance = 10;  // in blocks

constexpr double kBasicSummonerStrategyVisionRadius = 24.0;  // in blocks
constexpr double kBasicSummonerStrategyWalkTime = 5000;
constexpr double kBasicSummonerStrategyAttackTime = 1250;
constexpr double kBasicSummonerStrategySummonTime = 1000;  // before check
constexpr double kBasicSummonerStrategySummonDistance = 7.5;
constexpr double kBasicSummonerStrategySummonChance = 0.2;
constexpr int kBasicSummonerStrategySummonAttempts = 200;    // in blocks
constexpr double kBasicSummonerStrategyWalkPrecision = 0.1;  // in blocks
constexpr double kBasicSummonerStrategyRandomWalkChance = 0.015;
constexpr double kBasicSummonerStrategyRandomWalkDistance = 15;  // in blocks

constexpr double kEffectInterval = 1000;          // in ms
constexpr double kMobSoundChance = 1.0 / 1500.0;  // once in 1500 ticks

constexpr int kDefaultClearTimeMSec = 1'000;

}  // namespace constants

#endif  // MODEL_CONSTANTS_H_
