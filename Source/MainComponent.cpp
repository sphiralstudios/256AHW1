// Music 256a / CS 476a | fall 2016
// CCRMA, Stanford University
//
// Author: Romain Michon (rmichonATccrmaDOTstanfordDOTedu)
// Description: Simple JUCE sine wave synthesizer

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Sine.h"
#include <algorithm>

class MainContentComponent :
    public AudioAppComponent,
    private Slider::Listener,
    private ToggleButton::Listener,
    private ComboBox::Listener,
    private MidiInputCallback,
    private MidiKeyboardStateListener
{
public:
    //from MIDI Input tutorial
    MainContentComponent() : gain (0.0), onOff (0), samplingRate(0.0), lastInputIndex (0),
    isAddingFromMidiInput (false),
    keyboardComponent (keyboardState, MidiKeyboardComponent::horizontalKeyboard),
    startTime (Time::getMillisecondCounterHiRes() * 0.001)
    {
        setOpaque (true);
        
        addAndMakeVisible (midiInputListLabel);
        midiInputListLabel.setText ("MIDI Input:", dontSendNotification);
        midiInputListLabel.attachToComponent (&midiInputList, true);
        
        addAndMakeVisible (midiInputList);
        midiInputList.setTextWhenNoChoicesAvailable ("No MIDI Inputs Enabled");
        const StringArray midiInputs (MidiInput::getDevices());
        midiInputList.addItemList (midiInputs, 1);
        midiInputList.addListener (this);
        
        // find the first enabled device and use that by default
        for (int i = 0; i < midiInputs.size(); ++i)
        {
            if (deviceManager.isMidiInputEnabled (midiInputs[i]))
            {
                setMidiInput (i);
                break;
            }
        }
        
        // if no enabled devices were found just use the first one in the list
        if (midiInputList.getSelectedId() == 0)
            setMidiInput (0);
        
        //MIDI keyboard GUI and associated keyboard state
        addAndMakeVisible (keyboardComponent);
        keyboardState.addListener (this);
        
        // configuring frequency slider and adding it to the main window
//        addAndMakeVisible (frequencySlider);
//        frequencySlider.setRange (50.0, 5000.0);
//        frequencySlider.setSkewFactorFromMidPoint (500.0);
//        frequencySlider.setValue(1000); // will also set the default frequency of the sine osc
//        frequencySlider.addListener (this);
        
        // configuring frequency label box and adding it to the main window
//        addAndMakeVisible(frequencyLabel);
//        frequencyLabel.setText ("Frequency", dontSendNotification);
//        frequencyLabel.attachToComponent (&frequencySlider, true);
        
        // configuring gain slider and adding it to the main window
        addAndMakeVisible (gainSlider);
        gainSlider.setRange (0.0, 1.0);
        gainSlider.setValue(0.5); // will alsi set the default gain of the sine osc
        gainSlider.addListener (this);
        
        
        // configuring gain label and adding it to the main window
        addAndMakeVisible(gainLabel);
        gainLabel.setText ("Gain", dontSendNotification);
        gainLabel.attachToComponent (&gainSlider, true);
        
        // configuring interpolation slider and adding it to the main window
        addAndMakeVisible (interpolateSlider);
        interpolateSlider.setRange (0.0, 1.0);
        interpolateSlider.setValue(0.0); // will alsi set the default gain of the sine osc
        interpolateSlider.addListener (this);
        
        
        // configuring interpolate label and adding it to the main window
        addAndMakeVisible(interpolateLabel);
        interpolateLabel.setText ("Interpolate", dontSendNotification);
        interpolateLabel.attachToComponent (&interpolateSlider, true);
        
        
        // configuring on/off button and adding it to the main window
//        addAndMakeVisible(onOffButton);
//        onOffButton.addListener(this);
        
        // configuring on/off label and adding it to the main window
//        addAndMakeVisible(onOffLabel);
//        onOffLabel.setText ("On/Off", dontSendNotification);
//        onOffLabel.attachToComponent (&onOffButton, true);
        
        //group "A" bank of instruments
        addAndMakeVisible(sineButtonA);
        sineButtonA.setRadioGroupId(instrumentGroupAID);
        sineButtonA.addListener(this);
        
        addAndMakeVisible(sineLabelA);
        sineLabelA.setText ("Sine", dontSendNotification);
        sineLabelA.attachToComponent (&sineButtonA, true);
        
        addAndMakeVisible(fluteButtonA);
        fluteButtonA.setRadioGroupId(instrumentGroupAID);
        fluteButtonA.addListener(this);
        
        addAndMakeVisible(fluteLabelA);
        fluteLabelA.setText ("Flute", dontSendNotification);
        fluteLabelA.attachToComponent (&fluteButtonA, true);
        
        addAndMakeVisible(violinButtonA);
        violinButtonA.setRadioGroupId(instrumentGroupAID);
        violinButtonA.addListener(this);
        
        addAndMakeVisible(violinLabelA);
        violinLabelA.setText ("Violin", dontSendNotification);
        violinLabelA.attachToComponent (&violinButtonA, true);
        
        addAndMakeVisible(oboeButtonA);
        oboeButtonA.setRadioGroupId(instrumentGroupAID);
        oboeButtonA.addListener(this);
        
        addAndMakeVisible(oboeLabelA);
        oboeLabelA.setText ("Oboe", dontSendNotification);
        oboeLabelA.attachToComponent (&oboeButtonA, true);
        
        addAndMakeVisible(clarinetButtonA);
        clarinetButtonA.setRadioGroupId(instrumentGroupAID);
        clarinetButtonA.addListener(this);
        
        addAndMakeVisible(clarinetLabelA);
        clarinetLabelA.setText ("Clarinet", dontSendNotification);
        clarinetLabelA.attachToComponent (&clarinetButtonA, true);
        
        addAndMakeVisible(hornButtonA);
        hornButtonA.setRadioGroupId(instrumentGroupAID);
        hornButtonA.addListener(this);
        
        addAndMakeVisible(hornLabelA);
        hornLabelA.setText ("Horn", dontSendNotification);
        hornLabelA.attachToComponent (&hornButtonA, true);
        
        addAndMakeVisible(tromboneButtonA);
        tromboneButtonA.setRadioGroupId(instrumentGroupAID);
        tromboneButtonA.addListener(this);
        
        addAndMakeVisible(tromboneLabelA);
        tromboneLabelA.setText ("Trombone", dontSendNotification);
        tromboneLabelA.attachToComponent (&tromboneButtonA, true);
        
        // "B" group bank of instruments
        addAndMakeVisible(sineButtonB);
        sineButtonB.setRadioGroupId(instrumentGroupBID);
        sineButtonB.addListener(this);
        
        addAndMakeVisible(sineLabelB);
        sineLabelB.setText ("Sine", dontSendNotification);
        sineLabelB.attachToComponent (&sineButtonB, true);
        
        addAndMakeVisible(fluteButtonB);
        fluteButtonB.setRadioGroupId(instrumentGroupBID);
        fluteButtonB.addListener(this);
        
        addAndMakeVisible(fluteLabelB);
        fluteLabelB.setText ("Flute", dontSendNotification);
        fluteLabelB.attachToComponent (&fluteButtonB, true);
        
        addAndMakeVisible(violinButtonB);
        violinButtonB.setRadioGroupId(instrumentGroupBID);
        violinButtonB.addListener(this);
        
        addAndMakeVisible(violinLabelB);
        violinLabelB.setText ("Violin", dontSendNotification);
        violinLabelB.attachToComponent (&violinButtonB, true);
        
        addAndMakeVisible(oboeButtonB);
        oboeButtonB.setRadioGroupId(instrumentGroupBID);
        oboeButtonB.addListener(this);
        
        addAndMakeVisible(oboeLabelB);
        oboeLabelB.setText ("Oboe", dontSendNotification);
        oboeLabelB.attachToComponent (&oboeButtonB, true);
        
        addAndMakeVisible(clarinetButtonB);
        clarinetButtonB.setRadioGroupId(instrumentGroupBID);
        clarinetButtonB.addListener(this);
        
        addAndMakeVisible(clarinetLabelB);
        clarinetLabelB.setText ("Clarinet", dontSendNotification);
        clarinetLabelB.attachToComponent (&clarinetButtonB, true);
        
        addAndMakeVisible(hornButtonB);
        hornButtonB.setRadioGroupId(instrumentGroupBID);
        hornButtonB.addListener(this);
        
        addAndMakeVisible(hornLabelB);
        hornLabelB.setText ("Horn", dontSendNotification);
        hornLabelB.attachToComponent (&hornButtonB, true);
        
        addAndMakeVisible(tromboneButtonB);
        tromboneButtonB.setRadioGroupId(instrumentGroupBID);
        tromboneButtonB.addListener(this);
        
        addAndMakeVisible(tromboneLabelB);
        tromboneLabelB.setText ("Trombone", dontSendNotification);
        tromboneLabelB.attachToComponent (&tromboneButtonB, true);
        
        
        
        setSize (600, 250);
        nChans = 2;
        setAudioChannels (0, nChans); // no inputs, one output
        
  //      buffer = new float(2);
    }
    
    ~MainContentComponent()
    {
        shutdownAudio();
        keyboardState.removeListener (this);
        deviceManager.removeMidiInputCallback (MidiInput::getDevices()[midiInputList.getSelectedItemIndex()], this);
        midiInputList.removeListener (this);
 //       delete [] buffer;
        
    }
    
    void paint (Graphics& g) override
    {
        g.fillAll (Colours::grey);
    }
    
    void resized() override
    {
        // placing the UI elements in the main window
        // getWidth has to be used in case the window is resized by the user
        const int sliderLeft = 80;
 //       frequencySlider.setBounds (sliderLeft, 110, getWidth() - sliderLeft - 20, 20);
        gainSlider.setBounds (sliderLeft, 120, getWidth() - sliderLeft - 20, 20);
        interpolateSlider.setBounds (sliderLeft, 180, getWidth() - sliderLeft - 20, 20);
//        onOffButton.setBounds (sliderLeft, 170, getWidth() - sliderLeft - 20, 20);
        sineButtonA.setBounds (sliderLeft, 150, getWidth() - sliderLeft - 20, 20);
        fluteButtonA.setBounds (sliderLeft + 70, 150, getWidth() - sliderLeft - 20, 20);
        violinButtonA.setBounds (sliderLeft + 150, 150, getWidth() - sliderLeft - 20, 20);
        oboeButtonA.setBounds (sliderLeft + 220, 150, getWidth() - sliderLeft - 20, 20);
        clarinetButtonA.setBounds(sliderLeft + 310, 150, getWidth() - sliderLeft - 20, 20);
        hornButtonA.setBounds(sliderLeft + 380, 150, getWidth() - sliderLeft - 20, 20);
        tromboneButtonA.setBounds(sliderLeft + 480, 150, getWidth() - sliderLeft - 20, 20);
        sineButtonB.setBounds (sliderLeft, 210, getWidth() - sliderLeft - 20, 20);
        fluteButtonB.setBounds (sliderLeft + 70, 210, getWidth() - sliderLeft - 20, 20);
        violinButtonB.setBounds (sliderLeft + 150, 210, getWidth() - sliderLeft - 20, 20);
        oboeButtonB.setBounds (sliderLeft + 220, 210, getWidth() - sliderLeft - 20, 20);
        clarinetButtonB.setBounds(sliderLeft + 310, 210, getWidth() - sliderLeft - 20, 20);
        hornButtonB.setBounds(sliderLeft + 380, 210, getWidth() - sliderLeft - 20, 20);
        tromboneButtonB.setBounds(sliderLeft + 480, 210, getWidth() - sliderLeft - 20, 20);
        Rectangle<int> area (getLocalBounds());
        midiInputList.setBounds (area.removeFromTop (36).removeFromRight (getWidth() - 150).reduced (8));
        keyboardComponent.setBounds (area.removeFromTop (80).reduced(8));
//        midiMessagesBox.setBounds (area.reduced (8));
    }
    
    void sliderValueChanged (Slider* slider) override
    {
        if (samplingRate > 0.0){
//            if (slider == &interpolateSlider){
//                interp = interpolateSlider.getValue();
////                sines[0].setFrequency(fundamental);
//                for(int osc = 0; osc < (sizeof(sines)/sizeof(*sines)); ++osc) {
//                    sines[osc].setFrequency(fundamental * (osc + 1));
//                }
//            }
//            else
                if (slider == &gainSlider){
                gain = gainSlider.getValue();
            }
        }
    }
    
    void buttonClicked (Button* button) override
    {
        //handle bank A instrument change
        if(fluteButtonA.getToggleState()) {
            //set partialStrengths to Flute values
            int newHarmonicStrengths [] ={66,24,4,6,0,0,0,0,0,0,0,0};
            std::copy(newHarmonicStrengths, newHarmonicStrengths + 12, harmonicStrengthsA);
        } else if(violinButtonA.getToggleState()) {
            //set partialStrenghts to violin values
            int newHarmonicStrengths [] ={26,25,9,10,27,1,0,2,0,0,0,0};
            std::copy(newHarmonicStrengths, newHarmonicStrengths + 12, harmonicStrengthsA);
        } else if(oboeButtonA.getToggleState()) {
            //set partialStrenghts to oboe values
            int newHarmonicStrengths [] ={2,2,4,29,35,14,4,2,3,4,1,0};
            std::copy(newHarmonicStrengths, newHarmonicStrengths + 12, harmonicStrengthsA);
        } else if(clarinetButtonA.getToggleState()) {
            //set partialStrenghts to clarinet values
            int newHarmonicStrengths [] ={12,0,10,3,5,0,8,18,15,18,5,6};
            std::copy(newHarmonicStrengths, newHarmonicStrengths + 12, harmonicStrengthsA);
        } else if(hornButtonA.getToggleState()) {
            //set partialStrenghts to horn values
            int newHarmonicStrengths [] ={36,26,17,7,4,3,2,1,1,1,1,1};
            std::copy(newHarmonicStrengths, newHarmonicStrengths + 12, harmonicStrengthsA);
        } else if(tromboneButtonA.getToggleState()) {
            //set partialStrenghts to trombone values
            int newHarmonicStrengths [] ={6,11,35,12,8,11,6,4,3,2,1,1};
            std::copy(newHarmonicStrengths, newHarmonicStrengths + 12, harmonicStrengthsA);
        } else {
            //set pure sine tone default
            int newHarmonicStrengths [] ={100,0,0,0,0,0,0,0,0,0,0,0};
            std::copy(newHarmonicStrengths, newHarmonicStrengths + 12, harmonicStrengthsA);
        }

        //handle bank B instrument change
        if(fluteButtonB.getToggleState()) {
            //set partialStrengths to Flute values
            int newHarmonicStrengths [] ={66,24,4,6,0,0,0,0,0,0,0,0};
            std::copy(newHarmonicStrengths, newHarmonicStrengths + 12, harmonicStrengthsB);
        } else if(violinButtonB.getToggleState()) {
            //set partialStrenghts to violin values
            int newHarmonicStrengths [] ={26,25,9,10,27,1,0,2,0,0,0,0};
            std::copy(newHarmonicStrengths, newHarmonicStrengths + 12, harmonicStrengthsB);
        } else if(oboeButtonB.getToggleState()) {
            //set partialStrenghts to oboe values
            int newHarmonicStrengths [] ={2,2,4,29,35,14,4,2,3,4,1,0};
            std::copy(newHarmonicStrengths, newHarmonicStrengths + 12, harmonicStrengthsB);
        } else if(clarinetButtonB.getToggleState()) {
            //set partialStrenghts to clarinet values
            int newHarmonicStrengths [] ={12,0,10,3,5,0,8,18,15,18,5,6};
            std::copy(newHarmonicStrengths, newHarmonicStrengths + 12, harmonicStrengthsB);
        } else if(hornButtonB.getToggleState()) {
            //set partialStrenghts to horn values
            int newHarmonicStrengths [] ={36,26,17,7,4,3,2,1,1,1,1,1};
            std::copy(newHarmonicStrengths, newHarmonicStrengths + 12, harmonicStrengthsB);
        } else if(tromboneButtonB.getToggleState()) {
            //set partialStrenghts to trombone values
            int newHarmonicStrengths [] ={6,11,35,12,8,11,6,4,3,2,1,1};
            std::copy(newHarmonicStrengths, newHarmonicStrengths + 12, harmonicStrengthsB);
        } else {
            //set pure sine tone default
            int newHarmonicStrengths [] ={100,0,0,0,0,0,0,0,0,0,0,0};
            std::copy(newHarmonicStrengths, newHarmonicStrengths + 12, harmonicStrengthsB);
        }
    }
    
    void prepareToPlay (int /*samplesPerBlockExpected*/, double sampleRate) override
    {
        samplingRate = sampleRate;
        for(int osc = 0; osc < (sizeof(sines)/sizeof(*sines)); ++osc) {
            sines[osc].setSamplingRate(sampleRate);
        }
        //initialize harmonic strengths to pure sine
        int newHarmonicStrengthsA [] ={100,0,0,0,0,0,0,0,0,0,0,0};
        std::copy(newHarmonicStrengthsA, newHarmonicStrengthsA + 12, harmonicStrengthsA);
        int newHarmonicStrengthsB [] ={100,0,0,0,0,0,0,0,0,0,0,0};
        std::copy(newHarmonicStrengthsB, newHarmonicStrengthsB + 12, harmonicStrengthsB);

        
    }
    
    void releaseResources() override
    {
    }
    
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
 //       for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel) {
        // getting the audio output buffer to be filled
            float* const buffer = bufferToFill.buffer->getWritePointer (0, bufferToFill.startSample);
            float* const buffer1 = bufferToFill.buffer->getWritePointer (1, bufferToFill.startSample);
        
        
        // computing one block
            for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
            {
                buffer[sample] = 0.0;
                buffer1[sample] = 0.0;
                //iterate through oscillators, weighted based on interpolated harmonic strengths
                for(int osc = 0; osc < (sizeof(sines)/sizeof(*sines)); ++osc) {
                    buffer[sample] += sines[osc].tick() * gain * ((harmonicStrengthsA[osc] * (1-interpolateSlider.getValue())) + (harmonicStrengthsB[osc] * interpolateSlider.getValue())) * .01;
                    buffer1[sample] += sines[osc].tick() * gain * ((harmonicStrengthsA[osc] * (1-interpolateSlider.getValue())) + (harmonicStrengthsB[osc] * interpolateSlider.getValue())) * .01;
                }
            }
 //       }
    }
    
    
private:
 //   float** buffer;
    
    static String getMidiMessageDescription (const MidiMessage& m)
    {
        if (m.isNoteOn())           return "Note on "  + MidiMessage::getMidiNoteName (m.getNoteNumber(), true, true, 3);
        if (m.isNoteOff())          return "Note off " + MidiMessage::getMidiNoteName (m.getNoteNumber(), true, true, 3);
        if (m.isProgramChange())    return "Program change " + String (m.getProgramChangeNumber());
        if (m.isPitchWheel())       return "Pitch wheel " + String (m.getPitchWheelValue());
        if (m.isAftertouch())       return "After touch " + MidiMessage::getMidiNoteName (m.getNoteNumber(), true, true, 3) +  ": " + String (m.getAfterTouchValue());
        if (m.isChannelPressure())  return "Channel pressure " + String (m.getChannelPressureValue());
        if (m.isAllNotesOff())      return "All notes off";
        if (m.isAllSoundOff())      return "All sound off";
        if (m.isMetaEvent())        return "Meta event";
        
        if (m.isController())
        {
            String name (MidiMessage::getControllerName (m.getControllerNumber()));
            
            if (name.isEmpty())
                name = "[" + String (m.getControllerNumber()) + "]";
            
            return "Controller " + name + ": " + String (m.getControllerValue());
        }
        
        return String::toHexString (m.getRawData(), m.getRawDataSize());
    }
    
    void logMessage (const String& m)
    {
//        midiMessagesBox.moveCaretToEnd();
//        midiMessagesBox.insertTextAtCaret (m + newLine);
    }
    
    /** Starts listening to a MIDI input device, enabling it if necessary. */
    void setMidiInput (int index)
    {
        const StringArray list (MidiInput::getDevices());
        
        deviceManager.removeMidiInputCallback (list[lastInputIndex], this);
        
        const String newInput (list[index]);
        
        if (! deviceManager.isMidiInputEnabled (newInput))
            deviceManager.setMidiInputEnabled (newInput, true);
        
        deviceManager.addMidiInputCallback (newInput, this);
        midiInputList.setSelectedId (index + 1, dontSendNotification);
        
        lastInputIndex = index;
    }
    
    void comboBoxChanged (ComboBox* box) override
    {
        if (box == &midiInputList)
            setMidiInput (midiInputList.getSelectedItemIndex());
    }
    
    // These methods handle callbacks from the midi device + on-screen keyboard..
    void handleIncomingMidiMessage (MidiInput* source, const MidiMessage& message) override
    {
        const ScopedValueSetter<bool> scopedInputFlag (isAddingFromMidiInput, true);
        keyboardState.processNextMidiEvent (message);
        postMessageToList (message, source->getName());
    }
    
    void handleNoteOn (MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override
    {
      //  if ( isAddingFromMidiInput)
        {
           
            MidiMessage m (MidiMessage::noteOn (midiChannel, midiNoteNumber, velocity));
            //set current note so old noteoffs don't stop sound
            currentNote = midiNoteNumber;
            //set frequency to current MIDI note
            fundamental = m.getMidiNoteInHertz(currentNote);
            for(int osc = 0; osc < (sizeof(sines)/sizeof(*sines)); ++osc) {
                sines[osc].setFrequency(fundamental * (osc + 1));
            }
            m.setTimeStamp (Time::getMillisecondCounterHiRes() * 0.001);
            postMessageToList (m, "On-Screen Keybaord");
        }
    }
    
    void handleNoteOff (MidiKeyboardState*, int midiChannel, int midiNoteNumber, float /*velocity*/) override
    {
//        if (! isAddingFromMidiInput)
        {
            MidiMessage m (MidiMessage::noteOff (midiChannel, midiNoteNumber));
            // check if note off is for last played note
            if (currentNote == midiNoteNumber) {
                //zero frequencies on note off
                fundamental = 0;
                for(int osc = 0; osc < (sizeof(sines)/sizeof(*sines)); ++osc) {
                    sines[osc].setFrequency(fundamental * (osc + 1));
                }
            }
            m.setTimeStamp (Time::getMillisecondCounterHiRes() * 0.001);
            postMessageToList (m, "On-Screen Keyboard");
        }
    }
    // UI Elements

    Slider gainSlider, interpolateSlider; //interpolate slider to interpolate between instrument A & B
    ToggleButton sineButtonA, fluteButtonA, violinButtonA, oboeButtonA, clarinetButtonA, hornButtonA, tromboneButtonA, sineButtonB, fluteButtonB, violinButtonB, oboeButtonB, clarinetButtonB, hornButtonB, tromboneButtonB;
    
    Label gainLabel, interpolateLabel, sineLabelA, fluteLabelA, violinLabelA, oboeLabelA, clarinetLabelA, hornLabelA, tromboneLabelA, sineLabelB, fluteLabelB, violinLabelB, oboeLabelB, clarinetLabelB, hornLabelB, tromboneLabelB;

    
    Sine sines[12]; // the sine wave oscillators
    int harmonicStrengthsA[12], harmonicStrengthsB[12];  // relative strength of harmonics to set for different instruments
    int instrumentGroupAID = 1, instrumentGroupBID = 2;  // for Radiogrouping buttons
    float fundamental, interp;
    
    // Global Variables
    float gain;
    int onOff, samplingRate, nChans;
    
    // This is used to dispach an incoming message to the message thread
    class IncomingMessageCallback   : public CallbackMessage
    {
    public:
        IncomingMessageCallback (MainContentComponent* o, const MidiMessage& m, const String& s)
        : owner (o), message (m), source (s)
        {}
        
        void messageCallback() override
        {
            if (owner != nullptr)
                owner->addMessageToList (message, source);
        }
        
        Component::SafePointer<MainContentComponent> owner;
        MidiMessage message;
        String source;
    };
    
    
    
    void postMessageToList (const MidiMessage& message, const String& source)
    {
        (new IncomingMessageCallback (this, message, source))->post();
    }
    
    void addMessageToList (const MidiMessage& message, const String& source)
    {
        const double time = message.getTimeStamp() - startTime;
        
        const int hours = ((int) (time / 3600.0)) % 24;
        const int minutes = ((int) (time / 60.0)) % 60;
        const int seconds = ((int) time) % 60;
        const int millis = ((int) (time * 1000.0)) % 1000;
        
        const String timecode (String::formatted ("%02d:%02d:%02d.%03d",
                                                  hours,
                                                  minutes,
                                                  seconds,
                                                  millis));
        
        const String description (getMidiMessageDescription (message));
        
        const String midiMessageString (timecode + "  -  " + description + " (" + source + ")"); // [7]
        logMessage (midiMessageString);
    }
    
    //==============================================================================
    AudioDeviceManager deviceManager;           // [1]
    ComboBox midiInputList;                     // [2]
    Label midiInputListLabel;
    int lastInputIndex;                         // [3]
    bool isAddingFromMidiInput;                 // [4]
    int currentNote;
    
    MidiKeyboardState keyboardState;            // [5]
    MidiKeyboardComponent keyboardComponent;    // [6]
    
//    TextEditor midiMessagesBox;
    double startTime;
    
    //==============================================================================
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};

Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
