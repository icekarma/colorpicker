#pragma once

class ChannelInformation {

public:

    ChannelInformation( ) = delete;

    ChannelInformation( AllChannels const channel, int const minimum, int const maximum ):
        m_channel      { channel },
        m_minimumValue { minimum },
        m_maximumValue { maximum }
    {
        /*empty*/
    }

    AllChannels const m_channel;
    int         const m_minimumValue;
    int         const m_maximumValue;

};

extern ChannelInformation const AllChannelsInformation[6];
