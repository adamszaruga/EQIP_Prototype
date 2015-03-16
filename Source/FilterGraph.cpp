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

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginEditor.h"
#include "FilterGraph.h"
#include "internalFilters.h"


//==============================================================================
const int FilterGraph::midiChannelNumber = 0x1000;

FilterGraph::FilterGraph (AudioPluginFormatManager& formatManager_)
	: formatManager(formatManager_), lastUID(0)
{


}

FilterGraph::~FilterGraph()
{
    graph.clear();
}


uint32 FilterGraph::getNextUID() noexcept
{
    return ++lastUID;
}

//==============================================================================
int FilterGraph::getNumFilters() const noexcept
{
    return graph.getNumNodes();
}
void FilterGraph::addInitialFilters()
{
	InternalPluginFormat internalFormat;

	addFilter(internalFormat.getDescriptionFor(InternalPluginFormat::audioInputFilter), 0.5f, 0.1f);
	addFilter(internalFormat.getDescriptionFor(InternalPluginFormat::midiInputFilter), 0.25f, 0.1f);
	addFilter(internalFormat.getDescriptionFor(InternalPluginFormat::audioOutputFilter), 0.5f, 0.9f);
}

const AudioProcessorGraph::Node::Ptr FilterGraph::getNode (const int index) const noexcept
{
    return graph.getNode (index);
}

const AudioProcessorGraph::Node::Ptr FilterGraph::getNodeForId (const uint32 uid) const noexcept
{
    return graph.getNodeForId (uid);
}

void FilterGraph::addFilter (const PluginDescription* desc, double x, double y)
{
    if (desc != nullptr)
    {
        AudioProcessorGraph::Node* node = nullptr;

        String errorMessage;

		if (AudioPluginInstance* instance = formatManager.createPluginInstance(*desc, graph.getSampleRate(), graph.getBlockSize(), errorMessage))
		{
			node = graph.addNode(instance, getNextUID());
		}
			

        if (node != nullptr)
        {
            node->properties.set ("x", x);
            node->properties.set ("y", y);
        }
        else
        {
            AlertWindow::showMessageBox (AlertWindow::WarningIcon,
                                         TRANS("Couldn't create filter"),
                                         errorMessage);
        }
    }
}

void FilterGraph::trialFilter(const PluginDescription* desc, double x, double y)
{
	if (desc != nullptr)
	{

		if (lastUID != 3){
			removeFilter(lastUID);
		}
		AudioProcessorGraph::Node* node = nullptr;

		String errorMessage;

		if (AudioPluginInstance* instance = formatManager.createPluginInstance(*desc, graph.getSampleRate(), graph.getBlockSize(), errorMessage))
		{
			node = graph.addNode(instance, getNextUID());
		}
		if (node != nullptr)
		{
			node->properties.set("x", x);
			node->properties.set("y", y);
		}
		else
		{
			AlertWindow::showMessageBox(AlertWindow::WarningIcon,
				TRANS("Couldn't create filter"),
				errorMessage);
		}


		// Begin making connections here
		if (graph.canConnect(1, 0, node->nodeId, 0))    //device left channel to plugin in l cha,nel
		{	
			graph.addConnection(1, 0, node->nodeId, 0);  // TODO - change this to accept the incoming audio from the DAW
		}
		if (graph.canConnect(1, 1, node->nodeId, 1))    // device right channel to plugin in r channel
		{
			graph.addConnection(1, 1, node->nodeId, 1);
		}
		if (graph.canConnect(node->nodeId, 0, 3, 0))
		{
			graph.addConnection(node->nodeId, 0, 3, 0);
		}
		if (graph.canConnect(node->nodeId, 1, 3, 1))
		{
			graph.addConnection(node->nodeId, 1, 3, 1);
		}
	}

	
}

void FilterGraph::removeFilter (const uint32 id)
{
    PluginWindow::closeCurrentlyOpenWindowsFor (id);

	graph.removeNode(id);
        
}

void FilterGraph::disconnectFilter (const uint32 id)
{
	graph.disconnectNode(id);
      
}

void FilterGraph::removeIllegalConnections()
{
	graph.removeIllegalConnections();
    
}

void FilterGraph::setNodePosition (const int nodeId, double x, double y)
{
    const AudioProcessorGraph::Node::Ptr n (graph.getNodeForId (nodeId));

    if (n != nullptr)
    {
        n->properties.set ("x", jlimit (0.0, 1.0, x));
        n->properties.set ("y", jlimit (0.0, 1.0, y));
    }
}

void FilterGraph::getNodePosition (const int nodeId, double& x, double& y) const
{
    x = y = 0;

    const AudioProcessorGraph::Node::Ptr n (graph.getNodeForId (nodeId));

    if (n != nullptr)
    {
        x = (double) n->properties ["x"];
        y = (double) n->properties ["y"];
    }
}

//==============================================================================
int FilterGraph::getNumConnections() const noexcept
{
    return graph.getNumConnections();
}

const AudioProcessorGraph::Connection* FilterGraph::getConnection (const int index) const noexcept
{
    return graph.getConnection (index);
}

const AudioProcessorGraph::Connection* FilterGraph::getConnectionBetween (uint32 sourceFilterUID, int sourceFilterChannel,
                                                                          uint32 destFilterUID, int destFilterChannel) const noexcept
{
    return graph.getConnectionBetween (sourceFilterUID, sourceFilterChannel,
                                       destFilterUID, destFilterChannel);
}

bool FilterGraph::canConnect (uint32 sourceFilterUID, int sourceFilterChannel,
                              uint32 destFilterUID, int destFilterChannel) const noexcept
{
    return graph.canConnect (sourceFilterUID, sourceFilterChannel,
                             destFilterUID, destFilterChannel);
}

bool FilterGraph::addConnection (uint32 sourceFilterUID, int sourceFilterChannel,
                                 uint32 destFilterUID, int destFilterChannel)
{
    const bool result = graph.addConnection (sourceFilterUID, sourceFilterChannel,
                                             destFilterUID, destFilterChannel);


    return result;
}

void FilterGraph::removeConnection (const int index)
{
    graph.removeConnection (index);

}

void FilterGraph::removeConnection (uint32 sourceFilterUID, int sourceFilterChannel,
                                    uint32 destFilterUID, int destFilterChannel)
{
	graph.removeConnection(sourceFilterUID, sourceFilterChannel,
		destFilterUID, destFilterChannel);
}

void FilterGraph::clear()
{
    PluginWindow::closeAllCurrentlyOpenWindows();

    graph.clear();
}

//==============================================================================
//==============================================================================



