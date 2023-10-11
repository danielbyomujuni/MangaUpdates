//
// Created by Daniel Byomujuni on 10/11/23.
//

#ifndef MANGAUPDATES_LATESTCOMMAND_H
#define MANGAUPDATES_LATESTCOMMAND_H


#include <vector>
#include "BaseCommand.h"

class LatestCommand: public BaseCommand {

public:
    LatestCommand();
    void execute(std::vector<std::string> args);
};


#endif //MANGAUPDATES_LATESTCOMMAND_H
