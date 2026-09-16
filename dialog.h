#pragma once
#include <string>
using namespace std;
enum LabelStyles
{
	LABEL_BIG,
	LABEL_SMALL
};

enum SpacerTypes
{
	SPACER_BIG,
	SPACER_SMALL
};

enum CheckboxTypes
{
        CHECKBOX_NOT_SELECTED,	
        CHECKBOX_SELECTED
};
class Dialog
{
public:
	string dialogstr = "";
	void addSpacer(SpacerTypes type);
	void addStaticCustomFrameENDLine();
	void addStaticPurpleFrameWithIdText(const string itemID, const string textUnderFrame, const string buttonName, const bool isEndLine);
	void addLabelWithIcon(string text, int tileid, LabelStyles type);
	void addLabelWithIconButton(string text, int tileid, string buttontext);
	void addButton(string buttonname, string buttontext);
	void addPicker(string pickername, string pickertext, string headText);
	void addCheckbox(string checkboxname, string string, bool checked);
	void addTextBox(string str);
	void addSmallText(string str);
	void addCustomText(string str);
	void addCustomMargin1();
	void addCustomMargin2();
	void addItemWithCustomText(string itemID, string textUnderButton, string buttonName);
	void addStaticBlueFrame(string itemID, string textUnderFrame, string buttonName, bool isEndLine);
	void addInputBox(string name, string text, string cont, int size);
	void addStaticBlueFrameWithIdCountText(string itemID, string count, string textUnderFrame, string buttonName, bool isEndLine);
	void addQuickExit();
	void addNewLineAfterFrame();
	void endDialog(string name, string accept, string nvm);
	void addCustom(string name);
	string finishDialog() const;

	explicit operator string() const
	{
		return this->dialogstr;
	}
};
