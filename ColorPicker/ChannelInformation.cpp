#include "pch.h"

#include "ChannelInformation.h"

std::unordered_map<AllChannels, ChannelInformation> const AllChannelsInformation {
    { AllChannels::LabL,  { AllChannels::LabL,     0, 100 } },
    { AllChannels::LabA,  { AllChannels::LabA,  -128, 127 } },
    { AllChannels::LabB,  { AllChannels::LabB,  -128, 127 } },
    { AllChannels::SrgbR, { AllChannels::SrgbR,    0, 255 } },
    { AllChannels::SrgbG, { AllChannels::SrgbG,    0, 255 } },
    { AllChannels::SrgbB, { AllChannels::SrgbB,    0, 255 } }
};
