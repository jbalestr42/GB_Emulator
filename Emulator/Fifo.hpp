#pragma once

#include <stack>

class IDisplay {};

class Fifo
{
public:
	struct Pixel
	{
		uint8_t color; // Value between 0 and 3
		// Palette only for object
		// Background Priority: holds the value of the OBJ-to-BG Priority bit

		Pixel(uint8_t color) :
			color(color)
		{}
	};

	Fifo() = default;
	~Fifo() = default;

	bool isEmpty() const { return _fifo.empty(); }
	size_t size() const { return _fifo.size(); }
	Fifo::Pixel pop();
	void push(Fifo::Pixel pixel);
	void push(Fifo::Pixel pixels[]);
	//Fifo(IDisplay& display);

private:
	//IDisplay& _display;
	std::stack<Fifo::Pixel> _fifo;

};