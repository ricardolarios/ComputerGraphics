#pragma once

// Vertex Texture struct for compact vbo usage.
struct VertexData
{
	float x;
	float y;
	float z;
	float s;
	float t;

	bool operator==(const VertexData& v)
	{
		return (x == v.x) && (y == v.y) && (z == v.z) && (s == v.s) && (t == v.t);
	}
};

