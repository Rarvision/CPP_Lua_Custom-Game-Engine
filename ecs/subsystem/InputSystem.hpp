#pragma once

#include "System.hpp"
#include "Event.hpp"
#include "InPut.hpp"

class InputSystem : public ISystem
{
public:
	void Init() override;

	void Update() override;

	void Clear() override;

private:
	//////  event trigger /////
	void DispatchMousePosition_LeftClick(int mouseX, int mouseY);

	void DispatchMouseRightClickEvent_RightClick(int mouseX, int mouseY);

	void DispatchKeyboardEvent_W();

	void DispatchKeyboardEvent_A();

	void DispatchKeyboardEvent_S();

	void DispatchKeyboardEvent_D();

	void DispatchKeyboardEvent_O();

	void DispatchKeyboardEvent_I();

	void DispatchKeyboardEvent_U();

	void DispatchKeyboardEvent_SPACE();

	void HandleEditorState(const Entity& entity, Uint32 mouseState, const Uint8 *keyboard_State);

	void HandleGameState(const Entity& entity, Uint32 mouseState, const Uint8 *keyboard_State);

	int m_MouseX;

	int m_MouseY;
};