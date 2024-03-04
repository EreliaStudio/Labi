#pragma once

#include "sparkle.hpp"

enum class Event
{
    UpdateVisibleChunkRequest,
    PlayerMotionUp,
    PlayerMotionLeft,
    PlayerMotionDown,
    PlayerMotionRight,
    EnterBattleMode,
    ExitBattleMode
};

using EventSource = spk::Singleton<spk::EventManager<Event>>;
