#include "Input.hpp"
#include "HardwareRegisters.hpp"
#include "MMU.hpp"
#include "BitUtils.hpp"
#include <iostream>

Input::Input(MMU& mmu) :
	_mmu(mmu),
	_joyReg(0)
{
	std::memset(_inputsState, 0, Input::InputCount);
	_inputs[0] = sf::Keyboard::Scan::A;
	_inputs[1] = sf::Keyboard::Scan::B;
	_inputs[2] = sf::Keyboard::Scan::E;
	_inputs[3] = sf::Keyboard::Scan::S;
	_inputs[4] = sf::Keyboard::Scan::Right;
	_inputs[5] = sf::Keyboard::Scan::Left;
	_inputs[6] = sf::Keyboard::Scan::Up;
	_inputs[7] = sf::Keyboard::Scan::Down;
}

std::uint8_t Input::getValueFromInput(sf::Keyboard::Scan::Scancode scanCode)
{
	for (std::uint8_t i = 0; i < Input::InputCount; i++)
	{
		if (scanCode == _inputs[i])
		{
			return i;
		}
	}
	return 0;
}

void Input::update()
{
	for (size_t i = 0; i < Input::InputCount; i++)
	{
		_inputsState[i] = sf::Keyboard::isKeyPressed(_inputs[i]);
	}
}

uint8_t Input::read8()
{
	// Directions
	if (BitUtils::GetBit(_joyReg, 4) == 0)
	{
		_joyReg = BitUtils::SetBit(_joyReg, 3, _inputsState[getValueFromInput(sf::Keyboard::Scan::Down)]);
		_joyReg = BitUtils::SetBit(_joyReg, 2, _inputsState[getValueFromInput(sf::Keyboard::Scan::Up)]);
		_joyReg = BitUtils::SetBit(_joyReg, 1, _inputsState[getValueFromInput(sf::Keyboard::Scan::Left)]);
		_joyReg = BitUtils::SetBit(_joyReg, 0, _inputsState[getValueFromInput(sf::Keyboard::Scan::Right)]);
	}

	// Actions
	if (BitUtils::GetBit(_joyReg, 5) == 0)
	{
		_joyReg = BitUtils::SetBit(_joyReg, 3, _inputsState[getValueFromInput(sf::Keyboard::Scan::S)]);
		_joyReg = BitUtils::SetBit(_joyReg, 2, _inputsState[getValueFromInput(sf::Keyboard::Scan::E)]);
		_joyReg = BitUtils::SetBit(_joyReg, 1, _inputsState[getValueFromInput(sf::Keyboard::Scan::B)]);
		_joyReg = BitUtils::SetBit(_joyReg, 0, _inputsState[getValueFromInput(sf::Keyboard::Scan::A)]);
	}
	return _joyReg;
}

void Input::write8(uint8_t v)
{
	_joyReg = v | 0b11000000;
}