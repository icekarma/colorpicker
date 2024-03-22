#pragma once

#if defined _DEBUG && defined TIMING

class Timing {

public:

    Timing( ) = delete;

    Timing( std::string const& strReportTag ):
        Timing { strReportTag, false }
    {
        /*empty*/
    }

    Timing( std::string const& strReportTag, bool const fStartNow ):
        m_strReportTag { strReportTag }
    {
        if ( fStartNow ) {
            Start( );
        }
    }

    void Start( ) {
        _GetSystemTime( m_uStartTime );
    }

    void Stop( ) {
        _GetSystemTime( m_uStopTime );
        m_uDelta = m_uStopTime - m_uStartTime;
        debug( "%s: elapsed time %.4f ms; rate %.2f Hz\n", m_strReportTag.c_str( ), m_uDelta / 10'000.0, 1.0 / ( m_uDelta / 10'000'000.0 ) );
    }

    uint64_t GetElapsedTime( ) const {
        return m_uDelta;
    }

private:

    uint64_t    m_uStartTime   { };
    uint64_t    m_uStopTime    { };
    uint64_t    m_uDelta       { };
    std::string m_strReportTag;

    void _GetSystemTime( uint64_t& uTime ) {
        GetSystemTimeAsFileTime( reinterpret_cast<LPFILETIME>( &uTime ) );
    }

};

#else // !defined _DEBUG || !defined TIMING

class Timing {

public:

    Timing( )                                = delete;
    Timing( std::string const& )             { /*empty*/ }
    Timing( std::string const&, bool const ) { /*empty*/ }

    void Start( ) {
        /*empty*/
    }

    void Stop( ) {
        /*empty*/
    }

    uint64_t GetElapsedTime( ) const {
        return 0;
    }

};

#endif // defined _DEBUG && defined TIMING
