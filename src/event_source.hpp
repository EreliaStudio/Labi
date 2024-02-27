#pragma once

#include "sparkle.hpp"

enum class Event
{
    UpdateVisibleChunk
};

using EventSource = spk::Singleton<spk::EventManager<Event>>;
