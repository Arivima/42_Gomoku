#pragma once

#include <SFML/Graphics.hpp>

struct Palette {
    // ── Board ─────────────────────────────────────────────────────────────────
    sf::Color boardBackground;        // wooden surface + window clear color
    sf::Color boardLine;      // grid lines, star points
    sf::Color boardCoordinate;     // A-T / 1-19 coordinate labels

    // ── Ring overlays ─────────────────────────────────────────────────────────
    sf::Color ringLastMove;   // yellow  — last placed stone
    sf::Color ringForbidden;  // red     — double-three violation
    sf::Color ringThreat;     // orange  — opponent threat cells
    sf::Color ringSuggestion; // green   — coach move suggestions

    // ── Surfaces ──────────────────────────────────────────────────────────────
    sf::Color surface;        // panel, popup box, start screen background
    sf::Color surfaceSecondary; // bottom bar (slightly darker/lighter)
    sf::Color border;         // separators, dividers, idle buttons

    // ── Text ──────────────────────────────────────────────────────────────────
    sf::Color text;           // primary text everywhere
    sf::Color textSecondary;        // secondary text — labels, hints, column headers

    // ── Accents ───────────────────────────────────────────────────────────────
    sf::Color accent;         // blue  — selected button, turn bg, underline
    sf::Color accentActive;   // green — active toggle, win, start button
    sf::Color accentInactive; // gray  — inactive/disabled
    sf::Color accentDanger;   // red   — illegal move, capture event
    sf::Color accentWarning;  // amber — endgame capture

    // ── Popup overlay ─────────────────────────────────────────────────────────
    sf::Color popupBackdrop;  // semi-transparent backdrop behind popups

    // ── Warning box (panel endgame notice) ────────────────────────────────────
    sf::Color warningBackground; // amber-tinted box fill
    sf::Color warningText;       // amber warning text

    // ─────────────────────────────────────────────────────────────────────────

    static Palette dark() {
        Palette p;
        p.boardBackground   = sf::Color(222, 184, 135);
        p.boardLine      = sf::Color(  0,   0,   0);
        p.boardCoordinate = sf::Color( 90,  55,  20);

        p.ringLastMove   = sf::Color(255, 215,   0);
        p.ringForbidden  = sf::Color(220,  50,  50);
        p.ringThreat     = sf::Color(255, 140,   0);
        p.ringSuggestion = sf::Color( 50, 200,  50);

        p.surface        = sf::Color( 40,  45,  55);
        p.surfaceSecondary = sf::Color( 25,  28,  35);
        p.border         = sf::Color( 70,  75,  85);

        p.text           = sf::Color(220, 220, 220);
        p.textSecondary = sf::Color(140, 140, 140);

        p.accent         = sf::Color( 50, 100, 200);
        p.accentActive   = sf::Color( 75, 190,  90);
        p.accentInactive = sf::Color(160, 160, 160);
        p.accentDanger   = sf::Color(200,  60,  60);
        p.accentWarning  = sf::Color(200, 130,   0);

        p.popupBackdrop  = sf::Color(  0,   0,   0, 160);

        p.warningBackground = sf::Color( 60,  40,  10);
        p.warningText       = sf::Color(255, 180,   0);
        return p;
    }

    static Palette light() {
        Palette p;
        p.boardBackground   = sf::Color(245, 222, 175);
        p.boardLine      = sf::Color( 80,  50,  20);
        p.boardCoordinate     = sf::Color( 60,  35,  10);

        p.ringLastMove   = sf::Color(210, 165,   0);
        p.ringForbidden  = sf::Color(210,  40,  40);
        p.ringThreat     = sf::Color(215, 105,   0);
        p.ringSuggestion = sf::Color( 30, 160,  50);

        p.surface        = sf::Color(245, 245, 250);
        p.surfaceSecondary = sf::Color(228, 228, 236);
        p.border         = sf::Color(200, 200, 210);

        p.text           = sf::Color( 25,  25,  35);
        p.textSecondary        = sf::Color(130, 130, 140);

        p.accent         = sf::Color( 50, 100, 200);
        p.accentActive   = sf::Color( 40, 160,  58);
        p.accentInactive = sf::Color(160, 160, 170);
        p.accentDanger   = sf::Color(200,  50,  50);
        p.accentWarning  = sf::Color(190, 125,   0);

        p.popupBackdrop  = sf::Color( 40,  40,  50, 120);

        p.warningBackground = sf::Color(255, 248, 210);
        p.warningText       = sf::Color(150,  90,   0);
        return p;
    }
};
