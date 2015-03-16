/*
  ==============================================================================

    internalFilters.cpp
    Created: 6 Mar 2015 12:15:42am
    Author:  Adam

  ==============================================================================
*/


#include "../JuceLibraryCode/JuceHeader.h"
#include "internalFilters.h"



//==============================================================================
InternalPluginFormat::InternalPluginFormat()
{
    {
        AudioProcessorGraph::AudioGraphIOProcessor p (AudioProcessorGraph::AudioGraphIOProcessor::audioOutputNode);
        p.fillInPluginDescription (audioOutDesc);
    }

    {
        AudioProcessorGraph::AudioGraphIOProcessor p (AudioProcessorGraph::AudioGraphIOProcessor::audioInputNode);
        p.fillInPluginDescription (audioInDesc);
    }

    {
        AudioProcessorGraph::AudioGraphIOProcessor p (AudioProcessorGraph::AudioGraphIOProcessor::midiInputNode);
        p.fillInPluginDescription (midiInDesc);
    }
}

AudioPluginInstance* InternalPluginFormat::createInstanceFromDescription (const PluginDescription& desc,
                                                                          double /*sampleRate*/, int /*blockSize*/)
{
    if (desc.name == audioOutDesc.name)
        return new AudioProcessorGraph::AudioGraphIOProcessor (AudioProcessorGraph::AudioGraphIOProcessor::audioOutputNode);

    if (desc.name == audioInDesc.name)
        return new AudioProcessorGraph::AudioGraphIOProcessor (AudioProcessorGraph::AudioGraphIOProcessor::audioInputNode);

    if (desc.name == midiInDesc.name)
        return new AudioProcessorGraph::AudioGraphIOProcessor (AudioProcessorGraph::AudioGraphIOProcessor::midiInputNode);

    return 0;
}

const PluginDescription* InternalPluginFormat::getDescriptionFor (const InternalFilterType type)
{
    switch (type)
    {
        case audioInputFilter:      return &audioInDesc;
        case audioOutputFilter:     return &audioOutDesc;
        case midiInputFilter:       return &midiInDesc;
        default:                    break;
    }

    return 0;
}

void InternalPluginFormat::getAllTypes (OwnedArray <PluginDescription>& results)
{
    for (int i = 0; i < (int) endOfFilterTypes; ++i)
        results.add (new PluginDescription (*getDescriptionFor ((InternalFilterType) i)));
}
