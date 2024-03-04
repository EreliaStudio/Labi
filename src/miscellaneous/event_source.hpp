#pragma once

#include "sparkle.hpp"

enum class Event
{
    UpdateVisibleChunkRequest
};

using EventSource = spk::Singleton<spk::EventManager<Event>>;
