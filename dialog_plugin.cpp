/*
	MIT License

	Copyright (c) 2022 The Unstoppable

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

#include "General.h"
#include "engine.h"
#include "gmgame.h"
#include "gmplugin.h"

class DialogPlugin : public Plugin {
public:
	DialogPlugin() {
		RegisterEvent(EVENT_CHAT_HOOK, this);
		RegisterEvent(EVENT_DIALOG_HOOK, this);
	}

	~DialogPlugin() {
		UnregisterEvent(EVENT_CHAT_HOOK, this);
		UnregisterEvent(EVENT_DIALOG_HOOK, this);
	}

	bool OnChat(int PlayerID, TextMessageEnum Type, const wchar_t* Message, int recieverID) override 
	{
		if (!dialog) {
			if (!wcscmp(Message, L"!updialog")) {
				dialog = Create_User_Prompt(Get_GameObj(PlayerID), L"Demonstration", L"This is an example user prompt dialog, this one is also shipped as a sample.", 64, textbox_id, ok_btn, cancel_btn);
				Show_Dialog(dialog);
			}
			else if (!wcscmp(Message, L"!yndialog")) {
				dialog = Create_Yes_No_Dialog(Get_GameObj(PlayerID), L"Demonstration", L"This is an example Yes and No prompt, this one is also shipped as a sample.", yes_btn, no_btn);
				Show_Dialog(dialog);
			}
			else if (!wcscmp(Message, L"!yncdialog")) {
				dialog = Create_Yes_No_Cancel_Dialog(Get_GameObj(PlayerID), L"Demonstration", L"This is an example Yes, No and Cancel prompt, this one is also shipped as a sample.", yes_btn, no_btn, cancel_btn);
				Show_Dialog(dialog);
			}
		}
		else {
			if (!wcscmp(Message, L"!deletedialog")) {
				Reset_IDs();
			}
		}
		return true;
	}

	void OnDialog(int PlayerID, int DialogID, int ControlID, DialogMessageType MessageType) override
	{
		switch (MessageType) {
			case DialogMessageType::MESSAGE_TYPE_CONTROL_MOUSE_CLICK: {
				if (ControlID == ok_btn) {
					if (textbox_id != 0) {
						Console_Input(StringClass::getFormattedString("pamsg %d You wrote \"%ws\"!", PlayerID, dialog->Find_Control(textbox_id)->As_ScriptedTextAreaControlClass()->Get_Text()));
					}
					else {
						Console_Input(StringClass::getFormattedString("pamsg %d You clicked OK.", PlayerID));
					}
					Reset_IDs();
				}
				else if (ControlID == yes_btn) {
					Console_Input(StringClass::getFormattedString("pamsg %d You clicked Yes.", PlayerID));
					Reset_IDs();
				}
				else if (ControlID == no_btn) {
					Console_Input(StringClass::getFormattedString("pamsg %d You clicked No.", PlayerID));
					Reset_IDs();
				}
				else if (ControlID == cancel_btn) {
					Console_Input(StringClass::getFormattedString("pamsg %d You clicked Cancel.", PlayerID));
					Reset_IDs();
				}
			}
			break;

			case DialogMessageType::MESSAGE_TYPE_CONTROL_VALUE_CHANGE: {
				if (ControlID == textbox_id) {
					Console_Input(StringClass::getFormattedString("msg %s wrote \"%ws\"!", Get_Player_Name_By_ID(PlayerID), dialog->Find_Control(textbox_id)->As_ScriptedTextAreaControlClass()->Get_Text()));
				}
			}
			break;

			case DialogMessageType::MESSAGE_TYPE_DIALOG_SHOW: {
				Console_Input(StringClass::getFormattedString("msg %s opened a dialog with ID %d!", Get_Player_Name_By_ID(PlayerID), DialogID));
			}
			break;

			case DialogMessageType::MESSAGE_TYPE_DIALOG_CLOSE: {
				Console_Input(StringClass::getFormattedString("msg %s closed a dialog with ID %d!", Get_Player_Name_By_ID(PlayerID), DialogID));
			}
			break;
		}
	}

	void Reset_IDs() {
		Delete_Dialog(dialog);
		dialog = 0;
		textbox_id = 0;
		ok_btn = 0;
		yes_btn = 0;
		no_btn = 0;
		cancel_btn = 0;
	}

private:
	ScriptedDialogClass* dialog;
	int textbox_id;
	int ok_btn;
	int yes_btn;
	int no_btn;
	int cancel_btn;
};


DialogPlugin plugin;
extern "C" __declspec(dllexport) Plugin* Plugin_Init()
{
	return &plugin;
}
