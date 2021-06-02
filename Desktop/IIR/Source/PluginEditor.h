/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "PluginProcessor.h"

static const juce::Identifier ID("ID");

//==============================================================================
/**
 */
class JuceDemoAudioProcessorEditor : public juce::AudioProcessorEditor,
                                     public juce::Slider::Listener,
                                     public juce::ComboBox::Listener {
 public:
  JuceDemoAudioProcessorEditor(JuceDemoAudioProcessor&);
  ~JuceDemoAudioProcessorEditor() override;

  //==============================================================================
  void paint(juce::Graphics&) override;
  void resized() override;

 private:
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  JuceDemoAudioProcessor& audioProcessor;

  juce::Slider slider[MAX_PARAM_NUM];
  juce::Label label[MAX_PARAM_NUM];
  juce::ComboBox comboBox[MAX_PARAM_NUM];

  void sliderValueChanged(juce::Slider* slider) override;
  void comboBoxChanged(juce::ComboBox* comboBox) override;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(JuceDemoAudioProcessorEditor)
};
