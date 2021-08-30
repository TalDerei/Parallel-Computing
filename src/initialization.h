/* Header file for Set.cc */

#ifndef INITIALIZATION_DEF
#define INITIALIZATION_DEF

#include <map>

class Initialization {
    public:
        static void initialize_map(std::map<int, float> &);

    struct Account {
        int id = 0;
        float balance = 0.0;
    };
};

#endif 