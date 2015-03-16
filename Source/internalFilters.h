/*
  ==============================================================================

    internalFilters.h
    Created: 6 Mar 2015 12:15:19am
    Author:  Adam

  ==============================================================================
*/

#ifndef INTERNALFILTERS_H_INCLUDED
#define INTERNALFILTERS_H_INCLUDED


//==============================================================================
/**
    Manages the internal plugin types.
*/
class InternalPluginFormat   : public AudioPluginFormat
{
public:
    //==============================================================================
    InternalPluginFormat();
    ~InternalPluginFormat() {}

    //==============================================================================
    enum InternalFilterType
    {
        audioInputFilter = 0,
        audioOutputFilter,
        midiInputFilter,

        endOfFilterTypes
    };

    const PluginDescription* getDescriptionFor (const InternalFilterType type);

    void getAllTypes (OwnedArray <PluginDescription>& results);

    //==============================================================================
    String getName() const override                                      { return "Internal"; }
    bool fileMightContainThisPluginType (const String&) override         { return false; }
    FileSearchPath getDefaultLocationsToSearch() override                { return FileSearchPath(); }
    bool canScanForPlugins() const override                              { return false; }
    void findAllTypesForFile (OwnedArray <PluginDescription>&, const String&) override     {}
    bool doesPluginStillExist (const PluginDescription&) override        { return true; }
    String getNameOfPluginFromIdentifier (const String& fileOrIdentifier) override   { return fileOrIdentifier; }
    bool pluginNeedsRescanning (const PluginDescription&) override       { return false; }
    StringArray searchPathsForPlugins (const FileSearchPath&, bool) override         { return StringArray(); }
    AudioPluginInstance* createInstanceFromDescription (const PluginDescription&, double, int) override;

private:
    //==============================================================================
    PluginDescription audioInDesc;
    PluginDescription audioOutDesc;
    PluginDescription midiInDesc;
};


#endif  // INTERNALFILTERS_H_INCLUDED
