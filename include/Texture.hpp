#pragma once

class Texture
{
public:
	Texture() = default;
	Texture(const Texture&) = delete;
	Texture(const char*);

    void Bind(unsigned int = 0x84C0);
	static unsigned int LoadTexture(const char*, unsigned int, int, unsigned int);

private:
	friend class Model;
	unsigned int m_ID;

	void GiveID(unsigned int);
};
