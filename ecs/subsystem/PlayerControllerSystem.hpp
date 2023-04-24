#pragma once

#include "System.hpp"
#include "Event.hpp"
#include "Player.hpp"

class PlayerControllerSystem : public ISystem
{
public:
	void Init() override;

	void Update() override;

	void Clear() override;

	Player CreatePlayer();


private:
	void OnKeyboardClicked_A(const Event &event);

	void OnKeyboardClicked_D(const Event &event);

	/// @brief 
	/// @param direction -1 means left, 1 means right
	/// @param boundaryOffSet 
	void MovePlayer(int direction, int boundaryOffSet);

	void DispatchPlayerMovePosition(int x, int y);

	Player	m_Player;

	int 	m_TilemapMaxX;
};
