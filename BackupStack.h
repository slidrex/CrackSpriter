#pragma once
#include "Canvas.h"
#include <vector>

namespace Crack
{
	enum BackupItem
	{
		None,
		CanvasItem,
		PixelArray
	};
	struct WrittenPixelsHeap
	{
		int writtenPixelsCount;
		Pixel* pixels;
	};
	class BackupStack 
	{
	public:
		BackupStack();
		void BeginWritePixels();
		void WritePixel(Pixel pixel);
		void EndWritePixels();
		WrittenPixelsHeap GetLastBackupedPixels();
		BackupItem GetLastBackupedItem() { return lastBackupedItem; }
		int GetBackupPixelsStackCount() { return pixelStack.size(); }
		void ClearBackupStack();
	private:
		std::vector<Pixel> writtenPixels;
		BackupItem lastBackupedItem;
		bool openedStack;
		std::vector<Canvas> canvasStack;
		std::vector<WrittenPixelsHeap> pixelStack;
		int currentStackSize;
		const unsigned int MaxBackupCapacity = 50;
	};
}
