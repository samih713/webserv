#ifndef ERROR_HPP
#define ERROR_HPP

/* --------------------------------- COLORS --------------------------------- */
#define SET_COLOR(color) (std::cout << (color))
#define RE_COLOR         (std::cout << RE)

#define RE "\001\033[0;39m\002" // reset
#define L  "\001\033[1;92m\002" // green light
#define Y  "\001\033[1;93m\002" // yellow
#define M  "\001\033[1;95m\002" // magenta
#define R  "\001\033[1;91m\002" // red
#define B  "\001\033[1;94m\002" // blue
#define C  "\001\033[1;96m\002" // cyan
#define W  "\001\033[1;97m\002" // white
#define D  "\001\033[1;30m\002" // gray
#define O  "\001\033[1;33m\002" // orange

/* ------------------------------- DEBUG MACRO ------------------------------ */
#ifndef __DEBUG__
#define DEBUG_MSG(message, color)
#else
#define DEBUG_MSG(message, color)                                                        \
    do {                                                                                 \
        std::cerr << (color);                                                            \
        std::cerr << message;                                                            \
        std::cerr << std::endl;                                                          \
        std::cerr << RE;                                                                 \
        std::cerr.flush();                                                               \
    } while (0)
#endif // __DEBUG__

/* ------------------------------- DEBUG BUILD ------------------------------ */
#ifdef __DEBUG__
#include <cassert>
#define DEBUGASSERT(x) assert(x)
#else
#define DEBUGASSERT(x)
#endif // __DEBUG__

#endif // ERROR_HPP
