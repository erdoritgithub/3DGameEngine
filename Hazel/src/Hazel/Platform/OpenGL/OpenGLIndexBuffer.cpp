#include "hzpch.h"
#include "OpenGLIndexBuffer.h"
#include <Glad/glad.h>
namespace Hazel {
	OpenGLIndexBuffer::OpenGLIndexBuffer(unsigned int size)
		: m_RendererID(0), m_Size(size)
	{
		HZ_RENDER_S({
			glGenBuffers(1, &self->m_RendererID);
			});
	}
	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		HZ_RENDER_S({
			glDeleteBuffers(1, &self->m_RendererID);
			});
	}
	void OpenGLIndexBuffer::SetData(void* buffer, unsigned int size, unsigned int offset)
	{
		HZ_RENDER_S3(buffer, size, offset, {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->m_RendererID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, buffer, GL_STATIC_DRAW);
			});
	}
	void OpenGLIndexBuffer::Bind() const
	{
		HZ_RENDER_S({
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->m_RendererID);
			});
	}
}