#pragma once

// Self
#include "Condition.h"

// C++
#include <exception>
#include <memory>
#include <mutex>
#include <vector>

namespace nsGame
{
	class BallGame
	{
	public:
		
		/**
		 * \brief ����������� �� ���������
		 */
		explicit BallGame() = default;
		
		/**
		 * \brief ���������� �� ���������
		 */
		~BallGame() = default;
		
		/**
		 * \brief ������ ������ �������
		 */
		void Start();
	private:
		
		/**
		 * \brief �������� �������������� ������ �� �����
		 */
		void Initialize();

		/**
		 * \brief ������ ������ ������� � ������� ����������
		 */
		void StartWithExceptions();

		std::vector<std::shared_ptr<nsCondition::Condition>> _cases; ///< ���������� ����������� ���������
		std::vector<std::exception_ptr> _exceptions; ///< ����� ���������� 

		std::mutex _mtx; ///< ������� ��� ������������� ��������� � �������
	};
}