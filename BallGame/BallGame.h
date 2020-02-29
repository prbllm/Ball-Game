#pragma once

// Self
#include "Condition.h"

// C++
#include <queue>
#include <memory>

namespace nsGame
{
	class BallGame
	{
		using queue = std::queue<std::unique_ptr<nsCondition::Condition>>;
	public:
		
		/**
		 * \brief ����������� �� ���������
		 */
		explicit BallGame();
		
		/**
		 * \brief ���������� �� ���������
		 */
		~BallGame();
		
		/**
		 * \brief ������ ������ �������
		 */
		void Start();
	private:
		
		/**
		 * \brief �������� �������������� ������ �� �����
		 */
		void Initialize();

		queue cases; // ���������� ����������� ���������
	};
}