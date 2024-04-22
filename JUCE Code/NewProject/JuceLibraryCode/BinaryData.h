/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   DenContainer48k_wav;
    const int            DenContainer48k_wavSize = 59282;

    extern const char*   DenHall48k_wav;
    const int            DenHall48k_wavSize = 202571;

    extern const char*   Tent48k_wav;
    const int            Tent48k_wavSize = 59645;

    extern const char*   ChurchIR1_wav;
    const int            ChurchIR1_wavSize = 248447;

    extern const char*   Birds_LFO_wav;
    const int            Birds_LFO_wavSize = 1728044;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 5;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
