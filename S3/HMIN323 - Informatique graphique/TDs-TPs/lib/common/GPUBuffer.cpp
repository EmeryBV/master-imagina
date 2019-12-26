#include "GPUBuffer.hpp"

//////////////////////// GPUBuffer

GPUBuffer::GPUBuffer()
{
	glGenBuffers(1, &m_id);
}

GPUBuffer::~GPUBuffer()
{
	glDeleteBuffers(1, &m_id);
}
    
unsigned int GPUBuffer::id()
{
	return m_id;
}

//////////////////////// GPUBufferHelper

template<GLenum BUFFER_TYPE, GLenum DRAWING_METHOD>
GPUBufferHelper<BUFFER_TYPE, DRAWING_METHOD>::GPUBufferHelper(size_t data_byte_size, const void* data) : GPUBuffer() // taille
{
	assign(data_byte_size, data);
}

template<GLenum BUFFER_TYPE, GLenum DRAWING_METHOD>
void GPUBufferHelper<BUFFER_TYPE, DRAWING_METHOD>::bind()
{
	glBindBuffer(BUFFER_TYPE, id());
}

template<GLenum BUFFER_TYPE, GLenum DRAWING_METHOD>
void GPUBufferHelper<BUFFER_TYPE, DRAWING_METHOD>::assign(size_t data_byte_size, const void* data)
{
    glNamedBufferData(id(), data_byte_size, data, DRAWING_METHOD);
}
