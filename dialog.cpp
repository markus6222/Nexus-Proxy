#include "dialog.h"
void Dialog::addSpacer(const SpacerTypes type)
{
	switch (type)
	{
	case SPACER_BIG:
		dialogstr.append("add_spacer|big|\n");
		break;
	case SPACER_SMALL:
		dialogstr.append("add_spacer|small|\n");
		break;
	default:
		break;
	}
}

void Dialog::addStaticCustomFrameENDLine()
{
	dialogstr.append("\nadd_button_with_icon||END_LIST|noflags|0|0|");
}
void Dialog::addStaticPurpleFrameWithIdText(const string itemID, const string textUnderFrame, const string buttonName, const bool isEndLine)
{
	if (isEndLine == false)
	{
		dialogstr.append("\nadd_button_with_icon|" + buttonName + "|" + textUnderFrame + "|frame|" + itemID + "||purple");
	}
	else
	{
		dialogstr.append("\nadd_button_with_icon||END_LIST|noflags|0|0|\nadd_button_with_icon|" + buttonName + "|" + textUnderFrame + "|frame|" + itemID + "||purple");
	}
}
void Dialog::addItemWithCustomText(const string itemID, const string textUnderButton, const string buttonName)
{
	dialogstr.append("\nadd_button_with_icon|" + buttonName + "|" + textUnderButton + "|staticBlueFrame|" + itemID + "|");
}

void Dialog::addNewLineAfterFrame()
{
	dialogstr.append("\nadd_button_with_icon||END_LIST|noflags|0|0|");
}

void Dialog::addPicker(const string pickername, const string pickertext, const string headText)
{
	dialogstr.append("\nadd_item_picker|" + pickername + "|" + pickertext + "|" + headText + "|");
}

void Dialog::addStaticBlueFrame(const string itemID, const string textUnderFrame, const string buttonName, const bool isEndLine)
{
	if (isEndLine == false)
	{
		dialogstr.append("\nadd_button_with_icon|" + buttonName + "||staticBlueFrame|" + itemID + "|" + textUnderFrame + "|");
	}
	else
	{
		dialogstr.append("\nadd_button_with_icon||END_LIST|noflags|0|0|\nadd_button_with_icon|" + buttonName + "||staticBlueFrame|" + itemID + "|" + textUnderFrame + "|");
	}
}

void Dialog::addStaticBlueFrameWithIdCountText(const string itemID, string count, const string textUnderFrame, const string buttonName, const bool isEndLine)
{
	if (isEndLine == false)
	{
		dialogstr.append("\nadd_button_with_icon|" + buttonName + "|" + textUnderFrame + "|frame|" + itemID + "|" + count + "|");
	}
	else
	{
		dialogstr.append("\nadd_button_with_icon||END_LIST|noflags|0|0|\nadd_button_with_icon|" + buttonName + "|" + textUnderFrame + "|frame|" + itemID + "|" + count + "|");
	}
}

void Dialog::addLabelWithIcon(const string text, const int tileid, const LabelStyles type)
{
	switch (type)
	{
	case LABEL_BIG:
		dialogstr.append("add_label_with_icon|big|" + text + "|left|" + to_string(tileid) + "|\n");
		break;
	case LABEL_SMALL:
		dialogstr.append("add_label_with_icon|small|" + text + "|left|" + to_string(tileid) + "|\n");
		break;
	default:
		break;
	}
}

void Dialog::addLabelWithIconButton(const string text, const int tileid, const string buttontext)
{
	dialogstr.append(
		"add_label_with_icon_button||" + text + "|left|" + to_string(tileid) + "|" + buttontext + "|\n");
}

void Dialog::addButton(const string buttonname, const string buttontext)
{
	dialogstr.append("add_button|" + buttonname + "|" + buttontext + "|noflags|0|0|\n");
}

void Dialog::addCheckbox(const string checkboxname, const string string, bool checked)
{
	switch (checked)
	{
	case 1:
		dialogstr.append("add_checkbox|" + checkboxname + "|" + string + "|1|\n");
		break;
	case 0:
		dialogstr.append("add_checkbox|" + checkboxname + "|" + string + "|0|\n");
		break;
	default:
		break;
	}
}

void Dialog::addTextBox(const string str)
{
	dialogstr.append("add_textbox|" + str + "|left|\n");
}

void Dialog::addSmallText(const string str)
{
	dialogstr.append("add_smalltext|" + str + "|\n");
}

void Dialog::addCustomText(const string str)
{
	dialogstr.append("add_custom_textbox|" + str + "|size:tiny;color:255,255,255,255|\n");
}

void Dialog::addCustomMargin1() {
	dialogstr.append("add_custom_margin|x:0;y:10|\n");
}

void Dialog::addCustomMargin2() {
	dialogstr.append("add_custom_margin|x:0;y:-32|\n");
}

void Dialog::addInputBox(const string name, const string text, const string cont, const int size)
{
	dialogstr.append("add_text_input|" + name + "|" + text + "|" + cont + "|" + to_string(size) + "|\n");
}

void Dialog::addQuickExit()
{
	dialogstr.append("add_quick_exit|\n");
}

void Dialog::endDialog(const string name, const string accept, const string nvm)
{
	dialogstr.append("end_dialog|" + name + "|" + nvm + "|" + accept + "|\n");
}

void Dialog::addCustom(const string name)
{
	dialogstr.append(name + "\n");
}

string Dialog::finishDialog() const
{
	return dialogstr;
}