#pragma once

namespace Gomoku {
    inline constexpr int BOARD_SIZE          = 19;
    inline constexpr int WIN_LENGTH          = 5;
    inline constexpr int CAPTURE_WIN         = 10;  // pairs = stones removed
    inline constexpr int MIN_THREAT_RUN      = 3;   // shortest threatening run
    inline constexpr int MAX_CAPTURE_STEP    = 4;   // max stones checked per direction in AI
}
