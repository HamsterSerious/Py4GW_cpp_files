#pragma once

#include <GWCA/GameContainers/Array.h>
#include <GWCA/Utilities/Export.h>

namespace GW {
    struct PreGameContext;
    GWCA_API PreGameContext* GetPreGameContext();

    struct LoginCharacter {
        uint32_t unk0[13]; // Some kind of function call
        wchar_t character_name[20];
    };
    struct PreGameContext {
        /* +h0000 */ uint32_t frame_id;
        /* +h0004 */ uint32_t h0004[52];
        /* +h0124 */ uint32_t chosen_character_index;
        /* +h0144 */ uint32_t UNK01;
        /* +h0148 */ GW::Array<LoginCharacter> chars;
    };
}
