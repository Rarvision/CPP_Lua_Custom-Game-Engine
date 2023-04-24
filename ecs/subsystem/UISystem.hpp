#pragma once
#include "System.hpp"
#include "Event.hpp"
#include "UIComponent.hpp"

using namespace std;

class UISystem : public ISystem{
public:
	UISystem() = default;
	
	void Init() override;

	void Update() override;

	void Clear() override;

	~UISystem();

	// UIButton CreateUIButton(int x, int y, int w, int h);
private:
	void OnMouseClicked_Left(const Event &event);

	void OnMouseClicked_Right(const Event &event);

	void OnKeybardClicked_O(const Event &event);

	// save mouse position
    int                 m_MouseX;
    int                 m_MouseY;
};