/***********************************************************************************************
created: 		2022-01-20

author:			chensong

purpose:		input_device_event
************************************************************************************************/

#ifndef _C_INPUT_DEVICE_EVENT_H_
#define _C_INPUT_DEVICE_EVENT_H_
//#include <winnt.h>
#include "cnet_types.h"
#include "api/data_channel_interface.h"
#include <map>
#include "cprotocol.h"
namespace chen {

	namespace EMouseButtons
	{
		enum Type
		{
			Left = 0,
			Middle,
			Right,
			Thumb01,
			Thumb02,

			Invalid,
		};
	}



	

	/** The types of event which can be processed by the device. */
	enum class EventType
	{
		UNDEFINED,		 /** No value. */
		KEY_DOWN,		 /** A key has been pushed down. */
		KEY_UP,			 /** A key has been released. */
		KEY_PRESS,		 /** A key has been pressed and a character has been input. */
		MOUSE_ENTER,	 /** The mouse has entered canvas. */
		MOUSE_LEAVE,	 /** The mouse has left the canvas. */
		MOUSE_MOVE,		 /** The mouse has been moved. */
		MOUSE_DOWN,		 /** A mouse button has been clicked. */
		MOUSE_UP,		 /** A mouse button has been released. */
		MOUSE_WHEEL,	 /** The mouse wheel was scrolled. */
		TOUCH_START,	 /** A finger is put down onto the canvas. */
		TOUCH_END,		 /** A finger is lifted from the canvas. */
		TOUCH_MOVE,		 /** A finger is being dragged along the surface of the canvas. */
		GAMEPAD_PRESS,	 /** A gamepad button has been pressed. */
		GAMEPAD_RELEASE, /** A gamepad button has been released. */
		GAMEPAD_ANALOG	 /** A gamepad analog stick has been moved. */
	};

	/** A general input event. */
	struct FEvent
	{
		/** The type of the general event. */
		EventType Event;

		/** A generic piece of data which is used to hold information about the
		* event, specialized by making a union with an appropriate struct. */
		union
		{
			uint64 Word;

			struct   /** KEY_DOWN */
			{
				uint8 KeyCode;
				bool bIsRepeat;
			} KeyDown;

			struct   /* KEY_UP */
			{
				uint8 KeyCode;
			} KeyUp;

			struct   /** KEY_PRESSED */
			{
				TCHAR Character;
			} Character;

			struct   /** MOUSE_MOVE */
			{
				int16 DeltaX;
				int16 DeltaY;
				uint16 PosX;
				uint16 PosY;
			} MouseMove;

			struct   /** MOUSE_DOWN, MOUSE_UP */
			{
				uint8 Button;
				uint16 PosX;
				uint16 PosY;
			} MouseButton;

			struct   /** MOUSE_WHEEL */
			{
				int16 Delta;
				uint16 PosX;
				uint16 PosY;
			} MouseWheel;

			struct   /** TOUCH_START, TOUCH_END, TOUCH_MOVE */
			{
				uint8 TouchIndex;
				uint16 PosX;
				uint16 PosY;
				uint8 Force;
			} Touch;

			struct   /** GAMEPAD_PRESSED, GAMEPAD_RELEASED */
			{
				uint8 ControllerIndex;
				uint8 ButtonIndex;
				bool bIsRepeat;

			} GamepadButton;

			struct   /** GAMEPAD_PRESSED, GAMEPAD_RELEASED */
			{
				uint8 ControllerIndex;
				float AnalogValue;
				uint8 AxisIndex;
			} GamepadAnalog;
		} Data;

		/**
		* Create a completely empty event.
		*/
		FEvent()
			: Event(EventType::UNDEFINED)
		{
		}

		/**
		* Create an event of the given type.
		* @param InEvent - The type of the event.
		*/
		FEvent(EventType InEvent)
			: Event(InEvent)
		{
		}

		/**
		* An event related to gamepad analog stick being moved.
		* @param InControllerIndex - Number representing the index of the controller.
		* @param InAxisIndex - The axis that is non-centered.
		* @param InAnalogValue - The value of the analog stick.
		*/
		void SetGamepadAnalog(uint8 InControllerIndex, uint8 InAxisIndex, float InAnalogValue)
		{
			check(Event == EventType::GAMEPAD_ANALOG);
			Data.GamepadAnalog.ControllerIndex = InControllerIndex;
			Data.GamepadAnalog.AxisIndex = InAxisIndex;
			Data.GamepadAnalog.AnalogValue = InAnalogValue;
		}

		/**
		* An event related to a gamepad button being pushed down.
		* @param InControllerIndex - Number representing the index of the controller.
		* @param InButtonIndex - Numerical code identifying the pushed down button.
		* @param InIsRepeat - Whether the button is being kept down and is repeating.
		*/
		void SetGamepadButtonPressed(uint8 InControllerIndex, uint8 InButtonIndex, bool InIsRepeat)
		{
			check(Event == EventType::GAMEPAD_PRESS);
			Data.GamepadButton.ControllerIndex = InControllerIndex;
			Data.GamepadButton.ButtonIndex = InButtonIndex;
			Data.GamepadButton.bIsRepeat = InIsRepeat;
		}

		/**
		* An event related to a gamepad button being released.
		* @param InControllerIndex - Number representing the index of the controller.
		* @param InButtonIndex - Numerical code identifying the released button.
		*/
		void SetGamepadButtonReleased(uint8 InControllerIndex, uint8 InButtonIndex)
		{
			check(Event == EventType::GAMEPAD_RELEASE);
			Data.GamepadButton.ControllerIndex = InControllerIndex;
			Data.GamepadButton.ButtonIndex = InButtonIndex;
		}

		/**
		* An event related to a key being pushed down.
		* @param InKeyCode - Numerical code identifying the pushed down key.
		* @param InIsRepeat - Whether the key is being kept down and is repeating.
		*/
		void SetKeyDown(uint8 InKeyCode, bool InIsRepeat)
		{
			check(Event == EventType::KEY_DOWN);
			Data.KeyDown.KeyCode = InKeyCode;
			Data.KeyDown.bIsRepeat = InIsRepeat;
		}

		/**
		* An event related to a key being released.
		* @param InKeyCode - Numerical code identifying the released key.
		*/
		void SetKeyUp(uint8 InKeyCode)
		{
			check(Event == EventType::KEY_UP);
			Data.KeyUp.KeyCode = InKeyCode;
		}

		/**
		* An event related to character input.
		* @param InCharacter - The character being input.
		*/
		void SetCharCode(TCHAR InCharacter)
		{
			check(Event == EventType::KEY_PRESS);
			Data.Character.Character = InCharacter;
		}

		/**
		* An event related to mouse movement.
		* @param InPoxX - The X position of the mouse pointer.
		* @param InPosY - The Y position of the mouse pointer.
		* @param InDeltaX - The change in the X position of the mouse pointer.
		* @param InDeltaY - The change in the Y position of the mouse pointer.
		*/
		void SetMouseDelta(uint16 InPosX, uint16 InPosY, int16 InDeltaX, int16 InDeltaY)
		{
			check(Event == EventType::MOUSE_MOVE);
			Data.MouseMove.DeltaX = InDeltaX;
			Data.MouseMove.DeltaY = InDeltaY;
			Data.MouseMove.PosX = InPosX;
			Data.MouseMove.PosY = InPosY;
		}

		/**
		* An event related to mouse buttons.
		* @param InButton - The button number corresponding to left, middle, right, etc.
		* @param InPoxX - The X position of the mouse pointer.
		* @param InPosY - The Y position of the mouse pointer.
		*/
		void SetMouseClick(uint8 InButton, uint16 InPosX, uint16 InPosY)
		{
			check(Event == EventType::MOUSE_DOWN || Event == EventType::MOUSE_UP);
			Data.MouseButton.Button = InButton;
			Data.MouseButton.PosX = InPosX;
			Data.MouseButton.PosY = InPosY;
		}

		/**
		* An event related to the mouse scroll wheel.
		* @param InButton - The amount by which the mouse wheel was scrolled.
		* @param InPoxX - The X position of the mouse pointer when the wheel was scrolled.
		* @param InPosY - The Y position of the mouse pointer when the wheel was scrolled.
		*/
		void SetMouseWheel(int16 InDelta, uint16 InPosX, uint16 InPosY)
		{
			check(Event == EventType::MOUSE_WHEEL);
			Data.MouseWheel.Delta = InDelta;
			Data.MouseWheel.PosX = InPosX;
			Data.MouseWheel.PosY = InPosY;
		}

		/**
		* An event related to a finger touching the canvas.
		* @param InTouchIndex - The finger used in multi-touch.
		* @param InPoxX - The X position of the finger.
		* @param InPosY - The Y position of the finger.
		* @param InForce - The amount of pressure being applied by the finger.
		*/
		void SetTouch(uint8 InTouchIndex, uint16 InPosX, uint16 InPosY, uint8 InForce)
		{
			check(Event == EventType::TOUCH_START || Event == EventType::TOUCH_END || Event == EventType::TOUCH_MOVE);
			Data.Touch.TouchIndex = InTouchIndex;
			Data.Touch.PosX = InPosX;
			Data.Touch.PosY = InPosY;
			Data.Touch.Force = InForce;
		}

		/**
		* Get information about an event related to gamepad analog stick being moved.
		* @param OutControllerIndex - Number representing the index of the controller.
		* @param OutAxisIndex - Number representing the axis of the controller.
		* @param OutAnalogValue - The value of the analog stick.
		*/
		void GetGamepadAnalog(uint8& OutControllerIndex, uint8& OutAxisIndex, float& OutAnalogValue)
		{
			check(Event == EventType::GAMEPAD_ANALOG);
			OutControllerIndex = Data.GamepadAnalog.ControllerIndex;
			OutAxisIndex = Data.GamepadAnalog.AxisIndex;
			OutAnalogValue = Data.GamepadAnalog.AnalogValue;
		}

		/**
		* Get information about an event related to a gamepad button being pushed down.
		* @param OutControllerIndex - Number representing the index of the controller.
		* @param OutButtonIndex - Numerical code identifying the pushed down button.
		* @param OutIsRepeat - Whether the button is being kept down and is repeating.
		*/
		void GetGamepadButtonPressed(uint8& OutControllerIndex, uint8& OutButtonIndex, bool& OutIsRepeat)
		{
			check(Event == EventType::GAMEPAD_PRESS);
			OutControllerIndex = Data.GamepadButton.ControllerIndex;
			OutButtonIndex = Data.GamepadButton.ButtonIndex;
			OutIsRepeat = Data.GamepadButton.bIsRepeat;
		}

		/**
		* Get information about an event related to a gamepad button being released.
		* @param OutControllerIndex - Number representing the index of the controller.
		* @param OutButtonIndex - Numerical code identifying the pushed down button.
		*/
		void GetGamepadButtonReleased(uint8& OutControllerIndex, uint8& OutButtonIndex)
		{
			check(Event == EventType::GAMEPAD_RELEASE);
			OutControllerIndex = Data.GamepadButton.ControllerIndex;
			OutButtonIndex = Data.GamepadButton.ButtonIndex;
		}

		/**
		* Get information about an event related to a key being pushed down.
		* @param OutKeyCode - Numerical code identifying the pushed down key.
		* @param OutIsRepeat - Whether the key is being kept down and is repeating.
		*/
		void GetKeyDown(uint8& OutKeyCode, bool& OutIsRepeat)
		{
			check(Event == EventType::KEY_DOWN);
			OutKeyCode = Data.KeyDown.KeyCode;
			OutIsRepeat = Data.KeyDown.bIsRepeat;
		}

		/**
		* Get information about an event related to a key being released.
		* @param OutKeyCode - Numerical code identifying the released key.
		*/
		void GetKeyUp(uint8& OutKeyCode)
		{
			check(Event == EventType::KEY_UP);
			OutKeyCode = Data.KeyUp.KeyCode;
		}

		/**
		* Get information about an event related to character input.
		* @param OutCharacter - The character being input.
		*/
		void GetCharacterCode(TCHAR& OutCharacter)
		{
			check(Event == EventType::KEY_PRESS);
			OutCharacter = Data.Character.Character;
		}

		/**
		* Get information about an event related to mouse movement.
		* @param OutPoxX - The X position of the mouse pointer.
		* @param OutPosY - The Y position of the mouse pointer.
		* @param OutDeltaX - The change in the X position of the mouse pointer.
		* @param OutDeltaY - The change in the Y position of the mouse pointer.
		*/
		void GetMouseDelta(uint16& OutPosX, uint16& OutPosY, int16& OutDeltaX, int16& OutDeltaY)
		{
			check(Event == EventType::MOUSE_MOVE);
			OutPosX = Data.MouseMove.PosX;
			OutPosY = Data.MouseMove.PosY;
			OutDeltaX = Data.MouseMove.DeltaX;
			OutDeltaY = Data.MouseMove.DeltaY;
		}

		/**
		* Get information about an event related to mouse buttons.
		* @param OutButton - The button number corresponding to left, middle, right, etc.
		* @param OutPosX - The X position of the mouse pointer.
		* @param OutPosY - The Y position of the mouse pointer.
		*/
		void GetMouseClick(uint32 &OutButton/*EMouseButtons::Type& OutButton*/, uint16& OutPosX, uint16& OutPosY)
		{
			check(Event == EventType::MOUSE_DOWN || Event == EventType::MOUSE_UP);
			// https://developer.mozilla.org/en-US/docs/Web/Events/mousedown
			uint8 Button = Data.MouseButton.Button;
			#if defined(_MSC_VER)
			switch (Button)
			{
			case 0:
			{
				OutButton = Event ==EventType::MOUSE_DOWN  ?   WM_LBUTTONDOWN : WM_LBUTTONUP; // EMouseButtons::Left;
			}
			break;
			case 1:
			{
				OutButton = Event ==EventType::MOUSE_DOWN  ?    WM_MBUTTONDOWN : WM_MBUTTONUP;  ; // EMouseButtons::Middle;
			}
			break;
			case 2:
			{
				OutButton = Event ==EventType::MOUSE_DOWN  ?    WM_RBUTTONDOWN : WM_RBUTTONUP; // EMouseButtons::Right;
			}
			break;
			case 3:
			{
				//log error ---> 
				//OutButton = EMouseButtons::Thumb01;
			}
			break;
			case 4:
			{
				//log error ---> 
			//	OutButton = EMouseButtons::Thumb02;
			}
			break;
			default:
			{
				// log error ---> 
				//UE_LOG(PixelStreamerInputDevice, Error, TEXT("Unknown Pixel Streaming mouse click with button %d and word 0x%016llx"), Button, Data.Word);
			}
			break;
			}
			#endif //#if defined(_MSC_VER)
			OutPosX = Data.MouseButton.PosX;
			OutPosY = Data.MouseButton.PosY;
		}

		/**
		* Get information about an event related to the mouse wheel.
		* @param OutDelta - The amount by which the mouse wheel was scrolled.
		* @param PosX - The X position of the mouse pointer when the wheel was scrolled.
		* @param PosY - The Y position of the mouse pointer when the wheel was scrolled.
		*/
		void GetMouseWheel(int16& OutDelta, uint16& OutPosX, uint16& OutPosY)
		{
			check(Event == EventType::MOUSE_WHEEL);
			OutDelta = Data.MouseWheel.Delta;
			OutPosX = Data.MouseWheel.PosX;
			OutPosY = Data.MouseWheel.PosY;
		}

		/**
		* Get information about an event related to a finger touching the canvas.
		* @param OutTouchIndex - The finger used in multi-touch.
		* @param OutPoxX - The X position of the finger.
		* @param OutPosY - The Y position of the finger.
		* @param OutForce - Amount of pressure being applied by the finger.
		*/
		void GetTouch(uint8& OutTouchIndex, uint16& OutPosX, uint16& OutPosY, uint8& OutForce)
		{
			check(Event == EventType::TOUCH_START || Event == EventType::TOUCH_END || Event == EventType::TOUCH_MOVE);
			OutTouchIndex = Data.Touch.TouchIndex;
			OutPosX = Data.Touch.PosX;
			OutPosY = Data.Touch.PosY;
			OutForce = Data.Touch.Force;
		}
	};

}
#endif // _C_INPUT_DEVICE_EVENT_H_