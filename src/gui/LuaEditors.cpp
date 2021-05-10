/*
** Surge Synthesizer is Free and Open Source Software
**
** Surge is made available under the Gnu General Public License, v3.0
** https://www.gnu.org/licenses/gpl-3.0.en.html
**
** Copyright 2004-2021 by various individuals as described by the Git transaction log
**
** All source at: https://github.com/surge-synthesizer/surge.git
**
** Surge was a commercial product from 2004-2018, with Copyright and ownership
** in that period held by Claes Johanson at Vember Audio. Claes made Surge
** open source in September 2018.
*/

#include "LuaEditors.h"
#include "SurgeGUIEditor.h"
#include "RuntimeFont.h"

struct EditorColors
// http://www.zovirl.com/2011/07/22/solarized_cheat_sheet/
// replace this with skin engine one day
{
    static constexpr uint32_t base03 = 0xFF002b36;
    static constexpr uint32_t base02 = 0xFF073642;
    static constexpr uint32_t base01 = 0xFF586e75;
    static constexpr uint32_t base00 = 0xFF657b83;
    static constexpr uint32_t base0 = 0xFF839496;
    static constexpr uint32_t base1 = 0xFF93a1a1;
    static constexpr uint32_t base2 = 0xFFeee8d5;
    static constexpr uint32_t base3 = 0xFFfdf6e3;
    static constexpr uint32_t yellow = 0xFFb58900;
    static constexpr uint32_t orange = 0xFFcb4b16;
    static constexpr uint32_t red = 0xFFdc322f;
    static constexpr uint32_t magenta = 0xFFd33682;
    static constexpr uint32_t violet = 0xFF6c71c4;
    static constexpr uint32_t blue = 0xFF268bd2;
    static constexpr uint32_t cyan = 0xFF2aa198;
    static constexpr uint32_t green = 0xFF859900;

    static void setupLight(juce::CodeEditorComponent *comp)
    {
        auto cs = comp->getColourScheme();
        cs.set("Error", juce::Colour(red));
        cs.set("Comment", juce::Colour(base1));
        cs.set("Keyword", juce::Colour(violet));
        cs.set("Operator", juce::Colour(green));
        cs.set("Identifier", juce::Colour(cyan));
        cs.set("Float", juce::Colour(red));
        cs.set("Integer", juce::Colour(orange));
        cs.set("Bracket", juce::Colour(green));
        cs.set("Punctuation", juce::Colour(green));

        comp->setColourScheme(cs);

        comp->setColour(juce::CodeEditorComponent::defaultTextColourId, juce::Colour(base03));
        comp->setColour(juce::CodeEditorComponent::backgroundColourId, juce::Colour(base3));
        comp->setColour(juce::CodeEditorComponent::lineNumberTextId, juce::Colour(orange));
        comp->setColour(juce::CodeEditorComponent::lineNumberBackgroundId, juce::Colour(base2));

        comp->retokenise(0, -1);
    }
};

FormulaModulatorEditor::FormulaModulatorEditor(SurgeGUIEditor *ed, SurgeStorage *s,
                                               FormulaModulatorStorage *fs)
    : editor(ed), formulastorage(fs)
{
    applyButton = std::make_unique<juce::TextButton>("Apply");
    applyButton->setButtonText("Apply");
    applyButton->setEnabled(false);
    applyButton->setBounds(5, 340, 40, 15);
    applyButton->addListener(this);
    addAndMakeVisible(applyButton.get());

    mainDocument = std::make_unique<juce::CodeDocument>();
    mainDocument->insertText(0, fs->formula);
    mainDocument->addListener(this);
    tokenizer = std::make_unique<juce::LuaTokeniser>();

    mainEditor = std::make_unique<juce::CodeEditorComponent>(*mainDocument, tokenizer.get());
    mainEditor->setFont(Surge::GUI::getFontManager()->getFiraMonoAtSize(10));
    mainEditor->setBounds(5, 25, 730, 310);

    EditorColors::setupLight(mainEditor.get());
    addAndMakeVisible(mainEditor.get());

    warningLabel = std::make_unique<juce::Label>("Warning");
    warningLabel->setFont(Surge::GUI::getFontManager()->getLatoAtSize(14));
    warningLabel->setBounds(5, 5, 730, 20);
    warningLabel->setColour(juce::Label::textColourId, juce::Colour(255, 0, 0));
    warningLabel->setText("WARNING: Dont use this! Super alpha! It will crash and probably won't "
                          "load in the future or work now. And have fun!",
                          juce::NotificationType::dontSendNotification);

    addAndMakeVisible(warningLabel.get());
}
void FormulaModulatorEditor::buttonClicked(juce::Button *button)
{
    if (button == applyButton.get())
    {
        formulastorage->formula = mainDocument->getAllContent().toStdString();
        applyButton->setEnabled(false);
        editor->invalidateFrame();
    }
}

void FormulaModulatorEditor::resized()
{
    auto w = getWidth() - 5;
    auto h = getHeight() - 5; // this is a hack obvs
    int m = 3, m2 = m * 2;
    warningLabel->setBounds(m, m, w - m2, 20);
    mainEditor->setBounds(m, 20 + m2, w - m2, h - 40 - m2 - m2);
    applyButton->setBounds(m, h - 20, 50, 20 - m);
}
void FormulaModulatorEditor::codeDocumentTextInserted(const juce::String &newText, int insertIndex)
{
    applyButton->setEnabled(true);
}
void FormulaModulatorEditor::codeDocumentTextDeleted(int startIndex, int endIndex)
{
    applyButton->setEnabled(true);
}