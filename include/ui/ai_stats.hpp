#pragma once

#include "ui/ui_state.hpp"
#include <algorithm>
#include <limits>

struct AiStatsAccumulator {
    void record(float ms, int depth) {
        m_lastMs = ms;
        m_lastDepth = depth;
        if (m_count == 0) {
            m_minMs = m_maxMs = ms;
            m_minDepth = m_maxDepth = depth;
        } else {
            m_minMs = std::min(m_minMs, ms);
            m_maxMs = std::max(m_maxMs, ms);
            m_minDepth = std::min(m_minDepth, depth);
            m_maxDepth = std::max(m_maxDepth, depth);
        }
        m_totalMs += ms;
        m_totalDepth += depth;
        ++m_count;
    }

    UIState::AiStats snapshot() const {
        if (m_count == 0) return {};
        return {
            m_lastMs, m_minMs, m_maxMs, m_totalMs / m_count,
            m_lastDepth, m_minDepth, m_maxDepth, m_totalDepth / m_count
        };
    }

    void reset() {
        m_lastMs = m_minMs = m_maxMs = m_totalMs = 0.f;
        m_lastDepth = m_minDepth = m_maxDepth = m_totalDepth = m_count = 0;
    }

private:
    float m_lastMs = 0, m_minMs = 0, m_maxMs = 0, m_totalMs = 0;
    int m_lastDepth = 0, m_minDepth = 0, m_maxDepth = 0, m_totalDepth = 0;
    int m_count = 0;
};
