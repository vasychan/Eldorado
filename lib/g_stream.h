#ifndef G_SRTEAM_H
#define G_SRTEAM_H

#include "configure.h"

class g_stream
{
public:
    g_stream();
    ~g_stream();
    void record(const QString& filename, const int bitrate);
    void stop();
    void play();
    void SetUrl(const QString& uri);

public:
    bool is_playing;
private:
    GstElement *playbackBin, *recordBin, *fileSink, *lameEncoder;
    QString m_Uri;
};

#endif // G_STEAM_H
