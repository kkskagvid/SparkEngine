#pragma once

namespace Spark::Core::Event
{
    enum class EventCategory {
        EC_None = 0,
        EC_Player,
        EC_UI,
        EC_AI,
        EC_Physics,
        EC_Network,
        EC_Debug
    };
}
