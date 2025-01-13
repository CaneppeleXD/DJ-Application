/*
  ==============================================================================

    AudioRecorder.cpp
    Created: 26 Feb 2024 8:19:28am
    Author:  Pichau

  ==============================================================================
*/

#include "AudioRecorder.h"

AudioRecorder::AudioRecorder() {
    //starts the thread for further use

    writeThread.startThread();
}

AudioRecorder::~AudioRecorder() {
    stop();
}

void AudioRecorder::startRecording(const juce::File& file) {
    stop();

    if (sampleRate > 0) {
        
        // It's not possible to create a file if it already exists
        file.deleteFile();

        //create a kind of path for the data to be sent to the file
        if (auto fileStream = std::unique_ptr<juce::FileOutputStream> (file.createOutputStream())) {

            juce::WavAudioFormat wavFormat;

            //uses the "path" created previously to create a write which will send data to the file
            if (auto writer = wavFormat.createWriterFor(fileStream.get(), sampleRate, 1, 16, {}, 0)) {
                fileStream.release();
                    
                //creates the object which will buffer the data and then write it to disk using the writeThread in the background
                bufferData.reset(new juce::AudioFormatWriter::ThreadedWriter (writer, writeThread, 32768));

                nextSampleNum = 0;

                const juce::ScopedLock sl(writerLock);
                //sets that out active writer now is the object we just created
                activeWriter = bufferData.get();
            }
        }
    }
}

void AudioRecorder::stop() {
    {
        const juce::ScopedLock sl(writerLock);
        activeWriter = nullptr;
    }

    // The buffer data is finished after the activeWriter because in Juce documentation is says
    // that the buffer may take a time until is completely finished so it is better to stop the 
    // active writer right away and stop the data active data flow
    bufferData.reset();
}

bool AudioRecorder::isRecording() {
    return activeWriter.load() != nullptr;
}

void AudioRecorder::audioDeviceAboutToStart(int sourceSampleRate) {
    sampleRate = sourceSampleRate;
}

void AudioRecorder::audioDeviceStopped() {
    sampleRate = 0;
}

void AudioRecorder::audioRecorderCallback(const float* const* inputChannelData, int numInputChannels, int numSamples) 
{
    const juce::ScopedLock sl(writerLock);

    // checks if it is supposed to record something
    if (activeWriter.load() != nullptr) {
        // write the input data to a file
        activeWriter.load()->write(inputChannelData, numSamples);
        
        nextSampleNum += numSamples;
    }
}

