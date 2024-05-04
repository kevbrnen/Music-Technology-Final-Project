/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   Ballinafad_Castle48k_wav;
    const int            Ballinafad_Castle48k_wavSize = 63281;

    extern const char*   Ballymote_Castle48k_wav;
    const int            Ballymote_Castle48k_wavSize = 37115;

    extern const char*   Bedroom48k_wav;
    const int            Bedroom48k_wavSize = 31301;

    extern const char*   church_248k_wav;
    const int            church_248k_wavSize = 276155;

    extern const char*   ChurchIR1_wav;
    const int            ChurchIR1_wavSize = 248447;

    extern const char*   DenContainer48k_wav;
    const int            DenContainer48k_wavSize = 59282;

    extern const char*   DenHall48k_wav;
    const int            DenHall48k_wavSize = 202571;

    extern const char*   Farmhouse48k_wav;
    const int            Farmhouse48k_wavSize = 65825;

    extern const char*   Forest48k_wav;
    const int            Forest48k_wavSize = 110888;

    extern const char*   Friary48k_wav;
    const int            Friary48k_wavSize = 64370;

    extern const char*   Tent48k_wav;
    const int            Tent48k_wavSize = 59645;

    extern const char*   Tunnel48k_wav;
    const int            Tunnel48k_wavSize = 101078;

    extern const char*   Birds_LFO_wav;
    const int            Birds_LFO_wavSize = 1728044;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 13;

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
