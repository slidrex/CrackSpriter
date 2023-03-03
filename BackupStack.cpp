#include "BackupStack.h"

Crack::BackupStack::BackupStack()
{
	lastBackupedItem = Crack::None;
	currentStackSize = 0;
	openedStack = false;

}
void Crack::BackupStack::BeginWritePixels()
{
	openedStack = true;
}
void Crack::BackupStack::WritePixel(Pixel pixel)
{
	int pixCoords[] = { pixel.position[0], pixel.position[1] };
	for(int i = 0; i < writtenPixels.size(); i++)
	{
		if (pixCoords[0] == writtenPixels[i].position[0] && pixCoords[1] == writtenPixels[i].position[1]) return;
	}
	writtenPixels.push_back(pixel);
}
void Crack::BackupStack::EndWritePixels()
{
	if (!openedStack) return;
	int writtenPixelsLength = writtenPixels.size();
	Pixel* _pixelStack = new Pixel[writtenPixelsLength];

	for(int i = 0; i < writtenPixelsLength; i++)
	{
		Pixel currentPixel = writtenPixels.at(i);
		_pixelStack[i] = Pixel(glm::vec2(currentPixel.position[0], currentPixel.position[1]), glm::vec4(currentPixel.color[0], currentPixel.color[1], currentPixel.color[2], 1.0f));
	}
	if (currentStackSize < MaxBackupCapacity)
		currentStackSize++;
	else pixelStack.erase(pixelStack.begin());
	WrittenPixelsHeap heap = WrittenPixelsHeap();
	heap.writtenPixelsCount = writtenPixels.size();
	heap.pixels = _pixelStack;
	pixelStack.push_back(heap);
	writtenPixels.clear();
	openedStack = false;
}
Crack::WrittenPixelsHeap Crack::BackupStack::GetLastBackupedPixels()
{
	WrittenPixelsHeap pixel = pixelStack[pixelStack.size() - 1];
	pixelStack.erase(pixelStack.end() - 1);
	currentStackSize--;
    return pixel;
}

void Crack::BackupStack::ClearBackupStack()
{
	pixelStack.clear();
	canvasStack.clear();
	writtenPixels.clear();
	currentStackSize = 0;
}
