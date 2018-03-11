#pragma once

#include "resource.h"

void PaintWindow(const HWND hWnd);
void PaintMandelbrot(
	const HDC hDc,
	const int width,
	const int height,
	const double zoom,
	const double offsetX,
	const double offsetY);

void PaintMandelbrotParallel(
	const HDC hDc,
	const int width,
	const int height,
	const double zoom,
	const double offsetX,
	const double offsetY);

int* CalculatePixel(
	const int imgX,
	const int imgY,
	const int imageWidth,
	const int imageHeight,
	const double zoom,
	const double offsetX,
	const double offsetY);

void Trace(char* message);
void Trace(char* message, double var);