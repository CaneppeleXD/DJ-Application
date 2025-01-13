/*
  ==============================================================================

    Utils.cpp
    Created: 20 Feb 2024 7:33:53pm
    Author:  Pichau

  ==============================================================================
*/

#include "Utils.h"

int Utils::clearInteger(std::string dirtInteger) {
    std::string cleanInteger = "";
    char allowedChars[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
    for (int i = 0; i < dirtInteger.length(); ++i) {
        bool in = false;
        for (auto _char : allowedChars) {
            if (_char == dirtInteger[i]) {
                in = true;
                break;
            }
        }
        if (in)
            cleanInteger += dirtInteger[i];
    }
    if (cleanInteger.length() > 0)
        return std::stoi(cleanInteger);
    else
        return -1;
}
