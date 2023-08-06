#pragma once

#include <SDL.h>

#include <map>
#include <string>

#include "IotaEvent.hpp"

namespace iota {
	namespace Keyboard {
		enum class KeyCode {
			UnknownKey = 0,
			A = 4,
			B = 5,
			C = 6,
			D = 7,
			E = 8,
			F = 9,
			G = 10,
			H = 11,
			I = 12,
			J = 13,
			K = 14,
			L = 15,
			M = 16,
			N = 17,
			O = 18,
			P = 19,
			Q = 20,
			R = 21,
			S = 22,
			T = 23,
			U = 24,
			V = 25,
			W = 26,
			X = 27,
			Y = 28,
			Z = 29,
			One = 30,
			Two = 31,
			Three = 32,
			Four = 33,
			Five = 34,
			Six = 35,
			Seven = 36,
			Eight = 37,
			Nine = 38,
			Zero = 39,
			Return = 40,
			Escape = 41,
			Backspace = 42,
			Tab = 43,
			Space = 44,
			Minux = 45,
			Equals = 46,
			LeftBracket = 47,
			RightBracket = 48,
			Backslash = 49,
			Nonuslash = 50,
			SemiColon = 51,
			Apostrophe = 52,
			Grave = 53,
			Comma = 54,
			Period = 55,
			Slash = 56,
			Capslock = 57,
			F1 = 58,
			F2 = 59,
			F3 = 60,
			F4 = 61,
			F5 = 62,
			F6 = 63,
			F7 = 64,
			F8 = 65,
			F9 = 66,
			F10 = 67,
			F11 = 68,
			F12 = 69,
			PrintScreen = 70,
			ScrollLock = 71,
			Pause = 72,
			Insert = 73,
			Home = 74,
			PageUp = 75,
			Delete = 76,
			End = 77,
			PageDown = 78,
			Right = 79,
			Left = 80,
			Down = 81,
			Up = 82,
			Numlock = 83,
			NumpadDivide = 84,
			NumpadMultiply = 85,
			NumpadMinus = 86,
			NumpadPlus = 87,
			NumpadEnter = 88,
			NumpadOne = 89,
			NumpadTwo = 90,
			NumpadThree = 91,
			NumpadFour = 92,
			NumpadFive = 93,
			NumpadSix = 94,
			NumpadSeven = 95,
			NumpadEight = 96,
			NumpadNine = 97,
			NumpadZero = 98,
			NumpadPeriod = 99,
			NonusBackslash = 100,
			Application = 101,
			Power = 102,
			NumpadEquals = 103,
			F13 = 104,
			F14 = 105,
			F15 = 106,
			F16 = 107,
			F17 = 108,
			F18 = 109,
			F19 = 110,
			F20 = 111,
			F21 = 112,
			F22 = 113,
			F23 = 114,
			F24 = 115,
			Execute = 116,
			Help = 117,
			Menu = 118,
			Select = 119,
			Stop = 120,
			Again = 121,
			Undo = 122,
			Cut = 123,
			Copy = 124,
			Paste = 125,
			Find = 126,
			Mute = 127,
			VolumeUp = 128,
			VolumeDown = 129,
			NumpadComma = 133,
			NumpadEqualsAs400 = 134,
			International1 = 135,
			International2 = 136,
			International3 = 137,
			International4 = 138,
			International5 = 139,
			International6 = 140,
			International7 = 141,
			International8 = 142,
			International9 = 143,
			Lang1 = 144,
			Lang2 = 145,
			Lang3 = 146,
			Lang4 = 147,
			Lang5 = 148,
			Lang6 = 149,
			Lang7 = 150,
			Lang8 = 151,
			Lang9 = 152,
			AltErase = 153,
			SysRq = 154,
			Cancel = 155,
			Clear = 156,
			Prior = 157,
			Return2 = 158,
			Separator = 159,
			Out = 160,
			Oper = 161,
			ClearAgain = 162,
			CrSel = 163,
			ExSel = 164,
			NumpadDoubleZero = 176,
			NumpadTripleZero = 177,
			ThousandSeparator = 178,
			DecimalSeparator = 179,
			CurrencyUnit = 180,
			CurrencySubUnit = 181,
			NumpadLeftParenthesis = 182,
			NumpadRightParenthesis = 183,
			NumpadLeftBrace = 184,
			NumpadRightBrace = 185,
			NumpadTab = 186,
			NumpadBackspace = 187,
			NumpadA = 188,
			NumpadB = 189,
			NumpadC = 190,
			NumpadD = 191,
			NumpadE = 192,
			NumpadF = 193,
			NumpadXOR = 194,
			NumpadPower = 195,
			NumpadPercent = 196,
			NumpadLess = 197,
			NumpadGreater = 198,
			NumpadAmpersand = 199,
			NumpadDoubleAmpersand = 200,
			NumpadVerticalBar = 201,
			NumpadDoubleVerticalBar = 202,
			NumpadColon = 203,
			NumpadHash = 204,
			NumpadSpace = 205,
			NumpadAt = 206,
			NumpadExclamation = 207,
			NumpadMemStore = 208,
			NumpadMemRecall = 209,
			NumpadMemClear = 210,
			NumpadMemAdd = 211,
			NumpadMemSubtract = 212,
			NumpadMemMultiply = 213,
			NumpadMemDivide = 214,
			NumpadPlusMinus = 215,
			NumpadClear = 216,
			NumpadClearEntry = 217,
			NumpadBinary = 218,
			NumpadOctal = 219,
			NumpadDecimal = 220,
			NumpadHexadecimal = 221,
			LeftControl = 224,
			LeftShift = 225,
			LeftAlt = 226,
			LeftWindows = 227, //Command key (Apple)
			RightControl = 228,
			RightShift = 229,
			RightAlt = 230,
			RightWindows = 231,
			Mode = 257,
			AudioNext = 258,
			AudioPrevious = 259,
			AudioStop = 260,
			AudioPlay = 261,
			AudioMute = 262,
			MediaSelect = 263,
			WWW = 264,
			Mail = 265,
			Calculator = 266,
			Computer = 267,
			ApplicationControlSearch = 268,
			ApplicationControlHome = 269,
			ApplicationControlBack = 270,
			ApplicationControlForward = 271,
			ApplicationControlStop = 272,
			ApplicationControlRefresh = 273,
			ApplicationControlBookmarks = 274,
			BrightnessDown = 275,
			BrightnessUp = 276,
			DisplaySwitch = 277,
			KBDIllumToggle = 278,
			KBDIllumDown = 279,
			KBDIllumUp = 280,
			Eject = 281,
			Sleep = 282,
			App1 = 283,
			App2 = 284,
			AudioRewind = 285,
			AudioFastForward = 286,
		};
		enum class KeyState {
			RELEASE,
			DOWN,
		};

		SDL_Scancode GetKey(KeyCode key);
		KeyCode GetKey(SDL_Scancode scancode);

		const std::map<SDL_Scancode, KeyCode>& GetKeyEntries();
		const std::map<KeyCode, std::string>& GetKeystringEntries();

		Event::EventSignal<KeyCode>& GetKeydownEvent();
		Event::EventSignal<KeyCode>& GetKeyupEvent();

		void HandleKeyEvent(SDL_Scancode scancode, KeyState state);

		bool IsKeyDown(KeyCode key);
		bool IsKeyReleased(KeyCode key);
	};
};