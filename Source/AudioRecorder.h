/*
  ==============================================================================

    AudioRecorder.h
    Created: 26 Feb 2024 8:19:28am
    Author:  Pichau

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class AudioRecorder {
    
public:
    AudioRecorder();
    ~AudioRecorder();

    void startRecording(const juce::File& file);

    void stop();

    bool isRecording();

    /** Source calls it so the sample rate of the writerand the source are synchronized */ 
    void audioDeviceAboutToStart(int sourceSampleRate);

    /** It's called when the source stops */ 
    void audioDeviceStopped();

    /** It's called by the source when it has more data to input
        The data from the source is send in the inputChannetData variable
        The other two variables can be used for other purposes than recording data as shown in the AudioRecordingDemo example */
    void audioRecorderCallback(const float* const* inputChannelData, int numInputChannels, int numSamples);

private:

    /** This thread will be used to write the file in the background */
    juce::TimeSliceThread writeThread{ "Audio Recorder Thread" };

    /** Acts like a queue of buffer for the incoming data */
    std::unique_ptr<juce::AudioFormatWriter::ThreadedWriter> bufferData;    

    double sampleRate = 0.0;
    juce::int64 nextSampleNum = 0;

    /** CriticalSection is used to make sure the program is thread safe as explained in Juce documentation */
    juce::CriticalSection writerLock;
    std::atomic<juce::AudioFormatWriter::ThreadedWriter*> activeWriter{ nullptr };
};
