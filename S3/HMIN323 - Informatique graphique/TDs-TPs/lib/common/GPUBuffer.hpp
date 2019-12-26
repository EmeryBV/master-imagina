#ifndef GPU_BUFFER 
#define GPU_BUFFER 

#include <GL/glew.h>
#include <tuple>

class GPUBuffer
{
public:
    GPUBuffer();
    ~GPUBuffer();
    
    unsigned int id();

    virtual void bind() = delete;
    virtual void assign(size_t data_byte_size, const void* data) = delete;


private:
    unsigned int m_id;
};

template<GLenum BUFFER_TYPE, GLenum DRAWING_METHOD = GL_STATIC_DRAW>
class GPUBufferHelper : public GPUBuffer
{
public:
	using GPUBuffer::GPUBuffer;

    GPUBufferHelper(size_t data_byte_size, const void* data);
    
    void bind() override;
    void assign(size_t data_byte_size, const void* data) override;
};

template<GLenum DRAWING_METHOD = GL_STATIC_DRAW>
using GPUArray = GPUBufferHelper<GL_ARRAY_BUFFER, DRAWING_METHOD>;

template<GLenum DRAWING_METHOD = GL_STATIC_DRAW>
using GPUIndexArray = GPUBufferHelper<GL_ELEMENT_ARRAY_BUFFER, DRAWING_METHOD>;


template<unsigned int VERTEX_SIZE, typename VERTEX_VALUE_TYPE, bool NORMALIZED = false, size_t ATTRIBUTE_GAP = 0, size_t ATTRIBUTE_OFFSET = 0>
struct GPUBufferAttributeLayout;


const char* data = "bonjour je suis une chaine\n";

GPUBuffer* buf = new typename GPUArray(10, data);

#endif
