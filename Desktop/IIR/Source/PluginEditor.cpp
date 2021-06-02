/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"

#include "PluginProcessor.h"

//==============================================================================
JuceDemoAudioProcessorEditor::JuceDemoAudioProcessorEditor(
    JuceDemoAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p) {
  auto processor = audioProcessor._audioProcessor;

  int paramNum = processor->getParamNum();
  int MAX_COL_NUM = 7;
  int rowNum = ceilf(paramNum / (float)MAX_COL_NUM);
  float marginTop = 50;
  float marginBottom = 50;
  float marginLeft = 50;
  float marginRight = 50;
  float knobWidth = 100;
  float knobHeight = 100;
  float textBoxWidth = knobWidth;
  float textBoxHeight = 20;
  float comboBoxWidth = 80;
  float comboBoxHeight = 30;
  float labelWidth = knobWidth;
  float labelHeight = 20;
  float itemHeight = knobHeight + labelHeight + textBoxHeight;
  float itemWidth = knobWidth;

  for (int i = 0; i < paramNum; i++) {
    String paramLabel = processor->getParamLabel(i);
    ParamType paramType = processor->getParamType(i);
    FloatArray paramRange = processor->getParamRange(i);
    float paramDefaultValue = processor->getParamDefaultValue(i);

    int rowIndex = i / MAX_COL_NUM;
    int colIndex = i % MAX_COL_NUM;

    if (paramType == Slider) {
      // 处理 Slider 旋钮
      // 设置 ID
      slider[i].getProperties().set(ID, i);

      // 设置 range 和默认值
      slider[i].setRange(paramRange[0], paramRange[1], paramRange[2]);
      slider[i].setValue(paramDefaultValue);
      slider[i].setDoubleClickReturnValue(true, paramDefaultValue,
                                          juce::ModifierKeys::altModifier);

      // 设置样式
      slider[i].setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
      slider[i].setTextBoxStyle(juce::Slider::TextBoxAbove, true, textBoxWidth,
                                textBoxHeight);
      slider[i].setColour(juce::Slider::textBoxOutlineColourId,
                          juce::Colour::fromRGBA(0, 0, 0, 0));
      slider[i].setColour(juce::Slider::thumbColourId,
                          juce::Colour::fromRGBA(0, 0, 0, 0));
      slider[i].setColour(juce::Slider::rotarySliderFillColourId,
                          juce::Colour::fromRGB(73, 192, 217));

      // 设置位置 //改这里
      slider[i].setBounds(marginLeft + itemWidth * colIndex,
                          marginTop + itemHeight * rowIndex, knobWidth,
                          knobHeight);
      addAndMakeVisible(slider[i]);

      // 监听回调
      slider[i].addListener(this);
    } else if (paramType == ComboBox) {
      // 处理 ComboBox 旋钮
      // 设置 ID
      comboBox[i].getProperties().set(ID, i);

      // 设置 range 和默认值
      StringArray comboBoxLabel = processor->getParamRangeLabel(i);
      int selectedID = 0;
      for (int j = 0; j < comboBoxLabel.size(); j++) {
        comboBox[i].addItemList(comboBoxLabel[j], j + 1);
        if (paramRange[j] == paramDefaultValue) selectedID = j + 1;
      }
      comboBox[i].setSelectedId(selectedID);

      // 设置样式

      // 设置位置
      comboBox[i].setBounds(
          marginLeft + itemWidth * colIndex + (knobWidth - comboBoxWidth) / 2,
          marginTop + itemHeight * rowIndex +
              (knobHeight + textBoxHeight - comboBoxHeight) / 2,
          comboBoxWidth, comboBoxHeight);
      addAndMakeVisible(comboBox[i]);

      // 监听回调
      comboBox[i].addListener(this);
    }

    // 处理 Label
    label[i].setText(paramLabel, juce::dontSendNotification);
    label[i].setColour(juce::Label::textColourId,
                       juce::Colour::fromRGB(122, 144, 155));
    label[i].setBounds(marginLeft + itemWidth * colIndex,
                       marginTop + itemHeight * rowIndex + 100, labelWidth,
                       labelHeight);
    label[i].setJustificationType(juce::Justification::centred);
    addAndMakeVisible(label[i]);
  }

  int width = marginLeft + marginRight + knobWidth * MAX_COL_NUM;
  int height = marginTop + marginBottom + itemHeight * rowNum;
  setSize(width, height);
}

JuceDemoAudioProcessorEditor::~JuceDemoAudioProcessorEditor() {}

//==============================================================================
void JuceDemoAudioProcessorEditor::paint(juce::Graphics& g) {
  // (Our component is opaque, so we must completely fill the background with a
  // solid colour)
  g.fillAll(juce::Colour::fromRGB(35, 55, 77));
}

void JuceDemoAudioProcessorEditor::resized() {
  // This is generally where you'll want to lay out the positions of any
  // subcomponents in your editor..
}

void JuceDemoAudioProcessorEditor::sliderValueChanged(juce::Slider* slider) {
  int id = slider->getProperties().getWithDefault(ID, -1);
  if (id != -1) {
    audioProcessor._audioProcessor->setParam(id, slider->getValue());
  }
}

void JuceDemoAudioProcessorEditor::comboBoxChanged(juce::ComboBox* comboBox) {
  int id = comboBox->getProperties().getWithDefault(ID, -1);
  FloatArray paramRange = audioProcessor._audioProcessor->getParamRange(id);
  float value = paramRange[comboBox->getSelectedId() - 1];
  if (id != -1) {
    audioProcessor._audioProcessor->setParam(id, value);
  }
}
