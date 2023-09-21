#include "Fifo.hpp"

/*Fifo::Fifo(IDisplay& display) :
	_display(display)
{

}*/

Fifo::Pixel Fifo::pop()
{
	Pixel pixel = _fifo.top();
	_fifo.pop();
	return pixel;
}

void Fifo::push(Fifo::Pixel pixel)
{
	_fifo.push(pixel);
}

void Fifo::push(Fifo::Pixel pixels[])
{
	for (size_t i = 0; i < 8; i++)
	{
		_fifo.push(pixels[i]);
	}
}