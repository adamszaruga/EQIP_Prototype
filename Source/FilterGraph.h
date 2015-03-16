/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2013 - Raw Material Software Ltd.

   Permission is granted to use this software under the terms of either:
   a) the GPL v2 (or any later version)
   b) the Affero GPL v3

   Details of these licenses can be found at: www.gnu.org/licenses

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

   ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.juce.com for more information.

  ==============================================================================
*/

#ifndef __FILTERGRAPH_JUCEHEADER__
#define __FILTERGRAPH_JUCEHEADER__

class FilterGraph;

const char* const filenameSuffix = ".filtergraph";
const char* const filenameWildcard = "*.filtergraph";

//==============================================================================
/**
    A collection of filters and some connections between them.
*/
class FilterGraph 
{
public:
    //==============================================================================
    FilterGraph (AudioPluginFormatManager& formatManager);
    ~FilterGraph();

    //==============================================================================
    AudioProcessorGraph& getGraph() noexcept         { return graph; }

    int getNumFilters() const noexcept;
    const AudioProcessorGraph::Node::Ptr getNode (const int index) const noexcept;
    const AudioProcessorGraph::Node::Ptr getNodeForId (const uint32 uid) const noexcept;

    void addFilter (const PluginDescription* desc, double x, double y);
	void addInitialFilters();
	void trialFilter(const PluginDescription* desc, double x, double y);

    void removeFilter (const uint32 filterUID);
    void disconnectFilter (const uint32 filterUID);

    void removeIllegalConnections();

    void setNodePosition (const int nodeId, double x, double y);
    void getNodePosition (const int nodeId, double& x, double& y) const;

    //==============================================================================
    int getNumConnections() const noexcept;
    const AudioProcessorGraph::Connection* getConnection (const int index) const noexcept;

    const AudioProcessorGraph::Connection* getConnectionBetween (uint32 sourceFilterUID, int sourceFilterChannel,
                                                                 uint32 destFilterUID, int destFilterChannel) const noexcept;

    bool canConnect (uint32 sourceFilterUID, int sourceFilterChannel,
                     uint32 destFilterUID, int destFilterChannel) const noexcept;

    bool addConnection (uint32 sourceFilterUID, int sourceFilterChannel,
                        uint32 destFilterUID, int destFilterChannel);

    void removeConnection (const int index);

    void removeConnection (uint32 sourceFilterUID, int sourceFilterChannel,
                           uint32 destFilterUID, int destFilterChannel);

    void clear();



    /** The special channel index used to refer to a filter's midi channel.
    */
    static const int midiChannelNumber;

	// The below two things came with the default Juce Host, no idea what they do.
	uint32 lastUID;
	uint32 getNextUID() noexcept;


private:
    //==============================================================================
    AudioPluginFormatManager& formatManager;
    AudioProcessorGraph graph;

	

	

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterGraph)
};


#endif   // __FILTERGRAPH_JUCEHEADER__
