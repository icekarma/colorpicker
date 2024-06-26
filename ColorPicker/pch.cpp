#include "pch.h"

#include "ChannelInformation.h"
#include "Settings.h"

//================================================
// Global constants
//================================================

constinit ChannelInformation const AllChannelsInformation[6] {
    { AllChannels::LabL,     0, 100 },
    { AllChannels::LabA,  -128, 127 },
    { AllChannels::LabB,  -128, 127 },
    { AllChannels::SrgbR,    0, 255 },
    { AllChannels::SrgbG,    0, 255 },
    { AllChannels::SrgbB,    0, 255 }
};

//================================================
// Global variables
//================================================

TransformsManager g_Transforms;

CSettings*        g_pSettings { };
