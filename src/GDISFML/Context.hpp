#pragma once

namespace Context {
    enum class State { // Plant state
        Idle,
        CanShoot,
        Reloading,
        Dead,
    };
}
