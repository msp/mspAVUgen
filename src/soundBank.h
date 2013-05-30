//
//  soundBank.h
//  mspUgen
//
//  Created by msp on 30/05/2013.
//
//

#ifndef mspUgen_soundBank_h
#define mspUgen_soundBank_h

#include "ofMain.h"
#include "avUgen.h"

namespace msp {
    class soundBank{
        int currentPreset = 0;
        std::vector<string> avUgenNames;

    public:
        bool audioReady = true;
        ~soundBank();

        // Constants
        static const int TOTAL_SLOTS = 3;

        // Constructors
        soundBank();
        soundBank(std::vector<string> _avUgenNames);

        // Collections
        std::vector<std::vector <msp::avUgen*> > presetSlots;
        std::vector<msp::avUgen*> activeSlots;

        void cyclePreset();
    };
}

#endif
