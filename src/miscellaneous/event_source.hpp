#pragma once

#include "sparkle.hpp"

enum class Event
{
    UpdateVisibleChunkRequest,
    PlayerMotionUp,
    PlayerMotionLeft,
    PlayerMotionDown,
    PlayerMotionRight
};

using EventSource = spk::Singleton<spk::EventManager<Event>>;
