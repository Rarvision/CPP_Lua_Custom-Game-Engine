#pragma once

#include "System.hpp"
#include "Event.hpp"

class ControllerSystem : public ISystem
{
public:
	void Init() override;

	void Update() override;

private:
	void OnKeyboardClicked_A(const Event &event);

	void OnKeyboardClicked_D(const Event &event);

	void OnKeyboardClicked_W(const Event &event);

	void OnKeyboardClicked_S(const Event &event);

	void OnKeyboardClicked_SPACE(const Event &event);

	Player	m_Player;
};
