#include "g_stream.h"

g_stream::g_stream()
:is_playing(true)
{
    gst_init (NULL,NULL);
    // Record pipeline
    // autoaudiosrc ! audioconvert ! lame bitrate=192 ! filesink location=./ciao.mp3
    recordBin = gst_pipeline_new("record-pipeline");
    g_assert(recordBin);
    audioSrc = gst_element_factory_make("autoaudiosrc", "audio_in");
    g_assert(audioSrc);
    audioConvert = gst_element_factory_make("audioconvert", "audio_converter");
    g_assert(audioConvert);
    lameEncoder = gst_element_factory_make("lame", "lame_encoder");
    g_assert(audioConvert);
    fileSink = gst_element_factory_make("filesink", "file_sink");
    g_assert(fileSink);
    gst_bin_add_many(GST_BIN(recordBin), audioSrc, audioConvert, lameEncoder, fileSink, NULL);
    gst_element_link_many(audioSrc, audioConvert, lameEncoder, fileSink, NULL);
    // Playback
    playbackBin = gst_element_factory_make("playbin2", "playbin");
    g_assert(playbackBin);
}
g_stream::~g_stream()
{
    qDebug() << "DESTRUCTION";
    gst_element_set_state(GST_ELEMENT(playbackBin), GST_STATE_NULL);
    gst_object_unref(GST_OBJECT(playbackBin));
    gst_element_set_state(GST_ELEMENT(recordBin), GST_STATE_NULL);
    gst_object_unref(GST_OBJECT(recordBin));
}

void g_stream::record(const QString& filename, const int bitrate)
{
    qDebug() << "RECORD";
    gst_element_set_state(GST_ELEMENT(playbackBin), GST_STATE_READY);
    gst_element_set_state(GST_ELEMENT(recordBin), GST_STATE_NULL);
    g_object_set(G_OBJECT(fileSink), "location", qPrintable(filename), NULL);
    g_object_set(G_OBJECT(lameEncoder), "bitrate", bitrate, NULL);
    gst_element_set_state(GST_ELEMENT(recordBin), GST_STATE_PLAYING);
}

void g_stream::stop()
{
    qDebug() << "STOP";
    gst_element_set_state(GST_ELEMENT(playbackBin), GST_STATE_READY);
    gst_element_set_state(GST_ELEMENT(recordBin), GST_STATE_READY);
    this->is_playing = false;
}

void g_stream::play()
{
        qDebug() << "PLAY";
        g_object_set(G_OBJECT(playbackBin), "uri", qPrintable(m_Uri), NULL);
        gst_element_set_state(GST_ELEMENT(recordBin), GST_STATE_READY);
        gst_element_set_state(GST_ELEMENT(playbackBin), GST_STATE_PLAYING);
        this->is_playing = true;
}

void g_stream::replay()
{
    GstStateChangeReturn ret;
    GstState state = GST_STATE_PLAYING;
    ret = gst_element_get_state(GST_ELEMENT(playbackBin),NULL, NULL, NULL);
    qDebug() << gst_element_get_state (playbackBin, NULL, NULL, -1);
    qDebug() << gst_element_get_state (audioSrc, NULL, NULL, -1);
    qDebug() << gst_element_get_state (audioConvert, NULL, NULL, -1);
    qDebug() << gst_element_get_state (lameEncoder, NULL, NULL, -1);
    qDebug() << gst_element_get_state (fileSink, NULL, NULL, -1);
    //qDebug() << ret;
    //if( ret != GST_STATE_CHANGE_SUCCESS)
    if (gst_element_get_state (playbackBin, NULL, NULL, -1) == GST_STATE_CHANGE_FAILURE)
    {
    qDebug() << "RESTART!!!";
    stop();
    play();
    }
}

void g_stream::SetUrl(const QString& uri)
{
    m_Uri = uri;
}
