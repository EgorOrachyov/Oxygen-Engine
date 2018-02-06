/*
 *  Project: Oxygen Game Engine
 *  Created by Egor Orachyov on 04.02.2018.
 *  Copyright
 *
 *  xStrings is own realization of
 *  some functions for comfortable
 *  work with strings
 */

#ifndef OXYGEN_XSTRINGS_H
#define OXYGEN_XSTRINGS_H

// From wchar_t* to char*
void ConvertWCharToChar(char * des, wchar_t * res) {
    wcstombs(des, res, STRING_SIZE);
}

// From char* to wchar_t*
void ConvertCharToWChar(wchar_t * des, char * res) {
    mbstowcs(des, res, STRING_SIZE);
}

#endif //OXYGEN_XSTRINGS_H
