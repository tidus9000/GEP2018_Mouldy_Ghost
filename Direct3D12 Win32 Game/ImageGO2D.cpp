#include "pch.h"
#include "ImageGO2D.h"
#include <codecvt>
#include "RenderData.h"

ImageGO2D::ImageGO2D(RenderData * _RD, string _filename, Vector2 _spritesize, int _in_row)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	string fullpath = "../DDS/" + _filename + ".dds";
	std::wstring wFilename = converter.from_bytes(fullpath.c_str());

	ResourceUploadBatch resourceUpload(_RD->m_d3dDevice.Get());

	resourceUpload.Begin();

	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(_RD->m_d3dDevice.Get(), resourceUpload, wFilename.c_str(),
			m_texture.ReleaseAndGetAddressOf()));


	CreateShaderResourceView(_RD->m_d3dDevice.Get(), m_texture.Get(),
		_RD->m_resourceDescriptors->GetCpuHandle(m_resourceNum = _RD->m_resourceCount++));

	auto uploadResourcesFinished = resourceUpload.End(_RD->m_commandQueue.Get());

	uploadResourcesFinished.wait();

	if (_spritesize.x == 0)
	{
		m_spriteSize = Vector2(GetTextureSize
		(m_texture.Get()).x, GetTextureSize(m_texture.Get()).y);
	}
	else
	{
		m_spriteSize = _spritesize;
	}

	SetOrigin(m_spriteSize / 2);
}


ImageGO2D::~ImageGO2D()
{
	m_texture.Reset();
}

void ImageGO2D::Render(RenderData * _RD, int _sprite, Vector2 _cam_pos)
{
	Rectangle rect = Rectangle(0, 0, m_spriteSize.x, m_spriteSize.y);
	const RECT* r = &RECT(rect);

	_RD->m_spriteBatch->Draw(_RD->m_resourceDescriptors->GetGpuHandle(m_resourceNum),
		GetTextureSize(m_texture.Get()),
		m_pos + _cam_pos, r, m_colour, m_orientation, m_origin, m_scale);
}

void ImageGO2D::scaleFromPoint(Vector2 point, Vector2 scale)
{
	//make sure origin is in the middle
	CentreOrigin();

	//get the size of the texture
	XMUINT2 size = GetTextureSize(m_texture.Get());
	
	//get the current scale of the texture
	//this gives us the real size of the texture (width and height) as a vector2
	Vector2 realSize((size.x * m_scale.x), (size.y * m_scale.y));
	//obtain the real size of the texture after scaling
	Vector2 realSizeAfterScale((size.x * scale.x), (size.y * scale.y));
	//get the difference in size between these two
	Vector2 diff = realSizeAfterScale - realSize;
	//make the difference positive
	diff.x = abs(diff.x);
	diff.y = abs(diff.y);
	//get the distance of the scale point to the origin of the image as a percentage (1.0f = 100%)
	Vector2 distanceFactor((point.x - GetPos().x) / ((realSize.x / 2) - GetPos().x), ((point.y - GetPos().y) / ((realSize.y / 2) - GetPos().y)));
	//multiply the difference in size by this percentage
	diff = diff * distanceFactor;
	//now we have the values, we scale the image
	SetScale(scale);
	//this is how much we should move the image by
	SetPos(Vector2(GetPos().x + diff.x, GetPos().y + diff.y));
	//if point was bottom left we would do: current_pos.x + difference in width, current_pos.y + difference in height
}

void ImageGO2D::CentreOrigin()
{
	XMUINT2 size = GetTextureSize(m_texture.Get());
	
	m_origin = Vector2(float(size.x / 2), float(size.y / 2));
}

Vector2 ImageGO2D::TextureSize()
{
	if (m_spriteSize.x == 0)
	{
		XMUINT2 size = GetTextureSize(m_texture.Get());
		return Vector2(size.x, size.y);
	}
	else
	{
		return m_spriteSize;
	}
}

void ImageGO2D::FlipX()
{
	//this doesn't do anything, flipping textures is harder than I expected
	flipped = !flipped;

	m_orientation += 3.14159;
}
